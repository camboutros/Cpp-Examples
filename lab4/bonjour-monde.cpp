// $Id: bonjour-monde.cpp,v 1.1 2016-05-04 14:14:49-07 - - $

// Draw line from (0,0) to (1,1).

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

struct {
   size_t width = 384;
   size_t height = 256;
} window;

const GLubyte BLEU[] {0, 85, 164};
const GLubyte BLANC[] {0xFF, 0xFF, 0xFF};
const GLubyte ROUGE[] {239, 65, 53, 0};
vector<const GLubyte*> colors {BLEU, BLANC, ROUGE};

void draw_french_flag() {
   for (size_t i = 0; i < 3; ++i) {
      glBegin (GL_POLYGON);
      glColor3ubv (colors[i]);
      glVertex2f (window.width * i / 3.0, 0);
      glVertex2f (window.width * (i + 1) / 3.0, 0);
      glVertex2f (window.width * (i + 1) / 3.0, window.height);
      glVertex2f (window.width * i / 3.0, window.height);
      glEnd();
   }
}

void display() {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear (GL_COLOR_BUFFER_BIT);
   draw_french_flag();
   glutSwapBuffers();
}

void reshape (int width, int height) {
   window.height = height;
   window.width = width;
   glutReshapeWindow (window.width, window.height);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, window.width, 0, window.height);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window.width, window.height);
   glutPostRedisplay();
}


int main (int argc, char** argv) {
   glutInit (&argc, argv);
   glutInitWindowSize (window.width, window.height);
   glutInitWindowPosition (128, 128);
   glutCreateWindow (basename (argv[0]));
   glutDisplayFunc (display);
   glutReshapeFunc (reshape);
   glutMainLoop();
   return 0;
}

