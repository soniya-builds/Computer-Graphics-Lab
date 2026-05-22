#include <GL/glut.h>
#include <cmath>

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 700;

const int NUM_SEGMENTS = 90;

float snakeOffsetX = -250.0f;
float animationTime = 0.0f;

const float BASE_Y = 350.0f;

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

void drawHead(float x, float y) {

    glColor3f(0.85f, 0.0f, 0.0f);

    glBegin(GL_POLYGON);

    glVertex2f(x + 30, y);
    glVertex2f(x + 12, y + 20);
    glVertex2f(x - 15, y + 16);
    glVertex2f(x - 28, y);
    glVertex2f(x - 15, y - 16);
    glVertex2f(x + 12, y - 20);

    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);

    glBegin(GL_POLYGON);
    glVertex2f(x + 6, y + 7);
    glVertex2f(x + 12, y + 11);
    glVertex2f(x + 10, y + 4);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + 6, y - 7);
    glVertex2f(x + 12, y - 11);
    glVertex2f(x + 10, y - 4);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);

    glLineWidth(2);

    glBegin(GL_LINES);

    glVertex2f(x + 30, y);
    glVertex2f(x + 42, y + 4);

    glVertex2f(x + 30, y);
    glVertex2f(x + 42, y - 4);

    glEnd();
}

void updateSnake() {

    for (int i = 0; i < NUM_SEGMENTS; i++) {

        float segmentX = snakeOffsetX - i * 7.0f;

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

void drawSnake() {

    glColor3f(1.0f, 0.55f, 0.0f);

    for (int i = NUM_SEGMENTS - 1; i >= 0; i--) {

        drawCircle(
            snake[i].x,
            snake[i].y,
            snake[i].radius,
            40
        );
    }

    glColor3f(0.9f, 0.35f, 0.0f);

    for (int i = NUM_SEGMENTS - 1; i >= 0; i--) {

        float r = snake[i].radius * 0.55f;

        drawCircle(
            snake[i].x,
            snake[i].y,
            r,
            25
        );
    }

    drawHead(snake[0].x, snake[0].y);
}

void display() {

    glClearColor(0.0f, 0.75f, 0.15f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

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

    updateSnake();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    glutCreateWindow("Snake Movement Animation");

    init();

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);

    glutTimerFunc(0, animate, 0);

    glutMainLoop();

    return 0;
}