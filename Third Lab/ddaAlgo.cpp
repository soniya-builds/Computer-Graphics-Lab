#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

int x_start, y_start, x_end, y_end;

void drawLine() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.6f, 0.0f, 0.6f);
    glPointSize(3.0f);
    glBegin(GL_POINTS);

    float dx = x_end - x_start;
    float dy = y_end - y_start;

    float slope = dy / dx;
    cout << "Calculated Slope: " << slope << endl;

    float x = x_start;
    float y = y_start;

    if (fabs(slope) <= 1) {
        if (x_start > x_end) {
            swap(x_start, x_end);
            swap(y_start, y_end);
            x = x_start;
            y = y_start;
        }

        while (x <= x_end) {
            glVertex2i((int)round(x), (int)round(y));
            x = x + 1;
            y = y + slope;
        }
    }
    else {
        if (y_start > y_end) {
            swap(x_start, x_end);
            swap(y_start, y_end);
            x = x_start;
            y = y_start;
        }

        while (y <= y_end) {
            glVertex2i((int)round(x), (int)round(y));
            y = y + 1;
            x = x + (1 / slope);
        }
    }

    glEnd();
    glFlush();
}

int main(int argc, char** argv) {

    cout << "Enter starting point (x y): ";
    cin >> x_start >> y_start;

    cout << "Enter ending point (x y): ";
    cin >> x_end >> y_end;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DDA Line Algorithm");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);

    glutDisplayFunc(drawLine);
    glutMainLoop();

    return 0;
}