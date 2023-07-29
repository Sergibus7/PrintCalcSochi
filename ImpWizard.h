#pragma once

#include <QWizard>
#include <QGraphicsScene>
#include <QSpinBox>
#include <QPushButton>
#include "LogWindow.h"
#include "Product.h"

namespace Ui {
class ImpWizard;
}


// Индексы (столбцы) таблицы состояний
enum class EIndexStates : size_t {
	iPaperType,
	iDefaultWeight,
	iLessA2,

	i64x90,
	i70x100,
	i72x104,
	i43x61,
	i50x70,
	i50x70pc,
	iA3,
	iSRA3,
	iSRA3pc,
	i330x487,
	i330x487pc,

	iPortion1,
	iPortion2,
	iPortion4,
	iPortion6,

	i70,
	i80,
	i90,
	i100,
	i105,
	i115,
	i120,
	i130,
	i140,
	i150,
	i157,
	i160,
	i170,
	i180,
	i200,
	i230,
	i250,
	i265,
	i270,
	i280,
	i300,

	iKonica,
	iRyobi,
	iRoland,
	iHeidelberg,
	iRiso,

	iDummy,
};

_CONST(auto,	iMinStdFormat		, EIndexStates::i64x90			);
_CONST(auto,	iMaxStdFormat		, EIndexStates::i50x70pc		);
_CONST(auto,	iMinFormat			, iMinStdFormat					);
_CONST(auto,	iMinDigitalFormat	, EIndexStates::iA3				);
_CONST(auto,	iMaxDigitalFormat	, EIndexStates::i330x487pc		);
_CONST(auto,	iMaxFormat			, iMaxDigitalFormat				);
_CONST(auto,	iMinPortion			, EIndexStates::iPortion1		);
_CONST(auto,	iMaxPortion			, EIndexStates::iPortion6		);
_CONST(auto,	iMinWeight			, EIndexStates::i70				);
_CONST(auto,	iMaxWeight			, EIndexStates::i300			);
_CONST(auto,	iMinPress			, EIndexStates::iKonica			);
_CONST(auto,	iMaxPress			, EIndexStates::iRiso			);
_CONST(auto,	INDEXES_COUNT		, enum_count<EIndexStates>() - 1);

constexpr EIndexStates & operator++(EIndexStates & value) {
	Q_ASSERT(enum_contains(value));
	const auto index = EnumIndex(value) + 1;
	Q_ASSERT(index > 0 and index <= INDEXES_COUNT);
	return value = enum_value<EIndexStates>(index);
}

constexpr underlying_type_t<EIndexStates> operator-(EIndexStates lhs,
													EIndexStates rhs)
{
	const int64_t result = EnumIndex(lhs) - EnumIndex(rhs);
	Q_ASSERT(result >= 0 and result < 10000);
	return result;
}

constexpr inline int Id(auto _enum) {
	return EnumIndex(_enum);
};


// Состояние
struct State {
	EPaperType					paperType;
	EPaperFormat				defaultFormat;
	EPortion					defaultPortion;
	SizeT						defaultWeight;
	EPress						defaultPress;
	bool						isLessA2;
	QMap<EIndexStates, bool>	flags;
};


enum class EInksSetup {
	DefaultInks,		// Установить значения красок по умолчанию
	Face,				// Установить кол-во красок лица
	Back,				// Установить кол-во красок оборота
	NotOwnBack,			// Запретить свой оборот
	SetOwnBack,			// Установить свой оборот
};


struct InksSetup {
	EInksSetup eSetup = EInksSetup::DefaultInks;
	Inks inks;
};


enum class EStateKind {
	Init,				// Инициализация UI: все значения берем из модели
	PaperTypeStd,		// Устанавливаем стандартный тип бумаги
	PaperTypeDigital,	// Устанавливаем цифровой тип бумаги
	PaperTypeCustom,	// Устанавливаем дополнительный тип бумаги
	PaperFormatStd,		// Устанавливаем стандартный формат бумаги
	PaperFormatDigital,	// Устанавливаем цифровой формат бумаги
	PaperFormatCustom,	// Устанавливаем дополнительный формат бумаги
	PaperPortion,		// Устанавливаем долю листа
	CuttingEdge,		// Устанавливаем подрезку края бумаги
	PaperWeightStd,		// Устанавливаем стандартный граммаж бумаги
	PaperWeightCustom,	// Устанавливаем дополнительный граммаж бумаги
	NoPrint,			// Устанавливаем режим "без печати"
	Press,				// Устанавливаем печатную машину
	InksFace,			// Устанавливаем красочность печати (лицо)
	InksBack,			// Устанавливаем красочность печати (оборот)
	OwnBack,			// Устанавливаем свой оборот
	PrepSheets,			// Устанавливаем кол-во приладочных листов
	RecalcImp,			// Пересчитываем раскладку на печатном листе
	RenderImp,			// Отображение спуска
	RecalcCostImp,		// Пересчитываем стоимость спуска
};


class ImpWizard : public QWizard {
	Q_OBJECT

	Ui::ImpWizard *	ui;

	Imposition &		imp;
	Product &			p;
	Paper &				paper;
	Postpress &			pp;
	Opt<Lamination> &	lam;
	Opt<Foil> &			foil;
	QGraphicsScene		scene;

	// Разрешить изменение значений
	bool canChange = true;
	bool add;
	bool bPriceCanChange = true;
	const QString sDefaultTitle = S("Параметры спуска");
//	Opt<bool> oldPaperTypeIsStd;
//	Opt<EPaperFormatGroup> oldPaperFormatGroup;
	ELamType oldLamType = ELamType::Offset;
//	EPress oldPress = EPress::WrongPress;
	LogWindow * logWindow = nullptr;
	bool moveMeByLogWindow;
	int oldCustomWidth = 0;
	int oldCustomHeight = 0;
//	Opt<decltype(paper.weight)> oldWeight;

	bool validateCurrentPage() override;
	void RenderImp();
	void ChangeState(EStateKind stateKind);
	void SetLamPrice();
	void RecalcCost();
	void SetLamination(ELamType type);
	inline size_t GetLamSheets();
	void CalculateSampleNum();
	void ShowErrorInvalidSheetPortion();
	void CheckPaperPrice();
	void SetPacketLam(bool checked);
	void SetSpecialLam(bool checked);
	void EnableFoilWidgets(bool enable, bool backOnly = false);

	void SetValueSilent(QSpinBox * sb, int value) {
		Q_ASSERT(value < INT_MAX);
		bool oldProcess = canChange;
		canChange = false;
		sb->setValue(value);
		canChange = oldProcess;
	}

	void initializePage(int id) override;

public:
	ImpWizard(Imposition * imposition, bool add, QWidget * parent);
	~ImpWizard();
	int exec() override;

	static bool FitOnPaper(Product & p, Imposition & imp, bool bAdd);
};


using MapIndexPaperFormat	= QMap<EIndexStates, EPaperFormat>;
using MapIndexSheetPortion	= QMap<EIndexStates, EPortion>;
using MapIndexPaperWeight	= QMap<EIndexStates, SizeT>;
using MapIndexPress			= QMap<EIndexStates, EPress>;

extern MapIndexPaperFormat &	mapIndexPaperFormat;
extern MapIndexSheetPortion &	mapIndexSheetPortion;
extern MapIndexPaperWeight &	mapIndexPaperWeight;
extern MapIndexPress &			mapIndexPress;
