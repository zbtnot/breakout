# breakout
A demo of how to use the emscripten filesystem from both JavaScript and C++

## Building
### Prerequisites:
- emscripten sdk `brew install emscripten`
- node/npm `brew install node`
- raylib for multimedia 
    - desktop: `brew install raylib`
    - browser: `curl -sL https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_webassembly.zip | bsdtar -x -C ./dep`

### Steps:
- Clone the repo
- Download assets `git lfs pull`
- For desktop:
    - Run make `TARGET=desktop make`
    - Run from `./bin`
- For webassembly:
    - Install node deps: `npm ci`
    - Run make `TARGET=wasm make`
    - Launch a webserver: `npx http-server ./bin`


## Assets
All CCO / public domain
- assets/bg.png https://opengameart.org/content/oil-painting-landscapes
- assets/{ paddle.png, ball.png, block_*.png } from https://www.kenney.nl/assets/puzzle-pack
- assets/font.ttf https://www.kenney.nl/assets/kenney-fonts
- assets/{ bonk.ogg, womp.ogg } from https://opengameart.org/content/512-sound-effects-8-bit-style
- assets/bgm.ogg from https://opengameart.org/content/space-cadet-training-montage
- assets/gameover.ogg from https://opengameart.org/content/icy-game-over