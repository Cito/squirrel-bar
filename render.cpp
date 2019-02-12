#include "main.h"
#include "render.h"

void Renderer::fill_rect(SDL_Rect *rect) {
	SDL_RenderFillRect(ren, rect);
}
void Renderer::draw_rect(SDL_Rect *rect, u32 in) {
	SDL_Rect r = *rect;
	r.h = in;
	fill_rect(&r);
	r.y += rect->h - in;
	fill_rect(&r);
	r.y = rect->y;
	r.w = in;
	r.h = rect->h;
	fill_rect(&r);
	r.x += rect->w - in;
	fill_rect(&r);
}

void Renderer::render_character(char c, u32 x, u32 y, u32 multiplier) {
	u32 count;
	const SDL_Rect *rects = font_get_character(c, &count);
	if (!rects) {
		puts("error: rendering invalid character");
		return;
	}
	for (u32 i = 0; i < count; i++) {
		SDL_Rect rect = { i32(rects[i].x * multiplier), i32(rects[i].y * multiplier), i32(rects[i].w * multiplier), i32(rects[i].h * multiplier) };
		rect.x += x; rect.y += y;
		SDL_RenderFillRect(ren, &rect);
	}
}

u32 Renderer::render_text(char *text, i32 x, i32 y, SDL_Color color) {
	SDL_Surface *surf = TTF_RenderUTF8_Blended(get_font1(), text, color);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
	SDL_Rect rect = { x, y };
	SDL_QueryTexture(tex, nullptr, nullptr, &rect.w, &rect.h);
	SDL_RenderCopy(ren, tex, nullptr, &rect);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surf);
	return rect.w;
}

void Renderer::draw_heart(i32 x, i32 y, i32 sz, SDL_Color fg, SDL_Color bg) {
	const char heart[] = 
		"  xx xx  \n"
		" x**x**x \n"
		"x*x*****x\n"
		"x*x*****x\n"
		" x*****x \n"
		"  x***x  \n"
		"   x*x   \n"
		"    x    \n";
	SDL_Rect rect;
	i32 px = 0, py = 0;
	for (u32 i = 0; i < sizeof(heart) - 2; i++) {
		char c = heart[i];
		if (c == '\n') {
			py++;
			px = 0;
			continue;
		} else if (c == 'x' || c == '*') {
			SDL_Color color = c == 'x' ? fg : bg;
			set_color(color);
			rect = { x + px * sz, y + py * sz, sz, sz };
			fill_rect(&rect);
			px++;
		} else px++;
	}
}

void Renderer::render_time(u64 frame) {
	char time_str[] = "xx:xx:xx";
	time_t t = time(nullptr);
	if (t == (time_t)-1) return;
	tm *ti = localtime(&t);
	time_str[0] = (ti->tm_hour / 10) + '0';
	time_str[1] = (ti->tm_hour % 10) + '0';
	time_str[3] = (ti->tm_min / 10) + '0';
	time_str[4] = (ti->tm_min % 10) + '0';
	time_str[6] = (ti->tm_sec / 10) + '0';
	time_str[7] = (ti->tm_sec % 10) + '0';
	u32 cw = 3 * 3 + 1;
	u32 x = (w >> 1) - (4 * cw);
	for (u32 i = 0; i < fixed_size(time_str) - 1; i++) {
		SDL_Color color = colors::get_rainbow((frame * 50 + i * 60) % (6 * 256));
		colors::enlighten(&color, 160);
		set_color(color);
		render_character(time_str[i], x, 3, 3);
		x += cw;
	}
}

void Renderer::render_workspaces(u64 frame) {
	u32 x = 0;
	for (u32 i = 0; i < WS_MAX; i++) {
		Workspace ws = client->workspaces[i];
		SDL_Rect rect = { (i32)(x + 10), 6, 10, 10 };
		if (ws == Workspace::exists) {
			set_color(colors::fg);
			draw_rect(&rect, 2);
		} else if (ws == Workspace::none) {
			set_color(colors::fg2);
			draw_rect(&rect, 1);
		} else if (ws == Workspace::active) {
			set_color(colors::active);
			fill_rect(&rect);
		}
		x += 25;
	}
}

