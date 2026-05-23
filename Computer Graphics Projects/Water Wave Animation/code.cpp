#include <GL/glut.h>
#include <math.h>
#include <iostream>

using namespace std;

const int WINDOW_WIDTH  = 1000;
const int WINDOW_HEIGHT = 700;

float globalTime = 0.0f;
float boatX = -250.0f;

const float WATER_LEVEL = 220.0f;

const float waveAmp1 = 18.0f;
const float waveFreq1 = 0.015f;
const float waveSpeed1 = 2.2f;

const float waveAmp2 = 10.0f;
const float waveFreq2 = 0.028f;
const float waveSpeed2 = 3.5f;

float deg(float rad)
{
    return rad * 180.0f / 3.1415926f;
}

float getWaveHeight(float x)
{
    float wave1 = waveAmp1 * sin(waveFreq1 * x + globalTime * waveSpeed1);
    float wave2 = waveAmp2 * sin(waveFreq2 * x + globalTime * waveSpeed2);

    return WATER_LEVEL + wave1 + wave2;
}

float getWaveSlope(float x)
{
    float slope1 = waveAmp1 * waveFreq1 *
                   cos(waveFreq1 * x + globalTime * waveSpeed1);

    float slope2 = waveAmp2 * waveFreq2 *
                   cos(waveFreq2 * x + globalTime * waveSpeed2);

    return slope1 + slope2;
}

void drawCircle(float cx, float cy, float r, int segments = 50)
{
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(cx, cy);

    for(int i = 0; i <= segments; i++)
    {
        float theta = 2.0f * 3.1415926f * i / segments;

        float x = r * cos(theta);
        float y = r * sin(theta);

        glVertex2f(cx + x, cy + y);
    }

    glEnd();
}

void drawSky()
{
    glBegin(GL_QUADS);

    glColor3f(0.55f, 0.80f, 1.0f);
    glVertex2f(0, WINDOW_HEIGHT);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);

    glColor3f(1.0f, 0.82f, 0.55f);
    glVertex2f(WINDOW_WIDTH, WATER_LEVEL + 120);
    glVertex2f(0, WATER_LEVEL + 120);

    glEnd();

    glColor3f(1.0f, 0.92f, 0.45f);
    drawCircle(820, 560, 45);

    glColor4f(1.0f, 0.9f, 0.5f, 0.25f);
    drawCircle(820, 560, 70);
}

void drawWaves()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUAD_STRIP);

    for(int x = 0; x <= WINDOW_WIDTH; x += 5)
    {
        float y = getWaveHeight((float)x);

        glColor3f(0.02f, 0.18f, 0.55f);
        glVertex2f(x, 0);

        glColor3f(0.05f, 0.35f, 0.78f);
        glVertex2f(x, y);
    }

    glEnd();

    glBegin(GL_QUAD_STRIP);

    for(int x = 0; x <= WINDOW_WIDTH; x += 5)
    {
        float y =
            WATER_LEVEL +
            8 * sin(0.022f * x + globalTime * 4.2f);

        glColor4f(0.15f, 0.75f, 0.95f, 0.18f);
        glVertex2f(x, y - 20);

        glColor4f(0.55f, 0.95f, 1.0f, 0.45f);
        glVertex2f(x, y);
    }

    glEnd();

    glBegin(GL_LINE_STRIP);

    glColor4f(0.85f, 1.0f, 1.0f, 0.7f);

    for(int x = 0; x <= WINDOW_WIDTH; x += 4)
    {
        float y =
            WATER_LEVEL +
            5 * sin(0.045f * x + globalTime * 6.0f);

        glVertex2f(x, y + 8);
    }

    glEnd();

    glDisable(GL_BLEND);
}

