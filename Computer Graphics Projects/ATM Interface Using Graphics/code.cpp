#include <GL/glut.h>
#include <string>
#include <vector>

enum ATMState {
    WELCOME,
    PIN_ENTRY,
    MAIN_MENU,
    WITHDRAW_MENU,
    DEPOSIT_MENU,
    BALANCE_SCREEN,
    SUCCESS_SCREEN,
    ERROR_SCREEN
};

ATMState currentState = WELCOME;
std::string pinInput = "";
int balance = 5000;
int currentAmountSelection = 0;
std::string statusMessage = "";
int messageTimer = 0;

struct Button {
    float x, y, w, h;
    std::string label;
    int id;
};

std::vector<Button> screenButtons;
std::vector<Button> keypadButtons;

void initATM();
void changeState(ATMState newState);
void handleScreenButton(int id);
void handleKeypadButton(std::string label, int id);

void drawString(float x, float y, const std::string& str, float scale = 0.0005f) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);
    glLineWidth(1.5f);
    for (char c : str) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    }
    glPopMatrix();
}

void drawRect(float x, float y, float w, float h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawOutlineRect(float x, float y, float w, float h, float r, float g, float b, float lineWidth = 2.0f) {
    glLineWidth(lineWidth);
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void initATM() {
    screenButtons.clear();
    keypadButtons.clear();

    for (int i = 0; i < 4; ++i) {
        Button bLeft = { -0.72f, 0.25f - (i * 0.15f), 0.10f, 0.08f, ">", i };
        Button bRight = { 0.62f, 0.25f - (i * 0.15f), 0.10f, 0.08f, "<", i + 4 };
        screenButtons.push_back(bLeft);
        screenButtons.push_back(bRight);
    }

    float keyW = 0.12f, keyH = 0.09f;
    float startX = -0.21f, startY = -0.45f;
    int num = 1;

    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            Button b = { startX + (c * 0.15f), startY - (r * 0.12f), keyW, keyH, std::to_string(num), num };
            keypadButtons.push_back(b);
            num++;
        }
    }

    Button bClear = { startX + 0.45f, startY, 0.16f, keyH, "CLEAR", 10 };
    Button bCancel = { startX + 0.45f, startY - 0.12f, 0.16f, keyH, "CANCEL", 11 };
    Button bEnter = { startX + 0.45f, startY - 0.24f, 0.16f, keyH, "ENTER", 12 };
    Button bZero = { startX + 0.15f, startY - 0.36f, keyW, keyH, "0", 0 };

    keypadButtons.push_back(bClear);
    keypadButtons.push_back(bCancel);
    keypadButtons.push_back(bEnter);
    keypadButtons.push_back(bZero);
}

