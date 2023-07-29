
#include <QCloseEvent>
#include <QSettings>
#include "Globals.h"
#include "MsgBox.h"
#include "MainWindow.h"
#include "AboutDlg.hpp"
#include "ImpWizard.h"
#include "ProductDlg.h"
#include "SettingsDlg.h"

#define GLOBAL_REF(TYPE, NAME, ARGS) \
	Q_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME##Ptr, ARGS); \
	TYPE & NAME = *NAME##Ptr;

GLOBAL_REF(QString,	вид,		(S("вид"		)))
GLOBAL_REF(QString,	вида,		(S("вида"		)))
GLOBAL_REF(QString,	видов,		(S("видов"		)))
GLOBAL_REF(QString,	фальц,		(S("фальц"		)))
GLOBAL_REF(QString,	фальца,		(S("фальца"		)))
GLOBAL_REF(QString,	фальцев,	(S("фальцев"	)))
GLOBAL_REF(QString,	биг,		(S("биг"		)))
GLOBAL_REF(QString,	бига,		(S("бига"		)))
GLOBAL_REF(QString,	бигов,		(S("бигов"		)))
GLOBAL_REF(QString,	угол,		(S("угол"		)))
GLOBAL_REF(QString,	угла,		(S("угла"		)))
GLOBAL_REF(QString,	углов,		(S("углов"		)))
//GLOBAL_REF(QString,	полоса,		("полоса")	);
//GLOBAL_REF(QString,	полосы,		("полосы")	);
//GLOBAL_REF(QString,	полос,		("полос")	);
GLOBAL_REF(QString,	страница,	(S("страница"	)))
GLOBAL_REF(QString,	страницы,	(S("страницы"	)))
GLOBAL_REF(QString,	страниц,	(S("страниц"	)))


MainWindow::MainWindow(QWidget * parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QSplitter * splitter = new QSplitter(Qt::Horizontal, this);
	splitter->addWidget(ui->ProductsTree);
	splitter->addWidget(ui->PropertiesTree);
	splitter->setChildrenCollapsible(false);
	splitter->setHandleWidth(10);
	auto listSizes = splitter->sizes();
	listSizes.first() = 150;
	splitter->setSizes(listSizes);
	ui->horizontalLayout->addWidget(splitter);
	ui->centralwidget->setLayout(ui->horizontalLayout);

	// Находится здесь из-за того, что парсинг CSV-файлов
	// происходит раньше вызова Init()
	connect(ui->actionSettings, &QAction::triggered, this, [=] {
		const auto oldPathToCSV = sPathToCSV;
		SettingsDlg settingsDlg(this);
		if (settingsDlg.exec() != QDialog::Accepted)
			return;
		if (sPathToCSV != oldPathToCSV) {
			settings->setValue(sPathToCSVKeyName, sPathToCSV);
			if (!bPathToCSVInvalid)
				ParseCSVFiles();
		}
		settings->setValue(sRoundCostKeyName, QString(roundCost));
	});
}


void MainWindow::closeEvent(QCloseEvent * ce) {
	if (isModified) {
		MsgBox mb(this);
		const auto response = mb.SavePrompt();
		if (response == MsgBox::Cancel) {
			ce->ignore();
			return;
		}
		if (response == MsgBox::Yes) {
			if (products.isEmpty()) {
				mb.Error(S("Чтобы сохранить заказ или расчёт,\n"
						   "добавьте хотя бы одно изделие!"));
				ce->ignore();
				return;
			}
			ui->actionFileSave->trigger();
		}
	}
	ce->accept();
}


void MainWindow::SetMainTitle() {
	QString s(sProgramName);
	if (!sDocName.isEmpty())
		s += S(" – [%2]").arg(sDocName);
	if (isModified)
		s += S(" *");
	setWindowTitle(s);
}


void MainWindow::SetModified(bool modify) {
	if (isModified == modify)
		return;
	isModified = modify;
	SetMainTitle();
}


void MainWindow::InsertProduct(Product * product,
							   bool setModified,
							   qsizetype index)
{
	products.Insert(product, index);
	const auto item = new QTreeWidgetItem();
	const int insertIndex =
		index == -1 ? ui->ProductsTree->topLevelItemCount() : index;
	item->setData(0, Qt::UserRole, qintptr(product));
	ui->ProductsTree->insertTopLevelItem(insertIndex, item);
	SetProductItem(product, setModified, item);
}


void MainWindow::SetProductItem(Product * product,
								bool setModified,
								QTreeWidgetItem * productItem)
{
	if (!productItem)
		productItem = GetSelectedItem();

	Q_ASSERT(productItem and !productItem->parent());
	if (product->sName != productItem->text(0))
		productItem->setText(0, product->sName);

	for (const auto child : productItem->takeChildren())
		delete child;

	for (const auto imp : qAsConst(product->impositions)) {
		auto impItem = new QTreeWidgetItem(productItem, { imp->GetName() });
		impItem->setData(0, Qt::UserRole, enum_integer(imp->eType));
	}

	productItem->setExpanded(true);
	if (setModified) {
		if (!isModified)
			SetModified();
		const auto selItems = ui->ProductsTree->selectedItems();
		if (!selItems.isEmpty()) {
			Q_ASSERT(selItems.count() == 1);
			canChange = false;
			selItems.first()->setSelected(false);
			canChange = true;
		}
		productItem->setSelected(true);
	}
}


void MainWindow::EnableActions() {
	const auto selItem = GetSelectedItem();
	const bool isProductSelected	= selItem and !selItem->parent();
	const bool isImpSelected		= selItem and selItem->parent();

	ui->actionProductDuplicate	->setEnabled(isProductSelected or
											 isImpSelected);
	ui->actionProductEdit		->setEnabled(isProductSelected);
	ui->actionProductDelete		->setEnabled(isProductSelected);
	ui->actionEditImposition	->setEnabled(isImpSelected);
    ui->actionFileNew			->setEnabled(isModified);
	ui->actionFileSave			->setEnabled(isModified and
											 !products.isEmpty());
}


void MainWindow::Deselect() {
	for (auto item : ui->ProductsTree->selectedItems())
		item->setSelected(false);
}


bool MainWindow::ConfirmSave() {
	if (isModified) {
		MsgBox mb(this);
		switch (mb.SavePrompt()) {
			case MsgBox::Yes:
				ui->actionFileSave->trigger();
				return !isModified;

			case MsgBox::Cancel:
				return false;

			default:
				break;
		}
	}
	return true;
}




