// include Libs
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time()

// Inlcude Files
#include "Point.cpp"
#include "Object.cpp"


const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 900;
const int FPS = 60;
const int FRAME_DELAY = (1000 / FPS);


// Function to generate a random float between min and max
float randomFloat(float min, float max) {
  return min + static_cast<float>(std::rand()) / (2147483647.0 / (max - min));
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
  /*
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
     */
  std::vector<float> p;
  std::vector<float> v;// = { 1.0f, -10.0f };
  std::vector<float> a;// = { -0.0f, -0.0f };
  Point arr[NUMBEROFPOINTS];

  for (int i = 0; i < NUMBEROFPOINTS; ++i)
  {
    p = { static_cast<float>(rand() % (SCREEN_WIDTH - 1)),
      static_cast<float>(rand() % (SCREEN_HEIGHT - 1)) };
    v = { randomFloat(-5.0f, 5.0f), randomFloat(-5.0f, 5.0f) };
    a = { 0.0f, 0.5f };//{ randomFloat(-5.0f, 5.0f), randomFloat(-5.0f, 5.0f) };
    arr[i] = Point(p, v, a);
  }

  for (int i = 0; i < NUMBEROFPOINTS; ++i)
  {
    std::cout << "Point number : " << i;
    for (int j = 0; j < 2; ++j)
    {
      std::cout << " a: " << arr[i].acceleration[j] << " v: " << arr[i].velocity[j] << " pos: " << j << " " << arr[i].pos[j];
    }
    std::cout << std::endl;
  }


  bool running = true;
  SDL_Event event;

  RGBA bgcolor = { 27, 27, 27, 255 };

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
    SDL_SetRenderDrawColor(renderer, bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
    SDL_RenderClear(renderer);
    /*
    for (auto& obj : arr)
    {
      obj.rectCollisions(arr, NumberOjObjects);
      obj.move(SCREEN_HEIGHT, SCREEN_WIDTH);
      obj.renderObject(renderer);
    }
      */
    for (int i = 0; i < NUMBEROFPOINTS; ++i)
    {
      arr[i].collision(arr, NUMBEROFPOINTS);
      arr[i].applyGravity(arr, NUMBEROFPOINTS);
      arr[i].move(SCREEN_HEIGHT, SCREEN_WIDTH);
      arr[i].renderPoint(renderer);
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