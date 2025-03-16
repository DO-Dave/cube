#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "consts.h"

/* This is a class representing one Point on the screen, it is for handling collisions, moving, and rendering the point on the screen */
class Point
{
public:
  std::vector<float> pos;  // The center of the point; point has r = 1 pixel
  std::vector<float> velocity;
  std::vector<float> acceleration;

  int cellID = -1;

  RGBA color = { std::rand() % 256, std::rand() % 256, std::rand() % 256, 255 };

  RGBA color2 = { 255, 255, 255, 255 };

  Point(std::vector<float> p = { 0.0f ,0.0f }, std::vector<float> v = { 0.0f, 0.0f }, std::vector<float> a = { 0.0f, 0.0f });

  // Returns the distance between two points
  float getDistance(const Point& p1, const Point& p2)
  {
    float dx = p1.pos[0] - p2.pos[0];
    float dy = p1.pos[1] - p2.pos[1];
    return std::sqrt(dx * dx + dy * dy);
  }

  // Handles collision by calculating overlap and adjusting velocities
  void checkCollision(Point& p1, Point& p2)
  {
    float distance = getDistance(p1, p2);
    float overlap = 2.0f - distance; // The points have a radius of 1.0f, so the diameter is 2.0f
    //std::cout << "check collission got fucked in the ass" << std::endl;
    if (overlap > 0)
    {
      // std::cout << "moved some shit" << std::endl;
       // Calculate the direction of the overlap
      std::vector<float> direction = { p1.pos[0] - p2.pos[0], p1.pos[1] - p2.pos[1] };

      // Normalize the direction
      float length = std::sqrt(direction[0] * direction[0] + direction[1] * direction[1]);
      direction[0] /= length;
      direction[1] /= length;

      // Move each point back by half the overlap distance
// Move each point back by half the overlap distance
      p1.pos[0] += direction[0] * (overlap / 2.0f);
      p1.pos[1] += direction[1] * (overlap / 2.0f);
      p2.pos[0] -= direction[0] * (overlap / 2.0f);
      p2.pos[1] -= direction[1] * (overlap / 2.0f);

      // Reverse velocities and apply damping factor
      p1.velocity[0] *= -1.0f + DAMPING_FACTOR;  // Reverse and dampen the velocity
      p1.velocity[1] *= -1.0f + DAMPING_FACTOR;
      p2.velocity[0] *= -1.0f + DAMPING_FACTOR;
      p2.velocity[1] *= -1.0f + DAMPING_FACTOR;

    }
  }

  // Performs collision detection between all points in the array
  void collision(Point pointArray[], int size)
  {
    for (int index = 0; index < size; ++index)
    {
      for (int index2 = index + 1; index2 < size; ++index2)
      {
        // TODO look up a better way of partitioning space so that i can run more particles
        if (pointArray[index].cellID == pointArray[index2].cellID)
        {
          //std::cout << "we are in the same cell" << std::endl;
          checkCollision(pointArray[index], pointArray[index2]);
        }
      }
    }
  }

  // Check for edge collisions and resolve them by reflecting the points
  void checkEdges(int xConstraint, int yConstraint)
  {
    float leftEdge = 0 + 0.5f;
    float rightEdge = xConstraint - 0.5f;

    float topEdge = 0 + 0.5f;
    float botEdge = yConstraint - 0.5f;

    // Horizontal constraint
    if (pos[0] < leftEdge)
    {
      velocity[0] *= -1 + DAMPING_FACTOR;
      pos[0] = leftEdge;
    }
    else if (pos[0] > rightEdge)
    {
      velocity[0] *= -1 + DAMPING_FACTOR;
      pos[0] = rightEdge;
    }
    // Vertical constraint
    if (pos[1] < topEdge)
    {
      velocity[1] *= -1 + DAMPING_FACTOR;
      pos[1] = topEdge;
    }
    else if (pos[1] > botEdge)
    {
      velocity[1] *= -1 + DAMPING_FACTOR;
      pos[1] = botEdge;
    }
  }

  // Simplified gravity calculation (ignoring mass for simplicity)
  void calculateGravity(Point& p1, Point& p2)
  {
    float dx = p1.pos[0] - p2.pos[0];
    float dy = p1.pos[1] - p2.pos[1];

    // Calculate the distance squared (to avoid sqrt computation)
    float distanceSquared = dx * dx + dy * dy;

    // Avoid division by zero if points are exactly on top of each other
    if (distanceSquared == 0)
      return;

    // Gravitational force magnitude (we use inverse square law)
    float force = GRAVITYCONSTANT / distanceSquared;

    // Normalize direction
    float distance = std::sqrt(distanceSquared);
    float directionX = dx / distance;
    float directionY = dy / distance;

    // Apply the force to the accelerations
    p1.acceleration[0] += force * directionX;
    p1.acceleration[1] += force * directionY;

    p2.acceleration[0] -= force * directionX;
    p2.acceleration[1] -= force * directionY;
  }

  // Apply gravity between all points in the array
  void applyGravity(Point pointArray[], int size)
  {
    for (int index = 0; index < size; ++index)
    {
      for (int index2 = index + 1; index2 < size; ++index2)
      {
        // Check if the points are in the same cell
        if (pointArray[index].cellID == pointArray[index2].cellID)
        {
          calculateGravity(pointArray[index], pointArray[index2]);
        }
      }
    }
  }


  // Move the point based on its velocity and acceleration
  void move(int xConstraint, int yConstraint)
  {
    // Apply acceleration to velocity, then apply velocity to position
    for (size_t i = 0; i < velocity.size(); ++i)
    {
      //velocity[i] = 0;
      velocity[i] += acceleration[i];
      pos[i] += velocity[i];
    }

    // Check for edge collisions
    checkEdges(xConstraint, yConstraint);
    updateCellID();
  }

  // Update the cell ID based on position and grid width
// Update the cell ID based on position and grid size
  void updateCellID()
  {
    // Calculate the grid position (x, y) of the point using integer division
    int cellX = static_cast<int>(pos[0]) / CELLSIZE;
    int cellY = static_cast<int>(pos[1]) / CELLSIZE;

    // Calculate the cell ID based on the cell's (x, y) position
    cellID = cellY * GRIDWIDTH + cellX;
  }

  // Render the point to the screen
  void renderPoint(SDL_Renderer* renderer)
  {
    SDL_SetRenderDrawColor(renderer, color2.r, color2.g, color2.b, color2.a);
    SDL_RenderDrawPoint(renderer, static_cast<int>(pos[0]), static_cast<int>(pos[1]));
  }
};

// Define constructor outside the class and without set values
Point::Point(std::vector<float> p, std::vector<float> v, std::vector<float> a)
{
  pos = p;
  velocity = v;
  acceleration = a;
  updateCellID(); // Initial cellID based on position
}
