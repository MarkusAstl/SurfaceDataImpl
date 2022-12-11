#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>

#include <QtMath>

#include <QFile>
#include <Qstring>
#include <QStringList>
#include <QDebug>
#include <QDir>

#include <iostream>
#include <new>

#include <chrono>
#include <thread>

#include <armadillo>
#include <vector>

#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList readOneLn(QFile &f){

    // Seperate Qstring of one line into QStringList of its values
    QString separator(";");
    QString ln = f.readLine();
    QStringList SplitLn = ln.split(separator);

    return SplitLn;
}

bool saveValsToLists(QList<double> &TimeList, QList<double> &Amp1List, QList<double> &Amp2List, QStringList &SplitLn, int &colNum){

    // Save values of one line into corresponding lists
    bool ok;

    for (int colInd = 0; colInd < colNum; ++colInd) {
        double doubleVal = SplitLn[colInd].toDouble(&ok);

         if (ok == false) {
            return false;
            }

        switch (colInd) {
        case 0:
            TimeList.append(doubleVal/1000);
            break;
        case 1:
            Amp1List.append(doubleVal);
            break;
        case 2:
            Amp2List.append(doubleVal);
            break;
        }
    }
    return true;
}


void meanfilter(const QList<double>* signal, QList<double>* result, int N, int smthMeanWindow)
{
   // Define signal data
   QList<double> signalList = *signal;

   // Make sure smth window is an even number
   QErrorMessage errorMessage;
   errorMessage.showMessage("Error in function meanfilter: int sthmMeanWindow must be uneven.");
   if (smthMeanWindow % 2 != 1){errorMessage.exec();}

   // Apply moving average filter
   int halfWindow = smthMeanWindow / 2;
   for (int i = halfWindow; i < N - halfWindow; ++i){
       double smthSum = 0.0;
       for(int k = i-halfWindow; k<i+halfWindow+1; ++k){
           smthSum += signalList[k];
       }

       double smthMean = smthSum/smthMeanWindow;
       result->append(smthMean);
   }
}

void extremum_search(arma::vec* data, std::vector<int>* max_ind_list, std::vector<int>* min_ind_list,
                     int start_ind, bool search_max, int max_num_of_extr, bool* no_extr_left){

    bool is_max;
    bool is_min;
    int search_range = 20;
    int q = start_ind + search_range;
    int q_max;
    int q_min;
    double a1_diff;
    double a1_min_diff = 0.6;
    bool skip;
    int num_of_extr = 0;

    while(q <= data->size()-1){
        if(search_max){
            skip = false;
            is_max = false;
            q_max = q - search_range;

            // inspect if q_max is a local max
            if(data->at(q_max) > data->at(q)){
                for(int dq = 1; dq < search_range; ++dq){

                    if(data->at(q_max) > data->at(q-dq)){

                        a1_diff = abs(data->at(q_max) - data->at(q-dq));
                        if(a1_diff >= a1_min_diff){
                            is_max = true;
                        }
                    }
                    else{
                        is_max = false;
                        ++q;
                        skip = true;
                        break;
                    }
                }

                while(!skip && !is_max && q < data->size()-1){
                    ++q;

                    if(data->at(q_max) > data->at(q)){
                        a1_diff = abs(data->at(q_max) - data->at(q));

                        if(a1_diff >= a1_min_diff){
                            is_max = true;
                        }
                    }
                    else {
                        q = q + search_range;
                        break;
                    }
                }

                if(is_max){
                    max_ind_list->push_back(q_max);
                    ++ num_of_extr;

                    if(num_of_extr == max_num_of_extr){
                        break;
                    }

                    is_max = false;
                    search_max = false;
                    ++q;
                }
            }

            else{
                ++q;
                //continue;
            }
        }

        if(!search_max){
            skip = false;
            is_min = false;
            q_min = q - search_range;

            if(data->at(q_min) < data->at(q)){

                for(int dq = 1; dq < search_range; ++dq){

                    if(data->at(q_min) < data->at(q-dq)){

                        a1_diff = abs(data->at(q_min) - data->at(q-dq));
                        if(a1_diff >= a1_min_diff){
                            is_min = true;
                        }
                    }
                    else{
                        is_min = false;
                        ++q;
                        skip = true;
                        break;
                    }
                }

                while(!skip && !is_min && q < data->size()-1){
                    ++q;

                    if(data->at(q_min) < data->at(q)){
                        a1_diff = abs(data->at(q_min) - data->at(q));

                        if(a1_diff >= a1_min_diff){
                            is_min = true;
                        }
                    }
                    else {
                        q = q + search_range;
                        break;
                    }
                }

                if(is_min){
                    min_ind_list->push_back(q_min);
                    ++ num_of_extr;

                    if(num_of_extr == max_num_of_extr){
                        break;
                    }

                    is_min = false;
                    search_max = true;
                    ++q;
                }
            }

            else{
                ++q;
                //continue;
            }
        }
    }

    if(num_of_extr < max_num_of_extr){
        *no_extr_left = true;
    }
}


