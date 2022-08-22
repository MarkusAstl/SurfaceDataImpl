#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>

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
    //qDebug() << SplitLn;

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
   QList<double> signalList = * signal;

   QErrorMessage errorMessage;
   errorMessage.showMessage("Error in function meanfilter: int sthmMeanWindow must be uneven.");
   if (smthMeanWindow % 2 != 1){errorMessage.exec();}

   int halfWindow = smthMeanWindow / 2;

   // Move window through all elements of the signal
   for (int i = halfWindow; i < N - halfWindow; ++i)
   {
       double smthMean = 0.0;
       for(int k = i-halfWindow; k<i+halfWindow+1; ++k){
           smthMean += signalList[k];
       }
       smthMean = smthMean/smthMeanWindow;
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

            q_max = q - search_range;
            skip = false;
            is_max = false;
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

            q_min = q - search_range;
            skip = false;
            is_min = false;
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


void monotonocity_extr_search(arma::vec* data, std::vector<int>* max_ind_list, std::vector<int>* min_ind_list,
                              int start_ind, bool search_max, int max_num_of_extr, bool* no_extr_left){

    int num_of_extr = 0;
    int mono_range = 20;    // What if dataset size is < mono_range?
    bool is_min;
    bool is_max;

    for (int q = start_ind + mono_range; q < data->size() ; ++q){
        if(search_max){
            is_max = true;
            for (int dq = 0; dq < mono_range; ++dq){
                if (data->at(q-mono_range) <= data->at(q-dq)){
                        is_max = false;
                        break;
                }
            }
            if(is_max){
                max_ind_list->push_back(q-mono_range);
                ++num_of_extr;
                if (num_of_extr == max_num_of_extr){
                    break;
                }
                search_max = false;
            }
        }

        else {
            is_min = true;
            for (int dq = 0; dq < mono_range; ++dq){
                if (data->at(q-mono_range) >= data->at(q-dq)){
                        is_min = false;
                        break;
                }
            }
            if(is_min){
                min_ind_list->push_back(q-mono_range);
                ++num_of_extr;
                if (num_of_extr == max_num_of_extr){
                    break;
                }
                search_max = true;
            }
        }
        if (num_of_extr == max_num_of_extr){
            break;
        }
    }
    if (num_of_extr == 0){
        *no_extr_left = true;
    }
}

void find_first_max_ind(arma::vec* a1, std::vector<int>* max_ind_list, std::vector<int>* min_ind_list) {
    bool search_max = true;
    bool monotonicity_found = false;
    int m = 3;
    int start_ind;
    int first_extr_num;
    bool* no_extr_left;

    while (!monotonicity_found){
        if (m >= a1->size()-1){
            qDebug() << "Error in find_first_max_ind function";
        }
        else if(a1->at(m-2) < a1->at(m-3) && a1->at(m-1) < a1->at(m-2) && a1->at(m) < a1->at(m-1)){
            search_max = false;
            first_extr_num = 5;
            monotonicity_found = true;
            start_ind = m-3;
        }
        else if(a1->at(m-2) > a1->at(m-3) && a1->at(m-1) > a1->at(m-2) && a1->at(m) > a1->at(m-1)){
            search_max = true;
            first_extr_num = 4;
            monotonicity_found = true;
            start_ind = m-3;
        }
        ++ m;
    }

    extremum_search(a1, max_ind_list, min_ind_list, start_ind, search_max, first_extr_num, no_extr_left);
}


void find_extrema(arma::vec data_list, arma::vec time_list, int brth_cyc_ind_dur, double interval_factor,
                  std::vector<int>* max_ind_list, std::vector<int>* min_ind_list, int start_ind){
    // finds all extrema in data_list except the last one

    int interval_size = round(brth_cyc_ind_dur*interval_factor);    // index interval size for extremum search
    int end_ind;
    int cycle_num = 0;
    int num_of_max;
    arma::vec relevant_interval;
    int interv_loc_max_ind;
    int loc_max_ind;
    int interv_loc_min_ind;
    int loc_min_ind;
    bool end_of_data = false;
    bool mono_search;
    bool no_extr_left = false;
    bool only_this = true;

    while (start_ind <= data_list.size()-1 &&  !no_extr_left){
        mono_search = false;
        end_ind = start_ind + interval_size;

        if(end_ind > data_list.size()-1){
            end_ind = data_list.size()-1;
            end_of_data = true;
        }

        relevant_interval = data_list.subvec(start_ind, end_ind);
        interv_loc_max_ind = relevant_interval.index_max();
        loc_max_ind = start_ind + interv_loc_max_ind;

//        if(loc_max_ind == end_ind || no_extr_left){
        if(only_this){
//            if(end_of_data){
//                qDebug() << "";
//                qDebug() << "Break at loc_max_ind in function find_extrema";
//                qDebug() << "Breaking Max index: " << loc_max_ind << "  Breaking Max time: " << time_list[loc_max_ind];
//            break;         // If extremum is last element of data_list it will not be considered as extremum
//            }
//            else{
                qDebug() << "Sucht mit Monotonie ein Max ab t = " << time_list[start_ind] << "bei Index " << start_ind;
                extremum_search(&data_list, max_ind_list, min_ind_list, start_ind, true, 1, &no_extr_left);
                qDebug() << "Max gefunden bei t = " << time_list[max_ind_list->back()] << "bei Index " << max_ind_list->back();
                mono_search = true;
//            }
        }
        else{
            max_ind_list->push_back(loc_max_ind);
        }

        num_of_max = max_ind_list->size();
        if(!mono_search && num_of_max >= 2){
            interval_size = round((max_ind_list->at(num_of_max-1) - max_ind_list->at(num_of_max-2)) * interval_factor);
        }
        else{
            mono_search = false;
        }

        start_ind = max_ind_list->back();
        end_ind = start_ind + interval_size;

        if(end_ind > data_list.size()-1){
            end_ind = data_list.size()-1;
            end_of_data = true;
        }

        if(no_extr_left){
            break;
        }

        relevant_interval = data_list.subvec(start_ind, end_ind);
        interv_loc_min_ind = relevant_interval.index_min();
        loc_min_ind = start_ind + interv_loc_min_ind;

//        if(loc_min_ind == end_ind || no_extr_left){
        if(only_this){
//            if(end_of_data){
//                qDebug() << "";
//                qDebug() << "Break at loc_min_ind in function find_extrema";
//                qDebug() << "Breaking Min index: " << loc_min_ind << "  Breaking Min time: " << time_list[loc_min_ind];
//                break;
//            }
//            else{
                qDebug() << "Sucht mit Monotonie ein Min ab t = " << time_list[start_ind] << "bei Index " << start_ind;
                extremum_search(&data_list, max_ind_list, min_ind_list, start_ind, false, 1, &no_extr_left);
                qDebug() << "Min gefunden bei t = " << time_list[min_ind_list->back()] << "bei Index " << min_ind_list->back();
                mono_search = true;
//            }
        }
        else{
            min_ind_list->push_back(loc_min_ind);
        }

        start_ind = min_ind_list->back();

        ++cycle_num;
    }
}

void check_last_extremum(int last_max_ind, int last_min_ind, bool* ends_with_min){
    // Determines if last extremum is a minimum

    QErrorMessage lastExtrError;

    if (last_max_ind < last_min_ind){
        *ends_with_min = true;
    }
    else if (last_max_ind > last_min_ind){
        *ends_with_min = false;
    }
    else
        lastExtrError.showMessage("Error in function check_last_extremum: Index of last min = Index of last max");
}


void MainWindow::on_Button_clicked()
{
    // Define parameters
    double start_factor = 0.01;      // Cut dataset to start at this percentage
    double end_factor = 0.1;       // Cut dataset to end at this percentage
    int smthMeanWindow = 9;          // Window size for smoothing avergage filter
    double interval_factor = 1.0;   // Adjusts estimated breath cycle duration interval
                                    //   for search interval of next extremum
    int cycles_for_avg = 1;         // Number of last breath cycles considered for
                                    //   calculation of average breath amplitude range
    int intp_factor = 2;            // New amount of datapoints = old amount of datapoints * intp_factor


    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    QString path1 = "SurfaceData/BruMi-20210606-083243_clean.csv";

    QFile f(path1);

    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << f.errorString();
    }


    // Locate row of headers and determine number of columns

    int colNum = 0;
    int lastLnInd = 0;

    QString twoHeaders = { "Time (ms); Pri Amplitude (mm)\r\n" };
    QString threeHeaders = { "Time (ms); Pri Amplitude (mm); Sec Amplitude (mm);\r\n" };

    for (int lnInd = 0; lnInd <= 5; ++lnInd) {
        QString ln1 = f.readLine(0);

        if (lnInd == 5) {
            qDebug() << f.errorString();
            //return false;
        }
        else if (ln1 == twoHeaders) {
            colNum = 2;
            break;
        }
        else if (ln1 == threeHeaders) {
            colNum = 3;
            break;
        }
        lastLnInd += 1;
    }


    // Check if number of columns is permissible

    if (colNum == 0) {
        qDebug() << f.errorString();
    }


    // Read data

    QList<double> TimeList;
    QList<double> Amp1List;
    QList<double> Amp2List;

    while (!f.atEnd()){

        // Read one line
        QStringList SplitLn = readOneLn(f);

        // Save line in lists
        if (!saveValsToLists(TimeList, Amp1List, Amp2List, SplitLn, colNum)) {
            qDebug() << f.errorString();
        }
    }

    // Cut data to certain interval
    int new_start = round(TimeList.size() * start_factor);
    int new_end = round(TimeList.size() * end_factor);
    int new_size = new_end - new_start;
    QList<double> t = TimeList.mid(new_start, new_size);
    QList<double> a1 = Amp1List.mid(new_start, new_size);

    // Smooth list
    QList<double> a1_smth;
    QList<double> t_smth = t.mid(smthMeanWindow/2, t.size()-smthMeanWindow+1);
    meanfilter(&a1, &a1_smth, a1.size(), smthMeanWindow);

    // Interpolation
    arma::vec a1_vec(a1_smth.size());
    arma::vec t_vec(t_smth.size());
    for(int h = 0; h < a1_smth.size(); ++h){
        a1_vec[h] = a1_smth[h];
        t_vec[h] = t_smth[h];
    }
    arma::vec t_intp = arma::linspace(t_vec[0], t_vec[t_vec.size()-1], t_vec.size()*intp_factor);
    arma::vec a1_intp;
    arma::interp1(t_vec, a1_vec, t_intp, a1_intp);


    // Find beginning frequency
    std::vector<int> max_ind_list;
    std::vector<int> min_ind_list;
    find_first_max_ind(&a1_intp, &max_ind_list, &min_ind_list);
    qDebug() << "Size of data: " << a1_intp.size();
    qDebug() << "";
    qDebug() << "-----find_first_max_ind function: ";
    qDebug() << "Number of found maxima: " << max_ind_list.size();
    qDebug() << "Index of last max: " << max_ind_list.back() << "  Time of last max: " << t_intp[max_ind_list.back()];
    double dt_intp = t_intp[1] - t_intp[0];
    int brth_cyc_ind_dur = max_ind_list[1] - max_ind_list[0];
    double brth_cyc_dur = dt_intp * (max_ind_list[1] - max_ind_list[0]);
    qDebug() << "Calculated duration of first breath cycle = " << brth_cyc_dur << " s";

    // Make copy of first maxima for plotting
    std::vector<int> start_max_ind_list = max_ind_list;

    find_extrema(a1_intp, t_intp, brth_cyc_ind_dur, interval_factor, &max_ind_list, &min_ind_list, max_ind_list.back());
    qDebug() << "";
    qDebug() << "-----find_extrema function:";
    qDebug() << "Number of maxima: " << max_ind_list.size();
    qDebug() << "Number of minima: " << min_ind_list.size();

    // Find last extremum
    int last_max_ind = max_ind_list.back();
    int last_min_ind = min_ind_list.back();
    qDebug() << "Index of last max: " << last_max_ind << "  Time of last max: " << t_intp[last_max_ind];
    qDebug() << "Index of last min: " << last_min_ind << "  Time of last min: " << t_intp[last_min_ind];


//    check_last_extremum(last_max_ind, last_min_ind, &ends_with_min);
//    qDebug() << "max_ind_list.back()" << t_intp[max_ind_list.back()];

//    if (ends_with_min == true) {
//        arma::vec a1_maxSubvec = a1_intp.subvec(min_ind_list.back(), a1_intp.size()-1);
//        int last_interv_max_ind = a1_maxSubvec.index_max();
//        qDebug() << "Max: " << a1_maxSubvec[last_interv_max_ind];
//        int last_max_ind = min_ind_list.back() + last_interv_max_ind - 1;
//        max_ind_list.push_back(last_max_ind);
//    }
//    else {
//        arma::vec a1_minSubvec = a1_intp.subvec(max_ind_list.back(), a1_intp.size()-1);
//        int last_interv_min_ind = a1_minSubvec.index_min();
//        int last_min_ind = max_ind_list.back() + last_interv_min_ind - 1;
//        min_ind_list.push_back(last_min_ind);
//    }

    bool ends_with_min;
    check_last_extremum(last_max_ind, last_min_ind, &ends_with_min);

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

    // Calculate average amplitude range for phase seperation
    int num_of_cycles = min_ind_list.size()-1;
    arma::vec ampl_range_list(num_of_cycles);

    for (int cycle_ind = 0; cycle_ind < num_of_cycles; ++cycle_ind){
        double ampl_range = abs(a1_intp[max_ind_list[cycle_ind]] - a1_intp[min_ind_list[cycle_ind]]);
        ampl_range_list[cycle_ind] = ampl_range;
    }

    double avg_ampl_range = mean(ampl_range_list.subvec(ampl_range_list.size()-cycles_for_avg-1, ampl_range_list.size()-1));
    double phase_ampl_range = avg_ampl_range/5;
    qDebug() << "phase amplitude range: " << phase_ampl_range;

    // Determine last phase number
    double last_ampl_range = abs(half_cycle_data.back() - half_cycle_data[0]);
    int last_phase = 0;

    if (last_ampl_range <= phase_ampl_range){
        last_phase = 1;
    }
    else if (last_ampl_range <= phase_ampl_range * 2){
        last_phase = 2;
    }
    else if (last_ampl_range <= phase_ampl_range * 3){
        last_phase = 3;
    }
    else if (last_ampl_range <= phase_ampl_range * 4){
        last_phase = 4;
    }
    else if (last_ampl_range > phase_ampl_range * 4){
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

    // Check phase
    arma::vec phase_border_ind_list(border_num+1);

    for (int phase_num = 0; phase_num <= border_num; ++phase_num){
        double calc_border_val = incr_sign * phase_ampl_range * phase_num + a1_intp[half_cycle_start_ind];
        arma::vec diff_list(half_cycle_data.size());
        for (int hcd_ind = 0; hcd_ind<half_cycle_data.size(); ++hcd_ind){
            float hcd_abs = abs(half_cycle_data[hcd_ind]-calc_border_val);
            diff_list[hcd_ind] = hcd_abs;
        }
        int phase_border_ind = diff_list.index_min();
        phase_border_ind_list(phase_num) = half_cycle_start_ind + phase_border_ind+1;
    }

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
    IntpDataTitle->setText("Interpolated Data");
    IntpDataTitle->setFont(QFont("sans", 10, QFont::Bold));
    IntpDataChart->plotLayout()->insertRow(0); // insert an empty row above the axis rect
    IntpDataChart->plotLayout()->addElement(0, 0, IntpDataTitle); // place the title in the empty cell
    IntpDataChart->addGraph();
    IntpDataChart->graph(0)->setPen(PRPen);

//    // Create QCustomPlot for FFT data
//    FFTChart = new QCustomPlot();
////    QCPBars *freqs = new QCPBars(FFTChart->xAxis, FFTChart->yAxis);
////    freqs->setName("frequencies");
////    //freqs->setPen(QPen(QColor(111, 9, 176).lighter(170)));
////    //freqs->setBrush(QColor(111, 9, 176));
//    FFTChart->setFixedSize(700,210);
//    QCPTextElement *FFTTitle = new QCPTextElement(FFTChart);
//    FFTTitle->setText("Frequency Sprectrum");
//    FFTTitle->setFont(QFont("sans", 10, QFont::Bold));
//    FFTChart->plotLayout()->insertRow(0); // insert an empty row above the axis rect
//    FFTChart->plotLayout()->addElement(0, 0, FFTTitle); // place the title in the empty cell
//    FFTChart->addGraph();
//    FFTChart->graph(0)->setPen(PRPen);

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

//    FFTChart->graph(0)->setData(f_FFT, m_FFT);
//    FFTChart->graph(0)->setLineStyle(QCPGraph::lsNone);
//    FFTChart->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 4));

    QVector<double> tVecPR = tVec.mid(min_ind_list[min_ind_list.size()-2], -1);
    QVector<double> a1VecPR = a1Vec.mid(min_ind_list[min_ind_list.size()-2], -1);
    PhaseRecogChart->graph(0)->setData(tVecPR, a1VecPR);

    // add extrema
    IntpDataChart->addGraph();
    IntpDataChart->graph(1)->setPen(QPen(Qt::red));
    IntpDataChart->graph(1)->setLineStyle(QCPGraph::lsNone);
    IntpDataChart->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 4));
    QVector<double> tMinVec(min_ind_list.size());
    QVector<double> a1MinVec(min_ind_list.size());
    for(int y = 0; y<min_ind_list.size(); ++y){
        tMinVec[y] = t_intp[min_ind_list[y]];
        a1MinVec[y] = a1_intp[min_ind_list[y]];
    }
    IntpDataChart->graph(1)->setData(tMinVec, a1MinVec);

    IntpDataChart->addGraph();
    IntpDataChart->graph(2)->setPen(QPen(Qt::red));
    IntpDataChart->graph(2)->setLineStyle(QCPGraph::lsNone);
    IntpDataChart->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 4));
    QVector<double> tMaxVec(max_ind_list.size());
    QVector<double> a1MaxVec(max_ind_list.size());
    for(int z = 0; z<max_ind_list.size(); ++z){
        tMaxVec[z] = t_intp[max_ind_list[z]];
        a1MaxVec[z] = a1_intp[max_ind_list[z]];
    }
    IntpDataChart->graph(2)->setData(tMaxVec, a1MaxVec);

    IntpDataChart->addGraph();
    IntpDataChart->graph(3)->setPen(QPen(Qt::black));
    IntpDataChart->graph(3)->setLineStyle(QCPGraph::lsNone);
    IntpDataChart->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 8));
    QVector<double> tStartMaxVec(start_max_ind_list.size());
    QVector<double> a1StartMaxVec(start_max_ind_list.size());
    for(int a = 0; a<start_max_ind_list.size(); ++a){
        tStartMaxVec[a] = t_intp[start_max_ind_list[a]];
        a1StartMaxVec[a] = a1_intp[start_max_ind_list[a]];
    }
    IntpDataChart->graph(3)->setData(tStartMaxVec, a1StartMaxVec);

    // Define position parameters for layout
    double a1_Min = *std::min_element(a1VecRaw.begin(), a1VecRaw.end());
    double a1_Max = *std::max_element(a1VecRaw.begin(), a1VecRaw.end());
    qreal xPRAxisRange = tVecRaw.back() - tVecRaw[0];

    double a1PR_Min = *std::min_element(a1VecPR.begin(), a1VecPR.end());
    double a1PR_Max = *std::max_element(a1VecPR.begin(), a1VecPR.end());

//    double m_Min = *std::min_element(m_FFT.begin(), m_FFT.end());
//    double m_Max = *std::max_element(m_FFT.begin(), m_FFT.end());

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
