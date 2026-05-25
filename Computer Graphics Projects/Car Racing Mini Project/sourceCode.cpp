#define _USE_MATH_DEFINES
#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int FPS = 60;
const int TIMER_MS = 1000 / FPS;

const float WORLD_LEFT = -100.0f;
const float WORLD_RIGHT = 100.0f;
const float WORLD_BOTTOM = -56.25f;
const float WORLD_TOP = 56.25f;

const float TRACK_LEFT = -45.0f;
const float TRACK_RIGHT = 45.0f;

struct Obstacle {
    float x, y;
    float width, height;
    float r, g, b;
    bool passed;
};

bool gameOver = false;
int score = 0;
float gameSpeed = 1.2f;
float roadOffset = 0.0f;

float playerX = 0.0f;
float targetX = 0.0f; // Smooth transition target variable
float playerY = -40.0f;
float playerWidth = 14.0f; 
float playerHeight = 22.0f;

std::vector<Obstacle> obstacles;
int spawnTimer = 0;

void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(num_segments);
        glVertex2f(r * cosf(theta) + cx, r * sinf(theta) + cy);
    }
    glEnd();
}

void drawText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void initGame() {
    srand(static_cast<unsigned int>(time(NULL)));
    glClearColor(0.5f, 0.8f, 0.4f, 1.0f); 
    obstacles.clear();
    playerX = 0.0f;
    targetX = 0.0f;
    score = 0;
    gameSpeed = 1.2f;
    gameOver = false;
    spawnTimer = 0;
}

void spawnObstacle() {
    Obstacle obs;
    int lane = rand() % 3; 
    if (lane == 0) obs.x = -30.0f;
    else if (lane == 1) obs.x = 0.0f;
    else obs.x = 30.0f;

    obs.y = WORLD_TOP + 20.0f;
    obs.width = 13.0f;
    obs.height = 20.0f;
    
    int colorType = rand() % 3;
    if (colorType == 0) { obs.r = 1.0f; obs.g = 0.6f; obs.b = 0.8f; } 
    else if (colorType == 1) { obs.r = 1.0f; obs.g = 0.75f; obs.b = 0.85f; } 
    else { obs.r = 0.95f; obs.g = 0.5f; obs.b = 0.7f; } 
    
    obs.passed = false;
    obstacles.push_back(obs);
}

void drawCuteCar(float x, float y, float w, float h, float r, float g, float b, bool isPlayer) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    float bodyW = w * 0.85f;
    float bodyH = h * 0.90f;

    glColor3f(r, g, b);
    drawCircle(0.0f, bodyH/4.0f, bodyW/2.0f, 20); 
    glRectf(-bodyW/2.0f, -bodyH/4.0f, bodyW/2.0f, bodyH/4.0f); 
    drawCircle(0.0f, -bodyH/4.0f, bodyW/2.0f, 20); 

    if (isPlayer) {
        glColor3f(1.0f, 1.0f, 1.0f); 
        drawCircle(-bodyW/2.5f, bodyH/2.0f + 1.0f, 1.8f, 12);
        drawCircle(bodyW/2.5f, bodyH/2.0f + 1.0f, 1.8f, 12);
    } else {
        glColor3f(1.0f, 1.0f, 0.8f); 
        drawCircle(-bodyW/2.5f, bodyH/2.0f + 0.5f, 1.5f, 10);
        drawCircle(bodyW/2.5f, bodyH/2.0f + 0.5f, 1.5f, 10);
    }

    glColor4f(1.0f, 0.4f, 0.6f, 0.6f); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawCircle(-bodyW/3.0f, bodyH/3.5f, 1.8f, 10);
    drawCircle(bodyW/3.0f, bodyH/3.5f, 1.8f, 10);
    glDisable(GL_BLEND);

    glColor3f(0.3f, 0.7f, 0.9f); 
    glBegin(GL_POLYGON);
    glVertex2f(-bodyW/2.5f, bodyH/6.0f);
    glVertex2f(bodyW/2.5f, bodyH/6.0f);
    glVertex2f(bodyW/3.5f, bodyH/3.0f);
    glVertex2f(-bodyW/3.5f, bodyH/3.0f);
    glEnd();
    
    glRectf(-bodyW/3.0f, -bodyH/3.0f, bodyW/3.0f, -bodyH/4.0f);

    glPopMatrix();
}

