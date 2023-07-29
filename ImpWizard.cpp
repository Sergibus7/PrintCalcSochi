#include <ranges>
#include <QGraphicsItem>
#include "MsgBox.h"
#include "ImpWizard.h"
#include "ui_ImpWizard.h"


static qreal oldScale;



void ImpWizard::initializePage(int id) {
	if (page(id) == ui->pageInksAndImposition)
		ChangeState(EStateKind::PrepSheets);

	if (page(id) == ui->pagePostpress) {
		const auto oldChange = canChange;
		canChange = false;
		const bool isRiso = imp.ePress == EPress::Riso;
		ui->gbLamination->setEnabled(!isRiso);
		ui->gbFoil->setEnabled(!isRiso);

		if (isRiso) {
			if (lam)
				lam.reset();
			foil.reset();
		} else {
			const bool isKonica = imp.ePress == EPress::Konica;
			ui->rbLamOffset->setEnabled(!isKonica);
			if (isKonica) {
				if (ui->rbLamOffset->isChecked()) {
					ui->rbLamUltrabond->setChecked(true);
					if (lam)
						lam->eType = ELamType::UltraBond;
				}
			} else {
				if (!ui->rbLamOffset->isChecked()) {
					ui->rbLamOffset->setChecked(true);
					if (lam)
						lam->eType = ELamType::Offset;
				}
			}
		}

		ui->gbLamination->setChecked(bool(lam));
		if (lam) {
			Q_ASSERT(imp.ePress != EPress::Riso);
			const auto btn = ui->bgLamType->button(Id(lam->eType));
			btn->click();
			ui->comboLamSpecial->setEnabled(lam->IsSpecial());
			if (lam->IsSpecial()) {
				if (ui->comboLamSpecial->findText(lam->sCustomName) == -1)
					ui->comboLamSpecial->addItem(lam->sCustomName);
				else
					ui->comboLamSpecial->setCurrentText(lam->sCustomName);
			}
			SetIndexByDataFromCombobox(ui->comboLamThickness, *lam->thickness);
			if (lam->bGloss)
				ui->rbLamGlossy->setChecked(true);
			else
				ui->rbLamMatt->setChecked(true);

			ui->cbLamBack->setChecked(lam->bBack);
			SetLamPrice();
		}

		ui->gbFoil->setChecked(bool(foil));
		if (foil) {
			ui->cbFoilBack->setChecked(bool(foil->back));
			ui->cbFoilBack->setEnabled(true);
			EnableFoilWidgets(true);
			ui->sbFoilWidthFace->setValue(
				foil->face ? foil->face->width : 10);
			ui->sbFoilHeightFace->setValue(
				foil->face ? foil->face->height : 10);
			ui->sbFoilWidthBack->setValue(
				foil->back ? foil->back->width : 10);
			ui->sbFoilHeightBack->setValue(
				foil->back ? foil->back->height : 10);
		}

		canChange = oldChange;
	}
}


