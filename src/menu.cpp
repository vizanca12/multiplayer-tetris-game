#include <menu.hpp>
#include <globals.hpp>

#define MAX_CHOICES 4

Menu::Menu()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH_MENU, WINDOW_HEIGHT_MENU, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Tetris");

    choice = 0;
    hasMadeChoice = false;
    menuShowing = true;

    textSmaller = new Text(18);
    textSmall = new Text(24);
    textBig = new Text(60);

    bg = new Background(renderer);

    while (menuShowing)
    {
        input();
        render();
        bg->tick();
    }

    close();
}

void Menu::input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            menuShowing = false;
        if (event.key.state == SDL_PRESSED)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                choice = choice - 1;
                break;
            case SDLK_DOWN:
                choice = choice + 1;
                break;
            case SDLK_RETURN:
            {
                hasMadeChoice = true;
                menuShowing = false;
            }
            break;
            }

            choice = choice < 0 ? choice + MAX_CHOICES : choice % MAX_CHOICES;
        }
    }
}

void Menu::render()
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    SDL_SetRenderDrawColor(renderer, 15, 23, 42, 255);
    SDL_RenderClear(renderer);

    string welcomeText = "Welcome to";
    string tetris = "TETRIS";
    string singleplayer = "1. Singleplayer";
    string multiplayer = "2. Multiplayer";
    string vsIA = "3. Vs IA";
    string quit = "4. Quit";
    string str_choice = ">";
    string madeWithLove = "Made by Vizanca e Edi";

    bg->render();

    textSmall->drawCenter(0, 50, WINDOW_WIDTH_MENU, 50, welcomeText, renderer);
    textBig->drawCenter(0, 100, WINDOW_WIDTH_MENU, 100, tetris, renderer);
    

    textSmall->drawStart(50, choice * 50 + 285, str_choice, renderer);
    textSmall->drawCenter(0, 300, WINDOW_WIDTH_MENU, 300, singleplayer, renderer);
    textSmall->drawCenter(0, 350, WINDOW_WIDTH_MENU, 350, multiplayer, renderer);
    textSmall->drawCenter(0, 400, WINDOW_WIDTH_MENU, 400, vsIA, renderer);
    textSmall->drawCenter(0, 450, WINDOW_WIDTH_MENU, 450, quit, renderer);

    textSmaller->drawCenter(0, 550, WINDOW_WIDTH_MENU, 550, madeWithLove, renderer);

    SDL_RenderPresent(renderer);
}

void Menu::close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int Menu::getChoice()
{
    return choice;
}

bool Menu::hasChosen()
{
    return hasMadeChoice;
}
