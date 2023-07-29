/********************************************************************************
** Form generated from reading UI file 'SelectImpDlg.ui'
**
** Created by: Qt User Interface Compiler version 6.1.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTIMPDLG_H
#define UI_SELECTIMPDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectImpDlg
{
public:
    QGroupBox *groupBox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QCommandLinkButton *clbCover;
    QCommandLinkButton *clbBlock;
    QCommandLinkButton *clbBottom;
    QCommandLinkButton *clbInsert;

    void setupUi(QDialog *SelectImpDlg)
    {
        if (SelectImpDlg->objectName().isEmpty())
            SelectImpDlg->setObjectName(QString::fromUtf8("SelectImpDlg"));
        SelectImpDlg->resize(326, 347);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SelectImpDlg->sizePolicy().hasHeightForWidth());
        SelectImpDlg->setSizePolicy(sizePolicy);
        groupBox = new QGroupBox(SelectImpDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(28, 20, 270, 301));
        groupBox->setAlignment(Qt::AlignCenter);
        verticalLayoutWidget = new QWidget(groupBox);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(30, 30, 211, 261));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        clbCover = new QCommandLinkButton(verticalLayoutWidget);
        clbCover->setObjectName(QString::fromUtf8("clbCover"));

        verticalLayout->addWidget(clbCover);

        clbBlock = new QCommandLinkButton(verticalLayoutWidget);
        clbBlock->setObjectName(QString::fromUtf8("clbBlock"));

        verticalLayout->addWidget(clbBlock);

        clbBottom = new QCommandLinkButton(verticalLayoutWidget);
        clbBottom->setObjectName(QString::fromUtf8("clbBottom"));

        verticalLayout->addWidget(clbBottom);

        clbInsert = new QCommandLinkButton(verticalLayoutWidget);
        clbInsert->setObjectName(QString::fromUtf8("clbInsert"));

        verticalLayout->addWidget(clbInsert);


        retranslateUi(SelectImpDlg);

        QMetaObject::connectSlotsByName(SelectImpDlg);
    } // setupUi

    void retranslateUi(QDialog *SelectImpDlg)
    {
        SelectImpDlg->setWindowTitle(QCoreApplication::translate("SelectImpDlg", "\320\222\321\213\320\261\320\276\321\200 \321\201\320\277\321\203\321\201\320\272\320\260", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SelectImpDlg", "\320\242\320\270\320\277 \321\201\320\277\321\203\321\201\320\272\320\260", nullptr));
        clbCover->setText(QCoreApplication::translate("SelectImpDlg", "\320\236\320\261\320\273\320\276\320\266\320\272\320\260", nullptr));
#if QT_CONFIG(shortcut)
        clbCover->setShortcut(QCoreApplication::translate("SelectImpDlg", "1", nullptr));
#endif // QT_CONFIG(shortcut)
        clbCover->setDescription(QCoreApplication::translate("SelectImpDlg", "\320\224\320\273\321\217 \320\262\321\213\320\261\320\276\321\200\320\260 \320\275\320\260\320\266\320\274\320\270\321\202\320\265 \302\2531\302\273", nullptr));
        clbBlock->setText(QCoreApplication::translate("SelectImpDlg", "\320\221\320\273\320\276\320\272", nullptr));
#if QT_CONFIG(shortcut)
        clbBlock->setShortcut(QCoreApplication::translate("SelectImpDlg", "2", nullptr));
#endif // QT_CONFIG(shortcut)
        clbBlock->setDescription(QCoreApplication::translate("SelectImpDlg", "\320\224\320\273\321\217 \320\262\321\213\320\261\320\276\321\200\320\260 \320\275\320\260\320\266\320\274\320\270\321\202\320\265 \302\2532\302\273", nullptr));
        clbBottom->setText(QCoreApplication::translate("SelectImpDlg", "\320\237\320\276\320\264\320\273\320\276\320\266\320\272\320\260", nullptr));
#if QT_CONFIG(shortcut)
        clbBottom->setShortcut(QCoreApplication::translate("SelectImpDlg", "3", nullptr));
#endif // QT_CONFIG(shortcut)
        clbBottom->setDescription(QCoreApplication::translate("SelectImpDlg", "\320\224\320\273\321\217 \320\262\321\213\320\261\320\276\321\200\320\260 \320\275\320\260\320\266\320\274\320\270\321\202\320\265 \302\2533\302\273", nullptr));
        clbInsert->setText(QCoreApplication::translate("SelectImpDlg", "\320\222\321\201\321\202\320\260\320\262\320\272\320\260", nullptr));
#if QT_CONFIG(shortcut)
        clbInsert->setShortcut(QCoreApplication::translate("SelectImpDlg", "4", nullptr));
#endif // QT_CONFIG(shortcut)
        clbInsert->setDescription(QCoreApplication::translate("SelectImpDlg", "\320\224\320\273\321\217 \320\262\321\213\320\261\320\276\321\200\320\260 \320\275\320\260\320\266\320\274\320\270\321\202\320\265 \302\2534\302\273", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectImpDlg: public Ui_SelectImpDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTIMPDLG_H