ImpWizard::ImpWizard(Imposition * imposition, bool add, QWidget * parent) :
	QWizard(parent),
	ui(new Ui::ImpWizard),
	imp(*imposition),
	p(*imposition->product),
	paper(imp.paper),
	pp(imp.postpress),
	lam(imp.postpress.lamination),
	foil(imp.postpress.foil),
	add(add)
{
	Q_ASSERT((p.IsSimple() and imp.eType == EImpType::Simple) or
			 (!p.IsSimple() and imp.eType != EImpType::Simple));

	if (!FitOnPaper(p, imp, add)) {
		MsgBox mb(this);
		mb.Error(S("%1\nИзмените размер изделия!").arg(sErrorNotFit));
		reject();
	}

	ui->setupUi(this);

	for (const auto pageId : pageIds()) {
		auto p = page(pageId);
		Q_ASSERT(p);
		p->setTitle(S("Спуск: %1").arg(imp.GetName(true)));
	}

	ui->sbLamPrice->setMaximum(MAX_LAM_PRICE);
	ui->viewImposition->setScene(&scene);
	setWindowTitle(sDefaultTitle);

	setButtonText(QWizard::NextButton,		S("  Продолжить  "	));
	setButtonText(QWizard::BackButton,		S("Назад"			));
	setButtonText(QWizard::FinishButton,	S("  Завершить  "	));
	setButtonText(QWizard::CancelButton,	S("Отмена"			));


    ///////////////////////////////////////////////////////////////////////////
	//////////////////////////  Страница "Бумага"  ////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	{
		using enum EPaperType;
		ui->bgStandardPaperType->setId(ui->rbCoatedGlossy,	Id(CoatedGlossy));
		ui->bgStandardPaperType->setId(ui->rbCoatedMatte,	Id(CoatedMatte));
		ui->bgStandardPaperType->setId(ui->rbCarton,		Id(Carton));
		ui->bgStandardPaperType->setId(ui->rbOffset,		Id(Offset));
		ui->bgStandardPaperType->setId(ui->rbAdhesive,		Id(Adhesive));
		ui->bgStandardPaperType->setId(ui->rbLabels,		Id(Labels));
		ui->bgStandardPaperType->setId(ui->rbCraft,			Id(Craft));
		ui->bgStandardPaperType->setId(ui->rbNewspaper,		Id(NewsPaper));
		ui->bgDigitalPaperType-> setId(ui->rbColorCopy,		Id(ColorCopy));
		ui->bgDigitalPaperType-> setId(ui->rbLinen,			Id(Linen));
		ui->bgDigitalPaperType-> setId(ui->rbMonotex,		Id(Monotex));
		ui->bgDigitalPaperType-> setId(ui->rbShyne,			Id(Shyne));
		ui->bgDigitalPaperType-> setId(ui->rbSirioPearl,	Id(SirioPearl));
		ui->bgDigitalPaperType-> setId(ui->rbRaflatac,		Id(Raflatac));
		ui->bgDigitalPaperType-> setId(ui->rbTouchCover,	Id(TouchCover));
		ui->bgDigitalPaperType-> setId(ui->rbCustomType,	Id(CustomPaper));
	}

	/////////////////////////////// Типы бумаг ////////////////////////////////

	connect(ui->rbStandardPaper, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		if (canChange and checked)
			ChangeState(EStateKind::PaperTypeStd);
	});

	connect(ui->bgStandardPaperType, &QButtonGroup::buttonClicked, this, [=] {
		if (canChange)
			ChangeState(EStateKind::PaperTypeStd);
	});

	connect(ui->rbDigitalPaper, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		if (canChange and checked)
			ChangeState(EStateKind::PaperTypeDigital);
	});

	connect(ui->bgDigitalPaperType, &QButtonGroup::buttonClicked, this, [=] {
		if (canChange)
			ChangeState(EStateKind::PaperTypeDigital);
	});

	connect(ui->rbCustomType, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		if (canChange and checked)
			ChangeState(EStateKind::PaperTypeCustom);
	});

	connect(ui->leCustomPaperName, &QLineEdit::textEdited, this,
			[=] (const QString & text)
	{
		if (!canChange)
			return;
		Q_ASSERT(paper.IsCustomType());
		paper.sCustomName = text;
	});

	connect(ui->sbPaperPrice,
			QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
			[=] (double newPrice)
	{
		if (!canChange)
			return;
		paper.price = newPrice;
		ChangeState(EStateKind::RecalcCostImp);
	});


	////////////////////////////// Форматы бумаг //////////////////////////////

	{
		using enum EPaperFormat;
		ui->bgStandardPaperFormat->setId(ui->rbFormat50x70pc,	Id(_50x70pc));
		ui->bgStandardPaperFormat->setId(ui->rbFormat43x61,		Id(_43x61));
		ui->bgStandardPaperFormat->setId(ui->rbFormat50x70,		Id(_50x70));
		ui->bgStandardPaperFormat->setId(ui->rbFormat64x90,		Id(_64x90));
		ui->bgStandardPaperFormat->setId(ui->rbFormat70x100,	Id(_70x100));
		ui->bgStandardPaperFormat->setId(ui->rbFormat72x104,	Id(_72x104));
		ui->bgDigitalPaperFormat-> setId(ui->rbFormatA3,		Id(A3));
		ui->bgDigitalPaperFormat-> setId(ui->rbFormatSRA3,		Id(SRA3));
		ui->bgDigitalPaperFormat-> setId(ui->rbFormat330x487,	Id(_330x487));
		ui->bgDigitalPaperFormat-> setId(ui->rbFormatSRA3pc,	Id(SRA3pc));
		ui->bgDigitalPaperFormat-> setId(ui->rbFormat330x487pc,	Id(_330x487pc));
	}

	connect(ui->rbStandardFormats, &QAbstractButton::toggled, this,
			[=] (bool checked) {
		if (canChange and checked)
			ChangeState(EStateKind::PaperFormatStd);
	});

	connect(ui->bgStandardPaperFormat, &QButtonGroup::idClicked, this,
			[=] ([[maybe_unused]] int id)
	{
		if (canChange)
			ChangeState(EStateKind::PaperFormatStd);
	});

	connect(ui->rbDigitalFormats, &QAbstractButton::toggled, this,
			[=] (bool checked) {
		if (canChange and checked)
			ChangeState(EStateKind::PaperFormatDigital);
	});

	connect(ui->bgDigitalPaperFormat, &QButtonGroup::idClicked, this,
			[=] ([[maybe_unused]] int id)
	{
		if (canChange)
			ChangeState(EStateKind::PaperFormatDigital);
	});

	connect(ui->rbCustomFormat, &QAbstractButton::clicked, this,
			[=] (bool checked)
	{
		if (canChange and checked) {
			ChangeState(EStateKind::PaperFormatCustom);
			ui->sbPaperFormatWidth->setFocus();
			ui->sbPaperFormatWidth->selectAll();
		}
	});

	auto CheckCustomFormat = [=] (int newValue, bool isWidth) -> bool {
		ASSERT_RANGE(newValue, PAPER_SIZE);

		// Запас размера бумаги для служебных полей и т. п.
		// с каждой стороны
		int increment;

		switch (imp.ePress) {
			case EPress::Roland:		
			case EPress::Heidelberg:	increment = 8;	break;
			case EPress::Ryobi:			increment = 6;	break;
			case EPress::Riso:			increment = 0;	break;
			case EPress::Konica:		increment = 5;	break;
			default:					increment = -1;	break;
		}

		if (increment == -1)
			CRITICAL("Неверное значение ePress в CheckCustomFormat");

		const QSize paperSize {
			increment + (isWidth ? newValue : int(p.size.width)),
			increment + (isWidth ? int(p.size.height) : newValue),
		};

		if (EFit::NotFit != Fit(paperSize, p.GetSpreadSize(imp.eType)))
			return true;

		MsgBox mb(this);
		mb.Error(S("Изделие не поместится на бумагу!"));
		const auto sb = isWidth ? ui->sbPaperFormatWidth :
								  ui->sbPaperFormatHeight;
		const auto & valueToRestore = isWidth ? oldCustomWidth :
												oldCustomHeight;
		SetValueSilent(sb, valueToRestore);
		sb->setFocus();
		sb->selectAll();
		return false;
	};

	connect(ui->sbPaperFormatWidth,
			qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int newWidth)
	{
		if (!canChange or !CheckCustomFormat(newWidth, true))
			return;
		ChangeState(EStateKind::PaperFormatCustom);
	});

	connect(ui->sbPaperFormatHeight,
			qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int newHeight)
	{
		if (!canChange or !CheckCustomFormat(newHeight, false))
			return;
		ChangeState(EStateKind::PaperFormatCustom);
	});


	//////////////////////////////// Доля листа ///////////////////////////////

	{
		using enum EPortion;
		ui->bgSheetPortion->setId(ui->rbPortion1, enum_integer(Portion1));
		ui->bgSheetPortion->setId(ui->rbPortion2, enum_integer(Portion2));
		ui->bgSheetPortion->setId(ui->rbPortion4, enum_integer(Portion4));
		ui->bgSheetPortion->setId(ui->rbPortion6, enum_integer(Portion6));
	}

	connect(ui->bgSheetPortion, &QButtonGroup::idClicked, this,
			[=] ([[maybe_unused]] int id)
	{
		if (canChange)
			ChangeState(EStateKind::PaperPortion);
	});

	connect(ui->cbCuttingEdge1mm, &QAbstractButton::clicked, this, [=] {
		if (canChange)
			ChangeState(EStateKind::CuttingEdge);
	});


	////////////////////////////// Граммаж бумаги /////////////////////////////

	{
		constexpr auto WeightId = [=] (auto fi) -> int {
			return stdPaperWeights[fi - iMinWeight];
		};
		using enum EIndexStates;
		ui->bgStandardPaperWeight->setId(ui->rb70gm,  WeightId(i70 ));
		ui->bgStandardPaperWeight->setId(ui->rb80gm,  WeightId(i80 ));
		ui->bgStandardPaperWeight->setId(ui->rb90gm,  WeightId(i90 ));
		ui->bgStandardPaperWeight->setId(ui->rb100gm, WeightId(i100));
		ui->bgStandardPaperWeight->setId(ui->rb105gm, WeightId(i105));
		ui->bgStandardPaperWeight->setId(ui->rb115gm, WeightId(i115));
		ui->bgStandardPaperWeight->setId(ui->rb120gm, WeightId(i120));
		ui->bgStandardPaperWeight->setId(ui->rb130gm, WeightId(i130));
		ui->bgStandardPaperWeight->setId(ui->rb140gm, WeightId(i140));
		ui->bgStandardPaperWeight->setId(ui->rb150gm, WeightId(i150));
		ui->bgStandardPaperWeight->setId(ui->rb157gm, WeightId(i157));
		ui->bgStandardPaperWeight->setId(ui->rb160gm, WeightId(i160));
		ui->bgStandardPaperWeight->setId(ui->rb170gm, WeightId(i170));
		ui->bgStandardPaperWeight->setId(ui->rb180gm, WeightId(i180));
		ui->bgStandardPaperWeight->setId(ui->rb200gm, WeightId(i200));
		ui->bgStandardPaperWeight->setId(ui->rb230gm, WeightId(i230));
		ui->bgStandardPaperWeight->setId(ui->rb250gm, WeightId(i250));
		ui->bgStandardPaperWeight->setId(ui->rb265gm, WeightId(i265));
		ui->bgStandardPaperWeight->setId(ui->rb270gm, WeightId(i270));
		ui->bgStandardPaperWeight->setId(ui->rb280gm, WeightId(i280));
		ui->bgStandardPaperWeight->setId(ui->rb300gm, WeightId(i300));
	}

	connect(ui->bgStandardPaperWeight, &QButtonGroup::idClicked, this,
			[=] ([[maybe_unused]] int id)
	{
		if (canChange)
			ChangeState(EStateKind::PaperWeightStd);
	});

	connect(ui->rbStandardWeight, &QAbstractButton::toggled, this,
			[=] (bool checked) {
		if (canChange and checked)
			ChangeState(EStateKind::PaperWeightStd);
	});

	connect(ui->rbCustomWeight, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		if (canChange and checked) {
			ChangeState(EStateKind::PaperWeightCustom);
			ui->sbCustomWeight->setFocus();
			ui->sbCustomWeight->selectAll();
		}
	});

	connect(ui->sbCustomWeight, &QSpinBox::valueChanged, this,
			[=] ([[maybe_unused]] int newWeight)
	{
		if (!canChange)
			return;
		ASSERT_RANGE(newWeight, PAPER_WEIGHT);
		paper.weight = newWeight;
	});


	///////////////////////////////////////////////////////////////////////////
	//////////////////////////// Красочность и спуск //////////////////////////
	///////////////////////////////////////////////////////////////////////////

	{
		using enum EPress;
		ui->bgPress->setId(ui->rbRyobi,		 Id(Ryobi));
		ui->bgPress->setId(ui->rbKonica,	 Id(Konica));
		ui->bgPress->setId(ui->rbRoland,	 Id(Roland));
		ui->bgPress->setId(ui->rbHeidelberg, Id(Heidelberg));
		ui->bgPress->setId(ui->rbRisograph,	 Id(Riso));
	}

	ui->bgInksFace->setId(ui->rbInksFace0, 0);
	ui->bgInksFace->setId(ui->rbInksFace1, 1);
	ui->bgInksFace->setId(ui->rbInksFace2, 2);
	ui->bgInksFace->setId(ui->rbInksFace3, 3);
	ui->bgInksFace->setId(ui->rbInksFace4, 4);
	ui->bgInksBack->setId(ui->rbInksBack0, 0);
	ui->bgInksBack->setId(ui->rbInksBack1, 1);
	ui->bgInksBack->setId(ui->rbInksBack2, 2);
	ui->bgInksBack->setId(ui->rbInksBack3, 3);
	ui->bgInksBack->setId(ui->rbInksBack4, 4);

	connect(ui->bgPress, &QButtonGroup::idClicked, this,
			[=] ([[maybe_unused]] int id)
	{
		if (canChange)
			ChangeState(EStateKind::Press);
	});

	connect(ui->cbNoPrint, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		if (canChange)
			ChangeState(checked ? EStateKind::NoPrint : EStateKind::Press);
	});

	connect(ui->bgInksFace, &QButtonGroup::idClicked, this,
			[=] (int faceInks)
	{
		if (canChange)
			ChangeState(faceInks ? EStateKind::InksFace : EStateKind::NoPrint);
	});

	connect(ui->bgInksBack, &QButtonGroup::idClicked, this,
			[=] ([[maybe_unused]] int backInks)
	{
		if (canChange)
			ChangeState(EStateKind::InksBack);
	});

	connect(ui->cbOwnBack, &QAbstractButton::toggled, this,
			[=] ([[maybe_unused]] bool checked)
	{
		if (canChange)
			ChangeState(EStateKind::OwnBack);
	});

	connect(ui->sbPreparedSheets, &QSpinBox::valueChanged, this,
			[=] (int newValue)
	{
		if (!canChange)
			return;
		imp.prepSheets = newValue;
		ChangeState(EStateKind::RecalcCostImp);
		ui->lblFormatSheets->setText(t(imp.signatures.sourceSheets));
	});

	connect(ui->cbAutoImposition, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		ui->sbPlaces->setDisabled(checked);
		ui->gbIndents->setEnabled(checked);
		if (!canChange)
			return;
		imp.bAutoImposition = checked;
		ChangeState(EStateKind::RecalcImp);
	});

	connect(ui->cbAutoGap, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		ui->sbGap->setDisabled(checked);
		if (!canChange)
			return;
		imp.bAutoGap = checked;
		ChangeState(EStateKind::RecalcImp);
	});

	connect(ui->sbGap, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
			this, [=] (double newGap)
	{
		if (!canChange)
			return;
		imp.gap = newGap;
		ChangeState(EStateKind::RecalcImp);
	});

	connect(ui->cbAutoIndent, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		ui->sbHorizIndent->setDisabled(checked);
		ui->sbVertIndent->setDisabled(checked);
		if (!canChange)
			return;
		imp.bAutoIndent = checked;
		ChangeState(EStateKind::RecalcImp);
	});

	connect(ui->sbHorizIndent,
			QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
			[=] (double newIndent)
	{
		if (!canChange)
			return;
		imp.horizIndent = newIndent;
		ChangeState(EStateKind::RecalcImp);
	});

	connect(ui->sbVertIndent,
			QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
			[=] (double newIndent)
	{
		if (!canChange)
			return;
		imp.vertIndent = newIndent;
		ChangeState(EStateKind::RecalcImp);
	});

	connect(ui->sbPlaces, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int newPlaces)
	{
		if (!canChange)
			return;
		imp.SetTotalPlaces(newPlaces);
		ChangeState(EStateKind::RecalcImp);
	});


	///////////////////////////////////////////////////////////////////////////
	////////////////////////// Постпечатная обработка /////////////////////////
	///////////////////////////////////////////////////////////////////////////

	for (const auto thickness : stdLamThickness)
		ui->comboLamThickness->addItem(S("%1 мкм").arg(thickness), thickness);

	using enum ELamType;
	ui->bgLamType->setId(ui->rbLamOffset,		enum_integer(Offset));
	ui->bgLamType->setId(ui->rbLamDigital,		enum_integer(Digital));
	ui->bgLamType->setId(ui->rbLamUltrabond,	enum_integer(UltraBond));
	ui->bgLamType->setId(ui->rbLamPacket,		enum_integer(Packet));
	ui->bgLamType->setId(ui->rbLamSpecial,		enum_integer(LamCustom));

	connect(ui->gbLamination, &QGroupBox::toggled, this,
			[this] (bool checked)
	{
		if (!canChange)
			return;

		if (checked) {
			lam = Lamination();
			ui->rbLamOffset->setEnabled(imp.ePress != EPress::Konica);
			auto lamType =
				EnumCastOr<ELamType>(ui->bgLamType->checkedId(), LamCustom);
			if (lamType == ELamType::Offset and imp.ePress == EPress::Konica)
				lamType = ELamType::UltraBond;
			lam->bBack = ui->cbLamBack->isChecked();
			lam->bGloss = ui->rbLamGlossy->isChecked();
			switch (lamType) {
				case Packet:	SetPacketLam(checked);	break;
				case LamCustom:	SetSpecialLam(checked);	break;
				default:		SetLamination(lamType);	break;
			}
		} else
			lam.reset();

		RecalcCost();
	});

	connect(ui->bgLamType, &QButtonGroup::idToggled, this,
			[=] (int id, bool checked)
	{
		if (!canChange or !checked)
			return;

		switch ((ELamType) id) {
			case Packet:
				SetPacketLam(checked);
				break;

			case LamCustom:
				SetSpecialLam(checked);
				break;

			default:
				SetLamination(EnumCast<ELamType>(id));
				break;
		}

		RecalcCost();
	});

	connect(ui->rbLamOffset, &QAbstractButton::clicked, this, [=] {
		if (canChange)
			SetLamination(Offset);
	});

	connect(ui->rbLamDigital, &QAbstractButton::clicked, this, [=] {
		if (canChange)
			SetLamination(Digital);
	});

	connect(ui->rbLamUltrabond, &QAbstractButton::clicked, this, [=] {
		if (canChange)
			SetLamination(UltraBond);
	});

	connect(ui->rbLamPacket, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		if (canChange)
			SetPacketLam(checked);
	});

	connect(ui->rbLamSpecial, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		if (canChange)
			SetSpecialLam(checked);
	});

	connect(ui->rbLamGlossy, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		if (!canChange)
			return;
		lam->bGloss = checked;
		SetLamPrice();
		RecalcCost();
	});

	connect(ui->cbLamBack, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		if (!canChange)
			return;
		lam->bBack = checked;
		SetLamPrice();
		RecalcCost();
	});

	connect(ui->comboLamThickness,
			qOverload<int>(&QComboBox::currentIndexChanged), this, [this] 
	{
		if (!canChange)
			return;
		lam->thickness = int(GetDataFromCombobox(ui->comboLamThickness));
		SetLamPrice();
		RecalcCost();
	});

	connect(ui->sbLamPrice, qOverload<double>(&QDoubleSpinBox::valueChanged),
			this, [=] (double newPrice)
	{
		if (!canChange)
			return;
		lam->price = newPrice;
		RecalcCost();
	});

	connect(ui->gbFoil, &QGroupBox::toggled, this, [=] (bool checked) {
		if (!canChange)
			return;

		if (checked) {
			foil = Foil();
			foil->face = FoilSide(ui->sbFoilWidthFace->value(),
								  ui->sbFoilHeightFace->value());
			if (ui->cbFoilBack->isChecked())
				foil->back = FoilSide(ui->sbFoilWidthBack->value(),
									  ui->sbFoilHeightBack->value());
		} else
			foil.reset();

		ui->cbFoilBack->setEnabled(checked);
		EnableFoilWidgets(checked);
		RecalcCost();
	});

	connect(ui->cbFoilBack, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		if (!canChange)
			return;

		if (checked)
			foil->back = FoilSide(ui->sbFoilWidthBack->value(),
								  ui->sbFoilHeightBack->value());
		else
			foil->back.reset();

		EnableFoilWidgets(checked, true);
		RecalcCost();
	});

	connect(ui->sbFoilWidthFace, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int newWidth)
	{
		if (!canChange)
			return;
		foil->face->width = newWidth;
		RecalcCost();
	});

	connect(ui->sbFoilWidthBack, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int newWidth) 
	{
		if (!canChange)
			return;
		foil->back->width = newWidth;
		RecalcCost();
	});

	connect(ui->sbFoilHeightFace, qOverload<int>(&QSpinBox::valueChanged),
			this, [=] (int newHeight)
	{
		if (!canChange)
			return;
		foil->face->height = newHeight;
		RecalcCost();
	});

	connect(ui->sbFoilHeightBack, qOverload<int>(&QSpinBox::valueChanged),
			this, [=] (int newHeight)
	{
		if (!canChange)
			return;
		foil->back->height = newHeight;
		RecalcCost();
	});

	connect(ui->comboFoilColorFace, &QComboBox::currentTextChanged, this,
			[=] ([[maybe_unused]] const QString & text)
	{
		if (!canChange)
			return;
		foil->face->color = GetColorFromCombo(ui->comboFoilColorFace);
	});

	connect(ui->comboFoilColorBack, &QComboBox::currentTextChanged, this,
			[=] ([[maybe_unused]] const QString & text)
	{
		if (!canChange)
			return;
		foil->back->color = GetColorFromCombo(ui->comboFoilColorBack);
	});

	connect(ui->cbCreasing, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		ui->sbCreasings->setEnabled(checked);
		if (!canChange)
			return;
		if (checked)
			pp.creasings = ui->sbCreasings->value();
		else
			pp.creasings.reset();
		RecalcCost();
	});

	connect(ui->sbCreasings, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int nQuantity)
	{
		if (!canChange)
			return;
		pp.creasings = nQuantity;
		RecalcCost();
	});

	connect(ui->cbFolding, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		ui->sbFoldings->setEnabled(checked);
		ui->cbFoldByHand->setEnabled(checked);
		if (!canChange)
			return;
		if (checked)
			pp.foldings.emplace(ui->sbFoldings->value(), true);
		else
			pp.foldings.reset();
		RecalcCost();
	});

	connect(ui->cbFoldByHand, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		if (!canChange)
			return;
		pp.foldings->foldByHand = checked;
		RecalcCost();
	});

	connect(ui->sbFoldings, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int newValue) {
		if (!canChange)
			return;
		pp.foldings->count = newValue;
		RecalcCost();
	});

	connect(ui->cbPerforation, &QAbstractButton::toggled, this,
			[=] (bool checked) 
	{
		ui->sbPerforations->setEnabled(checked);
		if (!canChange)
			return;

		if (checked)
			pp.perforations = ui->sbPerforations->value();
		else
			pp.perforations.reset();
		RecalcCost();
	});

	connect(ui->sbPerforations, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int nQuantity)
	{
		if (!canChange)
			return;
		pp.perforations = nQuantity;
		RecalcCost();
	});

	connect(ui->cbGluePoint, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		ui->sbGluePoints->setEnabled(checked);
		if (!canChange)
			return;

		if (checked)
			pp.gluePoints = ui->sbGluePoints->value();
		else
			pp.gluePoints.reset();
		RecalcCost();
	});

	connect(ui->sbGluePoints, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int nQuantity)
	{
		if (!canChange)
			return;
		pp.gluePoints = nQuantity;
		RecalcCost();
	});

	ui->bgDblSidedTape->setId(ui->rbDblSidedTape_5mm, 5);
	ui->bgDblSidedTape->setId(ui->rbDblSidedTape_9mm, 9);

	connect(ui->cbDoubleSidedTape, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		ui->sbDoubleSidedTape->setEnabled(checked);
		if (!canChange)
			return;

		if (checked)
			pp.dblSidedTape.emplace(ui->sbDoubleSidedTape->value(),
									ui->sbDblSidedTapeLenght->value(),
									ui->bgDblSidedTape->checkedId());
		else
			pp.dblSidedTape.reset();

		ui->sbDoubleSidedTape->setEnabled(checked);
		EnableButtonGroup(ui->bgDblSidedTape, checked);
		ui->sbDblSidedTapeLenght->setEnabled(checked);
		ui->lblDblTapeWidth->setEnabled(checked);
		ui->lblDblTapeLenght->setEnabled(checked);
		RecalcCost();
	});

	connect(ui->sbDoubleSidedTape, qOverload<int>(&QSpinBox::valueChanged),
			this, [=] (int newQuantity)
	{
		if (!canChange)
			return;
		pp.dblSidedTape->count = newQuantity;
		RecalcCost();
	});

	connect(ui->bgDblSidedTape, &QButtonGroup::idToggled, this,
			[=] (int id, bool checked)
	{
		if (!canChange or !checked)
			return;
		pp.dblSidedTape->width = id;
		RecalcCost();
	});

	connect(ui->sbDblSidedTapeLenght, qOverload<int>(&QSpinBox::valueChanged),
			this, [=] (int newLenght)
	{
		if (!canChange)
			return;
		pp.dblSidedTape->length = newLenght;
		RecalcCost();
	});

	connect(ui->cbScrathStrip, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		ui->sbScrathStrips->setEnabled(checked);
		if (!canChange)
			return;
		if (checked)
			pp.scrathStrips = ui->sbScrathStrips->value();
		else
			pp.scrathStrips.reset();
		RecalcCost();
	});

	connect(ui->sbScrathStrips, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int nQuantity)
	{
		if (!canChange)
			return;
		pp.scrathStrips = nQuantity;
		RecalcCost();
	});

	connect(ui->cbRoundedCorners, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		ui->sbRoundedCorners->setEnabled(checked);
		ui->sbRoundedCornersRadius->setEnabled(checked);
		ui->lblRadiusRound->setEnabled(checked);
		if (!canChange)
			return;

		if (checked)
			pp.rndCorners.emplace(ui->sbRoundedCorners->value(),
								  ui->sbRoundedCornersRadius->value());
		else
			pp.rndCorners.reset();
		RecalcCost();
	});

	connect(ui->sbRoundedCorners, qOverload<int>(&QSpinBox::valueChanged),
			this, [=] (int nQuantity)
	{
		if (!canChange)
			return;
		pp.rndCorners->count = nQuantity;
		RecalcCost();
	});

	connect(ui->sbRoundedCornersRadius,
			qOverload<int>(&QSpinBox::valueChanged), this, [=] (int nRadius)
	{
		if (!canChange)
			return;
		pp.rndCorners->radius = nRadius;
	});

	connect(ui->cbEyelets, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		ui->sbEyelets->setEnabled(checked);
		ui->sbEyeletsDiameter->setEnabled(checked);
		ui->lblEyeletsDiam->setEnabled(checked);
		if (!canChange)
			return;

		if (checked)
			pp.eyelets.emplace(ui->sbEyelets->value(),
							   ui->sbEyeletsDiameter->value());
		else
			pp.eyelets.reset();

		RecalcCost();
	});

	connect(ui->sbEyelets, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int newQuantity)
	{
		if (!canChange)
			return;
		pp.eyelets->count = newQuantity;
		RecalcCost();
	});

	connect(ui->sbEyeletsDiameter, qOverload<int>(&QSpinBox::valueChanged),
			this, [=] (int newDiameter)
	{
		if (!canChange)
			return;
		pp.eyelets->diam = newDiameter;
	});

	connect(ui->cbHoles, &QAbstractButton::toggled, this, [=] (bool checked) {
		ui->sbHoles->setEnabled(checked);
		ui->sbHolesDiameter->setEnabled(checked);
		ui->lblHolesDiam->setEnabled(checked);
		if (!canChange)
			return;

		if (checked)
			pp.holes.emplace(ui->sbHoles->value(),
							 ui->sbHolesDiameter->value());
		else
			pp.holes.reset();

		RecalcCost();
	});

	connect(ui->sbHoles, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int nQuantity) {
		if (!canChange)
			return;
		pp.holes->count = nQuantity;
		RecalcCost();
	});

	connect(ui->sbHolesDiameter, qOverload<int>(&QSpinBox::valueChanged),
			this, [=] (int nDiameter)
	{
		if (!canChange)
			return;
		pp.holes->diam = nDiameter;
		RecalcCost();
	});

	connect(ui->cbNumeration, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		ui->lblNumFrom->	setEnabled(checked);
		ui->lblNumTo->		setEnabled(checked);
		ui->lblNumZeroes->	setEnabled(checked);
		ui->lblNumSample->	setEnabled(checked);
		ui->sbStartNumber->	setEnabled(checked);
		ui->sbEndNumber->	setEnabled(checked);
		ui->sbNumZeroes->	setEnabled(checked);

		if (canChange) {
			if (checked)
				pp.nums.emplace(ui->sbStartNumber->value(),
								ui->sbEndNumber->value(),
								ui->sbNumZeroes->value());
			else
				pp.nums.reset();
			RecalcCost();
		}
		if (checked)
			CalculateSampleNum();
	});

	connect(ui->sbStartNumber, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int nStartNumber)
	{
		if (canChange) {
			ui->sbEndNumber->setMinimum(nStartNumber + 1);
			pp.nums->start = nStartNumber;
		}
		CalculateSampleNum();
	});

	connect(ui->sbEndNumber, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int nEndNumber)
	{
		if (canChange)
			pp.nums->end = nEndNumber;
		CalculateSampleNum();
	});

	connect(ui->sbNumZeroes, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int newValue)
	{
		if (canChange)
			pp.nums->digits = newValue;
		CalculateSampleNum();
	});

	connect(ui->cbDieCutting, &QAbstractButton::toggled, this,
			[=] (bool checked)
	{
		ui->lblDieName->setEnabled(checked);
		ui->lblDieCount->setEnabled(checked);
		ui->sbDieCount->setEnabled(checked);
		ui->leDieName->setEnabled(checked);
		if (!canChange)
			return;

		if (checked) {
			pp.dieCutting.emplace(ui->sbDieCount->value(),
								  ui->leDieName->text());
			ui->sbDieCount->setFocus();
		} else
			pp.dieCutting.reset();
		RecalcCost();
	});

	connect(ui->sbDieCount, qOverload<int>(&QSpinBox::valueChanged), this,
			[=] (int nCount)
	{
		if (!canChange)
			return;
		pp.dieCutting->count = nCount;
		RecalcCost();
	});

	connect(ui->leDieName, &QLineEdit::editingFinished, this, [=] {
		if (!canChange)
			return;
		pp.dieCutting->sName = ui->leDieName->text();
	});


	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////  Инициализация  ////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	using enum EProductSize;
	const bool isPacketSize = QSet<EProductSize> {
			SizeA3, SizeA4, SizeA5, SizeA6,
	}.contains(p.size.GetESize());
	ui->rbLamPacket->setEnabled(isPacketSize);

	static_cast<QPushButton *>(button(QWizard::NextButton))->setDefault(true);
}


