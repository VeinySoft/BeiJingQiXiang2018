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
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FlightAndRaster
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QListWidget *DateTimeListWidget;
    QLabel *label_3;
    QToolBox *toolBox;
    QWidget *page;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QLabel *label;
    QDateTimeEdit *dateTimeEdit;
    QLabel *label_2;
    QDateTimeEdit *dateTimeEdit_2;
    QPushButton *pushButton;
    QPushButton *pushButton_4;
    QWidget *page_2;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QFrame *frame;
    QMenuBar *menubar;

    void setupUi(QMainWindow *FlightAndRaster)
    {
        if (FlightAndRaster->objectName().isEmpty())
            FlightAndRaster->setObjectName(QString::fromUtf8("FlightAndRaster"));
        FlightAndRaster->resize(1080, 817);
        centralwidget = new QWidget(FlightAndRaster);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setContentsMargins(2, 2, 2, 2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        DateTimeListWidget = new QListWidget(centralwidget);
        DateTimeListWidget->setObjectName(QString::fromUtf8("DateTimeListWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DateTimeListWidget->sizePolicy().hasHeightForWidth());
        DateTimeListWidget->setSizePolicy(sizePolicy);
        DateTimeListWidget->setMinimumSize(QSize(280, 0));
        DateTimeListWidget->setMaximumSize(QSize(1000000, 16777215));
        DateTimeListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        DateTimeListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout->addWidget(DateTimeListWidget, 1, 0, 1, 2);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        toolBox = new QToolBox(centralwidget);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        toolBox->setMaximumSize(QSize(16777215, 250));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 280, 198));
        pushButton_2 = new QPushButton(page);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(0, 24, 129, 23));
        pushButton_3 = new QPushButton(page);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(128, 24, 129, 23));
        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));
        label->setEnabled(true);
        label->setGeometry(QRect(10, 100, 100, 20));
        dateTimeEdit = new QDateTimeEdit(page);
        dateTimeEdit->setObjectName(QString::fromUtf8("dateTimeEdit"));
        dateTimeEdit->setGeometry(QRect(100, 100, 150, 20));
        label_2 = new QLabel(page);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 130, 100, 20));
        dateTimeEdit_2 = new QDateTimeEdit(page);
        dateTimeEdit_2->setObjectName(QString::fromUtf8("dateTimeEdit_2"));
        dateTimeEdit_2->setGeometry(QRect(100, 130, 150, 20));
        pushButton = new QPushButton(page);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(0, 48, 257, 23));
        pushButton_4 = new QPushButton(page);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(0, 0, 257, 25));
        toolBox->addItem(page, QString::fromUtf8("\351\243\236\350\241\214\350\275\250\350\277\271\345\211\226\351\235\242"));
        pushButton_2->raise();
        pushButton_3->raise();
        label->raise();
        dateTimeEdit->raise();
        label_2->raise();
        dateTimeEdit_2->raise();
        pushButton->raise();
        pushButton_4->raise();
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 280, 198));
        label_4 = new QLabel(page_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(6, 4, 31, 16));
        label_5 = new QLabel(page_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(148, 4, 37, 16));
        lineEdit = new QLineEdit(page_2);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(36, 2, 73, 20));
        lineEdit_2 = new QLineEdit(page_2);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(180, 2, 73, 20));
        pushButton_5 = new QPushButton(page_2);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(0, 30, 127, 23));
        pushButton_6 = new QPushButton(page_2);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(132, 30, 123, 23));
        pushButton_7 = new QPushButton(page_2);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(0, 54, 255, 23));
        toolBox->addItem(page_2, QString::fromUtf8("\345\233\272\345\256\232\347\202\271\345\211\226\351\235\242"));

        gridLayout->addWidget(toolBox, 2, 0, 1, 2);

        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setMinimumSize(QSize(200, 0));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);

        gridLayout->addWidget(frame, 1, 2, 3, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        FlightAndRaster->setCentralWidget(centralwidget);
        menubar = new QMenuBar(FlightAndRaster);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setEnabled(false);
        menubar->setGeometry(QRect(0, 0, 1080, 23));
        FlightAndRaster->setMenuBar(menubar);

        retranslateUi(FlightAndRaster);

        toolBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(FlightAndRaster);
    } // setupUi

    void retranslateUi(QMainWindow *FlightAndRaster)
    {
        FlightAndRaster->setWindowTitle(QApplication::translate("FlightAndRaster", "\347\273\217\345\220\221\345\211\226\351\235\242", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("FlightAndRaster", "\350\275\250\350\277\271->\346\225\260\346\215\256:", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("FlightAndRaster", "\345\214\271\351\205\215\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("FlightAndRaster", "\346\270\205\351\231\244\345\214\271\351\205\215", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FlightAndRaster", "\350\265\267\345\247\213:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("FlightAndRaster", "\347\273\223\346\235\237:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("FlightAndRaster", "\347\273\230\345\210\266", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("FlightAndRaster", "\345\212\240\350\275\275\350\275\250\350\277\271...", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("FlightAndRaster", "\351\243\236\350\241\214\350\275\250\350\277\271\345\211\226\351\235\242", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("FlightAndRaster", "\347\273\217\345\272\246:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("FlightAndRaster", "\347\272\254\345\272\246:", 0, QApplication::UnicodeUTF8));
        lineEdit->setText(QApplication::translate("FlightAndRaster", "115.73", 0, QApplication::UnicodeUTF8));
        lineEdit_2->setText(QApplication::translate("FlightAndRaster", "40.52", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("FlightAndRaster", "\345\214\271\351\205\215\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("FlightAndRaster", "\346\270\205\351\231\244\345\214\271\351\205\215", 0, QApplication::UnicodeUTF8));
        pushButton_7->setText(QApplication::translate("FlightAndRaster", "\347\273\230\345\210\266", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("FlightAndRaster", "\345\233\272\345\256\232\347\202\271\345\211\226\351\235\242", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FlightAndRaster: public Ui_FlightAndRaster {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLIGHTANDRASTER_H
