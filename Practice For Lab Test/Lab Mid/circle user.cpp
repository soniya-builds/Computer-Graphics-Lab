#include <windows.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

// Global variables
int cenX, cenY, r;

// Plot 8 symmetric points
void plotPoints(int x, int y) {
    glVertex2i(cenX + x, cenY + y);
    glVertex2i(cenX - x, cenY + y);
    glVertex2i(cenX + x, cenY - y);
    glVertex2i(cenX - x, cenY - y);
    glVertex2i(cenX + y, cenY + x);
    glVertex2i(cenX - y, cenY + x);
    glVertex2i(cenX + y, cenY - x);
    glVertex2i(cenX - y, cenY - x);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.4, 0.7); // Pink circle
    glBegin(GL_POINTS);

    int x = 0;
    int y = r;

    int p = 1 - r;  // Initial decision parameter

    while (x <= y) {
        plotPoints(x, y);

        x++;

        if (p < 0) {
            p = p + 2 * x + 1;
        } else {
            y--;
            p = p + 2 * x + 1 - 2 * y;
        }
    }

    glEnd();
    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);
}

int main(int argc, char** argv) {

    // 👉 User Input
    cout << "Enter center (cenX cenY): ";
    cin >> cenX >> cenY;

    cout << "Enter radius: ";
    cin >> r;

    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Midpoint Circle (User Input)");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}