#pragma once

#include <QUrl>
#include <QTreeWidget>
#include <QSplitter>
#include <QFileDialog>
#include <QMainWindow>
#include "Globals.h"
#include "Product.h"
#include "ui_MainWindow.h"

namespace Ui {
class MainWindow;
}


extern QString & sFontName;

Q_GLOBAL_STATIC_WITH_ARGS(QFont, font8pt,		(sFontName, 8));
Q_GLOBAL_STATIC_WITH_ARGS(QFont, font10pt,		(sFontName, 10));
Q_GLOBAL_STATIC_WITH_ARGS(QFont, font12pt,		(sFontName, 12));
Q_GLOBAL_STATIC_WITH_ARGS(QFont, font24pt,		(sFontName, 24));
Q_GLOBAL_STATIC_WITH_ARGS(QFont, font10ptBold,	(sFontName, 10,	QFont::Bold));
Q_GLOBAL_STATIC_WITH_ARGS(QFont, font12ptBold,	(sFontName, 12, QFont::Bold));
Q_GLOBAL_STATIC_WITH_ARGS(QFont, font24ptBold,	(sFontName, 24, QFont::Bold));


// Выбор окончания для слов:
// word1    = слово без окончания		 (например, "вид")
// word2_4  = окончание для чисел 2...4  (например, "вида")
// word5_10 = окончание для чисел 5...10 (например, "видов")
const QString & SelectWordEnding(size_t number,
								 const QString & word1,
								 const QString & word2_4,
								 const QString & word5_10);

class MainWindow : public QMainWindow {
	Q_OBJECT

	Ui::MainWindow *ui;

	const QString sCurrentFileVersion { S("ver=1.0") };
    const QString sColonSpace { S(", ") };
    const QString sDefaultDocName { S("Без имени") };
    const QString sFileFilter =
		S("Файлы полиграфического калькулятора (*.printcalc)");
	const QUrl urlMyDocuments =
		{ S("clsid:FDD39AD0-238F-46AF-ADB4-6C85480369C7") };

	bool isModified		= false;
	bool isFileExist	= false;
	bool canChange		= false;

	QString sDocPath;
	QString sDocName;
	qreal yPos;
	size_t page, pages;

	void DeleteAllProducts() {
		ui->ProductsTree->clear();
		ui->PropertiesTree->clear();
		products.clear();
	}

	void SetMainTitle();
	void SetModified(bool modify = true);
	void SetProductItem(Product * product,
						bool setModified = true,
						QTreeWidgetItem * productItem = nullptr);
	void InsertProduct(Product * product,
					   bool setModified = true,
					   qsizetype index = -1);
	void EnableActions();
	void Deselect();
	bool ConfirmSave();
	void closeEvent(QCloseEvent * ce) override;
	bool event(QEvent * ev) override;
	bool nativeEvent(const QByteArray & eventType,
					 void * message,
					 qintptr * result) override;
	Product * FetchProductFromItem(QTreeWidgetItem * item,
								   bool strongCheck = false) const;
	QTreeWidgetItem * GetSelectedItem() const;
	Imposition * FetchImpFromItem(const QTreeWidgetItem * item) const;
	void ShowProductInfo(const Product * product);
	void ShowImpositionInfo(const Imposition * imposition);

	QTreeWidgetItem * NewItem(QTreeWidgetItem * const parent,
							  const QString & s1,
							  const QString & s2,
							  bool setBold = false)
	{
		auto item = new QTreeWidgetItem(parent, { s1, s2 });
		if (setBold) {
			auto fontBold = item->font(0);
			fontBold.setBold(true);
			//item->setFont(0, fontBold);
			item->setFont(1, fontBold);
		}
		return item;
	}

public:
	EStartOption startOption = (EStartOption) - 1;
	QString sOpenFileName;
	bool bPathToCSVInvalid = false;

	MainWindow(QWidget * parent = nullptr);
	~MainWindow() override;
	void Init();
	void ParseCSVFiles();
	void OpenFile(const QString & sOpenFileName);

	inline void WriteHWND() {
		*static_cast <HWND *> (pBuf) = (HWND) winId();
	}
};
