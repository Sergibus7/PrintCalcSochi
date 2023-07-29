#ifdef _DEBUG
#include <QKeyEvent>
#endif // _DEBUG
#include "Globals.h"
#include "MsgBox.h"
#include "ImpWizard.h"
#include "DeleteImpDlg.h"
#include "ProductKindsDlg.h"
#include "SelectImpDlg.h"
#include "EditNotesDlg.h"
#include "ProductDlg.h"
#include "ui_ProductDlg.h"


ProductDlg::ProductDlg(Product * prod, QWidget * parent) :
	QDialog(parent),
	ui(new Ui::ProductDlg),
	p(prod)
{
	Q_ASSERT(p);
	imps = &p->impositions;
	kinds = &p->kinds;

	ui->setupUi(this);

	auto sorted = simpleProductsNames;
	sorted.sort();
	ui->comboSimpleProduct->addItems(sorted);

	auto multipageProducts = mapProductKind.keys();
	multipageProducts.sort();
	for (const auto & name : multipageProducts)
		ui->comboMultipageProduct->addItem(name,
										   enum_integer(mapProductKind[name]));

	FillStdColorNames({ ui->comboBindingColor,
						ui->comboCursorColor,
						ui->comboRigelColor, });

	using enum EBinding;
	ui->bgBinding->setId(ui->rbNoBinding,	enum_integer(NoBinding));
	ui->bgBinding->setId(ui->rbWire,		enum_integer(Wire));
	ui->bgBinding->setId(ui->rbBolts,		enum_integer(Bolts));
	ui->bgBinding->setId(ui->rbRings,		enum_integer(Rings));
	ui->bgBinding->setId(ui->rbStaples,		enum_integer(Staples));
	ui->bgBinding->setId(ui->rbSoftCover,	enum_integer(SoftCover));

	connect(this, &QDialog::rejected, [=] {
		Q_ASSERT(logWindow);
		if (!logWindow->isHidden())
			logWindow->close();
		//isModified = false;
	});

	connect(ui->pbCancel, &QAbstractButton::clicked, this, [=] {
		//if (!imps->isEmpty()) {
		//	MsgBox mb(this);
		//	if (mb.YesNoPrompt("В данном изделии присутствуют спуски.\n"
		//					   "Вы уверены, что хотите выйти?\n")
		//			== MsgBox::No)
		//		return;
		//}
		reject();
	});

	connect(ui->pbOK, &QAbstractButton::clicked, this, [=] {
		MsgBox mb(this);
		if (imps->isEmpty()) {
			if (p->IsSimple())
				mb.Error("Спуск не создан.\n"
						 "Необходимо создать спуск, чтобы продолжить!\n");
			else
				mb.Error("Спуски не созданы.\n"
						 "Необходимо создать все спуски изделия, "
						 "чтобы продолжить!\n");
			return;
		}

		if (!p->IsSimple()) {
			if (!ui->sbPagesCover->value() and !ui->sbPagesBlock->value() and
				!ui->sbPagesBottom->value() and !ui->sbPagesInsert->value())
			{
				mb.Error("Многостраничное изделие не может\n"
						 "иметь нулевое кол-во страниц!\n");
				return;
			}

			if (ui->sbPagesCover->value() and
				!imps->contains(EImpType::Cover))
			{
				mb.Error("Спуск обложки не создан!\n");
				return;
			}

			if (ui->sbPagesBlock->value() and
				!imps->contains(EImpType::Block))
			{
				mb.Error("Спуск блока не создан!\n");
				return;
			}

			if (ui->sbPagesBottom->value() and
				!imps->contains(EImpType::Bottom))
			{
				mb.Error("Спуск подложки не создан!\n");
				return;
			}

			if (ui->sbPagesInsert->value() and
				!imps->contains(EImpType::Insert))
			{
				mb.Error("Спуск вставки не создан!\n");
				return;
			}

			if (ui->cbCursor->isChecked() and
				ui->comboCursorColor->currentText().isEmpty())
			{
				mb.Error("Не выбран цвет курсора!");
				return;
			}

			if (ui->cbRigel->isChecked() and
				ui->comboRigelColor->currentText().isEmpty())
			{
				mb.Error("Не выбран цвет ригеля!");
				return;
			}

			if (qFuzzyIsNull(ui->sbBindPrice->value()) and
				(p->eBinding == EBinding::Bolts or
				 p->eBinding == EBinding::Rings or
				 p->eBinding == EBinding::Wire))
			{
				mb.Error("Не указана цена сборки!");
				ui->sbBindPrice->selectAll();
				ui->sbBindPrice->setFocus();
				return;
			}
		}

		if (ui->rbStandardSize->isChecked()) {
			const auto id = ui->bgStdProductSize->checkedId();
			Q_ASSERT(id != -1);
			p->size.eSize = EnumCast<EProductSize>(id);
		}
		p->bindColor = GetColorFromCombo(ui->comboBindingColor);
		p->cursorColor = GetColorFromCombo(ui->comboCursorColor);
		p->rigelColor = GetColorFromCombo(ui->comboRigelColor);
		p->nBindCount = ui->sbBindingCount->value();
		p->bindDiam = ui->sbBindingDiameter->value();

		accept();
	});

	auto SelectImposition = [=] (bool addImp) -> Opt<EImpType> {
		if (p->IsSimple())
			return EImpType::Simple;
		else {
			SelectImpDlg selectImpDlg(*p, addImp, this);
			if (selectImpDlg.exec() == QDialog::Accepted)
				return selectImpDlg.selImpType;
		}
		return std::nullopt;
	};

	connect(ui->pbAddImp, &QAbstractButton::clicked, this, [=] {
		const auto optImpType = SelectImposition(true);
		if (optImpType and ExecEditImposition(p, *optImpType, true, this)) {
			EnableImpButtons();
			RecalcCost();
		}
	});

	connect(ui->pbEditImp, &QAbstractButton::clicked, this, [=] {
		const auto optImpType = SelectImposition(false);
		if (optImpType and ExecEditImposition(p, *optImpType, false, this))
			RecalcCost();
	});

	connect(ui->pbDeleteImp, &QAbstractButton::clicked, this, [=] {
		Q_ASSERT(!imps->isEmpty());
		DeleteImpDlg deleteImpDlg(*imps, this);
		if (deleteImpDlg.exec() == QDialog::Accepted) {
			EnableImpButtons();
			RecalcCost();
		}
	});

	ui->bgProductType->setId(ui->rbSimpleProduct,		int(false));
	ui->bgProductType->setId(ui->rbMultipageProduct,	int(true));

	connect(ui->bgProductType, &QButtonGroup::idToggled, this,
			[=] (int id, bool checked) {
		if (!checked)
			return;

		const bool isMultipage = id;
		ui->gbPagesCount->setEnabled(isMultipage);
		ui->gbBinding->setEnabled(isMultipage);
		ui->comboMultipageProduct->setEnabled(isMultipage);
		ui->comboSimpleProduct->setEnabled(!isMultipage);
		ui->pbListKinds->setEnabled(!isMultipage);

		if (!canChange) {
			CalcTotalPages();
			return;
		}

		if (isMultipage) {
			const auto data = GetDataFromCombobox(ui->comboMultipageProduct);
			p->eKind = EnumCast<EProductKind>(data);
			Q_ASSERT(!p->IsSimple());
			p->sName = ui->comboMultipageProduct->currentText();
			CheckNotepad();
			SetDefaultBinding();
			if (!p->kinds.IsOnlyOneKind()) {
				p->kinds.resize(1);
				p->kinds.squeeze();
				ChangeCopies();
				MsgBox mb(this);
				mb.Info("Для многостраничной продукции допустим\n"
						"только один вид!");
			}
		} else {
			p->eKind = EProductKind::Simple;
			p->eBinding = EBinding::NoBinding;
			p->sName = ui->comboSimpleProduct->currentText();
		}

		DeleteAllImpositions();
		SetDefaultPages();
		CheckCalendar();
		EnableImpButtons();
		//UpdateProductSize();
		//CheckAndToggleConverts();
		RecalcCost();
		//isModified = true;
	});

	connect(ui->comboSimpleProduct,
			qOverload<int>(&QComboBox::currentIndexChanged), this, [=] {
		if (!canChange)
			return;
		p->sName = ui->comboSimpleProduct->currentText();
		//CheckAndToggleConverts();
		RecalcCost();
		//isModified = true;
	});

	connect(ui->comboMultipageProduct, &QComboBox::currentTextChanged, this,
			[=] (const QString & text) {
		if (!canChange or text == p->sName)
			return;
		DeleteAllImpositions();
		p->sName = text;
		p->eKind = mapProductKind[text];
		Q_ASSERT(!p->IsSimple());
		CheckNotepad();
		SetDefaultBinding();
		CheckCalendar();
		SetDefaultPages();
		RecalcCost();
		//isModified = true;
	});

//	auto SetTotalPages = [=] () -> void {
//		auto totalPages = ui->sbPagesCover->value() +
//						  ui->sbPagesBlock->value() +
//						  ui->sbPagesBottom->value() +
//						  ui->sbPagesInsert->value();
//		ui->lblTotalPages->setText(S("%1 стр.").arg(totalPages));
//	};

	const auto SetPages = [=] (EImpType impType) -> void {
		DeleteImposition(impType);
		CheckAndSetPagesCount(impType);
		RecalcCost();
		CalcTotalPages();
		//isModified = true;
	};

	connect(ui->sbPagesCover, &QSpinBox::editingFinished, this, [=] {
		if (!canChange)
			return;
		SetPages(EImpType::Cover);
	});

	connect(ui->sbPagesBlock, &QSpinBox::editingFinished, this, [=] {
		if (!canChange)
			return;
		SetPages(EImpType::Block);
	});

	connect(ui->sbPagesBottom, &QSpinBox::editingFinished, this, [=] {
		if (!canChange)
			return;
		SetPages(EImpType::Bottom);
	});

	connect(ui->sbPagesInsert, &QSpinBox::editingFinished, this, [=] {
		if (!canChange)
			return;
		SetPages(EImpType::Insert);
	});

	connect(ui->sbPagesCover, &QSpinBox::valueChanged, this, [=] {
		CalcTotalPages();
	});

	connect(ui->sbPagesBlock, &QSpinBox::valueChanged, this, [=] {
		CalcTotalPages();
	});

	connect(ui->sbPagesBottom, &QSpinBox::valueChanged, this, [=] {
		CalcTotalPages();
	});

	connect(ui->sbPagesInsert, &QSpinBox::valueChanged, this, [=] {
		CalcTotalPages();
	});

	connect(ui->cbIdenticalPages, &QCheckBox::toggled, this, [=] (bool checked) {
		if (!canChange)
			return;
		p->bIdenticalPages = checked;
		RecalcCost();
		CalcTotalPages();
		//isModified = true;
	});

	connect(ui->bgBinding, &QButtonGroup::idClicked, this, [=] (int id) {
		if (!canChange)
			return;
		SetBindings(EnumCast<EBinding>(id));
		//isModified = true;
	});

	connect(ui->cbBindingLongSide, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		if (!canChange or p->bLongSideBind == checked)
			return;
		p->bLongSideBind = checked;
		DeleteAllImpositions();
		EnableImpButtons();
		RecalcCost();
		//isModified = true;
	});

	connect(ui->sbBindingDiameter,
			QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
			[=] (double diameter)
	{
		if (!canChange or qFuzzyCompare(p->bindDiam, diameter))
			return;
		p->bindDiam = diameter;
		RecalcCost();
		//isModified = true;
	});

	connect(ui->sbBindPrice,
			QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
			[=] (double price)
	{
		if (!canChange or qFuzzyCompare(p->bindPrice, price))
			return;
		p->bUserBindPrice = true;
		p->bindPrice = price;
		RecalcCost();
		//isModified = true;
	});

	connect(ui->sbBindingCount, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int nCount) {
		if (!canChange or p->nBindCount == nCount)
			return;
		p->nBindCount = nCount;
		RecalcCost();
		//isModified = true;
	});

	connect(ui->cbCursor, &QAbstractButton::toggled, this, [=] (bool checked) {
		ui->comboCursorColor->setEnabled(checked);
		if (!canChange)
			return;
		p->bCurcor = checked;
		RecalcCost();
		//isModified = true;
	});

	connect(ui->comboCursorColor, &QComboBox::currentTextChanged, this,
			[=] ([[maybe_unused]] const QString& text)
	{
		if (!canChange)
			return;
		p->cursorColor = GetColorFromCombo(ui->comboCursorColor);
		//isModified = true;
	});

	connect(ui->cbRigel, &QAbstractButton::toggled, this, [=] (bool checked) {
		ui->comboRigelColor->setEnabled(checked);
		if (!canChange)
			return;
		p->bRigel = checked;
		RecalcCost();
		//isModified = true;
	});

	connect(ui->comboRigelColor, &QComboBox::currentTextChanged, this,
			[=] ([[maybe_unused]] const QString & text) {
		if (!canChange)
			return;
		p->rigelColor = GetColorFromCombo(ui->comboRigelColor);
		//isModified = true;
	});


	////////////////////////////////// Размер /////////////////////////////////

	using enum EProductSize;
	ui->bgStdProductSize->setId(ui->rbA7,		enum_integer(SizeA7)	);
	ui->bgStdProductSize->setId(ui->rbA6,		enum_integer(SizeA6)	);
	ui->bgStdProductSize->setId(ui->rbA5,		enum_integer(SizeA5)	);
	ui->bgStdProductSize->setId(ui->rbA4,		enum_integer(SizeA4)	);
	ui->bgStdProductSize->setId(ui->rbA3,		enum_integer(SizeA3)	);
	ui->bgStdProductSize->setId(ui->rbA2,		enum_integer(SizeA2)	);
	ui->bgStdProductSize->setId(ui->rb210x98,	enum_integer(Size210x98));
	ui->bgStdProductSize->setId(ui->rb90x50,	enum_integer(Size90x50)	);
	//ui->bgStdProductSize->setId(ui->rbC4,		enum_integer(SizeC4)	);
	//ui->bgStdProductSize->setId(ui->rbC5,		enum_integer(SizeC5)	);
	//ui->bgStdProductSize->setId(ui->rbC6,		enum_integer(SizeC6)	);
	//ui->bgStdProductSize->setId(ui->rbE65,		enum_integer(SizeE65)	);

