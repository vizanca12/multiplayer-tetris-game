#include <background.hpp>
#include <globals.hpp>
#include <cmath>
#include <ctime>

Background::Background(SDL_Renderer *renderer)
{
    this->renderer = renderer;
    startTime = SDL_GetTicks();
}

void Background::tick()
{
}

void Background::render()
{
    SDL_SetRenderDrawColor(renderer, 10, 15, 30, 255);
    SDL_Rect bgRect = {0, 0, WINDOW_WIDTH_MENU, WINDOW_HEIGHT_MENU};
    SDL_RenderFillRect(renderer, &bgRect);
    
    // Get elapsed time for wave animation
    Uint32 currentTime = SDL_GetTicks();
    float elapsed = (currentTime - startTime) / 1000.0f;
    
    // Grid properties
    const int gridSpacing = 40; // Distance between grid lines
    const int waveSpeed = 2; // Speed of wave propagation
    const float waveFrequency = 0.15f; // Frequency of the wave
    
    // Draw vertical lines with wave effect
    for (int x = 0; x < WINDOW_WIDTH_MENU; x += gridSpacing)
    {
        for (int y = 0; y < WINDOW_HEIGHT_MENU; y++)
        {
            // Calculate wave intensity based on position and time
            float wave = sin((y * waveFrequency) + (elapsed * waveSpeed)) * 0.5f + 0.5f;
            
            // Color fades based on wave (cyberpunk cyan/blue)
            int brightness = 70 + (int)(60 * wave);
            SDL_SetRenderDrawColor(renderer, brightness / 3, brightness / 2, brightness, 180);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    
    // Draw horizontal lines with wave effect
    for (int y = 0; y < WINDOW_HEIGHT_MENU; y += gridSpacing)
    {
        for (int x = 0; x < WINDOW_WIDTH_MENU; x++)
        {
            // Calculate wave intensity based on position and time (offset from vertical)
            float wave = sin((x * waveFrequency) + (elapsed * waveSpeed) + 1.57f) * 0.5f + 0.5f;
            
            // Color fades based on wave (cyberpunk cyan/blue)
            int brightness = 70 + (int)(60 * wave);
            SDL_SetRenderDrawColor(renderer, brightness / 3, brightness / 2, brightness, 180);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}
