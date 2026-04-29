#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

int xs, ys, xe, ye;

void display() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.2, 0.0, 0.8);
  glPointSize(2.0);
  glBegin(GL_POINTS);

  float dx = (float)(xe - xs);
  float dy = (float)(ye - ys);

  float m = (dy / dx);
  cout << "Slope m: " << m << endl;

  dx = abs(xe - xs);
  dy = abs(ye - ys);

  int _2Dx = 2 * dx;
  int _2Dy = 2 * dy;

  if (m < 1) {
    if (xs > xe) { swap(xs, xe); }

    int p = _2Dy - dx;
    int xk = xs;
    int yk = ys;

    int k = 0;
    while (k <= dx) {
      glVertex2i(xk, yk);

      if (p < 0) {
        xk = xk + 1;
        p = p + _2Dy;
      } else {
        xk = xk + 1;
        yk = (ye > ys) ? yk + 1 : yk - 1;
        p = p + _2Dy - _2Dx;
      }
      k++;
    }

  } else {
    if (ys > ye) { swap(ys, ye); }

    int p = _2Dx - dy;
    int xk = xs;
    int yk = ys;

    int k = 0;
    while (k <= dy) {
      glVertex2i(xk, yk);

      if (p < 0) {
        yk = yk + 1;
        p = p + _2Dx;
      } else {
        yk = yk + 1;
        xk = (xe > xs) ? xk + 1 : xk - 1;
        p = p + _2Dx - _2Dy;
      }
      k++;
    }
  }

  glEnd();
  glFlush();
}

int main(int argc, char **argv) {
  cout << "Enter xs ys: ";
  cin >> xs >> ys;

  cout << "Enter xe ye: ";
  cin >> xe >> ye;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Bresenham Algorithm m>1");
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 640.0, 0.0, 480.0);
  glutDisplayFunc(display);
  glutMainLoop();

  return 0;
}
//xs ys: 100 100
//xe ye: 200 300

//xs ys: 300 300
//xe ye: 100 100

//xs ys: 100 100
//xe ye: 300 200