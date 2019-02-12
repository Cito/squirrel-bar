#include "main.h"
#include "font.h"

namespace font {
	const SDL_Rect _0[] = {
		{ 0, 0, 3, 1 },
		{ 0, 4, 3, 1 },
		{ 0, 1, 1, 3 },
		{ 2, 1, 1, 3 },
	};
	const SDL_Rect _1[] = {
		{ 1, 0, 1, 5 },
		{ 0, 1, 1, 1 },
		{ 0, 4, 3, 1 },
	};
	const SDL_Rect _2[] = {
		{ 0, 0, 3, 1 },
		{ 0, 2, 3, 1 },
		{ 0, 4, 3, 1 },
		{ 2, 1, 1, 1 },
		{ 0, 3, 1, 1 },
	};
	const SDL_Rect _3[] = {
		{ 0, 0, 3, 1 },
		{ 0, 4, 3, 1 },
		{ 2, 1, 1, 3 },
		{ 1, 2, 1, 1 },
	};
	const SDL_Rect _4[] = {
		{ 0, 0, 1, 3 },
		{ 2, 1, 1, 4 },
		{ 1, 2, 1, 1 },
	};
	const SDL_Rect _5[] = {
		{ 0, 0, 3, 1 },
		{ 0, 2, 3, 1 },
		{ 0, 4, 3, 1 },
		{ 0, 1, 1, 1 },
		{ 2, 3, 1, 1 },
	};
	const SDL_Rect _6[] = {
		{ 0, 0, 3, 1 },
		{ 0, 2, 3, 1 },
		{ 0, 4, 3, 1 },
		{ 0, 1, 1, 1 },
		{ 2, 3, 1, 1 },
		{ 0, 3, 1, 1 },
	};
	const SDL_Rect _7[] = {
		{ 0, 0, 3, 1 },
		{ 0, 1, 1, 1 },
		{ 2, 1, 1, 4 },
		{ 1, 3, 1, 1 },
	};
	const SDL_Rect _8[] = {
		{ 0, 0, 1, 5 },
		{ 2, 0, 1, 5 },
		{ 1, 0, 1, 1 },
		{ 1, 2, 1, 1 },
		{ 1, 4, 1, 1 },
	};
	const SDL_Rect _9[] = {
		{ 0, 0, 1, 3 },
		{ 2, 0, 1, 5 },
		{ 1, 0, 1, 1 },
		{ 1, 2, 1, 1 },
		{ 0, 4, 2, 1 },
	};
	const SDL_Rect colon[] = {
		{ 1, 1, 1, 1 },
		{ 1, 3, 1, 1 },
	};
	const SDL_Rect blank[] = { };
}

const SDL_Rect *font_get_character(char c, u32 *count) {
	switch (c) {
		case '0': *count = fixed_size(font::_0); return font::_0;
		case '1': *count = fixed_size(font::_1); return font::_1;
		case '2': *count = fixed_size(font::_2); return font::_2;
		case '3': *count = fixed_size(font::_3); return font::_3;
		case '4': *count = fixed_size(font::_4); return font::_4;
		case '5': *count = fixed_size(font::_5); return font::_5;
		case '6': *count = fixed_size(font::_6); return font::_6;
		case '7': *count = fixed_size(font::_7); return font::_7;
		case '8': *count = fixed_size(font::_8); return font::_8;
		case '9': *count = fixed_size(font::_9); return font::_9;
		case ':': *count = fixed_size(font::colon); return font::colon;
		case ' ': *count = fixed_size(font::blank); return font::blank;
	}
	return nullptr;
}

TTF_Font *font1; TTF_Font *get_font1() { return font1; }

void init_fonts() {
	TTF_Init();
	font1 = TTF_OpenFont("/usr/share/fonts/75dpi/term14.pcf.gz", 12);
	if (!font1) {
		puts("error: loading font");
	}
	TTF_SetFontKerning(font1, true);
}
