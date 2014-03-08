#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

int main() {
	Display *dis;
    dis = XOpenDisplay(NULL);
	if (!dis) {fprintf(stderr, "unable to connect to display\n");return 7;}
	Window win;
    //win = XCreateSimpleWindow(dis, RootWindow(dis, 0), 1, 1, 500, 500, \
    //0, BlackPixel (dis, 0), BlackPixel(dis, 0));
    XSetWindowAttributes at;
    at.event_mask = KeyPressMask;
    win = XCreateWindow(
        dis,
        RootWindow(dis, 0),
        10,10,200,200,
        0,
        CopyFromParent,
        InputOnly,
        0,
        0,
        &at
    );    
	/* tell the display server what kind of events we would like to see */
	XSelectInput(dis, win, ButtonPressMask|StructureNotifyMask|KeyPressMask|KeyReleaseMask|KeymapStateMask);

    XMapWindow(dis, win);
    XFlush(dis);
    XEvent report;
    while (1) {
        XNextEvent(dis, &report);
        switch  (report.type) {
            case KeyPress:
                printf("A key was pressed.\n");
                if (XLookupKeysym(&report.xkey, 0) == XK_space)  {
                    fprintf (stdout, "The space bar was pressed.\n");
                }
            //case Expose:   
            //    fprintf(stdout, "I have been exposed.\n");
            /*A local program function to redraw the window should be called.*/
            default: 
                break;
        }
    }
    /*Sleep 5 seconds before closing.*/
    // sleep(5);
    return(0);
}

