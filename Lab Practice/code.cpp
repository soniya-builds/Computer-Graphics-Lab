#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int X1, Y1;
int X2, Y2;
int X3, Y3;

int shx, shy;

void drawTriangle(int ax, int ay,
                  int bx, int by,
                  int cx, int cy)
{
    glBegin(GL_LINES);

    glVertex2i(ax, ay);
    glVertex2i(bx, by);

    glVertex2i(bx, by);
    glVertex2i(cx, cy);

    glVertex2i(cx, cy);
    glVertex2i(ax, ay);

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0.0, 1280.0, 0.0, 960.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLineWidth(4);

    glColor3f(0.5, 0.0, 0.0);

    drawTriangle(
        X1, Y1,
        X2, Y2,
        X3, Y3
    );

    int SHX1 = X1 + shx * Y1;
    int SHY1 = Y1 + shy * X1;

    int SHX2 = X2 + shx * Y2;
    int SHY2 = Y2 + shy * X2;

    int SHX3 = X3 + shx * Y3;
    int SHY3 = Y3 + shy * X3;

    glColor3f(0.0, 0.7, 0.0);

    drawTriangle(
        SHX1, SHY1,
        SHX2, SHY2,
        SHX3, SHY3
    );

    glFlush();
}

int main(int argc, char *argv[])
{
    cout << "Enter Triangle Points:\n";

    cout << "X1 Y1: ";
    cin >> X1 >> Y1;

    cout << "X2 Y2: ";
    cin >> X2 >> Y2;

    cout << "X3 Y3: ";
    cin >> X3 >> Y3;

    cout << "\nEnter Shearing Factors (shx shy): ";
    cin >> shx >> shy;

    glutInit(&argc, argv);

    glutInitWindowSize(1000, 800);
    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("2D Shearing");

    glClearColor(1, 1, 1, 1);

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}