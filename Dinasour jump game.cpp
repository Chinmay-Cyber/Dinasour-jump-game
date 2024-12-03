#include "raylib.h"

const int windowWidth = 800;
const int windowHeight = 300;
const int groundHeight = 50;
const int gravity = 1;
const int jumpStrength = 15;

class Dino {
public:
    Rectangle rect;
    bool isJumping;
    int jumpVelocity;

    Dino(int x, int y, int width, int height)
        : rect{ x, y, width, height }, isJumping(false), jumpVelocity(0) {}

    void Draw() const {
        DrawRectangleRec(rect, GREEN);
        DrawFace();
    }

    void Jump() {
        if (!isJumping) {
            isJumping = true;
            jumpVelocity = -jumpStrength;
        }
    }

    void Update() {
        if (isJumping) {
            rect.y += jumpVelocity;
            jumpVelocity += gravity;

            if (rect.y >= windowHeight - groundHeight - rect.height) {
                rect.y = windowHeight - groundHeight - rect.height;
                isJumping = false;
            }
        }
    }

private:
    void DrawFace() const {
        int eyeOffsetX = 8;
        int eyeOffsetY = 10;
        int eyeRadius = 5;

        DrawCircle(rect.x + eyeOffsetX, rect.y + eyeOffsetY, eyeRadius, BLACK);
        DrawCircle(rect.x + eyeOffsetX + 12, rect.y + eyeOffsetY, eyeRadius, BLACK);

        int mouthOffsetY = 25;
        DrawLine(rect.x + 5, rect.y + mouthOffsetY, rect.x + 35, rect.y + mouthOffsetY, DARKGRAY);
    }
};

class Obstacle {
public:
    Rectangle rect;
    int speed;

    Obstacle(int x, int y, int width, int height, int speed)
        : rect{ x, y, width, height }, speed(speed) {}

    void Draw() const {
        DrawRectangleRec(rect, DARKGRAY);
    }

    void Update() {
        rect.x -= speed;

        if (rect.x + rect.width < 0) {
            rect.x = windowWidth;
        }
    }
};

int main() {
    InitWindow(windowWidth, windowHeight, "Google Dinosaur Game");

    Dino dino(100, windowHeight - groundHeight - 40, 40, 40);
    Obstacle obstacle(windowWidth, windowHeight - groundHeight - 40, 20, 40, 5);

    int score = 0;
    bool gameOver = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (gameOver) {
            if (IsKeyPressed(KEY_R)) {
                score = 0;
                gameOver = false;
                dino.rect.y = windowHeight - groundHeight - dino.rect.height;
                obstacle.rect.x = windowWidth;
            }
            else {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawRectangle(0, windowHeight - groundHeight, windowWidth, groundHeight, DARKGRAY);
                dino.Draw();
                obstacle.Draw();
                DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);
                DrawText("Game Over! Press 'R' to Replay", windowWidth / 2 - 150, windowHeight / 2, 20, RED);
                EndDrawing();
                continue;
            }
        }

        if (IsKeyPressed(KEY_SPACE)) {
            dino.Jump();
        }

        dino.Update();
        obstacle.Update();

        if (obstacle.rect.x + obstacle.rect.width < 0) {
            score++;
            obstacle.rect.x = windowWidth;
        }

        if (CheckCollisionRecs(dino.rect, obstacle.rect)) {
            gameOver = true;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle(0, windowHeight - groundHeight, windowWidth, groundHeight, DARKGRAY);
        dino.Draw();
        obstacle.Draw();
        DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
