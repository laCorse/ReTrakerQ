#ifndef HUMANDETECTOR_H
#define HUMANDETECTOR_H

#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <QDebug>

using namespace std;
using namespace cv;

class HumanDetector
{
public:
    HumanDetector();

    bool load_image(Mat & src);

    bool detect();


    vector<Rect> regions;


private:
    Mat image;
    HOGDescriptor hog;



};

#endif // HUMANDETECTOR_H