void ImpWizard::SetLamination(ELamType type) {
	Q_ASSERT(lam);
	lam->eType = type;
	lam->thickness = defaultLamThickness[type];
	Q_ASSERT(lam->thickness);
	bool oldChange = canChange;
	canChange = false;
	SetIndexByDataFromCombobox(ui->comboLamThickness, *lam->thickness);
	canChange = oldChange;
	SetLamPrice();
}


bool ImpWizard::validateCurrentPage() {
	MsgBox mb(this);

	if (currentPage() == ui->pagePaper) {
		if (paper.eType == EPaperType::CustomPaper and
			paper.sCustomName.isEmpty())
		{
			mb.Error(S("Укажите название бумаги!"));
			ui->leCustomPaperName->setFocus();
			return false;
		}


		if (!imp.GetTotalPlaces()) {
			mb.Error(S("Размер изделия превышает\n"
					   "размер печатного поля!"));
			return false;
		}

		if (qFuzzyIsNull(ui->sbPaperPrice->value())) {
			mb.Error(S("Цена бумаги не может быть нулевой!\n"
					   "\n"
					   "Возможно, для данной бумаги отсутствует "
					   "цена в прайсе.\n"
					   "Вам необходимо ввести ее самостоятельно"));
			ui->sbPaperPrice->setFocus();
			ui->sbPaperPrice->selectAll();
			return false;
		}

		return true;
	}

	if (currentPage() == ui->pagePostpress) {
		const auto sFillPrompt = S("\nПожалуйста, заполните значение "
								   "или выберите из списка!");
		if (lam) {
			if (qFuzzyIsNull(ui->sbLamPrice->value())) {
				ui->sbLamPrice->setFocus();
				ui->sbLamPrice->selectAll();
				mb.Error(S("Цена ламинации не может быть нулевой!"));
				return false;
			}
		}

		if (ui->rbLamSpecial->isChecked() and
			ui->comboLamSpecial->currentText().isEmpty()) {
			mb.Error(S("Не выбран тип ламинации!") + sFillPrompt);
			ui->comboLamSpecial->setFocus();
			return false;
		}

		if (ui->gbFoil->isChecked()) {
			const QString sFoilError("Не выбрана пленка для фольгирования!");
			if (ui->comboFoilColorFace->currentText().isEmpty()) {
				mb.Error(sFoilError + sFillPrompt);
				ui->comboFoilColorFace->setFocus();
				return false;
			}

			if (ui->cbFoilBack->isChecked() and
				ui->comboFoilColorBack->currentText().isEmpty())
			{
				mb.Error(sFoilError + sFillPrompt);
				ui->comboFoilColorBack->setFocus();
				return false;
			}
		}

//		lam->sCustomName = ui->comboLamSpecial->currentText();
//		lam->thickness = GetDataFromCombobox(ui->comboLamThickness);
	}
	return true;
}


