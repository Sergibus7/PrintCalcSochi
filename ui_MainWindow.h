/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.1.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionFileOpen;
    QAction *actionProductAdd;
    QAction *actionProductDelete;
    QAction *actionProductEdit;
    QAction *actionEditImposition;
    QAction *actionFileSave;
    QAction *actionExit;
    QAction *actionFileNew;
    QAction *actionOrderPrint;
    QAction *actionAbout;
    QAction *actionHelp;
    QAction *actionSettings;
    QAction *actionEditOrder;
    QAction *actionProductDuplicate;
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *ProductsTree;
    QTreeWidget *PropertiesTree;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuProduct;
    QMenu *menu;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(558, 518);
        actionFileOpen = new QAction(MainWindow);
        actionFileOpen->setObjectName(QString::fromUtf8("actionFileOpen"));
        actionProductAdd = new QAction(MainWindow);
        actionProductAdd->setObjectName(QString::fromUtf8("actionProductAdd"));
        actionProductDelete = new QAction(MainWindow);
        actionProductDelete->setObjectName(QString::fromUtf8("actionProductDelete"));
        actionProductDelete->setEnabled(false);
        actionProductEdit = new QAction(MainWindow);
        actionProductEdit->setObjectName(QString::fromUtf8("actionProductEdit"));
        actionProductEdit->setEnabled(false);
        actionEditImposition = new QAction(MainWindow);
        actionEditImposition->setObjectName(QString::fromUtf8("actionEditImposition"));
        actionEditImposition->setEnabled(false);
        actionEditImposition->setShortcutContext(Qt::ApplicationShortcut);
        actionFileSave = new QAction(MainWindow);
        actionFileSave->setObjectName(QString::fromUtf8("actionFileSave"));
        actionFileSave->setEnabled(false);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionExit->setMenuRole(QAction::QuitRole);
        actionFileNew = new QAction(MainWindow);
        actionFileNew->setObjectName(QString::fromUtf8("actionFileNew"));
        actionOrderPrint = new QAction(MainWindow);
        actionOrderPrint->setObjectName(QString::fromUtf8("actionOrderPrint"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout->setMenuRole(QAction::AboutRole);
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionEditOrder = new QAction(MainWindow);
        actionEditOrder->setObjectName(QString::fromUtf8("actionEditOrder"));
        actionProductDuplicate = new QAction(MainWindow);
        actionProductDuplicate->setObjectName(QString::fromUtf8("actionProductDuplicate"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(60, 20, 467, 221));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(15);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        ProductsTree = new QTreeWidget(centralwidget);
        ProductsTree->setObjectName(QString::fromUtf8("ProductsTree"));
        ProductsTree->setGeometry(QRect(50, 240, 150, 219));
        ProductsTree->setMinimumSize(QSize(150, 100));
        ProductsTree->setContextMenuPolicy(Qt::CustomContextMenu);
        ProductsTree->setFrameShape(QFrame::Panel);
        ProductsTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ProductsTree->setTabKeyNavigation(true);
        ProductsTree->setAlternatingRowColors(true);
        ProductsTree->setExpandsOnDoubleClick(false);
        ProductsTree->header()->setMinimumSectionSize(50);
        PropertiesTree = new QTreeWidget(centralwidget);
        PropertiesTree->setObjectName(QString::fromUtf8("PropertiesTree"));
        PropertiesTree->setGeometry(QRect(230, 260, 311, 192));
        PropertiesTree->setMinimumSize(QSize(300, 100));
        PropertiesTree->setFocusPolicy(Qt::NoFocus);
        PropertiesTree->setContextMenuPolicy(Qt::NoContextMenu);
        PropertiesTree->setFrameShape(QFrame::Panel);
        PropertiesTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
        PropertiesTree->setAlternatingRowColors(true);
        PropertiesTree->setSelectionMode(QAbstractItemView::NoSelection);
        PropertiesTree->header()->setVisible(false);
        PropertiesTree->header()->setMinimumSectionSize(150);
        PropertiesTree->header()->setDefaultSectionSize(160);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 558, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuProduct = new QMenu(menubar);
        menuProduct->setObjectName(QString::fromUtf8("menuProduct"));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        MainWindow->setMenuBar(menubar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuProduct->menuAction());
        menubar->addAction(menu->menuAction());
        menuFile->addAction(actionFileNew);
        menuFile->addAction(actionFileOpen);
        menuFile->addAction(actionFileSave);
        menuFile->addSeparator();
        menuFile->addAction(actionSettings);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuProduct->addAction(actionProductAdd);
        menuProduct->addAction(actionProductDuplicate);
        menuProduct->addAction(actionProductEdit);
        menuProduct->addAction(actionProductDelete);
        menuProduct->addSeparator();
        menuProduct->addAction(actionEditImposition);
        menu->addAction(actionAbout);
        menu->addSeparator();
        menu->addAction(actionHelp);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        actionFileOpen->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214...", nullptr));
#if QT_CONFIG(statustip)
        actionFileOpen->setStatusTip(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\320\262\320\260\320\265\321\202 \321\204\320\260\320\271\320\273", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        actionFileOpen->setWhatsThis(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\320\262\320\260\320\265\321\202 \321\204\320\260\320\271\320\273", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(shortcut)
        actionFileOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionProductAdd->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\270\320\267\320\264\320\265\320\273\320\270\320\265...", nullptr));
#if QT_CONFIG(tooltip)
        actionProductAdd->setToolTip(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\273\321\217\320\265\321\202 \320\275\320\276\320\262\320\276\320\265 \320\270\320\267\320\264\320\265\320\273\320\270\320\265", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        actionProductAdd->setStatusTip(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\273\321\217\320\265\321\202 \320\275\320\276\320\262\320\276\320\265 \320\270\320\267\320\264\320\265\320\273\320\270\320\265", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        actionProductAdd->setWhatsThis(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\273\321\217\320\265\321\202 \320\275\320\276\320\262\320\276\320\265 \320\270\320\267\320\264\320\265\320\273\320\270\320\265", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(shortcut)
        actionProductAdd->setShortcut(QCoreApplication::translate("MainWindow", "+", nullptr));
#endif // QT_CONFIG(shortcut)
        actionProductDelete->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\270\320\267\320\264\320\265\320\273\320\270\320\265", nullptr));
#if QT_CONFIG(tooltip)
        actionProductDelete->setToolTip(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\321\217\320\265\321\202 \320\270\320\267\320\264\320\265\320\273\320\270\320\265", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionProductDelete->setShortcut(QCoreApplication::translate("MainWindow", "Del", nullptr));
#endif // QT_CONFIG(shortcut)
        actionProductEdit->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\320\275\320\270\321\202\321\214 \320\270\320\267\320\264\320\265\320\273\320\270\320\265...", nullptr));
#if QT_CONFIG(tooltip)
        actionProductEdit->setToolTip(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\320\275\320\265\320\275\320\270\320\265 \320\270\320\267\320\264\320\265\320\273\320\270\321\217", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionProductEdit->setShortcut(QCoreApplication::translate("MainWindow", "Return", nullptr));
#endif // QT_CONFIG(shortcut)
        actionEditImposition->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\320\275\320\270\321\202\321\214 \321\201\320\277\321\203\321\201\320\272...", nullptr));
#if QT_CONFIG(tooltip)
        actionEditImposition->setToolTip(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\320\275\320\265\320\275\320\270\320\265 \321\201\320\277\321\203\321\201\320\272\320\260", nullptr));
#endif // QT_CONFIG(tooltip)
        actionFileSave->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214...", nullptr));
#if QT_CONFIG(tooltip)
        actionFileSave->setToolTip(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\321\217\320\265\321\202 \321\204\320\260\320\271\320\273", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        actionFileSave->setStatusTip(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\321\217\320\265\321\202 \321\204\320\260\320\271\320\273", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        actionFileSave->setWhatsThis(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\321\217\320\265\321\202 \321\204\320\260\320\271\320\273", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(shortcut)
        actionFileSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", nullptr));
#if QT_CONFIG(statustip)
        actionExit->setStatusTip(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\272\321\200\321\213\321\202\320\270\320\265 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\213", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        actionExit->setWhatsThis(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\272\321\200\321\213\321\202\320\270\320\265 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\213", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFileNew->setText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \321\200\320\260\321\201\321\207\321\221\321\202...", nullptr));
#if QT_CONFIG(statustip)
        actionFileNew->setStatusTip(QCoreApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\320\265\321\202 \320\275\320\276\320\262\321\213\320\271 \321\200\320\260\321\201\321\207\320\265\321\202 / \320\261\320\273\320\260\320\275\320\272 \320\267\320\260\320\272\320\260\320\267\320\260", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        actionFileNew->setWhatsThis(QCoreApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\320\265\321\202 \320\275\320\276\320\262\321\213\320\271 \321\200\320\260\321\201\321\207\320\265\321\202 / \320\261\320\273\320\260\320\275\320\272 \320\267\320\260\320\272\320\260\320\267\320\260", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(shortcut)
        actionFileNew->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOrderPrint->setText(QCoreApplication::translate("MainWindow", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202 \320\261\320\273\320\260\320\275\320\272\320\260 \320\267\320\260\320\272\320\260\320\267\320\260 \320\262 PDF...", nullptr));
#if QT_CONFIG(statustip)
        actionOrderPrint->setStatusTip(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\207\320\260\321\202\321\214 \320\261\320\273\320\260\320\275\320\272\320\260 \320\267\320\260\320\272\320\260\320\267\320\260", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        actionOrderPrint->setWhatsThis(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\207\320\260\321\202\321\214 \320\261\320\273\320\260\320\275\320\272\320\260 \320\267\320\260\320\272\320\260\320\267\320\260", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(shortcut)
        actionOrderPrint->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265...", nullptr));
#if QT_CONFIG(statustip)
        actionAbout->setStatusTip(QCoreApplication::translate("MainWindow", "\320\241\320\262\320\265\320\264\320\265\320\275\320\270\321\217 \320\276 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        actionAbout->setWhatsThis(QCoreApplication::translate("MainWindow", "\320\241\320\262\320\265\320\264\320\265\320\275\320\270\321\217 \320\276 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
#endif // QT_CONFIG(whatsthis)
        actionHelp->setText(QCoreApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260...", nullptr));
#if QT_CONFIG(statustip)
        actionHelp->setStatusTip(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\267\320\276\320\262 \321\201\320\277\321\200\320\260\320\262\320\272\320\270", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        actionHelp->setShortcut(QCoreApplication::translate("MainWindow", "F1", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSettings->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270...", nullptr));
#if QT_CONFIG(statustip)
        actionSettings->setStatusTip(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\213", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        actionSettings->setWhatsThis(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\213", nullptr));
#endif // QT_CONFIG(whatsthis)
        actionEditOrder->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\320\275\320\270\321\202\321\214 \320\267\320\260\320\272\320\260\320\267...", nullptr));
#if QT_CONFIG(tooltip)
        actionEditOrder->setToolTip(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\320\275\320\265\320\275\320\270\320\265 \320\261\320\273\320\260\320\275\320\272\320\260 \320\267\320\260\320\272\320\260\320\267\320\260", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        actionEditOrder->setStatusTip(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\320\275\320\265\320\275\320\270\320\265 \320\261\320\273\320\260\320\275\320\272\320\260 \320\267\320\260\320\272\320\260\320\267\320\260", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        actionEditOrder->setWhatsThis(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\320\275\320\265\320\275\320\270\320\265 \320\261\320\273\320\260\320\275\320\272\320\260 \320\267\320\260\320\272\320\260\320\267\320\260", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(shortcut)
        actionEditOrder->setShortcut(QCoreApplication::translate("MainWindow", "Shift+Return", nullptr));
#endif // QT_CONFIG(shortcut)
        actionProductDuplicate->setText(QCoreApplication::translate("MainWindow", "\320\224\321\203\320\261\320\273\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\270\320\267\320\264\320\265\320\273\320\270\320\265", nullptr));
#if QT_CONFIG(shortcut)
        actionProductDuplicate->setShortcut(QCoreApplication::translate("MainWindow", "D", nullptr));
#endif // QT_CONFIG(shortcut)
        QTreeWidgetItem *___qtreewidgetitem = ProductsTree->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "\320\230\320\267\320\264\320\265\320\273\320\270\321\217 \320\270 \321\201\320\277\321\203\321\201\320\272\320\270", nullptr));
        QTreeWidgetItem *___qtreewidgetitem1 = PropertiesTree->headerItem();
        ___qtreewidgetitem1->setText(1, QCoreApplication::translate("MainWindow", "\320\227\320\275\320\260\321\207\320\265\320\275\320\270\320\265", nullptr));
        ___qtreewidgetitem1->setText(0, QCoreApplication::translate("MainWindow", "\320\241\320\262\320\276\320\271\321\201\321\202\320\262\320\276", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
        menuProduct->setTitle(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\264\320\265\320\273\320\270\320\265", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", nullptr));
        (void)MainWindow;
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
