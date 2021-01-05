#include "videoclip.h"


VideoClip::VideoClip(QObject *parent):QObject(parent)
{
    frameCount = .0;
    currentFrame = .0;
    fps = .0;
}

VideoClip::~VideoClip()
{
    video.release();
}

bool VideoClip::clear()
{
    frameCount = .0;
    currentFrame = .0;
    fps = .0;
    personFrames.clear();
}

bool VideoClip::setDetector(NanoDet *detector)
{
    nanoDetector = detector;
    return true;
}

bool VideoClip::setVideoPath(QString pathFromDialog)
{
    qpath = pathFromDialog;
    path = pathFromDialog.toStdString();
    return true;
}

bool VideoClip::setVideoPath(string pathFromDialog)
{
    path = pathFromDialog;
    qpath = QString::fromStdString(pathFromDialog);
    return true;
}

bool VideoClip::setVideo()
{
    video.open(path);
    if(!video.isOpened())
    {
        return false;
    }

    frameCount = video.get(CAP_PROP_FRAME_COUNT);

}


bool VideoClip::detectPerson()
{
    for(;;)
    {
        Mat frame;
        video.read(frame);
        if (frame.empty()) {
            continue;
        }
        fps = video.get(CAP_PROP_FPS);
        currentFrame = video.get(CAP_PROP_POS_FRAMES);
        if(!processImg(frame,currentFrame))
        {
            cerr << "Frame " << currentFrame << " cannot been process!"<<endl;
        }

        if(currentFrame >= frameCount)
        {
            frameCount = .0;
            currentFrame = .0;
            fps = .0;
            return true;
        }

    }
    return true;
}

bool VideoClip::processImg(Mat & frame,double frameId)
{
    if(!frame.empty())
    {
        detectorUtils utils;
        if(nanoDetector)
        {
            vector<Rect> hmRects;
            vector<float> scores;
            utils.image_human_rect(*nanoDetector,frame,hmRects,scores);
            if(hmRects.size()!=0 || !hmRects.empty())
            {
                personFrame thisFame{frameId,hmRects,scores};
                personFrames.push_back(thisFame);
            }
        }
        else
        {
            return false;
        }

    }
    else
    {
        return false;
    }
    return true;
}


Mat VideoClip::getMaxRect_demo(double frameId)
{
    personFrame tmpFrameInfo{-1,vector<Rect>(),vector<float>()};
    for(auto & frame:personFrames)
    {
        if(frameId == frame.frame)
        {
            tmpFrameInfo = frame;
            break;
        }
    }
    if(tmpFrameInfo.frame!=-1)
    {
        double frameToStart = tmpFrameInfo.frame;
        video.set(CAP_PROP_POS_FRAMES, frameToStart);
        Mat dst;
        video.read(dst);
        return dst(tmpFrameInfo.persons[0]);
    }

}
