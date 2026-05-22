#include <GL/glut.h>
#include <cmath>

int windowWidth = 1200;
int windowHeight = 700;

float carX = -500.0f;
float wheelRotation = 0.0f;

void drawCircle(float cx, float cy, float r, int segments = 100)
{
    glBegin(GL_POLYGON);

    for (int i = 0; i < segments; i++)
    {
        float theta = 2.0f * 3.1416f * i / segments;

        float x = r * cosf(theta);
        float y = r * sinf(theta);

        glVertex2f(cx + x, cy + y);
    }

    glEnd();
}

void drawCloud(float x, float y)
{
    glColor3f(1.0f, 1.0f, 1.0f);

    drawCircle(x, y, 25);
    drawCircle(x + 25, y + 10, 30);
    drawCircle(x + 55, y, 25);
    drawCircle(x + 25, y - 10, 25);
}

void drawRoad()
{
    glColor3f(0.18f, 0.18f, 0.18f);

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(windowWidth, 0);
    glVertex2f(windowWidth, 180);
    glVertex2f(0, 180);
    glEnd();

    glLineWidth(5);

    glColor3f(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < windowWidth; i += 90)
    {
        glBegin(GL_LINES);
        glVertex2f(i, 90);
        glVertex2f(i + 45, 90);
        glEnd();
    }
}

void drawTree(float x, float y)
{
    glColor3f(0.35f, 0.18f, 0.02f);

    glBegin(GL_QUADS);
    glVertex2f(x - 12, y);
    glVertex2f(x + 12, y);
    glVertex2f(x + 12, y + 90);
    glVertex2f(x - 12, y + 90);
    glEnd();

    glColor3f(0.0f, 0.45f, 0.0f);
    drawCircle(x, y + 140, 45);

    glColor3f(0.0f, 0.55f, 0.0f);
    drawCircle(x - 30, y + 120, 40);

    glColor3f(0.0f, 0.65f, 0.0f);
    drawCircle(x + 30, y + 120, 40);

    glColor3f(0.0f, 0.50f, 0.0f);
    drawCircle(x, y + 100, 38);

    glColor3f(0.0f, 0.40f, 0.0f);
    drawCircle(x - 18, y + 155, 30);

    glColor3f(0.0f, 0.60f, 0.0f);
    drawCircle(x + 18, y + 155, 30);
}

void drawTrafficLight(float x, float y)
{
    glColor3f(0.3f, 0.3f, 0.3f);

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 12, y);
    glVertex2f(x + 12, y + 200);
    glVertex2f(x, y + 200);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f);

    glBegin(GL_QUADS);
    glVertex2f(x - 18, y + 130);
    glVertex2f(x + 30, y + 130);
    glVertex2f(x + 30, y + 220);
    glVertex2f(x - 18, y + 220);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(x + 6, y + 200, 9);

    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(x + 6, y + 175, 9);

    glColor3f(0.0f, 1.0f, 0.0f);
    drawCircle(x + 6, y + 150, 9);
}

void drawWheel(float x, float y, float radius)
{
    glColor3f(0.02f, 0.02f, 0.02f);
    drawCircle(x, y, radius);

    glColor3f(0.75f, 0.75f, 0.75f);
    drawCircle(x, y, radius * 0.625f);

    glPushMatrix();

    glTranslatef(x, y, 0);
    glRotatef(wheelRotation, 0, 0, 1);

    glColor3f(0.15f, 0.15f, 0.15f);

    for (int i = 0; i < 5; i++)
    {
        float angle = i * 72.0f * 3.1416f / 180.0f;

        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(cos(angle) * radius * 0.55f,
                   sin(angle) * radius * 0.55f);
        glEnd();
    }

    glPopMatrix();
}

