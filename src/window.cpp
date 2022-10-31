#include "window.h"
#include <iostream>

Xwindow EWindow::create_window_and_gc(Display* di, int w, int h, int x, int y, long event_mask) {
    Xwindow win;
    int screen_num = DefaultScreen(di);
    int win_border_width = 2;
    
    win.display = di;

    win.rootWindow = RootWindow(di, screen_num);
    win.depth = DefaultDepth(di, screen_num);
    win.visual = DefaultVisual(di, screen_num);
    win.attributes.background_pixel = WhitePixel(di, screen_num);
    win.attributes.border_pixel = XBlackPixel(di, screen_num);
    win.attributes.override_redirect = 0;
    win.attributes.event_mask = event_mask;

    win.window = XCreateWindow(di, win.rootWindow, x, y,
        w, h, 0, win.depth, InputOutput, win.visual, CWBorderPixel | CWColormap | CWEventMask,
        &win.attributes);

    GC gc;
    unsigned long valuemask = 0;
    XGCValues values;
    unsigned int line_width = 2;
    int line_style = LineSolid;
    int cap_style = CapButt;
    int join_style = JoinBevel;

    gc = XCreateGC(di, win.window, valuemask, &values);

    XSetLineAttributes(di, gc, line_width, line_style, cap_style, join_style);
    XSetFillStyle(di, gc, FillSolid);

    win.gc = gc;

    return win;
}
int EWindow::clear_window(Xwindow win) {
    return XClearWindow(win.display, win.window);
}