#pragma once

struct cputime {
	u64 total, working;
};

struct Renderer {
	SDL_Window *win;
	SDL_Renderer *ren;
	u32 w, h;
	Client *client;

	std::vector<cputime> last_cputimes;
	std::vector<f32> usages;
	u32 procs_running;
	f32 cpu_vals[100];

	inline void set_color(SDL_Color c) const { SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a); }

	void fill_rect(SDL_Rect *rect);
	void draw_rect(SDL_Rect *rect, u32 in);

	void render_character(char c, u32 x, u32 y, u32 multiplier);
	u32 render_text(char *text, i32 x, i32 y, SDL_Color color);

	void draw_heart(i32 x, i32 y, i32 sz, SDL_Color fg, SDL_Color bg);

	void render_time(u64 frame);
	void render_workspaces(u64 frame);
	void render_network(u64 frame);
	void render_battery(u64 frame);
	void render_cpu(u64 frame);

	void render(u64 frame);
};
