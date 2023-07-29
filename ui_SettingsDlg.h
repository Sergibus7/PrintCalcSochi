/********************************************************************************
** Form generated from reading UI file 'SettingsDlg.ui'
**
** Created by: Qt User Interface Compiler version 6.1.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDLG_H
#define UI_SETTINGSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDlg
{
public:
    QPushButton *btnOK;
    QPushButton *btnCancel;
    QGroupBox *groupBox;
    QSlider *horSlider;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_4;
    QLabel *label_3;
    QGroupBox *groupBox_11;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_61;
    QLineEdit *leManagerName;
    QLabel *label_57;
    QLineEdit *leManagerPhone;
    QLabel *label_58;
    QLineEdit *leManagerEmail;
    QGroupBox *groupBox_2;
    QLineEdit *lePath;
    QPushButton *pbBrowse;

    void setupUi(QDialog *SettingsDlg)
    {
        if (SettingsDlg->objectName().isEmpty())
            SettingsDlg->setObjectName(QString::fromUtf8("SettingsDlg"));
        SettingsDlg->resize(583, 436);
        btnOK = new QPushButton(SettingsDlg);
        btnOK->setObjectName(QString::fromUtf8("btnOK"));
        btnOK->setGeometry(QRect(348, 395, 101, 26));
        btnCancel = new QPushButton(SettingsDlg);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setGeometry(QRect(460, 395, 101, 26));
        groupBox = new QGroupBox(SettingsDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(30, 100, 531, 101));
        groupBox->setAlignment(Qt::AlignCenter);
        horSlider = new QSlider(groupBox);
        horSlider->setObjectName(QString::fromUtf8("horSlider"));
        horSlider->setGeometry(QRect(95, 55, 361, 31));
        horSlider->setMaximum(3);
        horSlider->setPageStep(1);
        horSlider->setOrientation(Qt::Horizontal);
        horSlider->setTickPosition(QSlider::TicksAbove);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(303, 35, 61, 16));
        label_5->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(421, 35, 61, 16));
        label_6->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(187, 35, 61, 16));
        label_4->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(51, 35, 100, 16));
        label_3->setAlignment(Qt::AlignCenter);
        groupBox_11 = new QGroupBox(SettingsDlg);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        groupBox_11->setGeometry(QRect(30, 220, 531, 151));
        groupBox_11->setAlignment(Qt::AlignCenter);
        formLayoutWidget = new QWidget(groupBox_11);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(20, 25, 491, 121));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout->setFormAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout->setVerticalSpacing(15);
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_61 = new QLabel(formLayoutWidget);
        label_61->setObjectName(QString::fromUtf8("label_61"));
        label_61->setMinimumSize(QSize(70, 0));
        label_61->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_61);

        leManagerName = new QLineEdit(formLayoutWidget);
        leManagerName->setObjectName(QString::fromUtf8("leManagerName"));
        leManagerName->setMaxLength(255);

        formLayout->setWidget(0, QFormLayout::FieldRole, leManagerName);

        label_57 = new QLabel(formLayoutWidget);
        label_57->setObjectName(QString::fromUtf8("label_57"));
        label_57->setMinimumSize(QSize(70, 0));
        label_57->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_57);

        leManagerPhone = new QLineEdit(formLayoutWidget);
        leManagerPhone->setObjectName(QString::fromUtf8("leManagerPhone"));
        leManagerPhone->setMaxLength(255);

        formLayout->setWidget(1, QFormLayout::FieldRole, leManagerPhone);

        label_58 = new QLabel(formLayoutWidget);
        label_58->setObjectName(QString::fromUtf8("label_58"));
        label_58->setMinimumSize(QSize(70, 0));
        label_58->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_58);

        leManagerEmail = new QLineEdit(formLayoutWidget);
        leManagerEmail->setObjectName(QString::fromUtf8("leManagerEmail"));
        leManagerEmail->setMaxLength(255);

        formLayout->setWidget(2, QFormLayout::FieldRole, leManagerEmail);

        groupBox_2 = new QGroupBox(SettingsDlg);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(29, 10, 531, 71));
        groupBox_2->setAlignment(Qt::AlignCenter);
        lePath = new QLineEdit(groupBox_2);
        lePath->setObjectName(QString::fromUtf8("lePath"));
        lePath->setGeometry(QRect(10, 30, 481, 22));
        pbBrowse = new QPushButton(groupBox_2);
        pbBrowse->setObjectName(QString::fromUtf8("pbBrowse"));
        pbBrowse->setGeometry(QRect(490, 29, 24, 23));
        QWidget::setTabOrder(leManagerName, leManagerPhone);
        QWidget::setTabOrder(leManagerPhone, leManagerEmail);
        QWidget::setTabOrder(leManagerEmail, lePath);
        QWidget::setTabOrder(lePath, pbBrowse);
        QWidget::setTabOrder(pbBrowse, horSlider);
        QWidget::setTabOrder(horSlider, btnOK);
        QWidget::setTabOrder(btnOK, btnCancel);

        retranslateUi(SettingsDlg);

        btnOK->setDefault(true);


        QMetaObject::connectSlotsByName(SettingsDlg);
    } // setupUi

    void retranslateUi(QDialog *SettingsDlg)
    {
        SettingsDlg->setWindowTitle(QCoreApplication::translate("SettingsDlg", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
        btnOK->setText(QCoreApplication::translate("SettingsDlg", "\320\236\320\232", nullptr));
        btnCancel->setText(QCoreApplication::translate("SettingsDlg", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SettingsDlg", "\320\236\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \320\272\320\276\320\275\320\265\321\207\320\275\320\276\320\271 \321\206\320\265\320\275\321\213", nullptr));
        label_5->setText(QCoreApplication::translate("SettingsDlg", "100", nullptr));
        label_6->setText(QCoreApplication::translate("SettingsDlg", "1000", nullptr));
        label_4->setText(QCoreApplication::translate("SettingsDlg", "10", nullptr));
        label_3->setText(QCoreApplication::translate("SettingsDlg", "\320\221\320\265\320\267 \320\276\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\321\217", nullptr));
        groupBox_11->setTitle(QCoreApplication::translate("SettingsDlg", "\320\230\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\217 \320\276 \320\274\320\265\320\275\320\265\320\264\320\266\320\265\321\200\320\265", nullptr));
        label_61->setText(QCoreApplication::translate("SettingsDlg", "\320\230\320\274\321\217", nullptr));
        label_57->setText(QCoreApplication::translate("SettingsDlg", "\320\242\320\265\320\273\320\265\321\204\320\276\320\275", nullptr));
        label_58->setText(QCoreApplication::translate("SettingsDlg", "E-mail", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("SettingsDlg", "\320\237\321\203\321\202\321\214 \320\272 \321\204\320\260\320\271\320\273\320\260\320\274 CSV", nullptr));
        pbBrowse->setText(QCoreApplication::translate("SettingsDlg", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDlg: public Ui_SettingsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDLG_H
