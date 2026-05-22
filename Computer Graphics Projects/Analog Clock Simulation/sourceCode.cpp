#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

int windowWidth = 800;
int windowHeight = 800;

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

void drawText(const char* text, float x, float y)
{
    glRasterPos2f(x, y);

    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,
                            text[i]);
    }
}

void drawClockFace()
{
    glColor3f(1.0f, 0.96f, 0.97f);
    drawCircle(0.75f);

    glColor3f(0.0f, 0.0f, 0.0f);

    glLineWidth(4);

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i++)
    {
        float angle = i * 3.14159265f / 180.0f;

        glVertex2f(0.75f * cos(angle),
                   0.75f * sin(angle));
    }

    glEnd();

    for (int i = 1; i <= 12; i++)
    {
        float angle = (90 - i * 30) * 3.14159265f / 180.0f;

        float x = 0.54f * cos(angle);
        float y = 0.54f * sin(angle);

        char num[3];

        sprintf(num, "%d", i);

        drawText(num, x - 0.03f, y - 0.02f);
    }

    for (int i = 0; i < 60; i++)
    {
        float angle = i * 6 * 3.14159265f / 180.0f;

        float x1 = 0.70f * cos(angle);
        float y1 = 0.70f * sin(angle);

        float x2, y2;

        if (i % 5 == 0)
        {
            x2 = 0.62f * cos(angle);
            y2 = 0.62f * sin(angle);

            glLineWidth(3);
        }
        else
        {
            x2 = 0.66f * cos(angle);
            y2 = 0.66f * sin(angle);

            glLineWidth(1);
        }

        glBegin(GL_LINES);

        glVertex2f(x1, y1);
        glVertex2f(x2, y2);

        glEnd();
    }
}

void drawEmojiFace()
{
    glColor3f(0.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(-0.18f, 0.15f, 0);
    drawCircle(0.03f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.18f, 0.15f, 0);
    drawCircle(0.03f);
    glPopMatrix();

    glLineWidth(5);

    glBegin(GL_LINE_STRIP);

    for (float angle = 210; angle <= 330; angle += 3)
    {
        float theta = angle * 3.14159265f / 180.0f;

        glVertex2f(0.22f * cos(theta),
                   0.22f * sin(theta) - 0.08f);
    }

    glEnd();
}

void drawHand(float angle,
              float length,
              float width,
              float r,
              float g,
              float b)
{
    glPushMatrix();

    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glColor3f(r, g, b);

    glLineWidth(width);

    glBegin(GL_LINES);

    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, length);

    glEnd();

    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    drawClockFace();

    drawEmojiFace();

    time_t now = time(0);

    struct tm* ltm = localtime(&now);

    int hours = ltm->tm_hour;
    int minutes = ltm->tm_min;
    int seconds = ltm->tm_sec;

    float secondAngle = -seconds * 6.0f;

    float minuteAngle =
        -(minutes * 6.0f +
          seconds * 0.1f);

    float hourAngle =
        -((hours % 12) * 30.0f +
          minutes * 0.5f);

    drawHand(hourAngle,
             0.35f,
             7,
             0.0f,
             0.0f,
             0.0f);

    drawHand(minuteAngle,
             0.50f,
             5,
             0.0f,
             0.0f,
             0.0f);

    drawHand(secondAngle,
             0.60f,
             2,
             1.0f,
             0.0f,
             0.0f);

    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(0.025f);

    glutSwapBuffers();
}

void update(int value)
{
    glutPostRedisplay();

    glutTimerFunc(1000, update, 0);
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

    glutInitWindowSize(windowWidth,
                       windowHeight);

    glutCreateWindow("Cute Emoji Analog Clock");

    init();

    glutDisplayFunc(display);

    glutTimerFunc(1000, update, 0);

    glutMainLoop();

    return 0;
}