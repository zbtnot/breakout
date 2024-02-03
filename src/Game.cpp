#include "Game.h"
#include <ctime>
#include <cstdlib>

Game::Game(const char *title) {
    // initialize raylib
    InitWindow(this->SCREEN_WIDTH, this->SCREEN_HEIGHT, title);
    InitAudioDevice();
    srand(time(nullptr));

    // load assets
    this->bonk = LoadSound("./assets/bonk.wav");
    this->bgm = LoadMusicStream("./assets/bgm.mp3");
    this->bg = LoadTexture("./assets/bg.png");
    this->paddleTexture = LoadTexture("./assets/paddle.png");
    this->ballTexture = LoadTexture("./assets/ball.png");
    
    this->blockTextures.push_back(LoadTexture("./assets/block_blue.png"));
    this->blockTextures.push_back(LoadTexture("./assets/block_green.png"));
    this->blockTextures.push_back(LoadTexture("./assets/block_purple.png"));
    this->blockTextures.push_back(LoadTexture("./assets/block_red.png"));
    this->blockTextures.push_back(LoadTexture("./assets/block_yellow.png"));
    
    this->font = LoadFontEx("./assets/font.ttf", 96, 0, 0);

    // load game objects
    auto paddlePositionX = this->SCREEN_WIDTH / 2 - this->paddleTexture.width / 2;
    auto paddlePositionY = this->SCREEN_HEIGHT - 50;

    auto launchHandler = [this]() {
        this->ball->launch();
    };

    auto bonkHandler = [this]() {
        PlaySound(this->bonk);
    }; 
    this->paddle = new Paddle(
        paddlePositionX,
        paddlePositionY,
        &(this->paddleTexture),
        &(this->VIEWPORT),
        launchHandler
    );
    this->ball = new Ball(&(this->ballTexture), &(this->VIEWPORT), this->paddle, bonkHandler);

    auto blockColumns = SCREEN_WIDTH / 2 / this->blockTextures[0].width;
    auto blockRows = 5;
    for (int i = 0; i < blockColumns; i++) {
        for (int j = 0; j < blockRows; j++) {
            auto *texture = &(this->blockTextures[rand() % 5]);
            auto x = (texture->width * 4) + texture->width * i + ((texture->width / 4) * i);
            auto y = texture->height * j + ((texture->height / 4) * j);
            this->blocks.push_back(new Block(x, y, texture, this->ball));
        }
    }

    // initialize the game
    SetMusicVolume(bgm, 0.15);
    SetSoundVolume(this->bonk, 0.15);
    PlayMusicStream(bgm);
}

Game::~Game() {
    // unload game objects
    delete this->paddle;
    delete this->ball;
    for (auto block : this->blocks) {
        delete block;
    }
    
    // unload assets
    UnloadFont(this->font);
    UnloadTexture(this->bg);
    UnloadTexture(this->paddleTexture);
    UnloadTexture(this->ballTexture);
    for (auto blockTexture : this->blockTextures) {
        UnloadTexture(blockTexture);
    }
    UnloadMusicStream(this->bgm);
    UnloadSound(this->bonk);

    // shutdown raylib
    CloseAudioDevice();
    CloseWindow();
}

void Game::frame() {
    this->update();
    this->draw();
}

void Game::update() {
    UpdateMusicStream(this->bgm);

    if (IsKeyPressed(KEY_Z)) {
        PlaySound(this->bonk);
    }

    // update game objects
    this->paddle->update();
    this->ball->update();

    for (auto block : this->blocks) {
        block->update();
    }
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    // draw the background image, scaling it to the destination viewport
    const Rectangle src = {.x = 0, .y = 0, .width = (float) this->bg.width, .height = (float) this->bg.height};
    DrawTexturePro(this->bg, src, this->VIEWPORT, Vector2Zero(), 0, WHITE);

    // draw game objects
    this->paddle->draw();
    this->ball->draw();

    for (auto block : this->blocks) {
        block->draw();
    }

    EndDrawing();
}

