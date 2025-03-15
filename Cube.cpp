#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <cmath> // For sin(), cos()

// Function to move the cursor
void moveCursor(int x, int y);

// Function to clear the screen
void clearScreen();

// Function to rotate the cube matrix
void rotateMatrix(int matrix[4][2], double theta);

class Cube
{
public:
    Cube();

    // Coordinates for a 2D square (cube)
    int Coordinates[4][2] = {
        {4, 2},   // Bottom left
        {17, 2},  // Bottom right
        {17, 8},  // Top right
        {4, 8}    // Top left
    };

    double theta = 30.0; // Rotation angle in degrees

    void DrawCube();
};

// Constructor
Cube::Cube()
{
    return;
}

// Function to move the cursor to position (x, y)
void moveCursor(int x, int y)
{
    std::cout << "\033[" << y << ";" << x << "H";
}

// Function to clear the screen
void clearScreen()
{
    std::cout << "\033[2J\033[1;1H";
    std::cout.flush();
}

// Function to rotate the cube around its center
void rotateMatrix(int matrix[4][2], double theta)
{
    // Convert degrees to radians
    double radians = theta * M_PI / 180.0;

    // Find the center of the cube (midpoint of bottom-left and top-right corners)
    double centerX = (matrix[0][0] + matrix[2][0]) / 2.0;
    double centerY = (matrix[0][1] + matrix[2][1]) / 2.0;

    for (int i = 0; i < 4; i++)
    {
        // Translate point to origin (subtract center)
        double x = matrix[i][0] - centerX;
        double y = matrix[i][1] - centerY;

        // Rotate using the rotation matrix
        double newX = x * cos(radians) - y * sin(radians);
        double newY = x * sin(radians) + y * cos(radians);

        // Translate back to original position (add center)
        matrix[i][0] = round(newX + centerX);
        matrix[i][1] = round(newY + centerY);
    }
}

// Function to draw the cube (corners and edges)
void Cube::DrawCube()
{
    clearScreen(); // Clear before drawing

    // Draw the corners of the cube
    for (int i = 0; i < 4; i++)
    {
        moveCursor(Coordinates[i][0], Coordinates[i][1]);
        std::cout << "0";
    }

    // Draw the edges of the cube by connecting consecutive corners
    for (int i = 0; i < 4; i++)
    {
        int next = (i + 1) % 4; // To wrap around to the first corner
        int x1 = Coordinates[i][0];
        int y1 = Coordinates[i][1];
        int x2 = Coordinates[next][0];
        int y2 = Coordinates[next][1];

        // Draw a line between the two points
        if (x1 == x2) // Vertical line
        {
            for (int y = std::min(y1, y2) + 1; y < std::max(y1, y2); y++)
            {
                moveCursor(x1, y);
                std::cout << "|";
            }
        }
        else if (y1 == y2) // Horizontal line
        {
            for (int x = std::min(x1, x2) + 1; x < std::max(x1, x2); x++)
            {
                moveCursor(x, y1);
                std::cout << "-";
            }
        }
    }

    std::cout.flush(); // Ensure everything is printed

    // Rotate the cube by `theta` degrees
    rotateMatrix(Coordinates, theta);
    theta = 0; // Reset theta after first rotation
}