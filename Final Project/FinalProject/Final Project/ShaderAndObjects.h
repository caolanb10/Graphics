#include <stdlib.h>
#include <string>

//Shaders and Objects directory
std::string shaders = "C:/Users/Caolan/Desktop/Computer Graphics/Final Project/FinalProject/Shaders";
std::string objects = "C:/Users/Caolan/Desktop/Computer Graphics/Final Project/FinalProject/Objects";

//Individual shader files
std::string vs1 = shaders + "/VertexShader.glsl";
std::string fs1 = shaders + "/FragmentShader.glsl";

//3D objects
std::string monk1 = objects + "/monkeyhead_smooth.dae";
std::string cyl1 = objects + "/cyl.dae";

const char * vs = vs1.c_str();
const char * fs = fs1.c_str();
const char * monk = monk1.c_str();
const char * cyl = cyl1.c_str();