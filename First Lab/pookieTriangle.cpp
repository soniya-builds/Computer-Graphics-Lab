#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Pink color for the triangle
    glColor3f(1.0f, 0.4f, 0.7f);

    glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.6f); 
        glVertex2f(-0.6f, -0.4f);
        glVertex2f(0.6f, -0.4f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Pookie Triangle using OpenGL");

    // White background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}