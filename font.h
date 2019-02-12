#pragma once

// /usr/share/fonts/75dpi/term14.pcf.gz

void init_fonts();
const SDL_Rect *font_get_character(char c, u32 *count);

TTF_Font *get_font1();
