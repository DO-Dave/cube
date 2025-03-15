#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time()
#include "Cube.cpp"

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 900;
const int FPS = 60;
const int FRAME_DELAY = 0* (1000 / FPS);

struct Coordinates
{
  int x;
  int y;
};

// Correct function declaration
void checkEdges(Coordinates& pos);

// Function to generate a random float between min and max
float randomFloat(float min, float max) {
  return min + static_cast<float>(std::rand()) / (static_cast<float>(2147483647 / (max - min)));
}

int main(int argc, char* argv[])
{
  // Seed the random number generator
  std::srand(static_cast<unsigned int>(std::time(0)));

  // Initialize SDL video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  // Create an SDL window centered on the screen
  SDL_Window* window = SDL_CreateWindow("SDL2 Basics",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN);
  if (!window)
  {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  // Create a renderer with hardware acceleration
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
  {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  // Initial SDL_Rect
  SDL_Rect r1 = { 500, 500, 50, 50 };
  const int NumberOjObjects = 50;
  Object arr[NumberOjObjects]; // Array of 5 objects
  // Initialize different velocity and acceleration for each object
  for (int i = 0; i < NumberOjObjects; ++i)
  {
    // Generate random velocity and acceleration
    std::vector<float> velocity = { randomFloat(-5.0f, 5.0f), randomFloat(-5.0f, 5.0f) };
    std::vector<float> acceleration = {0.0f, 1.0f};//{ randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f) };
    SDL_Rect r1 = { rand() % (SCREEN_WIDTH - 50), rand() % (SCREEN_HEIGHT - 50), 10, 10 };
    // Initialize each Object with different velocity and acceleration
    arr[i] = Object(r1, velocity, acceleration);
  }

  bool running = true;
  SDL_Event event;

  while (running)
  {
    // Get the starting tick for the current frame
    Uint32 frameStart = SDL_GetTicks();

    // Handle events
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = false;
      }
    }

    // Set the background color (e.g., dark gray) and clear the screen
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    for (auto& obj : arr)
    {
      obj.rectCollisions(arr, NumberOjObjects);
      obj.move(SCREEN_HEIGHT, SCREEN_WIDTH);
      obj.renderObject(renderer);
    }

    // Present the current frame
    SDL_RenderPresent(renderer);

    // Frame rate control: delay to cap FPS at 1000/fps
    Uint32 frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < FRAME_DELAY)
    {
      SDL_Delay(FRAME_DELAY - frameTime);
    }
  }

  // Clean up resources
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}