QT += core gui \
      network \
      xml \
      multimedia \
      multimediawidgets \
      widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/local/include \
               /usr/local/include/opencv \
               /usr/local/include/opencv4 \
               /home/lacorse/vulkansdk-linux-x86_64-1.2.154.0/1.2.154.0/x86_64/include \
               /home/lacorse/ncnn \
               /home/lacorse/ncnn/src \
               /home/lacorse/ncnn/build/install/include \
               /home/lacorse/ncnn/build/install/include/ncnn \
               /home/lacorse/ncnn/build/install/lib

QMAKE_CXXFLAGS += -fopenmp

LIBS += -fopenmp \
        /usr/local/lib/libopencv*.so
        #/home/lacorse/vulkansdk-linux-x86_64-1.2.154.0/1.2.154.0/x86_64/lib/libvulkan.so
        #/home/lacorse/ncnn/build/install/lib/libncnn.a


SOURCES += \
    humandetector.cpp \
    main.cpp \
    mainwindow.cpp \
    nanodet.cpp \
    videoclip.cpp \
    videoprocessor.cpp \
    videoprocessorthread.cpp

HEADERS += \
    cvplugininterface.h \
    humandetector.h \
    mainwindow.h \
    cvplugininterface.h \
    nanodet.h \
    utils.h \
    videoclip.h \
    videoprocessor.h \
    videoprocessorthread.h

FORMS += \
    mainwindow.ui




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    filter_plugins/libdenoise_median_plugin.so \
    filter_plugins/libdenoise_median_plugin.so.1 \
    filter_plugins/libdenoise_median_plugin.so.1.0 \
    filter_plugins/libdenoise_median_plugin.so.1.0.0

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ncnn/build/install/lib/release/ -lncnn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ncnn/build/install/lib/debug/ -lncnn
else:unix: LIBS += -L$$PWD/../../ncnn/build/install/lib/ -lncnn

INCLUDEPATH += $$PWD/../../ncnn/build/install/include
DEPENDPATH += $$PWD/../../ncnn/build/install/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../ncnn/build/install/lib/release/libncnn.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../ncnn/build/install/lib/debug/libncnn.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../ncnn/build/install/lib/release/ncnn.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../ncnn/build/install/lib/debug/ncnn.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../ncnn/build/install/lib/libncnn.a

unix:!macx: LIBS += -L$$PWD/../../vulkansdk-linux-x86_64-1.2.154.0/1.2.154.0/x86_64/lib/ -lvulkan

INCLUDEPATH += $$PWD/../../vulkansdk-linux-x86_64-1.2.154.0/1.2.154.0/x86_64/include
DEPENDPATH += $$PWD/../../vulkansdk-linux-x86_64-1.2.154.0/1.2.154.0/x86_64/include

unix:!macx: LIBS += -L$$PWD/../../ncnn/build/install/lib/ -lglslang

INCLUDEPATH += $$PWD/../../ncnn/build/install/include
DEPENDPATH += $$PWD/../../ncnn/build/install/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../ncnn/build/install/lib/libglslang.a

unix:!macx: LIBS += -L$$PWD/../../ncnn/build/install/lib/ -lOGLCompiler

INCLUDEPATH += $$PWD/../../ncnn/build/install/include
DEPENDPATH += $$PWD/../../ncnn/build/install/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../ncnn/build/install/lib/libOGLCompiler.a

unix:!macx: LIBS += -L$$PWD/../../ncnn/build/install/lib/ -lSPIRV

INCLUDEPATH += $$PWD/../../ncnn/build/install/include
DEPENDPATH += $$PWD/../../ncnn/build/install/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../ncnn/build/install/lib/libSPIRV.a

unix:!macx: LIBS += -L$$PWD/../../ncnn/build/install/lib/ -lOSDependent

INCLUDEPATH += $$PWD/../../ncnn/build/install/include
DEPENDPATH += $$PWD/../../ncnn/build/install/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../ncnn/build/install/lib/libOSDependent.a