void MainWindow::Init() {
	connect(ui->actionAbout, &QAction::triggered, this, [=] {
		AboutDlg aboutDlg(this);
		aboutDlg.exec();
	});

	connect(ui->actionHelp, &QAction::triggered, this, [=] {
		static_cast <MyApplication *> (QCoreApplication::instance())->Help();
	});

	auto AddProductPrompt = [this] () -> void {
		if (products.isEmpty()) {
			MsgBox mb(this);
			if (mb.YesNoPrompt(S("Желаете добавить изделие?"), true)
				== MsgBox::Yes)
				ui->actionProductAdd->trigger();
		}
	};

	connect(ui->actionFileNew, &QAction::triggered, this, [=] {
        if (startOption == EStartOption::New)
            SetModified();
        else
			if (startOption != EStartOption::CalcCostOnly and isModified) {
				MsgBox mb(this);
				if (mb.SavePrompt() == MsgBox::Yes) {
					ui->actionFileSave->trigger();
					if (isModified)
						return;
				}
				DeleteAllProducts();
				sDocPath.clear();
			}

		isFileExist = false;
        sDocName = sDefaultDocName;
		SetMainTitle();
		Deselect();
		EnableActions();
#ifdef _DEBUG
		ui->actionProductAdd->trigger();
#else
		AddProductPrompt();
#endif // _DEBUG
	});

	connect(ui->actionFileOpen, &QAction::triggered, this, [=] {
		if (!ConfirmSave())
			return;

		QFileDialog openFileDlg(this, S("Открыть файл"));
		openFileDlg.setDefaultSuffix(sFileExtention);
		openFileDlg.setFileMode(QFileDialog::ExistingFile);
		openFileDlg.setAcceptMode(QFileDialog::AcceptOpen);
		openFileDlg.setNameFilter(sFileFilter);
		if (isFileExist) {
			openFileDlg.setDirectory(sDocPath);
			openFileDlg.selectFile(sDocName + sFileExtention);
		} else
			openFileDlg.setDirectoryUrl(urlMyDocuments);

		if (openFileDlg.exec() != QDialog::Accepted)
			return;

		const auto & selFiles = openFileDlg.selectedFiles();
		Q_ASSERT(selFiles.count() == 1);
		OpenFile(selFiles.first());
	});

	connect(ui->actionFileSave, &QAction::triggered, this, [=] {
		Q_ASSERT(!products.isEmpty());
		Q_ASSERT(ui->ProductsTree->topLevelItemCount());

		QString fileName;

		if (isFileExist)
			fileName = sDocPath + sDocName + sFileExtention;
		else {
			QFileDialog saveFileDlg(this, S("Сохранить файл"));
			saveFileDlg.setAcceptMode(QFileDialog::AcceptSave);
			saveFileDlg.setDefaultSuffix(sFileExtention);
			saveFileDlg.setNameFilter(sFileFilter);
			fileName = order.GetName();
			if (!order.IsFilled())
				fileName +=
					S("_%1_%2").arg(loc.toString(QDate::currentDate(),
												 S("yyyy-MM-dd")),
									loc.toString(QTime::currentTime(),
												 S("hh-mm-ss")));
			saveFileDlg.selectFile(fileName);
			if (isFileExist)
				saveFileDlg.setDirectory(sDocPath);
			else
				saveFileDlg.setDirectoryUrl(urlMyDocuments);

			if (saveFileDlg.exec() != QDialog::Accepted or
				saveFileDlg.selectedFiles().isEmpty()) {
				return;
			}

			sDocPath = QDir::toNativeSeparators(
					saveFileDlg.directory().absolutePath());
			const auto & selFiles = saveFileDlg.selectedFiles();
			Q_ASSERT(selFiles.count() == 1);
			fileName = selFiles.first();
			isFileExist = true;
		}

		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly)) {
			MsgBox mb(this);
			mb.Error(S("Не удалось открыть файл для сохранения.\n"
					   "Сохранение не выполнено!\n"));
			return;
		}

		QDataStream stream(&file);
		stream.setVersion(CURRENT_QT_VERSION);
		stream << sCurrentFileVersion;
		stream << order;
		stream << (qint32) products.count();
		Q_ASSERT(stream.status() == QDataStream::Ok);
		for (const auto & product : qAsConst(products)) {
			stream << *product;
			if (stream.status() != QDataStream::Ok) {
				MsgBox mb(this);
				mb.Error(S("Неизвестная ошибка при сохранении файла "
						   "(stream error).\n"
						   "Сохранение не выполнено!\n"));
				return;
			}
		}

		if (!file.flush()) {
			MsgBox mb(this);
			mb.Error(S("Неизвестная ошибка при сохранении файла "
					   "(flush error).\n"
					   "Сохранение не выполнено!\n"));
			return;
		}

		file.close();
		sDocName = QFileInfo(fileName).baseName();
		Q_ASSERT(!sDocName.isEmpty());
		if (order.IsFilled())
			startOption = EStartOption::NormalOperation;
		SetModified(false);
		//Deselect();
		//EnableActions();
	});

	connect(ui->actionExit, &QAction::triggered, this, [=] {
		close();
	});

	connect(ui->actionEditOrder, &QAction::triggered, this, [=] {
		if (!EditOrder())
			return;
		using enum EStartOption;
		if (startOption == New or startOption == CalcCostOnly)
			startOption = NormalOperation;
		sDocName = order.GetName();
		SetModified(false);
		EnableActions();
		AddProductPrompt();
	});

	auto IsProductsCountValid = [this] () -> bool {
		if (products.count() <= MAX_PRODUCTS)
			return true;
		MsgBox mb(this);
		mb.Error(S("Вы не можете добавить больше %1 изделий!")
				 .arg(MAX_PRODUCTS));
		return false;
	};

	connect(ui->actionProductAdd, &QAction::triggered, this, [=] {
		if (!IsProductsCountValid())
			return;
		ProductDlg productDlg(new Product, this);
		if (productDlg.exec() == QDialog::Accepted)
			InsertProduct(productDlg.p);
	});

	connect(ui->actionProductDuplicate, &QAction::triggered, this, [=] {
		if (!IsProductsCountValid())
			return;
		auto selItem = GetSelectedItem();
		if (selItem->parent())
			selItem = selItem->parent();
		const auto product = FetchProductFromItem(selItem);
		Q_ASSERT(product);
		InsertProduct(new Product(*product),
					  ui->ProductsTree->indexOfTopLevelItem(selItem));
	});

	connect(ui->actionProductEdit, &QAction::triggered, this, [=] {
		auto product = FetchProductFromItem(GetSelectedItem());
		Q_ASSERT(product);
		ProductDlg productDlg(new Product(*product), this);
		if (productDlg.exec() != QDialog::Accepted or
			*product == *productDlg.p)
			return;
		*product = *productDlg.p;
		product->SetImpositionsOwner();
		SetProductItem(product, true);
		ShowProductInfo(product);
	});

	connect(ui->actionProductDelete, &QAction::triggered, this, [=] {
		MsgBox mb(this);
		if (mb.YesNoPrompt(S("Действительно удалить изделие?"))
			== MsgBox::Yes)
		{
			products.Remove(FetchProductFromItem(GetSelectedItem()));
			delete ui->ProductsTree->currentItem();
			if (!isModified)
				SetModified();
		}
		Deselect();
		EnableActions();
	});

	connect(ui->actionEditImposition, &QAction::triggered, this, [=] {
		const auto selItem = GetSelectedItem();
		Q_ASSERT(selItem);
		auto p = FetchProductFromItem(selItem);
		Q_ASSERT(p);
		auto imp = FetchImpFromItem(selItem);
		Q_ASSERT(imp);
		if (ProductDlg::ExecEditImposition(p, imp->eType, false, this)) {
			SetModified();
			ShowImpositionInfo(imp);
		}
	});

	connect(ui->ProductsTree, &QTreeWidget::itemDoubleClicked, this,
			[=] (QTreeWidgetItem * item) {
		if (item->parent())
			ui->actionEditImposition->trigger();
		else
			ui->actionProductEdit->trigger();
	});

	connect(ui->ProductsTree, &QTreeWidget::itemSelectionChanged, this, [=] {
		const auto selItem = GetSelectedItem();
		if (selItem) {
			const auto product = FetchProductFromItem(selItem);
			Q_ASSERT(product);
			if (!selItem->parent())
				// Просматриваем изделие
				ShowProductInfo(product);
			else
				// Просматриваем спуск
				ShowImpositionInfo(FetchImpFromItem(selItem));
		}
		EnableActions();
	});

	connect(ui->ProductsTree, &QTreeWidget::customContextMenuRequested, this,
			[=] (const QPoint & pos) {
		const auto item = ui->ProductsTree->itemAt(pos);
		QMenu menu(this);
		if (!item)
			menu.addAction(ui->actionProductAdd);
		else {
			EnableActions();
			if (item->parent())
				menu.addAction(ui->actionEditImposition);
			else {
				menu.addAction(ui->actionProductDuplicate);
				menu.addAction(ui->actionProductEdit);
				menu.addAction(ui->actionProductDelete);
			}
		}
		menu.exec(QCursor::pos());
	});

	connect(ui->actionOrderPrint, &QAction::triggered, this, [=] {
		if (products.isEmpty()) {
			MsgBox mb(this);
			mb.Error(S("Необходимо создать хотя бы одно изделие\n"
					   "перед экспортом!"));
			return;
		}

		if (!order.IsFilled()) {
			ui->actionEditOrder->trigger();
			if (!order.IsFilled()) {
				MsgBox mb(this);
				mb.Error(S("Необходимо заполнить параметры заказа\n"
						   "перед экспортом!"));
				return;
			}
		}

		QFileDialog fileDlg;
		fileDlg.setAcceptMode(QFileDialog::AcceptSave);
		fileDlg.setDefaultSuffix(S("pdf"));
		fileDlg.setMimeTypeFilters({ S("application/pdf") });
		fileDlg.setWindowTitle(S("Экспортировать бланк заказа в PDF"));
		fileDlg.selectFile(sDocName);
		if (fileDlg.exec() == QDialog::Accepted) {
			const auto & selFiles = fileDlg.selectedFiles();
			Q_ASSERT(!selFiles.isEmpty());
			Print(selFiles.first());
		}
	});

	SetMainTitle();

	if (manager.IsEmpty()) {
		MsgBox mb(this);
		mb.Info(S("Информация о менеджере не заполнена!\n"
				  "Заполните, пожалуйста, информацию в настройках."));
		ui->actionSettings->trigger();
		if (manager.IsEmpty()) {
			mb.Error(S("Без указания данных о менеджере\n"
					   "продолжение невозможно!"));
			exit(0);
		}
	}

	if (startOption != EStartOption::OpenFromShell) {
#ifdef _DEBUG
		startOption = EStartOption::CalcCostOnly;
#else
		StartDlg startDlg(this);
		startDlg.exec();
		startOption = startDlg.startOption;
#endif //_DEBUG
	}

	Q_ASSERT(startOption != (EStartOption) -1);

	switch (startOption) {
		case EStartOption::CalcCostOnly:
//#ifdef _DEBUG
//			ui->actionFileOpen->trigger();
//			break;
//#endif //_DEBUG

		case EStartOption::New:
			ui->actionFileNew->trigger();
			break;

		case EStartOption::Open:
			ui->actionFileOpen->trigger();
			break;

		case EStartOption::OpenFromShell:
			Q_ASSERT(!sOpenFileName.isEmpty());
			OpenFile(sOpenFileName);
			sOpenFileName.clear();
			break;

		default:
			Q_ASSERT(false);
			break;
	}
}


