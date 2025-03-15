#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <cmath> // For sin(), cos()

#include "Cube.cpp"

// Function to check if a key is pressed (non-blocking)
int kbhit()
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

// Function to get a key press (non-blocking)
char getKey()
{
  if (kbhit())
  {
    return getchar();
  }
  return 0;
}

// Main update loop
void updateLoop(Cube& mycube)
{
  while (true)
  {
    mycube.DrawCube();

    // Handle keyboard input (move up, down, left, right)
    char key = getKey();
    if (key == 'q')
      break; // Quit loop if 'q' is pressed

    usleep(10000); // Delay in microseconds to control the refresh rate
    clearScreen();  // Refresh the screen
  }
}

int main()
{
  Cube mycube;
  updateLoop(mycube);
  return 0;
}