void find_extrema(arma::vec data_list, arma::vec time_list,
                  std::vector<int>* max_ind_list, std::vector<int>* min_ind_list, int start_ind){
    // finds all extrema in data_list

    bool no_extr_left = false;

    while (!no_extr_left){

        // Find maximum
        extremum_search(&data_list, max_ind_list, min_ind_list, start_ind, true, 1, &no_extr_left);
        start_ind = max_ind_list->back();

        // Break if there are no extrema left in the data
        if(no_extr_left){
            break;
        }

        // Find minimum
        extremum_search(&data_list, max_ind_list, min_ind_list, start_ind, false, 1, &no_extr_left);
        start_ind = min_ind_list->back();

        // First detected extremum must be a maximum but it must not be right at beginning of data
        if(max_ind_list->front() < 10){
            max_ind_list->pop_back();
            min_ind_list->pop_back();
        }
    }
}


void movingAvgFilterExp(QList<double>* input_data, arma::vec* filtered_data, double* f_s, double* f_c, int* N){
    // Low pass filter construncted with convolution using a exponential moving avergage filter

    // Convert to arma::vec to use armadillo functions
    arma::vec input_vec(input_data->size());
    for(int z = 0; z < input_data->size(); ++z){
        input_vec[z] = input_data->at(z);
    }

    // Calculate parameter for given sampling frequency and cutoff frequency
    double a = qSqrt( qPow( qCos(2*M_PI* *f_c/ *f_s), 2) - 4*qCos(2*M_PI* *f_c/ *f_s) + 3) + qCos(2*M_PI* *f_c/ *f_s) - 1;

    // Display all parameters
    qDebug() << "Exponential moving avg filter: alpha: " << a << ",  f_s = " << *f_c << ",  f_s = " << *f_s;

    // Create convolution function h
    arma::vec h(*N);

    double f = 1-a;
    for (int k = 0; k <= *N; k++){
        h[k] = qPow(f, k);
    }

    //Filter data
    *filtered_data = a * arma::conv(input_vec, h);
}



