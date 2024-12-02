#include "raylib.h"

const int windowWidth = 800;
const int windowHeight = 300;
const int dinoWidth = 40;
const int dinoHeight = 40;
const int obstacleWidth = 20;
const int obstacleHeight = 40;
const int groundHeight = 50;
const int gravity = 1;
const int jumpStrength = 15;

struct Obstacle {
    Rectangle rect;
    int speed;
};

void DrawDinoFace(Rectangle dino) {
    int eyeOffsetX = 8;
    int eyeOffsetY = 10;
    int eyeRadius = 5;

    DrawCircle(dino.x + eyeOffsetX, dino.y + eyeOffsetY, eyeRadius, BLACK);
    DrawCircle(dino.x + eyeOffsetX + 12, dino.y + eyeOffsetY, eyeRadius, BLACK);

    int mouthOffsetY = 25;
    DrawLine(dino.x + 5, dino.y + mouthOffsetY, dino.x + 35, dino.y + mouthOffsetY, DARKGRAY);
}

int main() {
    InitWindow(windowWidth, windowHeight, "Google Dinosaur Game");

    Rectangle dino = { 100, windowHeight - groundHeight - dinoHeight, dinoWidth, dinoHeight };
    bool isJumping = false;
    int jumpVelocity = 0;

    Obstacle obstacle = { {windowWidth, windowHeight - groundHeight - obstacleHeight, obstacleWidth, obstacleHeight}, 5 };

    int score = 0;
    bool gameOver = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (gameOver) {
            if (IsKeyPressed(KEY_R)) {
                score = 0;
                gameOver = false;
                dino.y = windowHeight - groundHeight - dinoHeight;
                obstacle.rect.x = windowWidth;
            }
            else {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawRectangle(0, windowHeight - groundHeight, windowWidth, groundHeight, DARKGRAY);
                DrawRectangleRec(dino, GREEN);
                DrawRectangleRec(obstacle.rect, DARKGRAY);
                DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);
                DrawText("Game Over! Press 'R' to Replay", windowWidth / 2 - 150, windowHeight / 2, 20, RED);
                EndDrawing();
                continue;
            }
        }

        if (IsKeyPressed(KEY_SPACE) && !isJumping) {
            isJumping = true;
            jumpVelocity = -jumpStrength;
        }

        if (isJumping) {
            dino.y += jumpVelocity;
            jumpVelocity += gravity;

            if (dino.y >= windowHeight - groundHeight - dinoHeight) {
                dino.y = windowHeight - groundHeight - dinoHeight;
                isJumping = false;
            }
        }

        obstacle.rect.x -= obstacle.speed;

        if (obstacle.rect.x + obstacle.rect.width < 0) {
            obstacle.rect.x = windowWidth;
            obstacle.rect.y = windowHeight - groundHeight - obstacleHeight;
            score++;
        }

        if (CheckCollisionRecs(dino, obstacle.rect)) {
            gameOver = true;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle(0, windowHeight - groundHeight, windowWidth, groundHeight, DARKGRAY);
        DrawRectangleRec(dino, GREEN);
        DrawDinoFace(dino);
        DrawRectangleRec(obstacle.rect, DARKGRAY);
        DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
