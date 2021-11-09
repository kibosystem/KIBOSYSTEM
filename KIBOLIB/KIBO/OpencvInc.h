#pragma once

#pragma warning(disable : 4819)
#include <opencv2/opencv.hpp>                   // OpenCVƒwƒbƒ_
//#include <opencv2/text.hpp>
#include <vector>
#include <iostream>
#include <opencv2/videoio/videoio_c.h>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world451d.lib")
#else
#pragma comment(lib, "opencv_world451.lib")
#endif

using namespace cv;
using namespace std;
