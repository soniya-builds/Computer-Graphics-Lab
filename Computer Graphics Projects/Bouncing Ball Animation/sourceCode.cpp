#include <GL/glut.h>
#include <math.h>

float ballX = 0.0f;
float ballY = 0.3f;

float velocityX = 0.006f;
float velocityY = 0.0f;

float gravity = -0.0007f;

float radius = 0.07f;

float leftWall = -0.55f;
float rightWall = 0.55f;
float topWall = 0.55f;
float bottomWall = -0.45f;

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
    glColor3f(1.0f, 1.0f, 1.0f);

    drawCircle(x, y, 0.05f);
    drawCircle(x + 0.05f, y + 0.02f, 0.06f);
    drawCircle(x + 0.10f, y, 0.05f);
}

void drawWindow()
{
    glColor3f(0.45f, 0.80f, 0.95f);

    glBegin(GL_QUADS);

    glVertex2f(leftWall, bottomWall);
    glVertex2f(rightWall, bottomWall);
    glVertex2f(rightWall, topWall);
    glVertex2f(leftWall, topWall);

    glEnd();

    drawCloud(-0.35f, 0.25f);
    drawCloud(0.10f, -0.02f);

    glColor3f(0.9f, 0.9f, 0.9f);

    glLineWidth(6);

    glBegin(GL_LINE_LOOP);

    glVertex2f(leftWall, bottomWall);
    glVertex2f(rightWall, bottomWall);
    glVertex2f(rightWall, topWall);
    glVertex2f(leftWall, topWall);

    glEnd();

    glColor3f(0.85f, 0.85f, 0.85f);

    glBegin(GL_QUADS);

    glVertex2f(-0.68f, -0.48f);
    glVertex2f(-0.57f, -0.42f);
    glVertex2f(-0.57f, 0.58f);
    glVertex2f(-0.68f, 0.64f);

    glEnd();

    glBegin(GL_QUADS);

    glVertex2f(0.57f, -0.42f);
    glVertex2f(0.68f, -0.48f);
    glVertex2f(0.68f, 0.64f);
    glVertex2f(0.57f, 0.58f);

    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    glLineWidth(3);

    glBegin(GL_LINES);

    glVertex2f(-0.63f, -0.45f);
    glVertex2f(-0.63f, 0.61f);

    glVertex2f(0.63f, -0.45f);
    glVertex2f(0.63f, 0.61f);

    glVertex2f(-0.68f, 0.10f);
    glVertex2f(-0.57f, 0.10f);

    glVertex2f(0.57f, 0.10f);
    glVertex2f(0.68f, 0.10f);

    glEnd();

    glColor3f(0.92f, 0.92f, 0.92f);

    glBegin(GL_QUADS);

    glVertex2f(leftWall, -0.50f);
    glVertex2f(rightWall, -0.50f);
    glVertex2f(rightWall + 0.03f, bottomWall);
    glVertex2f(leftWall - 0.03f, bottomWall);

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    glColor3f(0.35f, 0.38f, 0.38f);

    glBegin(GL_QUADS);

    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);

    glEnd();

    drawWindow();

    glColor3f(1.0f, 0.25f, 0.25f);

    drawCircle(ballX, ballY, radius);

    glutSwapBuffers();
}

void update(int value)
{
    velocityY += gravity;

    ballX += velocityX;
    ballY += velocityY;

    if (ballY - radius <= bottomWall)
    {
        ballY = bottomWall + radius;

        velocityY = -velocityY * 0.92f;
    }

    if (ballY + radius >= topWall)
    {
        ballY = topWall - radius;

        velocityY = -velocityY;
    }

    if (ballX + radius >= rightWall)
    {
        ballX = rightWall - radius;

        velocityX = -velocityX;
    }

    if (ballX - radius <= leftWall)
    {
        ballX = leftWall + radius;

        velocityX = -velocityX;
    }

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
}

void init()
{
    glClearColor(0.35f, 0.38f, 0.38f, 1.0f);

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

    glutCreateWindow("Bouncing Ball Inside Open Window");

    init();

    glutDisplayFunc(display);

    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}