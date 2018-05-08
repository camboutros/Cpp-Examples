// $Id: graphics.cpp,v 1.1 2015-07-16 16:47:51-07 - - $

/////////////////////////////////////
// Name: Camellia Boutros          //
// CruzID: cboutros                //
// Class: CMPS 109                 //
// Title: asg4                     //
// Date: 5/16/2016                 //
/////////////////////////////////////

#include <iostream>
using namespace std;

#include <GL/freeglut.h>

#include "graphics.h"
#include "util.h"

int window::width = 640; // in pixels
int window::height = 480; // in pixels
vector<object> window::objects;
size_t window::selected_obj = 0;
mouse window::mus;

rgbcolor window::color_to_use{0xFF, 0x00, 0x00};
int window::border_to_use = 4;
int window::move_to_execute = 4; 
bool window::selected = false;


//void window::move_object(int x_dim, int y_dim){
//   if(selected_obj < objects.size()){
//     objects.at(selected_obj).move(x_dim, y_dim);
//   }
//}

object::object(const shared_ptr<shape>& s, vertex& c, rgbcolor& r){
   pshape = s;
   center = c;
   color = r;
}

// Executed when window system signals to shut down.
void window::close() {
   DEBUGF ('g', sys_info::execname() << ": exit ("
           << sys_info::exit_status() << ")");
   exit (sys_info::exit_status());
}

// Executed when mouse enters or leaves window.
void window::entry (int mouse_entered) {
   DEBUGF ('g', "mouse_entered=" << mouse_entered);
   window::mus.entered = mouse_entered;
   if (window::mus.entered == GLUT_ENTERED) {
      DEBUGF ('g', sys_info::execname() << ": width=" << window::width
           << ", height=" << window::height);
   }
   glutPostRedisplay();
}

//Constructor for object  //
// (MOVED TO HEADER FILE) //
//object::object(const shared_ptr<shape>& ptr,vertex& c,rgbcolor& cl): 
//         pshape(ptr), center(ct, color(cl){}
//////////////////////////////////////////////////

void object::move(GLfloat x_dim, GLfloat y_dim){
   center.xpos += x_dim;
   center.ypos += y_dim;
   if (center.xpos > window::getwidth()){
      center.xpos = 0;
   } else if (center.xpos < window::getwidth()){
      center.xpos = window::getwidth();
   }
   if (center.ypos > window::getheight()){
      center.ypos = 0;
   } else if (center.ypos < 0){
      center.ypos = window::getheight();
   }
}

// Called to display the objects in the window.
void window::display() {
   glClear (GL_COLOR_BUFFER_BIT);
   //int test_count = 0;
   
   ///////////////////////////// 
   // Test code
   //for (auto& object: window::objects) {
   //   cout << "Test: " << ++test_count << endl; //test 
   //   object.draw();
   //}
   /////////////////////////////
   size_t position = 0;
   for (auto i = window::objects.begin(); i != window::objects.end();
         ++i){
      if(window::selected_obj == position){
         window::selected = true;
      }
      i->draw();
      window::selected = false;
      if (position < 10){
         int wth = glutBitmapWidth(GLUT_BITMAP_8_BY_13, 'c');
         vertex ctr = i->getcenter();
         rgbcolor color = i->getcolor();
         GLubyte inv_red = (GLubyte) (0xFF - color.red);
         GLubyte inv_green = (GLubyte) (0xFF - color.green);
         GLubyte inv_blue = (GLubyte) (0xFF - color.blue);
         rgbcolor inv (inv_red, inv_green, inv_blue);
         glColor3ubv(inv.ubvec);
         glRasterPos2f(ctr.xpos - (wth/2), ctr.ypos - (wth/4));
         glutBitmapString(GLUT_BITMAP_8_BY_13, 
                  reinterpret_cast<const unsigned char*>
                  (to_string(position).c_str()));
      }
      ++position;
   }   

   mus.draw();
   glutSwapBuffers();
}

// Called when window is opened and when resized.
void window::reshape (int width, int height) {
   DEBUGF ('g', "width=" << width << ", height=" << height);
   window::width = width;
   window::height = height;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, window::width, 0, window::height);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window::width, window::height);
   glClearColor (0.25, 0.25, 0.25, 1.0);
   glutPostRedisplay();
}


// Executed when a regular keyboard key is pressed.
enum {BS=8, TAB=9, ESC=27, SPACE=32, DEL=127};
void window::keyboard (GLubyte key, int x, int y) {
   DEBUGF ('g', "key=" << (unsigned)key << ", x=" << x << ", y=" << y);
   window::mus.set (x, y);
   switch (key) {
      case 'Q': case 'q': case ESC:
         window::close();
         break;
      case 'H': case 'h':
         move_object((-1 * move_to_execute), 0);
         break;
      case 'J': case 'j':
         move_object(0, (-1 * move_to_execute) );
         break;
      case 'K': case 'k':
         move_object(0, (1 * move_to_execute));
         break;
      case 'L': case 'l':
         move_object((1 * move_to_execute), 0);
         break;
      case 'N': case 'n': case SPACE: case TAB:
         select_object(++selected_obj); 
         break;
      case 'P': case 'p': case BS:
         select_object(--selected_obj);
         break;
      case '0'...'9':
         select_object (key - '0');
         break;
      default:
         cerr << (unsigned)key << ": invalid keystroke" << endl;
         break;
   }
   glutPostRedisplay();
}


