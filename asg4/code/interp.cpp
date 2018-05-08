// $Id: interp.cpp,v 1.2 2015-07-16 16:57:30-07 - - $

/////////////////////////////////////
// Name: Camellia Boutros          //
// CruzID: cboutros                //
// Class: CMPS 109                 //
// Title: asg4                     //
// Date: 5/16/2016                 //
/////////////////////////////////////

#include <memory>
#include <string>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "util.h"

unordered_map<string,interpreter::interpreterfn>
interpreter::interp_map {
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
   {"border" , &interpreter::do_border },
   {"moveby" , &interpreter::do_move_by },
};

unordered_map<string,interpreter::factoryfn>
interpreter::factory_map {
   {"text"     , &interpreter::make_text     },
   {"ellipse"  , &interpreter::make_ellipse  },
   {"circle"   , &interpreter::make_circle   },
   {"polygon"  , &interpreter::make_polygon  },
   {"rectangle", &interpreter::make_rectangle},
   {"square"   , &interpreter::make_square   },
   {"triangle" , &interpreter::make_triangle },
   {"equilateral" , &interpreter::make_equilateral},
   {"diamond" ,  &interpreter::make_diamond},
   {"isoceles" ,      &interpreter::make_isoceles},
};

interpreter::shape_map interpreter::objmap;

interpreter::~interpreter() {
   for (const auto& itor: objmap) {
      cout << "objmap[" << itor.first << "] = "
           << *itor.second << endl;
   }
}

void interpreter::interpret (const parameters& params) {
   DEBUGF ('i', params);
   param begin = params.cbegin();
   string command = *begin;
   auto itor = interp_map.find (command);
   if (itor == interp_map.end()) throw runtime_error ("syntax error");
   interpreterfn func = itor->second; //do_draw or do_define // test
   func (++begin, params.cend()); //sends func name and end
   // works to send just two itors delimiting range because
   // vectors are contiguous
}

void interpreter::do_define (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string name = *begin;
   
   objmap.emplace (name, make_shape (++begin, end));
   
   /// For Testing Purposes: check what's in map ///////
   //for (auto i : objmap){  
   //   cout << "This object: " << endl;  //test
   //   cout << i.first << endl; //test
   //   //cout << (i.second)->diam << endl; //test
   //}
   /////////////////////////////////////////////////////
}


void interpreter::do_draw (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 4) throw runtime_error ("syntax error");
   string name = begin[1];
   shape_map::const_iterator itor = objmap.find (name);
   if (itor == objmap.end()) {
      throw runtime_error (name + ": no such shape");
   }
   rgbcolor color {begin[0]};
   vertex where {from_string<GLfloat> (begin[2]),
                 from_string<GLfloat> (begin[3])};
   //itor->second->draw (where, color);
   window::push_back(object(itor->second, where, color));
}

shape_ptr interpreter::make_shape (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string type = *begin++;
   auto itor = factory_map.find(type);
   if (itor == factory_map.end()) {
      throw runtime_error (type + ": no such shape");
   }
   factoryfn func = itor->second;
   return func (begin, end);
}

shape_ptr interpreter::make_text (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if(end-begin<2){
      throw runtime_error{"Wrong arg param"};
   }
   string font = *begin++;
   string * font_ptr(&font); 
   //*(font_ptr) = font;
   string text_data {};
   
   while(begin != end){
      if (text_data == ""){
         text_data += "";
      } else { 
         text_data += " ";
      }
      text_data += *begin++;
   }
    
   return make_shared<text> (font_ptr, text_data);
}

shape_ptr interpreter::make_ellipse (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   // begin and end should correlate to width and height
   if(end - begin < 2){
      throw runtime_error{"Wrong args param"};
   } 
   GLfloat dim_1 = stof(*begin++);
   GLfloat dim_2 = stof(*begin++);

   return make_shared<ellipse> (dim_1, dim_2);
}

shape_ptr interpreter::make_circle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   // begin and end should both correlate to diameter
   if(end - begin < 1){
      throw runtime_error{"Wrong arg param"};
   }
   
   float diameter = stof(*begin); 
   //float radius = diameter/2;   
   return make_shared<circle> (GLfloat(diameter));
}

shape_ptr interpreter::make_polygon (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vertex_list vertex_array{};
   for(auto i = begin; i != end; i+=2){
      vertex to_push = {GLfloat(stof(*begin)),
         GLfloat(stof(*(++begin)))};
      vertex_array.push_back(to_push);
   }
   return make_shared<polygon> (vertex_array);
}

shape_ptr interpreter::make_rectangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if(end - begin < 2){
      throw runtime_error {"Wrong args param"};
   }
   GLfloat dim_1 = (stof(*begin++));
   GLfloat dim_2 = (stof(*begin++));
   return make_shared<rectangle> (dim_1, dim_2);
}

shape_ptr interpreter::make_square (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   return make_shared<square> (GLfloat());
}

shape_ptr interpreter::make_triangle(param begin, param end){
   if(end - begin < 6){
      throw runtime_error{"Wrong args param"};
   }
   vertex_list vertex_array{};
   while(begin != end){
      GLfloat dimension1 = stof(*begin++);
      GLfloat dimension2 = stof(*begin++);
      vertex_array.push_back({dimension1, dimension2});
   }
   return make_shared<triangle>(vertex_array);
}

shape_ptr interpreter::make_equilateral(param begin, param end){
   if(end-begin < 1){
      throw runtime_error{"Wrong args param"};
   }
   GLfloat length = stof(begin[0]);
   return make_shared<equilateral>(length);
}


shape_ptr interpreter::make_diamond(param begin, param end){
   if (end-begin < 2){
      throw runtime_error{"Wrong args param"};
   }
   GLfloat dimension1 = stof(*begin++);
   GLfloat dimension2 = stof(*begin++);
   return make_shared<diamond>(dimension1, dimension2);
}
shape_ptr interpreter::make_isoceles(param begin, param end){
   if(end-begin < 2){
      throw runtime_error("Wrong arg params");
   }
   GLfloat hypotenuse1 = stof(*begin++);
   GLfloat hypotenuse2 = stof(*begin++);
   return make_shared<isoceles>(hypotenuse1, hypotenuse2);
}
void interpreter::do_border(param begin, param end){
   if (end-begin != 2){
      throw runtime_error("Wrong arg parameters");
   }
   window::color_to_use = rgbcolor{*begin};
   begin++;
   window::border_to_use = stoi(*begin);
}

void interpreter::do_move_by(param begin, param end){
   if(end-begin != 1){
      throw runtime_error("Wrong arg parameters");
   }
   window::move_to_execute = stoi(*begin);
} 
