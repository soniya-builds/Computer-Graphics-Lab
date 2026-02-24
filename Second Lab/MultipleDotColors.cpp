#include <windows.h>
#include <GL/glut.h>
#include <cstdlib>

void display() {
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(8.0);   // Slightly bigger dots

    glBegin(GL_POINTS);

    // Blue Dot
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(0.1, 0.2);

    // Purple Dot
    glColor3f(0.6, 0.0, 0.8);
    glVertex2f(0.5, 0.3);

    glEnd();
    glFlush();
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Multiple Dot Colors");
    glutDisplayFunc(display);
    glutMainLoop();
    return EXIT_SUCCESS;
}