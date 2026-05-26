#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Particle {
    float x, y;
    float vx, vy;
    float r, g, b, alpha;
    float size;
};

struct Rocket {
    float x, y;
    float vy;
    float r, g, b;
    bool exploded;
    float explodeHeight;
};

struct BackgroundStar {
    float x, y, size, brightness, twinkleSpeed;
};

std::vector<Particle> particles;
std::vector<Rocket> rockets;
std::vector<BackgroundStar> bgStars;
const int NUM_BG_STARS = 150;

void spawnRocket() {
    Rocket r;
    r.x = (rand() % 1600 - 800) / 1000.0f;
    r.y = -1.0f;
    r.vy = (rand() % 30 + 35) / 1000.0f;
    r.exploded = false;
    r.explodeHeight = (rand() % 1000 - 200) / 1000.0f;
    
    int colorScheme = rand() % 4;
    if (colorScheme == 0) { r.r = 1.0f; r.g = 0.3f; r.b = 0.3f; }
    else if (colorScheme == 1) { r.r = 0.3f; r.g = 1.0f; r.b = 0.4f; }
    else if (colorScheme == 2) { r.r = 0.3f; r.g = 0.6f; r.b = 1.0f; }
    else { r.r = 1.0f; r.g = 0.8f; r.b = 0.2f; }
    
    rockets.push_back(r);
}

void explode(float cx, float cy, float r, float g, float b) {
    int numParticles = rand() % 100 + 150;
    for (int i = 0; i < numParticles; ++i) {
        Particle p;
        p.x = cx;
        p.y = cy;
        
        float angle = (rand() % 3600) * M_PI / 1800.0f;
        float speed = (rand() % 100 + 20) / 10000.0f;
        
        p.vx = cos(angle) * speed;
        p.vy = sin(angle) * speed;
        
        p.r = r + ((rand() % 40 - 20) / 100.0f);
        p.g = g + ((rand() % 40 - 20) / 100.0f);
        p.b = b + ((rand() % 40 - 20) / 100.0f);
        p.alpha = 1.0f;
        p.size = (rand() % 25 + 15) / 10.0f;
        
        particles.push_back(p);
    }
}

void init() {
    glClearColor(0.04f, 0.04f, 0.12f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    
    for (int i = 0; i < NUM_BG_STARS; ++i) {
        BackgroundStar s;
        s.x = (rand() % 2000 - 1000) / 1000.0f;
        s.y = (rand() % 2000 - 1000) / 1000.0f;
        s.size = (rand() % 20 + 5) / 10.0f;
        s.brightness = (rand() % 100) / 100.0f;
        s.twinkleSpeed = (rand() % 40 + 10) / 1000.0f;
        bgStars.push_back(s);
    }
    
    spawnRocket();
}

void drawBackground() {
    glBegin(GL_QUADS);
    glColor3f(0.01f, 0.01f, 0.05f); glVertex2f(-1.0f, 1.0f);
    glColor3f(0.01f, 0.01f, 0.05f); glVertex2f(1.0f, 1.0f);
    glColor3f(0.06f, 0.06f, 0.20f); glVertex2f(1.0f, -1.0f);
    glColor3f(0.06f, 0.06f, 0.20f); glVertex2f(-1.0f, -1.0f);
    glEnd();

    for (auto &s : bgStars) {
        s.brightness += s.twinkleSpeed;
        if (s.brightness > 1.0f || s.brightness < 0.2f) {
            s.twinkleSpeed = -s.twinkleSpeed;
        }
        glPointSize(s.size);
        glBegin(GL_POINTS);
        glColor4f(1.0f, 1.0f, 1.0f, s.brightness);
        glVertex2f(s.x, s.y);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawBackground();

    for (const auto &r : rockets) {
        if (!r.exploded) {
            glPointSize(4.0f);
            glBegin(GL_POINTS);
            glColor4f(1.0f, 1.0f, 0.9f, 1.0f);
            glVertex2f(r.x, r.y);
            glEnd();
            
            glLineWidth(2.0f);
            glBegin(GL_LINES);
            glColor4f(r.r, r.g, r.b, 0.6f); glVertex2f(r.x, r.y);
            glColor4f(r.r, r.g, r.b, 0.0f); glVertex2f(r.x, r.y - 0.05f);
            glEnd();
        }
    }

    for (const auto &p : particles) {
        glPointSize(p.size);
        glBegin(GL_POINTS);
        glColor4f(p.r, p.g, p.b, p.alpha);
        glVertex2f(p.x, p.y);
        glEnd();
    }

    glutSwapBuffers();
}

void update(int value) {
    if (rand() % 45 == 0 && rockets.size() < 4) {
        spawnRocket();
    }

    for (size_t i = 0; i < rockets.size();) {
        if (!rockets[i].exploded) {
            rockets[i].y += rockets[i].vy;
            if (rockets[i].y >= rockets[i].explodeHeight) {
                explode(rockets[i].x, rockets[i].y, rockets[i].r, rockets[i].g, rockets[i].b);
                rockets[i].exploded = true;
            }
            ++i;
        } else {
            rockets.erase(rockets.begin() + i);
        }
    }

    for (size_t i = 0; i < particles.size();) {
        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;
        particles[i].vy -= 0.00015f; 
        particles[i].vx *= 0.985f;    
        particles[i].vy *= 0.985f;    
        particles[i].alpha -= 0.012f; 

        if (particles[i].alpha <= 0.0f) {
            particles.erase(particles.begin() + i);
        } else {
            ++i;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Fireworks System");
    
    srand(time(0));
    init();
    
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    
    glutMainLoop();
    return 0;
}