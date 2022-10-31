#pragma once
#define LINUX_BUILD
#ifdef LINUX_BUILD
#include <X11/Xlib.h>
#elif WINDOWS_BUILD
#include <Windows.h>
#else
#error Undefined build type. Specify: LINUX_BUILD or WINDOWS_BUILD
#endif

typedef struct _Xwindow {
    Display* display;
    Window rootWindow;
    Window window;
    GC gc;
} Xwindow;

class EWindow {
public:
    static Xwindow create_window_and_gc(
        Display* display, int width, int height, int x, int y
        );
    static GC create_gc(Display* display, Xwindow win, int reverse_video);
    static int clear_window(Xwindow window);
};
