// BIBLIOTECAS RAYLIB
#include "raylib.h"
#include "raymath.h"

// BIBLIOTECAS BÁSICAS
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

// FUNCTIONS
void resetBall(Vector2 *ballPos, Vector2 *ballSpeed, int screenWidth, int screenHeight) // Resetar bola pós 1 ponto
{
    *ballPos = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};

    float speedX = (float)(rand() % 3 + 1);
    float speedY = (float)(rand() % 3 + 1);
    if (rand() % 2 == 0)
        speedX *= -1;
    if (rand() % 2 == 0)
        speedY *= -1;

    *ballSpeed = (Vector2){speedX, speedY};
};

void endGame() // Encerrar jogo
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Jogo encerrado! Aperte ESC para sair", 100, 200, 30, WHITE);
        EndDrawing();
        if (IsKeyDown(KEY_ESCAPE))
            break;
    }
    CloseWindow();
    exit(0);
};

void winRight(int screenWidth) // Vitoria DIREITA
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        int textWidth = MeasureText("DIREITA GANHOU COM 10 PONTOS", 30);
        DrawText("DIREITA GANHOU COM 10 PONTOS", (screenWidth - textWidth) / 2, 200, 30, GOLD);
        EndDrawing();
        sleep(2); // 2 segundos
        endGame();
    }
};

void winLeft(int screenWidth) // Vitoria ESQUERDA
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        int textWidth = MeasureText("ESQUERDA GANHOU COM 10 PONTOS", 30);
        DrawText("ESQUERDA GANHOU COM 10 PONTOS", (screenWidth - textWidth) / 2, 200, 30, GOLD);
        EndDrawing();
        sleep(2); // 2 segundos
        endGame();
    }
};

// INICIO
int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    const int winScore = 10;

    InitWindow(screenWidth, screenHeight, "Pong Game");
    SetTargetFPS(120);
    srand(time(NULL));

    Vector2 ballPos;
    Vector2 ballSpeed;
    const float radius = 10.0f;
    const float ballSpeedGain = 1.1f; // GANHO DE VELOCIDADE APÓS CADA COLISÃO

    resetBall(&ballPos, &ballSpeed, screenWidth, screenHeight);

    Rectangle playerLeft = {50, screenHeight / 2 - 50, 20, 100};
    Rectangle playerRight = {screenWidth - 70, screenHeight / 2 - 50, 20, 100};
    const int playerSpeed = 400;

    int scoreLeft = 0;
    int scoreRight = 0;

    bool gameStart = false;

    while (!WindowShouldClose())
    {
        // INPUTS
        if (!gameStart)
        {
            if (IsKeyDown(KEY_ENTER))
            {
                gameStart = true;
            }
        }
        else
        {
            ballPos.x += ballSpeed.x;
            ballPos.y += ballSpeed.y;
        }

        float deltaTime = GetFrameTime();
        // Jogador esquerdo (W e S)
        if (IsKeyDown(KEY_W))
            playerLeft.y -= playerSpeed * deltaTime;
        if (IsKeyDown(KEY_S))
            playerLeft.y += playerSpeed * deltaTime;

        if (playerLeft.y < 0)
            playerLeft.y = 0;
        if (playerLeft.y + playerLeft.height > screenHeight)
            playerLeft.y = screenHeight - playerLeft.height;

        // Jogador direito (setas)
        if (IsKeyDown(KEY_UP))
            playerRight.y -= playerSpeed * deltaTime;
        if (IsKeyDown(KEY_DOWN))
            playerRight.y += playerSpeed * deltaTime;

        if (playerRight.y < 0)
            playerRight.y = 0;
        if (playerRight.y + playerRight.height > screenHeight)
            playerRight.y = screenHeight - playerRight.height;

        // COLISSION
        //  Rebater nas bordas superior/inferior
        if (ballPos.y <= radius || ballPos.y >= screenHeight - radius)
            ballSpeed.y *= -1;

        // Colisão com jogador esquerdo
        // Calcula o ponto mais próximo da bola dentro do retângulo do jogador esquerdo
        float closestX1 = Clamp(ballPos.x, playerLeft.x, playerLeft.x + playerLeft.width);
        float closestY1 = Clamp(ballPos.y, playerLeft.y, playerLeft.y + playerLeft.height);

        // Calcula a distância entre o centro da bola e esse ponto
        float dx1 = ballPos.x - closestX1;
        float dy1 = ballPos.y - closestY1;

        // Se a distância ao quadrado for menor que o raio ao quadrado, houve colisão
        if ((dx1 * dx1 + dy1 * dy1) < (radius * radius))
        {
            ballSpeed.x *= -1;                                    // Inverte a direção da bola no eixo X
            ballSpeed.x *= ballSpeedGain;                         // Aumenta um pouco a velocidade da bola
            ballPos.x = playerLeft.x + playerLeft.width + radius; // Reposiciona para fora do jogador para evitar travamento
        }

        // Colisão com jogador direito
        // Calcula o ponto mais próximo da bola dentro do retângulo do jogador direito
        float closestX2 = Clamp(ballPos.x, playerRight.x, playerRight.x + playerRight.width);
        float closestY2 = Clamp(ballPos.y, playerRight.y, playerRight.y + playerRight.height);

        // Calcula a distância entre o centro da bola e esse ponto
        float dx2 = ballPos.x - closestX2;
        float dy2 = ballPos.y - closestY2;

        // Se a distância ao quadrado for menor que o raio ao quadrado, houve colisão
        if ((dx2 * dx2 + dy2 * dy2) < (radius * radius))
        {
            ballSpeed.x *= -1;                  // Inverte a direção da bola no eixo X
            ballSpeed.x *= 1.1f;                // Aumenta um pouco a velocidade da bola
            ballPos.x = playerRight.x - radius; // Reposiciona para fora do jogador para evitar travamento
        }

        // POINTS
        if (ballPos.x < 0)
        {
            scoreRight++;
            resetBall(&ballPos, &ballSpeed, screenWidth, screenHeight);
        }
        else if (ballPos.x > screenWidth)
        {
            scoreLeft++;
            resetBall(&ballPos, &ballSpeed, screenWidth, screenHeight);
        }

        // SISTEMA VÍTORIA
        if (scoreLeft == winScore)
        {
            winLeft(screenWidth);
            endGame();
        }
        else if (scoreRight == winScore)
        {
            winRight(screenWidth);
            endGame();
        }

        // DRAW
        BeginDrawing();
        ClearBackground(BLACK);

        if (!gameStart)
        {
            DrawText("Pressione ENTER para começar", screenWidth / 2 - 250, screenHeight / 2, 30, GOLD);
        }
        else
        {
            // DESENHA ELEMENTOS BÁSICOS
            DrawCircleV(ballPos, radius, WHITE);
            DrawRectangleRec(playerLeft, WHITE);
            DrawRectangleRec(playerRight, WHITE);

            // DESENHA OS CONTADORES DE PONTOS
            DrawText(TextFormat("%d", scoreLeft), screenWidth / 4, 20, 40, WHITE);
            DrawText(TextFormat("%d", scoreRight), screenWidth * 3 / 4, 20, 40, WHITE);

            // DESENHA O FPS
            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