#ifdef _DEBUG
	for (const auto btn : ui->bgStdProductSize->buttons())
		Q_ASSERT(ui->bgStdProductSize->id(btn) != -1);
#endif // _DEBUG

	connect(ui->bgStdProductSize, &QButtonGroup::idClicked, this,
			[=] (int id) {
		if (!canChange)
			return;
		const auto eSize = EnumCast<EProductSize>(id);
		if (eSize != p->size.GetESize()) {
			p->size.eSize = eSize;
			UpdateProductSize();
			RecalcCost();
		}
	});

	connect(ui->rbCustomSize, &QAbstractButton::toggled, this,
			[=] (bool checked) {
		ui->sbProductHeight->setEnabled(checked);
		ui->sbProductWidth->setEnabled(checked);
		ui->lblWidth->setEnabled(checked);
		ui->lblHeight->setEnabled(checked);
		EnableButtonGroup(ui->bgStdProductSize, !checked);
		//CheckAndToggleConverts();
		if (checked) {
			p->size.SetSize(EProductSize::CustomSize,
							ui->sbProductWidth->value(),
							ui->sbProductHeight->value());
			RecalcCost();
			ui->sbProductWidth->setFocus();
			ui->sbProductWidth->selectAll();
			//isModified = true;
		}
	});

