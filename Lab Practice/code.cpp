#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int X1, Y1;
int X2, Y2;
int X3, Y3;

int tx, ty, tz;

float sx, sy;

int angle;

int shx, shy;

void drawTriangle(int ax, int ay,
    int bx, int by,
    int cx, int cy) {
    glBegin(GL_LINE_LOOP);

    glVertex2i(ax, ay);
    glVertex2i(bx, by);
    glVertex2i(cx, cy);

    glEnd();
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-1280.0, 1280.0, -960.0, 960.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLineWidth(3);


    glColor3f(0.5, 0.0, 0.0);

    drawTriangle(
        X1, Y1,
        X2, Y2,
        X3, Y3
    );

    glPushMatrix();

    glTranslatef(tx, ty, tz);

    glColor3f(0.0, 0.7, 0.0);

    drawTriangle(
        X1, Y1,
        X2, Y2,
        X3, Y3
    );

    glPopMatrix();

    glColor3f(0.0, 0.0, 1.0);

    drawTriangle(
        X1 * sx, Y1 * sy,
        X2 * sx, Y2 * sy,
        X3 * sx, Y3 * sy
    );

    float theta = angle * 3.1416 / 180.0;

    int ROTX1 = X1 * cos(theta) - Y1 * sin(theta);
    int ROTY1 = X1 * sin(theta) + Y1 * cos(theta);

    int ROTX2 = X2 * cos(theta) - Y2 * sin(theta);
    int ROTY2 = X2 * sin(theta) + Y2 * cos(theta);

    int ROTX3 = X3 * cos(theta) - Y3 * sin(theta);
    int ROTY3 = X3 * sin(theta) + Y3 * cos(theta);

    glColor3f(1.0, 0.5, 0.0);

    drawTriangle(
        ROTX1, ROTY1,
        ROTX2, ROTY2,
        ROTX3, ROTY3
    );

    int SHX1 = X1 + shx * Y1;
    int SHY1 = Y1 + shy * X1;

    int SHX2 = X2 + shx * Y2;
    int SHY2 = Y2 + shy * X2;

    int SHX3 = X3 + shx * Y3;
    int SHY3 = Y3 + shy * X3;

    glColor3f(1.0, 0.0, 1.0);

    drawTriangle(
        SHX1, SHY1,
        SHX2, SHY2,
        SHX3, SHY3
    );


    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_LINES);

    glVertex2i(-1280, -1280);
    glVertex2i(1280, 1280);

    glEnd();


    int RX1 = Y1;
    int RY1 = X1;

    int RX2 = Y2;
    int RY2 = X2;

    int RX3 = Y3;
    int RY3 = X3;

    glColor3f(0.0, 0.7, 0.7);

    drawTriangle(
        RX1, RY1,
        RX2, RY2,
        RX3, RY3
    );

    glFlush();
}

int main(int argc, char* argv[]) {
    cout << "Enter Triangle Points:\n";

    cout << "X1 Y1: ";
    cin >> X1 >> Y1;

    cout << "X2 Y2: ";
    cin >> X2 >> Y2;

    cout << "X3 Y3: ";
    cin >> X3 >> Y3;

    cout << "\nEnter Translation Factors (tx ty tz): ";
    cin >> tx >> ty >> tz;

    cout << "\nEnter Scaling Factors (sx sy): ";
    cin >> sx >> sy;

    cout << "\nEnter Rotation Angle: ";
    cin >> angle;

    cout << "\nEnter Shearing Factors (shx shy): ";
    cin >> shx >> shy;

    glutInit(&argc, argv);

    glutInitWindowSize(1000, 800);
    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("2D Transformations");

    glClearColor(1, 1, 1, 1);

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}