int ImpWizard::exec() {
	if (!logWindow)
		logWindow = new LogWindow(this);
	logWindow->UpdateTitle(S("спуска: %1").arg(imp.GetName()));
	logWindow->show();
	logWindow->Pin(this, !qApp->activeModalWidget());

	oldScale = 1;
	const auto prevPrice = paper.price;
	ChangeState(EStateKind::Init);
	if (!qFuzzyIsNull(prevPrice)) {
		paper.price = prevPrice;
		ui->sbPaperPrice->setValue(prevPrice);
	}

	return QDialog::exec();
}


bool ImpWizard::FitOnPaper(Product & p, Imposition & imp, bool bAdd) {
	const auto & productSize = p.GetSpreadSize(imp.eType);
	// Размер изделия с выходом за обрез
	const auto & bleedSize = productSize.grownBy(QMargins() + 2);

	if (imp.paper.eType == EPaperType::NewsPaper or
		imp.ePress == EPress::Riso)
	{
		const auto fit = Fit(imp.paper.printSize, productSize);
		if (fit == EFit::Fit or fit == EFit::Equal)
			return true;
	} else
		if (Fit(imp.paper.printSize, bleedSize) == EFit::Fit)
			return true;

	if (!bAdd)
		_CRITICAL(sErrorNotFit);

	// Пытаемся подобрать формат бумаги и долю листа под размер изделия

	const std::array<EPaperFormat, 3> acceptableFormats {
		EPaperFormat::_64x90,
		EPaperFormat::_70x100,
		EPaperFormat::_72x104,
	};

	const std::array<EPortion, 2> acceptablePortions {
		EPortion::Portion4,
		EPortion::Portion2,
	};

	// Минимально допустимая бумага
	Paper acceptablePaper {
		.eType = EPaperType::CoatedGlossy,
		.bCuttingEdge = true,
		.weight = 120,
	};

	bool acceptableFound = false;

	for (const auto eFormat : acceptableFormats) {
		acceptablePaper.format.eFormat = eFormat;
		for (const auto ePortion : acceptablePortions) {
			acceptablePaper.ePortion = ePortion;
			if (Fit(acceptablePaper.printSize, bleedSize) == EFit::Fit) {
				acceptableFound = true;
				break;
			}
		}
		if (acceptableFound)
			break;
	}

	if (!acceptableFound)
		return false;

	imp.paper = acceptablePaper;
	imp.inks.face = 4;
	imp.ePress =
		acceptablePaper.ePortion == EPortion::Portion2 ? EPress::Roland :
														 EPress::Ryobi;
	return true;
}


