#include <windows.h>
#include <GL/glut.h>
#include <cstdlib>

void display() {
    glClearColor(1.0,1.0,1.0,1.0);
    //output er background color.(1.0, 1.0, 1.0) = White (RGB), Last 1.0 = opacity
    glClear(GL_COLOR_BUFFER_BIT);
    //resetting the bg before drawing the points
    glPointSize(8.0);
    //setting the size of the points.each point will be 8×8 pixels, so clearly visible.
    glBegin(GL_POINTS);
    //Start drawing points....Everything between glBegin and glEnd defines what to draw.

    
    glColor3f(0.0, 0.0, 1.0);
    //current drawing color 001=blue
    glVertex2f(0.1, 0.2);
    /*
    This means coordinates are in range:
    -1 to +1 (both x and y)

    So:
    (0,0) → center
    (1,1) → top-right
    (-1,-1) → bottom-left

    So (0.1, 0.2) is a point slightly to the right and above the center.
    */
    
    glColor3f(0.6, 0.0, 0.8);
    //Changes color again before next point. 608= purple
    glVertex2f(0.5, 0.3);
    /*
    This means coordinates are in range:
    -1 to +1 (both x and y)

    So:
    (0,0) → center
    (1,1) → top-right
    (-1,-1) → bottom-left

    So (0.5, 0.3) is a point further to the right and above the center.
    */
    

    glEnd();
    //Ends the drawing block..No more vertices allowed after this until next glBegin
    glFlush();
    //ensures that all OpenGL commands are executed as quickly as possible. It forces the rendering pipeline to finish drawing the points before moving on.
}

int main(int argc,char** argv)
//Entry point of program. argc = number of command line arguments, argv = array of command line arguments
{
    glutInit(&argc,argv);
    //Initializes GLUT library
    glutInitWindowSize(640,480);
    //Sets the initial window size to 640 pixels wide and 480 pixels tall
    glutInitWindowPosition(10,10);
    //Sets window position on screen (top-left corner offset).
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    //Sets display/color mode to RGB color and single buffering no double buffering
    glutCreateWindow("Multiple Dot Colors");
    //Creates a window with the specified title
    glutDisplayFunc(display);
    //Registers display() as the function to draw graphics whenever the window needs to be redrawn. This is the main drawing function that will be called by GLUT.
    glutMainLoop();
    //starts the GLUT event processing loop. This function will not return until the program exits. It keeps the application running and responsive to user input and window events.
    return EXIT_SUCCESS;
    //Returns 0 to indicate successful execution of the program.
}