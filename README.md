# squirrel-bar
another hard coded i3-bar rendered with SDL2.

![screenshot of the bar](bar-screenshot.png?raw=true "a screenshot of the bar")

## 1. Install
### 1.1 Clone Repository
`git clone https://github.com/NatrixAeria/squirrel-bar`
### 1.2 Build it
To compile the project make sure you have installed:
- SDL2
- SLD2-ttf
#### 1.2.1 With `build` script
just type:
`sh build`.
The `build` script compiles and runs the code automatically.
#### 1.2.2 Manually
compile it with gcc:
`g++ -o sqbar *.cpp -L/usr/lib -pthread -lSDL2 -lSDL2_ttf -L/usr/X11/lib -lX11 -lstdc++ -std=c++17`
### 2. Add it to your configs
edit your i3 config ( `etc/i3/config` or `~/.i3/config` or `~/.config/i3/config` ):
```
...
bar {
    ...
    i3bar_command path/to/squirrel-bar/sqbar
    ...
}
...
```
and save the file.
### 2. Reload i3
reload i3 with `i3 reload`