void ImpWizard::RenderImp() {
	scene.clear();
	scene.setBackgroundBrush({ Qt::lightGray });

	auto printField = imp.GetPrintField();
	auto printSize = paper.printSize;
	QRectF rectPrintSheet(QPointF(0.0, 0.0), printSize);

	scene.addRect(rectPrintSheet, { QColor(32, 32, 32) }, { Qt::white });
	if ((printSize.width() > printField.width() or
		 printSize.height() > printField.height()) and
		(imp.horizIndent > 0.0 or imp.vertIndent > 0.0))
	{
		scene.addRect(*imp.horizIndent, *imp.vertIndent,
					  printField.width(), printField.height(),
					  { { Qt::darkGray }, 0, Qt::DashLine },
					  { Qt::white });
	}

	if (imp.bAutoImposition) {
		QPen penBleed(QColor(0, 135, 255));
		QPen penPage(Qt::red);
		QBrush brushBleed(QColor(95, 180, 255));
		auto sizeBleed = imp.GetBleedSize();
		const bool bBleedPortrait = sizeBleed.width() < sizeBleed.height();
		const auto horizCount = imp.GetWPlaces();
		const auto vertCount = imp.GetHPlaces();
		const auto halfGap = *imp.bleed - *imp.gap / 2.0;
		Q_ASSERT(halfGap >= 0.0);

		const auto w = horizCount * sizeBleed.width() -
					   (horizCount - 1) * halfGap * 2.0;
		const auto h = vertCount * sizeBleed.height() -
					   (vertCount - 1) * halfGap * 2.0;
		const QSizeF sizeFullBleeds(w, h);
		Q_ASSERT(sizeFullBleeds.width() <= printSize.width() and
				 sizeFullBleeds.height() <= printSize.height());

		const auto lastHoriz = horizCount - 1;
		const auto lastVert = vertCount - 1;
		const QPointF bleedOrigin {
			(printSize.width() - sizeFullBleeds.width()) / 2.0,
			(printSize.height() - sizeFullBleeds.height()) / 2.0
		};

		QSizeF sizeSpread = p.GetSpreadSize(imp.eType);
		bool bSpreadPortrait = sizeSpread.width() < sizeSpread.height();
		if (bBleedPortrait != bSpreadPortrait) {
			bSpreadPortrait = !bSpreadPortrait;
			sizeSpread.transpose();
		}

		QSizeF sizeBleeds;
		QPointF bleed, page;
		for (auto i = decltype(vertCount) { 0 }; i < vertCount; i++) {
			sizeBleeds.rheight() = sizeBleed.height();
			if (vertCount > 1)
				sizeBleeds.rheight() -= halfGap;
			if (i) {
				if (i < lastVert)
					sizeBleeds.rheight() -= halfGap;
			} else {
				bleed.ry() = bleedOrigin.y();
				page.ry() = bleed.y() + *imp.bleed;
			}

			for (auto j = decltype(horizCount) { 0 }; j < horizCount; j++) {
				sizeBleeds.setWidth(sizeBleed.width());
				if (vertCount > 1)
					sizeBleeds.rwidth() -= halfGap;
				if (j) {
					if (j < lastHoriz)
						sizeBleeds.rwidth() -= halfGap;
				} else {
					bleed.rx() = bleedOrigin.x();
					page.rx() = bleed.x() + *imp.bleed;
				}

				scene.addRect({ bleed, sizeBleeds }, penBleed, brushBleed);

				if (imp.bleed > 0.0)
					scene.addRect({ page, sizeSpread }, penPage);

				if (imp.IsSpreads()) {
					QPen penSpread(Qt::DashLine);
					QColor color = penPage.color();
					color.setAlpha(90);
					penSpread.setColor(color);
					qreal center;
					if (bSpreadPortrait) {
						center = page.x() + sizeSpread.width() / 2.0;
						scene.addLine(center, page.y(),
									  center, page.y() + sizeSpread.height(),
									  penSpread);
					} else {
						center = page.y() + sizeSpread.height() / 2.0;
						scene.addLine(page.x(), center,
									  page.x() + sizeSpread.width(), center,
									  penSpread);
					}
				}

				bleed.rx() += sizeBleeds.width();
				page.rx() += sizeSpread.width() + *imp.gap;
			}
			bleed.ry() += sizeBleeds.height();
			page.ry() += sizeSpread.height() + *imp.gap;
		}
	}

	const auto vSize = ui->viewImposition->size() - QSize(10, 10);
	const auto sceneSize = scene.sceneRect().size();
	const auto scale = qMin(vSize.width() / sceneSize.width(),
							vSize.height() / sceneSize.height());
	ui->viewImposition->scale(1 / oldScale,  1 / oldScale);
	ui->viewImposition->scale(scale, scale);
	oldScale = scale;
	ui->viewImposition->show();
}


