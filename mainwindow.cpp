#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define FILTERS_SUBFOLDER "/filter_plugins/"
#define DEBUG


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //window parameters
    resize(1200,800);
    //plugins
    getPluginsList();

    ///tag1
    //支持拖拽
    setAcceptDrops(true);

    nanoDetector = new NanoDet("./nanodet_m.param", "./nanodet_m.bin", true);


    humanDtr = new HumanDetector();




    //connect(ui->inputButton, &QPushButton::clicked, this, &MainWindow::open);

    processor = new VideoProcessor();
    processor->setDetector(nanoDetector);
    processor->moveToThread(new QThread(this));
    connect(processor->thread(),
                SIGNAL(started()),
                processor,
                SLOT(startVideo()));

        connect(processor->thread(),
                SIGNAL(finished()),
                processor,
                SLOT(deleteLater()));

        connect(processor,
                SIGNAL(inDisplay(QPixmap)),
                ui->video1,
                SLOT(setPixmap(QPixmap)));

        connect(processor,
                SIGNAL(outDisplay(QPixmap)),
                ui->video2,
                SLOT(setPixmap(QPixmap)));












}

static bool isPlaylist(const QUrl &url) // Check for ".m3u" playlists.
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}


void MainWindow::addToPlaylist(const QList<QUrl> &urls)
{
    for (auto &url: urls) {
        if (isPlaylist(url))
            m_playlist->load(url);
        else
            m_playlist->addMedia(url);
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    for (auto & ptr:videoClips) {
        ptr->thread()->quit();
        ptr->thread()->wait();
        delete ptr;
    }

    processor->stopVideo();
    processor->thread()->quit();
    processor->thread()->wait();

    delete humanDtr;
}

void MainWindow::open()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    QStringList supportedMimeTypes = m_player->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty()) {
        supportedMimeTypes.append("audio/x-m3u"); // MP3 playlists
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        addToPlaylist(fileDialog.selectedUrls());
}






void MainWindow::on_browseButton_pressed()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open Input Image",QDir::currentPath(),"Images (*.jpg *.png *.bmp)");
    if(QFile::exists(fileName))
    {
        ui->inputlineEdit->setText(fileName);
    }
}

void MainWindow::on_browseButton_2_pressed()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Select Input Image",QDir::currentPath(),"*.jpg;;*.png;;*.bmp");
    if(!fileName.isEmpty())
    {
        ui->outputlineEdit->setText(fileName);
        cv::imwrite(fileName.toStdString(),target);
    }


}

void MainWindow::showTarget()
{
    QImage image(target.data,target.cols,target.rows,target.step,QImage::Format_RGB888);
    ui->imageLabel->setPixmap(QPixmap::fromImage(image.rgbSwapped()));
}

void MainWindow::on_selectROIButton_pressed()
{
    cv::Mat inputImg;
    inputImg = cv::imread(ui->inputlineEdit->text().toStdString());
    cv::Rect2d roi = selectROI(inputImg);
    target = inputImg(roi);
    showTarget();

#ifdef DEBUG
    imshow("test roi",target);
    waitKey(0);
#endif

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int result = QMessageBox::warning(this,"Exit","你确定你想关掉此项目吗？",QMessageBox::Yes,QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        event->accept();
    }
    else
        event->ignore();
}


void MainWindow::getPluginsList()
{
    QDir filtersDir(qApp->applicationDirPath()+FILTERS_SUBFOLDER);
    QFileInfoList filters = filtersDir.entryInfoList(
                QDir::NoDotAndDotDot |
                QDir::Files, QDir::Name);

    foreach (QFileInfo filter, filters)
    {
        qDebug()<<filter.absoluteFilePath();
        if(QLibrary::isLibrary(filter.absoluteFilePath()))
        {
            QPluginLoader pluginLoader(filter.absoluteFilePath(),this);
            if(qobject_cast<CvPluginInterface *>(pluginLoader.instance()))
            {
                ui->pluginsList->addItem(filter.fileName());
                pluginLoader.unload();
            }
            else
            {
                QMessageBox::warning(this,tr("Warning"),
                                     QString(tr("Make sure %1 is a correct plugin for this application<br>").arg(filter.fileName())));
            }
        }
        else
        {
            QMessageBox::warning(this,tr("Warning"),
                                 QString(tr("Make sure plugins exist in plugins folder.<br> %1 is not a plugin.").arg(filter.fileName())));
        }
    }
    if(ui->pluginsList->count() <= 0)
    {
        QMessageBox::critical(this,tr("No Plugins"),
                              tr("This application cannot work without plugins!<br>"));
    }
}

