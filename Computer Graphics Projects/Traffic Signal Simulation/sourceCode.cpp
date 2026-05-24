#define _USE_MATH_DEFINES

#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WINDOW_WIDTH  = 1280;
const int WINDOW_HEIGHT = 720;

const float WORLD_LEFT   = -100.0f;
const float WORLD_RIGHT  = 100.0f;
const float WORLD_BOTTOM = -56.25f;
const float WORLD_TOP    = 56.25f;

struct Vec2 {
    float x, y;

    Vec2() : x(0), y(0) {}
    Vec2(float _x, float _y) : x(_x), y(_y) {}
};

struct Color3 {
    float r, g, b;

    Color3() : r(0), g(0), b(0) {}
    Color3(float _r, float _g, float _b)
        : r(_r), g(_g), b(_b) {}
};

float globalTime = 0.0f;
float cameraPanX = 0.0f;
float cameraZoom = 1.0f;

bool isRainMode = true;

enum TrafficState {
    TRAFFIC_GREEN,
    TRAFFIC_YELLOW,
    TRAFFIC_RED
};

TrafficState currentTrafficState = TRAFFIC_GREEN;
int trafficTimer = 0;

struct Vehicle {
    Vec2 pos;
    float speed;
    float length;
    Color3 color;
};

vector<Vehicle> cars;

struct RainDrop {
    Vec2 pos;
    float speed;
};

vector<RainDrop> rainParticles;

void drawCircle(float cx, float cy, float r,
                int segments,
                Color3 col,
                float alpha = 1.0f)
{
    glColor4f(col.r, col.g, col.b, alpha);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(cx, cy);

    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * M_PI * i / segments;

        float x = r * cosf(theta);
        float y = r * sinf(theta);

        glVertex2f(cx + x, cy + y);
    }

    glEnd();
}

void drawGradientRect(float x1, float y1,
                      float x2, float y2,
                      Color3 c1,
                      Color3 c2)
{
    glBegin(GL_QUADS);

    glColor3f(c1.r, c1.g, c1.b);
    glVertex2f(x1, y1);

    glVertex2f(x2, y1);

    glColor3f(c2.r, c2.g, c2.b);
    glVertex2f(x2, y2);

    glVertex2f(x1, y2);

    glEnd();
}

void initSimulation()
{
    srand((unsigned)time(NULL));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < 200; i++) {
        RainDrop r;

        r.pos.x = (rand() % 240) - 120.0f;
        r.pos.y = (rand() % 140) - 70.0f;
        r.speed = 1.5f + (rand() % 100) / 50.0f;

        rainParticles.push_back(r);
    }

    for (int i = 0; i < 5; i++) {
        Vehicle v;

        v.pos = Vec2(-120.0f - i * 30.0f, -8.0f);
        v.speed = 0.3f + i * 0.05f;
        v.length = 12.0f;

        v.color = Color3(
            0.3f + (rand() % 70) / 100.0f,
            0.3f + (rand() % 70) / 100.0f,
            0.3f + (rand() % 70) / 100.0f
        );

        cars.push_back(v);
    }
}

void drawSky()
{
    Color3 top(0.15f, 0.25f, 0.45f);
    Color3 bottom(0.95f, 0.55f, 0.35f);

    drawGradientRect(
        WORLD_LEFT,
        WORLD_BOTTOM,
        WORLD_RIGHT,
        WORLD_TOP,
        top,
        bottom
    );

    drawCircle(
        55.0f,
        30.0f,
        14.0f,
        50,
        Color3(1.0f, 0.9f, 0.6f),
        0.7f
    );
}

void drawRoad()
{
    glColor3f(0.2f, 0.22f, 0.25f);

    glRectf(
        WORLD_LEFT,
        -16.0f,
        WORLD_RIGHT,
        8.0f
    );

    glColor3f(0.95f, 0.8f, 0.2f);

    glLineWidth(3);

    glBegin(GL_LINES);

    glVertex2f(WORLD_LEFT, -4.0f);
    glVertex2f(WORLD_RIGHT, -4.0f);

    glEnd();
}

void drawTree(float x, float y)
{
    glColor3f(0.3f, 0.2f, 0.1f);

    glRectf(x - 1.0f, y, x + 1.0f, y + 10.0f);

    drawCircle(x, y + 14.0f, 6.0f, 20,
               Color3(0.2f, 0.5f, 0.3f));

    drawCircle(x - 3.0f, y + 12.0f, 5.0f, 20,
               Color3(0.25f, 0.55f, 0.35f));

    drawCircle(x + 3.0f, y + 12.0f, 5.0f, 20,
               Color3(0.25f, 0.55f, 0.35f));
}

void drawBuilding(float x, float y,
                  float w, float h)
{
    glColor3f(0.85f, 0.8f, 0.75f);

    glRectf(x, y, x + w, y + h);

    glColor3f(0.25f, 0.25f, 0.3f);

    glBegin(GL_TRIANGLES);

    glVertex2f(x - 2.0f, y + h);
    glVertex2f(x + w + 2.0f, y + h);
    glVertex2f(x + w / 2.0f, y + h + 8.0f);

    glEnd();

    glColor3f(1.0f, 0.9f, 0.6f);

    for (float wx = x + 3; wx < x + w - 3; wx += 6) {
        glRectf(wx, y + 5,
                wx + 3,
                y + 10);
    }
}

