#define _USE_MATH_DEFINES
#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int FPS = 60;
const int TIMER_MS = 1000 / FPS;

const float WORLD_LEFT = -100.0f;
const float WORLD_RIGHT = 100.0f;
const float WORLD_BOTTOM = -56.25f;
const float WORLD_TOP = 56.25f;

enum LaunchState { STATE_COUNTDOWN, STATE_IGNITION, STATE_ASCENT, STATE_CLEARING };

struct Vec2 {
    float x, y;
    Vec2() : x(0), y(0) {}
    Vec2(float x_, float y_) : x(x_), y(y_) {}
};

struct Color3 {
    float r, g, b;
    Color3() : r(0), g(0), b(0) {}
    Color3(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}
};

struct Particle {
    Vec2 pos;
    Vec2 vel;
    Color3 color;
    float size;
    float alpha;
    float life;
    float maxLife;
    bool isSmoke;
};

LaunchState currentState = STATE_COUNTDOWN;
float rocketY = -30.0f;
float rocketVelocity = 0.0f;
float rocketAcceleration = 0.015f;
int countdownTimer = 180; 
float globalTime = 0.0f;
float cameraShake = 0.0f;

std::vector<Particle> particles;

void drawCircle(float cx, float cy, float r, int num_segments, Color3 col, float alpha = 1.0f) {
    glColor4f(col.r, col.g, col.b, alpha);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(num_segments);
        glVertex2f(r * cosf(theta) + cx, r * sinf(theta) + cy);
    }
    glEnd();
}

