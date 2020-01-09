#ifndef PCH_HPP
#define PCH_HPP

#include <cstdlib> // exit(0);
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::list;
using std::string;
using std::vector;

// * api

#pragma comment(lib, "glew32d.lib")
#include <GL\glew.h>

#pragma comment(lib, "opencv_world400d.lib")
#include <opencv2/opencv.hpp>
// using namespace cv;

#pragma comment(lib, "freeglut.lib")
#include <GL\freeglut.h>

#pragma comment(lib, "fmod_vc.lib") // sound
#include <fmod.hpp>

#endif // PCH_HPP