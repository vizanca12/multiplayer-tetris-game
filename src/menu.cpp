#include <menu.hpp>
#include <globals.hpp>

// AUMENTADO PARA 5 OPÇÕES
#define MAX_CHOICES 5

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

            // Garante que o cursor rode entre 0 e 4
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
    
    // Novas strings de menu
    string singleplayer = "1. Singleplayer";
    string multiplayerOnline = "2. Multiplayer Online";
    string multiplayerLocal = "3. vs Bot"; // Nova opção
    string vsIA = "4. Local Multiplayer"; // Alterada para 4
    string quit = "5. Quit";
    
    string str_choice = ">";
    string madeWithLove = "Made by Vizanca e Edi";

    bg->render();

    // Título
    textSmall->drawCenter(0, 40, WINDOW_WIDTH_MENU, 40, welcomeText, renderer);
    textBig->drawCenter(0, 80, WINDOW_WIDTH_MENU, 80, tetris, renderer);
    
    // Lógica do cursor:
    // Começamos a desenhar as opções no Y = 240. 
    // O cursor precisa acompanhar. O offset +225 alinha com a nova posição inicial.
    textSmall->drawStart(50, choice * 50 + 225, str_choice, renderer);

    // Desenhando as opções (Espaçamento de 50px entre elas)
    textSmall->drawCenter(0, 240, WINDOW_WIDTH_MENU, 240, singleplayer, renderer);
    textSmall->drawCenter(0, 290, WINDOW_WIDTH_MENU, 290, multiplayerOnline, renderer);
    textSmall->drawCenter(0, 340, WINDOW_WIDTH_MENU, 340, multiplayerLocal, renderer);
    textSmall->drawCenter(0, 390, WINDOW_WIDTH_MENU, 390, vsIA, renderer);
    textSmall->drawCenter(0, 440, WINDOW_WIDTH_MENU, 440, quit, renderer);

    // Rodapé
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