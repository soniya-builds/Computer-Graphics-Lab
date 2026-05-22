#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>

const int rainCount = 300;

float rainX[rainCount];
float rainY[rainCount];
float rainSpeed[rainCount];

bool lightning = false;
int lightningTimer = 0;

void initializeRain()
{
    for (int i = 0; i < rainCount; i++)
    {
        rainX[i] = (rand() % 2000 - 1000) / 1000.0f;
        rainY[i] = (rand() % 2000) / 1000.0f;
        rainSpeed[i] = 0.008f + (rand() % 10) / 5000.0f;
    }
}

void drawCircle(float x, float y, float r)
{
    glBegin(GL_POLYGON);

    for (int i = 0; i < 360; i++)
    {
        float angle = i * 3.14159265f / 180.0f;

        glVertex2f(x + r * cos(angle),
                   y + r * sin(angle));
    }

    glEnd();
}

void drawCloud(float x, float y)
{
    glColor3f(0.85f, 0.85f, 0.85f);

    drawCircle(x, y, 0.10f);
    drawCircle(x + 0.10f, y + 0.04f, 0.12f);
    drawCircle(x + 0.22f, y, 0.10f);
    drawCircle(x + 0.06f, y - 0.04f, 0.10f);
    drawCircle(x + 0.16f, y - 0.04f, 0.10f);
}

void drawRain()
{
    glColor3f(0.65f, 0.85f, 1.0f);

    glLineWidth(2);

    glBegin(GL_LINES);

    for (int i = 0; i < rainCount; i++)
    {
        glVertex2f(rainX[i], rainY[i]);
        glVertex2f(rainX[i] - 0.015f,
                   rainY[i] - 0.05f);
    }

    glEnd();
}

void drawLightning()
{
    if (!lightning)
        return;

    glColor3f(1.0f, 1.0f, 0.8f);

    glLineWidth(6);

    glBegin(GL_LINE_STRIP);

    glVertex2f(0.2f, 1.0f);
    glVertex2f(0.1f, 0.7f);
    glVertex2f(0.25f, 0.7f);
    glVertex2f(0.05f, 0.3f);
    glVertex2f(0.2f, 0.3f);
    glVertex2f(0.0f, -0.1f);

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    if (lightning)
    {
        glColor3f(0.75f, 0.75f, 0.78f);
    }
    else
    {
        glColor3f(0.20f, 0.20f, 0.28f);
    }

    glBegin(GL_QUADS);

    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);

    glEnd();

    glColor3f(0.1f, 0.4f, 0.1f);

    glBegin(GL_QUADS);

    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.75f);
    glVertex2f(-1.0f, -0.75f);

    glEnd();

    drawCloud(-0.75f, 0.75f);
    drawCloud(-0.15f, 0.72f);
    drawCloud(0.45f, 0.76f);

    drawRain();

    drawLightning();

    glutSwapBuffers();
}

void update(int value)
{
    for (int i = 0; i < rainCount; i++)
    {
        rainY[i] -= rainSpeed[i];

        if (rainY[i] < -1.0f)
        {
            rainY[i] = 1.0f;
            rainX[i] = (rand() % 2000 - 1000) / 1000.0f;
        }
    }

    lightningTimer++;

    if (lightningTimer > 150)
    {
        lightning = true;
    }

    if (lightningTimer > 160)
    {
        lightning = false;
        lightningTimer = 0;
    }

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
}

void init()
{
    srand(time(0));

    initializeRain();

    glClearColor(0.15f, 0.15f, 0.2f, 1.0f);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(-1.0, 1.0,
               -1.0, 1.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(1000, 700);

    glutCreateWindow("Rainfall Animation System");

    init();

    glutDisplayFunc(display);

    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}