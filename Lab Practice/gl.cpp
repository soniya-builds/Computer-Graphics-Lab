#include <windows.h>
#include <GL/glut.h>

float x = -500;   

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(10);

    glColor3f(1, 0, 0);

    glBegin(GL_POINTS);
        glVertex2f(x, 0);
    glEnd();

    glFlush();
}

void update(int value)
{
    x += 5;   

    if(x > 500)
    {
        x = -500;
    }

    glutPostRedisplay();

    glutTimerFunc(30, update, 0);
}

void init()
{
    glClearColor(1, 1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-500, 500, -500, 500);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(800, 600);

    glutCreateWindow("Moving Point");

    init();

    glutDisplayFunc(display);

    glutTimerFunc(30, update, 0);

    glutMainLoop();

    return 0;
}