//	connect(ui->sbProductWidth,
//			qOverload<int>(&QSpinBox::valueChanged), this, [=] (int width) {
//		if (!canChange or p->size.width == width)
//			return;
//		p->size.width = width;
////		UpdateProductSize();
//		RecalcCost();
//		//isModified = true;
//	});

	ui->sbProductWidth	->setMaximum(MAX_PRODUCT_SIZE);
	ui->sbProductHeight	->setMaximum(MAX_PRODUCT_SIZE);

	connect(ui->sbProductWidth, &QSpinBox::editingFinished, this, [=] {
		if (!canChange)
			return;
		const decltype(p->size.width) w = ui->sbProductWidth->value();
		if (p->size.width != w) {
			p->size.width = w;
			RecalcCost();
		}
	});

//	connect(ui->sbProductHeight,
//			qOverload<int>(&QSpinBox::valueChanged), this, [=] (int height) {
//		if (!canChange)
//			return;
//		p->size.SetHeight(height);
////		UpdateProductSize();
//		RecalcCost();
//		//isModified = true;
//	});
//

	connect(ui->sbProductHeight, &QSpinBox::editingFinished, this, [=] {
		if (!canChange)
			return;
		const decltype(p->size.height) h = ui->sbProductHeight->value();
		if (p->size.height != h) {
			p->size.height = h;
			RecalcCost();
		}
	});

	//////////////////////////// Тираж и упаковка /////////////////////////////

	connect(ui->sbCopies, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int newCopies) {
		if (!canChange)
			return;
		Q_ASSERT(newCopies >= 1 and kinds->IsOnlyOneKind());
		if (kinds->GetCopies() == size_t(newCopies))
			return;
		ChangeCopies(newCopies);
		RecalcCost();
		//isModified = true;
	});

	connect(ui->pbListKinds, &QAbstractButton::clicked, this, [=]  {
		ProductKindsDlg productKindsDlg(*p, this);
		productKindsDlg.kinds = *kinds;
		if (productKindsDlg.exec() != QDialog::Accepted)
			return;
		*kinds = productKindsDlg.kinds;
		Q_ASSERT(kinds->count() >= 1);
		ChangeCopies();
		RecalcCost();
		//isModified = true;
	});

	connect(ui->sbPacks, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int newPacks) {
		if (!canChange)
			return;
		Q_ASSERT(kinds->IsOnlyOneKind());
		kinds->first().packs = newPacks;
		SetValueSilent(ui->sbPacksEvery, int(kinds->GetEvery()));
		//isModified = true;
	});

	connect(ui->sbPacksEvery, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int newEvery) {
		if (!canChange)
			return;
		Q_ASSERT(kinds->IsOnlyOneKind());
		auto & packs = kinds->first().packs;
		packs = Ceil(kinds->GetCopies() / qreal(newEvery));
		SetValueSilent(ui->sbPacks, packs);
		//isModified = true;
	});

	connect(ui->pbEditNotes, &QAbstractButton::clicked, this, [=] {
		EditNotesDlg editNotesDlg(this);
		editNotesDlg.html = p->notes;
		if (editNotesDlg.exec() != QDialog::Accepted)
			return;
		p->notes = editNotesDlg.html;
		CheckNotes();
		//isModified = true;
	});

	connect(ui->pbDeleteNotes, &QAbstractButton::clicked, this, [=] {
		Q_ASSERT(!p->notes.isEmpty());
		MsgBox mb(this);
		if (mb.YesNoPrompt("Действительно удалить примечания?") !=
				MsgBox::Yes)
			return;
		p->notes.clear();
		CheckNotes();
		//isModified = true;
	});
}


