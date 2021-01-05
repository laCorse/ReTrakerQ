#ifndef VIDEOCLIP_H
#define VIDEOCLIP_H


#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <QString>
#include <QMessageBox>
#include <QObject>
#include "utils.h"
#include "nanodet.h"
#include "humandetector.h"

using namespace std;
using namespace cv;


struct personFrame
{
    double frame;
    vector<Rect> persons;
    vector<float> detectScore;

};

class VideoClip :public QObject
{
    Q_OBJECT
public:
    explicit VideoClip(QObject *parent = nullptr);
    virtual ~VideoClip();

    bool setVideoPath(QString pathFromDialog);
    bool setVideoPath(string pathFromDialog);
    bool setVideo();
    bool setDetector(NanoDet *detector);
    bool detectPerson();
    Mat getMaxRect_demo(double frameId);

    bool clear();
signals:


public slots:


private:
    string path;
    QString qpath;
    VideoCapture video;
    double frameCount;
    double fps;
    double currentFrame;
    HumanDetector * hmDtr;
    NanoDet * nanoDetector;
    vector<personFrame> personFrames;

    bool processImg(Mat & frame,double frameId);

};

#endif // VIDEOCLIP_H
