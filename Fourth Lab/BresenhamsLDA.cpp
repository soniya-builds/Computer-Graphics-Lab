#include <windows.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;


int x1, y1, x2, y2;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.4, 0.7); 
    glBegin(GL_POINTS);

    int dx = x2 - x1;
    int dy = y2 - y1;

    int x = x1;
    int y = y1;

    int p = 2*dy-dx;

    glVertex2i(x,y);

    while (x<x2) {
        x++;

        if (p<0) {
            p += 2*dy;
        } else {
            y++;
            p += 2*dy - 2*dx;
        }

        glVertex2i(x, y);
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

    cout << "Enter x1 y1: ";
    cin >> x1 >> y1;

    cout << "Enter x2 y2: ";
    cin >> x2 >> y2;

    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Bresenham Line with Input");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}