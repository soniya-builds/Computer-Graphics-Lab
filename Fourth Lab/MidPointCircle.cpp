#include <windows.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

int xc = 320, yc = 240;
int r = 100;

void plotCirclePoints(int x, int y) {
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.4, 0.7);
    glBegin(GL_POINTS);

    int x = 0;
    int y = r;
    int p = 1 - r;

    plotCirclePoints(x, y);

    while (x < y) {
        x++;

        if (p < 0) {
            p = p + 2 * x + 1;
        } else {
            y--;
            p = p + 2 * x + 1 - 2 * y;
        }

        plotCirclePoints(x, y);
    }

    glEnd();
    glFlush();
}

void init() {
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Midpoint Circle");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}