i32 readnext(FILE *p, char *t, u32 max) {
	for (u32 i = 0; i < max; i++) {
		i32 c = fgetc(p);
		if (c == EOF || c == ':' || c == '\n') {
			t[i] = 0;
			return c;
		}
		else t[i] = c;
	}
	return 'e';
}

void Renderer::render_network(u64 frame) {
	FILE *p = popen("nmcli -t -c no -e no -f TYPE,STATE,CONNECTION -m tabular d", "r");
	if (!p) {
		puts("error: could not get network status");
		return;
	}
	
	char type[24], state[64], name[128];
	bool success = false, do_break = false;
	for (u32 i = 0; !do_break;) {
		i32 n1 = readnext(p, type, sizeof(type));
		if (n1 == ':') {
			i32 n2 = readnext(p, state, sizeof(state));
			if (n2 == ':') {
				if (readnext(p, name, sizeof(name)) == 'e')
					return;
				if (!strcmp(type, "tun")) continue;
				if (!strcmp(state, "unmanaged")) continue;
				if (!strcmp(state, "unavailable")) continue;
				if (success = strcmp(state, "disconnected"))
					break;
			} else if (n2 == EOF) break;
			else if (do_break = (n2 == 'e')) break;
		} else if (n1 == EOF) break;
		else if (do_break = (n1 == 'e')) return;
	}

	pclose(p);

	if (do_break) return;

	if (!success) {
		render_text((char*)"no network", 264, 3, colors::bad);
	} else if (!strcmp(state, "connected")) {
		std::ostringstream os1, os2, os3;
		os1 << type << ":";
		os2 << " [";

		p = popen("ip -4 -br a show", "r");
		u8 tmp[17];

		while (true) {
			fread(tmp, 1, 17, p);
			fread(tmp, 1, 2, p);
			if (tmp[0] == 'U' && tmp[1] == 'P') {
				fread(tmp, 1, 13, p);
				u32 n = 0;
				for (char c = fgetc(p); n < 16 && c != '\n' &&
						c != '/' && c != EOF; c = fgetc(p)) {
					tmp[n++] = c;
				}
				tmp[n] = 0;
				os2 << (char*)tmp;
				break;
			}
			for (char c = 0; c != '\n' && c != EOF; c = fgetc(p));
		}
		pclose(p);

		os2 << "]";

		std::string str1 = os1.str();
		std::string str2 = os2.str();
		u32 s1 = render_text((char*)str1.c_str(), 264, 3, colors::good);
		u32 s2 = render_text(name, 264 + s1, 3, colors::fg3);
		render_text((char*)str2.c_str(), 264 + s1 + s2, 3, colors::fg);
	} else {
		std::ostringstream os;
		os << type << ":" << name << "[" << state << "]";
		std::string str = os.str();
		render_text((char*)str.c_str(), 264, 3, colors::connecting);
	}
}

u32 u32_fromfile(const char *path) {
	FILE *f = fopen(path, "r");
	if (!f) printf("error: could not read %s\n", path);
	u32 n = 0;
	for (i32 c = fgetc(f); c != EOF; c = fgetc(f))
		n = n * 10 + c - '0';
	fclose(f);
	return n;
}

char bat_status() {
	const char path[] = "/sys/class/power_supply/BAT0/status";
	FILE *f = fopen(path, "r");
	if (!f) printf("error: could not read %s\n", path);
	char c = fgetc(f);
	fclose(f);
	return c;
}

void Renderer::render_battery(u64 frame) {
	u32 full = u32_fromfile("/sys/class/power_supply/BAT0/energy_full");
	u32 now = u32_fromfile("/sys/class/power_supply/BAT0/energy_now");
	char status = bat_status();
	f32 part = (f32)now / (f32)full;
	SDL_Color color = (status == 'D' ?
		colors::bad : (status == 'C' ? colors::good : colors::active));
	draw_heart(w - 20, 2, 2, colors::fg, color);
	u8 percent = u8(part * 100.f);
	i32 chars[3];
	for (u32 i = 1; i < 101; i *= 10) chars[i & 3] = ((percent / (i)) % 10) + '0';
	set_color(colors::fg);
	render_character(chars[1], w - 40, 3, 3);
	if (percent > 9) render_character(chars[2], w - 50, 3, 3);
	if (percent > 99) render_character(chars[0], w - 60, 3, 3);
}

