/********************************************************************************
** Form generated from reading UI file 'CubeSettings.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUBESETTINGS_H
#define UI_CUBESETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CubeSettingsDialog
{
public:
    QDialogButtonBox *buttonBox;
    QComboBox *HeightcomboBox;
    QLabel *label_3;
    QGroupBox *groupBox;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *LeftTopLonlineEdit;
    QLineEdit *LeftTopLatlineEdit;
    QGroupBox *groupBox_2;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *RightBottonLonlineEdit;
    QLineEdit *RightBottonLatlineEdit;
    QLabel *label_4;
    QLineEdit *NamelineEdit;
    QLabel *label_5;
    QComboBox *HeightDownComboBox;
    QComboBox *HeightUpComboBox;
    QLabel *label_8;
    QGroupBox *groupBox_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_7;
    QCheckBox *checkBox_8;
    QCheckBox *checkBox_11;
    QCheckBox *checkBox_14;
    QCheckBox *checkBox_9;
    QCheckBox *checkBox_15;
    QCheckBox *checkBox_10;
    QCheckBox *checkBox_12;
    QCheckBox *checkBox_1;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_13;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_16;
    QWidget *tab_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QListWidget *listWidget;
    QLineEdit *lineEdit;
    QLabel *label_11;
    QLineEdit *lineEdit_2;
    QLabel *label_9;
    QLabel *label_10;
    QGroupBox *groupBox_4;
    QComboBox *comboBox;
    QGroupBox *groupBox_5;
    QComboBox *comboBox_2;

    void setupUi(QDialog *CubeSettingsDialog)
    {
        if (CubeSettingsDialog->objectName().isEmpty())
            CubeSettingsDialog->setObjectName(QString::fromUtf8("CubeSettingsDialog"));
        CubeSettingsDialog->resize(459, 439);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../Debug/icon/boxSetting.png"), QSize(), QIcon::Normal, QIcon::Off);
        CubeSettingsDialog->setWindowIcon(icon);
        buttonBox = new QDialogButtonBox(CubeSettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(284, 404, 163, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        HeightcomboBox = new QComboBox(CubeSettingsDialog);
        HeightcomboBox->setObjectName(QString::fromUtf8("HeightcomboBox"));
        HeightcomboBox->setGeometry(QRect(158, 500, 63, 22));
        label_3 = new QLabel(CubeSettingsDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(120, 496, 31, 25));
        groupBox = new QGroupBox(CubeSettingsDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 72, 191, 81));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 20, 31, 21));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 50, 31, 21));
        LeftTopLonlineEdit = new QLineEdit(groupBox);
        LeftTopLonlineEdit->setObjectName(QString::fromUtf8("LeftTopLonlineEdit"));
        LeftTopLonlineEdit->setGeometry(QRect(50, 20, 113, 20));
        LeftTopLatlineEdit = new QLineEdit(groupBox);
        LeftTopLatlineEdit->setObjectName(QString::fromUtf8("LeftTopLatlineEdit"));
        LeftTopLatlineEdit->setGeometry(QRect(50, 50, 113, 20));
        LeftTopLatlineEdit->raise();
        label_2->raise();
        label->raise();
        LeftTopLonlineEdit->raise();
        groupBox_2 = new QGroupBox(CubeSettingsDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(210, 72, 191, 81));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 20, 31, 21));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 50, 31, 21));
        RightBottonLonlineEdit = new QLineEdit(groupBox_2);
        RightBottonLonlineEdit->setObjectName(QString::fromUtf8("RightBottonLonlineEdit"));
        RightBottonLonlineEdit->setGeometry(QRect(50, 20, 113, 20));
        RightBottonLatlineEdit = new QLineEdit(groupBox_2);
        RightBottonLatlineEdit->setObjectName(QString::fromUtf8("RightBottonLatlineEdit"));
        RightBottonLatlineEdit->setGeometry(QRect(50, 50, 113, 20));
        label_4 = new QLabel(CubeSettingsDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 12, 31, 21));
        NamelineEdit = new QLineEdit(CubeSettingsDialog);
        NamelineEdit->setObjectName(QString::fromUtf8("NamelineEdit"));
        NamelineEdit->setGeometry(QRect(60, 12, 339, 20));
        label_5 = new QLabel(CubeSettingsDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(18, 40, 61, 25));
        HeightDownComboBox = new QComboBox(CubeSettingsDialog);
        HeightDownComboBox->setObjectName(QString::fromUtf8("HeightDownComboBox"));
        HeightDownComboBox->setGeometry(QRect(78, 40, 95, 22));
        HeightUpComboBox = new QComboBox(CubeSettingsDialog);
        HeightUpComboBox->setObjectName(QString::fromUtf8("HeightUpComboBox"));
        HeightUpComboBox->setGeometry(QRect(278, 40, 79, 22));
        label_8 = new QLabel(CubeSettingsDialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(216, 40, 61, 25));
        groupBox_3 = new QGroupBox(CubeSettingsDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(8, 162, 443, 155));
        tabWidget = new QTabWidget(groupBox_3);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(6, 22, 429, 123));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        checkBox_5 = new QCheckBox(tab);
        checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));
        checkBox_5->setGeometry(QRect(218, 22, 35, 16));
        checkBox_6 = new QCheckBox(tab);
        checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));
        checkBox_6->setGeometry(QRect(266, 22, 35, 16));
        checkBox_7 = new QCheckBox(tab);
        checkBox_7->setObjectName(QString::fromUtf8("checkBox_7"));
        checkBox_7->setGeometry(QRect(308, 22, 37, 16));
        checkBox_8 = new QCheckBox(tab);
        checkBox_8->setObjectName(QString::fromUtf8("checkBox_8"));
        checkBox_8->setGeometry(QRect(350, 22, 35, 16));
        checkBox_11 = new QCheckBox(tab);
        checkBox_11->setObjectName(QString::fromUtf8("checkBox_11"));
        checkBox_11->setGeometry(QRect(136, 58, 35, 16));
        checkBox_14 = new QCheckBox(tab);
        checkBox_14->setObjectName(QString::fromUtf8("checkBox_14"));
        checkBox_14->setGeometry(QRect(266, 58, 33, 16));
        checkBox_9 = new QCheckBox(tab);
        checkBox_9->setObjectName(QString::fromUtf8("checkBox_9"));
        checkBox_9->setGeometry(QRect(46, 58, 39, 16));
        checkBox_15 = new QCheckBox(tab);
        checkBox_15->setObjectName(QString::fromUtf8("checkBox_15"));
        checkBox_15->setGeometry(QRect(310, 58, 35, 16));
        checkBox_10 = new QCheckBox(tab);
        checkBox_10->setObjectName(QString::fromUtf8("checkBox_10"));
        checkBox_10->setGeometry(QRect(94, 58, 37, 16));
        checkBox_12 = new QCheckBox(tab);
        checkBox_12->setObjectName(QString::fromUtf8("checkBox_12"));
        checkBox_12->setGeometry(QRect(178, 58, 37, 16));
        checkBox_1 = new QCheckBox(tab);
        checkBox_1->setObjectName(QString::fromUtf8("checkBox_1"));
        checkBox_1->setGeometry(QRect(46, 22, 39, 16));
        checkBox_2 = new QCheckBox(tab);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setGeometry(QRect(94, 22, 37, 16));
        checkBox_4 = new QCheckBox(tab);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
        checkBox_4->setGeometry(QRect(178, 22, 33, 16));
        checkBox_13 = new QCheckBox(tab);
        checkBox_13->setObjectName(QString::fromUtf8("checkBox_13"));
        checkBox_13->setGeometry(QRect(218, 58, 33, 16));
        checkBox_3 = new QCheckBox(tab);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setGeometry(QRect(136, 22, 37, 16));
        checkBox_16 = new QCheckBox(tab);
        checkBox_16->setObjectName(QString::fromUtf8("checkBox_16"));
        checkBox_16->setGeometry(QRect(350, 58, 37, 16));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        pushButton = new QPushButton(tab_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(152, 20, 75, 23));
        pushButton_2 = new QPushButton(tab_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(152, 50, 75, 23));
        listWidget = new QListWidget(tab_2);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(252, 4, 163, 89));
        lineEdit = new QLineEdit(tab_2);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(4, 36, 55, 20));
        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(68, 38, 16, 16));
        lineEdit_2 = new QLineEdit(tab_2);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(82, 36, 55, 20));
        tabWidget->addTab(tab_2, QString());
        label_9 = new QLabel(CubeSettingsDialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(176, 44, 41, 16));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        label_9->setFont(font);
        label_10 = new QLabel(CubeSettingsDialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(358, 44, 33, 16));
        label_10->setFont(font);
        groupBox_4 = new QGroupBox(CubeSettingsDialog);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(8, 334, 209, 59));
        comboBox = new QComboBox(groupBox_4);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(20, 22, 173, 22));
        groupBox_5 = new QGroupBox(CubeSettingsDialog);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(222, 334, 225, 59));
        comboBox_2 = new QComboBox(groupBox_5);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setGeometry(QRect(22, 20, 187, 22));

        retranslateUi(CubeSettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CubeSettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CubeSettingsDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CubeSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *CubeSettingsDialog)
    {
        CubeSettingsDialog->setWindowTitle(QApplication::translate("CubeSettingsDialog", "\350\267\237\350\270\252\346\241\206\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CubeSettingsDialog", "\351\253\230\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CubeSettingsDialog", "\345\267\246\344\270\212", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CubeSettingsDialog", "\347\272\254\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CubeSettingsDialog", "\347\273\217\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("CubeSettingsDialog", "\345\217\263\344\270\213", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CubeSettingsDialog", "\347\272\254\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CubeSettingsDialog", "\347\273\217\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CubeSettingsDialog", "\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CubeSettingsDialog", "\344\270\213\351\231\220\351\253\230\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CubeSettingsDialog", "\344\270\212\351\231\220\351\253\230\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("CubeSettingsDialog", "\345\217\215\345\260\204\347\216\207\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        checkBox_5->setText(QApplication::translate("CubeSettingsDialog", "15", 0, QApplication::UnicodeUTF8));
        checkBox_6->setText(QApplication::translate("CubeSettingsDialog", "20", 0, QApplication::UnicodeUTF8));
        checkBox_7->setText(QApplication::translate("CubeSettingsDialog", "25", 0, QApplication::UnicodeUTF8));
        checkBox_8->setText(QApplication::translate("CubeSettingsDialog", "30", 0, QApplication::UnicodeUTF8));
        checkBox_11->setText(QApplication::translate("CubeSettingsDialog", "45", 0, QApplication::UnicodeUTF8));
        checkBox_14->setText(QApplication::translate("CubeSettingsDialog", "60", 0, QApplication::UnicodeUTF8));
        checkBox_9->setText(QApplication::translate("CubeSettingsDialog", "35", 0, QApplication::UnicodeUTF8));
        checkBox_15->setText(QApplication::translate("CubeSettingsDialog", "65", 0, QApplication::UnicodeUTF8));
        checkBox_10->setText(QApplication::translate("CubeSettingsDialog", "40", 0, QApplication::UnicodeUTF8));
        checkBox_12->setText(QApplication::translate("CubeSettingsDialog", "50", 0, QApplication::UnicodeUTF8));
        checkBox_1->setText(QApplication::translate("CubeSettingsDialog", "-5", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("CubeSettingsDialog", "0", 0, QApplication::UnicodeUTF8));
        checkBox_4->setText(QApplication::translate("CubeSettingsDialog", "10", 0, QApplication::UnicodeUTF8));
        checkBox_13->setText(QApplication::translate("CubeSettingsDialog", "55", 0, QApplication::UnicodeUTF8));
        checkBox_3->setText(QApplication::translate("CubeSettingsDialog", "5", 0, QApplication::UnicodeUTF8));
        checkBox_16->setText(QApplication::translate("CubeSettingsDialog", "70", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("CubeSettingsDialog", "\351\200\211\346\213\251\345\267\262\346\234\211\347\232\204", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("CubeSettingsDialog", ">>", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("CubeSettingsDialog", "<<", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("CubeSettingsDialog", "-", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("CubeSettingsDialog", "\345\205\250\351\203\250\346\211\213\345\212\250\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("CubeSettingsDialog", "\357\274\210\347\261\263\357\274\211", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("CubeSettingsDialog", "\357\274\210\347\261\263\357\274\211", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("CubeSettingsDialog", "\345\233\236\346\263\242\351\241\266\351\253\230\351\230\200\345\200\274", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("CubeSettingsDialog", "\345\233\236\346\263\242\344\275\223\347\247\257\351\230\200\345\200\274", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CubeSettingsDialog: public Ui_CubeSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUBESETTINGS_H
