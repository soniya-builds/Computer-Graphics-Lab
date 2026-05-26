#define _USE_MATH_DEFINES
#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int FPS = 60;
const int TIMER_MS = 1000 / FPS;

const float WORLD_LEFT = -100.0f;
const float WORLD_RIGHT = 100.0f;
const float WORLD_BOTTOM = -56.25f;
const float WORLD_TOP = 56.25f;

struct Brick {
    float x, y;
    float width, height;
    float r, g, b;
    bool active;
};

enum GameState { STATE_START, STATE_PLAYING, STATE_GAME_OVER, STATE_VICTORY };
GameState currentState = STATE_START;

int score = 0;
int lives = 3;

float paddleX = 0.0f;
const float paddleY = -48.0f;
const float paddleWidth = 24.0f;
const float paddleHeight = 4.0f;
const float paddleSpeed = 2.5f;

float ballX = 0.0f;
float ballY = -44.0f;
float ballRadius = 2.0f;
float ballVelX = 0.0f;
float ballVelY = 0.0f;
const float ballSpeedConstant = 1.3f;

const int BRICK_ROWS = 5;
const int BRICK_COLS = 10;
const float BRICK_WIDTH = 16.0f;
const float BRICK_HEIGHT = 5.0f;
const float BRICK_GAP = 1.5f;
std::vector<Brick> bricks;

bool keys[256] = { false };
bool specialKeys[256] = { false };

void drawCircle(float cx, float cy, float r, int num_segments, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(num_segments);
        glVertex2f(r * cosf(theta) + cx, r * sinf(theta) + cy);
    }
    glEnd();
}

void drawText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18, float r = 1.0f, float g = 1.0f, float b = 1.0f) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void resetBall() {
    ballX = paddleX;
    ballY = paddleY + (paddleHeight / 2.0f) + ballRadius;
    ballVelX = 0.0f;
    ballVelY = 0.0f;
    if (currentState == STATE_PLAYING) {
        currentState = STATE_START;
    }
}

void initGame() {
    srand(static_cast<unsigned int>(time(NULL)));
    score = 0;
    lives = 3;
    paddleX = 0.0f;
    currentState = STATE_START;
    
    bricks.clear();
    
    float startX = -((BRICK_COLS * (BRICK_WIDTH + BRICK_GAP)) - BRICK_GAP) / 2.0f + (BRICK_WIDTH / 2.0f);
    float startY = WORLD_TOP - 15.0f;
    
    float rowColors[5][3] = {
        { 1.0f, 0.5f, 0.5f },
        { 1.0f, 0.75f, 0.4f },
        { 0.95f, 0.95f, 0.5f },
        { 0.5f, 0.85f, 0.5f },
        { 0.4f, 0.75f, 1.0f }
    };

    for (int r = 0; r < BRICK_ROWS; ++r) {
        for (int c = 0; c < BRICK_COLS; ++c) {
            Brick b;
            b.x = startX + c * (BRICK_WIDTH + BRICK_GAP);
            b.y = startY - r * (BRICK_HEIGHT + BRICK_GAP);
            b.width = BRICK_WIDTH;
            b.height = BRICK_HEIGHT;
            b.r = rowColors[r][0];
            b.g = rowColors[r][1];
            b.b = rowColors[r][2];
            b.active = true;
            bricks.push_back(b);
        }
    }
    resetBall();
}

void launchBall() {
    if (currentState == STATE_START) {
        currentState = STATE_PLAYING;
        ballVelX = ((rand() % 100 - 50) / 100.0f) * 0.8f; 
        ballVelY = ballSpeedConstant;
    }
}

