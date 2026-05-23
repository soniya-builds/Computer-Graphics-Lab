#include <GL/glut.h>
#include <cmath>

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 700;

const int NUM_SEGMENTS = 90;

float snakeOffsetX = -250.0f;
float animationTime = 0.0f;

const float BASE_Y = 180.0f;

const float MOVE_SPEED = 3.0f;
const float AMPLITUDE = 38.0f;
const float FREQUENCY = 0.022f;
const float WAVE_SPEED = 0.18f;

struct Segment {
    float x;
    float y;
    float radius;
};

Segment snake[NUM_SEGMENTS];

void drawCircle(float cx, float cy, float r, int segments = 40) {

    glBegin(GL_POLYGON);

    for (int i = 0; i < segments; i++) {

        float theta = 2.0f * 3.1415926f * i / segments;

        float x = r * cos(theta);
        float y = r * sin(theta);

        glVertex2f(cx + x, cy + y);
    }

    glEnd();
}

void drawBackground() {

    glBegin(GL_QUADS);

    glColor3f(0.55f, 0.85f, 1.0f);
    glVertex2f(0, WINDOW_HEIGHT);

    glColor3f(0.75f, 0.95f, 1.0f);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);

    glColor3f(0.90f, 1.0f, 1.0f);
    glVertex2f(WINDOW_WIDTH, 350);

    glColor3f(0.90f, 1.0f, 1.0f);
    glVertex2f(0, 350);

    glEnd();

    glColor3f(0.45f, 0.9f, 0.35f);

    drawCircle(250, 120, 250);
    drawCircle(700, 100, 320);
    drawCircle(1100, 150, 280);

    glBegin(GL_QUADS);

    glColor3f(0.2f, 0.8f, 0.2f);

    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, 180);
    glVertex2f(0, 180);

    glEnd();
}

void drawHead(float x, float y) {

    glColor3f(1.0f, 0.4f, 0.7f);

    drawCircle(x, y, 18, 40);

    glColor3f(1.0f, 0.6f, 0.8f);
    drawCircle(x - 6, y - 8, 4, 30);
    drawCircle(x - 6, y + 8, 4, 30);

    glColor3f(1.0f, 1.0f, 1.0f);

    drawCircle(x + 6, y + 6, 4.5f);
    drawCircle(x + 6, y - 6, 4.5f);

    glColor3f(0.1f, 0.1f, 0.1f);

    drawCircle(x + 7, y + 6, 2.5f);
    drawCircle(x + 7, y - 6, 2.5f);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(x + 8, y + 7, 0.8f);
    drawCircle(x + 8, y - 5, 0.8f);

    glColor3f(1.0f, 0.0f, 0.0f);

    glLineWidth(2);

    glBegin(GL_LINES);

    glVertex2f(x + 18, y);
    glVertex2f(x + 32, y + 5);

    glVertex2f(x + 18, y);
    glVertex2f(x + 32, y - 5);

    glEnd();
}

void updateSnake() {

    for (int i = 0; i < NUM_SEGMENTS; i++) {

        float segmentX = snakeOffsetX - i * 4.5f;

        float segmentY =
            BASE_Y +
            AMPLITUDE *
            sin(FREQUENCY * segmentX + animationTime);

        float radius =
            18.0f - (float(i) / NUM_SEGMENTS) * 15.0f;

        if (radius < 3.0f)
            radius = 3.0f;

        snake[i].x = segmentX;
        snake[i].y = segmentY;
        snake[i].radius = radius;
    }
}

void drawMandalaPattern(float cx, float cy, float r) {
    if (r < 6.0f) return;

    glColor3f(1.0f, 0.95f, 0.6f);
    drawCircle(cx, cy, r * 0.85f, 16);

    glColor3f(1.0f, 0.6f, 0.8f);
    drawCircle(cx, cy, r * 0.70f, 16);

    glColor3f(1.0f, 1.0f, 1.0f);
    for (int j = 0; j < 8; j++) {
        float angle = 2.0f * 3.1415926f * j / 8.0f;
        float petX = cx + (r * 0.45f) * cos(angle);
        float petY = cy + (r * 0.45f) * sin(angle);
        drawCircle(petX, petY, r * 0.22f, 12);
    }

    glColor3f(0.85f, 0.1f, 0.5f);
    for (int j = 0; j < 8; j++) {
        float angle = 2.0f * 3.1415926f * j / 8.0f;
        float petX = cx + (r * 0.45f) * cos(angle);
        float petY = cy + (r * 0.45f) * sin(angle);
        drawCircle(petX, petY, r * 0.12f, 12);
    }

    glColor3f(1.0f, 0.95f, 0.6f);
    drawCircle(cx, cy, r * 0.28f, 12);

    glColor3f(0.85f, 0.1f, 0.5f);
    drawCircle(cx, cy, r * 0.14f, 12);
}

void drawSnake() {

    for (int i = NUM_SEGMENTS - 1; i >= 0; i--) {

        glColor3f(1.0f, 0.4f, 0.7f);

        drawCircle(
            snake[i].x,
            snake[i].y,
            snake[i].radius,
            40
        );

        if (i % 7 == 0 && i > 0) {
            drawMandalaPattern(snake[i].x, snake[i].y, snake[i].radius);
        } else if (i > 0) {
            glColor3f(1.0f, 0.65f, 0.85f);
            drawCircle(snake[i].x, snake[i].y + (snake[i].radius * 0.4f), snake[i].radius * 0.15f, 12);
            drawCircle(snake[i].x, snake[i].y - (snake[i].radius * 0.4f), snake[i].radius * 0.15f, 12);
        }
    }

    drawHead(snake[0].x, snake[0].y);
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();

    drawSnake();

    glutSwapBuffers();
}

void animate(int value) {

    snakeOffsetX += MOVE_SPEED;

    animationTime -= WAVE_SPEED;

    if (snakeOffsetX - NUM_SEGMENTS * 7 > WINDOW_WIDTH + 150)
        snakeOffsetX = -250;

    updateSnake();

    glutPostRedisplay();

    glutTimerFunc(16, animate, 0);
}

void reshape(int w, int h) {

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
}

void init() {

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    updateSnake();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    glutCreateWindow("Pink Snake Movement Animation");

    init();

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);

    glutTimerFunc(0, animate, 0);

    glutMainLoop();

    return 0;
}