#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

const int NUM_FLOORS = 5;
const float FLOOR_HEIGHT = 0.35f;
const float BOTTOM_FLOOR_Y = -0.75f;

float elevatorY = BOTTOM_FLOOR_Y;
float targetY = BOTTOM_FLOOR_Y;
int currentFloor = 0;
int targetFloor = 0;

float doorWidth = 0.12f;
float doorOpenProgress = 0.0f;
bool doorsOpening = false;
bool doorsClosing = false;
int doorWaitTimer = 0;

struct Passenger {
    float x, y;
    int startFloor;
    int destFloor;
    bool inElevator;
    bool finished;
    float r, g, b;
};

std::vector<Passenger> passengers;

float getFloorY(int floor) {
    return BOTTOM_FLOOR_Y + (floor * FLOOR_HEIGHT);
}

void spawnPassenger() {
    if (passengers.size() >= 3) return;
    
    Passenger p;
    p.startFloor = rand() % NUM_FLOORS;
    do {
        p.destFloor = rand() % NUM_FLOORS;
    } while (p.destFloor == p.startFloor);
    
    p.x = -0.7f - ((rand() % 3) * 0.1f);
    p.y = getFloorY(p.startFloor) + 0.02f;
    p.inElevator = false;
    p.finished = false;
    
    p.r = (rand() % 100) / 100.0f;
    p.g = (rand() % 100) / 100.0f;
    p.b = (rand() % 100) / 100.0f;
    
    passengers.push_back(p);
}

