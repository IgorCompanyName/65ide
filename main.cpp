#include "mos6502.h"
#include "window.h"
#include "event.h"
#include "shader.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <sstream>
#include <time.h>
#include <fstream>
#include <ios>
#include <thread>
#include <chrono>
#include <vector>

/*
 * Memory mapping for 6502 IDE
 * RAM[0x0000 until 0x6FFE inc]    -rw
 * RAND[0x6FFF only]               -ro
 * SCREEN[0x7000 until 0x7FFF inc] -wo
 * ROM[0x8000 until 0xFFFF inc]    -ro {
 *      INSTRUCTION_ROM[0x8000 until 0xFF99 inc]
 *      NMI[0xFF9A until 0xFF99 inc]
 *      RESET[0xFFBA until 0xFFB9 inc]
 *      IRQ/BRK[0xFFDA until 0xFFF9 inc]
 *      
 *      VECTORS:
 *          NMI     0xFFFA + 0xFFFB
 *          RESET   0xFFFC + 0xFFFD
 *          IRQ     0xFFFE + 0xFFFF
 * }
 */
#define RAM_OFFSET 0
#define RAM_SIZE 0x6FFF
#define RAND_OFFSET 0x6FFF
#define SCREEN_OFFSET 0x7000
#define ROM_OFFSET 0x8000
#define ROM_SIZE 0x8000
#define CLEAR_SCREEN_OFFSET 0xFD01

#define _RGB(r, g, b) b + (g << 8) + (r << 16)

#define K64 65536
#define K32 32768
#define K16 16384
#define K8 8192
#define K4 4096
#define K2 2048
#define K1 1024

#define WIDTH 64
#define HEIGHT 64
#define SIZE_MULTIPLIER 8
#define BUFFER_LENGTH 0x1000

#define ORGNAME "migstudio"
#define APPGROUP "65ide"
#define APPNAME "emu"

uint8_t* ram;
uint8_t* rom;
uint8_t* pixelbuffer;

GLuint vbo, vao;

int stopped = 0, paused = 0, canPause = 1;

const float vertices[] = {
    -1, -1, -1, 0, 1,
    1, -1, -1, 1, 1,
    -1, 1, -1, 0, 0,

    -1, 1, -1, 0, 0,
    1, -1, -1, 1, 1,
    1, 1, -1, 1, 0
};
void drawFrame();
void clearFrame();

uint8_t readFunc(uint16_t addr) { // Explicitly specifying memory addressing
    if(addr >= RAM_OFFSET && addr <= RAM_OFFSET + RAM_SIZE - 1)
        return ram[addr + RAM_OFFSET];
    else if(addr >= ROM_OFFSET && addr <= ROM_OFFSET + ROM_SIZE - 1)
        return rom[addr - ROM_OFFSET];
    else if(addr == RAND_OFFSET)
        return rand() % 256;
    return 0x00;
}
void writeFunc(uint16_t addr, uint8_t data) {
    if(addr >= RAM_OFFSET && addr <= RAM_OFFSET + RAM_SIZE - 1)
        ram[addr] = data;
    else if(addr >= SCREEN_OFFSET && addr <= SCREEN_OFFSET + BUFFER_LENGTH - 1)
        pixelbuffer[addr - SCREEN_OFFSET] = data;
    else if(addr == RAND_OFFSET)
        srand(data);
    else if(addr == CLEAR_SCREEN_OFFSET)
        clearFrame();
}
void writethrough(uint16_t addr, uint8_t data) {
    if(addr >= RAM_OFFSET && addr <= RAM_OFFSET + RAM_SIZE - 1)
        ram[addr] = data;
    else if(addr >= SCREEN_OFFSET && addr <= SCREEN_OFFSET + BUFFER_LENGTH - 1)
        pixelbuffer[addr - SCREEN_OFFSET] = data;
    else if(addr >= ROM_OFFSET && addr <= ROM_OFFSET + ROM_SIZE - 1)
        rom[addr - ROM_OFFSET] = data;
}
static std::string getAppId() {
    std::stringstream ss;

    ss << ORGNAME << '.';
    ss << APPGROUP << '.';
    ss << APPNAME;

    return ss.str();
}
void clearFrame() {
    for(unsigned int i = 0; i < BUFFER_LENGTH; i++) {
        pixelbuffer[i] = 0;
    }
}
void run_cpu(mos6502 cpu, uint64_t &cycles) {
    while(!stopped) {
        if(!paused)
            cpu.Run(1, cycles, mos6502::CycleMethod::INST_COUNT);
        for(int i = 0; i < 100; i++) {}
    }
}
std::vector<uint8_t> loadProgram(const char* path) {
    using namespace std;
    ifstream file;
    file.open(path, ios::binary | ios::in);
    if(!file)
    {
        std::cerr << "Failed to load file!" << std::endl;
        return vector<uint8_t>();
    }
    vector<uint8_t> bytes;
    while(!file.eof()) {
        uint8_t byte;

        byte = file.get();

        bytes.push_back(byte);
    }
    file.close();
    return bytes;
}
int main(int argc, char** argv) {
    ram = (uint8_t*)malloc(RAM_SIZE);
    rom = (uint8_t*)malloc(ROM_SIZE);
    pixelbuffer = (uint8_t*)malloc(BUFFER_LENGTH); // allocate screen

    std::string appid = getAppId();
    std::vector<uint8_t> programBytes;
    if(argc == 2) {
        programBytes = loadProgram(argv[1]);
    } else {
        programBytes = loadProgram("/home/igor/Documents/Projects/65IDE/test");
    }
    int size = programBytes.size() * sizeof(uint8_t) - 1;

    if(size != 65536)
    {
        std::cerr << "File size is wrong!" << std::endl;
        return -1;
    }

    for(int i = 0; i < size; i++) {
        writethrough(i, programBytes[i]);
    }

    const int width = WIDTH * SIZE_MULTIPLIER, 
        height = HEIGHT * SIZE_MULTIPLIER;
    Window window(width, height, appid.c_str());
    Events events(window.window);

    std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\nOpenGL " <<
        glGetString(GL_VERSION) << "\n\n";

    srand(time(NULL));

    for(unsigned int i = 0; i < BUFFER_LENGTH; i++) {
        pixelbuffer[i] = 0;
    }

    Shader shader("/home/igor/Documents/Projects/65IDE/res/vertex.glsl", 
        "/home/igor/Documents/Projects/65IDE/res/fragment.glsl");

    glClearColor(1, 0, 1, 0);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R3_G3_B2, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE_3_3_2, pixelbuffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    shader.uniform2("tex_size", WIDTH, HEIGHT);

    mos6502 cpu(readFunc, writeFunc);
    uint64_t cycles;

    cpu.Reset();

    std::thread cpu_thread(run_cpu, cpu, std::ref(cycles));

    while(!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R3_G3_B2, WIDTH, HEIGHT, 0, GL_RGB, 
        GL_UNSIGNED_BYTE_3_3_2, pixelbuffer);

        shader.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        if(events.getKeyPress(GLFW_KEY_ESCAPE))
            window.setShouldClose(true);
        if(events.getKeyPress(GLFW_KEY_SPACE)) {
            if(canPause != 0) {
                paused = !paused;
                canPause = 0;
            }
        } else {
            canPause = 1;
        }
        window.swapBuffers();
        events.pollEvents();
    }
    stopped = 1;

    cpu_thread.join();

    std::cout << "Closed window. End.\nQuit after " << cycles << " cycles" << std::endl;
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &tex);

    return 0;
}
