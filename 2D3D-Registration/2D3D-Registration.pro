#-------------------------------------------------
#
# Project created by QtCreator 2019-11-21T10:57:19
#
#-------------------------------------------------

QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
TARGET = 2D3D-Registration
TEMPLATE = app

win32 {
    CONFIG += console
    QMAKE_LFLAGS += /STACK:48000000
}

SOURCES += main.cpp\
        mainwindow.cpp \
    convolution.cpp \
    chartdialog.cpp \
    loadingthread.cpp \
    plottingthread.cpp \
    qcustomplot.cpp \
    surfacedata.cpp

HEADERS  += mainwindow.h \
    convolution.h \
    chartdialog.h \
    loadingthread.h \
    qcustomplot.h \
    ui_chartdialog.h \
    plottingthread.h \
    surfacedata.h

win32 {
    HEADERS += unistd_.h
}


FORMS    += mainwindow.ui \
    chartdialog.ui

INCLUDEPATH += ../3rdParty/newuoa



win32 {

win32: LIBS += -L$$PWD/../3rdParty/clapack-3.2.1/Windows10/F2CLIBS/libf2c/Debug/ -llibf2cd
INCLUDEPATH += $$PWD/../3rdParty/clapack-3.2.1/INCLUDE
DEPENDPATH += $$PWD/../3rdParty/clapack-3.2.1/INCLUDE

win32: LIBS += -L$$PWD/../3rdParty/clapack-3.2.1/Windows10/BLAS/SRC/Debug/ -lblasd
INCLUDEPATH += $$PWD/../3rdParty/clapack-3.2.1/Windows10/INCLUDE
DEPENDPATH += $$PWD/../3rdParty/clapack-3.2.1/Windows10/INCLUDE

win32: LIBS += -L$$PWD/../3rdParty/clapack-3.2.1/Windows10/SRC/Debug/ -llapackd
INCLUDEPATH += $$PWD/../3rdParty/clapack-3.2.1/INCLUDE
DEPENDPATH += $$PWD/../3rdParty/clapack-3.2.1/INCLUDE


win32: LIBS += -L$$PWD/../3rdParty/armadillo-10.2.0/Windows10/Debug/ -larmadillo
INCLUDEPATH += $$PWD/../3rdParty/armadillo-10.2.0/include
DEPENDPATH += $$PWD/../3rdParty/armadillo-10.2.0/include




    CONFIG(debug, debug|release): LIBS += -L$$PWD/../../FIRE2-Build/build-CudaDrrRenderer-Desktop_Qt_5_8_0_MSVC2015_64bit/debug/ -lCudaDrrRenderer
    CONFIG(release, debug|release): LIBS += -L$$PWD/../../FIRE2-Build/build-CudaDrrRenderer-Desktop_Qt_5_8_0_MSVC2015_64bit/release/ -lCudaDrrRenderer
    INCLUDEPATH += $$PWD/../CudaDrrRenderer
    DEPENDPATH += $$PWD/../CudaDrrRenderer



    CONFIG(debug, debug|release): LIBS += -L$$PWD/../../FIRE2/3rdParty/nifticlib-2.0.0/Windows10/niftilib/Debug/ -lniftiio
    CONFIG(release, debug|release): LIBS += -L$$PWD/../../FIRE2/3rdParty/nifticlib-2.0.0/Windows10/niftilib/Release/ -lniftiio
    INCLUDEPATH += $$PWD/../3rdParty/nifticlib-2.0.0/include
    DEPENDPATH += $$PWD/../3rdParty/nifticlib-2.0.0/include

    CONFIG(release, debug|release): LIBS += -L$$PWD/../../FIRE2-Build/build-LibFire-Desktop_Qt_5_8_0_MSVC2015_64bit/release/ -lLibFire
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../../FIRE2-Build/build-LibFire-Desktop_Qt_5_8_0_MSVC2015_64bit/debug/ -lLibFire
    INCLUDEPATH += $$PWD/../LibFire
    DEPENDPATH += $$PWD/../LibFire



    CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdParty/nifticlib-2.0.0/Windows10/znzlib/Release/ -lznz
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdParty/nifticlib-2.0.0/Windows10/znzlib/Debug/ -lznz
    INCLUDEPATH += $$PWD/../3rdParty/nifticlib-2.0.0/znzlib
    DEPENDPATH += $$PWD/../3rdParty/nifticlib-2.0.0/znzlib

    CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdParty/zlib-1.2.11/Windows10/build/Release/ -lzlibstatic
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdParty/zlib-1.2.11/Windows10/build/Debug/ -lzlibstaticd
    INCLUDEPATH += $$PWD/../3rdParty/zlib-1.2.11
    DEPENDPATH += $$PWD/../3rdParty/zlib-1.2.11
}


unix {

    LIBS += -L$$PWD/../../FIRE2-Build/build-LibFire-Desktop_Qt_5_8_0_GCC_64bit/debug/ -lLibFire
    INCLUDEPATH += $$PWD/../LibFire
    DEPENDPATH += $$PWD/../LibFire
    PRE_TARGETDEPS += $$PWD/../../FIRE2-Build/build-LibFire-Desktop_Qt_5_8_0_GCC_64bit/debug/libLibFire.a

    LIBS += -L$$PWD/../3rdParty/nifticlib-2.0.0/niftilib/ -lniftiio
    INCLUDEPATH += $$PWD/../3rdParty/nifticlib-2.0.0/include
    DEPENDPATH += $$PWD/../3rdParty/nifticlib-2.0.0/include
    PRE_TARGETDEPS += $$PWD/../3rdParty/nifticlib-2.0.0/niftilib/libniftiio.a

    LIBS += -L$$PWD/../../../../../usr/local/lib/ -lznz
    INCLUDEPATH += $$PWD/../../../../../usr/local/include
    DEPENDPATH += $$PWD/../../../../../usr/local/include

    LIBS += -L$$PWD/../../FIRE2-Build/build-CudaDrrRenderer-Desktop/Debug/ -lCudaDrrRenderer
    INCLUDEPATH += $$PWD/../CudaDrrRenderer
    DEPENDPATH += $$PWD/../CudaDrrRenderer

    LIBS += -L/usr/local/cuda-9.2/lib64/ -lcudart
    INCLUDEPATH += /usr/local/cuda-9.2/include
    DEPENDPATH += /usr/local/cuda-9.2/include
}

