#include <windows.h>
#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.5, 1.0);

    // Square (house body)
    glBegin(GL_LINE_LOOP);
        glVertex2i(200,200);
        glVertex2i(400,200);
        glVertex2i(400,350);
        glVertex2i(200,350);
    glEnd();

    // Triangle (roof)
    glBegin(GL_LINE_LOOP);
        glVertex2i(200,350);
        glVertex2i(400,350);
        glVertex2i(300,450);
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(0,640,0,480);
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitWindowSize(640,480);
    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutCreateWindow("House");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}