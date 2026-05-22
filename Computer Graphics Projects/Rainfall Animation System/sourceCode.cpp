#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const int rainCount = 800;

float rainX[rainCount];
float rainY[rainCount];
float rainSpeed[rainCount];

bool lightning = false;
int lightningTimer = 0;

float cloudMove = 0.0f;

void initializeRain()
{
    for (int i = 0; i < rainCount; i++)
    {
        rainX[i] = (rand() % 2000 - 1000) / 1000.0f;
        rainY[i] = (rand() % 2000) / 1000.0f;
        rainSpeed[i] = 0.012f + (rand() % 20) / 7000.0f;
    }
}

void drawCircle(float x, float y, float r)
{
    glBegin(GL_POLYGON);

    for (int i = 0; i < 360; i++)
    {
        float angle =
            i * 3.14159265f / 180.0f;

        glVertex2f(x + cos(angle) * r,
                   y + sin(angle) * r);
    }

    glEnd();
}

void drawCloud(float x, float y)
{
    glColor3f(0.22f, 0.22f, 0.28f);

    drawCircle(x, y, 0.12f);
    drawCircle(x + 0.10f, y + 0.04f, 0.15f);
    drawCircle(x + 0.24f, y, 0.13f);
    drawCircle(x + 0.08f, y - 0.05f, 0.12f);
    drawCircle(x + 0.20f, y - 0.04f, 0.11f);
}

void drawRain()
{
    glLineWidth(1.5f);

    glBegin(GL_LINES);

    for (int i = 0; i < rainCount; i++)
    {
        float alpha =
            0.4f + rainSpeed[i] * 20;

        glColor4f(0.55f, 0.85f, 1.0f,
                  alpha);

        glVertex2f(rainX[i],
                   rainY[i]);

        glVertex2f(rainX[i] - 0.015f,
                   rainY[i] - 0.06f);
    }

    glEnd();
}

void drawLightning()
{
    if (!lightning)
        return;

    glColor3f(1.0f, 1.0f, 0.9f);

    glLineWidth(7);

    glBegin(GL_LINE_STRIP);

    glVertex2f(0.35f, 1.0f);
    glVertex2f(0.25f, 0.72f);
    glVertex2f(0.40f, 0.72f);
    glVertex2f(0.18f, 0.30f);
    glVertex2f(0.32f, 0.30f);
    glVertex2f(0.08f, -0.10f);

    glEnd();
}

void drawStreetLamp(float x)
{
    glColor3f(0.08f, 0.08f, 0.08f);

    glBegin(GL_QUADS);

    glVertex2f(x - 0.01f, -0.75f);
    glVertex2f(x + 0.01f, -0.75f);
    glVertex2f(x + 0.01f, 0.10f);
    glVertex2f(x - 0.01f, 0.10f);

    glEnd();

    glColor3f(1.0f, 0.9f, 0.6f);

    drawCircle(x, 0.14f, 0.04f);

    glColor4f(1.0f, 0.9f, 0.6f, 0.15f);

    drawCircle(x, 0.14f, 0.12f);
}

void drawPuddle(float x, float y)
{
    glColor4f(0.2f, 0.8f, 1.0f, 0.45f);

    glBegin(GL_POLYGON);

    for (int i = 0; i < 360; i++)
    {
        float angle =
            i * 3.14159265f / 180.0f;

        glVertex2f(x + cos(angle) * 0.12f,
                   y + sin(angle) * 0.03f);
    }

    glEnd();

    glColor4f(0.8f, 0.95f, 1.0f, 0.5f);

    glLineWidth(1);

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i++)
    {
        float angle =
            i * 3.14159265f / 180.0f;

        glVertex2f(x + cos(angle) * 0.15f,
                   y + sin(angle) * 0.05f);
    }

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    if (lightning)
    {
        glColor3f(0.45f, 0.45f, 0.55f);
    }
    else
    {
        glColor3f(0.05f, 0.05f, 0.12f);
    }

    glBegin(GL_QUADS);

    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);

    glEnd();

    glColor3f(0.10f, 0.10f, 0.14f);

    glBegin(GL_QUADS);

    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.55f);
    glVertex2f(-1.0f, -0.55f);

    glEnd();

    drawCloud(-0.80f + cloudMove,
              0.76f);

    drawCloud(-0.20f + cloudMove,
              0.72f);

    drawCloud(0.45f + cloudMove,
              0.78f);

    drawStreetLamp(-0.75f);

    drawStreetLamp(0.75f);

    drawPuddle(-0.35f, -0.80f);

    drawPuddle(0.25f, -0.76f);

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

            rainX[i] =
                (rand() % 2000 - 1000)
                / 1000.0f;
        }
    }

    cloudMove += 0.0003f;

    if (cloudMove > 0.2f)
    {
        cloudMove = -0.2f;
    }

    lightningTimer++;

    if (lightningTimer > 180)
    {
        lightning = true;
    }

    if (lightningTimer > 190)
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

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA,
                GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.04f, 0.04f, 0.08f,
                 1.0f);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(-1.0, 1.0,
               -1.0, 1.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(1200, 800);

    glutCreateWindow(
        "Rainfall Animation System");

    init();

    glutDisplayFunc(display);

    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}