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

    glColor3f(0.6, 0.3, 0.1); // brown cone

    // 🔺 Cone (triangle)
    int topY = 250;

    glBegin(GL_LINE_LOOP);
        glVertex2i(250, topY);   // left top
        glVertex2i(390, topY);   // right top
        glVertex2i(320, 100);    // bottom
    glEnd();

    // ⚪ Ice cream (circle touching cone)
    glColor3f(1.0, 0.4, 0.7); // pink scoop

    int radius = 60;
    int centerX = 320;
    int centerY = topY + radius;  // touching condition

    drawCircle(centerX, centerY, radius);

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
    glutCreateWindow("Ice Cream Cone");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}