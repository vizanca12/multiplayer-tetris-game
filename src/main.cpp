#include <iostream>
#include <SDL2/SDL.h>
#include <array>
#include <color.hpp>
#include <pos.hpp>
#include <tetrisMap.hpp>
#include <tetrimino.hpp>
#include <client.hpp>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include <menu.hpp>
#include <menuRoom.hpp>
#include <mutex>
#include "results.hpp"
#include "tetrisAI.hpp"


using namespace std;

bool windowOpen;
mutex mx;

void render(TetrisMap *tetrisMap, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    tetrisMap->draw(renderer);
    SDL_RenderPresent(renderer);
}

void physics(TetrisMap *tetrisMap)
{
    tetrisMap->tick();
}

void input(TetrisMap *tetrisMap)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            windowOpen = false;
        if (event.key.state == SDL_PRESSED)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_z:
                tetrisMap->tetriminoAction(ROTATE_LEFT);
                break;
            case SDLK_x:
                tetrisMap->tetriminoAction(ROTATE_RIGHT);
                break;
            case SDLK_c:
                tetrisMap->changeHold();
                break;
            case SDLK_LEFT:
                tetrisMap->tetriminoAction(MOVE_LEFT);
                break;
            case SDLK_RIGHT:
                tetrisMap->tetriminoAction(MOVE_RIGHT);
                break;
            case SDLK_UP:
                tetrisMap->tetriminoAction(ROTATE_RIGHT);
                break;
            case SDLK_DOWN:
                tetrisMap->tetriminoAction(MOVE_DOWN);
                break;
            case SDLK_SPACE:
                tetrisMap->tetriminoAction(DROP);
                break;
            }
        }
    }
}

void handleClient(Client *client, TetrisMap *tetrisMap)
{
    bool multiplayerGameOver = false;
    int mapSize = MATRIX_WIDTH * MATRIX_HEIGHT;

    // Main loop while the player is alive
    while (!tetrisMap->isGameOver() && !multiplayerGameOver && windowOpen)
    {
        usleep(50000); // Sleep for 50ms

        /* Send map */
        char code = CODE_PLAYER_MAP;
        char my_map[mapSize];
        tetrisMap->getMap(my_map);
        
        // Check if sending succeeded. If it fails, disconnect.
        if (client->send(&code, 1) <= 0) { multiplayerGameOver = true; break; }
        if (client->send(my_map, mapSize) <= 0) { multiplayerGameOver = true; break; }

        /* Send buffer lines*/
        int bufferLines = tetrisMap->getBufferLines();
        if (bufferLines < 0)
        {
            char buf = (char)(-bufferLines);
            code = CODE_PLAYER_LINES;
            client->send(&code, 1);
            client->send(&buf, 1);
            tetrisMap->resetBufferLines();
        }

        // --- FIX HERE: Check if the server sent something or closed ---
        // If recv returns <= 0, the server crashed/closed. We treat it as Game Over.
        int bytes_read = client->recv(&code, 1);
        
        if (bytes_read <= 0) 
        {
            multiplayerGameOver = true;
            break; 
        }

        // If read successfully, process the code
        if (code == CODE_PLAYER_MAP)
        {
            // For 4-player logic (Broadcast), add ID reading here if implemented
            // client->recv(&id, 1); 
            client->recv(tetrisMap->enemyMap(), mapSize);
        }
        else if (code == CODE_PLAYER_LINES)
        {
            char buf = 0;
            client->recv(&buf, 1);
            bufferLines = (int)buf;
            tetrisMap->addBufferLines(bufferLines);
        }
        else if (code == CODE_GAME_OVER)
        {
            multiplayerGameOver = true;
        }
    }

    if (multiplayerGameOver)
        cout << "Game Over! (Winner or Connection Closed)" << endl;
    else if (tetrisMap->isGameOver())
    {
        char code = CODE_PLAYER_DEAD;
        client->send(&code, 1);
        cout << "You lost, keep trying!" << endl;
    }

    // Spectator loop (when you die but the game continues)
    while (!multiplayerGameOver && windowOpen)
    {
        usleep(50000); // Sleep for 50ms
        char code = CODE_PLAYER_MAP;
        
        // --- FIX HERE TOO ---
        int res = client->recv(&code, 1);
        
        if (res <= 0) // Server closed the connection
        {
            multiplayerGameOver = true;
            break;
        }

        if (code == CODE_PLAYER_MAP)
            client->recv(tetrisMap->enemyMap(), mapSize);
        else if (code == CODE_GAME_OVER)
            multiplayerGameOver = true;
    }

    // DON'T close the window here if you want to see the results!
    // windowOpen = false; <--- REMOVE or COMMENT THIS LINE if it exists at the end
    
    // Just signal that the graphics loop should stop, but let main create the Results screen
    windowOpen = false; 
}