void ImpWizard::ChangeState(EStateKind stateKind) {
	enum class Group : size_t {
		StdFormat, DigitalFormat, Portion, Weight, Press, /*InkFace, InkBack,*/
	};

	canChange = false;
	using enum EStateKind;
	using enum EPaperType;
	bool userAction = stateKind != Init;
	const bool isProductLessA2 = !p.IsSizeLargerB3();
	const State * state;

	auto EnableAndCheckButtonsFromState = [&state, this]
			(Group group, bool enable, bool setByDefault = false) -> void
	{
		using enum Group;
		QButtonGroup * bg = nullptr;
		switch (group) {
			case StdFormat:		bg = ui->bgStandardPaperFormat;	break;
			case DigitalFormat:	bg = ui->bgDigitalPaperFormat;	break;
			case Portion:		bg = ui->bgSheetPortion;		break;
			case Weight:		bg = ui->bgStandardPaperWeight;	break;
			case Press:			bg = ui->bgPress;				break;
//			case InkFace:		bg = ui->bgInksFace;			break;
//			case InkBack:		bg = ui->bgInksBack;			break;
		}
		Q_ASSERT(bg);

		if (!enable) {
			DisableButtonGroup(bg);
			return;
		}

		EIndexStates start, finish;
		switch (group) {
			case StdFormat:
				start	= iMinStdFormat;
				finish	= iMaxStdFormat;
				break;

			case DigitalFormat:
				start	= iMinDigitalFormat;
				finish	= iMaxDigitalFormat;
				break;

			case Portion:
				start	= iMinPortion;
				finish	= iMaxPortion;
				break;

			case Weight:
				start	= iMinWeight;
				finish	= iMaxWeight;
				break;

			case Press:
				start	= iMinPress;
				finish	= iMaxPress;
				break;

//			case InkFace:
//			case InkBack:
//				start	= enum_value<EIndexStates>(0);
//				finish	= enum_value<EIndexStates>(4);
//				break;
		}

		auto id = [=] (EIndexStates i) -> int {
			switch (group) {
				case StdFormat:
				case DigitalFormat:
					return EnumIndex(mapIndexPaperFormat[i]);

				case Portion: {
//				case InkFace:
//				case InkBack:
					auto portion = enum_value<EPortion>(i - start);
					return enum_integer(portion);
				}

				case Weight:
					return stdPaperWeights[i - start];

				case Press:
					return Id(enum_value<EPress>(i - start));

				default:
					Q_ASSERT(false);
					return -1;
			}
		};

		for (auto i = start; i <= finish; ++i) {
			const auto btn = bg->button(id(i));
			Q_ASSERT(btn);
			btn->setEnabled(state->flags[i]);
		}

		if (!setByDefault)
			return;

		int defaultId;
		switch (group) {
			case DigitalFormat:
			case StdFormat:		defaultId = Id(state->defaultFormat);	break;
			case Portion:		defaultId = Id(state->defaultPortion);	break;
			case Weight:		defaultId = state->defaultWeight;		break;
			case Press:			defaultId = Id(state->defaultPress);	break;
			default:
				Q_ASSERT(false);
				defaultId = -1;
				break;
		}
		bg->button(defaultId)->setChecked(true);
	};

	// Устанавливаем тип бумаги
	if (stateKind <= PaperTypeCustom) {
		bool isStdType;
		// Если состояние изменил пользователь,
		// выбираем тип бумаги из нажатой кнопки, иначе из модели
		if (userAction) {
			if (stateKind == PaperTypeCustom) {
				isStdType = false;
				paper.eType = CustomPaper;
				paper.sCustomName = ui->leCustomPaperName->text();
			} else {
				isStdType = stateKind == PaperTypeStd;
				const auto id =
					isStdType ?	ui->bgStandardPaperType->checkedId() :
								ui->bgDigitalPaperType->checkedId();
				Q_ASSERT(id >= 0 and id < int(enum_count<EPaperType>()));
				paper.eType = enum_value<EPaperType>(id);
				paper.sCustomName.clear();
			}
		} else {
			isStdType = paper.IsStdType();
			if (paper.IsCustomType()) {
				Q_ASSERT(!paper.sCustomName.isEmpty());
				ui->rbCustomType->setChecked(true);
				ui->leCustomPaperName->setText(paper.sCustomName);
			} else {
				if (isStdType)
					ui->rbStandardPaper->setChecked(true);
				else
					ui->rbDigitalPaper->setChecked(true);
				SetBtnChecked(isStdType ?	ui->bgStandardPaperType :
											ui->bgDigitalPaperType,
							  Id(paper.eType));
			}
		}

		const bool isCustomType = paper.IsCustomType();
		EnableButtonGroup(ui->bgStandardPaperType,
						  isStdType and !isCustomType);
		EnableButtonGroup(ui->bgDigitalPaperType,
						  !isStdType and !isCustomType);
		ui->leCustomPaperName->setEnabled(isCustomType);
		if (isCustomType) {
			ui->leCustomPaperName->setFocus();
			ui->leCustomPaperName->selectAll();
		}
	}

	// Ищем состояние по типу бумаги и размеру изделия
	if (stateKind <= Press) {
		const auto it =
			std::ranges::find_if(statesTable, [=] (auto st) {
				return	st->paperType == paper.eType and
						st->isLessA2 == isProductLessA2;
			});
		if (it == statesTable.end())
			CRITICAL("Состояние не найдено!");
		state = *it;
	}

	// Устанавливаем формат бумаги
	if (stateKind <= PaperFormatCustom) {
		using enum EPaperFormat;
		bool setByDefault = false;

		switch (stateKind) {
			case Init:
				if (paper.format.IsCustom()) {
					ui->rbCustomFormat->setChecked(true);
					ui->sbPaperFormatWidth->setValue(paper.format.width);
					ui->sbPaperFormatHeight->setValue(paper.format.height);
				} else {
					const auto id = Id(paper.format.eFormat);
					if (paper.format.IsStandard()) {
						ui->rbStandardFormats->setChecked(true);
						SetBtnChecked(ui->bgStandardPaperFormat, id);
					} else {
						ui->rbDigitalFormats->setChecked(true);
						SetBtnChecked(ui->bgDigitalPaperFormat, id);
					}
				}
				break;

			case PaperFormatStd:
			case PaperFormatDigital: {
				const auto isStdFormat =
					ui->rbStandardFormats->isChecked();
				const auto bg = isStdFormat ?	ui->bgStandardPaperFormat :
												ui->bgDigitalPaperFormat;
				paper.format = enum_value<EPaperFormat>(bg->checkedId());
				auto id = mapIndexPaperFormat.key(paper.format.eFormat);
				if (!state->flags[id]) {
					const auto iMin = isStdFormat ? iMinStdFormat :
						iMinDigitalFormat;
					const auto iMax = isStdFormat ? iMaxStdFormat :
						iMaxDigitalFormat;
					const auto it =
						std::ranges::find(state->flags.constFind(iMin),
										  state->flags.constFind(iMax),
										  true);
					Q_ASSERT(it != state->flags.end());
					paper.format = mapIndexPaperFormat[it.key()];
					SetBtnChecked(bg, Id(paper.format.GetFormat()));
				}
				if (!isStdFormat) {
					using enum EPaperType;
					const bool isSRA3Enabled =
						paper.format == SRA3		or
						paper.eType == ColorCopy	or
						paper.eType == Monotex		or
						paper.eType == Raflatac		or
						paper.IsCustomType();

					const bool isA3Enabled =
						paper.format == A3			or
						paper.eType == NewsPaper	or
						paper.eType == Offset		or
						paper.IsCustomType();

					const bool is330x487Enabled =
						paper.format == _330x487	or
						!(paper.eType == NewsPaper	or
						  paper.eType == ColorCopy	or
						  paper.eType == Monotex	or
						  paper.eType == Raflatac)	or
						paper.IsCustomType();

					Q_ASSERT(isSRA3Enabled or isA3Enabled or is330x487Enabled);

					ui->rbFormatSRA3->		setEnabled(isSRA3Enabled);
					ui->rbFormatSRA3pc->	setEnabled(isSRA3Enabled);
					ui->rbFormatA3->		setEnabled(isA3Enabled);
					ui->rbFormat330x487->	setEnabled(is330x487Enabled);
					ui->rbFormat330x487pc->	setEnabled(is330x487Enabled);
				}
			}
			break;

			case PaperFormatCustom:
				paper.format.SetFormat(CustomFormat,
									   ui->sbPaperFormatWidth->value(),
									   ui->sbPaperFormatHeight->value());
				break;

			default:
				Q_ASSERT(state->defaultFormat != WrongFormat);
				paper.format = state->defaultFormat;
				setByDefault = true;
				break;
		}

		Q_ASSERT(!paper.format.IsWrong());
		const auto isStdFormat = paper.format.IsStandard();
		const auto isDigitalFormat = paper.format.IsDigital();
		const auto isCustomFormat = !isStdFormat and !isDigitalFormat;
		Q_ASSERT(isCustomFormat == paper.format.IsCustom());

		if (isStdFormat)
			ui->rbStandardFormats->setChecked(true);
		else
			if (isDigitalFormat)
				ui->rbDigitalFormats->setChecked(true);
			else
				ui->rbCustomFormat->setChecked(true);

		EnableAndCheckButtonsFromState(Group::StdFormat,
									   isStdFormat,
									   setByDefault);
		EnableAndCheckButtonsFromState(Group::DigitalFormat,
									   isDigitalFormat,
									   setByDefault);

//		const bool enableDigitalGroupBtn =
//			paper.IsDigitalType()	or
//			paper.IsCustomType()	or
//			(paper.IsStdType() and
//			 (paper.format == _70x100	or
//			  paper.format == _72x104	or
//			  paper.format == _50x70	or
//			  paper.format == _50x70pc	or
//			  paper.format.IsDigital()	or
//			  paper.format.IsCustom()));
//		ui->rbDigitalFormats->setEnabled(enableDigitalGroupBtn);
		ui->rbStandardFormats->setEnabled(paper.IsStdType() and
										  paper.eType != NewsPaper);
		ui->labelX->setEnabled(isCustomFormat);
		ui->sbPaperFormatWidth->setEnabled(isCustomFormat);
		ui->sbPaperFormatHeight->setEnabled(isCustomFormat);
	}

	// Устанавливаем долю листа
	if (stateKind <= PaperPortion) {
		if (userAction) {
			if (stateKind == PaperPortion)
				paper.ePortion =
					EnumCast<EPortion>(ui->bgSheetPortion->checkedId());
			else {
				// Допустимые доли листа
				QSet<EPortion> allowedSP;
				if (stateKind < EStateKind::PaperPortion) {
					using enum EPaperFormat;
					using enum EPortion;
					switch (paper.format.eFormat) {
						case _64x90:
						case _70x100:
						case _72x104:
							allowedSP << Portion2 << Portion4 << Portion6;
							paper.ePortion = Portion4;
							break;

						case _43x61:
						case _50x70:
						case _50x70pc:
							allowedSP << Portion1 << Portion2;
							paper.ePortion = Portion2;
							break;

						case A3:
						case SRA3:
						case _330x487:
						case SRA3pc:
						case _330x487pc:
							allowedSP << Portion1;
							paper.ePortion = Portion1;
							break;

						default:
							allowedSP	<< Portion1 << Portion2
										<< Portion4 << Portion6;
							paper.ePortion = Portion4;
							break;
					}
				}

				SetBtnChecked(ui->bgSheetPortion,
							  enum_integer(paper.ePortion));
				for (const auto btn : ui->bgSheetPortion->buttons()) {
					const auto id = ui->bgSheetPortion->id(btn);
					const auto portion = EnumCast<EPortion>(id);
					btn->setEnabled(allowedSP.contains(portion));
				}
			}
		} else {
			EnableAndCheckButtonsFromState(Group::Portion, true, false);
			SetBtnChecked(ui->bgSheetPortion, enum_integer(paper.ePortion));
		}
	}

	// Устанавливаем подрезку края
	if (stateKind <= CuttingEdge) {
		if (stateKind != CuttingEdge) {
			Q_ASSERT(!paper.format.IsWrong());
			paper.bCuttingEdge =
				!paper.format.IsDigital() and !paper.format.IsCustom();
			ui->cbCuttingEdge1mm->setChecked(paper.bCuttingEdge);
			ui->cbCuttingEdge1mm->setEnabled(!paper.format.IsDigital());
		}

		if (!paper.format.IsCustom()) {
			const auto & size = paper.format.GetSize();
			ui->sbPaperFormatWidth->setValue(size.width());
			ui->sbPaperFormatHeight->setValue(size.height());
		}

		const auto & printSize = paper.printSize;
		ui->leFinalFormat->setText(S("%1 x %2 мм").arg(printSize.width())
												  .arg(printSize.height()));
	}

	// Устанавливаем граммаж бумаги
	if (stateKind <= PaperWeightCustom) {
		const bool isAdhesive = paper.IsAdhesive();
		bool isStandardWeight;

		switch (stateKind) {
			case PaperWeightCustom:
				paper.weight = ui->sbCustomWeight->value();
				isStandardWeight = false;
				break;

			case PaperWeightStd:
				paper.weight = ui->bgStandardPaperWeight->checkedId();
				//if (paper.weight == -1)
				//	paper.weight = state->defaultWeight;
				//else {
				//	const auto iWeight = stdPaperWeights.indexOf(paper.weight);
				//	Q_ASSERT(iWeight != -1);
				//	const auto index = iWeight + EnumIndex(iMinWeight);
				//	const auto key = enum_value<EIndexStates>(index);
				//	if (!state->flags[key]) {
				//		paper.weight = state->defaultWeight;
				//		SetBtnChecked(ui->bgStandardPaperWeight, paper.weight);
				//	}
				//}
				isStandardWeight = true;
				EnableAndCheckButtonsFromState(Group::Weight, true);
				break;

			default:
				if (userAction)
					paper.weight = state->defaultWeight;
				isStandardWeight = paper.IsStdWeight();
				EnableAndCheckButtonsFromState(Group::Weight,
											   isStandardWeight,
											   isStandardWeight and
											   !isAdhesive);
				break;
		}

		if (isAdhesive) {
			Q_ASSERT(!paper.weight);
			ui->sbCustomWeight->setRange(0, 0);
			ui->sbCustomWeight->setSpecialValueText(S("(нет)"));
		} else {
			ASSERT_RANGE(paper.weight, PAPER_WEIGHT);
			ui->sbCustomWeight->setRange(MIN_PAPER_WEIGHT,
										 MAX_PAPER_WEIGHT);
			ui->sbCustomWeight->setSpecialValueText({ });
		}

		ui->rbStandardWeight->setChecked(isStandardWeight);
		ui->rbStandardWeight->setEnabled(!isAdhesive and
										 paper.eType != NewsPaper);
		if (!isStandardWeight)
			DisableButtonGroup(ui->bgStandardPaperWeight);
		ui->sbCustomWeight->setValue(paper.weight);
		ui->rbCustomWeight->setEnabled(!isAdhesive);
		ui->sbCustomWeight->setEnabled(!isAdhesive and !isStandardWeight);

		CheckPaperPrice();
	}

	const bool isPaperWeightSignal =	stateKind == PaperWeightStd or
										stateKind == PaperWeightCustom;
	if (stateKind == NoPrint) {
		imp.ePress = EPress::WrongPress;
		imp.ownBack.reset(); //imp.eOwnBack = EOwnBack::Forbidden;
		if (!ui->cbNoPrint->isChecked())
			ui->cbNoPrint->setChecked(true);
		SetBtnChecked(ui->bgInksFace, 0);
		SetBtnChecked(ui->bgInksBack, 0);
	}

	// Устанавливаем печатную машину
	if (stateKind <= Press and !isPaperWeightSignal) {
		using enum EPress;
		if (stateKind == Press)
			imp.ePress = EnumCast<EPress>(ui->bgPress->checkedId());

		// Допустимые печатные машины
		QSet<EPress> allowedPresses;
		const bool setPress =
			stateKind == PaperTypeStd		or
			stateKind == PaperTypeDigital	or
			stateKind == PaperFormatStd		or
			stateKind == PaperFormatDigital	or
			stateKind == PaperPortion;

		if (imp.ePress != WrongPress) {
			if (p.IsSizeLargerB3() or paper.IsPrintSizeLargerB3()) {
				allowedPresses << Roland << Heidelberg;
				if (setPress)
					imp.ePress = Roland;
			} else
				if (p.IsSizeLargerDigital()				or
					paper.IsPrintSizeLargerDigital()	or
					!paper.IsPrintSizeLargerA3())
				{
					allowedPresses << Ryobi;
					if (!paper.IsPrintSizeLargerA3()) {
						allowedPresses << Riso;
						if (setPress)
							imp.ePress = Riso;
					} else
						if (setPress)
							imp.ePress = Ryobi;
				} else {
					if (Fit({ 448, 318 }, paper.GetPrintSize()) == EFit::Equal)
						allowedPresses << Konica;
					if (paper.format.IsDigital()) {
						if (setPress)
							imp.ePress = Konica;
					} else {
						allowedPresses << Ryobi;
						if (setPress)
							imp.ePress = Ryobi;
					}
				}
		}

		for (auto button : ui->bgPress->buttons()) {
			const auto ePress = EnumCast<EPress>(ui->bgPress->id(button));
			button->setEnabled(allowedPresses.contains(ePress));
			button->setChecked(ePress == imp.ePress);
		}
	}

	// Устанавливаем кол-во красок
	if (stateKind <= OwnBack and !isPaperWeightSignal) {
		using enum EPress;
		using SetInks = QSet<int>;
		static const SetInks set234	{ 2, 3, 4, };
		static const SetInks set23	{ 2, 3, };

		auto CheckAndEnableInks = [this] {
			auto DisableInksButtons = [this] (bool face,
											  const SetInks & disabled) {
				const auto bg = face ? ui->bgInksFace : ui->bgInksBack;
				for (SetInks::value_type i = 0; i < bg->buttons().count(); ++i)
					bg->button(i)->setEnabled(!disabled.contains(i));
			};

			switch (imp.ePress) {
				case Konica:
					Q_ASSERT(imp.IsOwnBackForbidden());
					DisableInksButtons(true, set23);
					DisableInksButtons(false, set23);
					break;

				case Riso:
					DisableInksButtons(true, set234);
					if (!imp.IsOwnBack())
						DisableInksButtons(false, set234);
					break;

				case WrongPress:
					DisableButtonGroup(ui->bgInksFace);
					DisableButtonGroup(ui->bgInksBack);
					break;

				default:
					EnableButtonGroup(ui->bgInksFace);
					EnableButtonGroup(ui->bgInksBack);
					break;
			}
		};

		ui->lblInksFace->		setEnabled(!imp.IsNoPrint());
		ui->lblInksBack->		setEnabled(!imp.IsNoPrint());
		ui->lblInksPlus->		setEnabled(!imp.IsNoPrint());
		ui->sbPreparedSheets->	setEnabled(!imp.IsNoPrint());
		ui->gbPrintingPress->	setEnabled(!imp.IsNoPrint());

		switch (stateKind) {
			case InksFace:
				imp.inks.face = ui->bgInksFace->checkedId();
				Q_ASSERT((SetInks { 1, 2, 3, 4, }
							.contains(imp.inks.face)));
				if (imp.inks.back != imp.inks.face) {
					imp.inks.back = imp.inks.face;
					SetBtnChecked(ui->bgInksBack, imp.inks.back);
				}
				break;

			case InksBack:
				imp.inks.back = ui->bgInksBack->checkedId();
				if (!imp.inks.back and imp.IsOwnBack())
					imp.ownBack = false;
				Q_ASSERT(!paper.IsAdhesive() or
						 (paper.IsAdhesive() and !imp.inks.back));
				break;

			case OwnBack:
				imp.ownBack = ui->cbOwnBack->isChecked();
				if (imp.IsOwnBack()) {
					DisableButtonGroup(ui->bgInksBack);
					ui->rbInksBack0->setEnabled(true);
 					if (imp.inks.back != imp.inks.face) {
						imp.inks.back = imp.inks.face;
						SetBtnChecked(ui->bgInksBack, imp.inks.back);
						for (int i = 4; i > imp.inks.face; --i)
							ui->bgInksBack->button(i)->setEnabled(false);
					}
				} else
					CheckAndEnableInks();
				break;

			default:
				if (userAction) {
					imp.inks.face =
						imp.IsNoPrint() ? 0 : (imp.ePress == Riso ? 1 : 4);
					imp.inks.back = 0;
					imp.ownBack.reset();
				}
				CheckAndEnableInks();
				SetBtnChecked(ui->bgInksFace, imp.inks.face);
				SetBtnChecked(ui->bgInksBack, imp.inks.back);
				break;
		}
	}

	if (stateKind < PrepSheets)
		imp.prepSheets.reset();

	RecalcCost();

	ui->cbOwnBack->setEnabled(imp.signatures.count() == 1 and
							  imp.IsOwnBackAllowed());
	ui->cbOwnBack->setChecked(imp.IsOwnBack());

	if (stateKind <= RenderImp and
		!isPaperWeightSignal and
		currentPage() == ui->pageInksAndImposition)
	{
		ui->sbPlaces->			setValue(imp.GetTotalPlaces());
		ui->lblPrintSheets->	setText(t(imp.signatures.printSheets));
		ui->lblFormatSheets->	setText(t(imp.signatures.sourceSheets));
		ui->sbPreparedSheets->	setValue(int(imp.signatures.prepSheets));
		ui->sbPreparedSheets->	setEnabled(imp.signatures.count() == 1);
		ui->sbHorizIndent->		setMaximum(imp.maxIndents.width());
		ui->sbHorizIndent->		setValue(*imp.horizIndent);
		ui->sbVertIndent->		setMaximum(imp.maxIndents.height());
		ui->sbVertIndent->		setValue(*imp.vertIndent);
		ui->sbGap->				setMaximum(*imp.bleed * 2.0);
		ui->sbGap->				setValue(*imp.gap);
		ui->lblBleed->			setText(S("%1 мм").arg(t(*imp.bleed, 1)));
		this->RenderImp();
	}

	if (!userAction) {
		ui->cbCreasing->setChecked(bool(pp.creasings));
		ui->sbCreasings->setValue(pp.creasings.value_or(0));

		ui->cbFolding->setChecked(bool(pp.foldings));
		ui->sbFoldings->setValue(pp.foldings ? pp.foldings->count : 0);
		ui->cbFoldByHand->setChecked(pp.foldings ?
									 pp.foldings->foldByHand : true);

		ui->cbPerforation->setChecked(bool(pp.perforations));
		ui->sbPerforations->setValue(pp.perforations.value_or(0));

		ui->cbGluePoint->setChecked(bool(pp.gluePoints));
		ui->sbGluePoints->setValue(pp.gluePoints.value_or(0));

		ui->cbDoubleSidedTape->setChecked(bool(pp.dblSidedTape));
		ui->sbDoubleSidedTape->setValue(pp.dblSidedTape ?
										pp.dblSidedTape->count : 0);
		ui->sbDblSidedTapeLenght->setValue(pp.dblSidedTape ?
										   pp.dblSidedTape->length : 0);
		ui->bgDblSidedTape->idClicked(pp.dblSidedTape ?
									  pp.dblSidedTape->width : 5);

		ui->cbScrathStrip->setChecked(bool(pp.scrathStrips));
		ui->sbScrathStrips->setValue(pp.scrathStrips.value_or(0));

		ui->cbRoundedCorners->setChecked(bool(pp.rndCorners));
		ui->sbRoundedCorners->setValue(pp.rndCorners ?
									   pp.rndCorners->count : 4);
		ui->sbRoundedCornersRadius->setValue(pp.rndCorners ?
											 pp.rndCorners->radius : 0);

		ui->cbEyelets->setChecked(bool(pp.eyelets));
		ui->sbEyelets->setValue(pp.eyelets ?
								pp.eyelets->count : 0);
		ui->sbEyeletsDiameter->setValue(pp.eyelets ? pp.eyelets->diam : 0);

		ui->cbHoles->setChecked(bool(pp.holes));
		ui->sbHoles->setValue(pp.holes ? pp.holes->count : 0);
		ui->sbHolesDiameter->setValue(pp.holes ? pp.holes->diam : 0);

		ui->cbNumeration->setChecked(bool(pp.nums));
		ui->sbStartNumber->setValue(pp.nums ? pp.nums->start : 0);
		ui->sbEndNumber->setValue(pp.nums ? pp.nums->end : 100);
		ui->sbNumZeroes->setValue(pp.nums ? pp.nums->digits : 0);

		ui->cbDieCutting->setChecked(bool(pp.dieCutting));
		ui->lblDieName->setEnabled(bool(pp.dieCutting));
		ui->lblDieCount->setEnabled(bool(pp.dieCutting));
		ui->sbDieCount->setEnabled(bool(pp.dieCutting));
		ui->sbDieCount->setValue(pp.dieCutting ? pp.dieCutting->count : 0);
		if (pp.dieCutting)
			ui->leDieName->setText(pp.dieCutting->sName);
	}

	canChange = true;
}


