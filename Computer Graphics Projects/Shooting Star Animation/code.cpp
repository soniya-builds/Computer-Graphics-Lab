#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct TrailPoint {
    float x, y;
    float alpha;
};

struct ShootingStar {
    float x, y;
    float speedX, speedY;
    float size;          
    float angle;         
    float rotationSpeed;
    float r, g, b; 
    std::vector<TrailPoint> trail;
    int maxTrailLength;

    void reset() {
        x = (rand() % 2000 - 1000) / 1000.0f;
        y = 1.0f;
        
        speedX = (rand() % 100 + 50) / 5000.0f;
        speedY = -((rand() % 100 + 50) / 5000.0f);
        
        size = (rand() % 20 + 15) / 1000.0f; 
        angle = (rand() % 360);
        rotationSpeed = ((rand() % 100) - 50) / 20.0f; 
        
        maxTrailLength = rand() % 30 + 20;
        
        if (rand() % 2 == 0) {
            r = 0.7f; g = 0.9f; b = 1.0f; 
        } else {
            r = 1.0f; g = 0.95f; b = 0.7f; 
        }
        trail.clear();
    }
};

struct BackgroundStar {
    float x, y, size, brightness, twinkleSpeed;
};

std::vector<ShootingStar> shootingStars;
std::vector<BackgroundStar> bgStars;
const int NUM_SHOOTING_STARS = 3;
const int NUM_BG_STARS = 150;

void drawStarShape(float cx, float cy, float outerRadius, float r, float g, float b, float rotationAngle) {
    float innerRadius = outerRadius * 0.4f; 
    int numPoints = 5;
    
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
    glVertex2f(0.0f, 0.0f);            
    
    for (int i = 0; i <= numPoints * 2; ++i) {
        float angle = i * M_PI / numPoints;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        
        if (i % 2 == 0) glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        else glColor4f(r, g, b, 0.9f);

        glVertex2f(cos(angle) * radius, sin(angle) * radius);
    }
    glEnd();
    glPopMatrix();
}

void init() {
    glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    
    for (int i = 0; i < NUM_BG_STARS; ++i) {
        BackgroundStar s;
        s.x = (rand() % 2000 - 1000) / 1000.0f;
        s.y = (rand() % 2000 - 1000) / 1000.0f;
        s.size = (rand() % 20 + 5) / 10.0f;
        s.brightness = (rand() % 100) / 100.0f;
        s.twinkleSpeed = (rand() % 50 + 10) / 1000.0f;
        bgStars.push_back(s);
    }

    for (int i = 0; i < NUM_SHOOTING_STARS; ++i) {
        ShootingStar star;
        star.reset();
        star.x -= (i * 0.4f);
        shootingStars.push_back(star);
    }
}

void drawBackground() {
    glBegin(GL_QUADS);
    glColor3f(0.02f, 0.02f, 0.08f); glVertex2f(-1.0f, 1.0f);
    glColor3f(0.02f, 0.02f, 0.08f); glVertex2f(1.0f, 1.0f);
    glColor3f(0.08f, 0.1f, 0.25f); glVertex2f(1.0f, -1.0f);
    glColor3f(0.08f, 0.1f, 0.25f); glVertex2f(-1.0f, -1.0f);
    glEnd();

    glBegin(GL_POINTS);
    for (auto &s : bgStars) {
        s.brightness += s.twinkleSpeed;
        if (s.brightness > 1.0f || s.brightness < 0.2f) {
            s.twinkleSpeed = -s.twinkleSpeed;
        }
        glColor4f(1.0f, 1.0f, 1.0f, s.brightness);
        glVertex2f(s.x, s.y);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawBackground();

    for (auto &star : shootingStars) {
        glLineWidth(star.size * 150.0f); 
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < star.trail.size(); ++i) {
            float alphaFactor = (float)i / star.trail.size(); 
            glColor4f(star.r, star.g, star.b, star.trail[i].alpha * alphaFactor);
            glVertex2f(star.trail[i].x, star.trail[i].y);
        }
        glEnd();

        drawStarShape(star.x, star.y, star.size, star.r, star.g, star.b, star.angle);
    }

    glutSwapBuffers();
}

void update(int value) {
    for (auto &star : shootingStars) {
        TrailPoint p = { star.x, star.y, 1.0f };
        star.trail.push_back(p);

        star.x += star.speedX;
        star.y += star.speedY;
        star.angle += star.rotationSpeed; 

        for (auto &tp : star.trail) {
            tp.alpha -= 0.02f;
        }

        if (star.trail.size() > star.maxTrailLength) {
            star.trail.erase(star.trail.begin());
        }

        if (star.y < -1.1f || star.x > 1.1f) {
            star.reset();
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Dreamy Ghibli Shooting Stars");
    
    srand(time(0));
    init();
    
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    
    glutMainLoop();
    return 0;
}