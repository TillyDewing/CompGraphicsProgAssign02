//Tilly Dewing - Computer Graphics Assignment 2 - 3/20/2023
//Sam Houston State University - COSC 4332 - Dr. Islam

#include<windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

//Data structure for storing and transforming 2D points
struct Vector2
{
    GLfloat x;
    GLfloat y;
    //Constructor
    Vector2(GLfloat x1, GLfloat y1)
    {
        x = x1;
        y = y1;
    }

    //Translate a point by x & y
    void Translate(GLfloat x2, GLfloat y2)
    {
        x += x2;
        y += y2;
    }

    //rotate point about 0,0 by theta degrees
    void Rotate(GLfloat theta)
    {
        //get theta in radians
        theta = theta * (M_PI / 180);
        //create another struct to store temp values
        Vector2 rotated = Vector2(x,y);
        //Rotate point
        rotated.x = x * cos(theta) - y * sin(theta);
        rotated.y = x * sin(theta) + y * cos(theta);
        //Set values
        x = rotated.x;
        y = rotated.y;
        //return rotated;
    }
};


//Color struct for generating pseudo random colors
//Also not necessary but random colors were more fun.
//Colors of shapes are randomized every draw call.
struct Color
{
    GLfloat r;
    GLfloat g;
    GLfloat b;

    Color()
    {
        r = (float)rand()/(float)RAND_MAX;
        g = (float)rand()/(float)RAND_MAX;
        b = (float)rand()/(float)RAND_MAX;
    }
};

//draw a n sided filled polygon
//Param Defs:         | x & y pos to draw at  | # of sides  | radius of polygon
void DrawFilledPolygon(GLint posX, GLint posY, int numSides, GLint scale)
{
    //Get random color and set it to draw color
    Color randCol = Color();
    glColor3f(randCol.r, randCol.g, randCol.b);

    //Can draw a n sided polygon by rotating n points by 360/n degrees
    float theta = -360.0f / numSides;
    //Starting point before rotation.
    Vector2 point = Vector2(0,1);//Arbitrary can be any point on a circle with radius 1, controls final polygon rotation

    //Fill shape by drawing triangles fanning out from center(origin) of polygon
    glBegin(GL_TRIANGLES); //Begin drawing triangles
    for(int i = 1; i <= numSides; i++)
    {
        glVertex2f(posX,posY); //point 1 origin of polygon
        point.Rotate(theta);
        glVertex2f((point.x * scale) + posX, (point.y * scale) + posY);//point 2 1st rotation
        point.Rotate(theta);
        glVertex2f((point.x * scale) + posX, (point.y * scale) + posY);//point 3 2nd rotation
        point.Rotate(-theta); //Rotate back once to avoid throwing of next triangle draw
    }
    glEnd();
}

// Initialization function sets the background color, color for drawing, size of dots, and the projection matrix
void init()
{
    glutSetWindowTitle("Computer Graphics Assign 1 : Tilly Dewing");
    // Set the background color
    glClearColor(0.5, 0.5, 0.55, 0.0);

    // Set the orthographic projection matrix with the specified left, right, bottom, and top values
    gluOrtho2D(-250, 250, -250, 250);
}

// Display function performs the actual drawing
void display() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Send all output to the screen
    glFlush();
}

void GetInput()
{
    printf("Enter the 4 points for the quad:\n");
    Vector2 points[4];
    for(int i = 0; i < 4; i++)
    {
        printf("Point %i: x,y ", i);
        scanf("%d,%d", &points[i].x, &points[i].y);
    }
}

// Main function
int main(int argc, char** argv)
{

    GetInput();
    // Initialize the GLUT toolkit
    glutInit(&argc, argv);

    // Set the display mode to single buffering, RGBA model
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Set the window size on the screen
    glutInitWindowSize(500, 500);

    // Set the window position on the screen
    glutInitWindowPosition(100, 500);

    // Open the screen window
    glutCreateWindow(argv[0]);

    // Call the initialization function
    init();

    // Point to the display function
    glutDisplayFunc(display);

    // Go into perpetual loop
    glutMainLoop();

    // Return 0
    return 0;
}
