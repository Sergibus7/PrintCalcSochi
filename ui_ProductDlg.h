/********************************************************************************
** Form generated from reading UI file 'ProductDlg.ui'
**
** Created by: Qt User Interface Compiler version 6.1.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCTDLG_H
#define UI_PRODUCTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProductDlg
{
public:
    QGroupBox *gbPagesCount;
    QWidget *gridLayoutWidget_7;
    QGridLayout *gridLayout_7;
    QSpinBox *sbPagesBottom;
    QLabel *label_13;
    QLabel *label_2;
    QSpinBox *sbPagesInsert;
    QSpinBox *sbPagesCover;
    QLabel *label_14;
    QSpinBox *sbPagesBlock;
    QLabel *label_12;
    QSpacerItem *horizontalSpacer;
    QLabel *label_4;
    QCheckBox *cbIdenticalPages;
    QCheckBox *cbFlyLeaf;
    QLineEdit *leTotalPages;
    QLabel *label_5;
    QGroupBox *groupBox;
    QRadioButton *rbMultipageProduct;
    QRadioButton *rbSimpleProduct;
    QComboBox *comboSimpleProduct;
    QComboBox *comboMultipageProduct;
    QGroupBox *gbBinding;
    QCheckBox *cbBindingLongSide;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *rbWire;
    QLineEdit *leWireCount;
    QRadioButton *rbNoBinding;
    QRadioButton *rbStaples;
    QRadioButton *rbSoftCover;
    QRadioButton *rbHardCover;
    QRadioButton *rbSoftCoverPUR;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *rbBolts;
    QSpinBox *sbBoltsCount;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *rbRings;
    QSpinBox *sbRingsCount;
    QGroupBox *gbProductSize;
    QRadioButton *rbStandardSize;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayoutStandardFormats;
    QRadioButton *rbA3;
    QRadioButton *rbA5;
    QRadioButton *rbA6;
    QRadioButton *rbA7;
    QRadioButton *rbA2;
    QRadioButton *rb210x98;
    QRadioButton *rbA4;
    QRadioButton *rb90x50;
    QRadioButton *rbCustomSize;
    QSpinBox *sbProductWidth;
    QSpinBox *sbProductHeight;
    QLabel *lblHeight;
    QLabel *lblWidth;
    QPushButton *pbOK;
    QPushButton *pbCancel;
    QPushButton *pbAddImp;
    QPushButton *pbEditImp;
    QPushButton *pbDeleteImp;
    QGroupBox *groupBox_11;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_9;
    QSpinBox *sbCopies;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_10;
    QDoubleSpinBox *sbCoeff;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_11;
    QSpinBox *sbDesignCost;
    QButtonGroup *bgBinding;
    QButtonGroup *bgStdProductSize;
    QButtonGroup *bgProductType;

    void setupUi(QDialog *ProductDlg)
    {
        if (ProductDlg->objectName().isEmpty())
            ProductDlg->setObjectName(QString::fromUtf8("ProductDlg"));
        ProductDlg->resize(749, 636);
        gbPagesCount = new QGroupBox(ProductDlg);
        gbPagesCount->setObjectName(QString::fromUtf8("gbPagesCount"));
        gbPagesCount->setEnabled(false);
        gbPagesCount->setGeometry(QRect(380, 20, 341, 181));
        gbPagesCount->setAlignment(Qt::AlignCenter);
        gridLayoutWidget_7 = new QWidget(gbPagesCount);
        gridLayoutWidget_7->setObjectName(QString::fromUtf8("gridLayoutWidget_7"));
        gridLayoutWidget_7->setGeometry(QRect(17, 25, 301, 71));
        gridLayout_7 = new QGridLayout(gridLayoutWidget_7);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_7->setHorizontalSpacing(7);
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        sbPagesBottom = new QSpinBox(gridLayoutWidget_7);
        sbPagesBottom->setObjectName(QString::fromUtf8("sbPagesBottom"));
        sbPagesBottom->setMaximum(100);
        sbPagesBottom->setSingleStep(2);

        gridLayout_7->addWidget(sbPagesBottom, 0, 4, 1, 1);

        label_13 = new QLabel(gridLayoutWidget_7);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_7->addWidget(label_13, 0, 3, 1, 1);

        label_2 = new QLabel(gridLayoutWidget_7);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_7->addWidget(label_2, 1, 0, 1, 1);

        sbPagesInsert = new QSpinBox(gridLayoutWidget_7);
        sbPagesInsert->setObjectName(QString::fromUtf8("sbPagesInsert"));
        sbPagesInsert->setMaximum(1000);
        sbPagesInsert->setSingleStep(2);

        gridLayout_7->addWidget(sbPagesInsert, 1, 4, 1, 1);

        sbPagesCover = new QSpinBox(gridLayoutWidget_7);
        sbPagesCover->setObjectName(QString::fromUtf8("sbPagesCover"));
        sbPagesCover->setMaximum(4);
        sbPagesCover->setSingleStep(2);

        gridLayout_7->addWidget(sbPagesCover, 0, 1, 1, 1);

        label_14 = new QLabel(gridLayoutWidget_7);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_7->addWidget(label_14, 1, 3, 1, 1);

        sbPagesBlock = new QSpinBox(gridLayoutWidget_7);
        sbPagesBlock->setObjectName(QString::fromUtf8("sbPagesBlock"));
        sbPagesBlock->setMaximum(10000);
        sbPagesBlock->setSingleStep(2);

        gridLayout_7->addWidget(sbPagesBlock, 1, 1, 1, 1);

        label_12 = new QLabel(gridLayoutWidget_7);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_7->addWidget(label_12, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer, 1, 2, 1, 1);

        label_4 = new QLabel(gbPagesCount);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 150, 41, 22));
        label_4->setWordWrap(true);
        cbIdenticalPages = new QCheckBox(gbPagesCount);
        cbIdenticalPages->setObjectName(QString::fromUtf8("cbIdenticalPages"));
        cbIdenticalPages->setGeometry(QRect(20, 98, 291, 20));
        cbFlyLeaf = new QCheckBox(gbPagesCount);
        cbFlyLeaf->setObjectName(QString::fromUtf8("cbFlyLeaf"));
        cbFlyLeaf->setGeometry(QRect(20, 123, 171, 20));
        leTotalPages = new QLineEdit(gbPagesCount);
        leTotalPages->setObjectName(QString::fromUtf8("leTotalPages"));
        leTotalPages->setEnabled(false);
        leTotalPages->setGeometry(QRect(60, 150, 41, 22));
        QFont font;
        font.setBold(true);
        leTotalPages->setFont(font);
        leTotalPages->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leTotalPages->setReadOnly(true);
        label_5 = new QLabel(gbPagesCount);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(106, 150, 51, 22));
        label_5->setWordWrap(true);
        groupBox = new QGroupBox(ProductDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(30, 20, 311, 141));
        groupBox->setStyleSheet(QString::fromUtf8(""));
        groupBox->setAlignment(Qt::AlignCenter);
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        rbMultipageProduct = new QRadioButton(groupBox);
        bgProductType = new QButtonGroup(ProductDlg);
        bgProductType->setObjectName(QString::fromUtf8("bgProductType"));
        bgProductType->addButton(rbMultipageProduct);
        rbMultipageProduct->setObjectName(QString::fromUtf8("rbMultipageProduct"));
        rbMultipageProduct->setGeometry(QRect(20, 80, 201, 17));
        rbSimpleProduct = new QRadioButton(groupBox);
        bgProductType->addButton(rbSimpleProduct);
        rbSimpleProduct->setObjectName(QString::fromUtf8("rbSimpleProduct"));
        rbSimpleProduct->setGeometry(QRect(20, 22, 201, 17));
        rbSimpleProduct->setChecked(true);
        rbSimpleProduct->setAutoExclusive(true);
        comboSimpleProduct = new QComboBox(groupBox);
        comboSimpleProduct->setObjectName(QString::fromUtf8("comboSimpleProduct"));
        comboSimpleProduct->setGeometry(QRect(40, 47, 231, 20));
        comboSimpleProduct->setMaxVisibleItems(50);
        comboSimpleProduct->setInsertPolicy(QComboBox::InsertAlphabetically);
        comboMultipageProduct = new QComboBox(groupBox);
        comboMultipageProduct->setObjectName(QString::fromUtf8("comboMultipageProduct"));
        comboMultipageProduct->setEnabled(false);
        comboMultipageProduct->setGeometry(QRect(40, 103, 231, 20));
        comboMultipageProduct->setMaxVisibleItems(50);
        comboMultipageProduct->setInsertPolicy(QComboBox::InsertAlphabetically);
        gbBinding = new QGroupBox(ProductDlg);
        gbBinding->setObjectName(QString::fromUtf8("gbBinding"));
        gbBinding->setEnabled(false);
        gbBinding->setGeometry(QRect(380, 220, 341, 231));
        gbBinding->setAlignment(Qt::AlignCenter);
        cbBindingLongSide = new QCheckBox(gbBinding);
        cbBindingLongSide->setObjectName(QString::fromUtf8("cbBindingLongSide"));
        cbBindingLongSide->setGeometry(QRect(10, 180, 251, 21));
        cbBindingLongSide->setFont(font);
        cbBindingLongSide->setChecked(true);
        gridLayoutWidget = new QWidget(gbBinding);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 30, 321, 138));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(16);
        gridLayout->setVerticalSpacing(12);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(10);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        rbWire = new QRadioButton(gridLayoutWidget);
        bgBinding = new QButtonGroup(ProductDlg);
        bgBinding->setObjectName(QString::fromUtf8("bgBinding"));
        bgBinding->addButton(rbWire);
        rbWire->setObjectName(QString::fromUtf8("rbWire"));
        QFont font1;
        font1.setPointSize(9);
        rbWire->setFont(font1);

        horizontalLayout_4->addWidget(rbWire);

        leWireCount = new QLineEdit(gridLayoutWidget);
        leWireCount->setObjectName(QString::fromUtf8("leWireCount"));
        leWireCount->setEnabled(false);
        QFont font2;
        font2.setPointSize(11);
        font2.setBold(true);
        leWireCount->setFont(font2);
        leWireCount->setReadOnly(true);

        horizontalLayout_4->addWidget(leWireCount);


        gridLayout->addLayout(horizontalLayout_4, 2, 1, 1, 1);

        rbNoBinding = new QRadioButton(gridLayoutWidget);
        bgBinding->addButton(rbNoBinding);
        rbNoBinding->setObjectName(QString::fromUtf8("rbNoBinding"));
        rbNoBinding->setFont(font1);

        gridLayout->addWidget(rbNoBinding, 0, 0, 1, 1);

        rbStaples = new QRadioButton(gridLayoutWidget);
        bgBinding->addButton(rbStaples);
        rbStaples->setObjectName(QString::fromUtf8("rbStaples"));
        rbStaples->setFont(font1);
        rbStaples->setChecked(true);

        gridLayout->addWidget(rbStaples, 0, 1, 1, 1);

        rbSoftCover = new QRadioButton(gridLayoutWidget);
        bgBinding->addButton(rbSoftCover);
        rbSoftCover->setObjectName(QString::fromUtf8("rbSoftCover"));
        rbSoftCover->setFont(font1);

        gridLayout->addWidget(rbSoftCover, 1, 0, 1, 1);

        rbHardCover = new QRadioButton(gridLayoutWidget);
        rbHardCover->setObjectName(QString::fromUtf8("rbHardCover"));
        rbHardCover->setFont(font1);

        gridLayout->addWidget(rbHardCover, 2, 0, 1, 1);

        rbSoftCoverPUR = new QRadioButton(gridLayoutWidget);
        rbSoftCoverPUR->setObjectName(QString::fromUtf8("rbSoftCoverPUR"));
        rbSoftCoverPUR->setFont(font1);

        gridLayout->addWidget(rbSoftCoverPUR, 1, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        rbBolts = new QRadioButton(gridLayoutWidget);
        bgBinding->addButton(rbBolts);
        rbBolts->setObjectName(QString::fromUtf8("rbBolts"));
        rbBolts->setFont(font1);

        horizontalLayout_2->addWidget(rbBolts);

        sbBoltsCount = new QSpinBox(gridLayoutWidget);
        sbBoltsCount->setObjectName(QString::fromUtf8("sbBoltsCount"));
        sbBoltsCount->setEnabled(false);
        sbBoltsCount->setMinimum(1);

        horizontalLayout_2->addWidget(sbBoltsCount);


        gridLayout->addLayout(horizontalLayout_2, 3, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(10);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        rbRings = new QRadioButton(gridLayoutWidget);
        bgBinding->addButton(rbRings);
        rbRings->setObjectName(QString::fromUtf8("rbRings"));
        rbRings->setFont(font1);

        horizontalLayout_3->addWidget(rbRings);

        sbRingsCount = new QSpinBox(gridLayoutWidget);
        sbRingsCount->setObjectName(QString::fromUtf8("sbRingsCount"));
        sbRingsCount->setEnabled(false);
        sbRingsCount->setMinimum(1);

        horizontalLayout_3->addWidget(sbRingsCount);


        gridLayout->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        gridLayout->setColumnStretch(0, 50);
        gridLayout->setColumnStretch(1, 50);
        gbProductSize = new QGroupBox(ProductDlg);
        gbProductSize->setObjectName(QString::fromUtf8("gbProductSize"));
        gbProductSize->setGeometry(QRect(30, 180, 311, 271));
        gbProductSize->setStyleSheet(QString::fromUtf8("QGroupBox	{ pane: #f0f0f0}"));
        gbProductSize->setAlignment(Qt::AlignCenter);
        rbStandardSize = new QRadioButton(gbProductSize);
        rbStandardSize->setObjectName(QString::fromUtf8("rbStandardSize"));
        rbStandardSize->setGeometry(QRect(17, 20, 151, 21));
        rbStandardSize->setChecked(true);
        gridLayoutWidget_3 = new QWidget(gbProductSize);
        gridLayoutWidget_3->setObjectName(QString::fromUtf8("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(47, 41, 261, 121));
        gridLayoutStandardFormats = new QGridLayout(gridLayoutWidget_3);
        gridLayoutStandardFormats->setObjectName(QString::fromUtf8("gridLayoutStandardFormats"));
        gridLayoutStandardFormats->setContentsMargins(0, 0, 0, 0);
        rbA3 = new QRadioButton(gridLayoutWidget_3);
        bgStdProductSize = new QButtonGroup(ProductDlg);
        bgStdProductSize->setObjectName(QString::fromUtf8("bgStdProductSize"));
        bgStdProductSize->addButton(rbA3);
        rbA3->setObjectName(QString::fromUtf8("rbA3"));
        QFont font3;
        font3.setPointSize(11);
        rbA3->setFont(font3);

        gridLayoutStandardFormats->addWidget(rbA3, 1, 1, 1, 1);

        rbA5 = new QRadioButton(gridLayoutWidget_3);
        bgStdProductSize->addButton(rbA5);
        rbA5->setObjectName(QString::fromUtf8("rbA5"));
        rbA5->setFont(font3);

        gridLayoutStandardFormats->addWidget(rbA5, 0, 2, 1, 1);

        rbA6 = new QRadioButton(gridLayoutWidget_3);
        bgStdProductSize->addButton(rbA6);
        rbA6->setObjectName(QString::fromUtf8("rbA6"));
        rbA6->setFont(font3);

        gridLayoutStandardFormats->addWidget(rbA6, 0, 1, 1, 1);

        rbA7 = new QRadioButton(gridLayoutWidget_3);
        bgStdProductSize->addButton(rbA7);
        rbA7->setObjectName(QString::fromUtf8("rbA7"));
        rbA7->setFont(font3);

        gridLayoutStandardFormats->addWidget(rbA7, 0, 0, 1, 1);

        rbA2 = new QRadioButton(gridLayoutWidget_3);
        bgStdProductSize->addButton(rbA2);
        rbA2->setObjectName(QString::fromUtf8("rbA2"));
        rbA2->setFont(font3);

        gridLayoutStandardFormats->addWidget(rbA2, 1, 2, 1, 1);

        rb210x98 = new QRadioButton(gridLayoutWidget_3);
        bgStdProductSize->addButton(rb210x98);
        rb210x98->setObjectName(QString::fromUtf8("rb210x98"));
        rb210x98->setFont(font3);

        gridLayoutStandardFormats->addWidget(rb210x98, 2, 0, 1, 1);

        rbA4 = new QRadioButton(gridLayoutWidget_3);
        bgStdProductSize->addButton(rbA4);
        rbA4->setObjectName(QString::fromUtf8("rbA4"));
        rbA4->setFont(font3);
        rbA4->setChecked(true);

        gridLayoutStandardFormats->addWidget(rbA4, 1, 0, 1, 1);

        rb90x50 = new QRadioButton(gridLayoutWidget_3);
        bgStdProductSize->addButton(rb90x50);
        rb90x50->setObjectName(QString::fromUtf8("rb90x50"));
        rb90x50->setFont(font3);

        gridLayoutStandardFormats->addWidget(rb90x50, 2, 1, 1, 1);

        rbCustomSize = new QRadioButton(gbProductSize);
        rbCustomSize->setObjectName(QString::fromUtf8("rbCustomSize"));
        rbCustomSize->setEnabled(true);
        rbCustomSize->setGeometry(QRect(17, 170, 161, 21));
        sbProductWidth = new QSpinBox(gbProductSize);
        sbProductWidth->setObjectName(QString::fromUtf8("sbProductWidth"));
        sbProductWidth->setEnabled(false);
        sbProductWidth->setGeometry(QRect(78, 200, 81, 24));
        QFont font4;
        font4.setPointSize(10);
        font4.setBold(true);
        sbProductWidth->setFont(font4);
        sbProductWidth->setMinimum(10);
        sbProductWidth->setMaximum(1000);
        sbProductHeight = new QSpinBox(gbProductSize);
        sbProductHeight->setObjectName(QString::fromUtf8("sbProductHeight"));
        sbProductHeight->setEnabled(false);
        sbProductHeight->setGeometry(QRect(217, 200, 81, 24));
        sbProductHeight->setFont(font4);
        sbProductHeight->setMinimum(10);
        sbProductHeight->setMaximum(1000);
        sbProductHeight->setSingleStep(1);
        lblHeight = new QLabel(gbProductSize);
        lblHeight->setObjectName(QString::fromUtf8("lblHeight"));
        lblHeight->setEnabled(false);
        lblHeight->setGeometry(QRect(180, 202, 31, 21));
        lblHeight->setMouseTracking(false);
        lblHeight->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblWidth = new QLabel(gbProductSize);
        lblWidth->setObjectName(QString::fromUtf8("lblWidth"));
        lblWidth->setEnabled(false);
        lblWidth->setGeometry(QRect(30, 202, 41, 21));
        lblWidth->setMouseTracking(false);
        lblWidth->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pbOK = new QPushButton(ProductDlg);
        pbOK->setObjectName(QString::fromUtf8("pbOK"));
        pbOK->setGeometry(QRect(480, 585, 121, 31));
        pbCancel = new QPushButton(ProductDlg);
        pbCancel->setObjectName(QString::fromUtf8("pbCancel"));
        pbCancel->setGeometry(QRect(610, 585, 111, 31));
        pbAddImp = new QPushButton(ProductDlg);
        pbAddImp->setObjectName(QString::fromUtf8("pbAddImp"));
        pbAddImp->setGeometry(QRect(30, 585, 130, 31));
        pbAddImp->setFont(font);
        pbEditImp = new QPushButton(ProductDlg);
        pbEditImp->setObjectName(QString::fromUtf8("pbEditImp"));
        pbEditImp->setEnabled(false);
        pbEditImp->setGeometry(QRect(170, 585, 130, 31));
        pbEditImp->setFont(font);
        pbDeleteImp = new QPushButton(ProductDlg);
        pbDeleteImp->setObjectName(QString::fromUtf8("pbDeleteImp"));
        pbDeleteImp->setEnabled(false);
        pbDeleteImp->setGeometry(QRect(310, 585, 130, 31));
        pbDeleteImp->setFont(font);
        groupBox_11 = new QGroupBox(ProductDlg);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        groupBox_11->setGeometry(QRect(30, 480, 691, 66));
        groupBox_11->setAlignment(Qt::AlignCenter);
        horizontalLayoutWidget = new QWidget(groupBox_11);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(31, 20, 631, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(horizontalLayoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font);
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_9);

        sbCopies = new QSpinBox(horizontalLayoutWidget);
        sbCopies->setObjectName(QString::fromUtf8("sbCopies"));
        sbCopies->setFont(font);
        sbCopies->setAccelerated(true);
        sbCopies->setMinimum(1);
        sbCopies->setMaximum(999999999);
        sbCopies->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        sbCopies->setValue(1);

        horizontalLayout->addWidget(sbCopies);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label_10 = new QLabel(horizontalLayoutWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_10);

        sbCoeff = new QDoubleSpinBox(horizontalLayoutWidget);
        sbCoeff->setObjectName(QString::fromUtf8("sbCoeff"));
        sbCoeff->setMinimumSize(QSize(60, 0));
        sbCoeff->setAccelerated(true);

        horizontalLayout->addWidget(sbCoeff);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        label_11 = new QLabel(horizontalLayoutWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_11);

        sbDesignCost = new QSpinBox(horizontalLayoutWidget);
        sbDesignCost->setObjectName(QString::fromUtf8("sbDesignCost"));
        sbDesignCost->setMinimumSize(QSize(95, 0));
        sbDesignCost->setAccelerated(true);
        sbDesignCost->setMaximum(10000000);
        sbDesignCost->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

        horizontalLayout->addWidget(sbDesignCost);

#if QT_CONFIG(shortcut)
        label_13->setBuddy(sbPagesBottom);
        label_2->setBuddy(sbPagesBlock);
        label_14->setBuddy(sbPagesInsert);
        label_12->setBuddy(sbPagesCover);
        lblHeight->setBuddy(sbProductHeight);
        lblWidth->setBuddy(sbProductWidth);
        label_9->setBuddy(sbCopies);
        label_10->setBuddy(sbCopies);
        label_11->setBuddy(sbCopies);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(rbSimpleProduct, comboSimpleProduct);
        QWidget::setTabOrder(comboSimpleProduct, rbMultipageProduct);
        QWidget::setTabOrder(rbMultipageProduct, comboMultipageProduct);
        QWidget::setTabOrder(comboMultipageProduct, rbStandardSize);
        QWidget::setTabOrder(rbStandardSize, rbA7);
        QWidget::setTabOrder(rbA7, rbA6);
        QWidget::setTabOrder(rbA6, rbA5);
        QWidget::setTabOrder(rbA5, rbA4);
        QWidget::setTabOrder(rbA4, rbA3);
        QWidget::setTabOrder(rbA3, rbA2);
        QWidget::setTabOrder(rbA2, rb210x98);
        QWidget::setTabOrder(rb210x98, rb90x50);
        QWidget::setTabOrder(rb90x50, rbCustomSize);
        QWidget::setTabOrder(rbCustomSize, sbProductWidth);
        QWidget::setTabOrder(sbProductWidth, sbProductHeight);
        QWidget::setTabOrder(sbProductHeight, sbPagesCover);
        QWidget::setTabOrder(sbPagesCover, sbPagesBottom);
        QWidget::setTabOrder(sbPagesBottom, sbPagesBlock);
        QWidget::setTabOrder(sbPagesBlock, sbPagesInsert);
        QWidget::setTabOrder(sbPagesInsert, cbIdenticalPages);
        QWidget::setTabOrder(cbIdenticalPages, cbBindingLongSide);
        QWidget::setTabOrder(cbBindingLongSide, pbAddImp);
        QWidget::setTabOrder(pbAddImp, pbEditImp);
        QWidget::setTabOrder(pbEditImp, pbDeleteImp);
        QWidget::setTabOrder(pbDeleteImp, pbOK);
        QWidget::setTabOrder(pbOK, pbCancel);

        retranslateUi(ProductDlg);

        pbOK->setDefault(true);


        QMetaObject::connectSlotsByName(ProductDlg);
    } // setupUi

    void retranslateUi(QDialog *ProductDlg)
    {
        ProductDlg->setWindowTitle(QCoreApplication::translate("ProductDlg", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213 \320\270\320\267\320\264\320\265\320\273\320\270\321\217", nullptr));
        gbPagesCount->setTitle(QCoreApplication::translate("ProductDlg", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \321\201\321\202\321\200\320\260\320\275\320\270\321\206", nullptr));
        sbPagesBottom->setSuffix(QCoreApplication::translate("ProductDlg", " \321\201\321\202\321\200.", nullptr));
        label_13->setText(QCoreApplication::translate("ProductDlg", "\320\237\320\276\320\264\320\273\320\276\320\266\320\272\320\260", nullptr));
        label_2->setText(QCoreApplication::translate("ProductDlg", "\320\221\320\273\320\276\320\272", nullptr));
        sbPagesInsert->setSuffix(QCoreApplication::translate("ProductDlg", " \321\201\321\202\321\200.", nullptr));
        sbPagesCover->setSuffix(QCoreApplication::translate("ProductDlg", " \321\201\321\202\321\200.", nullptr));
        label_14->setText(QCoreApplication::translate("ProductDlg", "\320\222\321\201\321\202\320\260\320\262\320\272\320\260", nullptr));
        sbPagesBlock->setSuffix(QCoreApplication::translate("ProductDlg", " \321\201\321\202\321\200.", nullptr));
        label_12->setText(QCoreApplication::translate("ProductDlg", "\320\236\320\261\320\273\320\276\320\266\320\272\320\260", nullptr));
        label_4->setText(QCoreApplication::translate("ProductDlg", "\320\222\321\201\320\265\320\263\320\276", nullptr));
        cbIdenticalPages->setText(QCoreApplication::translate("ProductDlg", "\320\236\320\264\320\270\320\275\320\260\320\272\320\276\320\262\321\213\320\265 \321\201\321\202\321\200\320\260\320\275\320\270\321\206\321\213 \320\261\320\273\320\276\320\272\320\260 (\320\264\320\273\321\217 \320\261\320\273\320\276\320\272\320\275\320\276\321\202\320\260)", nullptr));
        cbFlyLeaf->setText(QCoreApplication::translate("ProductDlg", "\320\236\320\264\320\270\320\275\320\260\320\272\320\276\320\262\321\213\320\265 \321\204\320\276\321\200\320\267\320\260\321\206\321\213", nullptr));
        leTotalPages->setText(QCoreApplication::translate("ProductDlg", "9999", nullptr));
        label_5->setText(QCoreApplication::translate("ProductDlg", "\320\277\320\276\320\273\320\276\321\201", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ProductDlg", "\320\242\320\270\320\277 \320\270\320\267\320\264\320\265\320\273\320\270\321\217", nullptr));
        rbMultipageProduct->setText(QCoreApplication::translate("ProductDlg", "\320\234\320\275\320\276\320\263\320\276\321\201\321\202\321\200\320\260\320\275\320\270\321\207\320\275\320\276\320\265 \320\270\320\267\320\264\320\265\320\273\320\270\320\265", nullptr));
        rbSimpleProduct->setText(QCoreApplication::translate("ProductDlg", "\320\237\321\200\320\276\321\201\321\202\320\276\320\265 \320\270\320\267\320\264\320\265\320\273\320\270\320\265", nullptr));
        gbBinding->setTitle(QCoreApplication::translate("ProductDlg", "\320\237\320\265\321\200\320\265\320\277\320\273\320\265\321\202 \320\270 \321\201\320\261\320\276\321\200\320\272\320\260", nullptr));
        cbBindingLongSide->setText(QCoreApplication::translate("ProductDlg", "\320\237\320\265\321\200\320\265\320\277\320\273\320\265\321\202 \320\262\320\264\320\276\320\273\321\214 \320\264\320\273\320\270\320\275\320\275\320\276\320\271 \321\201\321\202\320\276\321\200\320\276\320\275\321\213", nullptr));
        rbWire->setText(QCoreApplication::translate("ProductDlg", "\320\277\321\200\321\203\320\266\320\270\320\275\320\260:", nullptr));
        rbNoBinding->setText(QCoreApplication::translate("ProductDlg", "\320\261\320\265\320\267 \320\277\320\265\321\200\320\265\320\277\320\273\320\265\321\202\320\260", nullptr));
        rbStaples->setText(QCoreApplication::translate("ProductDlg", "\321\201\320\272\320\276\320\261\321\213", nullptr));
        rbSoftCover->setText(QCoreApplication::translate("ProductDlg", "\321\202\320\265\321\200\320\274\320\276\320\272\320\273\320\265\320\271", nullptr));
        rbHardCover->setText(QCoreApplication::translate("ProductDlg", "\321\202\320\262\320\265\321\200\320\264\321\213\320\271 \320\277\320\265\321\200\320\265\320\277\320\273\320\265\321\202", nullptr));
        rbSoftCoverPUR->setText(QCoreApplication::translate("ProductDlg", "\321\202\320\265\321\200\320\274\320\276\320\272\320\273\320\265\320\271 PUR", nullptr));
        rbBolts->setText(QCoreApplication::translate("ProductDlg", "\320\261\320\276\320\273\321\202\321\213:", nullptr));
        rbRings->setText(QCoreApplication::translate("ProductDlg", "\320\272\320\276\320\273\321\214\321\206\320\260:", nullptr));
        gbProductSize->setTitle(QCoreApplication::translate("ProductDlg", "\320\240\320\260\320\267\320\274\320\265\321\200 \320\270\320\267\320\264\320\265\320\273\320\270\321\217", nullptr));
        rbStandardSize->setText(QCoreApplication::translate("ProductDlg", "\320\241\321\202\320\260\320\275\320\264\320\260\321\200\321\202\320\275\321\213\320\271", nullptr));
        rbA3->setText(QCoreApplication::translate("ProductDlg", "\320\2203", nullptr));
        rbA5->setText(QCoreApplication::translate("ProductDlg", "\320\2205", nullptr));
        rbA6->setText(QCoreApplication::translate("ProductDlg", "\320\2206", nullptr));
        rbA7->setText(QCoreApplication::translate("ProductDlg", "\320\2207", nullptr));
        rbA2->setText(QCoreApplication::translate("ProductDlg", "\320\2202", nullptr));
        rb210x98->setText(QCoreApplication::translate("ProductDlg", "210x98", nullptr));
        rbA4->setText(QCoreApplication::translate("ProductDlg", "\320\2204", nullptr));
        rb90x50->setText(QCoreApplication::translate("ProductDlg", "90x50", nullptr));
        rbCustomSize->setText(QCoreApplication::translate("ProductDlg", "\320\237\321\200\320\276\320\270\320\267\320\262\320\276\320\273\321\214\320\275\321\213\320\271", nullptr));
        sbProductWidth->setSuffix(QCoreApplication::translate("ProductDlg", " \320\274\320\274", nullptr));
        sbProductHeight->setSuffix(QCoreApplication::translate("ProductDlg", " \320\274\320\274", nullptr));
        lblHeight->setText(QCoreApplication::translate("ProductDlg", "\320\262\321\213\321\201.", nullptr));
        lblWidth->setText(QCoreApplication::translate("ProductDlg", "\321\210\320\270\321\200.", nullptr));
        pbOK->setText(QCoreApplication::translate("ProductDlg", "\320\236\320\232", nullptr));
        pbCancel->setText(QCoreApplication::translate("ProductDlg", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        pbAddImp->setText(QCoreApplication::translate("ProductDlg", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\201\320\277\321\203\321\201\320\272...", nullptr));
        pbEditImp->setText(QCoreApplication::translate("ProductDlg", "\320\230\320\267\320\274\320\265\320\275\320\270\321\202\321\214 \321\201\320\277\321\203\321\201\320\272...", nullptr));
        pbDeleteImp->setText(QCoreApplication::translate("ProductDlg", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \321\201\320\277\321\203\321\201\320\272...", nullptr));
        groupBox_11->setTitle(QCoreApplication::translate("ProductDlg", "\320\242\320\270\321\200\320\260\320\266 \320\270 \320\277\321\200\320\276\321\207\320\265\320\265", nullptr));
        label_9->setText(QCoreApplication::translate("ProductDlg", "\320\242\320\270\321\200\320\260\320\266", nullptr));
        sbCopies->setSuffix(QCoreApplication::translate("ProductDlg", " \321\215\320\272\320\267.", nullptr));
        label_10->setText(QCoreApplication::translate("ProductDlg", "\320\232\320\276\321\215\321\204\321\204\320\270\321\206\320\270\320\265\320\275\321\202", nullptr));
        label_11->setText(QCoreApplication::translate("ProductDlg", "\320\224\320\270\320\267\320\260\320\271\320\275 \320\270 \320\262\320\265\321\200\321\201\321\202\320\272\320\260", nullptr));
        sbDesignCost->setSuffix(QCoreApplication::translate("ProductDlg", " \321\200.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProductDlg: public Ui_ProductDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCTDLG_H
