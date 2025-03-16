#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "consts.h"

/*This is a class representing one Point on the screen, is is for handling collsions, moving and rendering the point on the scrren*/
class Point
{
private:
  /* data */
public:
  std::vector<float> pos; // is the center of the point; point has r = 1 pixel
  std::vector<float> velocity;
  std::vector<float> acceleration;
  float damping = 0.1; // how much energy is lost on collision
  RGBA color = { std::rand() % 256, std::rand() % 256,
    std::rand() % 256, 255 };

  Point(std::vector<float> p = { 0.0f ,0.0f },
    std::vector<float> v = { 0.0f, 0.0f },
    std::vector<float> a = { 0.0f, 0.0f });

  float getDinstance(Point p1, Point p2)
  {
    float d = 0;
    d = sqrt(pow((p1.pos[0]-p2.pos[0]), 2)+pow((p1.pos[1]-p2.pos[1]), 2));
    return d;
  }

  void checkEdges(int xConstraint, int yConstraint)
  {
    float leftEdge = 0 + 0.5f;
    float rightEdge = xConstraint - 0.5f;

    float topEdge = 0 + 0.5f;
    float botEdge = yConstraint - 0.5f;

    // horizontal constraint
    if (pos[0] < leftEdge)
    {
      velocity[0] *= -1 + damping;
      pos[0] = leftEdge;
    }
    else if (pos[0] > rightEdge)
    {
      velocity[0] *= -1 + damping;
      pos[0] = rightEdge;
    }
    // Vertical constraint
    if (pos[1] < topEdge)
    {
      velocity[1] *= -1 + damping;
      pos[1] = topEdge;
    }
    else if (pos[1] > botEdge)
    {
      velocity[1] *= -1 + damping;
      pos[1] = botEdge;
    }


  }

  void move(int xConstraint, int yConstraint)
  {
    for (size_t i = 0; i < velocity.size(); ++i) //.size() in case i want to switch to 3d later
    {
      checkEdges(xConstraint, yConstraint);
      velocity[i] += acceleration[i];
      pos[i] += velocity[i];

    }
  }
  void renderPoint(SDL_Renderer* renderer)
  {
    SDL_SetRenderDrawColor(renderer, color.r, color.g,
      color.b, color.a); // r,b,g,a
    SDL_RenderDrawPoint(renderer, (int)pos[0], (int)pos[1]);
  }

};


// Define constructor outside the class and without set values
Point::Point(std::vector<float> p, std::vector<float> v, std::vector<float> a)
{
  pos = p;
  velocity = v;
  acceleration = a;
}