void drawScene() {
    glColor3f(0.25f, 0.25f, 0.28f);
    glRectf(TRACK_LEFT, WORLD_BOTTOM, TRACK_RIGHT, WORLD_TOP);

    glColor3f(1.0f, 0.8f, 0.9f); 
    glRectf(TRACK_LEFT - 1.5f, WORLD_BOTTOM, TRACK_LEFT, WORLD_TOP);
    glColor3f(0.8f, 1.0f, 0.9f); 
    glRectf(TRACK_RIGHT, WORLD_BOTTOM, TRACK_RIGHT + 1.5f, WORLD_TOP);

    glColor3f(0.9f, 0.9f, 0.95f);
    for (float y = WORLD_BOTTOM + roadOffset; y < WORLD_TOP + 40.0f; y += 40.0f) {
        glRectf(-16.0f, y, -14.0f, y + 20.0f);
        glRectf(14.0f, y, 16.0f, y + 20.0f);
    }
}

void updateGame() {
    if (gameOver) return;

    roadOffset -= gameSpeed;
    if (roadOffset <= -40.0f) {
        roadOffset += 40.0f;
    }

    // Smooth movement interpolation toward the target lane
    playerX += (targetX - playerX) * 0.25f;

    spawnTimer++;
    if (spawnTimer >= 70) {
        spawnObstacle();
        spawnTimer = 0;
    }

    for (auto it = obstacles.begin(); it != obstacles.end();) {
        it->y -= gameSpeed;

        float xDist = std::fabs(playerX - it->x);
        float yDist = std::fabs(playerY - it->y);
        if (xDist < (playerWidth + it->width) / 2.3f && yDist < (playerHeight + it->height) / 2.3f) {
            gameOver = true;
        }

        if (!it->passed && it->y < playerY) {
            it->passed = true;
            score += 10;
            if (score % 50 == 0) {
                gameSpeed += 0.15f;
            }
        }

        if (it->y < WORLD_BOTTOM - 20.0f) {
            it = obstacles.erase(it);
        } else {
            ++it;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    drawScene();

    for (const auto& obs : obstacles) {
        drawCuteCar(obs.x, obs.y, obs.width, obs.height, obs.r, obs.g, obs.b, false);
    }

    drawCuteCar(playerX, playerY, playerWidth, playerHeight, 1.0f, 0.8f, 0.9f, true);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(WORLD_LEFT + 10.0f, WORLD_TOP - 10.0f, "SCORE: " + std::to_string(score));
    drawText(WORLD_LEFT + 10.0f, WORLD_TOP - 16.0f, "SPEED: " + std::to_string(static_cast<int>(gameSpeed * 10)) + " MPH");

    if (gameOver) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRectf(WORLD_LEFT, WORLD_BOTTOM, WORLD_RIGHT, WORLD_TOP);
        glDisable(GL_BLEND);

        glColor3f(1.0f, 0.4f, 0.6f); 
        drawText(-22.0f, 5.0f, "G A M E   O V E R", GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(-28.0f, -5.0f, "Press 'R' to Restart Mini Game", GLUT_BITMAP_HELVETICA_18);
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(WORLD_LEFT, WORLD_RIGHT, WORLD_BOTTOM, WORLD_TOP, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    updateGame();
    glutPostRedisplay();
    glutTimerFunc(TIMER_MS, timer, 0);
}

void specialKeyboard(int key, int x, int y) {
    if (gameOver) return;
    
    if (key == GLUT_KEY_LEFT) {
        targetX -= 30.0f;
        if (targetX < -30.0f) targetX = -30.0f;
    }
    if (key == GLUT_KEY_RIGHT) {
        targetX += 30.0f;
        if (targetX > 30.0f) targetX = 30.0f;
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (!gameOver) {
        if (key == 'a' || key == 'A') {
            targetX -= 30.0f;
            if (targetX < -30.0f) targetX = -30.0f;
        }
        if (key == 'd' || key == 'D') {
            targetX += 30.0f;
            if (targetX > 30.0f) targetX = 30.0f;
        }
    }
    if ((key == 'r' || key == 'R') && gameOver) {
        initGame();
    }
    if (key == 27) {
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(80, 40);
    glutCreateWindow("Car Racing Mini Game");

    initGame();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeyboard);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(TIMER_MS, timer, 0);

    glutMainLoop();
    return 0;
}
//A te left
//d te right
//R te restart 