#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <readingthread.h>

#include <QFile>
#include <Qstring>
#include <QStringList>
#include <QDebug>
#include <QDir>

#include <iostream>
#include <new>

#include <chrono>
#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Reading surface data during treatment
    rThread = new ReadingThread(this);
    connect(rThread,SIGNAL(LnReadingFinished(QStringList, int)), this, SLOT(onLnReadingFinished(QStringList, int)));

    // Loading surface data from planning CT
    lThread = new ReadingThread(this);
    connect(lThread,SIGNAL(LnReadingFinished(QStringList, int)), this, SLOT(onLoadingFinished(QStringList, int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLnReadingFinished(QStringList SplitLn, int colNum)
{
    QString timeVal = SplitLn[0];
    ui->timeLabel->setText(timeVal);

    QString amp1Val = SplitLn[1];

    if(colNum == 2){
        amp1Val.remove(amp1Val.size()-4, amp1Val.size()-1);
    }

    ui->amp1Label->setText(amp1Val);

    if(colNum == 3){
        QString amp2Val = SplitLn[2];
        ui->amp2Label->setText(amp2Val);
    }
}

void MainWindow::onLoadingFinished(QStringList SplitLn, int colNum)
{
    QString timeValAvg = SplitLn[0];
    ui->timeLabelAvg->setText(timeValAvg);

    QString amp1ValAvg = SplitLn[1];

    if(colNum == 2){
        amp1ValAvg.remove(amp1ValAvg.size()-4, amp1ValAvg.size()-1);
    }

    ui->amp1LabelAvg->setText(amp1ValAvg);

    if(colNum == 3){
        QString amp2ValAvg = SplitLn[2];
        ui->amp2LabelAvg->setText(amp2ValAvg);
    }
}

void MainWindow::on_StartReadingButton_clicked()
{
    // Started
    rThread->path = MainWindow::path2;
    rThread->readDynBool = true;
    rThread->sleepingTime = 500;
    rThread->start();
}

void MainWindow::on_StopReadingButton_clicked()
{
    // Stopped
    rThread->Stop = true;
}

void MainWindow::on_LoadingButton_clicked()
{
    // Started
    lThread->path = MainWindow::path1;
    rThread->readDynBool = false;
    lThread->sleepingTime = 0;
    lThread->start();
}

void MainWindow::on_StopLoadingButton_clicked()
{
    // Stopped
    lThread->Stop = true;
}