void init() {
    glClearColor(0.1f, 0.12f, 0.18f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    srand(time(0));
    spawnPassenger();
}

void drawBuilding() {
    glBegin(GL_QUADS);
    glColor3f(0.18f, 0.22f, 0.30f);
    glVertex2f(-0.25f, BOTTOM_FLOOR_Y);
    glVertex2f(0.25f, BOTTOM_FLOOR_Y);
    glVertex2f(0.25f, BOTTOM_FLOOR_Y + (NUM_FLOORS * FLOOR_HEIGHT) + 0.05f);
    glVertex2f(-0.25f, BOTTOM_FLOOR_Y + (NUM_FLOORS * FLOOR_HEIGHT) + 0.05f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (int i = 0; i <= NUM_FLOORS; ++i) {
        float y = BOTTOM_FLOOR_Y + (i * FLOOR_HEIGHT);
        glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
        glVertex2f(-0.9f, y);
        glVertex2f(0.9f, y);
    }
    
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    glVertex2f(-0.16f, BOTTOM_FLOOR_Y);
    glVertex2f(-0.16f, BOTTOM_FLOOR_Y + (NUM_FLOORS * FLOOR_HEIGHT));
    glVertex2f(0.16f, BOTTOM_FLOOR_Y);
    glVertex2f(0.16f, BOTTOM_FLOOR_Y + (NUM_FLOORS * FLOOR_HEIGHT));
    glEnd();
}

void drawElevator() {
    glBegin(GL_QUADS);
    glColor3f(0.35f, 0.4f, 0.5f);
    glVertex2f(-0.15f, elevatorY);
    glVertex2f(0.15f, elevatorY);
    glVertex2f(0.15f, elevatorY + FLOOR_HEIGHT - 0.02f);
    glVertex2f(-0.15f, elevatorY + FLOOR_HEIGHT - 0.02f);
    
    glColor3f(0.7f, 0.8f, 0.9f);
    glVertex2f(-0.13f, elevatorY + 0.02f);
    glVertex2f(0.13f, elevatorY + 0.02f);
    glVertex2f(0.13f, elevatorY + FLOOR_HEIGHT - 0.04f);
    glVertex2f(-0.13f, elevatorY + FLOOR_HEIGHT - 0.04f);
    glEnd();

    float currentDoorWidth = doorWidth * (1.0f - doorOpenProgress);

    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.55f, 0.65f);
    glVertex2f(-0.13f, elevatorY + 0.02f);
    glVertex2f(-0.13f + currentDoorWidth, elevatorY + 0.02f);
    glVertex2f(-0.13f + currentDoorWidth, elevatorY + FLOOR_HEIGHT - 0.04f);
    glVertex2f(-0.13f, elevatorY + FLOOR_HEIGHT - 0.04f);

    glVertex2f(0.13f, elevatorY + 0.02f);
    glVertex2f(0.13f - currentDoorWidth, elevatorY + 0.02f);
    glVertex2f(0.13f - currentDoorWidth, elevatorY + FLOOR_HEIGHT - 0.04f);
    glVertex2f(0.13f, elevatorY + FLOOR_HEIGHT - 0.04f);
    glEnd();
}

void drawPassengers() {
    for (const auto &p : passengers) {
        if (p.finished) continue;

        glBegin(GL_QUADS);
        glColor3f(p.r, p.g, p.b);
        glVertex2f(p.x - 0.02f, p.y);
        glVertex2f(p.x + 0.02f, p.y);
        glVertex2f(p.x + 0.02f, p.y + 0.12f);
        glVertex2f(p.x - 0.02f, p.y + 0.12f);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(p.x, p.y + 0.16f);
        glVertex2f(p.x - 0.015f, p.y + 0.13f);
        glVertex2f(p.x + 0.015f, p.y + 0.13f);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawBuilding();
    drawElevator();
    drawPassengers();
    
    glutSwapBuffers();
}

void update(int value) {
    if (rand() % 300 == 0) {
        spawnPassenger();
    }

    bool elevatorBusy = (elevatorY != targetY || doorsOpening || doorsClosing || doorWaitTimer > 0);
    
    if (!elevatorBusy && !passengers.empty()) {
        for (auto &p : passengers) {
            if (!p.finished && !p.inElevator && p.startFloor == currentFloor && p.x < -0.15f) {
                doorsOpening = true;
                break;
            }
        }
        
        if (!doorsOpening) {
            for (auto &p : passengers) {
                if (!p.finished) {
                    if (p.inElevator) {
                        targetFloor = p.destFloor;
                        targetY = getFloorY(targetFloor);
                        break;
                    } else {
                        targetFloor = p.startFloor;
                        targetY = getFloorY(targetFloor);
                        break;
                    }
                }
            }
        }
    }

    if (elevatorY < targetY) {
        elevatorY += 0.005f;
        if (elevatorY >= targetY) {
            elevatorY = targetY;
            currentFloor = targetFloor;
            doorsOpening = true;
        }
    } else if (elevatorY > targetY) {
        elevatorY -= 0.005f;
        if (elevatorY <= targetY) {
            elevatorY = targetY;
            currentFloor = targetFloor;
            doorsOpening = true;
        }
    }

    if (doorsOpening) {
        doorOpenProgress += 0.02f;
        if (doorOpenProgress >= 1.0f) {
            doorOpenProgress = 1.0f;
            doorsOpening = false;
            doorWaitTimer = 40; 
        }
    }

    if (doorWaitTimer > 0) {
        bool passengerMoving = false;
        for (auto &p : passengers) {
            if (p.finished) continue;

            if (!p.inElevator && p.startFloor == currentFloor) {
                if (p.x < 0.0f) {
                    p.x += 0.01f;
                    passengerMoving = true;
                    if (p.x >= 0.0f) {
                        p.inElevator = true;
                    }
                }
            }
            else if (p.inElevator && p.destFloor == currentFloor) {
                if (p.x < 0.4f) {
                    p.x += 0.01f;
                    p.y = getFloorY(currentFloor) + 0.02f;
                    passengerMoving = true;
                    if (p.x >= 0.4f) {
                        p.inElevator = false;
                        p.finished = true;
                    }
                }
            }
        }

        if (!passengerMoving) {
            doorWaitTimer--;
            if (doorWaitTimer == 0) {
                doorsClosing = true;
            }
        }
    }

    if (doorsClosing) {
        doorOpenProgress -= 0.02f;
        if (doorOpenProgress <= 0.0f) {
            doorOpenProgress = 0.0f;
            doorsClosing = false;
        }
    }

    for (auto &p : passengers) {
        if (p.inElevator) {
            p.y = elevatorY + 0.02f;
        }
    }

    for (size_t i = 0; i < passengers.size();) {
        if (passengers[i].finished) {
            passengers.erase(passengers.begin() + i);
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
    glutCreateWindow("Elevator Simulation System");
    
    init();
    
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    
    glutMainLoop();
    return 0;
}