# Einstein Puzzle — WebAssembly Port

Play it now: **https://martg0.github.io/einstein-puzzle/**

Einstein Puzzle is a free cross-platform open source remake of the old DOS game Sherlock, inspired by Albert Einstein's famous logic puzzle. The goal is to open all cards in a 6x6 grid using logical hints about relationships between card positions.

## WebAssembly Port

This is a browser-playable port of [lksj/einstein-puzzle](https://github.com/lksj/einstein-puzzle), compiled from C++ to WebAssembly using Emscripten. The original game logic is unchanged.

### What was added for the web port

- **Emscripten/SDL2 build** — SDL1-to-SDL2 compatibility layer (`sdl1_compat.c`) and scancode-to-keycode mapping for browser keyboard input
- **ASYNCIFY support** — allows the original blocking event loops to work in the browser, with all Command objects heap-allocated to prevent stack corruption
- **Persistent storage** — saves, settings, and Hall of Fame stored in IndexedDB via IDBFS
- **Mobile/touch support** — tap to confirm, long press (500ms) to eliminate; responsive layout
- **Fullscreen** — scales to fill screen while maintaining aspect ratio
- **Reset saves** — button to clear all persisted data

### How to build

```bash
# Install Emscripten
brew install emscripten   # or via emsdk

# Build
make -f Makefile.emscripten

# Run locally
python3 -m http.server 8080
# Open http://localhost:8080/einstein.html
```

### Deploying

Copy these 4 files to any static hosting:
- `einstein.html` (or rename to `index.html`)
- `einstein.js`
- `einstein.wasm`
- `einstein.data`

## Original project

- **Author:** Alexander Babichev / Flowix Games
- **Source:** [lksj/einstein-puzzle](https://github.com/lksj/einstein-puzzle) (based on Debian patches)
- **Original site:** http://games.flowix.com (archived at [web.archive.org](https://web.archive.org))

Einstein 2.0 features sound support, full internationalization, Russian localization, reengineered resource subsystem for custom graphics themes, and an in-game rules browser.
