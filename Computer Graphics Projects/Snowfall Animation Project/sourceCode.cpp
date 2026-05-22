#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#define NUM_SNOWFLAKES 300

// Window size
int width = 800;
int height = 800;

// Snowflake structure
struct Snowflake {
    float x, y;
    float speed;
    float drift;
};

Snowflake snow[NUM_SNOWFLAKES];

// Initialize snowflakes
void initSnow() {
    for (int i = 0; i < NUM_SNOWFLAKES; i++) {
        snow[i].x = rand() % width;
        snow[i].y = rand() % height;
        snow[i].speed = 1 + rand() % 3;
        snow[i].drift = ((rand() % 100) / 100.0f) - 0.5f;
    }
}

// Draw circle for snowflakes
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

// Draw snowy tree
void drawTree() {

    // Tree trunk
    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(370, 100);
    glVertex2f(430, 100);
    glVertex2f(430, 230);
    glVertex2f(370, 230);
    glEnd();

    // Tree layers
    glColor3f(0.0f, 0.5f, 0.3f);

    // Bottom layer
    glBegin(GL_TRIANGLES);
    glVertex2f(200, 230);
    glVertex2f(600, 230);
    glVertex2f(400, 480);
    glEnd();

    // Middle layer
    glBegin(GL_TRIANGLES);
    glVertex2f(250, 380);
    glVertex2f(550, 380);
    glVertex2f(400, 600);
    glEnd();

    // Top layer
    glBegin(GL_TRIANGLES);
    glVertex2f(300, 520);
    glVertex2f(500, 520);
    glVertex2f(400, 740);
    glEnd();

    // Snow on layers
    glColor3f(1.0f, 1.0f, 1.0f);

    glLineWidth(8);

    glBegin(GL_LINE_STRIP);
    glVertex2f(220, 240);
    glVertex2f(300, 220);
    glVertex2f(400, 250);
    glVertex2f(500, 220);
    glVertex2f(580, 240);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(270, 390);
    glVertex2f(340, 370);
    glVertex2f(400, 400);
    glVertex2f(470, 370);
    glVertex2f(530, 390);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(320, 530);
    glVertex2f(370, 510);
    glVertex2f(400, 540);
    glVertex2f(440, 510);
    glVertex2f(480, 530);
    glEnd();

    // Ground snow
    glBegin(GL_POLYGON);
    glVertex2f(0, 100);
    glVertex2f(800, 100);
    glVertex2f(800, 0);
    glVertex2f(0, 0);
    glEnd();
}

// Display function
void display() {

    // Background color
    glClearColor(0.65f, 0.75f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawTree();

    // Draw snowflakes
    glColor3f(1, 1, 1);

    for (int i = 0; i < NUM_SNOWFLAKES; i++) {
        drawCircle(snow[i].x, snow[i].y, 3);
    }

    glutSwapBuffers();
}

// Update snow animation
void update(int value) {

    for (int i = 0; i < NUM_SNOWFLAKES; i++) {

        snow[i].y -= snow[i].speed;
        snow[i].x += snow[i].drift;

        // Reset snowflake to top
        if (snow[i].y < 0) {
            snow[i].y = height;
            snow[i].x = rand() % width;
        }

        // Keep inside screen
        if (snow[i].x < 0)
            snow[i].x = width;

        if (snow[i].x > width)
            snow[i].x = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

// Setup projection
void init() {
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, width, 0, height);
}

// Main function
int main(int argc, char** argv) {

    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(width, height);
    glutCreateWindow("Snowfall Animation");

    initSnow();
    init();

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}