u32 u32_until(char **s) {
	u32 n = 0;
	for (; **s != '\n' && **s != 0 && **s != ' '; (*s)++)
		n = n * 10 + **s - '0';
	if (**s == ' ') (*s)++;
	return n;
}

void Renderer::render_cpu(u64 frame) {
	if ((frame % 5) == 0) {
		FILE *f = fopen("/proc/stat", "r");
		std::vector<cputime> cpus;
		char line[1024];
		while (fgets(line, sizeof(line), f)) {
			char *s = line;
			if (s[0] == 'c' && s[1] == 'p' && s[2] == 'u') {
				s += 3;
				u32 index;
				if (*s == ' ') { index = 0; s++; }
				else index = 1 + u32_until(&s);

				u32 total = 0, working = 0;
				for (u32 i = 0; i < 10; i++) {
					u32 t = u32_until(&s);
					total += t;
					if (i < 3) working += t;
				}

				if (cpus.size() <= index)
					cpus.resize(index + 1);

				cpus[index] = { total, working };
			} else if (s[0] == 'p' && s[6] == 'r') {
				s += 14;
				procs_running = u32_until(&s);
			}
		}
		fclose(f);

		usages.resize(cpus.size());
		if (!last_cputimes.empty()) {
			for (u32 i = 0; i < cpus.size(); i++) {
				/* i64 tdelta = cpus[i].total - last_cputimes[i].total;
				i64 idelta = cpus[i].idle - last_cputimes[i].idle;
				usages[i] = (f32)idelta / (f32)tdelta;
				if (i == 0) {
					usages[i] = pow(usages[i], 0.9) * 5.f;
				} else usages[i] = 1.f - usages[i];
				if (usages[i] > 1.f) usages[i] = 1.f;
				if (usages[i] < 0.f) usages[i] = 0.f; */
				i64 tdelta = cpus[i].total - last_cputimes[i].total;
				i64 wdelta = cpus[i].working - last_cputimes[i].working;
				usages[i] = pow((f32)wdelta / (f32)tdelta, 0.8f);
			}
		}
		last_cputimes = cpus;

		for (u32 i = 0; i < fixed_size(cpu_vals) - 1; i++) cpu_vals[i] = cpu_vals[i + 1];
		cpu_vals[fixed_size(cpu_vals) - 1] = usages[0];
	}

	SDL_Rect rect = { i32(w - 171), 1, fixed_size(cpu_vals) + 2, i32(h - 2) };
	set_color(colors::bg2);
	fill_rect(&rect);
	set_color(colors::fg2);
	draw_rect(&rect, 1);


	set_color(colors::fg);
	u32 height = h - 4;
	for (u32 i = 0; i < fixed_size(cpu_vals); i++) {
		SDL_Color color = colors::get_rainbow((frame * 25 + i * 10) % (6 * 256));
		set_color(color);
		u32 v = height - (u32)(cpu_vals[i] * height);
		SDL_Rect rect = { i32(w - 170 + i), i32(2 + v), 1, 1 };
		fill_rect(&rect);
	}
	i32 ww = i32(8 * (usages.size() - 1));
	rect = { i32(w - 171 - 5 - ww), 1, ww, i32(h - 2) };
	set_color(colors::fg);
	fill_rect(&rect);
	set_color(colors::bg);
	rect.w = 6;
	rect.h -= 2;
	rect.x++;
	for (u32 i = 1; i < usages.size(); i++) {
		rect.h = (u32)((h - 4) * usages[i]);
		rect.y = h - 2 - rect.h;
		SDL_Color color = colors::get_rainbow((frame * 100 + i * 50) % (6 * 256));
		colors::enlighten(&color, -160);
		set_color(color);
		if (rect.h)
			fill_rect(&rect);
		rect.x += 8;
	}
	set_color(colors::fg);
	if (procs_running > 9) {
		procs_running = 9;
		set_color(colors::bad);
	}
	render_character(procs_running + '0', w - 256, 3, 3);
}

void Renderer::render(u64 frame) {
	set_color(colors::bg);
	SDL_RenderClear(ren);
	
	render_time(frame);
	render_workspaces(frame);
	render_network(frame);
	render_battery(frame);
	render_cpu(frame);
	
	SDL_RenderPresent(ren);
}
