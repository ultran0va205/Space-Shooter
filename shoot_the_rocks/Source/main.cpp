//Aayush Sarcar

#include "raylib.h"
#include "Game.h"
#include "Resources.h"
#include <stack>
#include "myMath.h"

enum class State
{
    MAIN_MENU,
    GAME,
};

void guiLabel(Rectangle rect, const char* label_text)
{
    Vector2i textSize = { MeasureText(label_text, 40), 40 };
    Vector2i textTopLeft = (Vector2i{ static_cast<int>(rect.x),static_cast<int>(rect.y) } +
        (Vector2i{ static_cast<int>(rect.width),static_cast<int>(rect.height) } - textSize) / 2);

    DrawText(label_text, textTopLeft.x, textTopLeft.y, 40, WHITE);
}

bool guiButton(Rectangle rect, const char* button_text)
{
    bool isClicked = false;

    Color backgroundColour = Color{ 64, 64, 64, 255 };

    if (CheckCollisionPointRec(GetMousePosition(), rect))
    {
        backgroundColour = Color{ 96, 96, 96, 255 };

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            backgroundColour = Color{ 32, 32, 32, 255 };
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            isClicked = true;
        }
    }

    DrawRectangle(rect.x, rect.y, rect.width, rect.height, backgroundColour);
    guiLabel(rect, button_text);

    return isClicked;
}

void MainMenuFrame(std::stack<State>& states, bool& gameIsRunning)
{
    const Vector2i buttonSize = Vector2i(300, 64);
    const int buttonMarginY = 10;
    const int buttonCount = 3;

    Rectangle buttonRect;
    buttonRect.x = (GetRenderWidth() - buttonSize.x) / 2;
    buttonRect.y = (GetRenderHeight() - (buttonSize.y * buttonCount) - (buttonCount - 1) * buttonMarginY) / 2;
    buttonRect.width = buttonSize.x;
    buttonRect.height = buttonSize.y;

    if (guiButton(buttonRect, "PLAY"))
    {
        states.push(State::GAME);
    }
    buttonRect.y += buttonSize.y + buttonMarginY;

    if (guiButton(buttonRect, "EXIT"))
    {
        gameIsRunning = false;
    }
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
    InitAudioDevice();

    Game game;
    game.SpawnSpaceship();

    ResourceManager::LoadResources();
    std::stack<State> states;
    states.push(State::MAIN_MENU);
    bool gameIsRunning = true;

    while (gameIsRunning)
    {
        if (WindowShouldClose())
        {
            gameIsRunning = false;
        }
        State mainMenu = states.top();
        switch (mainMenu)
        {
        case State::MAIN_MENU:
            BeginDrawing();
            ClearBackground(BLACK);
            MainMenuFrame(states, gameIsRunning);
            EndDrawing();
            break;

        case State::GAME:
            BeginDrawing();
            ClearBackground(BLACK);
            game.Render(ResourceManager::textures);
            game.Update();
            DrawText(TextFormat("%i", game.score), GetScreenWidth() / 2, 10, 40, SKYBLUE);
            EndDrawing();
            if (IsKeyPressed(KEY_BACKSPACE) || game.isDead == true)
            {
                states.pop();
                game.ResetGame();
            }
            break;
        }
    }
    ResourceManager::UnloadResources();
    CloseAudioDevice();
    CloseWindow();        
    return 0;
}

