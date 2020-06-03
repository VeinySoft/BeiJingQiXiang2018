/********************************************************************************
** Form generated from reading UI file 'FlightAndRaster.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLIGHTANDRASTER_H
#define UI_FLIGHTANDRASTER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FlightAndRaster
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QDateTimeEdit *dateTimeEdit;
    QDateTimeEdit *dateTimeEdit_2;
    QFrame *frame;
    QLabel *label;
    QLabel *label_2;
    QListWidget *DateTimeListWidget;
    QPushButton *pushButton;
    QLabel *label_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QMenuBar *menubar;

    void setupUi(QMainWindow *FlightAndRaster)
    {
        if (FlightAndRaster->objectName().isEmpty())
            FlightAndRaster->setObjectName(QString::fromUtf8("FlightAndRaster"));
        FlightAndRaster->resize(1080, 729);
        centralwidget = new QWidget(FlightAndRaster);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setContentsMargins(2, 2, 2, 2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        dateTimeEdit = new QDateTimeEdit(centralwidget);
        dateTimeEdit->setObjectName(QString::fromUtf8("dateTimeEdit"));

        gridLayout->addWidget(dateTimeEdit, 4, 1, 1, 1);

        dateTimeEdit_2 = new QDateTimeEdit(centralwidget);
        dateTimeEdit_2->setObjectName(QString::fromUtf8("dateTimeEdit_2"));

        gridLayout->addWidget(dateTimeEdit_2, 5, 1, 1, 1);

        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(200, 0));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);

        gridLayout->addWidget(frame, 1, 2, 6, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 4, 0, 1, 1);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 5, 0, 1, 1);

        DateTimeListWidget = new QListWidget(centralwidget);
        DateTimeListWidget->setObjectName(QString::fromUtf8("DateTimeListWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(DateTimeListWidget->sizePolicy().hasHeightForWidth());
        DateTimeListWidget->setSizePolicy(sizePolicy1);
        DateTimeListWidget->setMinimumSize(QSize(130, 0));
        DateTimeListWidget->setMaximumSize(QSize(400, 16777215));
        DateTimeListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        DateTimeListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout->addWidget(DateTimeListWidget, 1, 0, 1, 2);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 6, 0, 1, 2);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 2, 0, 1, 2);

        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 3, 0, 1, 2);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        FlightAndRaster->setCentralWidget(centralwidget);
        menubar = new QMenuBar(FlightAndRaster);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setEnabled(false);
        menubar->setGeometry(QRect(0, 0, 1080, 23));
        FlightAndRaster->setMenuBar(menubar);

        retranslateUi(FlightAndRaster);

        QMetaObject::connectSlotsByName(FlightAndRaster);
    } // setupUi

    void retranslateUi(QMainWindow *FlightAndRaster)
    {
        FlightAndRaster->setWindowTitle(QApplication::translate("FlightAndRaster", "\347\273\217\345\220\221\345\211\226\351\235\242", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FlightAndRaster", "\350\265\267\345\247\213:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("FlightAndRaster", "\347\273\223\346\235\237:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("FlightAndRaster", "\347\273\230\345\210\266", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("FlightAndRaster", "\350\275\250\350\277\271->\346\225\260\346\215\256:", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("FlightAndRaster", "\345\214\271\351\205\215\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("FlightAndRaster", "\346\270\205\351\231\244\345\214\271\351\205\215", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FlightAndRaster: public Ui_FlightAndRaster {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLIGHTANDRASTER_H
