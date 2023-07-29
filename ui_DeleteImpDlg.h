/********************************************************************************
** Form generated from reading UI file 'DeleteImpDlg.ui'
**
** Created by: Qt User Interface Compiler version 6.1.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DELETEIMPDLG_H
#define UI_DELETEIMPDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeleteImpDlg
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QCheckBox *cb1;
    QCheckBox *cb2;
    QCheckBox *cb3;
    QCheckBox *cb4;
    QButtonGroup *bgImps;

    void setupUi(QDialog *DeleteImpDlg)
    {
        if (DeleteImpDlg->objectName().isEmpty())
            DeleteImpDlg->setObjectName(QString::fromUtf8("DeleteImpDlg"));
        DeleteImpDlg->resize(291, 288);
        DeleteImpDlg->setModal(true);
        buttonBox = new QDialogButtonBox(DeleteImpDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(20, 240, 251, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        groupBox = new QGroupBox(DeleteImpDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(40, 30, 210, 191));
        groupBox->setAlignment(Qt::AlignCenter);
        verticalLayoutWidget = new QWidget(groupBox);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(40, 20, 121, 161));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        cb1 = new QCheckBox(verticalLayoutWidget);
        bgImps = new QButtonGroup(DeleteImpDlg);
        bgImps->setObjectName(QString::fromUtf8("bgImps"));
        bgImps->setExclusive(false);
        bgImps->addButton(cb1);
        cb1->setObjectName(QString::fromUtf8("cb1"));

        verticalLayout->addWidget(cb1);

        cb2 = new QCheckBox(verticalLayoutWidget);
        bgImps->addButton(cb2);
        cb2->setObjectName(QString::fromUtf8("cb2"));

        verticalLayout->addWidget(cb2);

        cb3 = new QCheckBox(verticalLayoutWidget);
        bgImps->addButton(cb3);
        cb3->setObjectName(QString::fromUtf8("cb3"));

        verticalLayout->addWidget(cb3);

        cb4 = new QCheckBox(verticalLayoutWidget);
        bgImps->addButton(cb4);
        cb4->setObjectName(QString::fromUtf8("cb4"));

        verticalLayout->addWidget(cb4);


        retranslateUi(DeleteImpDlg);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, DeleteImpDlg, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, DeleteImpDlg, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(DeleteImpDlg);
    } // setupUi

    void retranslateUi(QDialog *DeleteImpDlg)
    {
        DeleteImpDlg->setWindowTitle(QCoreApplication::translate("DeleteImpDlg", "\320\243\320\264\320\260\320\273\320\265\320\275\320\270\320\265 \321\201\320\277\321\203\321\201\320\272\320\276\320\262", nullptr));
        groupBox->setTitle(QCoreApplication::translate("DeleteImpDlg", "\320\236\321\202\320\274\320\265\321\202\321\214\321\202\320\265 \321\201\320\277\321\203\321\201\320\272(\320\270) \320\264\320\273\321\217 \321\203\320\264\320\260\320\273\320\265\320\275\320\270\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DeleteImpDlg: public Ui_DeleteImpDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DELETEIMPDLG_H
