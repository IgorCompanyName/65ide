#include "window.h"
#include <iostream>

Xwindow EWindow::create_window_and_gc(Display* di, int w, int h, int x, int y) {
    Xwindow win;
    int screen_num = DefaultScreen(di);
    int win_border_width = 2;
    
    win.display = di;

    win.rootWindow = RootWindow(di, screen_num);
    win.window = XCreateSimpleWindow(di, win.rootWindow, x, y,
        w, h, win_border_width,
        BlackPixel(di, screen_num),
        WhitePixel(di, screen_num));
    XMapWindow(di, win.window);
    XFlush(di);

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