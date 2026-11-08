#include "raylib.h"
#include <algorithm>

int WINDOW_WIDTH = 400;
int WINDOW_HEIGHT = 720;

enum GameState
{
  MENU,
  PLAYING,
  PAUSED,
  GAME_OVER
};

void UpdateGame(
    Rectangle &player,
    Rectangle &enemy,
    float playerSpeed,
    float &enemySpeed,
    float deltaTime,
    int &score,
    GameState &currentState)
{
  switch (currentState)
  {
  case MENU:
    if (IsKeyPressed(KEY_SPACE))
      currentState = PLAYING;
    break;

  case PLAYING:
    // player input handling
    if (IsKeyDown(KEY_A))
      player.x -= playerSpeed * deltaTime;
    if (IsKeyDown(KEY_D))
      player.x += playerSpeed * deltaTime;

    // to keep player inside the game window
    player.x = std::clamp(player.x, 0.0f, (float)GetScreenWidth() - player.width);

    // handle enemy
    enemy.y += enemySpeed * deltaTime;
    // enemy reached the bottom of the window
    if (enemy.y > GetScreenHeight())
    {
      enemy.y = -enemy.height;
      enemy.x = (float)GetRandomValue(0, GetScreenWidth() - (int)enemy.width);

      score++;
      enemySpeed += 10.0f; // increasing player speed with each turn
    }

    if (CheckCollisionRecs(player, enemy))
      currentState = GAME_OVER;
    if (IsKeyPressed(KEY_P))
      currentState = PAUSED;
    break;

  case PAUSED:
    if (IsKeyPressed(KEY_P))
      currentState = PLAYING;
    break;

  case GAME_OVER:
    if (IsKeyPressed(KEY_R))
    {
      player.x = 180;
      enemy.x = (float)GetRandomValue(0, GetScreenWidth() - (int)enemy.width);
      enemy.y = -50;
      enemySpeed = 220.0f;

      score = 0;
      currentState = MENU;
    }
    break;
  }
}

void DrawTextInCenter(const char *title, int fontSize, int yOffset = 0, Color color = WHITE)
{
  int textWidth = MeasureText(title, fontSize);
  int x = (GetScreenWidth() - textWidth) / 2;
  int y = (GetScreenHeight() - fontSize) / 2 + yOffset;
  DrawText(title, x, y, fontSize, color);
}

void DrawGame(const Rectangle &player, const Rectangle &enemy, int score, GameState currentState)
{
  DrawRectangleRec(player, SKYBLUE);
  DrawRectangleRec(enemy, RED);

  DrawText(TextFormat("Score: %d", score), 20, 20, 25, WHITE);

  if (currentState == MENU)
    DrawTextInCenter("Press Space to Start", 24);

  if (currentState == PAUSED)
    DrawTextInCenter("Press P to unpause", 24);

  if (currentState == GAME_OVER)
  {
    DrawTextInCenter("GAME OVER", 40, -20, RED);
    DrawTextInCenter("Press R to Restart", 24, 20, WHITE);
  }
}

int main()
{
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dodge the blocks");
  SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  SetTargetFPS(60);

  Rectangle player = {180, 600, 40, 40};
  float playerSpeed = 240.0f;

  Rectangle enemy = {200, -100, 40, 40};
  float enemySpeed = 220.0f;

  int score = 0;
  GameState currentState = MENU;

  while (!WindowShouldClose())
  {
    float deltaTime = GetFrameTime();
    UpdateGame(player, enemy, playerSpeed, enemySpeed, deltaTime, score, currentState);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawGame(player, enemy, score, currentState);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}