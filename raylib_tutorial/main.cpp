#include "raylib.h"
#include <string>
#define NUM_FRAME 3

int main() {
    // window variables
    int screen_width = 800;
    int screen_height = 600;
    const char* title_text = "PONG";

    InitWindow(screen_width, screen_height, title_text);

    // title screen title_text variables
    const char* one_player_button_text = "ONE PLAYER";
    const char* two_player_button_text = "TWO PLAYER";
    int titleButtonTextSize = 25;
    int title_text_posX = (static_cast<int>(screen_width) / 2) - 75;
    //int title_text_posY = static_cast<int>(screen_height) / 2;
    int title_text_posY = 100;
    Vector2 onePlayerTextSize = MeasureTextEx(GetFontDefault(), one_player_button_text, titleButtonTextSize, 1);
    Vector2 twoPlayerTextSize = MeasureTextEx(GetFontDefault(), two_player_button_text, titleButtonTextSize, 1);
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

    bool clicked = false;

    SetTargetFPS(60);

    // main game loop
    
    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        bool hoveringOne = CheckCollisionPointRec(mouse, onePlayerButton);
        bool hoveringTwo = CheckCollisionPointRec(mouse, twoPlayerButton);
        bool pressedOne = hoveringOne && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        bool pressedTwo = hoveringTwo && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        if (pressedOne || pressedTwo) clicked = true;

        // screen background
        BeginDrawing();
        ClearBackground(BLACK);

        // button backgroundd
        DrawRectangleRec(onePlayerButton, hoveringOne ? LIGHTGRAY : GRAY);
        DrawRectangleRec(twoPlayerButton, hoveringTwo ? LIGHTGRAY : GRAY);

        // button border
        DrawRectangleLinesEx(onePlayerButton, 2, WHITE);
        DrawRectangleLinesEx(twoPlayerButton, 2, WHITE);

        // title screen text
        DrawText(title_text, title_text_posX, title_text_posY, 50, WHITE);
        DrawText(one_player_button_text, onePlayerButton.x + 5, onePlayerButton.y + 10, titleButtonTextSize, WHITE);
        DrawText(two_player_button_text, twoPlayerTextSize.x + 160, twoPlayerButton.y + 10, titleButtonTextSize, WHITE);

        if (clicked) {
            DrawText("Button clicked!", 10, 10, 20, RED);
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
