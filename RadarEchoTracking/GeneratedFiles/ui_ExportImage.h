/********************************************************************************
** Form generated from reading UI file 'ExportImage.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTIMAGE_H
#define UI_EXPORTIMAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_ExportDialog
{
public:
    QDialogButtonBox *buttonBox;
    QComboBox *comboBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;

    void setupUi(QDialog *ExportDialog)
    {
        if (ExportDialog->objectName().isEmpty())
            ExportDialog->setObjectName(QString::fromUtf8("ExportDialog"));
        ExportDialog->resize(202, 163);
        buttonBox = new QDialogButtonBox(ExportDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(38, 118, 147, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        comboBox = new QComboBox(ExportDialog);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(46, 76, 139, 22));
        label = new QLabel(ExportDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(12, 12, 54, 12));
        label_2 = new QLabel(ExportDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(12, 46, 54, 12));
        label_3 = new QLabel(ExportDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(12, 80, 54, 12));
        lineEdit = new QLineEdit(ExportDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(44, 8, 139, 20));
        lineEdit_2 = new QLineEdit(ExportDialog);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(44, 42, 141, 20));

        retranslateUi(ExportDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ExportDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ExportDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ExportDialog);
    } // setupUi

    void retranslateUi(QDialog *ExportDialog)
    {
        ExportDialog->setWindowTitle(QApplication::translate("ExportDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ExportDialog", "\345\256\275\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ExportDialog", "\351\253\230\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ExportDialog", "\346\240\274\345\274\217\357\274\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ExportDialog: public Ui_ExportDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTIMAGE_H
