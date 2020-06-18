/********************************************************************************
** Form generated from reading UI file 'SectionDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECTIONDIALOG_H
#define UI_SECTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SectionDialog
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QDialog *SectionDialog)
    {
        if (SectionDialog->objectName().isEmpty())
            SectionDialog->setObjectName(QString::fromUtf8("SectionDialog"));
        SectionDialog->resize(883, 448);
        gridLayout_2 = new QGridLayout(SectionDialog);
        gridLayout_2->setContentsMargins(3, 3, 3, 3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new QGraphicsView(SectionDialog);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_3 = new QPushButton(SectionDialog);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setCheckable(true);

        horizontalLayout->addWidget(pushButton_3);

        pushButton_2 = new QPushButton(SectionDialog);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(SectionDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(SectionDialog);

        QMetaObject::connectSlotsByName(SectionDialog);
    } // setupUi

    void retranslateUi(QDialog *SectionDialog)
    {
        SectionDialog->setWindowTitle(QApplication::translate("SectionDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("SectionDialog", "\346\250\252\345\235\220\346\240\207\344\270\272\347\273\217\347\272\254\345\272\246", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("SectionDialog", "\345\257\274\345\207\272CVS", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("SectionDialog", "\344\277\235\345\255\230\345\233\276\347\211\207", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SectionDialog: public Ui_SectionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECTIONDIALOG_H
