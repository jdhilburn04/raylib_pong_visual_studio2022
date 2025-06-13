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
    int paddle2PosX = 50;
    int paddle2PosY = screen_height / 2;
    int botPaddlePosX = 50;
    int botPaddlePosY = screen_height / 2;

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
        else if (currentScreen == ONE_PLAYER) {
            DrawRectangle(paddle1PosX, paddle1PosY, 10, 50, WHITE);
            DrawRectangle(botPaddlePosX, botPaddlePosY, 10, 50, WHITE);
            DrawRectangle(screen_width / 2, 0, 10, screen_height, WHITE);

            if (IsKeyDown(KEY_UP)) paddle1PosY -= 5;
            if (IsKeyDown(KEY_DOWN)) paddle1PosY += 5;
        }
        else if (currentScreen == TWO_PLAYER) {
            DrawRectangle(paddle1PosX, paddle1PosY, 10, 50, WHITE);
            DrawRectangle(paddle2PosX, paddle2PosY, 10, 50, WHITE);
            DrawRectangle(screen_width / 2, 0, 10, screen_height, WHITE);

            if (IsKeyDown(KEY_UP)) paddle1PosY -= 5;
            if (IsKeyDown(KEY_DOWN)) paddle1PosY += 5;
            if (IsKeyDown(KEY_W)) paddle2PosY -= 5;
            if (IsKeyDown(KEY_S)) paddle2PosY += 5;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
