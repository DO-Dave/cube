#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <algorithm>  // Needed for std::remove
#include "consts.h"

class Point {
public:
  std::vector<float> pos;
  std::vector<float> velocity;
  std::vector<float> acceleration;
  int cellID = -1;

  RGBA color2 = { std::rand() % 256, std::rand() % 256, std::rand() % 256, 255 };
  RGBA color = { 255, 255, 255, 255 };

  // Static spatial hash map for faster collision detection
  static std::unordered_map<int, std::vector<Point*>> spatialHash;

  // Constructor (Preserved as you originally wrote)
  Point(std::vector<float> p = {0.0f, 0.0f}, std::vector<float> v = {0.0f, 0.0f}, std::vector<float> a = {0.0f, 0.0f}) {
    pos = p;
    velocity = v;
    acceleration = a;
    updateCellID();
    spatialHash[cellID].push_back(this);
  }

  // Destructor: Remove the point from the spatial hash
  ~Point() {
    removeFromSpatialHash();
  }

  // Properly remove from spatial hash
  void removeFromSpatialHash() {
    auto& vec = spatialHash[cellID];
    vec.erase(std::remove(vec.begin(), vec.end(), this), vec.end());
  }

  // Get distance between two points
  static float getDistance(const Point& p1, const Point& p2) {
    float dx = p1.pos[0] - p2.pos[0];
    float dy = p1.pos[1] - p2.pos[1];
    return std::sqrt(dx * dx + dy * dy);
  }

  // Collision detection and response
  static void checkCollision(Point& p1, Point& p2) {
    float distance = getDistance(p1, p2);
    float overlap = 2.0f - distance; // Radius = 1px, so diameter = 2px

    if (overlap > 0) {
      std::vector<float> direction = { p1.pos[0] - p2.pos[0], p1.pos[1] - p2.pos[1] };
      float length = std::sqrt(direction[0] * direction[0] + direction[1] * direction[1]);

      if (length > 0) {
        direction[0] /= length;
        direction[1] /= length;
      }

      p1.pos[0] += direction[0] * (overlap / 2.0f);
      p1.pos[1] += direction[1] * (overlap / 2.0f);
      p2.pos[0] -= direction[0] * (overlap / 2.0f);
      p2.pos[1] -= direction[1] * (overlap / 2.0f);

      // Reverse and dampen velocities
      p1.velocity[0] *= -1.0f + DAMPING_FACTOR;
      p1.velocity[1] *= -1.0f + DAMPING_FACTOR;
      p2.velocity[0] *= -1.0f + DAMPING_FACTOR;
      p2.velocity[1] *= -1.0f + DAMPING_FACTOR;
    }
  }

  // Perform collision detection with spatial hash
  void collision() {
    auto& cellPoints = spatialHash[cellID];
    for (Point* other : cellPoints) {
      if (other != this) {
        checkCollision(*this, *other);
      }
    }
  }

  // Edge detection and response
  void checkEdges(int yConstraint, int xConstraint) {
    float leftEdge = 0 + 0.5f;
    float rightEdge = xConstraint - 0.5f;
    float topEdge = 0 + 0.5f;
    float botEdge = yConstraint - 0.5f;

    if (pos[0] < leftEdge) {
      velocity[0] *= -1 + DAMPING_FACTOR;
      pos[0] = leftEdge;
    }
    else if (pos[0] > rightEdge) {
      velocity[0] *= -1 + DAMPING_FACTOR;
      pos[0] = rightEdge;
    }

    if (pos[1] < topEdge) {
      velocity[1] *= -1 + DAMPING_FACTOR;
      pos[1] = topEdge;
    }
    else if (pos[1] > botEdge) {
      velocity[1] *= -1 + DAMPING_FACTOR;
      pos[1] = botEdge;
    }
  }

  // Gravity calculation (ignoring mass)
  static void calculateGravity(Point& p1, Point& p2) {
    float dx = p1.pos[0] - p2.pos[0];
    float dy = p1.pos[1] - p2.pos[1];
    float distanceSquared = dx * dx + dy * dy;

    if (distanceSquared == 0)
      return;

    float force = GRAVITYCONSTANT / distanceSquared;
    float distance = std::sqrt(distanceSquared);
    float directionX = dx / distance;
    float directionY = dy / distance;

    p1.acceleration[0] += force * directionX;
    p1.acceleration[1] += force * directionY;
    p2.acceleration[0] -= force * directionX;
    p2.acceleration[1] -= force * directionY;
  }

  // Apply gravity based on spatial hash
  void applyGravity() {
    auto& cellPoints = spatialHash[cellID];
    for (Point* other : cellPoints) {
      if (other != this) {
        calculateGravity(*this, *other);
      }
    }
  }

  // Move function (updates position based on velocity & acceleration)
  void move(int xConstraint, int yConstraint) {
    for (size_t i = 0; i < velocity.size(); ++i) {
      velocity[i] += acceleration[i];
      pos[i] += velocity[i];
    }

    checkEdges(xConstraint, yConstraint);
    updateCellID();
  }

  // Update cellID in the spatial hash
  void updateCellID() {
    int newCellX = static_cast<int>(pos[0]) / CELLSIZE;
    int newCellY = static_cast<int>(pos[1]) / CELLSIZE;
    int newCellID = newCellY * GRIDWIDTH + newCellX;

    if (newCellID != cellID) {
      removeFromSpatialHash();
      cellID = newCellID;
      spatialHash[cellID].push_back(this);
    }
  }

  // Render the point on the screen
  void renderPoint(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, color2.r, color2.g, color2.b, color2.a);
    SDL_RenderDrawPoint(renderer, static_cast<int>(pos[0]), static_cast<int>(pos[1]));
  }
};

// Define spatial hash outside the class
std::unordered_map<int, std::vector<Point*>> Point::spatialHash;
