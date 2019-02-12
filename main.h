#ifndef _HDEF_SERVER_MAINH
#define _HDEF_SERVER_MAINH

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <linux/limits.h>
#include <sys/un.h>
#include <sstream>
#include <string>
#include <vector>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SOCKET_ERROR (-1)

using namespace std;

using u8 = uint8_t; using i8 = int8_t; 
using u16 = uint16_t; using i16 = int16_t; 
using u32 = uint32_t; using i32 = int32_t; 
using u64 = uint64_t; using i64 = int64_t; 
using f32 = float; using f64 = double;
static_assert(sizeof(f32) == 4, "float must be a 32-bit float");
static_assert(sizeof(f64) == 8, "double must be a 64-bit float");
using usize = size_t;

using cstr = char *; using ccstr = const char *;

#define fixed_size(arr) (sizeof((arr)) / sizeof((arr)[0]))

#include "color.h"
#include "client.h"
#include "font.h"
#include "render.h"
#include "window.h"

#endif