void updateGame() {
    if (keys['a'] || keys['A'] || specialKeys[GLUT_KEY_LEFT]) {
        paddleX -= paddleSpeed;
        if (paddleX - (paddleWidth / 2.0f) < WORLD_LEFT) {
            paddleX = WORLD_LEFT + (paddleWidth / 2.0f);
        }
    }
    if (keys['d'] || keys['D'] || specialKeys[GLUT_KEY_RIGHT]) {
        paddleX += paddleSpeed;
        if (paddleX + (paddleWidth / 2.0f) > WORLD_RIGHT) {
            paddleX = WORLD_RIGHT - (paddleWidth / 2.0f);
        }
    }

    if (currentState == STATE_START) {
        ballX = paddleX;
        ballY = paddleY + (paddleHeight / 2.0f) + ballRadius;
        return;
    }

    if (currentState != STATE_PLAYING) return;

    ballX += ballVelX;
    ballY += ballVelY;

    if (ballX - ballRadius <= WORLD_LEFT) {
        ballX = WORLD_LEFT + ballRadius;
        ballVelX = -ballVelX;
    }
    if (ballX + ballRadius >= WORLD_RIGHT) {
        ballX = WORLD_RIGHT - ballRadius;
        ballVelX = -ballVelX;
    }
    if (ballY + ballRadius >= WORLD_TOP) {
        ballY = WORLD_TOP - ballRadius;
        ballVelY = -ballVelY;
    }
    if (ballY - ballRadius <= WORLD_BOTTOM) {
        lives--;
        if (lives <= 0) {
            currentState = STATE_GAME_OVER;
        } else {
            resetBall();
        }
        return;
    }

    float pLeft = paddleX - (paddleWidth / 2.0f);
    float pRight = paddleX + (paddleWidth / 2.0f);
    float pTop = paddleY + (paddleHeight / 2.0f);
    float pBottom = paddleY - (paddleHeight / 2.0f);

    if (ballX + ballRadius >= pLeft && ballX - ballRadius <= pRight &&
        ballY - ballRadius <= pTop && ballY + ballRadius >= pBottom) {
        
        if (ballVelY < 0) {
            ballY = pTop + ballRadius;
            ballVelY = -ballVelY;
            
            float hittingPosition = (ballX - paddleX) / (paddleWidth / 2.0f); 
            ballVelX = hittingPosition * ballSpeedConstant * 0.9f;
        }
    }

    bool anyBricksLeft = false;
    for (auto& b : bricks) {
        if (!b.active) continue;
        anyBricksLeft = true;

        float bLeft = b.x - (b.width / 2.0f);
        float bRight = b.x + (b.width / 2.0f);
        float bTop = b.y + (b.height / 2.0f);
        float bBottom = b.y - (b.height / 2.0f);

        if (ballX + ballRadius >= bLeft && ballX - ballRadius <= bRight &&
            ballY + ballRadius >= bBottom && ballY - ballRadius <= bTop) {
            
            b.active = false;
            score += 20;

            float overlapX = std::min(ballX + ballRadius - bLeft, bRight - (ballX - ballRadius));
            float overlapY = std::min(ballY + ballRadius - bBottom, bTop - (ballY - ballRadius));

            if (overlapX < overlapY) {
                ballVelX = -ballVelX;
                if (ballVelX > 0) ballX += overlapX; else ballX -= overlapX;
            } else {
                ballVelY = -ballVelY;
                if (ballVelY > 0) ballY += overlapY; else ballY -= overlapY;
            }
            break; 
        }
    }

    if (!anyBricksLeft) {
        currentState = STATE_VICTORY;
    }
}

void display() {
    glClearColor(0.12f, 0.14f, 0.16f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    for (const auto& b : bricks) {
        if (!b.active) continue;
        glColor3f(b.r, b.g, b.b);
        glRectf(b.x - b.width / 2.0f, b.y - b.height / 2.0f, b.x + b.width / 2.0f, b.y + b.height / 2.0f);
    }

    glColor3f(0.92f, 0.94f, 0.95f);
    glRectf(paddleX - paddleWidth / 2.0f, paddleY - paddleHeight / 2.0f, paddleX + paddleWidth / 2.0f, paddleY + paddleHeight / 2.0f);

    drawCircle(ballX, ballY, ballRadius, 24, 1.0f, 0.82f, 0.35f);

    drawText(WORLD_LEFT + 8.0f, WORLD_BOTTOM + 6.0f, "SCORE: " + std::to_string(score));
    drawText(WORLD_RIGHT - 24.0f, WORLD_BOTTOM + 6.0f, "LIVES: " + std::to_string(lives));

    if (currentState == STATE_START) {
        drawText(-22.0f, -20.0f, "PRESS SPACEBAR TO LAUNCH BALL", GLUT_BITMAP_HELVETICA_18, 0.9f, 0.9f, 0.9f);
    }
    else if (currentState == STATE_GAME_OVER) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.75f);
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRectf(WORLD_LEFT, WORLD_BOTTOM, WORLD_RIGHT, WORLD_TOP);
        glDisable(GL_BLEND);
        
        drawText(-14.0f, 4.0f, "G A M E   O V E R", GLUT_BITMAP_TIMES_ROMAN_24, 1.0f, 0.4f, 0.4f);
        drawText(-22.0f, -4.0f, "Press 'R' to Rebuild Grid and Try Again", GLUT_BITMAP_HELVETICA_12);
    }
    else if (currentState == STATE_VICTORY) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.75f);
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRectf(WORLD_LEFT, WORLD_BOTTOM, WORLD_RIGHT, WORLD_TOP);
        glDisable(GL_BLEND);
        
        drawText(-14.0f, 4.0f, "V I C T O R Y !", GLUT_BITMAP_TIMES_ROMAN_24, 0.4f, 0.9f, 0.5f);
        drawText(-16.0f, -4.0f, "Press 'R' to Start Next Round", GLUT_BITMAP_HELVETICA_12);
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

void keyboardDown(unsigned char key, int x, int y) {
    keys[key] = true;
    
    if (key == 32) { 
        launchBall();
    }
    if ((key == 'r' || key == 'R')) {
        initGame();
    }
    if (key == 27) { 
        exit(0);
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void specialKeyDown(int key, int x, int y) {
    specialKeys[key] = true;
}

void specialKeyUp(int key, int x, int y) {
    specialKeys[key] = false;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(80, 40);
    glutCreateWindow("Retro Brick Breaker Project");

    initGame();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
    
    glutTimerFunc(TIMER_MS, timer, 0);

    glutMainLoop();
    return 0;
}