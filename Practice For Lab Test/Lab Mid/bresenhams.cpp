#include <windows.h>
#include <GL/glut.h>

int xs = 100, ys = 100;
int xe = 400, ye = 300;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.4, 0.7); // Pink line
    glBegin(GL_POINTS);

    int dx = xe - xs;
    int dy = ye - ys;

    int x = xs;
    int y = ys;

    // Case 1: slope < 1
    if (abs(dy) < abs(dx)) {

        if (xs > xe) {
            // swap points
            int temp;
            temp = xs; xs = xe; xe = temp;
            temp = ys; ys = ye; ye = temp;

            dx = xe - xs;
            dy = ye - ys;
        }

        int p = 2 * dy - dx;

        while (x <= xe) {
            glVertex2i(x, y);

            x++;

            if (p < 0) {
                p = p + 2 * dy;
            } else {
                y++;
                p = p + 2 * dy - 2 * dx;
            }
        }
    }

    // Case 2: slope > 1
    else {

        if (ys > ye) {
            // swap points
            int temp;
            temp = xs; xs = xe; xe = temp;
            temp = ys; ys = ye; ye = temp;

            dx = xe - xs;
            dy = ye - ys;
        }

        int p = 2 * dx - dy;

        while (y <= ye) {
            glVertex2i(x, y);

            y++;

            if (p < 0) {
                p = p + 2 * dx;
            } else {
                x++;
                p = p + 2 * dx - 2 * dy;
            }
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
    glutCreateWindow("Bresenham Line");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}