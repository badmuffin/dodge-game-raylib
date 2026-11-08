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

int main()
{
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dodge the blocks");
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

    /*================ user input handling =================*/
    if (currentState == PLAYING)
    {
      if (IsKeyDown(KEY_A))
        player.x -= playerSpeed * deltaTime;
      if (IsKeyDown(KEY_D))
        player.x += playerSpeed * deltaTime;
    }

    // to keep player inside the game window.
    player.x = std::clamp(player.x, 0.0f, (float)WINDOW_WIDTH - player.width);

    // ================ update the scene =========================
    if (currentState == PLAYING)
    {
      enemy.y += enemySpeed * deltaTime;

      // enemy reached the bottom of the window
      if (enemy.y > WINDOW_HEIGHT)
      {
        enemy.y = -enemy.height;
        enemy.x = (float)GetRandomValue(0, WINDOW_WIDTH - (int)enemy.width);

        score++;
        enemySpeed += 10.0f; // increasing player speed with each turn
      }

      if (CheckCollisionRecs(player, enemy))
        currentState = GAME_OVER;
    }

    if (currentState == MENU and IsKeyPressed(KEY_SPACE))
      currentState = PLAYING;

    // restart
    if (currentState == GAME_OVER and IsKeyPressed(KEY_R))
    {
      player.x = 180;
      enemy.x = (float)GetRandomValue(0, WINDOW_WIDTH - (int)enemy.width);
      enemy.y = -50;
      enemySpeed = 220.0f;

      score = 0;
      currentState = MENU;
    }

    // ================== Draw ==========================
    BeginDrawing();

    ClearBackground(BLACK);

    DrawRectangleRec(player, SKYBLUE);
    DrawRectangleRec(enemy, RED);

    DrawText(TextFormat("Score: %d", score), 20, 20, 25, WHITE);

    if (currentState == MENU)
      DrawText("Press Space to Start", 80, 250, 24, WHITE);

    if (currentState == GAME_OVER)
    {
      DrawText("GAME OVER", 80, 250, 40, RED);
      DrawText("Press R to Restart", 80, 310, 20, WHITE);
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
