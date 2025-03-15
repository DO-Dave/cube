#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

struct RGBA
{
  int r;
  int g;
  int b;
  int a;
};

class Object
{
private:
public:
  std::vector<float> pos;
  std::vector<float> velocity;
  std::vector<float> acceleration;
  int width;
  int height;
  RGBA color{ std::rand() % 256, std::rand() % 256,
    std::rand() % 256, 255};
  SDL_Rect rect;
  float damping = 0.1;

  // Constructor declaration (without 'Object::' qualification)
  Object(SDL_Rect rect = { 0, 0, 10, 10 }, std::vector<float> v = { 0.0f, 0.0f }, std::vector<float> a = { 0.0f, 0.0f });

  SDL_Rect setRect()
  {
    return SDL_Rect{ (int)pos[0], (int)pos[1], width, height };
  }

  void checkEdges(int xConstraint, int yConstraint)
  {
    if (pos[0] < 0)
    {
      velocity[0] *= -1 + damping;
      pos[0] = 0;
    }
    else if (pos[0] + width > xConstraint)
    {
      velocity[0] *= -1 + damping;
      pos[0] = xConstraint - width;
    }
    if (pos[1] < 0)
    {
      velocity[1] *= -1 + damping;
      pos[1] = 0;
    }
    else if (pos[1] + width > yConstraint)
    {
      velocity[1] *= -1 + damping;
      pos[1] = yConstraint - width;
    };
  }

  void move(int xConstraint, int yConstraint)
  {

    for (size_t i = 0; i < velocity.size(); i++)
    {
      checkEdges(xConstraint, yConstraint);
      velocity[i] += acceleration[i];
      pos[i] += velocity[i];
    }
  }

  void renderObject(SDL_Renderer* renderer)
  {
    SDL_Rect rect = setRect();
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // r,b,g,a
    SDL_RenderFillRect(renderer, &rect);
  }

  bool checkOverlap(Object o1, Object o2)
  {
    return (o1.pos[0] + o1.width >= o2.pos[0] &&
      o1.pos[0] <= o2.pos[0] + o2.width &&
      o1.pos[1] + o1.height >= o2.pos[1] &&
      o1.pos[1] <= o2.pos[1] + o2.height);
  }

  void resolveCollision(Object& o1, Object& o2)
  {
    float overlapX = std::min(o1.pos[0] + o1.width - o2.pos[0], o2.pos[0] + o2.width - o1.pos[0]);
    float overlapY = std::min(o1.pos[1] + o1.height - o2.pos[1], o2.pos[1] + o2.height - o1.pos[1]);

    if (overlapX < overlapY) {
      if (o1.pos[0] < o2.pos[0]) {
        o1.pos[0] -= overlapX;
        o2.pos[0] += overlapX;
      }
      else {
        o1.pos[0] += overlapX;
        o2.pos[0] -= overlapX;
      }
    }
    else {
      if (o1.pos[1] < o2.pos[1]) {
        o1.pos[1] -= overlapY;
        o2.pos[1] += overlapY;
      }
      else {
        o1.pos[1] += overlapY;
        o2.pos[1] -= overlapY;
      }
    }

    // Apply damping to velocities
    for (int index = 0; index < 2; ++index) {
      o1.velocity[index] *= -1 + o1.damping;
      o2.velocity[index] *= -1 + o2.damping;
    }
  }

  void rectCollisions(Object arr[], int size)
  {
    for (int i = 0; i < size; ++i)
    {
      for (int j = i + 1; j < size; ++j)
      {
        // If overlap detected
        if (checkOverlap(arr[i], arr[j]))
        {
          resolveCollision(arr[i], arr[j]);
        }
      }
    }
  }

};

// Constructor definition outside the class (without default arguments)
Object::Object(SDL_Rect rect, std::vector<float> v, std::vector<float> a)
{
  height = rect.h;
  width = rect.w;
  pos.push_back(rect.x);
  pos.push_back(rect.y);
  velocity = v;
  acceleration = a;
}