void MainWindow::ParseCSVFiles() {
	MsgBox mb(this);
	QDir csvDir(sPathToCSV.isEmpty() ?
						S("%1/CSV").arg(QDir::currentPath()) :
						sPathToCSV);

	if (!csvDir.exists()) {
		if (mb.YesNoPrompt(S("Неверный путь к файлам CSV.\n\n"
							 "Запустить диалог настроек?\n"
							 "*(если Вы выберете «Нет», "
							 "программа завершит работу)"))
			== MsgBox::Yes)
		{
			bPathToCSVInvalid = true;
			ui->actionSettings->trigger();
			if (!QDir(sPathToCSV).exists())
				exit(EXIT_FAILURE);
			bPathToCSVInvalid = false;
		} else
			exit(EXIT_FAILURE);
	}

	QFile file;
	QString sFileName;
	QTextStream ts;
	const auto sFileEmpty		S("Файл пуст"						);
	const auto sUnableOpenFile	S("Невозможно открыть файл"			);
	const auto sWrongParameter	S("Неверное значение"				);
	const auto sWrongOperation	S("Неизвестная операция"			);
	const auto sWrongColumns	S("Неверное количество столбцов"	);
	const auto sWrongPrice		S("Неверная цена"					);
	size_t lineNumber = 0;

	auto ErrorInFile = [&] (const QString & sErrorInfo,
							const QString & sCaused = { }) -> void
	{
		auto s = S("Ошибка чтения файла «%1»:\n"
					"%2").arg(sFileName, sErrorInfo);
		if (sCaused.isEmpty())
			s += u'!';
		else {
			s += S(" в строке %1").arg(lineNumber);
			if (!sCaused.isEmpty())
				s += S(": «%1»").arg(sCaused);
			s += u'.';
		}
		s += S("\nПродолжение невозможно. Программа завершает работу!");
		mb.Error(s);
		exit(EXIT_FAILURE);
	};

	auto ErrorInFile1 = [&] (const QString & sErrorInfo,
							 const QString & sCaused = { }) -> void
	{
		// Уменьшаем текущий счетчик строк для ошибок, возникающих в первой строке
		--lineNumber;
		ErrorInFile(sErrorInfo, sCaused);
	};

	auto OpenStream = [&] (const QString & fileName) -> void {
		file.close();
		sFileName = fileName;
		file.setFileName(S("%1/%2").arg(csvDir.absolutePath(), sFileName));
		if (!file.open(QFile::ReadOnly | QFile::ExistingOnly))
			ErrorInFile(sUnableOpenFile);
		ts.setDevice(&file);
	};

	OpenStream(sCSVFileNamePaper);

	auto eOldType = EPaperType::WrongPaper;
	auto eType = EPaperType::WrongPaper;
	QList<EPaperFormat> eFormats;
	bool bEmptyLine = false;
	QSize customSize;
	paperPrices.clear();

	while (!ts.atEnd()) {
		++lineNumber;
		const auto & line = ts.readLine();
		if (line.isEmpty()) {
			bEmptyLine = true;
			continue;
		}
		const auto tokens = line.split(chTab);
		Q_ASSERT(tokens.count() >= 2);
		if (tokens[0].isEmpty() and tokens[1].isEmpty()) {
			bEmptyLine = true;
			continue;
		}

		if (bEmptyLine) {
			eOldType = eType;
			bEmptyLine = false;
		}

		auto s = tokens[0].simplified();
		const auto weight = s.toInt(&ok);
		if (!ok and !s.isEmpty()) {
			if (s.startsWith(S("Мелованная"), Qt::CaseInsensitive))
				s = sCoatedGlossy;

			eType = stdPaperTypes.GetType(s);
			if (eType == EPaperType::WrongPaper)
				ErrorInFile(S("Неизвестный тип бумаги"), s);

			eFormats.clear();

			for (SizeT i = 1; i < tokens.count(); i++) {
				s = tokens[i].simplified();
				if (!s.isEmpty()) {
					// Меняем русские буквы на латинские
					s.replace(QChar(u'х'), QChar(u'x'), Qt::CaseInsensitive);
					s.replace(QChar(u'а'), QChar(u'A'), Qt::CaseInsensitive);
					const auto eFormat = stdPaperFormats.GetEFormat(s);
					if (eFormat == EPaperFormat::WrongFormat)
						ErrorInFile(S("Неизвестный формат бумаги"), s);
					eFormats << eFormat;
				}
			}

			if (eFormats.count() < 1)
				ErrorInFile(S("Недостаточно форматов бумаг"));

			if (paperPrices.contains(eType))
				ErrorInFile(S("Этот тип бумаги уже был объявлен"));
		} else {
			Q_ASSERT(eType != EPaperType::WrongPaper and
					 eType != eOldType and
					 eFormats.count());
			size_t i = 1;
			for (const auto eFormat : eFormats) {
				const auto & s = tokens[i++];
				const auto price = s.isEmpty() ? 0.0 : loc.toFloat(s, &ok);
				if (!ok) {
					ErrorInFile(sWrongPrice, s);
					return;
				}
				const auto & size = eFormat == EPaperFormat::CustomFormat ?
										customSize :
										stdPaperFormats.GetSize(eFormat);
				paperPrices.AddPrice(eType, size, weight, price);
			}
		}
	}

	if (!lineNumber)
		ErrorInFile(sFileEmpty);

	OpenStream(sCSVFileNameOperations);

	operationPrices.clear();
	lineNumber = 0;
	bool isFirstLine = true;
	using enum EOperation;
	constexpr qsizetype MIN_TOKENS_COUNT	= 6;
	constexpr qsizetype COL_LAM_KIND		= 1;
	constexpr qsizetype COL_LAM_THICKNESS	= 2;
	constexpr qsizetype COL_LAM_FORMAT		= 3;
	constexpr qsizetype COL_PREPARATION		= 4;
	constexpr qsizetype COL_FIRST_PRICE		= COL_PREPARATION + 1;
	constexpr qsizetype COL_MAX_PRICES		= 12;
	qsizetype colPrices = -1;
	auto eOp = EOperation::WrongOperation;
	size_t setupCost = 0;
	Opt<bool> isGloss;
	Opt<size_t> thickness;
	Opt<EProductSize> eSize;

#ifdef _DEBUG
	QFile logFile("log.txt");
	logFile.open(QFile::WriteOnly | QFile::Truncate);
	QTextStream log(&logFile);
	const QString endl S("\r\n");
#endif // _DEBUG

	while (!ts.atEnd()) {
		++lineNumber;
		const auto & line1 = ts.readLine();
		if (line1.isEmpty())
			continue;

		++lineNumber;
		const auto & line2 = ts.readLine();
		if (line2.isEmpty())
			continue;

		const auto tokens1 = line1.split(chTab);
		if (tokens1.count() <= MIN_TOKENS_COUNT)
			ErrorInFile1(sWrongColumns);

		if (isFirstLine) {
			isFirstLine = false;
			const auto sNotes S("Примечания");
			for (const auto & s : tokens1) {
				if (s.simplified().startsWith(sNotes, Qt::CaseInsensitive))
					break;
				++colPrices;
			}
			if (colPrices <= COL_PREPARATION or colPrices > COL_MAX_PRICES)
				ErrorInFile1(S("Отсутствует столбец «%1»").arg(sNotes));
			continue;
		}

		const auto & sOp = tokens1.first();
		if (sOp.isEmpty()) {
			if (eOp == WrongOperation)
				ErrorInFile1(sWrongOperation);
		} else {
			eOp = mapOperationNames.value(sOp, WrongOperation);
			if (eOp == WrongOperation)
				ErrorInFile1(sWrongOperation, sOp);
		}

		const auto isLamOperation = IsLamOperation(eOp);
		if (!isLamOperation and operationPrices.contains(eOp))
			ErrorInFile1(S("Дублирование операций"), sOp);

		const auto tokens2 = line2.split(chTab);
		if (tokens2.count() <= MIN_TOKENS_COUNT)
			ErrorInFile(sWrongColumns);

		OperationPrice op;
		bool isTerminate = false;
		decltype(OperationPrice::Price::limit) limit, prevLimit;
		decltype(OperationPrice::Price::price) price, prevPrice;

		for (qsizetype i = 1; i < tokens1.count(); ++i) {
			const auto & s1 = tokens1.at(i);
			const auto & s2 = tokens2.at(i);

			auto IsLamCellValid = [&] () -> bool {
				if (isLamOperation) {
					if (!s2.isEmpty())
						ErrorInFile(sWrongParameter, s2);
					return true;
				} else
					// Если НЕ операция ламинации, то первые три столбца
					// должны быть пусты в обеих строках
					if (s1.isEmpty() and s2.isEmpty())
						return true;
				if (s2.isEmpty())
					ErrorInFile1(sWrongParameter, s1);
				else
					ErrorInFile(sWrongParameter, s2);
				return false;
			};

			if (i < COL_FIRST_PRICE and !s2.isEmpty())
				// Если НЕ колонка цен, то вторая строка в столбце
				// должна быть пуста
				ErrorInFile(sWrongParameter, s2);

			auto & lam = op.lam;

			switch (i) {
				// Вид ламинации: матовая или глянцевая
				case COL_LAM_KIND:
					if (IsLamCellValid() and !isLamOperation)
						continue;
					Q_ASSERT(!lam);
					lam = { .eSize = EProductSize::CustomSize };
					if (s1.isEmpty())
						if (isGloss)
							// Если пустая ячейка, берем предыдущее значение
							lam->isGloss = *isGloss;
						else
							ErrorInFile1(sWrongParameter, s1);
					else {
						if (s1.startsWith(u'г', Qt::CaseInsensitive))
							lam->isGloss = true;
						else
							if (s1.startsWith(u'м', Qt::CaseInsensitive))
								lam->isGloss = false;
							else
								ErrorInFile1(sWrongParameter, s1);
						isGloss = lam->isGloss;
					}
					break;

				// Толщина ламинации в мкм
				case COL_LAM_THICKNESS:
					if (IsLamCellValid() and !isLamOperation)
						continue;
					Q_ASSERT(lam);
					if (s1.isEmpty())
						if (thickness)
							// Если пустая ячейка, берем предыдущее значение
							lam->thickness = *thickness;
						else
							ErrorInFile1(sWrongParameter, s1);
					else {
						thickness = lam->thickness = loc.toUInt(s1, &ok);
						if (!ok)
							ErrorInFile1(sWrongParameter, s1);
						if (lam->thickness < MIN_LAM_THICKNESS)
							ErrorInFile1(S("Слишком тонкая ламинация"), s1);
						if (lam->thickness > MAX_LAM_THICKNESS)
							ErrorInFile1(S("Слишком толстая ламинация"), s1);
					}
					break;

				// Формат (для пакетной ламинации)
				case COL_LAM_FORMAT:
					if (IsLamCellValid() and !isLamOperation)
						continue;
					Q_ASSERT(lam);
					if (s1.isEmpty()) {
						if (eOp == EOperation::LamPacket) {
							if (eSize.value_or(EProductSize::CustomSize) ==
								EProductSize::CustomSize)
								ErrorInFile1(S("Неверный формат "
											   "пакетной ламинации"), s1);
							lam->eSize = *eSize;
						}
					} else {
						if (eOp != EOperation::LamPacket)
							ErrorInFile1(sWrongParameter, s1);
						auto sFormat = loc.toUpper(s1);
						if (sFormat.at(0) == u'A')
							sFormat[0] = u'А';
						eSize = lam->eSize = stdProductSizes.GetType(sFormat);
						if (lam->eSize == EProductSize::CustomSize)
							ErrorInFile1(sWrongParameter, s1);
					}
					break;

				// Стоимость приладки
				case COL_PREPARATION:
					if (s1.isEmpty())
						op.setupCost = setupCost;
					else {
						const auto & sNum = s1.endsWith(u'₽') ? s1.chopped(1) : s1;
						op.setupCost = setupCost = loc.toUInt(sNum, &ok);
						if (!ok or op.setupCost > 100000)
							ErrorInFile1(sWrongPrice, s1);
					}
					break;

				// Пороги и цены
				default:
					if (i > colPrices or (s1.isEmpty() and s2.isEmpty())) {
						// Завершаем, если обе ячейки пусты
						// или индекс == макс. индексу для цен
						isTerminate = true;
						break;
					}

					if (i == COL_FIRST_PRICE) {
						// Первый столбец с порогами цен
						if (!s1.isEmpty())
							ErrorInFile1(sWrongParameter, s1);
						if (s2.isEmpty())
							ErrorInFile(sWrongParameter);
						limit = prevLimit = 0;
						prevPrice = INT_MAX;
					} else {
						limit = loc.toUInt(s1, &ok);
						if (!ok or limit > 5000000)
							ErrorInFile1(S("Неверный порог"), s1);
						if (limit <= prevLimit)
							ErrorInFile1(S("Пороги не упорядочены "
										   "по возрастанию"), s1);
						prevLimit = limit;
					}

					const auto & sNum = s2.endsWith(u'₽') ? s2.chopped(1) : s2;
					price = loc.toDouble(sNum, &ok);
					if (!ok or price <= 0.009 or price > 100000.0)
						ErrorInFile(sWrongPrice, s2);
					if (price >= prevPrice)
						ErrorInFile1(S("Цены не упорядочены по убыванию"), s2);
					prevPrice = price;
					op.prices << OperationPrice::Price { limit, price };
				break;
			}

			if (isTerminate)
				break;
		}

		operationPrices.Add(eOp, op);

//#ifdef _DEBUG
//		ts << mapOperationNames.key(eOp);
//
//		if (op.lam.has_value()) {
//			Q_ASSERT(isLamOperation);
//			QString s;
//			if (eOp == EOperation::LamPacket)
//				s = S(", %1").arg(stdSizeNames[op.lam->eSize]);
//			ts << S(", %1, %2 мкм%3")
//				.arg(op.lam->isGloss ? S("гл") : S("мат"))
//				.arg(op.lam->thickness, 3)
//				.arg(s);
//		}
//
//		ts << "\r\n";
//
//		if (op.setupCost) {
//			ts << S("\tприладка: %1 р").arg(op.setupCost);
//			ts << "\r\n";
//		}
//
//		for (const auto price : op.prices) {
//			ts << S("\t%1: %2 р")
//				.arg(price.limit, 5)
//				.arg(price.price, 5);
//			ts << "\r\n";
//		}
//
//		ts << "\r\n";
//#endif // _DEBUG
	}

//#ifdef _DEBUG
//	ts.flush();
//	logFile.close();
//#endif // _DEBUG

	if (!lineNumber or operationPrices.isEmpty())
		ErrorInFile(sFileEmpty);

	OpenStream(sCSVFileNameStates);
	lineNumber = 0;

	using enum EIndexStates;

	while (!ts.atEnd()) {
		++lineNumber;
		const auto & line = ts.readLine();
		if (line.isEmpty())
			ErrorInFile(sWrongParameter);
		if (lineNumber < 5)
			continue;

#ifdef _DEBUG
		log << S("(%1)   ").arg(lineNumber, 3);
#endif // _DEBUG

		const auto state = new State;
		const auto & tokens = line.split(chTab);
		Q_ASSERT(tokens.count() == INDEXES_COUNT);

		Opt<EPaperFormat>	defaultFormat;
		Opt<EPortion>		defaultPortion;
		Opt<EPress>			defaultPress;
		static const QChar	chPlus		{ '+' };
		static const QChar	chAsteriks	{ '*' };

		for (auto i = EnumCast<EIndexStates>(0);
			 i < EIndexStates::iDummy;
			 ++i)
		{
			const auto & s = tokens.at(EnumIndex(i));
			switch (i) {
				// Тип бумаги
				case iPaperType: {
					Q_ASSERT(!s.isEmpty());
					state->paperType = stdPaperTypes.GetType(s);
					Q_ASSERT(state->paperType != EPaperType::WrongPaper);
#ifdef _DEBUG
					log << s << "   ";
#endif // _DEBUG
				}
				break;

				// Граммаж бумаги по умолчанию
				case iDefaultWeight:
					if (s.isEmpty()) {
						state->defaultWeight = 0;
						break;
					}
					state->defaultWeight = s.toInt(&ok);
					Q_ASSERT(ok and state->defaultWeight >= 0);
#ifdef _DEBUG
					log << s << "   ";
#endif // _DEBUG
					break;

				case iLessA2:
					Q_ASSERT(s.isEmpty() or s.length() == 1);
					state->isLessA2 = (s == chPlus);
#ifdef _DEBUG
					log << "isLessA2: "
						<< (state->isLessA2 ? S("true") : S("false"))
						<< "   "
						<< endl;
#endif // _DEBUG
					break;

				// Флаги
				default:
					switch (s.length()) {
						case 1:
							if (s.at(0) != chPlus)
								ErrorInFile(sWrongParameter);
							break;

						case 2:
							if (s.at(0) != chPlus and s.at(1) != chAsteriks)
								ErrorInFile(sWrongParameter);

							if (CheckRange(i, iMinFormat, iMaxFormat)) {
								using enum EPaperFormat;
								defaultFormat =
									mapIndexPaperFormat.value(i, WrongFormat);
								if (*defaultFormat == WrongFormat)
									CRITICAL("Неверный формат по умолчанию");
								break;
							}

							if (CheckRange(i, iMinPortion, iMaxPortion)) {
								using enum EPortion;
								defaultPortion =
									mapIndexSheetPortion.value(i, WrongPortion);
								if (*defaultPortion == WrongPortion)
									CRITICAL("Неверная доля листа "
											 "по умолчанию");
								break;
							}

							if (CheckRange(i, iMinPress, iMaxPress)) {
								using enum EPress;
								defaultPress =
									mapIndexPress.value(i, WrongPress);
								if (*defaultPress == WrongPress)
									CRITICAL("Неверная печ. машина "
											 "по умолчанию");
								break;
							}

							break;

						default:
							if (!s.isEmpty())
								ErrorInFile(sWrongParameter, s);
							break;
					}

					state->flags[i] = !s.isEmpty();

#ifdef _DEBUG
					log << S("\t(%1) %2 ")
								.arg(EnumIndex(i), 2)
								.arg(enum_name(i).data(), -12)
						<< (state->flags[i] ? chPlus : QChar())
						<< endl;
#endif // _DEBUG
					break;
			}
		}

		Q_ASSERT(defaultFormat and defaultPortion and defaultPress);
		state->defaultFormat	= *defaultFormat;
		state->defaultPortion	= *defaultPortion;
		state->defaultPress		= *defaultPress;

		statesTable << state;
	}

#ifdef _DEBUG
	log.flush();
	logFile.close();

	auto CheckFlags = [] (EPaperType eType,
						  EIndexStates eIndex,
						  bool inverseCondition = false) -> bool
	{
		const auto it = std::ranges::find_if(statesTable, [eType] (auto st) {
			return st->paperType == eType;
		});
		if (it == statesTable.end())
			return false;
		const bool result = (*it)->flags[eIndex];
		return inverseCondition ? !result : result;
	};

	Q_ASSERT(CheckFlags(EPaperType::NewsPaper,	iA3				));
	Q_ASSERT(CheckFlags(EPaperType::Offset,		i80				));
	Q_ASSERT(CheckFlags(EPaperType::Offset,		i120			));
	Q_ASSERT(CheckFlags(EPaperType::Offset,		i160			));
	Q_ASSERT(CheckFlags(EPaperType::Offset,		i115,		true));
	Q_ASSERT(CheckFlags(EPaperType::Offset,		i150,		true));
	Q_ASSERT(CheckFlags(EPaperType::Offset,		i300,		true));

	Q_ASSERT(CheckFlags(EPaperType::ColorCopy,	iSRA3			));
	Q_ASSERT(CheckFlags(EPaperType::Monotex,	iSRA3			));
	Q_ASSERT(CheckFlags(EPaperType::Raflatac,	iSRA3			));
	Q_ASSERT(CheckFlags(EPaperType::Linen,		i330x487		));
	Q_ASSERT(CheckFlags(EPaperType::Shyne,		i330x487		));
	Q_ASSERT(CheckFlags(EPaperType::SirioPearl,	i330x487		));
	Q_ASSERT(CheckFlags(EPaperType::TouchCover,	i330x487		));

	Q_ASSERT(CheckFlags(EPaperType::ColorCopy,	i120			));
	Q_ASSERT(CheckFlags(EPaperType::ColorCopy,	i160			));
	Q_ASSERT(CheckFlags(EPaperType::ColorCopy,	i200			));
	Q_ASSERT(CheckFlags(EPaperType::ColorCopy,	i300			));
	Q_ASSERT(CheckFlags(EPaperType::ColorCopy,	i280,		true));
	Q_ASSERT(CheckFlags(EPaperType::ColorCopy,	i80,		true));
	Q_ASSERT(CheckFlags(EPaperType::Monotex,	i300,		true));
	Q_ASSERT(CheckFlags(EPaperType::Raflatac,	i300,		true));
	Q_ASSERT(CheckFlags(EPaperType::Linen,		i265,		true));
	Q_ASSERT(CheckFlags(EPaperType::Shyne,		i120,		true));
	Q_ASSERT(CheckFlags(EPaperType::SirioPearl,	i150,		true));
	Q_ASSERT(CheckFlags(EPaperType::TouchCover,	i270,		true));

	//QProcess::execute("notepad", { "log.txt" });

#endif // _DEBUG

	file.close();
}