// Executed when a special function key is pressed.
void window::special (int key, int x, int y) {
   DEBUGF ('g', "key=" << key << ", x=" << x << ", y=" << y);
   window::mus.set (x, y);
   switch (key) {
      case GLUT_KEY_LEFT: //move_selected_object (-1, 0); break;
         move_object((-1 * move_to_execute), 0);
         break;
      case GLUT_KEY_DOWN: //move_selected_object (0, -1); break;
         move_object(0, (-1 * move_to_execute) );
         break;
      case GLUT_KEY_UP: //move_selected_object (0, +1); break;
         move_object(0, (1 * move_to_execute));
         break;
      case GLUT_KEY_RIGHT: //move_selected_object (+1, 0); break;
         move_object((1 * move_to_execute), 0);
         break;
      case GLUT_KEY_F1: //select_object (1); break; 
         select_object (1);
         break;
      case GLUT_KEY_F2: //select_object (2); break;
         select_object(2);
         break;
      case GLUT_KEY_F3: //select_object (3); break;
         select_object(3);
         break;
      case GLUT_KEY_F4: //select_object (4); break;
         select_object(4);
         break;
      case GLUT_KEY_F5: //select_object (5); break;
         select_object(5);
         break;
      case GLUT_KEY_F6: //select_object (6); break;
         select_object(6);
         break;
      case GLUT_KEY_F7: //select_object (7); break;
         select_object(7);
         break;
      case GLUT_KEY_F8: //select_object (8); break;
         select_object(8);
         break;
      case GLUT_KEY_F9: //select_object (9); break;
         select_object(9);
         break;
      case GLUT_KEY_F10: //select_object (10); break;
         select_object(10);
         break;
      case GLUT_KEY_F11: //select_object (11); break;
         select_object(11);
         break;
      case GLUT_KEY_F12: //select_object (12); break; 
         select_object(12);
         break;
      default:
         cerr << (unsigned)key << ": invalid function key" << endl;
         break;
   }
   glutPostRedisplay();
}

void window::move_object(int x_dim, int y_dim){
   if(selected_obj < objects.size()){
      objects.at(selected_obj).move(x_dim, y_dim);
   }
}

void window::select_object(size_t object){
   if (object < objects.size()){
      window::selected_obj = object;
   } else if (object == objects.size()){
      window::selected_obj = 0;
   } else if (object > objects.size()){
      size_t siz = objects.size() - 1;
      window::selected_obj = siz;
   }
}

void window::motion (int x, int y) {
   DEBUGF ('g', "x=" << x << ", y=" << y);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::passivemotion (int x, int y) {
   DEBUGF ('g', "x=" << x << ", y=" << y);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::mousefn (int button, int state, int x, int y) {
   DEBUGF ('g', "button=" << button << ", state=" << state
           << ", x=" << x << ", y=" << y);
   window::mus.state (button, state);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::main () {
   static int argc = 0;
   glutInit (&argc, nullptr);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window::width, window::height);
   glutInitWindowPosition (128, 128);
   glutCreateWindow (sys_info::execname().c_str());
   glutCloseFunc (window::close);
   glutEntryFunc (window::entry);
   glutDisplayFunc (window::display);
   glutReshapeFunc (window::reshape);
   glutKeyboardFunc (window::keyboard);
   glutSpecialFunc (window::special);
   glutMotionFunc (window::motion);
   glutPassiveMotionFunc (window::passivemotion);
   glutMouseFunc (window::mousefn);
   DEBUGF ('g', "Calling glutMainLoop()");
   glutMainLoop();
}


void mouse::state (int button, int state) {
   switch (button) {
      case GLUT_LEFT_BUTTON: left_state = state; break;
      case GLUT_MIDDLE_BUTTON: middle_state = state; break;
      case GLUT_RIGHT_BUTTON: right_state = state; break;
   }
}

void mouse::draw() {
   static rgbcolor color ("green");
   ostringstream text;
   text << "(" << xpos << "," << window::height - ypos << ")";
   if (left_state == GLUT_DOWN) text << "L"; 
   if (middle_state == GLUT_DOWN) text << "M"; 
   if (right_state == GLUT_DOWN) text << "R"; 
   if (entered == GLUT_ENTERED) {
      void* font = GLUT_BITMAP_HELVETICA_18;
      glColor3ubv (color.ubvec);
      glRasterPos2i (10, 10);
      glutBitmapString (font, (GLubyte*) text.str().c_str());
   }
}

///////////////////////////////////////
// Found code by Brad Bernard that   //
// successfully dealt with a string  //
// conversion error; using a similar //
// method on line 100                //
///////////////////////////////////////
