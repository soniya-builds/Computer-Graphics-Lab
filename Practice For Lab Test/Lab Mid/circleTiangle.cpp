#include <windows.h>
#include <GL/glut.h>
#include <cmath>

// Function to draw circle using integer center
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

    glColor3f(0.0, 0.6, 0.3); // green

    // 🔺 Inverted Triangle (using int)
    int topY = 300;

    glBegin(GL_LINE_LOOP);
        glVertex2i(200, topY);   // left top
        glVertex2i(400, topY);   // right top
        glVertex2i(300, 100);    // bottom
    glEnd();

    // ⚪ Circle (touching triangle)
    int radius = 50;
    int centerX = 300;
    int centerY = topY + radius;  // ensures touching

    drawCircle(centerX, centerY, radius);

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480); // pixel coordinates
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Circle Touching Triangle (int)");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}