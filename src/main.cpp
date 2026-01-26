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

    // Loop principal enquanto o jogador está vivo
    while (!tetrisMap->isGameOver() && !multiplayerGameOver && windowOpen)
    {
        usleep(50000); // Sleep for 50ms

        /* Send map */
        char code = CODE_PLAYER_MAP;
        char my_map[mapSize];
        tetrisMap->getMap(my_map);
        
        // Verifica se conseguiu enviar. Se falhar, desconecta.
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

        // --- CORREÇÃO AQUI: Verificar se o servidor mandou algo ou fechou ---
        // Se recv retornar <= 0, o servidor caiu/fechou. Tratamos como Fim de Jogo.
        int bytes_read = client->recv(&code, 1);
        
        if (bytes_read <= 0) 
        {
            multiplayerGameOver = true;
            break; 
        }

        // Se leu com sucesso, processa o código
        if (code == CODE_PLAYER_MAP)
        {
            // Para lógica de 4 jogadores (Broadcast), adicione a leitura do ID aqui se tiver implementado
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

    // Loop de espectador (quando você morre mas o jogo continua)
    while (!multiplayerGameOver && windowOpen)
    {
        usleep(50000); // Sleep for 50ms
        char code = CODE_PLAYER_MAP;
        
        // --- CORREÇÃO AQUI TAMBÉM ---
        int res = client->recv(&code, 1);
        
        if (res <= 0) // Servidor fechou a conexão
        {
            multiplayerGameOver = true;
            break;
        }

        if (code == CODE_PLAYER_MAP)
            client->recv(tetrisMap->enemyMap(), mapSize);
        else if (code == CODE_GAME_OVER)
            multiplayerGameOver = true;
    }

    // NÃO feche a janela aqui se quiser ver os resultados!
    // windowOpen = false; <--- REMOVA ou COMENTE ESTA LINHA se ela existir no final
    
    // Apenas sinalize que o loop gráfico deve parar, mas deixe o main criar a tela de Results
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
    
    // Draw player map
    playerMap->draw(renderer);
    
    // Get AI map and set it as enemy map for display
    char aiMapData[MATRIX_WIDTH * MATRIX_HEIGHT];
    aiMap->getMap(aiMapData);
    
    // Copy AI map to player's enemy map for rendering
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

// Função de Input que controla DOIS jogadores
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
            // --- CONTROLES DO JOGADOR 1 (WASD + Space + C) ---
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

            // --- CONTROLES DO JOGADOR 2 (Setas + Enter + Shift) ---
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
    // Cria dois mapas
    TetrisMap *p1Map = new TetrisMap(false); // Player 1 (Esquerda)
    TetrisMap *p2Map = new TetrisMap(false); // Player 2 (Direita)

   int totalWidth = 1500;

    // Configura o deslocamento visual do Player 2
    // Usamos TETRIS_ENEMY_MAP_INIT_X (que já deve existir nos seus globals) ou calculamos um valor
    // Supondo que TETRIS_ENEMY_MAP_INIT_X seja onde o mapa do inimigo fica no online
    // Se não tiver essa const, use algo como 500 (pixels)
    // Configurações dos mapas...
    p1Map->setRenderOffset(0);
    p1Map->setMirrorLayout(false);
    p2Map->setRenderOffset(720); // Empurrei um pouco mais para a direita
    p2Map->setMirrorLayout(true);// Ajuste esse valor 450 para afastar os tabuleiros

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
        // 1. Inputs para ambos
        inputLocalMultiplayer(p1Map, p2Map);

        // 2. Física / Ticks
        p1Map->tick();
        p2Map->tick();

        // 3. Troca de Lixo (Garbage Lines)
        // Se P1 fez linhas de ataque (buffer negativo), manda pro P2
        int p1Attack = p1Map->getBufferLines();
        if (p1Attack < 0) {
            p2Map->addBufferLines(-p1Attack); // Adiciona linhas no P2
            p1Map->resetBufferLines();
        }

        // Se P2 fez linhas de ataque
        int p2Attack = p2Map->getBufferLines();
        if (p2Attack < 0) {
            p1Map->addBufferLines(-p2Attack); // Adiciona linhas no P1
            p2Map->resetBufferLines();
        }

        // 4. Verificação de Vitória/Derrota
        if (p1Map->isGameOver()) {
            winnerName = "Player 2 Wins!";
            windowOpen = false;
        } else if (p2Map->isGameOver()) {
            winnerName = "Player 1 Wins!";
            windowOpen = false;
        }

        // 5. Renderização
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
    
    // Se não fechou pelo X da janela, mostra resultado
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
