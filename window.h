#pragma once

struct BarWindow {
	Display *dis;
	Window root, xwin;
	SDL_Window *win;
	SDL_Renderer *ren;
	i32 black;
	u32 xc, yc, w, h;
	u32 delay;
	Client *client;
	bool running;

	struct {
		Atom window_type,
			 dock,
			 strut_partial;
	} atoms;

	void init();
	void init_atoms();
	void open();
};
