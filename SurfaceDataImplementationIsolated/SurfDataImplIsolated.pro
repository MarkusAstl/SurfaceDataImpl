QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chartdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    surfacedata.cpp \
    loadingthread.cpp \
    debugthread.cpp

HEADERS += \
    chartdialog.h \
    mainwindow.h \
    qcustomplot.h \
    surfacedata.h \
    loadingthread.h \
    debugthread.h

FORMS += \
    chartdialog.ui \
    mainwindow.ui

win32 {

win32: LIBS += -L$$PWD/../PhaseRecognition_Masterarbeit/armadillo-10.2.0/Windows10/Debug/ -larmadillo
INCLUDEPATH += $$PWD/../PhaseRecognition_Masterarbeit/armadillo-10.2.0/include
DEPENDPATH += $$PWD/../PhaseRecognition_Masterarbeit/armadillo-10.2.0/include

}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