void MainWindow::on_helpButton_pressed()
{
    if(ui->pluginsList->currentRow()>=0)
    {
        QPluginLoader pluginLoader(qApp->applicationDirPath()+FILTERS_SUBFOLDER+ui->pluginsList->currentItem()->text());
        CvPluginInterface *plugin = qobject_cast<CvPluginInterface *>(pluginLoader.instance());
        if(plugin)
        {
            QMessageBox::information(this,tr("Plugin Description"),
                                     plugin->description());
        }
        else
        {
            QMessageBox::warning(this,tr("Warning"),
                                 QString(tr("Make sure plugin %1 exists and is usable.")).arg(ui->pluginsList->currentItem()->text()));
        }

    }
    else
    {
         QMessageBox::warning(this,tr("Warning"),
                              QString(tr("First select a filter plugin from the list.")));

    }
}

void MainWindow::on_pluginsButton_pressed()
{
    if(ui->pluginsList->currentRow()>=0 && !target.empty())
    {
        QPluginLoader pluginLoader(qApp->applicationDirPath()+FILTERS_SUBFOLDER+ui->pluginsList->currentItem()->text());
        CvPluginInterface * plugin = qobject_cast<CvPluginInterface *>(pluginLoader.instance());
        if(plugin)
        {
            plugin->processImage(target, target);
#ifdef DEBUG
            imshow(tr("After Plugins").toStdString(),target);
#endif
        }
        else
        {
            QMessageBox::warning(this,tr("Warning"),
                                 QString(tr("Make sure plugins exists and is usable.")));

        }
    }
    else
    {
        QMessageBox::warning(this,tr("Warning"),
                             QString(tr("Select a plugin first.")));
    }
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    QStringList acceptFileTypes;
    acceptFileTypes.append("jpg");
    acceptFileTypes.append("png");
    acceptFileTypes.append("bmp");

    if(event->mimeData()->hasUrls()&&event->mimeData()->urls().count()==1)
    {
        QFileInfo file(event->mimeData()->urls().at(0).toLocalFile());
        if(acceptFileTypes.contains(file.suffix().toLower()))
        {
            event->acceptProposedAction();
        }
    }

}

void MainWindow::dropEvent(QDropEvent *event)
{
    QFileInfo file(event->mimeData()->urls().at(0).toLocalFile());
    if(pixmap.load(file.absoluteFilePath()))
    {
        ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),tr("The image file cannot be read!"));
    }
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if(!pixmap.isNull())
    {
        ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->width()-5,ui->imageLabel->height()-5,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    }
}

void MainWindow::on_videoInButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open Input Video",QDir::currentPath(),"Videos (*.mp4 *.avi)");
    pathsOfClips.push_back(fileName.toStdString());
}



void MainWindow::on_videoInitButton_clicked()
{
    if(!pathsOfClips.empty())
    {
        for (int i = 0;i<pathsOfClips.size();i++)
        {
            VideoClip * clip = new VideoClip();
            clip->moveToThread(new QThread(this));
            clip->thread()->start();
            clip->setVideoPath(pathsOfClips[i]);
            clip->setDetector(nanoDetector);
            clip->setVideo();
            videoClips.push_back(clip);
        }
    }

}

void MainWindow::on_videoProcessButton_clicked()
{
    static int index = 0;
    if(!videoClips.empty())
    {
        for (auto & pClip : videoClips)
        {
            Mat tmp;
            pClip->detectPerson();
            tmp = pClip->getMaxRect_demo(10);
            index = index % 4;
            if(index == 0)
            {
                QImage image(tmp.data,tmp.cols,tmp.rows,tmp.step,QImage::Format_RGB888);
                ui->searchLabel1->setPixmap(QPixmap::fromImage(image.rgbSwapped()));
            }
            if(index == 1)
            {
                QImage image(tmp.data,tmp.cols,tmp.rows,tmp.step,QImage::Format_RGB888);
                ui->searchLabel2->setPixmap(QPixmap::fromImage(image.rgbSwapped()));
            }
            if(index == 2)
            {
                QImage image(tmp.data,tmp.cols,tmp.rows,tmp.step,QImage::Format_RGB888);
                ui->searchLabel3->setPixmap(QPixmap::fromImage(image.rgbSwapped()));
            }
            if(index == 3)
            {
                QImage image(tmp.data,tmp.cols,tmp.rows,tmp.step,QImage::Format_RGB888);
                ui->searchLabel4->setPixmap(QPixmap::fromImage(image.rgbSwapped()));
            }

            index++;
        }


    }
}

void MainWindow::on_onButton_clicked()
{
    processor->goonVideo();
    processor->thread()->start();
}


void MainWindow::on_offButton_clicked()
{
    processor->stopVideo();
    processor->thread()->terminate();
}
