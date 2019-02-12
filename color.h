#pragma once

namespace colors {
	constexpr SDL_Color bg = { 3, 17, 22, 255 };
	constexpr SDL_Color bg2 = { 5, 37, 30, 255 };
	constexpr SDL_Color fg = { 255, 255, 255, 255 };
	constexpr SDL_Color fg2 = { 220, 230, 238, 255 };
	constexpr SDL_Color fg3 = { 220, 255, 230, 255 };
	constexpr SDL_Color active = { 160, 125, 136, 255 };

	constexpr SDL_Color bad = { 220, 100, 100, 255 };
	constexpr SDL_Color connecting = { 255, 200, 200, 255 };
	constexpr SDL_Color good = { 0, 255, 255, 255 };

	SDL_Color get_rainbow(u32 i);
	void enlighten(SDL_Color *c, i32 d);
}
