#include <windows.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

// Global variables for input
float xs, ys, xe, ye;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.4, 0.7); // Pink line
    glBegin(GL_POINTS);

    float dx = xe - xs;
    float dy = ye - ys;

    float m = dy / dx;

    float x, y;

    // Case 1: m <= 1
    if (m <= 1) {
        if (xs > xe) {
            float temp;
            temp = xs; xs = xe; xe = temp;
            temp = ys; ys = ye; ye = temp;
        }

        x = xs;
        y = ys;

        while (x <= xe) {
            glVertex2i(x, y);

            x = x + 1;
            y = y + m;
        }
    }

    // Case 2: m > 1
    else {
        if (ys > ye) {
            float temp;
            temp = xs; xs = xe; xe = temp;
            temp = ys; ys = ye; ye = temp;
        }

        x = xs;
        y = ys;

        while (y <= ye) {
            glVertex2i(x, y);

            y = y + 1;
            x = x + (1 / m);
        }
    }

    glEnd();
    glFlush();
}

void init() {
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);
}

int main(int argc, char** argv) {

    // 👉 User Input
    cout << "Enter starting point (xs ys): ";
    cin >> xs >> ys;

    cout << "Enter ending point (xe ye): ";
    cin >> xe >> ye;

    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("DDA Line (User Input)");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

//    float xs = 100, ys = 100;
//   float xe = 400, ye = 300;