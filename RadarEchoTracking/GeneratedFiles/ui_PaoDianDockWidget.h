/********************************************************************************
** Form generated from reading UI file 'PaoDianDockWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAODIANDOCKWIDGET_H
#define UI_PAODIANDOCKWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PaoDianDockWidget
{
public:
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QTreeView *treeView;

    void setupUi(QDockWidget *PaoDianDockWidget)
    {
        if (PaoDianDockWidget->objectName().isEmpty())
            PaoDianDockWidget->setObjectName(QString::fromUtf8("PaoDianDockWidget"));
        PaoDianDockWidget->resize(333, 412);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout_2 = new QGridLayout(dockWidgetContents);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        treeView = new QTreeView(dockWidgetContents);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        gridLayout->addWidget(treeView, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        PaoDianDockWidget->setWidget(dockWidgetContents);

        retranslateUi(PaoDianDockWidget);

        QMetaObject::connectSlotsByName(PaoDianDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *PaoDianDockWidget)
    {
        PaoDianDockWidget->setWindowTitle(QApplication::translate("PaoDianDockWidget", "\347\202\256\347\202\271\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PaoDianDockWidget: public Ui_PaoDianDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAODIANDOCKWIDGET_H
