#include <GL/glut.h>

float angle = 0.0f; // Rotation angle

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Move triangle slightly into screen
    glTranslatef(0.0f, 0.0f, -5.0f);

    // Rotate triangle
    glRotatef(angle, 1.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex3f(0.0f, 1.0f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex3f(-1.0f, -1.0f, 0.0f);

        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glutSwapBuffers();
}

void update(int value) {
    angle += 1.0f;           // Increase rotation
    if (angle > 360)
        angle -= 360;

    glutPostRedisplay();     // Redraw
    glutTimerFunc(16, update, 0); // ~60 FPS
}

void init() {
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("3D Rotating Triangle");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}