void ImpWizard::SetLamPrice() {
	bool oldChange = canChange;
	bool changeThickness = oldLamType != lam->eType;
	if (oldLamType != lam->eType)
		oldLamType = lam->eType;
	if (!*lam->thickness or changeThickness) {
		lam->thickness = defaultLamThickness[lam->eType];
		canChange = false;
		SetIndexByDataFromCombobox(ui->comboLamThickness, *lam->thickness);
		canChange = oldChange;
	}

	lam->FillPrice(GetLamSheets());
	canChange = false;
	ui->sbLamPrice->setValue(lam->price.value_or(0));
	ui->sbLamPrice->setEnabled(!lam->price or qFuzzyIsNull(*lam->price));
	canChange = oldChange;
}


void ImpWizard::RecalcCost() {
	p.CalcCost();
	logWindow->ShowText(imp.GetLog());
}


inline size_t ImpWizard::GetLamSheets() {
	Q_ASSERT(lam);
	if (lam->eType == ELamType::Packet)
		return p.kinds.GetCopies();
	else {
		auto sheets = imp.signatures.printSheets;
		if (lam->bBack)
			sheets *= 2;
		return sheets;
	}
}


void ImpWizard::ShowErrorInvalidSheetPortion() {
	MsgBox mb(this);
	mb.Error("Недопустимая доля листа\n"
			 "для данного формата!");
	ui->bgSheetPortion->button(enum_integer(paper.ePortion))
			->setChecked(true);
}


