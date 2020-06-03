/********************************************************************************
** Form generated from reading UI file 'ClipNcFileImage.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIPNCFILEIMAGE_H
#define UI_CLIPNCFILEIMAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NcFileImageWidget
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QGraphicsView *graphicsView;

    void setupUi(QWidget *NcFileImageWidget)
    {
        if (NcFileImageWidget->objectName().isEmpty())
            NcFileImageWidget->setObjectName(QString::fromUtf8("NcFileImageWidget"));
        NcFileImageWidget->resize(400, 300);
        gridLayout = new QGridLayout(NcFileImageWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listWidget = new QListWidget(NcFileImageWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setMaximumSize(QSize(120, 16777215));
        listWidget->setIconSize(QSize(150, 200));

        horizontalLayout->addWidget(listWidget);

        graphicsView = new QGraphicsView(NcFileImageWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        horizontalLayout->addWidget(graphicsView);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(NcFileImageWidget);

        QMetaObject::connectSlotsByName(NcFileImageWidget);
    } // setupUi

    void retranslateUi(QWidget *NcFileImageWidget)
    {
        NcFileImageWidget->setWindowTitle(QApplication::translate("NcFileImageWidget", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NcFileImageWidget: public Ui_NcFileImageWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIPNCFILEIMAGE_H