void ProductDlg::SetBindings(EBinding binding, bool setByDefault) {
	if (setByDefault)
		p->eBinding = binding;

	using enum EBinding;
	const bool isNoBinding	= binding == NoBinding;
	const bool isSoftCover	= binding == SoftCover;
	const bool isStaples	= binding == Staples;
	const bool isWire		= binding == Wire;
	const bool isRings		= binding == Rings;
	const bool isBolts		= binding == Bolts;

	bool oldProcess = canChange;
	canChange = false;
	ui->bgBinding->button(Id(binding))->setChecked(true);
	canChange = oldProcess;

	ui->cbBindingLongSide->setEnabled(!isNoBinding);
	ui->sbPagesBottom->setEnabled(!isSoftCover and !isStaples);
	if ((isSoftCover or isStaples) and setByDefault) {
		p->nPagesBottom = 0;
		ui->sbPagesBottom->setValue(0);
	}
	ui->comboBindingColor->setEnabled(isBolts or isRings or isWire);
	ui->sbBindingDiameter->setEnabled(isBolts or isRings or isWire);
	ui->sbBindingCount->setEnabled(!isNoBinding and
								   !isSoftCover and
								   !isStaples);

	if (setByDefault) {
		p->nBindCount = p->eKind == EProductKind::QuartCalend ? 3 : 1;
		SetValueSilent(ui->sbBindingCount, p->nBindCount);
	} else {
		ui->sbBindingCount->setValue(p->nBindCount);
		ui->sbBindingDiameter->setValue(p->bindDiam);
		ui->cbBindingLongSide->setChecked(p->bLongSideBind);
		if (isBolts or isRings or isWire or isSoftCover)
			ui->sbBindPrice->setValue(p->bindPrice);
	}

	SetStepForPages();
	if (setByDefault)
		RecalcCost();
}


