#include "main.h"

i32 main(i32 argc, cstr argv[]) {

	Client client;
	client.init();
	client.run();

	BarWindow win;
	win.xc = win.yc = 0;
	win.w = 1920;
	win.h = 21;
	win.delay = 120;
	win.client = &client;
	win.init();
	win.open();

	return 0;
}
