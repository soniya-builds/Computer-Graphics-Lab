#include <windows.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

void display(){
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    float xs=-0.1, ys=0.5;
    float xe=0.8, ye=0.5;

    if(xs>xe)
        swap(xs,xe);

    float x=xs, y=ys;

    while(x<=xe){
        glColor3f(1.0, 0.4, 0.7);
        glVertex2f(x,y);
        x+=0.001;
    }

    glEnd();
    glFlush();
}

int main(int argc,char* argv[]){
    glutInit(&argc,argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Horizontal Line");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}