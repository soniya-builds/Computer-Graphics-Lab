#include <windows.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

void display(){
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    float xs=0.1, ys=0.2;
    float xe=0.1, ye=0.8;

    if(ys>ye)
        swap(ys,ye);

    float x=xs, y=ys;

    while(y<=ye){
        glColor3f(1.0, 0.4, 0.7); 
        glVertex2f(x,y);
        y+=0.001;
    }

    glEnd();
    glFlush();
}

int main(int argc,char* argv[]){
    glutInit(&argc,argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Vertical Line");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}