void drawGradientRect(float x1, float y1, float x2, float y2, Color3 c1, Color3 c2) {
    glBegin(GL_QUADS);
    glColor3f(c1.r, c1.g, c1.b);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glColor3f(c2.r, c2.g, c2.b);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void initSimulation() {
    srand(static_cast<unsigned int>(time(NULL)));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void spawnPlumeParticle(float x, float y, bool heavyEngine) {
    Particle p;
    float angle = (1.5f * M_PI) + (((rand() % 100) - 50) / 100.0f) * 0.45f;
    float speed = heavyEngine ? (2.0f + (rand() % 100) / 40.0f) : (1.0f + (rand() % 100) / 60.0f);
    
    p.pos.x = x + ((rand() % 100) - 50) / 30.0f;
    p.pos.y = y;
    p.vel.x = cosf(angle) * speed;
    p.vel.y = sinf(angle) * speed;
    
    if ((rand() % 100) < 35) {
        p.isSmoke = false;
        p.color.r = 1.0f;
        p.color.g = 0.4f + (rand() % 100) / 200.0f;
        p.color.b = 0.0f;
        p.size = 2.5f + (rand() % 100) / 40.0f;
        p.maxLife = 15.0f + rand() % 15;
    } else {
        p.isSmoke = true;
        float grey = 0.3f + (rand() % 100) / 300.0f;
        p.color.r = grey + 0.1f;
        p.color.g = grey;
        p.color.b = grey;
        p.size = 3.5f + (rand() % 100) / 150.0f;
        p.maxLife = 50.0f + rand() % 40;
    }
    
    p.life = p.maxLife;
    p.alpha = 1.0f;
    particles.push_back(p);
}

void drawEnvironment() {
    drawGradientRect(WORLD_LEFT, WORLD_BOTTOM, WORLD_RIGHT, WORLD_TOP, Color3(0.05f, 0.08f, 0.15f), Color3(0.15f, 0.25f, 0.4f));
    
    for (int i = 0; i < 40; i++) {
        float x = sinf(i * 23.5f) * 90.0f;
        float y = cosf(i * 45.1f) * 45.0f + 10.0f;
        float size = 0.2f + fabs(sinf(i + globalTime * 0.05f)) * 0.3f;
        drawCircle(x, y, size, 4, Color3(1.0f, 1.0f, 1.0f), 0.8f);
    }
    
    drawGradientRect(WORLD_LEFT, WORLD_BOTTOM, WORLD_RIGHT, -42.0f, Color3(0.1f, 0.12f, 0.15f), Color3(0.05f, 0.05f, 0.07f));
}

void drawLaunchPad() {
    glColor3f(0.25f, 0.28f, 0.32f);
    glRectf(-25.0f, -42.0f, 25.0f, -38.0f);
    
    glColor3f(0.18f, 0.2f, 0.22f);
    glRectf(-3.5f, -38.0f, 3.5f, -34.0f);
    
    glColor3f(0.35f, 0.38f, 0.42f);
    glRectf(-22.0f, -38.0f, -14.0f, 15.0f);
    
    glLineWidth(2.0f);
    glColor3f(0.22f, 0.25f, 0.28f);
    glBegin(GL_LINES);
    for (float h = -38.0f; h <= 15.0f; h += 4.0f) {
        glVertex2f(-22.0f, h); glVertex2f(-14.0f, h + 4.0f);
        glVertex2f(-14.0f, h); glVertex2f(-22.0f, h + 4.0f);
    }
    glEnd();
    
    glColor3f(0.3f, 0.33f, 0.36f);
    glRectf(-14.0f, -5.0f, -2.5f, -3.0f);
    glRectf(-14.0f, 8.0f, -2.5f, 10.0f);
}

void drawRocket(float y) {
    if (currentState == STATE_CLEARING) return;

    glPushMatrix();
    glTranslatef(0.0f, y, 0.0f);
    
    glColor3f(0.15f, 0.15f, 0.18f);
    glRectf(-2.8f, -2.5f, -2.0f, 0.0f);
    glRectf(2.0f, -2.5f, 2.8f, 0.0f);
    glRectf(-0.6f, -1.8f, 0.6f, 0.0f);
    
    glColor3f(0.85f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-2.5f, 0.0f); glVertex2f(-4.5f, 0.0f); glVertex2f(-2.5f, 6.0f);
    glVertex2f(2.5f, 0.0f);  glVertex2f(4.5f, 0.0f);  glVertex2f(2.5f, 6.0f);
    glEnd();
    
    drawGradientRect(-2.5f, 0.0f, 2.5f, 24.0f, Color3(0.95f, 0.95f, 0.95f), Color3(0.8f, 0.82f, 0.85f));
    
    glColor3f(0.15f, 0.2f, 0.35f);
    glRectf(-2.5f, 14.0f, 2.5f, 15.5f);
    glRectf(-2.5f, 4.0f, 2.5f, 4.5f);
    
    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.9f, 0.9f);
    glVertex2f(-2.5f, 24.0f);
    glVertex2f(2.5f, 24.0f);
    glColor3f(0.7f, 0.1f, 0.1f);
    glVertex2f(0.0f, 31.0f);
    glEnd();
    
    drawCircle(0.0f, 19.0f, 0.9f, 16, Color3(0.2f, 0.3f, 0.4f));
    drawCircle(0.0f, 19.0f, 0.7f, 16, Color3(0.6f, 0.8f, 0.95f));
    
    glPopMatrix();
}

void drawParticles() {
    for (const auto& p : particles) {
        drawCircle(p.pos.x, p.pos.y, p.size, 12, p.color, p.alpha);
    }
}

void updateSimulation() {
    globalTime += 1.0f;
    
    if (currentState == STATE_COUNTDOWN) {
        countdownTimer--;
        if (countdownTimer <= 30) {
            currentState = STATE_IGNITION;
        }
    }
    
    if (currentState == STATE_IGNITION) {
        countdownTimer--;
        cameraShake = 0.6f;
        for (int i = 0; i < 6; i++) {
            spawnPlumeParticle(-2.4f, rocketY - 2.5f, false);
            spawnPlumeParticle(2.4f, rocketY - 2.5f, false);
            spawnPlumeParticle(0.0f, rocketY - 1.8f, true);
        }
        if (countdownTimer <= 0) {
            currentState = STATE_ASCENT;
        }
    }
    
    if (currentState == STATE_ASCENT) {
        rocketVelocity += rocketAcceleration;
        rocketY += rocketVelocity;
        
        cameraShake = 1.5f / (1.0f + (rocketY + 30.0f) * 0.015f);
        if (cameraShake < 0.15f) cameraShake = 0.15f;
        
        for (int i = 0; i < 15; i++) {
            spawnPlumeParticle(-2.4f, rocketY - 2.5f, false);
            spawnPlumeParticle(2.4f, rocketY - 2.5f, false);
            spawnPlumeParticle(0.0f, rocketY - 1.8f, true);
        }
        
        if (rocketY > WORLD_TOP + 40.0f) {
            currentState = STATE_CLEARING;
        }
    }

    if (currentState == STATE_CLEARING) {
        cameraShake = 0.0f;
        if (particles.empty()) {
            rocketY = -30.0f;
            rocketVelocity = 0.0f;
            countdownTimer = 180;
            currentState = STATE_COUNTDOWN;
        }
    }
    
    for (auto it = particles.begin(); it != particles.end();) {
        it->pos.x += it->vel.x;
        it->pos.y += it->vel.y;
        it->life -= 1.0f;
        
        float ageRatio = it->life / it->maxLife;
        it->alpha = ageRatio;
        
        if (it->isSmoke) {
            it->size += 0.25f;
            it->vel.x *= 0.95f;
            it->vel.y *= 0.95f;
            it->vel.y += 0.015f; 
        } else {
            it->size *= 0.93f;
            it->color.g *= 0.90f; 
        }
        
        if (it->life <= 0.0f || it->alpha <= 0.0f) {
            it = particles.erase(it);
        } else {
            ++it;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    if (cameraShake > 0.0f) {
        float shakeX = ((rand() % 100) - 50) / 100.0f * cameraShake;
        float shakeY = ((rand() % 100) - 50) / 100.0f * cameraShake;
        glTranslatef(shakeX, shakeY, 0.0f);
    }
    
    drawEnvironment();
    drawLaunchPad();
    drawParticles();
    drawRocket(rocketY);
    
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
    updateSimulation();
    glutPostRedisplay();
    glutTimerFunc(TIMER_MS, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(80, 40);
    glutCreateWindow("Rocket Launch Simulation");
    
    initSimulation();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(TIMER_MS, timer, 0);
    
    glutMainLoop();
    return 0;
}