void ProductDlg::SetDefaultBinding() {
	Q_ASSERT(!p->IsSimple());
	switch (p->eKind) {
		case EProductKind::QuartCalend:
		case EProductKind::WallCalend:
		case EProductKind::Notepad:
			SetBindings(EBinding::Wire);
			break;

		case EProductKind::Book:
			SetBindings(EBinding::SoftCover);
			break;

		case EProductKind::Brochure:
			SetBindings(EBinding::Staples);
			break;

		default:
			SetBindings(EBinding::NoBinding);
			break;
	}
}


void ProductDlg::ChangeCopies(size_t newCopies) {
	Q_ASSERT(newCopies == WRONG or
			 (newCopies != WRONG and newCopies < INT_MAX));
	Q_ASSERT(kinds->GetCopies()	< INT_MAX);
	Q_ASSERT(kinds->GetPacks()	< INT_MAX);
	Q_ASSERT(kinds->GetEvery()	< INT_MAX);

	const auto copies =
		int(newCopies == WRONG ? kinds->GetCopies() : newCopies);
	int packs = int(kinds->GetPacks());
	if (packs > copies) {
		Q_ASSERT(kinds->IsOnlyOneKind());
		kinds->first().packs = packs = copies;
	}

	const auto oldProcess = canChange;
	canChange = false;

	ui->sbPacks->setMaximum(copies);
	ui->sbPacksEvery->setMaximum(copies);
	const auto oneKind = kinds->IsOnlyOneKind();
	ui->sbPacksEvery->setMinimum(oneKind ? 1 : 0);
	ui->sbPacksEvery->setSpecialValueText(oneKind ? QString() :
													S("(неприменимо)"));
	ui->sbCopies->setEnabled(oneKind);
	ui->sbPacks->setEnabled(oneKind);
	ui->sbPacksEvery->setEnabled(oneKind);

	if (newCopies != WRONG)
		kinds->first().copies = copies;
	if (ui->sbCopies->value() != copies)
		ui->sbCopies->setValue(copies);

	if (ui->sbPacks->value() != packs)
		ui->sbPacks->setValue(packs);

	const int every = int(kinds->GetEvery());
	if (ui->sbPacksEvery->value() != every)
		ui->sbPacksEvery->setValue(every);

	canChange = oldProcess;
}


