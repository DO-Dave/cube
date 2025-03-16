// consts.h
#ifndef CONSTS_H
#define CONSTS_H

// for screen setup
const int SCREEN_WIDTH = 1400 ;
const int SCREEN_HEIGHT =1400;
const int FPS = 60;
const int FRAME_DELAY = (1000 / FPS);


struct RGBA {
    int r, g, b, a;
};

const int GRIDWIDTH = 500;
const int CELLSIZE = 100;
const int NUMBEROFPOINTS = 5000; // can run 11k at a reasonable framerate


// Gravity constant (you can adjust this for stronger/weaker gravity)
const float GRAVITYCONSTANT = 0.001f;
const float DAMPING_FACTOR = 0.3f; // How much energy is lost on collision
#endif // CONSTS_H