void MainWindow::OpenFile(const QString & sOpenFileName) {
	if (!isActiveWindow())
		activateWindow();

	if (qApp->activeModalWidget()) {
		MsgBox mb(this);
		if (mb.YesNoPrompt(S("Закрыть все диалоговые окна и открыть файл?"))
			== MsgBox::Yes)
		{
			while (true) {
				auto activeDialog =
						qobject_cast<QDialog *>(qApp->activeModalWidget());
				if (activeDialog)
					activeDialog->reject();
				else
					break;
			}
		}
		if (!ConfirmSave())
			return;
	}

	auto Error = [&] (const QString errorDescription) -> void {
		MsgBox mb(this);
		mb.Error(S("Не удалось открыть файл\n«%2»\n(%3)").
				 arg(QDir::toNativeSeparators(sOpenFileName),
					 errorDescription));
	};

	Q_ASSERT(!sOpenFileName.isEmpty());
	QFile file(sOpenFileName);

	if (!file.exists()) {
		Error(S("файл не существует"));
		return;
	}

	if (!file.open(QIODevice::ReadOnly)) {
		Error(S("ошибка открытия файла"));
		return;
	}

	QDataStream stream(&file);
	if (stream.version() != CURRENT_QT_VERSION) {
		Error(S("неверная версия библиотеки"));
		return;
	}

	QString sFileVersion;
	stream >> sFileVersion;
	if (sFileVersion != sCurrentFileVersion) {
		Error(S("неверная версия файла"));
		return;
	}

	Order newOrder;
	stream >> newOrder;
	if (stream.status() != QDataStream::Ok) {
		Error(S("ошибка чтения данных [order]"));
		return;
	}

	Products newProducts;
	qint32 productsCount;
	stream >> productsCount;
	Q_ASSERT(productsCount > 0 and productsCount <= MAX_PRODUCTS);
	for (size_t i = 0; i < productsCount; ++i) {
		auto product = new Product;
		stream >> *product;
		if (stream.status() != QDataStream::Ok) {
			Error(S("ошибка чтения данных [product]"));
			return;
		}
		newProducts.Insert(product);
	}

	DeleteAllProducts();

	for (auto p : newProducts) {
		p->SetImpositionsOwner();
		p->CalcCost();
		InsertProduct(p, false);
	}

	Q_ASSERT(!products.isEmpty() and products.count() <= MAX_PRODUCTS);

	file.close();

	isFileExist = true;
	QFileInfo fi(sOpenFileName);
	sDocPath = QDir::toNativeSeparators(fi.absolutePath());
	if (!sDocPath.isEmpty())
		sDocPath += '\\';
	sDocName = order.IsFilled() ? order.GetName() : fi.baseName();
	SetModified(false);
	startOption = EStartOption::NormalOperation;
	ui->ProductsTree->topLevelItem(0)->setSelected(true);
}