void runTetrisMultiplayer(Client *client)
{
    TetrisMap *tetrisMap = new TetrisMap(true);
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH_MULTIPLAYER, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Tetris");
    windowOpen = true;

    thread t_client(handleClient, client, tetrisMap);

    while (windowOpen)
    {
        input(tetrisMap);
        physics(tetrisMap);
        render(tetrisMap, renderer);
    }

    t_client.join();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    int score, linesCleared, level;
    tetrisMap->getGameStatus(&score, &linesCleared, &level);
    Results results(score, linesCleared, !tetrisMap->isGameOver());
}

void runTetrisSingleplayer()
{
    TetrisMap *tetrisMap = new TetrisMap(false);
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Tetris");
    windowOpen = true;

    while (!tetrisMap->isGameOver() && windowOpen)
    {
        input(tetrisMap);
        physics(tetrisMap);
        render(tetrisMap, renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    int score, linesCleared, level;
    tetrisMap->getGameStatus(&score, &linesCleared, &level);
    Results results(score, linesCleared, !tetrisMap->isGameOver());
}

void handleClientRoom(MenuRoom *menuRoom, Client *client)
{
    char code;
    client->recv(&code, 1);
    if (code != CODE_ROOM_UNAVAILABLE)
    {
        while (code == CODE_ROOM_SIZE)
        {
            char frac[3];
            client->recv(frac, 3);
            string playersInRoom(frac);
            menuRoom->setPlayers(frac);
            client->recv(&code, 1);
        }
        if (code == CODE_START_GAME)
        {
            menuRoom->setStatus(MENU_ROOM_STARTING);
        }
    }
    else
        menuRoom->setStatus(MENU_ROOM_UNAVAILABLE);
}

void renderVsAI(TetrisMap *playerMap, TetrisMap *aiMap, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    playerMap->draw(renderer);
    
    char aiMapData[MATRIX_WIDTH * MATRIX_HEIGHT];
    aiMap->getMap(aiMapData);
    
    char *enemyMap = playerMap->enemyMap();
    for (int i = 0; i < MATRIX_WIDTH * MATRIX_HEIGHT; i++)
    {
        enemyMap[i] = aiMapData[i];
    }
    
    SDL_RenderPresent(renderer);
}

void inputVsAI(TetrisMap *tetrisMap, bool *running)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            *running = false;
            windowOpen = false;
        }
        if (event.key.state == SDL_PRESSED)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_z:
                tetrisMap->tetriminoAction(ROTATE_LEFT);
                break;
            case SDLK_x:
                tetrisMap->tetriminoAction(ROTATE_RIGHT);
                break;
            case SDLK_c:
                tetrisMap->changeHold();
                break;
            case SDLK_LEFT:
                tetrisMap->tetriminoAction(MOVE_LEFT);
                break;
            case SDLK_RIGHT:
                tetrisMap->tetriminoAction(MOVE_RIGHT);
                break;
            case SDLK_UP:
                tetrisMap->tetriminoAction(ROTATE_RIGHT);
                break;
            case SDLK_DOWN:
                tetrisMap->tetriminoAction(MOVE_DOWN);
                break;
            case SDLK_SPACE:
                tetrisMap->tetriminoAction(DROP);
                break;
            case SDLK_ESCAPE:
                *running = false;
                windowOpen = false;
                break;
            }
        }
    }
}

