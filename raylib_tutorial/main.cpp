#include "raylib.h"
#include <string>

enum GameScreen { TITLE, ONE_PLAYER, TWO_PLAYER, GAME_OVER};

int main() {
    // Window setup
    int screen_width = 800;
    int screen_height = 600;
    InitWindow(screen_width, screen_height, "PONG");

    // Paddle positions
    int paddle1PosX = screen_width - 50;
    int paddle1PosY = screen_height / 2;
    int paddle1Width = 10;
    int paddle1Height = 75;
    int paddle2PosX = 50;
    int paddle2PosY = screen_height / 2;
    int paddle2Width = 10;
    int paddle2Height = 75;
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

    // ball reset
    bool ballPaused = false;
    double pauseStartTime = 0.0;
    double pauseDuration = 2.0;

    // Game state variable
    GameScreen currentScreen = TITLE;
    int player1Score = 0;
    int player2Score = 0;
    int botScore = 0;
    int winner = 0;
    bool shouldClose = false;

    // Title screen text
    const char* title_text = "PONG";
    const char* one_player_button_text = "ONE PLAYER";
    const char* two_player_button_text = "TWO PLAYER";
    const char* playAgainButtonText = "PLAY AGAIN?";
    const char* quitButtonText = "QUIT?";
    int titleFontSize = 150;
    int buttonFontSize = 25;
    int playAgainButtonFontSize = 25;

    // Measure button text sizes
    Vector2 onePlayerTextSize = MeasureTextEx(GetFontDefault(), one_player_button_text, static_cast<float>(buttonFontSize), 1.0f);
    Vector2 twoPlayerTextSize = MeasureTextEx(GetFontDefault(), two_player_button_text, static_cast<float>(buttonFontSize), 1.0f);
    Vector2 playAgainTextSize = MeasureTextEx(GetFontDefault(), playAgainButtonText,
                                static_cast<float>(playAgainButtonFontSize), 1.0f);
    Vector2 quitTextSize = MeasureTextEx(GetFontDefault(), quitButtonText, static_cast<float>(buttonFontSize), 1.0f);

    Rectangle onePlayerButton = {
        screen_width / 2 - onePlayerTextSize.x / 2 - 10,
        screen_height / 2 - onePlayerTextSize.y / 2 + 50,
        twoPlayerTextSize.x + 20,
        twoPlayerTextSize.y + 20
    };
    Rectangle twoPlayerButton = {
        screen_width / 2 - twoPlayerTextSize.x / 2 - 10,    
        screen_height / 2 - twoPlayerTextSize.y / 2 + 150,  
        twoPlayerTextSize.x + 20,                           
        twoPlayerTextSize.y + 20                            
    };
    Rectangle playAgain = {
        screen_width / 2 - playAgainTextSize.x / 2 - 10,
        screen_height / 2 - playAgainTextSize.y / 2,
        playAgainTextSize.x + 20,
        50
    };
    Rectangle quitButton = {
        screen_width / 2 - playAgainTextSize.x / 2 - 10,
        screen_height / 2 - quitTextSize.y / 2 + 125,
        playAgainTextSize.x + 20,
        50
    };

    SetTargetFPS(60);

    while (!WindowShouldClose() && !shouldClose) {
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
            DrawText(one_player_button_text,
                static_cast<int>(onePlayerButton.x + 7),
                static_cast<int>(onePlayerButton.y + 10), buttonFontSize, WHITE);

            DrawRectangleRec(twoPlayerButton, hoveringTwo ? LIGHTGRAY : GRAY);
            DrawRectangleLinesEx(twoPlayerButton, 2, WHITE);
            DrawText(two_player_button_text,
                static_cast<int>(twoPlayerButton.x + 5), 
                static_cast<int>(twoPlayerButton.y + 10), buttonFontSize, WHITE);
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
            DrawRectangle((screen_width / 2) - 5, 0, 10, screen_height, WHITE);
            // Draw ball
            DrawCircle(static_cast<int>(ballPosX), static_cast<int>(ballPosY), static_cast<float>(ballRadius), WHITE);
            // Draw scores
            DrawText(TextFormat("%i", player1Score), 175, 50, 50, WHITE);
            DrawText(TextFormat("%i", player2Score), 600, 50, 50, WHITE);

            // Take user input
            if (IsKeyDown(KEY_UP)) paddle1PosY -= 5;
            if (IsKeyDown(KEY_DOWN)) paddle1PosY += 5;
            if (IsKeyDown(KEY_W)) paddle2PosY -= 5;
            if (IsKeyDown(KEY_S)) paddle2PosY += 5;

            // left paddle collision
            if (ballPosX + ballRadius >= paddle1PosX &&
                ballPosX - ballRadius <= paddle1PosX + paddle1Width &&
                ballPosY >= paddle1PosY &&
                ballPosY <= paddle1PosY + paddle1Height)
            {
                ballSpeedX *= -1;
                ballPosX = paddle1PosX - ballRadius;
            }
            // right paddle collision
            if (ballPosX - ballRadius <= paddle2PosX + paddle2Width &&  
                ballPosX - ballRadius >= paddle2PosX &&                 
                ballPosY >= paddle2PosY &&
                ballPosY <= paddle2PosY + paddle2Height)
            {
                ballSpeedX *= -1;
                ballPosX = paddle2PosX + paddle2Width + ballRadius; 
            }

            // player 1 scores
            if (ballPosX <= ballRadius) {
                player2Score += 1;
                ballPosX = screen_width / 2;
                ballPosY = screen_height / 2;
                ballPaused = true;
                pauseStartTime = GetTime();
            }
            // player 2 scores
            if (ballPosX >= screen_width - ballRadius) {
                player1Score += 1;
                ballPosX = screen_width / 2;
                ballPosY = screen_height / 2;
                ballPaused = true;
                pauseStartTime = GetTime();
            }
            if (!ballPaused) {
                ballPosX += ballSpeedX;
                ballPosY += ballSpeedY;
            }
            else if (GetTime() - pauseStartTime >= pauseDuration) {
                ballPaused = false;
                ballSpeedX = (GetRandomValue(0, 1) == 0) ? -abs(ballSpeedX) : abs(ballSpeedX);
                ballSpeedY = (GetRandomValue(0, 1) == 0) ? -abs(ballSpeedY) : abs(ballSpeedY);
            }

            if ((ballPosY >= (screen_height - ballRadius)) || (ballPosY <= ballRadius)) ballSpeedY *= -1;

            // paddle 1 floor and ceiling collision
            if (paddle1Height + paddle1PosY >= screen_height) {
                //paddle1PosY -= (paddle1Height / 2) - 20;
                paddle1PosY -= 5;
            }
            if (paddle1PosY <= 0) {
                paddle1PosY *= 0;
            }

            // paddle 2 floor and ceiling collision
            if (paddle2Height + paddle2PosY >= screen_height) {
                paddle2PosY -= 5 ;
            }
            if (paddle2PosY <= 0) {
                paddle2PosY *= 0;
            }

            if (player1Score == 11) {
                currentScreen = GAME_OVER;
                winner = 1;
            }
            if (player2Score == 11) {
                currentScreen = GAME_OVER;
                winner = 2;
            }
        }
        else if (currentScreen == GAME_OVER) 
            {
            Vector2 mouse = GetMousePosition();
            bool hoveringOne = CheckCollisionPointRec(mouse, playAgain);
            bool hoveringTwo = CheckCollisionPointRec(mouse, quitButton);

            DrawText(winner == 1 ? "PLAYER 1 WINS" : "PLAYER 2 WINS", 200, 150, 56, WHITE);

            DrawRectangleRec(playAgain, hoveringOne ? LIGHTGRAY : GRAY);
            DrawRectangleLinesEx(playAgain, 2, WHITE);
            DrawText(playAgainButtonText, playAgain.x + 5, playAgain.y + 10, 25, WHITE);

            DrawRectangleRec(quitButton, hoveringTwo ? LIGHTGRAY : GRAY);
            DrawRectangleLinesEx(quitButton, 2, WHITE);
            DrawText(quitButtonText, quitButton.x + 55, quitButton.y + 10, 25, WHITE);

            if (hoveringOne && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                player1Score = 0;
                player2Score = 0;
                paddle1PosX = screen_width - 50;
                paddle1PosY = (screen_height / 2) - (paddle1Height / 2);
                paddle2PosX = 50;
                paddle2PosY = (screen_height / 2) - (paddle1Height / 2);
                ballPosX = screen_width / 2;
                ballPosY = screen_height / 2;
                currentScreen = TITLE;
            }
            else if (hoveringTwo && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                shouldClose = true;
            }
            
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