void drawCar()
{
    glPushMatrix();

    glTranslatef(carX, 20, 0);

    float L = 420.0f;

    float wheelRadius = 0.08f * L;

    float frontWheelX = 0.22f * L;
    float rearWheelX = 0.78f * L;

    float wheelY = 60;

    glColor4f(0.0f, 0.0f, 0.0f, 0.25f);

    glBegin(GL_QUADS);
    glVertex2f(0.05f * L, 15);
    glVertex2f(0.95f * L, 15);
    glVertex2f(0.92f * L, 30);
    glVertex2f(0.08f * L, 30);
    glEnd();

    glBegin(GL_POLYGON);

    glColor3f(0.05f, 0.20f, 0.75f);
    glVertex2f(0.00f * L, 95);

    glColor3f(0.10f, 0.40f, 1.0f);
    glVertex2f(0.08f * L, 115);

    glColor3f(0.15f, 0.55f, 1.0f);
    glVertex2f(0.25f * L, 120);

    glColor3f(0.25f, 0.70f, 1.0f);
    glVertex2f(0.40f * L, 175);

    glColor3f(0.20f, 0.65f, 1.0f);
    glVertex2f(0.70f * L, 175);

    glColor3f(0.12f, 0.45f, 1.0f);
    glVertex2f(0.82f * L, 125);

    glColor3f(0.08f, 0.30f, 0.90f);
    glVertex2f(1.00f * L, 105);

    glColor3f(0.05f, 0.20f, 0.75f);
    glVertex2f(1.00f * L, 70);

    glVertex2f(0.90f * L, 65);
    glVertex2f(0.82f * L, 65);

    glVertex2f(0.78f * L, 98);

    glVertex2f(0.66f * L, 100);
    glVertex2f(0.34f * L, 100);

    glVertex2f(0.22f * L, 98);

    glVertex2f(0.18f * L, 65);
    glVertex2f(0.08f * L, 65);

    glVertex2f(0.00f * L, 70);

    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.75f, 0.90f, 1.0f, 0.55f);

    glBegin(GL_POLYGON);
    glVertex2f(0.28f * L, 122);
    glVertex2f(0.40f * L, 170);
    glVertex2f(0.55f * L, 170);
    glVertex2f(0.55f * L, 122);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.56f * L, 122);
    glVertex2f(0.56f * L, 170);
    glVertex2f(0.70f * L, 170);
    glVertex2f(0.80f * L, 122);
    glEnd();

    glDisable(GL_BLEND);

    glColor3f(0.05f, 0.05f, 0.05f);

    glLineWidth(4);

    glBegin(GL_LINES);
    glVertex2f(0.55f * L, 122);
    glVertex2f(0.55f * L, 170);
    glEnd();

    glColor3f(0.15f, 0.15f, 0.15f);

    glBegin(GL_QUADS);
    glVertex2f(0.03f * L, 88);
    glVertex2f(0.08f * L, 88);
    glVertex2f(0.08f * L, 112);
    glVertex2f(0.03f * L, 112);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.7f);
    drawCircle(0.97f * L, 92, 10);

    glColor3f(1.0f, 0.1f, 0.1f);

    glBegin(GL_QUADS);
    glVertex2f(0.00f * L, 80);
    glVertex2f(0.02f * L, 80);
    glVertex2f(0.02f * L, 105);
    glVertex2f(0.00f * L, 105);
    glEnd();

    drawWheel(frontWheelX, wheelY, wheelRadius);
    drawWheel(rearWheelX, wheelY, wheelRadius);

    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);

    glColor3f(0.2f, 0.75f, 0.25f);

    glBegin(GL_QUADS);
    glVertex2f(0, 180);
    glVertex2f(windowWidth, 180);
    glVertex2f(windowWidth, 450);
    glVertex2f(0, 450);
    glEnd();

    drawCloud(150, 600);
    drawCloud(500, 620);
    drawCloud(900, 590);

    drawTree(120, 190);
    drawTree(280, 210);
    drawTree(470, 200);
    drawTree(700, 205);
    drawTree(920, 210);
    drawTree(1080, 190);

    drawTrafficLight(650, 180);
    drawTrafficLight(980, 180);

    drawRoad();

    drawCar();

    glutSwapBuffers();
}

void update(int value)
{
    carX += 4.0f;

    if (carX > windowWidth + 100)
    {
        carX = -500.0f;
    }

    wheelRotation -= 10.0f;

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
}

void init()
{
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(0, windowWidth, 0, windowHeight);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(windowWidth, windowHeight);

    glutCreateWindow("Advanced Sports Car Animation");

    init();

    glutDisplayFunc(display);

    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}