void ProductDlg::CheckNotes() {
	const auto isNotesEmpty = p->notes.isEmpty();
	ui->pbDeleteNotes->setEnabled(!isNotesEmpty);
	ui->pbEditNotes->setText(isNotesEmpty ? sAddNotes : sEditNotes);
}


void ProductDlg::CheckNotepad() {
	const bool isNotepad =
			mapProductKind[p->sName] == EProductKind::Notepad;
	ui->cbIdenticalPages->setEnabled(isNotepad);
	ui->cbIdenticalPages->setChecked(isNotepad);
}


void ProductDlg::CalcTotalPages([[maybe_unused]] bool reflectToProduct) {
//	if (reflectToProduct) {
//		if (p->nPagesCover != ui->sbPagesCover->value())
//			p->nPagesCover = ui->sbPagesCover->value();
//		if (p->nPagesBlock != ui->sbPagesBlock->value())
//			p->nPagesBlock = ui->sbPagesBlock->value();
//		if (p->nPagesBottom != ui->sbPagesBottom->value())
//			p->nPagesBottom = ui->sbPagesBottom->value();
//		if (p->nPagesInsert != ui->sbPagesInsert->value())
//			p->nPagesInsert = ui->sbPagesInsert->value();
//	}
	const auto totalPages =
			p->nPagesCover + p->nPagesBlock +
			p->nPagesBottom + p->nPagesInsert;
	ui->lblTotalPages->setText(S("%1 стр.").arg(totalPages));
}


void ProductDlg::SetStepForPages() {
	const int step = p->eBinding == EBinding::Staples ? 4 : 2;
	ui->sbPagesCover	->setSingleStep(step);
	ui->sbPagesBlock	->setSingleStep(step);
	ui->sbPagesBottom	->setSingleStep(step);
	ui->sbPagesInsert	->setSingleStep(step);
}


void ProductDlg::SetValueSilent(QSpinBox * sb, size_t value) {
	Q_ASSERT(value < INT_MAX);
	bool oldProcess = canChange;
	canChange = false;
	sb->setValue(int(value));
	canChange = oldProcess;
}


void ProductDlg::RecalcCost() {
	p->CalcCost();
	logWindow->UpdateTitle(S("изделия: ") + p->sName);
	logWindow->ShowText(p->GetLog());
	if (!p->IsSimple()) {
		ui->sbBindPrice->setEnabled(p->bUserBindPrice);
		ui->sbBindPrice->setValue(p->bindPrice);
	}
}