void drawBoatwoman()
{
    glPushMatrix();

    float rowing = sin(globalTime * 2.2f);
    float armAngle = rowing * 22.0f;
    float torsoLean = rowing * 4.0f;

    glTranslatef(0, 0, 0);
    glRotatef(torsoLean, 0, 0, 1);

    glBegin(GL_POLYGON);

    glColor3f(1.0f, 0.41f, 0.71f);
    glVertex2f(-34, -10);

    glColor3f(1.0f, 0.55f, 0.78f);
    glVertex2f(-48, -58);

    glColor3f(0.92f, 0.30f, 0.62f);
    glVertex2f(-25, -72);

    glColor3f(1.0f, 0.60f, 0.82f);
    glVertex2f(0, -78);

    glColor3f(0.92f, 0.30f, 0.62f);
    glVertex2f(25, -72);

    glColor3f(1.0f, 0.55f, 0.78f);
    glVertex2f(48, -58);

    glColor3f(1.0f, 0.41f, 0.71f);
    glVertex2f(34, -10);

    glEnd();

    glColor3f(0.86f, 0.18f, 0.56f);

    glBegin(GL_LINES);

    glVertex2f(-20, -15);
    glVertex2f(-10, -70);

    glVertex2f(0, -15);
    glVertex2f(0, -76);

    glVertex2f(20, -15);
    glVertex2f(10, -70);

    glEnd();

    glBegin(GL_POLYGON);

    glColor3f(1.0f, 0.41f, 0.71f);

    glVertex2f(-20, -10);
    glVertex2f(-14, 28);
    glVertex2f(14, 28);
    glVertex2f(20, -10);

    glEnd();

    glColor3f(0.96f, 0.79f, 0.66f);

    glBegin(GL_QUADS);

    glVertex2f(-4, 28);
    glVertex2f(4, 28);
    glVertex2f(4, 38);
    glVertex2f(-4, 38);

    glEnd();

    glColor3f(0.96f, 0.79f, 0.66f);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(0, 58);

    for(int i = 0; i <= 360; i++)
    {
        float theta = i * 3.1415926f / 180.0f;

        float rx = 18 * cos(theta);
        float ry = 23 * sin(theta);

        glVertex2f(rx, 58 + ry);
    }

    glEnd();

    glColor3f(0.15f, 0.08f, 0.04f);

    glBegin(GL_POLYGON);

    glVertex2f(-18, 62);
    glVertex2f(-12, 80);
    glVertex2f(12, 80);
    glVertex2f(18, 62);
    glVertex2f(14, 50);
    glVertex2f(-14, 50);

    glEnd();

    drawCircle(0, 82, 9);

    glLineWidth(4.0f);

    glBegin(GL_LINE_STRIP);

    glVertex2f(0, 45);
    glVertex2f(-2, 35);
    glVertex2f(2, 25);
    glVertex2f(-2, 15);
    glVertex2f(0, 5);

    glEnd();

    glLineWidth(1.0f);

    glPointSize(3.0f);

    glBegin(GL_POINTS);

    glColor3f(0.0f, 0.0f, 0.0f);

    glVertex2f(-6, 60);
    glVertex2f(6, 60);

    glEnd();

    glBegin(GL_LINE_STRIP);

    glVertex2f(-5, 50);
    glVertex2f(0, 47);
    glVertex2f(5, 50);

    glEnd();

    glPushMatrix();

    glTranslatef(0, 10, 0);
    glRotatef(armAngle, 0, 0, 1);

    glColor3f(0.96f, 0.79f, 0.66f);

    glBegin(GL_QUADS);

    glVertex2f(-12, 8);
    glVertex2f(-8, 12);
    glVertex2f(-42, -5);
    glVertex2f(-46, -9);

    glEnd();

    glBegin(GL_QUADS);

    glVertex2f(8, 12);
    glVertex2f(12, 8);
    glVertex2f(60, -12);
    glVertex2f(56, -16);

    glEnd();

    glColor3f(0.55f, 0.27f, 0.07f);

    glBegin(GL_QUADS);

    glVertex2f(-45, -7);
    glVertex2f(70, -15);
    glVertex2f(70, -20);
    glVertex2f(-45, -12);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(70, -26);
    glVertex2f(92, -32);
    glVertex2f(98, -14);
    glVertex2f(70, -8);

    glEnd();

    glPopMatrix();

    glPopMatrix();
}

void drawBoat()
{
    float boatY = getWaveHeight(boatX + 120);

    float slope = getWaveSlope(boatX + 120);

    float angle = deg(atan(slope));

    glPushMatrix();

    glTranslatef(boatX, boatY + 20, 0);
    glRotatef(angle * 0.8f, 0, 0, 1);

    glColor4f(0.0f, 0.0f, 0.0f, 0.18f);

    glBegin(GL_POLYGON);

    glVertex2f(10, -12);
    glVertex2f(235, -12);
    glVertex2f(210, -28);
    glVertex2f(30, -28);

    glEnd();

    glBegin(GL_POLYGON);

    glColor3f(0.36f, 0.18f, 0.05f);

    glVertex2f(0, 0);
    glVertex2f(240, 0);
    glVertex2f(210, -42);
    glVertex2f(35, -42);

    glEnd();

    glColor3f(0.52f, 0.28f, 0.09f);

    glBegin(GL_QUADS);

    glVertex2f(0, 0);
    glVertex2f(240, 0);
    glVertex2f(232, 10);
    glVertex2f(8, 10);

    glEnd();

    glColor3f(0.42f, 0.20f, 0.08f);

    glBegin(GL_POLYGON);

    glVertex2f(18, -2);
    glVertex2f(222, -2);
    glVertex2f(198, -28);
    glVertex2f(40, -28);

    glEnd();

    glColor3f(0.60f, 0.35f, 0.12f);

    glBegin(GL_QUADS);

    glVertex2f(70, -8);
    glVertex2f(160, -8);
    glVertex2f(150, -16);
    glVertex2f(80, -16);

    glEnd();

    glPushMatrix();

    glTranslatef(110, 30, 0);
    glScalef(0.9f, 0.9f, 1.0f);

    drawBoatwoman();

    glPopMatrix();

    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawWaves();
    drawBoat();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void update(int value)
{
    globalTime += 0.03f;

    boatX += 1.2f;

    if(boatX > WINDOW_WIDTH + 120)
    {
        boatX = -260.0f;
    }

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(120, 80);

    glutCreateWindow("Animated Water Waves with Boat");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.75f, 0.90f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutTimerFunc(16, update, 0);

    cout << "Simulation started successfully." << endl;

    glutMainLoop();

    return 0;
}