void drawTrafficLight()
{
    float x = 30.0f;
    float y = 8.0f;

    glColor3f(0.35f, 0.35f, 0.35f);

    glRectf(
        x - 1.0f,
        y,
        x - 0.2f,
        y + 20.0f
    );

    glColor3f(0.2f, 0.2f, 0.2f);

    glRectf(x, y + 10.0f, x + 3, y + 30.0f);

    Color3 red(0.4f, 0.0f, 0.0f);
    Color3 yellow(0.4f, 0.4f, 0.0f);
    Color3 green(0.0f, 0.4f, 0.0f);

    if (currentTrafficState == TRAFFIC_RED)
        red = Color3(1.0f, 0.0f, 0.0f);

    if (currentTrafficState == TRAFFIC_YELLOW)
        yellow = Color3(1.0f, 1.0f, 0.0f);

    if (currentTrafficState == TRAFFIC_GREEN)
        green = Color3(0.0f, 1.0f, 0.0f);

    drawCircle(x + 1.5f, y + 26, 1.2f, 20, red);
    drawCircle(x + 1.5f, y + 21, 1.2f, 20, yellow);
    drawCircle(x + 1.5f, y + 16, 1.2f, 20, green);
}

void drawVehicle(Vehicle &v)
{
    glPushMatrix();

    glTranslatef(v.pos.x, v.pos.y, 0);

    glColor3f(v.color.r, v.color.g, v.color.b);

    glRectf(
        -v.length / 2,
        0,
        v.length / 2,
        4
    );

    glColor3f(
        v.color.r * 0.8f,
        v.color.g * 0.8f,
        v.color.b * 0.8f
    );

    glRectf(
        -v.length / 4,
        4,
        v.length / 4,
        7
    );

    drawCircle(
        -v.length / 3,
        0,
        1.2f,
        15,
        Color3(0.1f, 0.1f, 0.1f)
    );

    drawCircle(
        v.length / 3,
        0,
        1.2f,
        15,
        Color3(0.1f, 0.1f, 0.1f)
    );

    glPopMatrix();
}

void drawRain()
{
    if (!isRainMode)
        return;

    glColor4f(0.7f, 0.8f, 1.0f, 0.5f);

    glBegin(GL_LINES);

    for (auto &r : rainParticles) {
        glVertex2f(r.pos.x, r.pos.y);
        glVertex2f(r.pos.x - 0.5f, r.pos.y - 3.0f);
    }

    glEnd();
}

void drawScene()
{
    drawSky();

    drawRoad();

    drawBuilding(-80, 8, 30, 20);
    drawBuilding(45, 8, 28, 18);

    drawTree(-95, 8);
    drawTree(-35, 8);

    drawTrafficLight();

    drawTree(85, 8);

    for (auto &v : cars)
        drawVehicle(v);

    drawRain();
}

void updateTraffic()
{
    trafficTimer++;

    if (currentTrafficState == TRAFFIC_GREEN &&
        trafficTimer > 300)
    {
        currentTrafficState = TRAFFIC_YELLOW;
        trafficTimer = 0;
    }

    else if (currentTrafficState == TRAFFIC_YELLOW &&
             trafficTimer > 100)
    {
        currentTrafficState = TRAFFIC_RED;
        trafficTimer = 0;
    }

    else if (currentTrafficState == TRAFFIC_RED &&
             trafficTimer > 300)
    {
        currentTrafficState = TRAFFIC_GREEN;
        trafficTimer = 0;
    }
}

void updateCars()
{
    for (auto &v : cars) {

        bool stop = false;

        if (currentTrafficState != TRAFFIC_GREEN &&
            v.pos.x > -20 &&
            v.pos.x < 10)
        {
            stop = true;
        }

        if (!stop)
            v.pos.x += v.speed;

        if (v.pos.x > 120)
            v.pos.x = -120;
    }
}

void updateRain()
{
    for (auto &r : rainParticles) {

        r.pos.y -= r.speed;
        r.pos.x -= 0.2f;

        if (r.pos.y < WORLD_BOTTOM) {
            r.pos.y = WORLD_TOP;
            r.pos.x = (rand() % 240) - 120.0f;
        }
    }
}

void updateAnimation()
{
    globalTime += 1.0f;

    cameraPanX = sinf(globalTime * 0.008f) * 2.0f;

    cameraZoom = 1.0f +
        (cosf(globalTime * 0.005f) * 0.02f);

    updateTraffic();
    updateCars();
    updateRain();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    glPushMatrix();

    glScalef(cameraZoom, cameraZoom, 1.0f);
    glTranslatef(cameraPanX, 0, 0);

    drawScene();

    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    glOrtho(
        WORLD_LEFT,
        WORLD_RIGHT,
        WORLD_BOTTOM,
        WORLD_TOP,
        -1,
        1
    );

    glMatrixMode(GL_MODELVIEW);
}

void timer(int value)
{
    updateAnimation();

    glutPostRedisplay();

    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key,
              int x,
              int y)
{
    switch (key) {

    case 'r':
    case 'R':
        isRainMode = !isRainMode;
        break;

    case 27:
        exit(0);
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB
    );

    glutInitWindowSize(
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    glutCreateWindow(
        "Traffic Light Simulation"
    );

    initSimulation();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(16, timer, 0);

    glClearColor(0, 0, 0, 1);

    glutMainLoop();

    return 0;
}

