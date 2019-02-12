#include "main.h"
#include "color.h"

#define MIN(a, b) ( (a) < (b) ? (a) : (b) )
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )

namespace colors {
	SDL_Color get_rainbow(u32 i) {
		SDL_Color c;
		u32 z = i / 256;
		u8 x = i % 256;
		u8 y = 255 - x;
		if (z == 0) c = { 255, x, 0 };
		else if (z == 1) c = { y, 255, 0 };
		else if (z == 2) c = { 0, 255, x };
		else if (z == 3) c = { 0, y, 255 };
		else if (z == 4) c = { x, 0, 255 };
		else if (z == 5) c = { 255, 0, y };
		return c;
	}

	void enlighten(SDL_Color *c, i32 d) {
		if (d > 0) {
			c->r = MIN(c->r + d, 255);
			c->g = MIN(c->g + d, 255);
			c->b = MIN(c->b + d, 255);
		} else if (d < 0) {
			c->r = MAX(c->r + d, 0);
			c->g = MAX(c->g + d, 0);
			c->b = MAX(c->b + d, 0);
		}
	}
}
