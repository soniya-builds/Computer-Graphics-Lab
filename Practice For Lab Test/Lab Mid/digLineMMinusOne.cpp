#include <windows.h>
#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.4, 0.7); // Pink line
    glBegin(GL_POINTS);

    // Starting point
    int x = 100, y = 300;

    // Ending condition
    int xe = 400;

    while (x <= xe) {
        glVertex2i(x, y);

        x = x + 1;
        y = y - 1;  // decrease y → slope = -1
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
    glutCreateWindow("Diagonal Line (m = -1)");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}