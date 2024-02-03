#include "Game.h"
#include <cstdlib>
#include <ctime>

Game::Game(const char *title) {
    // initialize raylib
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(this->SCREEN_WIDTH, this->SCREEN_HEIGHT, title);
    InitAudioDevice();
    srand(time(nullptr));

    // load assets
    this->bonk = LoadSound("./assets/bonk.wav");
    this->womp = LoadSound("./assets/womp.wav");
    this->bgm = LoadMusicStream("./assets/bgm2.wav");
    this->gameOverBgm = LoadMusicStream("./assets/gameover.mp3");
    this->bg = LoadTexture("./assets/bg.png");
    this->paddleTexture = LoadTexture("./assets/paddle.png");
    this->ballTexture = LoadTexture("./assets/ball.png");
    this->blockTextures.push_back(LoadTexture("./assets/block_blue.png"));
    this->blockTextures.push_back(LoadTexture("./assets/block_green.png"));
    this->blockTextures.push_back(LoadTexture("./assets/block_purple.png"));
    this->blockTextures.push_back(LoadTexture("./assets/block_red.png"));
    this->blockTextures.push_back(LoadTexture("./assets/block_yellow.png"));
    this->font = LoadFontEx("./assets/font.ttf", 96, 0, 0);

    // lambda for passing around the event emitter
    auto eventHandler = [this](Event event, Entity *entity) { this->handleEvents(event, entity); };

    // load game objects
    auto paddlePositionX = this->SCREEN_WIDTH / 2 - this->paddleTexture.width / 2;
    auto paddlePositionY = this->SCREEN_HEIGHT - 50;
    this->paddle = new Paddle(&(this->paddleTexture), &(this->VIEWPORT), eventHandler);
    this->ball = new Ball(&(this->ballTexture), &(this->VIEWPORT), this->paddle, eventHandler);
    this->scoreBoard = new ScoreBoard(&(this->VIEWPORT), &(this->font), eventHandler);

    auto blockColumns = SCREEN_WIDTH / 2 / this->blockTextures[0].width;
    auto blockRows = 5;
    for (int i = 0; i < blockColumns; i++) {
        for (int j = 0; j < blockRows; j++) {
            auto *texture = &(this->blockTextures[rand() % 5]);
            auto x = (texture->width * 4) + texture->width * i + ((texture->width / 4) * i);
            auto y = texture->height * j + ((texture->height / 4) * j) + this->scoreBoard->GetDimensions().height + 10;
            this->blocks.push_back(new Block(x, y, texture, this->ball, eventHandler));
        }
    }

    // initialize the game
    SetMusicVolume(this->bgm, 0.15);
    SetMusicVolume(this->gameOverBgm, 0.15);
    SetSoundVolume(this->bonk, 0.15);
    SetSoundVolume(this->womp, 0.20);
    PlayMusicStream(this->bgm);
}

Game::~Game() {
    // unload game objects
    delete this->paddle;
    delete this->ball;
    for (auto block: this->blocks) {
        delete block;
    }
    delete this->scoreBoard;

    // unload assets
    UnloadFont(this->font);
    UnloadTexture(this->bg);
    UnloadTexture(this->paddleTexture);
    UnloadTexture(this->ballTexture);
    for (auto blockTexture: this->blockTextures) {
        UnloadTexture(blockTexture);
    }
    UnloadMusicStream(this->bgm);
    UnloadMusicStream(this->gameOverBgm);
    UnloadSound(this->bonk);
    UnloadSound(this->womp);

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

    if (!this->gameOver) {
        this->scoreBoard->update();
        // update game objects
        this->paddle->update();
        this->ball->update();

        for (auto block: this->blocks) {
            block->update();
        }
    } else {
        UpdateMusicStream(this->gameOverBgm);

        // reset game
        if (IsKeyPressed(KEY_SPACE) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))) {
            StopMusicStream(this->gameOverBgm);
            PlayMusicStream(this->bgm);
            this->gameOver = false;
            this->ball->reset();
            this->paddle->reset();
            this->scoreBoard->reset();
            for (auto block: this->blocks) {
                block->reset();
            }
        }
    }
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (!this->gameOver) {
        // draw the background image, scaling it to the destination viewport
        const Rectangle src = {.x = 0, .y = 0, .width = (float) this->bg.width, .height = (float) this->bg.height};
        DrawTexturePro(this->bg, src, this->VIEWPORT, Vector2Zero(), 0, WHITE);
        this->scoreBoard->draw();

        // draw game objects
        this->paddle->draw();
        this->ball->draw();
        for (auto block: this->blocks) {
            block->draw();
        }
    } else {
        gameOverMessage.str("");
        gameOverMessage << "Game Over!\n\n";
        gameOverMessage << "Score: " << this->scoreBoard->getScore() << "\n\n";
        gameOverMessage << "Retry with <space> or <a button>\n";
        auto gameOverMessageStr = gameOverMessage.str();
        auto position = Vector2 {
            .x = (float)(this->VIEWPORT.width / 2 - MeasureText(gameOverMessageStr.c_str(), 32) / 2),
            .y = (float)(this->VIEWPORT.height / 2 - 64),
        };
        DrawTextEx(this->font, gameOverMessageStr.c_str(), position, 32, 0.0f, WHITE);
    }


    EndDrawing();
}

void Game::handleEvents(Event e, Entity *entity) {
    switch (e) {
        case Event::BLOCK_HIT: {
            this->scoreBoard->setScore(this->scoreBoard->getScore() + 10);
        } // intentional fall-thru to bonk
        case Event::BONK: {
            PlaySound(this->bonk);
            this->ball->invertVelocity();
        } break;

        case Event::BALL_LOST: {
            PlaySound(this->womp);
            this->scoreBoard->setLives(this->scoreBoard->getLives() - 1);
            this->ball->reset();
            this->paddle->reset();
        } break;

        case Event::LAUNCH: {
            this->ball->launch();
        } break;

        case Event::GAME_OVER: {
            this->gameOver = true;
            StopMusicStream(this->bgm);
            PlayMusicStream(this->gameOverBgm);
        } break;
        default:
            break;
    }
}