void handleAIGame(TetrisMap *playerMap, TetrisMap *aiMap, TetrisAI *ai, bool *running)
{
    while (*running && !playerMap->isGameOver() && !aiMap->isGameOver())
    {
        usleep(16000); // ~60 FPS tick rate for AI

        // AI makes its move
        ai->makeMove();
        aiMap->tick();

        // Exchange lines between player and AI
        int playerLines = playerMap->getBufferLines();
        if (playerLines < 0)
        {
            aiMap->addBufferLines(-playerLines);
            playerMap->resetBufferLines();
        }

        int aiLines = aiMap->getBufferLines();
        if (aiLines < 0)
        {
            playerMap->addBufferLines(-aiLines);
            aiMap->resetBufferLines();
        }
    }

    *running = false;
}

void runTetrisVsAI()
{
    // Create both maps - player and AI
    TetrisMap *playerMap = new TetrisMap(true);  // extended for showing enemy
    TetrisMap *aiMap = new TetrisMap(false);
    TetrisAI *ai = new TetrisAI(aiMap);
    ai->setSpeed(200); // AI move delay in ms

    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH_MULTIPLAYER, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Tetris - VS IA");
    windowOpen = true;

    bool running = true;

    while (running && windowOpen && !playerMap->isGameOver() && !aiMap->isGameOver())
    {
        // Handle player input
        inputVsAI(playerMap, &running);
        
        // Update player
        playerMap->tick();
        
        // Update AI - runs in same thread for safety
        ai->makeMove();
        aiMap->tick();

        // Exchange lines between player and AI
        int playerLines = playerMap->getBufferLines();
        if (playerLines < 0)
        {
            aiMap->addBufferLines(-playerLines);
            playerMap->resetBufferLines();
        }

        int aiLines = aiMap->getBufferLines();
        if (aiLines < 0)
        {
            playerMap->addBufferLines(-aiLines);
            aiMap->resetBufferLines();
        }
        
        // Render both maps
        renderVsAI(playerMap, aiMap, renderer);
        
        // Small delay to control frame rate
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // Show results
    int score, linesCleared, level;
    playerMap->getGameStatus(&score, &linesCleared, &level);
    
    bool playerWon = aiMap->isGameOver() && !playerMap->isGameOver();
    Results results(score, linesCleared, playerWon);

    delete playerMap;
    delete aiMap;
    delete ai;
}

// Input function that controls TWO players
void inputLocalMultiplayer(TetrisMap *p1, TetrisMap *p2)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            windowOpen = false;
        
        if (event.key.state == SDL_PRESSED)
        {
            switch (event.key.keysym.sym)
            {
            // --- PLAYER 1 CONTROLS (WASD + Space + C) ---
            case SDLK_w:
                p1->tetriminoAction(ROTATE_RIGHT);
                break;
            case SDLK_a:
                p1->tetriminoAction(MOVE_LEFT);
                break;
            case SDLK_d:
                p1->tetriminoAction(MOVE_RIGHT);
                break;
            case SDLK_s:
                p1->tetriminoAction(MOVE_DOWN);
                break;
            case SDLK_SPACE:
                p1->tetriminoAction(DROP);
                break;
            case SDLK_c:
                p1->changeHold();
                break;

            // --- PLAYER 2 CONTROLS (Arrows + Enter + Shift) ---
            case SDLK_UP:
                p2->tetriminoAction(ROTATE_RIGHT);
                break;
            case SDLK_LEFT:
                p2->tetriminoAction(MOVE_LEFT);
                break;
            case SDLK_RIGHT:
                p2->tetriminoAction(MOVE_RIGHT);
                break;
            case SDLK_DOWN:
                p2->tetriminoAction(MOVE_DOWN);
                break;
            case SDLK_RETURN: // Enter
                p2->tetriminoAction(DROP);
                break;
            case SDLK_RSHIFT: // Shift Direito
                p2->changeHold();
                break;

            case SDLK_ESCAPE:
                windowOpen = false;
                break;
            }
        }
    }
}

