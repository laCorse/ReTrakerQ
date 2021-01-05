#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QFileDialog>
#include <QStandardPaths>
#include <QWidget>
#include <QStringList>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLibrary>
#include <QPluginLoader>
#include <QFileInfoList>
#include <QPixmap>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QResizeEvent>
#include <QImage>
#include <QGraphicsScene>
#include <QThread>

#include <opencv2/opencv.hpp>

#include "humandetector.h"
#include "cvplugininterface.h"
#include "videoprocessor.h"
#include "videoclip.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

//! 公有成员函数
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addToPlaylist(const QList<QUrl> &urls);
//! 槽
private slots:
    void open();

    void on_browseButton_pressed();
    void on_browseButton_2_pressed();
    void on_selectROIButton_pressed();

//! 成员变量
    void on_helpButton_pressed();

    void on_pluginsButton_pressed();


    void on_videoInButton_clicked();

    void on_videoProcessButton_clicked();

    void on_videoInitButton_clicked();

    void on_onButton_clicked();

    void on_offButton_clicked();

protected:
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent * event);
    void resizeEvent(QResizeEvent * event);

private:
    void getPluginsList();

    //把target展示到pixmap上
    void showTarget();

private:
    Ui::MainWindow *ui;
    HumanDetector *humanDtr;
    QMediaPlayer *m_player = nullptr;
    QMediaPlaylist *m_playlist = nullptr;
    cv::Mat target;
    QPixmap pixmap;
    VideoProcessor * processor;
    vector<VideoClip*> videoClips;
    vector<string> pathsOfClips;
    NanoDet * nanoDetector;

};
#endif // MAINWINDOW_H
