#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#define NUM_BUBBLES 80
#define NUM_FISH 5
#define NUM_PLANTS 6

int width = 1000;
int height = 700;

float plantOffset = 0;

struct Bubble {
    float x, y;
    float speed;
};

struct Fish {
    float x, y;
    float speed;
    float r, g, b;
};

Bubble bubbles[NUM_BUBBLES];
Fish fish[NUM_FISH];

void initBubbles() {
    for (int i = 0; i < NUM_BUBBLES; i++) {
        bubbles[i].x = rand() % width;
        bubbles[i].y = rand() % height;
        bubbles[i].speed = 1 + rand() % 3;
    }
}

void initFish() {
    for (int i = 0; i < NUM_FISH; i++) {
        fish[i].x = rand() % width;
        fish[i].y = 150 + rand() % 400;
        fish[i].speed = 1 + rand() % 3;

        fish[i].r = (rand() % 100) / 100.0f;
        fish[i].g = (rand() % 100) / 100.0f;
        fish[i].b = (rand() % 100) / 100.0f;
    }
}

void drawCircle(float cx, float cy, float r) {
    glBegin(GL_POLYGON);

    for (int i = 0; i < 100; i++) {
        float theta = 2.0f * 3.1416f * i / 100;

        float x = r * cos(theta);
        float y = r * sin(theta);

        glVertex2f(cx + x, cy + y);
    }

    glEnd();
}

void drawFish(float x, float y, float r, float g, float b) {

    glColor3f(r, g, b);

    glBegin(GL_POLYGON);

    for (int i = 0; i < 100; i++) {
        float theta = 2.0f * 3.1416f * i / 100;

        float px = 40 * cos(theta);
        float py = 20 * sin(theta);

        glVertex2f(x + px, y + py);
    }

    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(x - 40, y);
    glVertex2f(x - 70, y + 20);
    glVertex2f(x - 70, y - 20);
    glEnd();

    glColor3f(1, 1, 1);
    drawCircle(x + 20, y + 5, 4);

    glColor3f(0, 0, 0);
    drawCircle(x + 20, y + 5, 2);
}

void drawPlant(float x) {

    glColor3f(0.0f, 0.7f, 0.2f);

    glLineWidth(6);

    glBegin(GL_LINE_STRIP);

    for (int i = 0; i < 120; i += 10) {

        float sway = sin((i + plantOffset) * 0.05f) * 10;

        glVertex2f(x + sway, i);
    }

    glEnd();
}

void drawAquarium() {

    glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.9f, 0.8f, 0.4f);

    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(width, 0);
    glVertex2f(width, 80);
    glVertex2f(0, 80);
    glEnd();

    for (int i = 0; i < NUM_PLANTS; i++) {
        drawPlant(100 + i * 150);
    }

    for (int i = 0; i < NUM_BUBBLES; i++) {

        glColor4f(1, 1, 1, 0.7f);
        drawCircle(bubbles[i].x, bubbles[i].y, 5);
    }

    for (int i = 0; i < NUM_FISH; i++) {

        drawFish(
            fish[i].x,
            fish[i].y,
            fish[i].r,
            fish[i].g,
            fish[i].b
        );
    }

    glutSwapBuffers();
}

void update(int value) {

    for (int i = 0; i < NUM_BUBBLES; i++) {

        bubbles[i].y += bubbles[i].speed;

        if (bubbles[i].y > height) {

            bubbles[i].y = 0;
            bubbles[i].x = rand() % width;
        }
    }

    for (int i = 0; i < NUM_FISH; i++) {

        fish[i].x += fish[i].speed;

        if (fish[i].x > width + 80) {

            fish[i].x = -80;
            fish[i].y = 150 + rand() % 400;
        }
    }

    plantOffset += 2;

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
}

void init() {

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, width, 0, height);
}

int main(int argc, char** argv) {

    srand(time(0));

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(width, height);

    glutCreateWindow("Fish Aquarium Simulation");

    init();

    initBubbles();

    initFish();

    glutDisplayFunc(drawAquarium);

    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}