MainWindow::~MainWindow() {
	delete ui;
}


bool MainWindow::event(QEvent * ev) {
	if (ev->type() == QEvent::WinIdChange)
		WriteHWND();
	return QWidget::event(ev);
}


void Cell::Print(QPainter & painter, QSizeF size, bool backgroundOnly) const {
	if (backgroundOnly and colorBackground == -1)
		return;

	Q_ASSERT(IsValid());
	Q_ASSERT(borderWidth >= 0.0);
	Q_ASSERT(*font != QGuiApplication::font());

	QRectF rectCell(0, 0,
					size.width() * PRINT_SCALE,
					size.height() * PRINT_SCALE);

	if (backgroundOnly) {
		painter.fillRect(rectCell, fromBlack(colorBackground));
		return;
	}

	if (!qFuzzyIsNull(borderWidth)) {
		painter.setPen(QPen(fromBlack(colorBorder),
							borderWidth * PT_PER_MM * PRINT_SCALE));
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(rectCell);
	}

	const auto margin = padding * PRINT_SCALE;
	painter.setFont(*font);
	painter.setPen(fromBlack(colorText));
	painter.save();
	if (rotated) {
		painter.rotate(-90);
		painter.translate(-rectCell.height(), 0);
	}
	painter.drawText(rectCell.marginsRemoved({ margin, margin,
											   margin, margin, }),
					 alignment, text);
	painter.restore();
}


