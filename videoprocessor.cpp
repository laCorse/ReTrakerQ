
#include "videoprocessor.h"

VideoProcessor::VideoProcessor(QObject *parent) : QObject(parent)
{
    //hmDtr = new HumanDetector();
    hmDtr = nullptr;
    nanoDetector = nullptr;
}

bool VideoProcessor::setDetector(NanoDet *detector)
{
    nanoDetector = detector;
}

void VideoProcessor::startVideo()
{
    using namespace cv;
    VideoCapture camera(0);
    Mat inFrame, outFrame;
    stopped = false;
    while(camera.isOpened() && !stopped)
    {
        camera >> inFrame;
        if(inFrame.empty())
            continue;

        //processImg(inFrame, outFrame);
        detectorUtils utils;
        utils.image_demo_show(*nanoDetector,inFrame, outFrame);
        emit inDisplay(
                    QPixmap::fromImage(
                        QImage(
                            inFrame.data,
                            inFrame.cols,
                            inFrame.rows,
                            inFrame.step,
                            QImage::Format_RGB888)
                        .rgbSwapped()));

        emit outDisplay(
                    QPixmap::fromImage(
                        QImage(
                            outFrame.data,
                            outFrame.cols,
                            outFrame.rows,
                            outFrame.step,
                            QImage::Format_RGB888)
                        .rgbSwapped()));
    }
}

void VideoProcessor::stopVideo()
{
    qDebug() << Q_FUNC_INFO;
    stopped = true;
}

void VideoProcessor::goonVideo()
{
    qDebug() << Q_FUNC_INFO;
    stopped = false;
}

bool VideoProcessor::processImg(Mat & in,Mat & out)
{
    if(!in.empty())
    {
        hmDtr->load_image(in);
        hmDtr->detect();
        out = in.clone();
        for (size_t i = 0; i < hmDtr->regions.size(); i++)
        {
                cv::rectangle(out, hmDtr->regions[i], cv::Scalar(0,0,255), 2);
        }
        return true;
    }
    else {
        return false;
    }
}
