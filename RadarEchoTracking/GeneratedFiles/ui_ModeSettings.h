/********************************************************************************
** Form generated from reading UI file 'ModeSettings.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODESETTINGS_H
#define UI_MODESETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModeSettingDialog
{
public:
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QGroupBox *groupBox;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QGroupBox *groupBox_2;
    QLabel *label_4;
    QComboBox *comboBox;
    QListView *listView;
    QGroupBox *groupBox_4;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QGroupBox *groupBox_3;
    QCheckBox *checkBox_1;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_7;
    QCheckBox *checkBox_8;
    QCheckBox *checkBox_9;
    QCheckBox *checkBox_10;
    QCheckBox *checkBox_11;
    QCheckBox *checkBox_12;
    QCheckBox *checkBox_13;
    QCheckBox *checkBox_14;
    QCheckBox *checkBox_15;
    QCheckBox *checkBox_16;
    QPushButton *ApplaypushButton;
    QGroupBox *groupBox_5;
    QPushButton *SetPathPushButton;
    QLabel *label;
    QLineEdit *PathLineEdit;
    QGroupBox *groupBox_6;
    QLabel *label_2;
    QDateTimeEdit *dateTimeEdit_2;
    QPushButton *StartFilePushButton;
    QLabel *label_5;
    QDateTimeEdit *dateTimeEdit;
    QPushButton *EndFilePushButton;
    QLabel *label_3;
    QPushButton *SetPathPushButton_2;
    QLineEdit *PathLineEdit_2;
    QGroupBox *groupBox_7;
    QLabel *label_6;
    QLineEdit *PathLineEdit_3;
    QPushButton *SetPathPushButton_3;

    void setupUi(QDialog *ModeSettingDialog)
    {
        if (ModeSettingDialog->objectName().isEmpty())
            ModeSettingDialog->setObjectName(QString::fromUtf8("ModeSettingDialog"));
        ModeSettingDialog->resize(632, 318);
        buttonBox = new QDialogButtonBox(ModeSettingDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(454, 272, 167, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tabWidget = new QTabWidget(ModeSettingDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(8, 588, 619, 111));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        groupBox = new QGroupBox(ModeSettingDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 355, 279, 51));
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(10, 18, 89, 21));
        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(130, 18, 89, 21));
        groupBox_2 = new QGroupBox(ModeSettingDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(12, 342, 617, 219));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(22, 30, 54, 12));
        comboBox = new QComboBox(groupBox_2);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(68, 24, 515, 22));
        listView = new QListView(groupBox_2);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(20, 104, 563, 107));
        groupBox_4 = new QGroupBox(groupBox_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(22, 54, 561, 45));
        pushButton = new QPushButton(groupBox_4);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(6, 14, 55, 23));
        pushButton_2 = new QPushButton(groupBox_4);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(68, 14, 49, 23));
        pushButton_3 = new QPushButton(groupBox_4);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(122, 14, 55, 23));
        groupBox_3 = new QGroupBox(groupBox_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(326, 26, 273, 185));
        checkBox_1 = new QCheckBox(groupBox_3);
        checkBox_1->setObjectName(QString::fromUtf8("checkBox_1"));
        checkBox_1->setGeometry(QRect(58, 36, 39, 16));
        checkBox_2 = new QCheckBox(groupBox_3);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setGeometry(QRect(106, 36, 71, 16));
        checkBox_3 = new QCheckBox(groupBox_3);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setGeometry(QRect(148, 36, 71, 16));
        checkBox_4 = new QCheckBox(groupBox_3);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
        checkBox_4->setGeometry(QRect(190, 36, 71, 16));
        checkBox_5 = new QCheckBox(groupBox_3);
        checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));
        checkBox_5->setGeometry(QRect(58, 70, 71, 16));
        checkBox_6 = new QCheckBox(groupBox_3);
        checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));
        checkBox_6->setGeometry(QRect(106, 70, 71, 16));
        checkBox_7 = new QCheckBox(groupBox_3);
        checkBox_7->setObjectName(QString::fromUtf8("checkBox_7"));
        checkBox_7->setGeometry(QRect(148, 70, 71, 16));
        checkBox_8 = new QCheckBox(groupBox_3);
        checkBox_8->setObjectName(QString::fromUtf8("checkBox_8"));
        checkBox_8->setGeometry(QRect(190, 70, 71, 16));
        checkBox_9 = new QCheckBox(groupBox_3);
        checkBox_9->setObjectName(QString::fromUtf8("checkBox_9"));
        checkBox_9->setGeometry(QRect(58, 102, 71, 16));
        checkBox_10 = new QCheckBox(groupBox_3);
        checkBox_10->setObjectName(QString::fromUtf8("checkBox_10"));
        checkBox_10->setGeometry(QRect(106, 102, 71, 16));
        checkBox_11 = new QCheckBox(groupBox_3);
        checkBox_11->setObjectName(QString::fromUtf8("checkBox_11"));
        checkBox_11->setGeometry(QRect(148, 102, 71, 16));
        checkBox_12 = new QCheckBox(groupBox_3);
        checkBox_12->setObjectName(QString::fromUtf8("checkBox_12"));
        checkBox_12->setGeometry(QRect(190, 102, 71, 16));
        checkBox_13 = new QCheckBox(groupBox_3);
        checkBox_13->setObjectName(QString::fromUtf8("checkBox_13"));
        checkBox_13->setGeometry(QRect(58, 138, 33, 16));
        checkBox_14 = new QCheckBox(groupBox_3);
        checkBox_14->setObjectName(QString::fromUtf8("checkBox_14"));
        checkBox_14->setGeometry(QRect(106, 138, 33, 16));
        checkBox_15 = new QCheckBox(groupBox_3);
        checkBox_15->setObjectName(QString::fromUtf8("checkBox_15"));
        checkBox_15->setGeometry(QRect(150, 138, 35, 16));
        checkBox_16 = new QCheckBox(groupBox_3);
        checkBox_16->setObjectName(QString::fromUtf8("checkBox_16"));
        checkBox_16->setGeometry(QRect(190, 138, 37, 16));
        ApplaypushButton = new QPushButton(ModeSettingDialog);
        ApplaypushButton->setObjectName(QString::fromUtf8("ApplaypushButton"));
        ApplaypushButton->setGeometry(QRect(376, 374, 75, 23));
        groupBox_5 = new QGroupBox(ModeSettingDialog);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 12, 615, 63));
        SetPathPushButton = new QPushButton(groupBox_5);
        SetPathPushButton->setObjectName(QString::fromUtf8("SetPathPushButton"));
        SetPathPushButton->setGeometry(QRect(558, 20, 41, 23));
        label = new QLabel(groupBox_5);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(24, 20, 43, 21));
        PathLineEdit = new QLineEdit(groupBox_5);
        PathLineEdit->setObjectName(QString::fromUtf8("PathLineEdit"));
        PathLineEdit->setGeometry(QRect(68, 22, 491, 20));
        groupBox_6 = new QGroupBox(ModeSettingDialog);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(8, 86, 615, 103));
        label_2 = new QLabel(groupBox_6);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(12, 64, 54, 19));
        dateTimeEdit_2 = new QDateTimeEdit(groupBox_6);
        dateTimeEdit_2->setObjectName(QString::fromUtf8("dateTimeEdit_2"));
        dateTimeEdit_2->setGeometry(QRect(360, 63, 139, 22));
        dateTimeEdit_2->setCalendarPopup(true);
        StartFilePushButton = new QPushButton(groupBox_6);
        StartFilePushButton->setObjectName(QString::fromUtf8("StartFilePushButton"));
        StartFilePushButton->setGeometry(QRect(204, 63, 35, 23));
        label_5 = new QLabel(groupBox_6);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 29, 54, 21));
        dateTimeEdit = new QDateTimeEdit(groupBox_6);
        dateTimeEdit->setObjectName(QString::fromUtf8("dateTimeEdit"));
        dateTimeEdit->setGeometry(QRect(70, 63, 137, 22));
        dateTimeEdit->setCurrentSection(QDateTimeEdit::YearSection);
        dateTimeEdit->setCalendarPopup(true);
        EndFilePushButton = new QPushButton(groupBox_6);
        EndFilePushButton->setObjectName(QString::fromUtf8("EndFilePushButton"));
        EndFilePushButton->setGeometry(QRect(498, 63, 35, 23));
        label_3 = new QLabel(groupBox_6);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(306, 66, 54, 15));
        SetPathPushButton_2 = new QPushButton(groupBox_6);
        SetPathPushButton_2->setObjectName(QString::fromUtf8("SetPathPushButton_2"));
        SetPathPushButton_2->setGeometry(QRect(560, 27, 41, 23));
        PathLineEdit_2 = new QLineEdit(groupBox_6);
        PathLineEdit_2->setObjectName(QString::fromUtf8("PathLineEdit_2"));
        PathLineEdit_2->setGeometry(QRect(70, 29, 491, 20));
        groupBox_7 = new QGroupBox(ModeSettingDialog);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setGeometry(QRect(8, 196, 615, 55));
        label_6 = new QLabel(groupBox_7);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(12, 20, 101, 16));
        PathLineEdit_3 = new QLineEdit(groupBox_7);
        PathLineEdit_3->setObjectName(QString::fromUtf8("PathLineEdit_3"));
        PathLineEdit_3->setGeometry(QRect(72, 18, 491, 20));
        SetPathPushButton_3 = new QPushButton(groupBox_7);
        SetPathPushButton_3->setObjectName(QString::fromUtf8("SetPathPushButton_3"));
        SetPathPushButton_3->setGeometry(QRect(562, 16, 41, 23));

        retranslateUi(ModeSettingDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ModeSettingDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ModeSettingDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ModeSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *ModeSettingDialog)
    {
        ModeSettingDialog->setWindowTitle(QApplication::translate("ModeSettingDialog", "\347\273\274\345\220\210\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ModeSettingDialog", "\345\256\236\346\227\266\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ModeSettingDialog", "\345\216\206\345\217\262\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ModeSettingDialog", "\346\250\241\345\274\217\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("ModeSettingDialog", "\345\256\236\346\227\266\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("ModeSettingDialog", "\345\216\206\345\217\262\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("ModeSettingDialog", "\350\267\237\350\270\252\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ModeSettingDialog", "\350\267\237\350\270\252\346\241\206\357\274\232", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("ModeSettingDialog", "\350\267\237\350\270\252\346\241\206\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("ModeSettingDialog", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("ModeSettingDialog", "\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("ModeSettingDialog", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("ModeSettingDialog", "\345\217\215\345\260\204\347\216\207\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        checkBox_1->setText(QApplication::translate("ModeSettingDialog", "-5", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("ModeSettingDialog", "0", 0, QApplication::UnicodeUTF8));
        checkBox_3->setText(QApplication::translate("ModeSettingDialog", "5", 0, QApplication::UnicodeUTF8));
        checkBox_4->setText(QApplication::translate("ModeSettingDialog", "10", 0, QApplication::UnicodeUTF8));
        checkBox_5->setText(QApplication::translate("ModeSettingDialog", "15", 0, QApplication::UnicodeUTF8));
        checkBox_6->setText(QApplication::translate("ModeSettingDialog", "20", 0, QApplication::UnicodeUTF8));
        checkBox_7->setText(QApplication::translate("ModeSettingDialog", "25", 0, QApplication::UnicodeUTF8));
        checkBox_8->setText(QApplication::translate("ModeSettingDialog", "30", 0, QApplication::UnicodeUTF8));
        checkBox_9->setText(QApplication::translate("ModeSettingDialog", "35", 0, QApplication::UnicodeUTF8));
        checkBox_10->setText(QApplication::translate("ModeSettingDialog", "40", 0, QApplication::UnicodeUTF8));
        checkBox_11->setText(QApplication::translate("ModeSettingDialog", "45", 0, QApplication::UnicodeUTF8));
        checkBox_12->setText(QApplication::translate("ModeSettingDialog", "50", 0, QApplication::UnicodeUTF8));
        checkBox_13->setText(QApplication::translate("ModeSettingDialog", "55", 0, QApplication::UnicodeUTF8));
        checkBox_14->setText(QApplication::translate("ModeSettingDialog", "60", 0, QApplication::UnicodeUTF8));
        checkBox_15->setText(QApplication::translate("ModeSettingDialog", "65", 0, QApplication::UnicodeUTF8));
        checkBox_16->setText(QApplication::translate("ModeSettingDialog", "70", 0, QApplication::UnicodeUTF8));
        ApplaypushButton->setText(QApplication::translate("ModeSettingDialog", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("ModeSettingDialog", "\345\256\236\346\227\266\346\225\260\346\215\256\346\272\220", 0, QApplication::UnicodeUTF8));
        SetPathPushButton->setText(QApplication::translate("ModeSettingDialog", "...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ModeSettingDialog", "\350\267\257\345\276\204\357\274\232", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("ModeSettingDialog", "\345\216\206\345\217\262\346\225\260\346\215\256\346\272\220", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ModeSettingDialog", "\345\274\200\345\247\213\346\227\266\346\254\241\357\274\232", 0, QApplication::UnicodeUTF8));
        dateTimeEdit_2->setDisplayFormat(QApplication::translate("ModeSettingDialog", "yyyy-MM-dd HH:mm:ss", 0, QApplication::UnicodeUTF8));
        StartFilePushButton->setText(QApplication::translate("ModeSettingDialog", "\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ModeSettingDialog", "\346\225\260\346\215\256\350\267\257\345\276\204\357\274\232", 0, QApplication::UnicodeUTF8));
        dateTimeEdit->setDisplayFormat(QApplication::translate("ModeSettingDialog", "yyyy-MM-dd HH:mm:ss", 0, QApplication::UnicodeUTF8));
        EndFilePushButton->setText(QApplication::translate("ModeSettingDialog", "\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ModeSettingDialog", "\347\273\223\346\235\237\346\227\266\346\254\241\357\274\232", 0, QApplication::UnicodeUTF8));
        SetPathPushButton_2->setText(QApplication::translate("ModeSettingDialog", "...", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("ModeSettingDialog", "\350\267\237\350\270\252\347\273\223\346\236\234", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ModeSettingDialog", "\350\276\223\345\207\272\350\267\257\345\276\204\357\274\232", 0, QApplication::UnicodeUTF8));
        SetPathPushButton_3->setText(QApplication::translate("ModeSettingDialog", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ModeSettingDialog: public Ui_ModeSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODESETTINGS_H