Table::Table(ETableType tableType) :
	type(tableType), rows(0), columns(0), topIndent(0)
{
	Q_ASSERT(type == ETableType::ProductBody or
			 type == ETableType::ProductNotes or
			 type == ETableType::ImpPostpress or
			 type == ETableType::DummyNewPage);
}


Table::Table(ETableType tableType, size_t rows, size_t columns,
			 QList<qreal> & colWidths, QList<qreal> & rowHeights,
			 qreal topIndent, qreal borderWidth, qint8 borderColor) :
	type(tableType), rows(rows), columns(columns), topIndent(topIndent),
	borderWidth(borderWidth), borderColor(borderColor),
	colWidths(colWidths), rowHeights(rowHeights)
{
	Q_ASSERT(rows > 0 and columns > 0);
	Q_ASSERT(colWidths.count() == columns and rowHeights.count() == rows);
	Q_ASSERT(topIndent >= 0.0 and topIndent <= 50.0);
	Q_ASSERT(borderWidth >= 0.0 and borderWidth <= 3.0);
	Q_ASSERT(borderColor >= 0 and borderColor <= 100);

	cells.resize(rows);
	for (size_t row = 0; row < rows; ++row)
		cells[row].resize(columns);
}


qreal Table::GetHeight() const {
	auto height = 0.0;
	for (const auto rh : rowHeights)
		height += rh;
	return height + topIndent;
}


