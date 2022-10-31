# 65IDE

Simple IDE for development on mos6502

## Usage

For now there is no concrete usage but soon it will be updated.

## Build

### Ubuntu or Debian

To build this project you will need gcc toolchain and cmake installed

Installing dependencies (mainly libglfw3-dev):

```bash
sudo apt-get install libglfw3-dev -y
sudo apt-get install libglu1-mesa libxi-dev libxmu-dev libglu1-mesa-dev -y
```

To build:

```bash
# Configure CMake
# Change <working directory> to directory of a project and
# change <build type> to RELEASE or DEBUG, etc.
cmake -B <working directory>/build -DCMAKE_BUILD_TYPE=<build type>

# Build program with given configuration
# Change <working directory> and <build type> as before
# Change <number of threads> to number of threads on your cpu for faster compilation
cmake --build <working directory>/build --config <build type> --target main -j <number of threads>
```

Your executable will be in \<working directory\>/build/main by default.