void init() {
    glClearColor(0.12f, 0.14f, 0.18f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    initATM();
}

void renderScreenContent() {
    drawRect(-0.60f, -0.30f, 1.20f, 0.70f, 0.05f, 0.30f, 0.20f);
    drawOutlineRect(-0.60f, -0.30f, 1.20f, 0.70f, 0.7f, 0.8f, 0.7f, 3.0f);

    glColor3f(0.9f, 1.0f, 0.9f);

    if (currentState == WELCOME) {
        drawString(-0.43f, 0.20f, "BANK OF COMPUTER GRAPHICS", 0.00045f);
        drawString(-0.25f, 0.00f, "Please Insert Your Card", 0.00035f);
        drawString(-0.40f, -0.10f, "[ Press Any Side Button to Insert ]", 0.0003f);
    } 
    else if (currentState == PIN_ENTRY) {
        drawString(-0.22f, 0.20f, "ENTER YOUR PIN", 0.0005f);
        
        drawRect(-0.15f, 0.02f, 0.30f, 0.08f, 0.0f, 0.15f, 0.10f);
        drawOutlineRect(-0.15f, 0.02f, 0.30f, 0.08f, 0.5f, 0.6f, 0.5f, 1.5f);
        
        std::string stars = "";
        for (size_t i = 0; i < pinInput.length(); ++i) stars += "* ";
        glColor3f(1.0f, 1.0f, 1.0f);
        drawString(-0.06f, 0.04f, stars, 0.0005f);

        glColor3f(0.8f, 0.9f, 0.8f);
        drawString(-0.35f, -0.15f, "Use Keypad Below & Press ENTER", 0.0003f);
        drawString(-0.30f, -0.22f, statusMessage, 0.0003f);
    } 
    else if (currentState == MAIN_MENU) {
        drawString(-0.22f, 0.25f, "MAIN INTERFACE", 0.0005f);

        drawString(-0.55f, 0.13f, "Check Balance", 0.00035f);
        drawString(-0.55f, -0.02f, "Cash Deposit", 0.00035f);

        drawString(0.18f, 0.13f, "Cash Withdrawal", 0.00035f);
        drawString(0.40f, -0.02f, "Exit", 0.00035f);
    } 
    else if (currentState == WITHDRAW_MENU) {
        drawString(-0.22f, 0.25f, "SELECT AMOUNT", 0.0005f);

        drawString(-0.55f, 0.13f, "$50", 0.00035f);
        drawString(-0.55f, -0.02f, "$100", 0.00035f);
        drawString(-0.55f, -0.17f, "$200", 0.00035f);

        drawString(0.40f, 0.13f, "$500", 0.00035f);
        drawString(0.35f, -0.02f, "$1000", 0.00035f);
        drawString(0.40f, -0.17f, "Back", 0.00035f);
    } 
    else if (currentState == DEPOSIT_MENU) {
        drawString(-0.27f, 0.25f, "DEPOSIT SIMULATION", 0.0005f);

        drawString(-0.55f, 0.13f, "+ $100", 0.00035f);
        drawString(-0.55f, -0.02f, "+ $500", 0.00035f);
        drawString(-0.55f, -0.17f, "+ $1000", 0.00035f);

        drawString(0.40f, -0.17f, "Back", 0.00035f);
        
        glColor3f(0.8f, 1.0f, 0.8f);
        drawString(-0.20f, 0.05f, "Current: $" + std::to_string(balance), 0.0004f);
    } 
    else if (currentState == BALANCE_SCREEN) {
        drawString(-0.24f, 0.25f, "ACCOUNT BALANCE", 0.0005f);

        drawString(-0.22f, 0.05f, "Available Balance:", 0.00035f);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawString(-0.10f, -0.07f, "$" + std::to_string(balance), 0.0005f);

        glColor3f(0.9f, 1.0f, 0.9f);
        drawString(0.40f, -0.17f, "Back", 0.00035f);
    } 
    else if (currentState == SUCCESS_SCREEN) {
        glColor3f(0.6f, 1.0f, 0.6f);
        drawString(-0.35f, 0.10f, "TRANSACTION SUCCESSFUL", 0.00045f);
        glColor3f(0.9f, 1.0f, 0.9f);
        drawString(-0.25f, -0.05f, "Please take your receipt.", 0.00035f);
    } 
    else if (currentState == ERROR_SCREEN) {
        glColor3f(1.0f, 0.4f, 0.4f);
        drawString(-0.30f, 0.10f, "INSUFFICIENT FUNDS", 0.00045f);
        glColor3f(0.9f, 1.0f, 0.9f);
        drawString(-0.32f, -0.05f, "Transaction has been aborted.", 0.00035f);
    }
}

void drawHardwareFrame() {
    drawRect(-0.80f, -0.95f, 1.60f, 1.90f, 0.70f, 0.72f, 0.75f);
    drawOutlineRect(-0.80f, -0.95f, 1.60f, 1.90f, 0.45f, 0.47f, 0.50f, 4.0f);

    drawRect(-0.75f, -0.38f, 1.50f, 0.85f, 0.35f, 0.37f, 0.40f);
    drawOutlineRect(-0.75f, -0.38f, 1.50f, 0.85f, 0.20f, 0.22f, 0.25f, 2.0f);

    for (const auto& b : screenButtons) {
        drawRect(b.x, b.y, b.w, b.h, 0.25f, 0.27f, 0.30f);
        drawOutlineRect(b.x, b.y, b.w, b.h, 0.85f, 0.87f, 0.90f, 1.5f);
        glColor3f(0.9f, 0.92f, 0.95f);
        drawString(b.x + 0.035f, b.y + 0.025f, b.label, 0.00035f);
    }

    drawRect(-0.45f, -0.90f, 0.90f, 0.48f, 0.25f, 0.27f, 0.30f);
    drawOutlineRect(-0.45f, -0.90f, 0.90f, 0.48f, 0.15f, 0.17f, 0.20f, 2.0f);

    for (const auto& b : keypadButtons) {
        if (b.label == "ENTER") drawRect(b.x, b.y, b.w, b.h, 0.1f, 0.6f, 0.2f);
        else if (b.label == "CANCEL") drawRect(b.x, b.y, b.w, b.h, 0.7f, 0.1f, 0.1f);
        else if (b.label == "CLEAR") drawRect(b.x, b.y, b.w, b.h, 0.8f, 0.6f, 0.0f);
        else drawRect(b.x, b.y, b.w, b.h, 0.5f, 0.52f, 0.55f);

        drawOutlineRect(b.x, b.y, b.w, b.h, 0.8f, 0.82f, 0.85f, 1.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        
        if (b.label.length() > 1) {
            drawString(b.x + 0.01f, b.y + 0.03f, b.label, 0.00022f);
        } else {
            drawString(b.x + 0.045f, b.y + 0.03f, b.label, 0.00035f);
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawHardwareFrame();
    renderScreenContent();
    
    glutSwapBuffers();
}

void changeState(ATMState newState) {
    currentState = newState;
    pinInput = "";
    statusMessage = "";
    glutPostRedisplay();
}

void handleScreenButton(int id) {
    if (currentState == WELCOME) {
        changeState(PIN_ENTRY);
        return;
    }

    if (currentState == MAIN_MENU) {
        if (id == 0) changeState(BALANCE_SCREEN);
        else if (id == 1) changeState(DEPOSIT_MENU);
        else if (id == 4) changeState(WITHDRAW_MENU);
        else if (id == 5) changeState(WELCOME);
    } 
    else if (currentState == WITHDRAW_MENU) {
        if (id == 0) { if (balance >= 50) { balance -= 50; changeState(SUCCESS_SCREEN); } else changeState(ERROR_SCREEN); }
        else if (id == 1) { if (balance >= 100) { balance -= 100; changeState(SUCCESS_SCREEN); } else changeState(ERROR_SCREEN); }
        else if (id == 2) { if (balance >= 200) { balance -= 200; changeState(SUCCESS_SCREEN); } else changeState(ERROR_SCREEN); }
        else if (id == 4) { if (balance >= 500) { balance -= 500; changeState(SUCCESS_SCREEN); } else changeState(ERROR_SCREEN); }
        else if (id == 5) { if (balance >= 1000) { balance -= 1000; changeState(SUCCESS_SCREEN); } else changeState(ERROR_SCREEN); }
        else if (id == 6) changeState(MAIN_MENU);
    } 
    else if (currentState == DEPOSIT_MENU) {
        if (id == 0) { balance += 100; changeState(SUCCESS_SCREEN); }
        else if (id == 1) { balance += 500; changeState(SUCCESS_SCREEN); }
        else if (id == 2) { balance += 1000; changeState(SUCCESS_SCREEN); }
        else if (id == 6) changeState(MAIN_MENU);
    } 
    else if (currentState == BALANCE_SCREEN) {
        if (id == 6) changeState(MAIN_MENU);
    }
}

void handleKeypadButton(std::string label, int id) {
    if (currentState != PIN_ENTRY) {
        if (label == "CANCEL") changeState(WELCOME);
        return;
    }

    if (id >= 0 && id <= 9) {
        if (pinInput.length() < 4) {
            pinInput += label;
        }
    } 
    else if (label == "CLEAR") {
        pinInput = "";
    } 
    else if (label == "CANCEL") {
        changeState(WELCOME);
    } 
    else if (label == "ENTER") {
        if (pinInput == "1234") {
            changeState(MAIN_MENU);
        } else {
            pinInput = "";
            statusMessage = "INVALID PIN! TRY AGAIN.";
        }
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float mx = (x / 400.0f) - 1.0f;
        float my = 1.0f - (y / 300.0f);

        for (const auto& b : screenButtons) {
            if (mx >= b.x && mx <= b.x + b.w && my >= b.y && my <= b.y + b.h) {
                handleScreenButton(b.id);
                glutPostRedisplay();
                return;
            }
        }

        for (const auto& b : keypadButtons) {
            if (mx >= b.x && mx <= b.x + b.w && my >= b.y && my <= b.y + b.h) {
                handleKeypadButton(b.label, b.id);
                glutPostRedisplay();
                return;
            }
        }
    }
}

void update(int value) {
    if (currentState == SUCCESS_SCREEN || currentState == ERROR_SCREEN) {
        messageTimer++;
        if (messageTimer > 120) {
            messageTimer = 0;
            changeState(MAIN_MENU);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Graphical ATM Simulation Interface");
    
    init();
    
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(16, update, 0);
    
    glutMainLoop();
    return 0;
}
//enter your pin to similate