Table * Table::Split(qreal restHeight) {
	Q_ASSERT(restHeight > 10 and rows >= 4);
	size_t restCount = floor(restHeight / rowHeights.first());
	if (restCount < 2)
		restCount = 2;
	if (rows - restCount < 2)
		restCount = rows - 2;

	size_t highCount = rows - restCount;
	QList<qreal> highRowHeights(highCount);
	for (size_t i = restCount; i < rows; ++i)
		highRowHeights[i - restCount] = rowHeights[i];

	auto highTable = new Table(type, highCount, columns,
							   colWidths, highRowHeights,
							   topIndent, borderWidth, borderColor);

	for (size_t i = restCount; i < rows; ++i)
		highTable->cells[i - restCount] = cells[i];

	rows = restCount;
	rowHeights.resize(restCount);
	rowHeights.squeeze();
	cells.resize(restCount);
	cells.squeeze();

	return highTable;
}


void Table::Print(QPainter & painter, qreal yTop) const {

	auto PrintCells = [=, &painter] (bool backgroundOnly) -> void {
		qreal w, h;
		auto y = yTop + topIndent;
		for (size_t row = 0; row < rows; ++row) {
			const auto rowHeight = rowHeights[row];
			auto transform = QTransform::fromTranslate(0, y * PRINT_SCALE);
			painter.setTransform(transform);
			for (size_t col = 0; col < columns; ++col) {
				if (col)
					painter.translate(colWidths[col - 1] * PRINT_SCALE, 0);
				auto & c = cells[row][col];
				if (c.IsValid()) {
					w = c.colSpan ? 0.0 : colWidths[col];
					if (c.colSpan) {
						const auto lastCol = col + c.colSpan;
						for (size_t i = col; i < lastCol; ++i)
							w += colWidths[i];
					}

					h = c.rowSpan ? 0.0 : rowHeight;
					if (c.rowSpan)
						for (size_t i = row; i < c.rowSpan; ++i)
							h += rowHeights[i];

					c.Print(painter, { w, h }, backgroundOnly);
				}
			}
			y += rowHeights[row];
		}
	};

	Q_ASSERT(yTop >= 0.0 and yTop <= PRINT_HEIGHT_MAX);
	// Первый проход: печатаем фоны ячеек
	PrintCells(true);

	if (!qFuzzyIsNull(borderWidth)) {
		// Печатаем рамку таблицы
		qreal fullWidth = 0.0;
		for (const auto & w : colWidths)
			fullWidth += w;
		painter.setBrush(Qt::NoBrush);
		painter.setPen(QPen(fromBlack(borderColor),
							borderWidth * PRINT_SCALE));
		painter.setTransform(QTransform::fromTranslate(0, yTop * PRINT_SCALE));
		painter.drawRect(0, 0,
						 fullWidth * PRINT_SCALE,
						 GetHeight() * PRINT_SCALE);
	}

	// Второй проход: печатаем текст и обрамление
	if (type == ETableType::ProductNotes) {
		painter.save();
		auto transform = painter.transform();
		const auto scale = 12.5;
		transform.scale(scale, scale);
		painter.setTransform(transform);
		doc.setHtml(cells.first().first().text);
		doc.setTextWidth(NOTES_WIDTH);
		doc.drawContents(&painter,
						 { 0, 0, NOTES_WIDTH,
						   (rowHeights.first() / PT_PER_MM) / 0.75 });
		painter.restore();
	} else
		PrintCells(false);
}


void Table::SetCell(size_t row, size_t column,
					const Cell & cell,
					const QString & text, QFont * const font)
{
	auto & c = cells[row][column];
	c = cell;
	if (!text.isEmpty())
		c.text = text;
	if (font)
		c.font = font;
}


QColor fromBlack(qint8 percentBlack) {
	Q_ASSERT(percentBlack >= 0 and percentBlack <= 100);
	const int component = 255 - (255 * percentBlack) / 100;
	return QColor(component, component, component);
}


const QString & SelectWordEnding(size_t number,
								 const QString & word1,
								 const QString & word2_4,
								 const QString & word5_10)
{
	const auto & s = QString::number(number);
	const auto lastDigit = s.last(1).toInt();
	if ((number >= 11 and s.endsWith("11")) or
		(number >= 5 and (!lastDigit or (lastDigit >= 5 and lastDigit <= 9))))
		return word5_10;
	if (lastDigit >= 2 and lastDigit <= 4)
		return word2_4;
	return word1;
}


#include <Windows.h>

bool MainWindow::nativeEvent([[maybe_unused]] const QByteArray & eventType,
							 void * message,
							 qintptr * result)
{
	const auto msg = static_cast <MSG *> (message);
	if (msg->message != WM_COPYDATA)
		return false;
	const auto data = (COPYDATASTRUCT *) msg->lParam;
	OpenFile(QString::fromLocal8Bit((char *) data->lpData,
									(size_t) data->cbData));
	*result = TRUE;
	return true;
}


bool MainWindow::EditOrder() {
	Order oldOrder(order);
	OrderDlg orderDlg(this);
	if (orderDlg.exec() == QDialog::Accepted)
		return order != oldOrder;
	order = oldOrder;
	return false;
}


Product * MainWindow::FetchProductFromItem(QTreeWidgetItem * item,
										   bool strongCheck) const
{
	if (!item or (strongCheck and item->parent()))
		return nullptr;
	const auto toplevelItem = item->parent() ? item->parent() : item;
	const auto data = toplevelItem->data(0, Qt::UserRole).toULongLong(&ok);
	Q_ASSERT(ok and data);
	return (Product *) data;
}


QTreeWidgetItem * MainWindow::GetSelectedItem() const {
	const auto selItems = ui->ProductsTree->selectedItems();
	if (selItems.isEmpty())
		return nullptr;
	Q_ASSERT(selItems.count() == 1);
	const auto selItem = selItems.first();
	Q_ASSERT(selItem);
	return selItem;
}


Imposition * MainWindow::FetchImpFromItem(const QTreeWidgetItem * item) const {
	if (!item or !item->parent())
		return nullptr;
	const auto data = item->data(0, Qt::UserRole).toLongLong(&ok);
	Q_ASSERT(ok and data >= 0 and enum_contains<EImpType>(data));
	const auto product = FetchProductFromItem(item->parent());
	return product->impositions.GetImp(EnumCast<EImpType>(data));
}


