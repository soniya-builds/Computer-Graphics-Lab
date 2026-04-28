#include <windows.h>
#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0); // black

    // ⭐ Star using lines
    glBegin(GL_LINE_LOOP);
        glVertex2i(320, 400); // top
        glVertex2i(350, 300);
        glVertex2i(420, 300);
        glVertex2i(360, 250);
        glVertex2i(390, 170);
        glVertex2i(320, 220);
        glVertex2i(250, 170);
        glVertex2i(280, 250);
        glVertex2i(220, 300);
        glVertex2i(290, 300);
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
    glutCreateWindow("Star Shape");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}