void ImpWizard::CheckPaperPrice() {
	const auto type =
			paper.eType == EPaperType::CoatedMatte ? EPaperType::CoatedGlossy :
													 paper.eType;
	paper.price = paperPrices.GetPrice(type,
									   paper.format.size,
									   paper.weight);
	ui->sbPaperPrice->setValue(paper.price);
	ui->sbPaperPrice->setEnabled(qFuzzyIsNull(paper.price));
}


void ImpWizard::SetPacketLam(bool checked) {
	ui->cbLamBack->setEnabled(!checked);
	if (checked)
		SetLamination(ELamType::Packet);
}


void ImpWizard::SetSpecialLam(bool checked) {
	ui->comboLamSpecial->setEnabled(checked);
	if (checked)
		SetLamination(ELamType::LamCustom);
}


void ImpWizard::EnableFoilWidgets(bool enable, bool backOnly) {
	if (!backOnly) {
		ui->lblFoilWidthFace->setEnabled(enable);
		ui->lblFoilHeightFace->setEnabled(enable);
		ui->lblFoilColorFace->setEnabled(enable);
		ui->sbFoilWidthFace->setEnabled(enable);
		ui->sbFoilHeightFace->setEnabled(enable);
		ui->comboFoilColorFace->setEnabled(enable);
	}

	const bool backEnabled = enable and ui->cbFoilBack->isChecked();
	ui->lblFoilWidthBack->setEnabled(backEnabled);
	ui->lblFoilHeightBack->setEnabled(backEnabled);
	ui->lblFoilColorBack->setEnabled(backEnabled);
	ui->sbFoilWidthBack->setEnabled(backEnabled);
	ui->sbFoilHeightBack->setEnabled(backEnabled);
	ui->comboFoilColorBack->setEnabled(backEnabled);
}


void ImpWizard::CalculateSampleNum() {
	const auto nDigits = ui->sbNumZeroes->value();
	const auto nValue = (ui->sbStartNumber->value() +
						 ui->sbEndNumber->value()) / 2;
	ui->leSampleNum->setText(S("%1").arg(nValue, nDigits, 10, QChar(u'0')));
}


ImpWizard::~ImpWizard() {
	delete ui;
}