int ProductDlg::exec() {
	if (!logWindow)
		logWindow = new LogWindow(this);
	logWindow->show();
	logWindow->Pin(this, true);

	canChange = false;

	ui->bgProductType->button(!p->IsSimple())->setChecked(true);
	EnableImpButtons();
	if (p->sName.isEmpty()) {
		Q_ASSERT(p->IsSimple());
		p->sName = sDefaultProductsName;
	}
	auto * cb = p->IsSimple() ? ui->comboSimpleProduct :
										 ui->comboMultipageProduct;
	cb->setCurrentText(p->sName);
	cb->setFocus();

	const auto eSize = p->size.GetESize();
	if (eSize == EProductSize::CustomSize)
		ui->rbCustomSize->setChecked(true);
	else
		SetBtnChecked(ui->bgStdProductSize, enum_integer(eSize));

	const auto & size = p->size.GetSize();
	ui->sbProductWidth->setValue(size.width());
	ui->sbProductHeight->setValue(size.height());

	if (kinds->isEmpty())
		*kinds << ProductKind();
	ChangeCopies();

	if (!p->IsSimple()) {
		ui->sbPagesCover->setValue(p->nPagesCover);
		ui->sbPagesBlock->setValue(p->nPagesBlock);
		ui->sbPagesBottom->setValue(p->nPagesBottom);
		ui->sbPagesInsert->setValue(p->nPagesInsert);
		SetBindings(p->eBinding, false);
	}

	SetColorToCombo(ui->comboBindingColor, p->bindColor);
	SetColorToCombo(ui->comboCursorColor, p->cursorColor);
	SetColorToCombo(ui->comboRigelColor, p->rigelColor);
	ui->cbCursor->setChecked(p->bCurcor);
	ui->cbRigel->setChecked(p->bRigel);
	CheckNotes();

	canChange = true;
	RecalcCost();

#ifdef _DEBUG
	//if (imps->isEmpty()) {
	//	ui->rbMultipageProduct->click();
	//	ui->comboMultipageProduct->setCurrentText("Блокнот"/*"Брошюра"*/);
	//	ui->rbA5->click();
	//}
	ui->pbAddImp->click();
#endif // _DEBUG

	return QDialog::exec();
}


bool ProductDlg::ExecEditImposition(Product * product,
									EImpType eImpType,
									bool bAdd,
									QWidget * parent)
{
	Q_ASSERT(product and !product->sName.isEmpty());
	const auto temp = new Product(*product);
	Imposition * imp;
	if (bAdd) {
		imp = new Imposition(eImpType);
		temp->impositions.Add(imp);
	} else
		imp = temp->impositions.GetImp(eImpType);
	temp->SetImpositionsOwner();
	ImpWizard impWizard(imp, bAdd, parent);
	if (impWizard.exec() != QDialog::Accepted)
		return false;
	*product = *temp;
	product->SetImpositionsOwner();
	return true;
}


ProductDlg::~ProductDlg() {
	delete ui;
}


void ProductDlg::UpdateProductSize() {
	const auto & size = p->size.GetSize();
	if (ui->sbProductWidth->value() != size.width())
		SetValueSilent(ui->sbProductWidth, size.width());
	if (ui->sbProductHeight->value() != size.height())
		SetValueSilent(ui->sbProductHeight, size.height());
	if (!canChange)
		return;
	if (!p->IsSimple())
		DeleteAllImpositions();
}


void ProductDlg::EnableImpButtons() {
	const bool impsPresent = !imps->isEmpty();
	const bool isProductSimple = p->IsSimple();
	bool canAddImp;
	if (impsPresent)
		if (isProductSimple)
			canAddImp = false;
		else
			canAddImp = (ui->sbPagesCover->value() and
						 !imps->contains(EImpType::Cover)) or
						(ui->sbPagesBlock->value() and
						 !imps->contains(EImpType::Block)) or
						(ui->sbPagesBottom->value()	and
						 !imps->contains(EImpType::Bottom)) or
						(ui->sbPagesInsert->value() and
						 !imps->contains(EImpType::Insert));
	else
		canAddImp = true;
	ui->pbAddImp->		setEnabled(canAddImp);
	ui->pbEditImp->		setEnabled(impsPresent);
	ui->pbDeleteImp->	setEnabled(!isProductSimple and impsPresent);
	ui->pbAddImp->		setDefault(canAddImp);
	ui->pbOK->			setDefault(!canAddImp);
}


void ProductDlg::DeleteImposition(EImpType type) {
	bool removed = p->impositions.remove(type);
	EnableImpButtons();
	if (removed) {
		MsgBox mb(this);
		mb.Info(sOne);
	}
}

void ProductDlg::DeleteAllImpositions() {
	auto & imps = p->impositions;
	size_t removed = 0;
	if (!imps.isEmpty())
		for (const auto type : enum_values<EImpType>())
			if (imps.remove(type))
				++removed;
	EnableImpButtons();
	if (removed) {
		MsgBox mb(this);
		mb.Info(removed > 1 ? sMany : sOne);
	}
}


