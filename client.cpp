#include "main.h"
#include "client.h"

bool is_whitespace(char c) {
	return c == ' ' || c == '\n' || c == '\t';
}

void Client::init() {
	FILE *p = popen("i3 --get-socketpath", "r");
	if (!p) {
		puts("error: could not get the i3 socket path");
		return;
	}
	fgets(sockpath, sizeof(sockpath), p);
	u32 n = strlen(sockpath);
	for (;is_whitespace(sockpath[n - 1]); n--) sockpath[n - 1] = 0;
	
	addr.sun_family = AF_UNIX;

	if (n + 1 > sizeof(addr.sun_path)) {
		puts("error: socket path too long");
		return;
	}

	for (u32 i = 0; i <= n; i++) addr.sun_path[i] = sockpath[i];
}

void Client::run() {
	running = true;
	client_thread = std::thread(&Client::threaded, this);
}

void Client::threaded() {
	sock = socket(AF_UNIX, SOCK_STREAM, 0);

	if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
		puts("error: i3 socket connection failed");
		return;
	}

	puts("info: connected to i3 socket");

	char subscription[] = "[\"workspace\",\"shutdown\"]";
	send_request(I3Request::get_workspaces, nullptr, 0);
	bool is_subscripted = false;

	while (running) {
		union {
			char magic[6];
			struct {
				u32 size;
				I3Response type;
			} _;
		} head;
		recv(sock, head.magic, sizeof(head.magic), 0);
		if (head.magic[0] != 'i' && head.magic[1] != '3') {
			puts("error: invalid response");
			break;
		}
		recv(sock, &head._.size, 8, 0);
		char *data = new char[head._.size + 1];
		recv(sock, data, head._.size, 0);
		data[head._.size] = 0;

		switch (head._.type) {
			case I3Response::workspaces:
				for (u32 i = 0; i < WS_MAX; i++) workspaces[i] = Workspace::none;
				for (u32 _ = 0; _ < WS_MAX; _++) {
					u32 num = 0;
					for (;*data != '{'; data++);
					for (u32 i = 0; i < 2; i++, data++) for (;*data != '"'; data++);
					data++;
					Workspace ws = Workspace::exists;
					for (; *data >= '0' && *data <= '9'; num = num * 10 + *(data++) - '0');
					num--;
					for (u32 i = 0; i < 6; i++, data++) for (;*data != '"'; data++);
					data++;
					if (*data == 't') ws = Workspace::active;
					for (u32 i = 0; i < 7; i++, data++) for (;*data != ','; data++);
					for (;*data != '}'; data++);
					for (;*data != ',' && *data != ']'; data++);
					if (num >= 0 && num < 10) workspaces[num] = ws;
					if (*data == ']') break;
				}
				if (!is_subscripted)
					send_request(I3Request::subscribe, (u8*)subscription, sizeof(subscription) - 1);
				break;
			case I3Response::subscribe:
				if (data[11] != 't')
					puts("error: workspace subscription failed");
				else if (!is_subscripted)
					puts("info: workspace subscription succeeded");
				is_subscripted = true;
				break;
			case I3Response::event_workspace:
				// puts(data);
				// puts("");
				/* for (u32 i = 0; i < 3; i++, data++) for (;*data != '"'; data++);
				switch (*data) {
					case 'f': // focus
						break;
					case 'i': // init
						break;
					case 'e': // emptyr
				}*/
				send_request(I3Request::get_workspaces, nullptr, 0);
				break;
			case I3Response::event_shutdown:
				exit(0);
				break;
		}
	}
}

void Client::send_request(I3Request type, u8 *data, u32 size) {
	char buf[] = "i3-ipc____????";
	*(u32*)(buf + 6) = size;
	*(u32*)(buf + 10) = (u32)type;
	send(sock, buf, sizeof(buf) - 1, 0);
	if (size)
		send(sock, data, size, 0);
}
