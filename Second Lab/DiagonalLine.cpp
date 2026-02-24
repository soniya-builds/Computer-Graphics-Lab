#include <windows.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

void display(){
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    float xs=0.1, ys=0.3;
    float xe=0.6, ye=0.9;

    if(xs>xe){
        swap(xs,xe);
        swap(ys,ye);
    }

    float x=xs, y=ys;

    while(x<=xe){
        glColor3f(1.0, 0.4, 0.7); 
        glVertex2f(x,y);
        x+=0.0001;
        y+=0.0001;
    }

    glEnd();
    glFlush();
}

int main(int argc,char* argv[]){
    glutInit(&argc,argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Diagonal Line m=1");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}