/********************************************************************************
** Form generated from reading UI file 'QwtPlotDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QWTPLOTDIALOG_H
#define UI_QWTPLOTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_QwtPlotDialogSetup
{
public:
    QwtPlot *qwtPlot;

    void setupUi(QDialog *QwtPlotDialogSetup)
    {
        if (QwtPlotDialogSetup->objectName().isEmpty())
            QwtPlotDialogSetup->setObjectName(QString::fromUtf8("QwtPlotDialogSetup"));
        QwtPlotDialogSetup->resize(587, 358);
        qwtPlot = new QwtPlot(QwtPlotDialogSetup);
        qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));
        qwtPlot->setGeometry(QRect(14, 20, 553, 321));
        QBrush brush(QColor(240, 240, 240, 255));
        brush.setStyle(Qt::SolidPattern);
        qwtPlot->setCanvasBackground(brush);
        qwtPlot->setAutoReplot(false);

        retranslateUi(QwtPlotDialogSetup);

        QMetaObject::connectSlotsByName(QwtPlotDialogSetup);
    } // setupUi

    void retranslateUi(QDialog *QwtPlotDialogSetup)
    {
        QwtPlotDialogSetup->setWindowTitle(QApplication::translate("QwtPlotDialogSetup", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QwtPlotDialogSetup: public Ui_QwtPlotDialogSetup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QWTPLOTDIALOG_H
