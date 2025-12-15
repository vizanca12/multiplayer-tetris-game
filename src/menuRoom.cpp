#include <menuRoom.hpp>
#include <globals.hpp>
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

MenuRoom::MenuRoom()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH_MENU, WINDOW_HEIGHT_MENU, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Tetris");

    menuShowing = true;
    status = MENU_ROOM_WAITING;

    textSmaller = new Text(18);
    textSmall = new Text(24);
    textBig = new Text(60);

    bg = new Background(renderer);
}

void MenuRoom::runMainLoop()
{
    while (menuShowing)
    {
        input();
        render();
        tick();
    }

    if (status == MENU_ROOM_STARTING)
    {
        render();
        sleep(2);
    }
    close();
}

void MenuRoom::tick()
{
    bg->tick();
}

void MenuRoom::input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            menuShowing = false;
        if (event.key.state == SDL_PRESSED)
        {
            if (event.key.keysym.sym == SDLK_BACKSPACE)
                menuShowing = false;
        }
    }
}

void MenuRoom::render()
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    string welcomeText = "Welcome to";
    string tetris = "TETRIS";
    string msg1 = "Connected to server!";
    string msg2 = "Waiting for other players";
    string msg3 = "Players in the room: " + playersInTheRoom;

    if (status == MENU_ROOM_UNAVAILABLE)
    {
        msg1 = "Room unavailable...";
        msg2 = "Wait a while and try again!";
        msg3 = "-";
    }

    if (status == MENU_ROOM_STARTING)
    {
        msg1 = "Get ready!";
        msg2 = "Game will start soon...";
        msg3 = "-";
    }

    string backspace = "Press backspace to leave room";
    string madeWithLove = "Made by Vizanca e Edi";

    bg->render();

    textSmall->drawCenter(0, 50, WINDOW_WIDTH_MENU, 50, welcomeText, renderer);
    textBig->drawCenter(0, 100, WINDOW_WIDTH_MENU, 100, tetris, renderer);

    textSmall->drawCenter(0, 300, WINDOW_WIDTH_MENU, 300, msg1, renderer);
    textSmall->drawCenter(0, 350, WINDOW_WIDTH_MENU, 350, msg2, renderer);
    textSmall->drawCenter(0, 400, WINDOW_WIDTH_MENU, 400, msg3, renderer);

    textSmaller->drawCenter(0, 500, WINDOW_WIDTH_MENU, 500, backspace, renderer);
    textSmaller->drawCenter(0, 550, WINDOW_WIDTH_MENU, 550, madeWithLove, renderer);

    SDL_RenderPresent(renderer);
}

void MenuRoom::close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void MenuRoom::setStatus(int newStatus)
{
    status = newStatus;
    if (status == MENU_ROOM_STARTING)
        menuShowing = false;
}

int MenuRoom::getStatus()
{
    return status;
}

void MenuRoom::setPlayers(string newPlayers)
{
    playersInTheRoom = newPlayers;
}