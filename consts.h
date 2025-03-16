// consts.h
#ifndef CONSTS_H
#define CONSTS_H

struct RGBA {
    int r, g, b, a;
};

const int GRIDWIDTH = 500;
const int CELLSIZE = 100;
const int NUMBEROFPOINTS = 100;

// Gravity constant (you can adjust this for stronger/weaker gravity)
const float GRAVITYCONSTANT = 0.001f;
const float DAMPING_FACTOR = 0.3f; // How much energy is lost on collision
#endif // CONSTS_H
