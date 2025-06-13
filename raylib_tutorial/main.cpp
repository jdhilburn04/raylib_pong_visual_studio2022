#include "raylib.h"
#include <string>

int main() {
    int window_width = 800;
    int window_height = 600;
    int font_size = 20;
    const char* text = "-";

    int textWidth = MeasureText(text, font_size);
    int center_x = static_cast<int>(window_width) / 2;
    int center_y = static_cast<int>(window_height) / 2;


    InitWindow(window_width, window_height, text);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(text, center_x - textWidth, center_y, font_size, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