void runTetrisLocalMultiplayer()
{
    // Create two maps
    TetrisMap *p1Map = new TetrisMap(false); // Player 1 (Left)
    TetrisMap *p2Map = new TetrisMap(false); // Player 2 (Right)

   int totalWidth = 1500;

    // Configure the visual offset of Player 2
    // We use TETRIS_ENEMY_MAP_INIT_X (which should already exist in your globals) or calculate a value
    // Assuming TETRIS_ENEMY_MAP_INIT_X is where the enemy map is in online mode
    // If you don't have this const, use something like 500 (pixels)
    // Map configurations...
    p1Map->setRenderOffset(0);
    p1Map->setMirrorLayout(false);
    p2Map->setRenderOffset(720); // Pushed a bit more to the right
    p2Map->setMirrorLayout(true);// Adjust this value 450 to separate the boards

    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_EVERYTHING);
    // Janela mais larga para caber os dois
    SDL_CreateWindowAndRenderer(totalWidth, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Tetris - Local Multiplayer");
    windowOpen = true;

    string winnerName = "";

    while (windowOpen)
    {
        // 1. Inputs for both players
        inputLocalMultiplayer(p1Map, p2Map);

        // 2. Physics / Ticks
        p1Map->tick();
        p2Map->tick();

        // 3. Garbage Lines Exchange
        // If P1 made attack lines (negative buffer), send to P2
        int p1Attack = p1Map->getBufferLines();
        if (p1Attack < 0) {
            p2Map->addBufferLines(-p1Attack); // Adiciona linhas no P2
            p1Map->resetBufferLines();
        }

        // If P2 made attack lines
        int p2Attack = p2Map->getBufferLines();
        if (p2Attack < 0) {
            p1Map->addBufferLines(-p2Attack); // Add lines to P1
            p2Map->resetBufferLines();
        }

        // 4. Win/Loss Check
        if (p1Map->isGameOver()) {
            winnerName = "Player 2 Wins!";
            windowOpen = false;
        } else if (p2Map->isGameOver()) {
            winnerName = "Player 1 Wins!";
            windowOpen = false;
        }

        // 5. Rendering
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        p1Map->draw(renderer);
        p2Map->draw(renderer);

        SDL_RenderPresent(renderer);
        
        SDL_Delay(16); // ~60 FPS cap
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    // If not closed by the window X, show result
    if (winnerName != "") {
        Results results(winnerName, "Congratulations!");
    }

    delete p1Map;
    delete p2Map;
}

int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));
    const char *hostname = argv[1];
    int option = 0;
    while (option != 4)
    {
        Menu menu;

        if (!menu.hasChosen())
            break;

        option = menu.getChoice();

        /* Tetris singleplayer mode! */
        if (option == 0)
            runTetrisSingleplayer();

        /* Tetris multiplayer option */
        if (option == 1)
        {
            Client *client = new Client(hostname, SERVER_PORT);

            if (client->is_connected())
            {
                MenuRoom *menuRoom = new MenuRoom();

                thread t1(handleClientRoom, menuRoom, client);
                t1.detach();
                menuRoom->runMainLoop();

                if (menuRoom->getStatus() == MENU_ROOM_STARTING)
                {
                    runTetrisMultiplayer(client);
                    client->disconnect();
                }
                else
                {
                    char code = CODE_PLAYER_DISCONNECT;
                    client->send(&code, 1);
                    // client->disconnect();
                }
            }
        }

        if (option == 2)
        {
            runTetrisVsAI();
        }

        if(option == 3)
        {
            runTetrisLocalMultiplayer();
        }
    }

    return 0;
}
