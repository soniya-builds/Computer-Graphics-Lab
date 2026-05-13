#include <windows.h>
#include <GL/glut.h>

float x = -300;
float y = -300;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(10);

    glColor3f(1, 0, 0);

    glBegin(GL_POINTS);

        glVertex2f(x, y);

    glEnd();

    glFlush();
}

void update(int value)
{
    x += 5;
    y += 5;

    if(x > 300 || y > 300)
    {
        x = -300;
        y = -300;
    }

    glutPostRedisplay();

    glutTimerFunc(30, update, 0);
}

void init()
{
    glClearColor(1, 1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-300, 300, -300, 300);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Standard window size

    glutInitWindowSize(640, 480);

    glutInitWindowPosition(100, 100);

    glutCreateWindow("Moving Point");

    init();

    glutDisplayFunc(display);

    glutTimerFunc(30, update, 0);

    glutMainLoop();

    return 0;
}