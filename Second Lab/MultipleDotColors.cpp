#include <windows.h>
#include <GL/glut.h>
#include <cstdlib>

void display() {
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(5.0);

    glBegin(GL_POINTS);

    glColor3f(1.0,0.2,0.0);
    glVertex2f(0.1,0.2);

    glColor3f(1.0,0.6,0.0);
    glVertex2f(0.5,0.3);

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