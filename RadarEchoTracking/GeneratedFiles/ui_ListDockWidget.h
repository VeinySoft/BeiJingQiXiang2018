/********************************************************************************
** Form generated from reading UI file 'ListDockWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTDOCKWIDGET_H
#define UI_LISTDOCKWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DockList
{
public:
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_6;
    QSpacerItem *horizontalSpacer;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QListView *listView;

    void setupUi(QDockWidget *DockList)
    {
        if (DockList->objectName().isEmpty())
            DockList->setObjectName(QString::fromUtf8("DockList"));
        DockList->resize(326, 578);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(6);
        gridLayout->setVerticalSpacing(3);
        gridLayout->setContentsMargins(0, 0, 0, 3);
        widget = new QWidget(dockWidgetContents);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(0, 26));
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(widget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);

        pushButton_6 = new QPushButton(widget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));

        horizontalLayout->addWidget(pushButton_6);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        horizontalLayout_4->addLayout(horizontalLayout);


        gridLayout->addWidget(widget, 1, 0, 1, 1);

        widget_2 = new QWidget(dockWidgetContents);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton_4 = new QPushButton(widget_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        horizontalLayout_2->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(widget_2);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

        horizontalLayout_2->addWidget(pushButton_5);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        gridLayout->addWidget(widget_2, 5, 0, 1, 1);

        listView = new QListView(dockWidgetContents);
        listView->setObjectName(QString::fromUtf8("listView"));

        gridLayout->addWidget(listView, 4, 0, 1, 1);

        DockList->setWidget(dockWidgetContents);

        retranslateUi(DockList);

        QMetaObject::connectSlotsByName(DockList);
    } // setupUi

    void retranslateUi(QDockWidget *DockList)
    {
        DockList->setWindowTitle(QApplication::translate("DockList", "DockWidget", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("DockList", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("DockList", "\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("DockList", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("DockList", "\345\244\215\345\210\266", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("DockList", "\344\270\212\344\270\200\344\270\252", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("DockList", "\344\270\213\344\270\200\344\270\252", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DockList: public Ui_DockList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTDOCKWIDGET_H
