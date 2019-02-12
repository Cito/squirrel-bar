#include "main.h"
#include "window.h"

void BarWindow::init() {
	dis = XOpenDisplay(0);
	root = DefaultRootWindow(dis);
	black = BlackPixel(dis, DefaultScreen(dis));
	init_atoms();
}

void BarWindow::init_atoms() {
	atoms.window_type = XInternAtom(dis, "_NET_WM_WINDOW_TYPE", False);
	atoms.dock = XInternAtom(dis, "_NET_WM_WINDOW_TYPE_DOCK", False);
	atoms.strut_partial = XInternAtom(dis, "_NET_WM_STRUT_PARTIAL", False);
}

void BarWindow::open() {
	running = true;

	xwin = XCreateSimpleWindow(dis, root, xc, yc, w, h, 0, black, black);

	XChangeProperty(dis, xwin, atoms.window_type, XA_ATOM, 32, PropModeReplace, (u8*)&atoms.dock, 1);

	XSelectInput(dis, xwin, StructureNotifyMask);
	XMapWindow(dis, xwin);
	for (XEvent e;;) {
		XNextEvent(dis, &e);
		if (e.type == MapNotify) break;
	}

	SDL_Init(SDL_INIT_VIDEO);
	init_fonts();

	win = SDL_CreateWindowFrom((void*)xwin);

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);

	Renderer render = { win, ren, w, h, client };

	for (u64 frame = 0; running; frame++) {

		render.render(frame);

		SDL_Delay(delay);
	}
}
