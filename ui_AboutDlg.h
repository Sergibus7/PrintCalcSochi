/********************************************************************************
** Form generated from reading UI file 'AboutDlg.ui'
**
** Created by: Qt User Interface Compiler version 6.1.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDLG_H
#define UI_ABOUTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_AboutDlg
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_3;

    void setupUi(QDialog *AboutDlg)
    {
        if (AboutDlg->objectName().isEmpty())
            AboutDlg->setObjectName(QString::fromUtf8("AboutDlg"));
        AboutDlg->resize(470, 410);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush1(QColor(0, 120, 215, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        QBrush brush2(QColor(240, 240, 240, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush2);
        QBrush brush3(QColor(120, 120, 120, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush1);
        AboutDlg->setPalette(palette);
        label = new QLabel(AboutDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 30, 401, 21));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        label->setFont(font);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setWordWrap(true);
        label_2 = new QLabel(AboutDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 54, 251, 21));
        label_4 = new QLabel(AboutDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(40, 360, 251, 20));
        label_4->setOpenExternalLinks(true);
        label_5 = new QLabel(AboutDlg);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(30, 100, 411, 211));
        label_5->setPixmap(QPixmap(QString::fromUtf8("ColoritLogo.png")));
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_3 = new QLabel(AboutDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 331, 251, 21));

        retranslateUi(AboutDlg);

        QMetaObject::connectSlotsByName(AboutDlg);
    } // setupUi

    void retranslateUi(QDialog *AboutDlg)
    {
        AboutDlg->setWindowTitle(QCoreApplication::translate("AboutDlg", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
        label->setText(QCoreApplication::translate("AboutDlg", "\320\232\320\260\320\273\321\214\320\272\321\203\320\273\321\217\321\202\320\276\321\200 \320\277\320\276\320\273\320\270\320\263\321\200\320\260\321\204\320\270\321\207\320\265\321\201\320\272\320\276\320\271  \320\277\321\200\320\276\320\264\321\203\320\272\321\206\320\270\320\270", nullptr));
        label_2->setText(QCoreApplication::translate("AboutDlg", "\320\222\320\265\321\200\321\201\320\270\321\217 1.0", nullptr));
        label_4->setText(QCoreApplication::translate("AboutDlg", "<a href=\"http://www.qt.io\">Build on Qt version 6</a>", nullptr));
        label_3->setText(QCoreApplication::translate("AboutDlg", "(c) \320\241\320\276\321\207\320\270 \320\237\321\200\320\265\321\201\321\201, 2023", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDlg: public Ui_AboutDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDLG_H
