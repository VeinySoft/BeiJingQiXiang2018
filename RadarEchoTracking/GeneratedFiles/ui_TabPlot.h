/********************************************************************************
** Form generated from reading UI file 'TabPlot.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABPLOT_H
#define UI_TABPLOT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TabPlot
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QPushButton *pushButton;

    void setupUi(QWidget *TabPlot)
    {
        if (TabPlot->objectName().isEmpty())
            TabPlot->setObjectName(QString::fromUtf8("TabPlot"));
        TabPlot->resize(400, 300);
        gridLayout_2 = new QGridLayout(TabPlot);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(TabPlot);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));

        gridLayout->addWidget(tabWidget, 1, 0, 1, 1);

        pushButton = new QPushButton(TabPlot);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(0, 30));
        pushButton->setMaximumSize(QSize(16777215, 60));

        gridLayout->addWidget(pushButton, 2, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(TabPlot);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(TabPlot);
    } // setupUi

    void retranslateUi(QWidget *TabPlot)
    {
        TabPlot->setWindowTitle(QApplication::translate("TabPlot", "Form", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("TabPlot", "\346\213\267\350\264\235\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TabPlot: public Ui_TabPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABPLOT_H
