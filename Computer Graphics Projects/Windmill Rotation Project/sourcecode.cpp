#include <GL/glut.h>
#include <math.h>

float bladeAngle = 0.0f;

void drawCircle(float radius)
{
    glBegin(GL_POLYGON);

    for (int i = 0; i < 360; i++)
    {
        float angle = i * 3.14159265f / 180.0f;

        glVertex2f(radius * cos(angle),
                   radius * sin(angle));
    }

    glEnd();
}

void drawBlade()
{
    glBegin(GL_POLYGON);

    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.08f, 0.0f);
    glVertex2f(0.03f, 0.55f);
    glVertex2f(-0.03f, 0.55f);

    glEnd();
}

void drawCloud(float x, float y)
{
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(x - 0.10f, y, 0.0f);
    drawCircle(0.09f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x, y + 0.06f, 0.0f);
    drawCircle(0.11f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x + 0.12f, y + 0.04f, 0.0f);
    drawCircle(0.12f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x + 0.22f, y, 0.0f);
    drawCircle(0.09f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x + 0.05f, y - 0.05f, 0.0f);
    drawCircle(0.09f);
    glPopMatrix();
}

void drawWindmill()
{
    glColor3f(0.65f, 0.45f, 0.25f);

    glBegin(GL_POLYGON);

    glVertex2f(-0.05f, -0.9f);
    glVertex2f(0.05f, -0.9f);
    glVertex2f(0.02f, -0.1f);
    glVertex2f(-0.02f, -0.1f);

    glEnd();

    glPushMatrix();

    glTranslatef(0.0f, -0.1f, 0.0f);

    glRotatef(bladeAngle, 0.0f, 0.0f, 1.0f);

    glColor3f(0.95f, 0.95f, 0.95f);

    for (int i = 0; i < 4; i++)
    {
        drawBlade();

        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    }

    glColor3f(0.2f, 0.2f, 0.2f);

    drawCircle(0.05f);

    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    glColor3f(0.55f, 0.85f, 1.0f);

    glBegin(GL_QUADS);

    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);

    glEnd();

    glColor3f(0.35f, 0.8f, 0.35f);

    glBegin(GL_QUADS);

    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.5f);
    glVertex2f(-1.0f, -0.5f);

    glEnd();

    drawCloud(-0.55f, 0.52f);

    drawCloud(0.20f, 0.68f);

    drawWindmill();

    glutSwapBuffers();
}

void update(int value)
{
    bladeAngle -= 2.0f;

    if (bladeAngle < -360.0f)
    {
        bladeAngle = 0.0f;
    }

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
}

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(-1.0, 1.0,
               -1.0, 1.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(900, 700);

    glutCreateWindow("Windmill Rotation Project");

    init();

    glutDisplayFunc(display);

    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}