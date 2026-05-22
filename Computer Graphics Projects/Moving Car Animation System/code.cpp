#include <GL/glut.h>
#include <cmath>

int windowWidth = 1200;
int windowHeight = 700;

float carX = -250.0f;
float wheelRotation = 0.0f;

void drawCircle(float cx, float cy, float r, int segments = 100)
{
    glBegin(GL_POLYGON);

    for (int i = 0; i < segments; i++)
    {
        float theta = 2.0f * 3.1416f * float(i) / float(segments);

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
    glColor3f(0.2f, 0.2f, 0.2f);

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(windowWidth, 0);
    glVertex2f(windowWidth, 180);
    glVertex2f(0, 180);
    glEnd();

    glLineWidth(5);

    glColor3f(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < windowWidth; i += 80)
    {
        glBegin(GL_LINES);
        glVertex2f(i, 90);
        glVertex2f(i + 40, 90);
        glEnd();
    }
}

void drawTree(float x, float y)
{
    glColor3f(0.4f, 0.2f, 0.0f);

    glBegin(GL_QUADS);
    glVertex2f(x - 10, y);
    glVertex2f(x + 10, y);
    glVertex2f(x + 10, y + 80);
    glVertex2f(x - 10, y + 80);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f);
    drawCircle(x, y + 110, 35);

    glColor3f(0.0f, 0.6f, 0.0f);
    drawCircle(x - 20, y + 95, 30);
    drawCircle(x + 20, y + 95, 30);

    glColor3f(0.0f, 0.45f, 0.0f);
    drawCircle(x, y + 140, 28);
}

void drawTrafficLight(float x, float y)
{
    glColor3f(0.3f, 0.3f, 0.3f);

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 10, y);
    glVertex2f(x + 10, y + 180);
    glVertex2f(x, y + 180);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f);

    glBegin(GL_QUADS);
    glVertex2f(x - 15, y + 120);
    glVertex2f(x + 25, y + 120);
    glVertex2f(x + 25, y + 200);
    glVertex2f(x - 15, y + 200);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(x + 5, y + 180, 8);

    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(x + 5, y + 160, 8);

    glColor3f(0.0f, 1.0f, 0.0f);
    drawCircle(x + 5, y + 140, 8);
}

void drawWheel(float x, float y)
{
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(x, y, 25);

    glColor3f(0.7f, 0.7f, 0.7f);
    drawCircle(x, y, 12);

    glPushMatrix();

    glTranslatef(x, y, 0);
    glRotatef(wheelRotation, 0.0f, 0.0f, 1.0f);

    glColor3f(0.2f, 0.2f, 0.2f);

    for (int i = 0; i < 4; i++)
    {
        glRotatef(90, 0.0f, 0.0f, 1.0f);

        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(18, 0);
        glEnd();
    }

    glPopMatrix();
}

void drawCar()
{
    glPushMatrix();

    glTranslatef(carX, -55, 0);

    glColor3f(0.1f, 0.3f, 0.9f);

    glBegin(GL_POLYGON);
    glVertex2f(250, 220);
    glVertex2f(420, 220);
    glVertex2f(450, 260);
    glVertex2f(450, 310);
    glVertex2f(220, 310);
    glVertex2f(200, 260);
    glEnd();

    glColor3f(0.15f, 0.4f, 0.95f);

    glBegin(GL_POLYGON);
    glVertex2f(250, 310);
    glVertex2f(310, 370);
    glVertex2f(390, 370);
    glVertex2f(430, 310);
    glEnd();

    glColor3f(0.7f, 0.9f, 1.0f);

    glBegin(GL_POLYGON);
    glVertex2f(270, 315);
    glVertex2f(320, 360);
    glVertex2f(350, 360);
    glVertex2f(350, 315);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(360, 315);
    glVertex2f(370, 360);
    glVertex2f(390, 360);
    glVertex2f(415, 315);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.6f);
    drawCircle(445, 270, 8);

    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(205, 270, 8);

    drawWheel(260, 220);
    drawWheel(390, 220);

    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);

    glColor3f(0.2f, 0.7f, 0.2f);

    glBegin(GL_QUADS);
    glVertex2f(0, 180);
    glVertex2f(windowWidth, 180);
    glVertex2f(windowWidth, 450);
    glVertex2f(0, 450);
    glEnd();

    drawCloud(150, 600);
    drawCloud(500, 620);
    drawCloud(900, 590);

    drawTree(120, 200);
    drawTree(300, 220);
    drawTree(520, 210);
    drawTree(760, 215);
    drawTree(1020, 205);

    drawTrafficLight(600, 180);
    drawTrafficLight(950, 180);

    drawRoad();

    drawCar();

    glutSwapBuffers();
}

void update(int value)
{
    carX += 4.0f;

    if (carX > windowWidth)
    {
        carX = -500.0f;
    }

    wheelRotation -= 12.0f;

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

    glutCreateWindow("Moving Sedan Car Animation");

    init();

    glutDisplayFunc(display);

    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}