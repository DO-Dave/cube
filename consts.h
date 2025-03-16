// consts.h
#ifndef CONSTS_H
#define CONSTS_H

struct RGBA {
    int r, g, b, a;
};

const int GRIDWIDTH = 10;
const int NUMBEROFPOINTS = 100;
float DAMPING_FACTOR = 0.3f; // How much energy is lost on collision
#endif // CONSTS_H