void MainWindow::ShowProductInfo(const Product * product) {
	ui->PropertiesTree->clear();
	if (!product)
		return;

	const auto & p = *product;
	Q_ASSERT(!p.sName.isEmpty());
	Q_ASSERT(ui->ProductsTree->topLevelItemCount() > 0 and
			 ui->ProductsTree->selectedItems().count() == 1);
	QString s;
	//auto fontBold = ui->ProductsTree->topLevelItem(0)->font(0);
	//fontBold.setBold(true);

	const auto groupProduct = new QTreeWidgetItem(ui->PropertiesTree);
	groupProduct->setText(0, S("Параметры изделия"));
	groupProduct->setExpanded(true);

	NewItem(groupProduct,
			S("Общий тираж"),
			S("%1 экз.").arg(t(p.kinds.GetCopies())),
			true);

	const auto & sSize = S("%1 x %2 мм")
							.arg(p.size.GetWidth())
							.arg(p.size.GetHeight());
	const auto eps = p.size.GetESize();
	s = eps == EProductSize::CustomSize ?
				sSize :
				S("%1 (%2)").arg(stdProductSizes.GetName(eps), sSize);
	NewItem(groupProduct, S("Размер"), s, true);

	if (!p.IsSimple()) {
		QTreeWidgetItem * groupBinding =
			new QTreeWidgetItem(ui->PropertiesTree);
		groupBinding->setText(0, S("Переплет"));
		groupBinding->setExpanded(true);

		const auto & sName = stdBindingNames[p.eBinding];
		NewItem(groupBinding, S("Тип"), sName, true);

		if (p.eBinding == EBinding::Wire or
			p.eBinding == EBinding::Bolts or
			p.eBinding == EBinding::Rings) {
			if (p.eBinding != EBinding::Wire)
				NewItem(groupBinding, S("Количество"), t(p.nBindCount));
			NewItem(groupBinding,
					S("Диаметр"),
					S("%1 мм").arg(t(p.bindDiam)));
			NewItem(groupBinding, S("Цвет"), p.bindColor.ColorName());
		}

		const auto Pages = [] (size_t pages) -> const QString {
			Q_ASSERT(pages > 0);
			return S("%1 стр.").arg(pages);
		};

		if (p.nPagesCover)
			NewItem(groupBinding, S("Обложка"),		Pages(p.nPagesCover));
		if (p.nPagesBottom)
			NewItem(groupBinding, S("Подложка"),	Pages(p.nPagesBottom));
		if (p.nPagesBlock)
			NewItem(groupBinding, S("Блок"),		Pages(p.nPagesBlock));
		if (p.nPagesInsert)
			NewItem(groupBinding, S("Вставка"),		Pages(p.nPagesInsert));
		if (p.bCurcor)
			NewItem(groupBinding, S("Курсор"),	p.cursorColor.ColorName());
		if (p.bRigel)
			NewItem(groupBinding, S("Ригель"),	p.rigelColor.ColorName());
	}

	const auto groupCost = new QTreeWidgetItem(ui->PropertiesTree);
	groupCost->setText(0, S("Стоимость"));
	groupCost->setExpanded(true);
	const auto productCost = p.GetCost();
	const auto priceOneCopy = productCost / qreal(p.kinds.GetCopies());
	NewItem(groupCost,
			S("Итого за изделие"),
			S("%1 р. (%2 р./экз.)").arg(t(productCost), t(priceOneCopy, 2)));
}


void MainWindow::ShowImpositionInfo(const Imposition * imposition) {
	ui->PropertiesTree->clear();
	if (!imposition)
		return;

	Q_ASSERT(ui->ProductsTree->topLevelItemCount() > 0 and
			 ui->ProductsTree->selectedItems().count() == 1);

	QString s;
	//auto fontBold = ui->ProductsTree->topLevelItem(0)->font(0);
	//fontBold.setBold(true);
	
	const auto & imp = *imposition;
	const auto & paper = imp.paper;
	const auto & pp = imp.postpress;

	const auto groupPaper = new QTreeWidgetItem(ui->PropertiesTree);
	groupPaper->setText(0, S("Бумага"));
	groupPaper->setExpanded(true);

	s = paper.eType == EPaperType::CustomPaper ?
							paper.sCustomName :
							stdPaperTypes.GetName(paper.eType);
	NewItem(groupPaper, S("Тип"), s, true);

	if (paper.format.IsCustom())
		s = S("%1x%2 мм (нестанд.)").
			arg(paper.format.GetWidth()).
			arg(paper.format.GetHeight());
	else
		s = stdPaperFormats.GetName(paper.format.eFormat);

	NewItem(groupPaper, S("Формат листа"), s);
	NewItem(groupPaper,
			S("Доля листа"),
			stdPortionNames[paper.ePortion]);
	if (paper.weight > 0)
		NewItem(groupPaper,
				S("Граммаж"),
				S("%1 г/м²").arg(paper.weight),
				true);

	//for (int i = 0; i < groupPaper->childCount(); i++)
	//	groupPaper->child(i)->setFont(1, fontBold);

	const auto groupPrint = new QTreeWidgetItem(ui->PropertiesTree);
	groupPrint->setText(0, S("Печать"));
	groupPrint->setExpanded(true);

	s = (imp.IsNoPrint()) ?
			S("без печати") : S("%1+%2").arg(imp.inks.face).arg(imp.inks.back);
	NewItem(groupPrint, S("Красочность"), s, true);

	if (!imp.IsNoPrint())
		NewItem(groupPrint,
				S("Печатная машина"),
				pressNames.value(imp.ePress));
	NewItem(groupPrint, S("Мест на печ. листе"), t(imp.GetTotalPlaces()));
	NewItem(groupPrint,
			S("Печатных листов"),
			t(imp.signatures.printSheets),
			true);

	//for (int i = 0; i < groupPrint->childCount(); i++)
	//	groupPrint->child(i)->setFont(1, fontBold);

	const auto groupPostpress = new QTreeWidgetItem(ui->PropertiesTree);
	groupPostpress->setText(0, S("Постпечать"));
	groupPostpress->setExpanded(true);

	if (pp.lamination) {
		s = sFace;
		if (pp.lamination->bBack)
			s += S(" + %1").arg(sBack);
		NewItem(groupPostpress, S("Ламинация"), s);
	}

	if (pp.foil) {
		s = sFace;
		if (pp.foil->back)
			s += S(" + %1").arg(sBack);
		NewItem(groupPostpress, S("Фольгирование"), s);
	}

	if (pp.creasings) {
		s = S("%1 %2").
			arg(*pp.creasings).
			arg(SelectWordEnding(*pp.creasings, биг, бига, бигов));
		NewItem(groupPostpress, S("Биговка"), s);
	}

	if (pp.foldings) {
		s = S("%1 %2").
			arg(pp.foldings->count).
			arg(SelectWordEnding(pp.foldings->count, фальц, фальца, фальцев));
		NewItem(groupPostpress, S("Фальцовка"), s);
	}

	if (pp.perforations)
		NewItem(groupPostpress,
				S("Отрывная перфорация"),
				S("%1 перф.").arg(*pp.perforations));
	if (pp.gluePoints)
		NewItem(groupPostpress,
				S("Клеевая точка"),
				S("%1 шт.").arg(*pp.gluePoints));
	if (pp.dblSidedTape)
		NewItem(groupPostpress,
				S("Двусторонний скотч"),
				S("%1 шт.").arg(pp.dblSidedTape->count));
	if (pp.scrathStrips)
		NewItem(groupPostpress,
				S("Скретч-полоса"),
				S("%1 шт.").arg(*pp.scrathStrips));
	if (pp.rndCorners)
		NewItem(groupPostpress,
				S("Скругление углов"),
				S("%1 шт., радиус %2 мм").
				arg(pp.rndCorners->count).
				arg(pp.rndCorners->radius));
	if (pp.eyelets)
		NewItem(groupPostpress,
				S("Люверсы"),
				S("%1 шт., диам. %2 мм").
				arg(pp.eyelets->count).
				arg(pp.eyelets->diam));
	if (pp.holes)
		NewItem(groupPostpress,
				S("Отверстия"),
				S("%1 шт., диам. %2 мм").
				arg(pp.holes->count).
				arg(pp.holes->diam));
	if (pp.nums)
		NewItem(groupPostpress,
				S("Нумерация"),
				S("от %1 до %2").
				arg(pp.nums->start).
				arg(pp.nums->end));
	if (pp.dieCutting)
		NewItem(groupPostpress, S("Высечка"), S("Да"));
	if (!groupPostpress->childCount())
	//	for (qsizetype i = 0; i < groupPostpress->childCount(); i++)
	//		groupPostpress->child(i)->setFont(1, fontBold);
	//else
		NewItem(groupPostpress, S("(нет)"), { });

	const auto groupCost = new QTreeWidgetItem(ui->PropertiesTree);
	groupCost->setText(0, S("Стоимость"));
	groupCost->setExpanded(true);
	NewItem(groupCost,
			S("Итого спуск"),
			S("%1 руб.").arg(t(imp.GetCost())));
}
