/********************************************************************************
** Form generated from reading UI file 'chartdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTDIALOG_H
#define UI_CHARTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_ChartDialog
{
public:

    void setupUi(QDialog *ChartDialog)
    {
        if (ChartDialog->objectName().isEmpty())
            ChartDialog->setObjectName(QString::fromUtf8("ChartDialog"));
        ChartDialog->resize(400, 300);

        retranslateUi(ChartDialog);

        QMetaObject::connectSlotsByName(ChartDialog);
    } // setupUi

    void retranslateUi(QDialog *ChartDialog)
    {
        ChartDialog->setWindowTitle(QCoreApplication::translate("ChartDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChartDialog: public Ui_ChartDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTDIALOG_H
