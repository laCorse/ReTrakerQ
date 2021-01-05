#include "humandetector.h"

HumanDetector::HumanDetector()
{
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
}


bool HumanDetector::load_image(Mat & src)
{
    if(src.empty())
    {
        qDebug()<<"frame is empty";
        return false;
    }
    else
    {
        image = src;
        return true;
    }
}

bool HumanDetector::detect()
{
    hog.detectMultiScale(image, regions, 0, cv::Size(4,4), cv::Size(32,32), 1.2, 1);

//    for (size_t i = 0; i < regions.size(); i++)
//    {
//            cv::rectangle(image, regions[i], cv::Scalar(0,0,255), 2);
//    }
//    cv::imshow("hog", image);
//        cv::waitKey(0);
}
