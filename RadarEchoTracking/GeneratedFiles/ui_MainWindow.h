/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindow
{
public:
    QAction *action;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName(QString::fromUtf8("mainWindow"));
        mainWindow->resize(800, 600);
        QIcon icon;
        icon.addFile(QString::fromUtf8("icon/mainWindow.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainWindow->setWindowIcon(icon);
        action = new QAction(mainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("icon/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        action->setIcon(icon1);
        centralwidget = new QWidget(mainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        mainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        mainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(mainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        mainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(mainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        mainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(mainWindow);

        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindow)
    {
        mainWindow->setWindowTitle(QApplication::translate("mainWindow", "\346\260\224\350\261\241\351\233\267\350\276\276\350\267\237\350\270\252\347\263\273\347\273\237", 0, QApplication::UnicodeUTF8));
        action->setText(QApplication::translate("mainWindow", "\351\200\200\345\207\272(&X)", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("mainWindow", "\345\267\245\345\205\267\346\240\217", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class mainWindow: public Ui_mainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
