#include <windows.h>
#include <GL/glut.h>
#include <cmath>

// Draw circle
void drawCircle(int cx, int cy, int r) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        float angle = 2 * 3.1416 * i / 100;
        int x = r * cos(angle);
        int y = r * sin(angle);
        glVertex2i(cx + x, cy + y);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0); // black

    // ⚪ Face
    drawCircle(320, 300, 100);

    // 👁 Eyes
    drawCircle(280, 330, 10);
    drawCircle(360, 330, 10);

    // 😊 Smile (using straight lines)
    glBegin(GL_LINE_STRIP);
        glVertex2i(260, 260);
        glVertex2i(300, 240);
        glVertex2i(340, 240);
        glVertex2i(380, 260);
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
    glutCreateWindow("Smiley (Circle + Line)");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}