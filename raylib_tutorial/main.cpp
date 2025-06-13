#include "raylib.h"
#include <string>

enum GameScreen { TITLE, ONE_PLAYER, TWO_PLAYER };

int main() {
    // Window setup
    int screen_width = 800;
    int screen_height = 600;
    InitWindow(screen_width, screen_height, "PONG");

    // Paddle positions
    int paddle1PosX = screen_width - 50;
    int paddle1PosY = screen_height / 2;
    int paddle1Width = 10;
    int paddle1Height = 50;
    int paddle2PosX = 50;
    int paddle2PosY = screen_height / 2;
    int paddle2Width = 10;
    int paddle2Height = 50;
    int botPaddlePosX = 50;
    int botPaddlePosY = screen_height / 2;
    int botPaddleSizeX = 10;
    int botPaddleSizeY = 50;

    // Ball positions
    int ballPosX = screen_width / 2;
    int ballPosY = screen_height / 2;
    int ballSpeedX = 5;
    int ballSpeedY = 5;
    int ballRadius = 10;

    // Game state variable
    GameScreen currentScreen = TITLE;

    // Title screen text
    const char* title_text = "PONG";
    const char* one_player_button_text = "ONE PLAYER";
    const char* two_player_button_text = "TWO PLAYER";
    int titleFontSize = 50;
    int buttonFontSize = 25;

    // Measure button sizes
    Vector2 onePlayerTextSize = MeasureTextEx(GetFontDefault(), one_player_button_text, buttonFontSize, 1);
    Vector2 twoPlayerTextSize = MeasureTextEx(GetFontDefault(), two_player_button_text, buttonFontSize, 1);

    Rectangle onePlayerButton = {
        screen_width / 2 - onePlayerTextSize.x / 2 - 10,
        screen_height / 2 - onePlayerTextSize.y / 2 + 50,
        onePlayerTextSize.x + 20,
        onePlayerTextSize.y + 20
    };
    Rectangle twoPlayerButton = {
        screen_width / 2 - twoPlayerTextSize.x / 2 - 10,
        screen_height / 2 - twoPlayerTextSize.y / 2 + 150,
        twoPlayerTextSize.x + 20,
        twoPlayerTextSize.y + 20
    };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (currentScreen == TITLE) {
            Vector2 mouse = GetMousePosition();
            bool hoveringOne = CheckCollisionPointRec(mouse, onePlayerButton);
            bool hoveringTwo = CheckCollisionPointRec(mouse, twoPlayerButton);

            if (hoveringOne && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = ONE_PLAYER;
            }
            else if (hoveringTwo && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = TWO_PLAYER;
            }

            // Draw title
            int titleWidth = MeasureText(title_text, titleFontSize);
            DrawText(title_text, screen_width / 2 - titleWidth / 2, 100, titleFontSize, WHITE);

            // Draw buttons
            DrawRectangleRec(onePlayerButton, hoveringOne ? LIGHTGRAY : GRAY);
            DrawRectangleLinesEx(onePlayerButton, 2, WHITE);
            DrawText(one_player_button_text, onePlayerButton.x + 10, onePlayerButton.y + 10, buttonFontSize, WHITE);

            DrawRectangleRec(twoPlayerButton, hoveringTwo ? LIGHTGRAY : GRAY);
            DrawRectangleLinesEx(twoPlayerButton, 2, WHITE);
            DrawText(two_player_button_text, twoPlayerButton.x + 10, twoPlayerButton.y + 10, buttonFontSize, WHITE);
        }
        // One Player Mode
        else if (currentScreen == ONE_PLAYER) {
            DrawRectangle(paddle1PosX, paddle1PosY, paddle1Width, paddle1Height, WHITE);
            DrawRectangle(botPaddlePosX, botPaddlePosY, paddle2PosX, paddle2PosY, WHITE);
            DrawRectangle(screen_width / 2, 0, 10, screen_height, WHITE);

            if (IsKeyDown(KEY_UP)) paddle1PosY -= 5;
            if (IsKeyDown(KEY_DOWN)) paddle1PosY += 5;
        }
        // Two Player Mode
        else if (currentScreen == TWO_PLAYER) {
            // Draw paddles
            DrawRectangle(paddle1PosX, paddle1PosY, paddle1Width, paddle1Height, WHITE);
            DrawRectangle(paddle2PosX, paddle2PosY, paddle2Width, paddle2Height, WHITE);
            // Draw divider
            DrawRectangle(screen_width / 2, 0, 10, screen_height, WHITE);
            // Draw ball
            DrawCircle(ballPosX, ballPosY, ballRadius, WHITE);
            
            // Take user input
            if (IsKeyDown(KEY_UP)) paddle1PosY -= 5;
            if (IsKeyDown(KEY_DOWN)) paddle1PosY += 5;
            if (IsKeyDown(KEY_W)) paddle2PosY -= 5;
            if (IsKeyDown(KEY_S)) paddle2PosY += 5;

            // Ball velocity
            ballPosX += ballSpeedX;
            ballPosY += ballSpeedY;

            if (ballPosX + ballRadius >= paddle1PosX &&
                ballPosX - ballRadius <= paddle1PosX + paddle1Width &&
                ballPosY >= paddle1PosY &&
                ballPosY <= paddle1PosY + paddle1Height)
            {
                ballSpeedX *= -1;
                ballPosX = paddle1PosX - ballRadius;
            }
            if (ballPosX - ballRadius <= paddle2PosX + paddle2Width &&
                ballPosX - ballRadius >= paddle2PosX &&
                ballPosY >= paddle2PosY + paddle2PosY &&
                ballPosY <= paddle2PosY)
            {
                ballSpeedX *= -1;
                ballPosX = paddle2PosX + paddle2Width + ballRadius;
            }
            if ((ballPosX >= (screen_width - ballRadius)) || (ballPosX <= ballRadius)) ballSpeedX *= -1;
            if ((ballPosY >= (screen_height - ballRadius)) || (ballPosY <= ballRadius)) ballSpeedY *= -1;

            // paddle 1 floor and ceiling collision
            if (paddle1Height + paddle1PosY >= screen_height) {
                paddle1PosY -= (paddle1Height / 2) - 20;
            }
            if (paddle1PosY <= 0) {
                paddle1PosY *= 0;
            }

            // paddle 2 floor and ceiling collision
            if (paddle2Height + paddle2PosY >= screen_height) {
                paddle2PosY -= (paddle2Height / 2) - 20;
            }
            if (paddle2PosY <= 0) {
                paddle2PosY *= 0;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
