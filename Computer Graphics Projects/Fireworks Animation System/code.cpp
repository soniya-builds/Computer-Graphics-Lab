#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

const int WIDTH = 1400;
const int HEIGHT = 900;

struct Particle
{
    float x, y;
    float dx, dy;
    float r, g, b;
    float life;
    float size;
};

struct Firework
{
    vector<Particle> particles;
};

vector<Firework> fireworks;

float twinkle = 0.0f;

void drawCircle(float x, float y, float radius)
{
    glBegin(GL_POLYGON);

    for (int i = 0; i < 360; i++)
    {
        float angle = i * 3.1416f / 180.0f;

        glVertex2f(
            x + cos(angle) * radius,
            y + sin(angle) * radius);
    }

    glEnd();
}

void drawSky()
{
    glBegin(GL_QUADS);

    glColor3f(0.01f, 0.01f, 0.08f);
    glVertex2f(0, HEIGHT);

    glVertex2f(WIDTH, HEIGHT);

    glColor3f(0.05f, 0.02f, 0.12f);
    glVertex2f(WIDTH, 0);

    glVertex2f(0, 0);

    glEnd();
}

void drawStars()
{
    for (int i = 0; i < 200; i++)
    {
        float x = (i * 137) % WIDTH;
        float y = (i * 97) % HEIGHT;

        float glow =
            0.5f +
            sin(twinkle + i) * 0.5f;

        glColor3f(glow, glow, glow);

        drawCircle(x, y, 1.5f);
    }
}

void drawGround()
{
    glColor3f(0.03f, 0.08f, 0.03f);

    glBegin(GL_QUADS);

    glVertex2f(0, 0);
    glVertex2f(WIDTH, 0);
    glVertex2f(WIDTH, 120);
    glVertex2f(0, 120);

    glEnd();
}

void createFirework()
{
    Firework fw;

    float centerX = 150 + rand() % (WIDTH - 300);
    float centerY = 300 + rand() % 450;

    float baseR = (rand() % 100) / 100.0f;
    float baseG = (rand() % 100) / 100.0f;
    float baseB = (rand() % 100) / 100.0f;

    int count = 120;

    for (int i = 0; i < count; i++)
    {
        float angle =
            (2.0f * 3.1416f * i) / count;

        float speed =
            2.0f + (rand() % 50) / 10.0f;

        Particle p;

        p.x = centerX;
        p.y = centerY;

        p.dx = cos(angle) * speed;
        p.dy = sin(angle) * speed;

        p.r = baseR;
        p.g = baseG;
        p.b = baseB;

        p.life = 1.0f;

        p.size = 2 + rand() % 3;

        fw.particles.push_back(p);
    }

    fireworks.push_back(fw);
}

void updateFireworks()
{
    if (rand() % 20 == 0)
    {
        createFirework();
    }

    for (size_t i = 0; i < fireworks.size(); i++)
    {
        for (size_t j = 0;
             j < fireworks[i].particles.size();
             j++)
        {
            Particle &p =
                fireworks[i].particles[j];

            p.x += p.dx;
            p.y += p.dy;

            p.dy -= 0.02f;

            p.life -= 0.01f;
        }
    }

    for (size_t i = 0; i < fireworks.size();)
    {
        bool alive = false;

        for (size_t j = 0;
             j < fireworks[i].particles.size();
             j++)
        {
            if (fireworks[i].particles[j].life > 0)
            {
                alive = true;
                break;
            }
        }

        if (!alive)
        {
            fireworks.erase(
                fireworks.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

void drawFireworks()
{
    for (size_t i = 0; i < fireworks.size(); i++)
    {
        for (size_t j = 0;
             j < fireworks[i].particles.size();
             j++)
        {
            Particle &p =
                fireworks[i].particles[j];

            if (p.life <= 0)
                continue;

            glColor4f(
                p.r,
                p.g,
                p.b,
                p.life);

            drawCircle(
                p.x,
                p.y,
                p.size);

            glColor4f(
                p.r,
                p.g,
                p.b,
                p.life * 0.2f);

            drawCircle(
                p.x,
                p.y,
                p.size * 3);
        }
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    drawSky();

    drawStars();

    drawGround();

    drawFireworks();

    glutSwapBuffers();
}

void update(int value)
{
    twinkle += 0.03f;

    updateFireworks();

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
}

void init()
{
    glClearColor(0, 0, 0, 1);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char **argv)
{
    srand(time(0));

    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(WIDTH, HEIGHT);

    glutCreateWindow(
        "Fireworks Animation System");

    init();

    glutDisplayFunc(display);

    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}