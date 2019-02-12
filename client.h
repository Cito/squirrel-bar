#pragma once

enum class I3Request : u32 {
	run_command = 0,
	get_workspaces = 1,
	subscribe = 2,
	get_outputs = 3,
	get_tree = 4,
	get_marks = 5,
	get_bar_config = 6,
	get_version = 7,
	get_binding_modes = 8,
	get_config = 9,
	send_tick = 10,
	sync = 11,
};

enum class I3Response : u32 {
	command = 0,
	workspaces = 1,
	subscribe = 2,
	outputs = 3,
	tree = 4,
	marks = 5,
	bar_config = 6,
	version = 7,
	binding_modes = 8,
	config = 9,
	tick = 10,
	sync = 11,

	event_workspace = (0x80000000) | 0,
	event_output = (0x80000000) | 1,
	event_mode = (0x80000000) | 2,
	event_window = (0x80000000) | 3,
	event_barconfig_update = (0x80000000) | 4,
	event_binding = (0x80000000) | 5,
	event_shutdown = (0x80000000) | 6,
	event_tick = (0x80000000) | 7,
};

#define WS_MAX 10

enum class Workspace : u32 {
	none = 0,
	exists = 1,
	active = 2,
};

struct Client {
	sockaddr_un addr;
	std::thread client_thread;
	i32 sock;
	bool running;

	Workspace workspaces[WS_MAX];

	char sockpath[PATH_MAX];
	void init();

	void send_request(I3Request type, u8 *data, u32 size);

	void run();
	void threaded();
};
