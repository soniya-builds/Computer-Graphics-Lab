#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

float animationTime = 0.0f;

struct Bubble {
    float x, y;
    float radius;
    float speed;
    float wobbleSpeed;
    float wobbleAmplitude;
};
std::vector<Bubble> bubbles;

struct Fish {
    float x, y;
    float r, g, b;
    float scale;
    float speed;
    bool movingRight;
    float tailPhase;
};
std::vector<Fish> fishes;

void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void drawPlantBlade(float base_x, float base_y, float height, float max_width, float swayOffset) {
    int segments = 15;
    float segHeight = height / segments;

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float current_y = base_y + (i * segHeight);
        float currentSway = sin(animationTime * 2.0f + (i * 0.3f) + swayOffset) * (i * 1.5f);
        float current_center_x = base_x + currentSway;
        float current_width = max_width * (1.0f - (float)i / segments);

        glVertex2f(current_center_x - current_width, current_y);
        glVertex2f(current_center_x + current_width, current_y);
    }
    glEnd();
}

void initEnvironment() {
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < 15; ++i) {
        Bubble b;
        b.x = 200 + rand() % 400;
        b.y = rand() % 600;
        b.radius = 4.0f + (rand() % 6);
        b.speed = 1.0f + (rand() % 100) / 50.0f;
        b.wobbleSpeed = 2.0f + (rand() % 100) / 25.0f;
        b.wobbleAmplitude = 2.0f + (rand() % 5);
        bubbles.push_back(b);
    }

    Fish fish1 = {250.0f, 350.0f, 0.95f, 0.35f, 0.15f, 1.2f, 1.5f, true, 0.0f};
    Fish fish2 = {550.0f, 330.0f, 0.95f, 0.75f, 0.15f, 1.0f, -1.2f, false, 3.14f};
    
    fishes.push_back(fish1);
    fishes.push_back(fish2);
}

void drawBackground() {
    glBegin(GL_QUADS);
    glColor3f(0.44f, 0.78f, 0.86f);
    glVertex2f(0, 600);
    glVertex2f(800, 600);
    glColor3f(0.22f, 0.53f, 0.68f);
    glVertex2f(800, 0);
    glVertex2f(0, 0);
    glEnd();
}

void drawSandBed() {
    glColor3f(0.85f, 0.73f, 0.53f);
    glBegin(GL_QUADS);
    glVertex2f(0, 80);
    glVertex2f(800, 80);
    glVertex2f(800, 0);
    glVertex2f(0, 0);
    glEnd();

    glColor3f(0.78f, 0.66f, 0.46f);
    glBegin(GL_POLYGON);
    glVertex2f(0, 80);
    for(int x = 0; x <= 800; x += 40) {
        float y = 70.0f + sin(x * 0.02f) * 10.0f;
        glVertex2f(x, y);
    }
    glVertex2f(800, 0);
    glVertex2f(0, 0);
    glEnd();
}

void drawScenery() {
    glColor3f(0.25f, 0.28f, 0.3f);
    drawCircle(220, 80, 50, 8);
    drawCircle(320, 70, 40, 6);
    drawCircle(580, 75, 45, 7);

    glColor3f(0.9f, 0.35f, 0.3f);
    glLineWidth(8.0f);
    glBegin(GL_LINES);
    glVertex2f(400, 65); glVertex2f(400, 120);
    glVertex2f(400, 100); glVertex2f(380, 130);
    glVertex2f(400, 90); glVertex2f(425, 125);
    glVertex2f(380, 130); glVertex2f(370, 150);
    glVertex2f(425, 125); glVertex2f(435, 145);
    glEnd();
    glLineWidth(1.0f);
}

void drawFishScene() {
    for (auto &f : fishes) {
        glPushMatrix();
        glTranslatef(f.x, f.y, 0.0f);
        
        if (!f.movingRight) {
            glScalef(-1.0f, 1.0f, 1.0f); 
        }

        float tailSway = sin(animationTime * 8.0f + f.tailPhase) * 15.0f;
        glPushMatrix();
        glTranslatef(-40.0f * f.scale, 0.0f, 0.0f);
        glRotatef(tailSway, 0.0f, 0.0f, 1.0f);
        
        glColor3f(f.r, f.g * 1.1f, f.b);
        glBegin(GL_TRIANGLES);
        glVertex2f(0, 0);
        glVertex2f(-35.0f * f.scale, 25.0f * f.scale);
        glVertex2f(-35.0f * f.scale, -25.0f * f.scale);
        glEnd();
        glPopMatrix();

        glColor3f(f.r, f.g, f.b);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i += 10) {
            float rad = i * 3.14159f / 180.0f;
            glVertex2f(cos(rad) * 45.0f * f.scale, sin(rad) * 30.0f * f.scale);
        }
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle(22.0f * f.scale, 8.0f * f.scale, 7.0f * f.scale, 12);
        glColor3f(0.05f, 0.1f, 0.15f);
        drawCircle(24.0f * f.scale, 8.0f * f.scale, 3.5f * f.scale, 12);

        glColor3f(f.r * 0.9f, f.g * 0.9f, f.b);
        glBegin(GL_TRIANGLES);
        glVertex2f(-5.0f * f.scale, -10.0f * f.scale);
        glVertex2f(-20.0f * f.scale, -30.0f * f.scale);
        glVertex2f(10.0f * f.scale, -25.0f * f.scale);
        glEnd();

        glPopMatrix();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();
    
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    for (const auto &b : bubbles) {
        float dynamicX = b.x + sin(animationTime * b.wobbleSpeed) * b.wobbleAmplitude;
        drawCircle(dynamicX, b.y, b.radius, 12);
    }

    glColor3f(0.18f, 0.52f, 0.22f);
    drawPlantBlade(150, 70, 320, 18, 0.0f);
    drawPlantBlade(190, 70, 240, 14, 1.5f);
    
    glColor3f(0.26f, 0.65f, 0.22f);
    drawPlantBlade(650, 70, 380, 20, 0.7f);
    drawPlantBlade(690, 70, 280, 15, 2.2f);

    drawScenery();
    drawFishScene();
    drawSandBed();

    glutSwapBuffers();
}

void update(int value) {
    animationTime += 0.03f;

    for (auto &b : bubbles) {
        b.y += b.speed;
        if (b.y > 620) {
            b.y = -10;
            b.x = 150 + rand() % 500;
        }
    }

    for (auto &f : fishes) {
        if (f.movingRight) {
            f.x += f.speed;
            if (f.x > 850) f.movingRight = false;
        } else {
            f.x -= std::abs(f.speed);
            if (f.x < -50) f.movingRight = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Organic Fish Aquarium Simulation");
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    initEnvironment();
    
    glutDisplayFunc(display);
    glutReshapeFunc(handleResize);
    glutTimerFunc(16, update, 0);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutMainLoop();
    return 0;
}