void ProductDlg::CheckAndSetPagesCount(EImpType impType) {
	using enum EImpType;

	auto CheckAndSet = [&] (EImpType type) {
		QSpinBox * spinBox = nullptr;
		SizeT * pPages = nullptr;
		switch (type) {
			case Cover:
				spinBox = ui->sbPagesCover;
				pPages = &p->nPagesCover;
				break;

			case Block:
				spinBox = ui->sbPagesBlock;
				pPages = &p->nPagesBlock;
				break;

			case Bottom:
				spinBox = ui->sbPagesBottom;
				pPages = &p->nPagesBottom;
				break;

			case Insert:
				spinBox = ui->sbPagesInsert;
				pPages = &p->nPagesInsert;
				break;

			default:
				Q_ASSERT(false);
				break;
		}

		SizeT pages = spinBox->value();
		const auto oldPages = pages;
		if (type == Block and p->eBinding == EBinding::Staples)
			pages = 4 * Ceil(pages / 4.0);
		else
			pages = 2 * Ceil(pages / 2.0);

		*pPages = pages;
		if (pages != oldPages)
			SetValueSilent(spinBox, pages);
	};

	if (impType == Simple) {
		for (const auto type : enum_values<EImpType>())
			if (type != Simple)
				CheckAndSet(type);
	} else
		CheckAndSet(impType);
}


void ProductDlg::SetDefaultPages() {
	bool isBottomDisabled = false;
	p->nPagesCover = p->nPagesBottom = p->nPagesInsert = 0;

	if (p->IsSimple())
		p->nPagesBlock = 0;
	else {
		switch (p->eKind) {
			case EProductKind::Book:		p->nPagesBlock = 16;		break;
			case EProductKind::Brochure:	p->nPagesBlock = 8;			break;
			case EProductKind::QuartCalend:	p->nPagesBlock = 36;		break;
			case EProductKind::WallCalend:	p->nPagesBlock = 6;			break;
			case EProductKind::Notepad:		p->nPagesBlock = 48 * 2;	break;
											//     48 листов ^^^^^^
			default:						p->nPagesBlock = 4;			break;
		}

		if (p->eBinding == EBinding::SoftCover or
			p->eBinding == EBinding::Staples)
		{
			p->nPagesBlock = 8;
			p->nPagesCover = 4;
			isBottomDisabled = true;
		} else {
			switch (p->eKind) {
				case EProductKind::Newspaper:
					isBottomDisabled = true;
					break;

				case EProductKind::QuartCalend:
					p->nPagesBottom = 8;
					//		 4 подложки ^
					break;

				default:
					p->nPagesCover = 2;
					if (p->eKind != EProductKind::WallCalend)
						p->nPagesBottom = 2;
					break;
			}
		}
	}

	SetValueSilent(ui->sbPagesBlock,	p->nPagesBlock);
	SetValueSilent(ui->sbPagesCover,	p->nPagesCover);
	SetValueSilent(ui->sbPagesBottom,	p->nPagesBottom);
	SetValueSilent(ui->sbPagesInsert,	p->nPagesInsert);
	ui->sbPagesBottom->setEnabled(!isBottomDisabled);
	CalcTotalPages();
}


//void ProductDlg::CheckAndToggleConverts() {
//	if (ui->rbCustomSize->isChecked())
//		return;
//
//	auto GetESize = [] (int id) -> EProductSize {
//		return EnumCastOr<EProductSize>(id, EProductSize::CustomSize);
//	};
//
//	bool bConvertsSelected =
//		ui->rbSimpleProduct->isChecked() and
//		ui->comboSimpleProduct->currentText() == sConvert;
//	for (const auto btn : ui->bgStdProductSize->buttons()) {
//		const auto id = ui->bgStdProductSize->id(btn);
//		bool bConvertSize = stdConvertSizes.contains(GetESize(id));
//		btn->setEnabled(bConvertsSelected == bConvertSize);
//	}
//	const auto eSizeChecked = GetESize(ui->bgStdProductSize->checkedId());
//	if (bConvertsSelected != stdConvertSizes.contains(eSizeChecked)) {
//		const auto eSize = bConvertsSelected ? EProductSize::SizeC5 :
//											   DEFAULT_PRODUCT_SIZE;
//		ui->bgStdProductSize->button(enum_integer(eSize))->toggle();
//		p->size.SetSize(eSize);
//		UpdateProductSize();
//	}
//}


void ProductDlg::CheckCalendar() {
	const bool isCalend = p->eKind == EProductKind::QuartCalend or
						  p->eKind == EProductKind::WallCalend;
	ui->gbOther->setEnabled(isCalend);
	ui->cbRigel->setEnabled(isCalend);
	ui->cbCursor->setEnabled(isCalend);
	ui->cbRigel->setChecked(p->eKind == EProductKind::WallCalend);
	ui->cbCursor->setChecked(p->eKind == EProductKind::QuartCalend);
}
