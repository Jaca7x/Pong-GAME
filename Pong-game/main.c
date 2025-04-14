#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

//FUNCTIONS
void resetBall(Vector2 *ballPos, Vector2 *ballSpeed, int screenWidth, int screenHeight) {
    *ballPos = (Vector2) { screenWidth / 2.0f, screenHeight / 2.0f };

    float speedX = (float)(rand() % 2 + 1);
    float speedY = (float)(rand() % 2 + 1); 
    if (rand() % 2 == 0) speedX *= -1; 
    if (rand() % 2 == 0) speedY *= -1; 

    *ballSpeed = (Vector2) {speedX, speedY};
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Pong Game");
    SetTargetFPS(120);
    srand(time(NULL));

    Vector2 ballPos;
    Vector2 ballSpeed;
    const float radius = 10.0f;

    resetBall(&ballPos, &ballSpeed, screenWidth, screenHeight);

    Rectangle playerLeft = {50, screenHeight / 2 - 50, 20, 100};
    Rectangle playerRight = {screenWidth - 70, screenHeight / 2 - 50, 20, 100};

    int scoreLeft = 0;
    int scoreRight = 0;

while (!WindowShouldClose()) {
    //INPUTS
        float deltaTime = GetFrameTime();
        // Jogador esquerdo (W e S)
        if (IsKeyDown(KEY_W)) playerLeft.y -= 200 * deltaTime;
        if (IsKeyDown(KEY_S)) playerLeft.y +=  200 * deltaTime;

        if (playerLeft.y < 0) playerLeft.y = 0;
        if (playerLeft.y + playerLeft.height > screenHeight) 
            playerLeft.y = screenHeight - playerLeft.height;

        // Jogador direito (setas)
        if (IsKeyDown(KEY_UP)) playerRight.y -=  200 * deltaTime;
        if (IsKeyDown(KEY_DOWN)) playerRight.y +=  200 * deltaTime;

        if (playerRight.y < 0) playerRight.y = 0;
        if (playerRight.y + playerRight.height > screenHeight) 
            playerRight.y = screenHeight - playerRight.height;

        //UPDATE
        ballPos.x += ballSpeed.x;
        ballPos.y += ballSpeed.y;
        //COLISSION
        // Rebater nas bordas superior/inferior
        if (ballPos.y <= radius || ballPos.y >= screenHeight - radius) ballSpeed.y *= -1;

        // Colisão com jogador esquerdo
        float closestX1 = Clamp(ballPos.x, playerLeft.x, playerLeft.x + playerLeft.width); 
        float closestY1 = Clamp(ballPos.y, playerLeft.y, playerLeft.y + playerLeft.height);
        float dx1 = ballPos.x - closestX1;
        float dy1 = ballPos.y - closestY1;

        if ((dx1 * dx1 + dy1 * dy1) < (radius * radius)) {
            ballSpeed.x *= -1;
            ballSpeed.x *= 1.1f; 
            ballPos.x = playerLeft.x + playerLeft.width + radius; // evita travar
        }

        // Colisão com jogador direito 
        float closestX2 = Clamp(ballPos.x, playerRight.x, playerRight.x + playerRight.width);
        float closestY2 = Clamp(ballPos.y, playerRight.y, playerRight.y + playerRight.height);
        float dx2 = ballPos.x - closestX2;
        float dy2 = ballPos.y - closestY2;

        if ((dx2 * dx2 + dy2 * dy2) < (radius * radius)) {
            ballSpeed.x *= -1;
            ballSpeed.x *= 1.1f; 
            ballPos.x = playerRight.x - radius; // evita travar
        }

        //POINTS
        if (ballPos.x < 0)
        {
            scoreRight++;
            resetBall(&ballPos, &ballSpeed, screenWidth, screenHeight);
        } else if (ballPos.x > screenWidth)
        {
           scoreLeft++;
           resetBall(&ballPos, &ballSpeed, screenWidth, screenHeight);
        }

        //DRAW
        BeginDrawing();
        ClearBackground(BLACK);

        DrawCircleV(ballPos, radius, WHITE);
        DrawRectangleRec(playerLeft, WHITE);
        DrawRectangleRec(playerRight, WHITE);

        DrawText(TextFormat("%d", scoreLeft), screenWidth / 4, 20, 40, WHITE);
        DrawText(TextFormat("%d", scoreRight), screenWidth * 3 / 4, 20, 40, WHITE);
        
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