void MainWindow::on_Button_clicked()
{
    //------------------------------------------------------------------------------------------------------------------
    //------- Define working parameters

    double start_factor = 0.01;          // Cut dataset to start at this percentage
    double end_factor = 0.3;           // Cut dataset to end at this percentage
    int smthMeanWindow = 9;             // Window size for smoothing avergage filter
    int cycles_for_avg = 1;             // Number of last breath cycles considered for calculation of average breath amplitude range
    int intp_factor = 2;                // New amount of datapoints = old amount of datapoints * intp_factor



    //------------------------------------------------------------------------------------------------------------------
    //------- Read data ------------------------------------------------------------------------------------------------

//    // Check files and directories in current folder
//    QDir dir;
//    foreach(QFileInfo item, dir.entryInfoList() )
//        {
//            if(item.isDir())
//                qDebug() << "Dir: " << item.absoluteFilePath();
//            if(item.isFile())
//                qDebug() << "File: " << item.absoluteFilePath();
//        }

    // Define path of data file
    QString path1 = "SurfaceData/BruMi-20210615-081652_clean.csv";
    QFile f(path1);

    // Open file
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << f.errorString();
    }


    // Locate row of headers and determine number of columns
    int colNum = 0;

    QString twoHeaders = { "Time (ms); Pri Amplitude (mm)\r\n" };
    QString threeHeaders = { "Time (ms); Pri Amplitude (mm); Sec Amplitude (mm);\r\n" };

    for (int lnInd = 0; lnInd <= 5; ++lnInd) {
        QString ln1 = f.readLine(0);

        if (lnInd == 5) {
            qDebug() << f.errorString();
        }
        else if (ln1 == twoHeaders) {
            colNum = 2;
            break;
        }
        else if (ln1 == threeHeaders) {
            colNum = 3;
            break;
        }
    }


    // Check if number of columns is permissible
    if (colNum == 0) {
        qDebug() << f.errorString();
    }

    // Define lists to store data
    QList<double> TimeList;
    QList<double> Amp1List;
    QList<double> Amp2List;

    // Read data line by line
    while (!f.atEnd()){

        // Read one line
        QStringList SplitLn = readOneLn(f);

        // Save line in lists
        if (!saveValsToLists(TimeList, Amp1List, Amp2List, SplitLn, colNum)) {
            qDebug() << f.errorString();
        }
    }

    // Cut data to interval specified with working parameters
    int new_start = round(TimeList.size() * start_factor);
    int new_end = round(TimeList.size() * end_factor);
    int new_size = new_end - new_start;
    QList<double> t = TimeList.mid(new_start, new_size);
    QList<double> a1 = Amp1List.mid(new_start, new_size);



    //------------------------------------------------------------------------------------------------------------------
    //------- Process data ------------------------------------------------------------------------------------------------

    // Smooth list by using moving average filter
    QList<double> a1_smth;
    QList<double> t_smth = t.mid(smthMeanWindow/2, t.size()-smthMeanWindow+1);
    meanfilter(&a1, &a1_smth, a1.size(), smthMeanWindow);                       // window size specified in working parameters

    double fs = 1/(t[1]-t[0]);
    double fc = 0.333;
    int window_size = 9;
    arma::vec filtered_data(a1.size()-window_size);
    movingAvgFilterExp(&a1, &filtered_data, &fs, &fc, &window_size);        // Decrease window_size to make code faster

    // Convert from QList to arma::vec to use Armadillo functions
    arma::vec a1_vec(a1_smth.size());
    arma::vec t_vec(t_smth.size());
    for(int h = 0; h < a1_smth.size(); ++h){
        a1_vec[h] = a1_smth[h];
        t_vec[h] = t_smth[h];
    }

    // Interpolation (time increments are not equally sized)
    arma::vec t_intp = arma::linspace(t_vec[0], t_vec[t_vec.size()-1], t_vec.size()*intp_factor);
    arma::vec a1_intp;
    arma::interp1(t_vec, a1_vec, t_intp, a1_intp);



    //------------------------------------------------------------------------------------------------------------------
    //------- Analyse data ------------------------------------------------------------------------------------------------

    // Find all the extrema
    std::vector<int> max_ind_list;
    std::vector<int> min_ind_list;
    find_extrema(a1_intp, t_intp, &max_ind_list, &min_ind_list, 0);
    qDebug() << "";
    qDebug() << "-----find_extrema function:";
    qDebug() << "Number of maxima: " << max_ind_list.size();
    qDebug() << "Number of minima: " << min_ind_list.size();

    // Find last extremum
    int last_max_ind = max_ind_list.back();
    int last_min_ind = min_ind_list.back();
    qDebug() << "Index of last max: " << last_max_ind << "  Time of last max: " << t_intp[last_max_ind];
    qDebug() << "Index of last min: " << last_min_ind << "  Time of last min: " << t_intp[last_min_ind];

    // Check if last extremum is a minimum
    bool ends_with_min = last_max_ind < last_min_ind;

    arma::vec half_cycle_data;
    int half_cycle_start_ind;

    // Define range of last half breathing cycle
    if (ends_with_min == true){
        half_cycle_data = a1_intp.subvec(min_ind_list.back(), a1_intp.size()-1);
        half_cycle_start_ind = min_ind_list.back();
    }
    else{
        half_cycle_data = a1_intp.subvec(max_ind_list.back(), a1_intp.size()-1);
        half_cycle_start_ind = max_ind_list.back();
    }    

    // Save amplitude size of every cycle into ampl_list
    int num_of_cycles = min_ind_list.size()-1;
    arma::vec ampl_list(num_of_cycles);
    for (int cycle_ind = 0; cycle_ind < num_of_cycles; ++cycle_ind){
        double ampl_size = abs(a1_intp[max_ind_list[cycle_ind]] - a1_intp[min_ind_list[cycle_ind]]);
        ampl_list[cycle_ind] = ampl_size;
    }

    // Calculate mean amplitude size for last n cycles. n = 'cycles_for_avg' in working parameters
    double avg_ampl = mean(ampl_list.subvec(ampl_list.size()-cycles_for_avg-1, ampl_list.size()-1));
    double phase_ampl_size = avg_ampl/5;
    qDebug() << "phase amplitude range: " << phase_ampl_size;

    // Determine last phase number
    double last_ampl_size = abs(half_cycle_data.back() - half_cycle_data[0]);
    int last_phase = 0;

    if (last_ampl_size <= phase_ampl_size){
        last_phase = 1;
    }
    else if (last_ampl_size <= phase_ampl_size * 2){
        last_phase = 2;
    }
    else if (last_ampl_size <= phase_ampl_size * 3){
        last_phase = 3;
    }
    else if (last_ampl_size <= phase_ampl_size * 4){
        last_phase = 4;
    }
    else if (last_ampl_size > phase_ampl_size * 4){
        last_phase = 5;
    }

    int incr_sign;
    int border_num;

    if (ends_with_min == false){
        last_phase = last_phase + 5;
        incr_sign = -1;
        border_num = last_phase - 6;
    }
    else{
        incr_sign = 1;
        border_num = last_phase - 1;
    }

    // Find indices of phase borders of last cycle
    arma::vec phase_border_ind_list(border_num+1);
    arma::vec diff_list(half_cycle_data.size());

    for (int phase_num = 0; phase_num <= border_num; ++phase_num){
        double calc_border_val = incr_sign * phase_ampl_size * phase_num + a1_intp[half_cycle_start_ind];
        for (int hcd_ind = 0; hcd_ind < half_cycle_data.size(); ++hcd_ind){
            float hcd_abs = abs(half_cycle_data[hcd_ind]-calc_border_val);
            diff_list[hcd_ind] = hcd_abs;
        }
        int phase_border_ind = diff_list.index_min();
        phase_border_ind_list(phase_num) = half_cycle_start_ind + phase_border_ind+1;
    }


    //------------------------------------------------------------------------------------------------------------------
    //------- Plot results ---------------------------------------------------------------------------------------------


    // Create dialog window
    chartDialog = new ChartDialog(this);        // "this" points at MainWindow as a parent
    chartDialog->setDisabled(false);            // enables input events

    // Set Pen
    QPen PRPen;
    PRPen.setWidth(1);
    PRPen.setColor(Qt::blue);

    // Create QCustomPlot for raw data
    RawDataChart = new QCustomPlot();
    RawDataChart->setFixedSize(700,210);
    QCPTextElement *RawDataTitle = new QCPTextElement(RawDataChart);
    RawDataTitle->setText("Raw Data");
    RawDataTitle->setFont(QFont("sans", 10, QFont::Bold));
    RawDataChart->plotLayout()->insertRow(0); // insert an empty row above the axis rect
    RawDataChart->plotLayout()->addElement(0, 0, RawDataTitle); // place the title in the empty cell we've just created
    RawDataChart->addGraph();
    RawDataChart->graph(0)->setPen(PRPen);

    // Create QCustomPlot for smoothed data
    SmthDataChart = new QCustomPlot();
    SmthDataChart->setFixedSize(700,210);
    QCPTextElement *SmthDataTitle = new QCPTextElement(SmthDataChart);
    SmthDataTitle->setText("Smoothed Data");
    SmthDataTitle->setFont(QFont("sans", 10, QFont::Bold));
    SmthDataChart->plotLayout()->insertRow(0); // insert an empty row above the axis rect
    SmthDataChart->plotLayout()->addElement(0, 0, SmthDataTitle); // place the title in the empty cell we've just created
    SmthDataChart->addGraph();
    SmthDataChart->graph(0)->setPen(PRPen);

    // Create QCustomPlot for interpolated data
    IntpDataChart = new QCustomPlot();
    IntpDataChart->setFixedSize(700,210);
    QCPTextElement *IntpDataTitle = new QCPTextElement(IntpDataChart);
    IntpDataTitle->setText("Interpolated and Smoothed Data");
    IntpDataTitle->setFont(QFont("sans", 10, QFont::Bold));
    IntpDataChart->plotLayout()->insertRow(0); // insert an empty row above the axis rect
    IntpDataChart->plotLayout()->addElement(0, 0, IntpDataTitle); // place the title in the empty cell
    IntpDataChart->addGraph();
    IntpDataChart->graph(0)->setPen(PRPen);

    // Create QCustomPlot for phase recognition
    PhaseRecogChart = new QCustomPlot();
    PhaseRecogChart->setFixedSize(700,210);
    QCPTextElement *PhaseRecogTitle = new QCPTextElement(PhaseRecogChart);
    PhaseRecogTitle->setText("Phase Recognition");
    PhaseRecogTitle->setFont(QFont("sans", 10, QFont::Bold));
    PhaseRecogChart->plotLayout()->insertRow(0); // insert an empty row above the axis rect
    PhaseRecogChart->plotLayout()->addElement(0, 0, PhaseRecogTitle); // place the title in the empty cell
    PhaseRecogChart->addGraph();
    PhaseRecogChart->graph(0)->setPen(PRPen);

    // Add data
    QVector<double> tVecRaw = t.toVector();
    QVector<double> a1VecRaw = a1.toVector();
    RawDataChart->graph(0)->setData(tVecRaw, a1VecRaw);

    QVector<double> tSmthVec = t_smth.toVector();
    QVector<double> a1SmthVec = a1_smth.toVector();
    SmthDataChart->graph(0)->setData(tSmthVec, a1SmthVec);

    QVector<double> tVec(t_intp.size());
    QVector<double> a1Vec(a1_intp.size());
    for (int d = 0; d<t_intp.size(); ++d){
        tVec[d] = t_intp[d];
        a1Vec[d] = a1_intp[d];
    }
    IntpDataChart->graph(0)->setData(tVec, a1Vec);


    QVector<double> tVecPR = tVec.mid(min_ind_list[min_ind_list.size()-2], -1);
    QVector<double> a1VecPR = a1Vec.mid(min_ind_list[min_ind_list.size()-2], -1);
    PhaseRecogChart->graph(0)->setData(tVecPR, a1VecPR);

    // add extrema
    IntpDataChart->addGraph();
    IntpDataChart->graph(1)->setPen(QPen(Qt::red));
    IntpDataChart->graph(1)->setLineStyle(QCPGraph::lsNone);
    IntpDataChart->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 4));
    QVector<double> tMinVec(int(min_ind_list.size()));
    QVector<double> a1MinVec(int(min_ind_list.size()));
    for(int y = 0; y<min_ind_list.size(); ++y){
        tMinVec[y] = t_intp[min_ind_list[y]];
        a1MinVec[y] = a1_intp[min_ind_list[y]];
    }
    IntpDataChart->graph(1)->setData(tMinVec, a1MinVec);

    IntpDataChart->addGraph();
    IntpDataChart->graph(2)->setPen(QPen(Qt::red));
    IntpDataChart->graph(2)->setLineStyle(QCPGraph::lsNone);
    IntpDataChart->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 4));
    QVector<double> tMaxVec(int(max_ind_list.size()));
    QVector<double> a1MaxVec(int(max_ind_list.size()));
    for(int z = 0; z<max_ind_list.size(); ++z){
        tMaxVec[z] = t_intp[max_ind_list[z]];
        a1MaxVec[z] = a1_intp[max_ind_list[z]];
    }
    IntpDataChart->graph(2)->setData(tMaxVec, a1MaxVec);

    // Define position parameters for layout
    double a1_Min = *std::min_element(a1VecRaw.begin(), a1VecRaw.end());
    double a1_Max = *std::max_element(a1VecRaw.begin(), a1VecRaw.end());
    qreal xPRAxisRange = tVecRaw.back() - tVecRaw[0];

    double a1PR_Min = *std::min_element(a1VecPR.begin(), a1VecPR.end());
    double a1PR_Max = *std::max_element(a1VecPR.begin(), a1VecPR.end());

    // Add vertical line and phase number
    QPen borderPen(Qt::black, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    borderPen.setWidth(1);
    for(int veLnInd = 0; veLnInd < phase_border_ind_list.size(); ++veLnInd){
        QCPItemStraightLine *statBorderLine = new QCPItemStraightLine(PhaseRecogChart);
        statBorderLine->point1->setCoords(t_intp[phase_border_ind_list[veLnInd]], a1PR_Min);  // location (x,y)
        statBorderLine->point2->setCoords(t_intp[phase_border_ind_list[veLnInd]], a1PR_Max);  // location (x,y)
        statBorderLine->setPen(borderPen);
    }

    // Add text
    QCPItemText *textLabel = new QCPItemText(PhaseRecogChart);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    textLabel->position->setCoords(tVecPR[0]+ (tVecPR.back()-tVecPR[0])*0.1, a1PR_Max+(a1PR_Max-a1PR_Min)*0.2); // place position at center/top of axis rect
    textLabel->setText("Current phase: " + QString::number(last_phase, 10));
    textLabel->setFont(QFont(font().family(), 8));

    // Adjust Axes
    RawDataChart->xAxis->setLabel("t [s]");
    RawDataChart->yAxis->setLabel("Amp. [mm]");
    RawDataChart->xAxis->setRange(tVecRaw[0], tVecRaw[0]+xPRAxisRange);
    RawDataChart->xAxis->setLabelFont(QFont("sans",8, QFont::Bold));
    RawDataChart->yAxis->setRange(a1_Min-(a1_Max-a1_Min)*0.15, a1_Max+(a1_Max-a1_Min)*0.3);
    RawDataChart->yAxis->setLabelFont(QFont("sans",8, QFont::Bold));
    RawDataChart->replot();

    SmthDataChart->xAxis->setLabel("t [s]");
    SmthDataChart->yAxis->setLabel("Amp. [mm]");
    SmthDataChart->xAxis->setRange(tVecRaw[0], tVecRaw[0]+xPRAxisRange);
    SmthDataChart->xAxis->setLabelFont(QFont("sans",8, QFont::Bold));
    SmthDataChart->yAxis->setRange(a1_Min-(a1_Max-a1_Min)*0.15, a1_Max+(a1_Max-a1_Min)*0.3);
    SmthDataChart->yAxis->setLabelFont(QFont("sans",8, QFont::Bold));
    SmthDataChart->replot();

    IntpDataChart->xAxis->setLabel("t [s]");
    IntpDataChart->yAxis->setLabel("Amp. [mm]");
    IntpDataChart->xAxis->setRange(tVecRaw[0], tVecRaw[0]+xPRAxisRange);
    IntpDataChart->xAxis->setLabelFont(QFont("sans",8, QFont::Bold));
    IntpDataChart->yAxis->setRange(a1_Min-(a1_Max-a1_Min)*0.15, a1_Max+(a1_Max-a1_Min)*0.3);
    IntpDataChart->yAxis->setLabelFont(QFont("sans",8, QFont::Bold));
    IntpDataChart->replot();

//    FFTChart->xAxis->setLabel("f [Hz]");
//    FFTChart->yAxis->setLabel("Magn. [1]");
//    FFTChart->xAxis->setRange(f_MinRange, f_MaxRange);
//    FFTChart->xAxis->setLabelFont(QFont("sans",8, QFont::Bold));
//    FFTChart->yAxis->setRange(0, mMax_FFT*1.1);
//    FFTChart->yAxis->setLabelFont(QFont("sans",8, QFont::Bold));
//    FFTChart->replot();

    PhaseRecogChart->xAxis->setLabel("t [s]");
    PhaseRecogChart->yAxis->setLabel("Amp. [mm]");
    PhaseRecogChart->xAxis->setRange(tVecPR[0], tVecPR.back()+(tVecPR.back()-tVecPR[0])*0.05);
    PhaseRecogChart->xAxis->setLabelFont(QFont("sans",8, QFont::Bold));
    PhaseRecogChart->yAxis->setRange(a1PR_Min-(a1PR_Max-a1PR_Min)*0.15, a1PR_Max+(a1PR_Max-a1PR_Min)*0.3);
    PhaseRecogChart->yAxis->setLabelFont(QFont("sans",8, QFont::Bold));
    PhaseRecogChart->replot();


    // Set layout
    chartDialog->setLayout(new QVBoxLayout());
    chartDialog->layout()->addWidget(RawDataChart);
    //chartDialog->layout()->addWidget(SmthDataChart);
    chartDialog->layout()->addWidget(IntpDataChart);
    //chartDialog->layout()->addWidget(FFTChart);
    chartDialog->layout()->addWidget(PhaseRecogChart);
    chartDialog->resize(700, 900);
    chartDialog->show();
}


// Useful code:
// qDebug() << typeid(variable).name();
