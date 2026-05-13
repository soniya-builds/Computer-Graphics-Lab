#include <windows.h>
#include <GL/glut.h>

int cenX = 320, cenY = 240;
int r = 100;

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

    glColor3f(1.0, 0.4, 0.7); 
    glBegin(GL_POINTS);

    int x = 0;
    int y = r;

    int p = 1 - r;   
    while (x <= y) {
        plotPoints(x, y);

        x = x + 1;

        if (p < 0) {
            p = p + 2 * x + 1;
        } else {
            y = y - 1;
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
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Midpoint Circle");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}