#pragma once

#include <QAbstractItemModel>
#include "Globals.h"


// Макс. толщина ламинации
_CONST(qreal,	MAX_LAM_PRICE			, 10000		)
// Мин. толщина ламинации
_CONST(SizeT,	MIN_LAM_THICKNESS		, 10		)
// Макс. толщина ламинации
_CONST(SizeT,	MAX_LAM_THICKNESS		, 500		)
// Макс. кол-во изделий
_CONST(SizeT,	MAX_PRODUCTS			, 100		)
// Мин. размер фольгирования в ширину или высоту
_CONST(SizeT,	MIN_FOIL_SIZE			, 10		)
// Макс. размер фольгирования в ширину или высоту
_CONST(SizeT,	MAX_FOIL_SIZE			, 300000	)
// Макс. размер изделия в ширину или высоту
_CONST(SizeT,	MIN_PRODUCT_SIZE		, 10		)
// Макс. размер изделия в ширину или высоту
_CONST(SizeT,	MAX_PRODUCT_SIZE		, 720		)
// Мин. размер бумаги в ширину или высоту
_CONST(SizeT,	MIN_PAPER_SIZE			, 100		)
// Макс. размер бумаги в ширину или высоту
_CONST(SizeT,	MAX_PAPER_SIZE			, 1050		)
// Мин. граммаж бумаги
_CONST(SizeT,	MIN_PAPER_WEIGHT		, 30		)
// Макс. граммаж бумаги
_CONST(SizeT,	MAX_PAPER_WEIGHT		, 500		)
// Кол-во печатных листов офсетных машин в час
_CONST(qreal,	SHEETS_PER_HOUR			, 5000		)
// Макс. кол-во печатных листов на комплект форм
_CONST(qreal,	LIMIT_PER_PLATE			, 50000		)
// Кол-во часов на приладку одной стороны при печати в 1 или 2 краски
_CONST(qreal,	PREP_HOURS_BW			, 0.25		)
// Кол-во часов на приладку одной стороны при печати в 3 или 4 краски
_CONST(qreal,	PREP_HOURS_FULL_COLOR	, 0.5		)


template<typename T>
QDataStream & operator <<(QDataStream & ds, const Opt<T> & opt) {
	const auto hasValue = bool(opt);
	ds << hasValue;
	if (hasValue)
		ds << *opt;
	return ds;
}

template<typename T>
QDataStream & operator >>(QDataStream & ds, Opt<T> & opt) {
	bool hasValue;
	ds >> hasValue;
	if (hasValue) {
		T t;
		ds >> t;
		opt = t;
	} else
		opt.reset();
	return ds;
}


// Вид изделия
enum class EProductKind: size_t {
	Simple,
	Brochure,
	Notepad,
	Book,
	Newspaper,
	Menu,
	QuartCalend,
	WallCalend
};


// Размер изделия
enum class EProductSize: size_t {
	SizeA7,
	SizeA6,
	SizeA5,
	SizeA4,
	SizeA3,
	SizeA2,
	Size210x98,
	Size90x50,
	CustomSize,
};
_CONST(auto, DEFAULT_PRODUCT_SIZE, EProductSize::SizeA4)


// Соответствия размеров изделия
struct StdProductSize {
	QString		name;
	QSize		size;
};


// Стандартные размеры изделий
class StdProductSizes : public QMap<EProductSize, StdProductSize> {
public:
	StdProductSizes() = default;

	StdProductSizes(std::initializer_list<std::pair<QMap::key_type,
					QMap::mapped_type>> list) : QMap(list) {
	}

	auto GetType(const QString & name) const {
		const auto it = std::ranges::find_if(*this, [name] (auto ps) {
			return name == ps.name;
		});
		return it == cend() ? EProductSize::CustomSize : it.key();
	}

	auto GetName(EProductSize eType) const {
		if (eType == EProductSize::CustomSize)
			CRITICAL("Неверный тип бумаги в StdProductSizes::GetName");
		const auto it = constFind(eType);
		Q_ASSERT(it != cend());
		return it->name;
	}

	auto GetSize(const QString & name) const {
		const auto it = std::ranges::find_if(*this, [name] (auto ps) {
			return name == ps.name;
		});
		return it == cend() ? EProductSize::CustomSize : it.key();
	}

	auto GetSize(EProductSize eps) const {
		if (eps == EProductSize::CustomSize)
			CRITICAL("Неверный размер изделия в StdProductSizes::GetSize");
		const auto it = constFind(eps);
		return it == cend() ? QSize() : it->size;
	}
};
extern StdProductSizes & stdProductSizes;


// Переплет изделия
enum class EBinding: size_t {
	NoBinding,
	SoftCover,
	Staples,
	Wire,
	Rings,
	Bolts
};


// Тип бумаги
enum class EPaperType: size_t {
	WrongPaper,
	CoatedGlossy,
	CoatedMatte,
	Offset,
	Adhesive,
	NewsPaper,
	Carton,
	Labels,
	Craft,
	ColorCopy,
	Linen,
	Shyne,
	SirioPearl,
	TouchCover,
	Monotex,
	Raflatac,
	CustomPaper,
};
_CONST(auto, DEFAULT_PAPER_TYPE, EPaperType::NewsPaper)


// Соответствия стандартных бумаг
struct StdPaperType {
	QString		name;
	bool		isDigitalOnly;
};


// Стандартные типы бумаг
class StdPaperTypes : public QMap<EPaperType, StdPaperType> {
public:
	StdPaperTypes() = default;

	StdPaperTypes(std::initializer_list<
				  std::pair<QMap::key_type, QMap::mapped_type>> list)
		: QMap(list)
	{ }

	auto GetType(const QString & name) const {
		const auto it = std::ranges::find_if(*this, [name] (auto pt) {
			return name == pt.name;
		});
		return it == cend() ? EPaperType::WrongPaper : it.key();
	}

	auto GetName(EPaperType eType) const {
		if (eType == EPaperType::WrongPaper)
			CRITICAL("Неверный тип бумаги в StdPaperTypes::GetName");
		const auto it = constFind(eType);
		Q_ASSERT(it != cend());
		return it->name;
	}
};
extern StdPaperTypes & stdPaperTypes;


// Формат бумаги
enum class EPaperFormat: size_t {
	WrongFormat = 11,
	_64x90,
	_70x100,
	_72x104,
	_43x61,
	_50x70,
	_50x70pc,	// для плот. резки, соответствует 50х70 1/2
	A3,
	SRA3,
	SRA3pc,		// для плот. резки, соответствует SRA3
	_330x487,
	_330x487pc,	// для плот. резки, соответствует 330х487
	CustomFormat,
};
_CONST(auto, DEFAULT_PAPER_FORMAT, EPaperFormat::A3)


// Соответствия стандартных форматов
struct StdPaperFormat {
	QString			name;
	QSize			size;
	bool			isDigitalFormat;
};


// Стандартные размеры бумаг
class StdPaperFormats : public QMap<EPaperFormat, StdPaperFormat> {
public:
	StdPaperFormats() = default;

	StdPaperFormats(std::initializer_list<std::pair<QMap::key_type,
					QMap::mapped_type>> list) : QMap(list) {
	}

	auto GetEFormat(const QString & name) const {
		const auto it = std::ranges::find_if(*this, [&name] (auto fmt) {
			return name == fmt.name;
		});
		return it == cend() ? EPaperFormat::WrongFormat : it.key();
	}

	//auto GetEFormat(const QString & name) const {
	//	const auto it = std::ranges::find_if(*this, [&name] (auto fmt) {
	//		return name == fmt.name;
	//	});
	//	return it == cend() ? EPaperFormat::WrongFormat : it.key();
	//}

	const auto GetSize(const QString & name) const {
		const auto it = std::ranges::find_if(*this, [&name] (auto fmt) {
			return name == fmt.name;
		});
		return it == cend() ? QSize() : it->size;
	}

	const auto GetName(EPaperFormat eFormat) const {
		if (eFormat == EPaperFormat::WrongFormat)
			CRITICAL("Неверный формат бумаги в StdPaperFormats::GetName");
		const auto it = constFind(eFormat);
		return it == cend() ? QString() : it->name;
	}

	const auto GetSize(EPaperFormat eFormat) const {
		if (eFormat == EPaperFormat::WrongFormat)
			CRITICAL("Неверный формат бумаги в StdPaperFormats::GetSize");
		const auto it = constFind(eFormat);
		return it == cend() ? QSize() : it->size;
	}
};
extern StdPaperFormats & stdPaperFormats;


// Доля листа
enum class EPortion: size_t {
	Portion1 = 1,
	Portion2 = 2,
	//Portion3 = 3,
	Portion4 = 4,
	Portion6 = 6,
	WrongPortion,
};


// Печатная машина
enum class EPress: size_t {
	Konica,
	Ryobi,
	Roland,
	Heidelberg,
	Riso,
	WrongPress,
};


// Виды ламинации
enum class ELamType : size_t {
//	LamNone,
	Offset,
	Digital,
	UltraBond,
	Packet,
	LamCustom,
};


//// Вид своего оборота
//enum class EOwnBack : size_t {
//	Forced,		// Свой оборот
//	NotOwn,		// Чужой оборот
//	Allowed,	// Свой оборот возможен
//	Forbidden,	// Запрет своего оборота (цифровая печать)
//};


// Тип спуска
enum class EImpType : size_t {
	Simple	= 9,
	Cover	= 11,
	Block,
	Bottom,
	Insert,
};


// Виды операций
enum class EOperation : size_t {
	PlateB3,
	PlateB2,
	RyobiMin_4_4,
	Ryobi_4_0,
	Ryobi_1_0,
	Riso_1_0,
	Riso_1_1,
	Konica_4_0,
	Konica_4_4,
	Konica_1_0,
	Konica_1_1,
	FoldAuto,
	FoldByHand,
	Creasing,
	Numbering,
	DieCutting,
	Perforation,
	Rounding,
	Eyelet,
	Hole,
	GluePoint,
	ScratchStrip,
	DoubleSidedTape5mm,
	DoubleSidedTape9mm,
	ImpAssembly,
	BindStaples,
	BindBolts,
	BindRings,
	BindWire,
	BindSoftCoverA4,
	BindSoftCoverA5,
	Cursor,
	Rigel,
	FoilSqMeter,
	FoilExtra,
	LamOffset,
	LamUltraBond,
	LamDigital,
	LamPacket,
	LamCustom,
	WrongOperation,
};


// Бумага
struct Paper {
public:
	class Format {
		using enum EPaperFormat;

		EPaperFormat	epf;	// формат
		SizeT			w;		// ширина в мм
		SizeT			h;		// высота в мм

		friend QDataStream & operator <<(QDataStream &, const Format &);
		friend QDataStream & operator >>(QDataStream &, Format &);

	public:
		void SetFormat(EPaperFormat eFormat,
					   SizeT width = 0,
					   SizeT height = 0);

		Format() {
			SetFormat(DEFAULT_PAPER_FORMAT);
		}

		inline EPaperFormat GetFormat() const {
			return epf;
		}

		inline QSize GetSize() const {
			Q_ASSERT(w < INT_MAX and h < INT_MAX);
			return { int(w), int(h) };
		}

		inline SizeT GetWidth() const {
			return w;
		}

		inline void SetWidth(SizeT newWidth) {
			Q_ASSERT(IsCustom());
			w = newWidth;
		}

		inline SizeT GetHeight() const {
			return h;
		}

		inline void SetHeight(SizeT newHeight) {
			Q_ASSERT(IsCustom());
			h = newHeight;
		}

		inline auto operator ==(EPaperFormat eFormat) const {
			return epf == eFormat;
		}

		auto operator ==(const Format & other) const {
			auto stdEqual = epf == other.epf;
			if (IsCustom())
				stdEqual and_eq w == other.w and h == other.h;
			return stdEqual;
		}

		inline Format & operator=(EPaperFormat eFormat) {
			SetFormat(eFormat);
			return *this;
		}

		bool IsStandard() const {
			return
				QSet {
					_64x90, _70x100, _72x104, _43x61, _50x70, _50x70pc,
				}.contains(epf);
		}

		inline bool IsDigital() const {
			return IsDigital(epf);
		}

		inline bool IsCustom() const {
			return epf == CustomFormat;
		}

		inline bool IsWrong() const {
			return epf == WrongFormat;
		}

		inline bool IsDigital() {
			return IsDigital(epf);
		}

		static bool IsDigital(EPaperFormat eFormat) {
			return
				QSet {
					A3, SRA3, _330x487, SRA3pc, _330x487pc,
				}.contains(eFormat);
		}

		bool IsValid() const {
			return CHECK_RANGE(w, PAPER_SIZE) and CHECK_RANGE(h, PAPER_SIZE);
		}

		PROP_RO(QSize,			size,		Size	)
		PROP_RW(EPaperFormat,	eFormat,	Format	)
		PROP_RW(SizeT,			width,		Width	)
		PROP_RW(SizeT,			height,		Height	)
	};

	EPaperType		eType			// Тип бумаги
		{ DEFAULT_PAPER_TYPE };
	Format			format;			// Формат бумаги
	EPortion		ePortion		// Доля листа, по умолчанию 1/1
		{ EPortion::Portion1 };
	bool			bCuttingEdge	// Подрезка краев листа по 1 мм
		{ false };
	QString			sCustomName;	// Название (для спец. бумаг)
	qreal			price			// Стоимость исх. листа
		{ 0.0 };
	SizeT			weight			// Граммаж
		{ 48 };

	// Возвращает размер печатного листа (например, 64х90 1/4 --> 448х318)
	QSize GetPrintSize() const;

	PROP_RO(QSize,	printSize,	PrintSize);

	inline bool IsPrintSizeLargerB3() const {
		return Fit({ 520, 360 }, printSize) == EFit::NotFit;
	}

	inline bool IsPrintSizeLargerDigital() const {
		return Fit({ 487, 330 }, printSize) == EFit::NotFit;
	}

	inline bool IsPrintSizeLargerA3() const {
		return Fit({ 420, 297 }, printSize) == EFit::NotFit;
	}

	inline auto IsCustomType() const {
		return eType == EPaperType::CustomPaper;
	}

	static const auto IsStdType(EPaperType eType) {
		//if (eType == EPaperType::WrongPaper)
		//	CRITICAL("Неверный тип бумаги в StdPaperTypes::IsDigital");
		const auto it = stdPaperTypes.constFind(eType);
		Q_ASSERT(it != stdPaperTypes.cend());
		return !it->isDigitalOnly;
	}

	inline auto IsStdType() const {
		return IsStdType(eType);
	}

	inline auto IsDigitalType() const {
		return !IsStdType(eType);
	}

	inline auto IsAdhesive() const {
		return eType == EPaperType::Adhesive or eType == EPaperType::Raflatac;
	}

	static bool IsStdWeight(decltype(weight) weight);

	inline auto IsStdWeight() const {
		return IsStdWeight(weight);
	}

	auto operator ==(const Paper & other) const {
		return
			eType			== other.eType			and
			format			== other.format			and
			ePortion		== other.ePortion		and
			bCuttingEdge	== other.bCuttingEdge	and
			sCustomName		== other.sCustomName	and
			price			== other.price			and
			weight			== other.weight;
	}

	bool IsValid() const {
		using enum EPaperType;
		return
			eType != WrongPaper and
			format.IsValid() and
			((weight > 0 and eType != Adhesive) or
			 (weight == 0 and eType == Adhesive));
	}
};


// Соответствие граммаж бумаги --> цена
class PaperWeightPrices : protected QHash<size_t, qreal> {
	friend class PaperPricesByFormat;
	friend class MainWindow;

	qreal GetPrice(size_t weight) const {
		return value(weight, 0.0);
	}

	void AddPrice(size_t weight, qreal price) {
		Q_ASSERT(!contains(weight));
		insert(weight, price);
	}
};


// Соответствие размер бумаги --> граммаж
class PaperPricesByFormat : protected QHash<QSize, PaperWeightPrices> {
	friend class PaperPricesByType;
	friend class MainWindow;

	void AddPrice(const QSize & size, size_t weight, qreal price);
	qreal GetPrice(const QSize & size, size_t weight) const;
};


// Соответствие тип бумаги --> размеры
class PaperPricesByType : protected QMap<EPaperType, PaperPricesByFormat> {
	friend class MainWindow;

public:
	void AddPrice(EPaperType type, const QSize & size,
				  size_t weight, qreal price);
	qreal GetPrice(EPaperType type, const QSize & size, size_t weight);

	inline bool contains(EPaperType type) const {
		return QMap::contains(type);
	}
};


// Ламинация
struct Lamination {
	ELamType		eType			// Тип ламинации
		{ ELamType::Offset };
	bool			bGloss			// Глянцевая или матовая
		{ true };
	EProductSize	eSize			// Размер пакета
		{ DEFAULT_PRODUCT_SIZE };
	QString			sCustomName;	// Название нестанд. лам.
	Opt<int>		thickness;		// Толщина в мкм
	bool			bBack			// Ламинировать оборот
		{ false };
	Opt<qreal>		price;			// Стоимость за формат А3

	void FillPrice(size_t sheets);

	inline bool IsSpecial() const {
		return eType == ELamType::LamCustom;
	}

	auto operator ==(const Lamination & other) const {
		return 
			eType				== other.eType				and
			bGloss				== other.bGloss				and
			eSize				== other.eSize				and
			sCustomName			== other.sCustomName		and
			thickness			== other.thickness			and
			bBack				== other.bBack				and
			price				== other.price;
	}

//#ifdef _DEBUG
//	bool IsValid() const {
//		using enum ELamType;
//		if (!IsLam())
//			return true;
//		return
//			((eType == LamCustom and !sCustomName.isEmpty()) or
//			 (eType != LamCustom and sCustomName.isEmpty())) and
//			thickness and
//			thickness >= MIN_LAM_THICKNESS and
//			thickness <= MAX_LAM_THICKNESS and
//			price and price > 0.0 and price < 100000.0;
//	}
//#endif // _DEBUG
};


// Фольгирование
struct FoilSide {
	int		width;		// Ширина фольгирования в мм
	int		height;		// Высота фольгирования в мм

	FoilSide() {
		width = height = 0;
	}

	FoilSide(auto w, auto h) {
		SetFoil(w, h);
	}

	void SetFoil(auto w, auto h) {
		ASSERT_RANGE(w, FOIL_SIZE);
		ASSERT_RANGE(h, FOIL_SIZE);
		width = w;
		height = h;
	}

	qreal GetSquareMeters() const {
		return (width / 1000.) * (height / 1000.);
	}

	auto operator ==(const FoilSide & other) const {
		return
			width	== other.width	and
            height	== other.height;
	}
};


struct Foil {
	Opt<FoilSide> face;
	Opt<FoilSide> back;

	inline auto operator ==(const Foil & other) const {
		return face == other.face and back == other.back;
	}

//#ifdef _DEBUG
//	bool IsValid() const {
//		return face.IsFoil() or (!face.IsFoil() and !back.IsFoil());
//	}
//#endif // _DEBUG
};


// Постпечатная обработка
struct Postpress {
	struct Folding {
		int count = 0;			// Кол-во фальцовок
		bool foldByHand = true;	// Фальцовка вручную

		inline auto operator ==(const Folding & other) const {
			return count == other.count and foldByHand == other.foldByHand;
		}
	};

	struct DblSidedTape {
		int count = 0;		// Кол-во двустор. скотчей
		int length = 0;		// Длина скотча в мм
		int width = 0;		// Ширина скотча в мм

		inline auto operator ==(const DblSidedTape & other) const {
			return
				count	== other.count	and
				length	== other.length	and
				width	== other.width;
		}
	};

	struct RndCorners {
		int count = 0;		// Кол-во скруглений
		int radius = 0;		// Радиус скруглений

		inline auto operator ==(const RndCorners & other) const {
			return count == other.count and radius == other.radius;
		}
	};

	struct Eyelets {
		int count = 0;		// Кол-во люверсов
		int diam = 0;		// Диаметр люверсов

		inline auto operator ==(const Eyelets & other) const {
			return count == other.count and diam == other.diam;
		}
	};

	struct Holes {
		int count = 0;		// Кол-во отверстий
		int diam = 0;		// Диаметр отверстий

		inline auto operator ==(const Holes & other) const {
			return count == other.count and diam == other.diam;
		}
	};

	struct Numbering {
		int start = 1;		// Начальный номер
		int end = 100;		// Конечный номер
		int digits = 1;		// Формат номера (кол-во цифр)

		inline auto operator ==(const Numbering & other) const {
			return
				start	== other.start	and
				end		== other.end	and
				digits	== other.digits;
		}
	};

	struct DieCutting {
		int count = 0;		// Кол-во штанцформ на печ. листе
		QString sName;		// Название штанцформы

		inline auto operator ==(const DieCutting & other) const {
			return count == other.count and sName == other.sName;
		}
	};

	Opt<Lamination>		lamination;			// Ламинация
	Opt<Foil>			foil;				// Фольгирование
	Opt<int>			creasings;			// Биговки
	Opt<Folding>		foldings;			// Фальцовки
	Opt<int>			perforations;		// Кол-во перфораций
	Opt<int>			gluePoints;			// Кол-во клеевых точек
	Opt<DblSidedTape>	dblSidedTape;		// Двусторонний скотч
	Opt<int>			scrathStrips;		// Скретч-полосы
	Opt<RndCorners>		rndCorners;			// Скругления углов
	Opt<Eyelets>		eyelets;			// Люверсы
	Opt<Holes>			holes;				// Oтверстия
	Opt<Numbering>		nums;				// Нумерация
	Opt<DieCutting>		dieCutting;			// Высечка

	bool operator ==(const Postpress & other) const {
		return
			lamination		== other.lamination		and
			foil			== other.foil			and
			creasings		== other.creasings		and
			foldings		== other.foldings		and
			perforations	== other.perforations	and
			gluePoints		== other.gluePoints		and
			dblSidedTape	== other.dblSidedTape	and
			scrathStrips	== other.scrathStrips	and
			rndCorners		== other.rndCorners		and
			eyelets			== other.eyelets		and
			holes			== other.holes			and
			nums			== other.nums			and
			dieCutting		== other.dieCutting;
	}
//#ifdef _DEBUG
//	bool IsValid() const {
//		return
//			lamination.IsValid()		and
//			foil.IsValid()				and
//			nCreasings			>= 0	and
//			nFoldings			>= 0	and
//			nPerforations		>= 0	and
//			nGluePoints			>= 0	and
//			nDblSidedTapeQnt	>= 0	and
//			nDblSidedTapeLength	>= 0	and
//			nDblSidedTapeWidth	>= 0	and
//			nScrathStrips		>= 0	and
//			nRoundedCorners		>= 0	and
//			nRndCornersRadius	>= 0	and
//			nEyelets			>= 0	and
//			nEyeletsDiam		>= 0	and
//			nHoles				>= 0	and
//			nHolesDiam			>= 0	and
//			nStartNumber		>= 0	and
//			nEndNumber			>= 0	and
//			nDigitsInNum		>= 0;
//	}
//#endif // _DEBUG
};


class Imposition;

// Печатный лист
struct Signature {
	SizeT	prepSheets;		// Кол-во приладочных листов
	SizeT	printSheets;	// Кол-во печатных листов
	SizeT	sourceSheets;	// Кол-во исходных листов
	SizeT	platesB3;		// Формы A3
	SizeT	platesB2;		// Формы A2
	bool	isPartial;		// Частичная сигнатура
	bool	isOwnBack;		// Свой оборот при двустор. печати
	qreal	hoursFC;		// Часы печати полноцвет (3-4 краски)
	qreal	hoursBW;		// Часы печати ЧБ (1-2 краски)

	inline auto ActualPrintSheets() const {
		return prepSheets + printSheets;
	}

	auto operator ==(const Signature & other) const {
		return
			prepSheets		== other.prepSheets		and
			printSheets		== other.printSheets	and
			sourceSheets	== other.sourceSheets	and
			platesB3		== other.platesB3		and
			platesB2		== other.platesB2		and
			isPartial		== other.isPartial		and
			isOwnBack		== other.isOwnBack		and
			hoursFC			== other.hoursFC		and
			hoursBW			== other.hoursBW;
	}

#ifdef _DEBUG
	bool IsValid() const {
		return
			prepSheets		>= 0	and
			printSheets		>= 0	and
			sourceSheets	>= 0	and
			platesB3		>= 0	and
			platesB2		>= 0	and
			hoursFC			>= 0	and
			hoursBW			>= 0;
	}
#endif // _DEBUG
};


// Список печатных листов
class Signatures: protected QList<Signature> {
	Imposition * imp;

	void Zeroes() {
		hoursFC			= hoursBW		=
		fullCount		= partialCount	=
		prepSheets		= printSheets	= sourceSheets =
		platesB3		= platesB2		=
		platesSetsB3	= platesSetsB2	= 0;
	}

public:
	SizeT	fullCount;		// Кол-во полных сигнатур
	SizeT	partialCount;	// Кол-во частичных сигнатур
	SizeT	prepSheets;		// Всего приладочных листов
	SizeT	printSheets;	// Всего печатных листов
	SizeT	sourceSheets;	// Всего исходных листов
	SizeT	platesB3;		// Всего форм A3
	SizeT	platesB2;		// Всего форм A2
	SizeT	platesSetsB3;	// Комплекты форм A3
	SizeT	platesSetsB2;	// Комплекты форм A2
	qreal	hoursFC;		// Часы печати полноцвет (3-4 краски)
	qreal	hoursBW;		// Часы печати ЧБ (1-2 краски)

	Signatures(Imposition * parent) : imp(parent) {
		Zeroes();
	}

	void Add(size_t copies, bool isPartial, bool isOwnBack);

	inline Signature & first() {
		return QList::first();
	}

	//inline const Signature & first() const {
	//	return QList::first();
	//}

	inline auto begin() const {
		return QList::begin();
	}

	inline auto end() const {
		return QList::end();
	}

	inline auto & operator [](qsizetype index) const {
		return at(index);
	}

	inline auto count() const {
		return QList::count();
	}

	inline void clear() {
		QList::clear();
		Zeroes();
	}

	inline auto ActualPrintSheets() const {
		return prepSheets + printSheets;
	}

	bool operator ==(const Signatures & other) const {
		return
			fullCount		==	other.fullCount		and
			partialCount	==	other.partialCount	and
			prepSheets		==	other.prepSheets	and
			printSheets		==	other.printSheets	and
			sourceSheets	==	other.sourceSheets	and
			platesB3		==	other.platesB3		and
			platesB2		==	other.platesB2		and
			platesSetsB3	==	other.platesSetsB3	and
			platesSetsB2	==	other.platesSetsB2	and
			hoursFC			==	other.hoursFC		and
			hoursBW			==	other.hoursBW		and
			*this			==	other;
	}
};


// Красочность
struct Inks {
	int face = 1;
	int back = 0;

	inline auto operator ==(const Inks & other) const {
		return face == other.face and back == other.back;
	}
};


class Product;
class Impositions;

// Спуск
class Imposition {
	Product *		owner;			// Изделие-владелец спуска
	Opt<SizeT>		placesInWidth;	// Мест на листе по ширине
	Opt<SizeT>		placesInHeight;	// Мест на листе по высоте
	Opt<SizeT>		totalPlaces;	// Всего мест на листе
	QSizeF			bleedSize;		// Размер изделия с учетом выхода за обрез
	QSizeF			printField;		// Размер печатного поля
	Opt<SizeT>		cost;			// Стоимость (в целых рублях)
	QList<QString>	log;			// Лог расчета стоимости
	Opt<bool>		ownBack;		// Свой оборот
//	EOwnBack		eOwnBack		// Свой оборот
//		{ EOwnBack::Allowed };

	friend class ImpWizard;

public:
	EImpType	eType			// Тип спуска
		{ EImpType::Simple };
	Paper		paper;			// Бумага
	Inks		inks;			// Красочность
	EPress		ePress			// Печатная машина
		{ EPress::Riso };
	Opt<SizeT>	prepSheets;		// Листов на приладку
	Signatures	signatures;		// Сигнатуры спуска
	bool		bAutoImposition	// Авторазмещение
		{ true };
	bool		bAutoIndent		// Автоотступ от края листа
		{ true };
	Opt<qreal>	horizIndent;	// Отступ от края листа по горизонтали
	Opt<qreal>	vertIndent;		// Отступ от края листа по вертикали
	bool		bAutoGap		// Автоотступ от края листа
		{ true };
	Opt<qreal>	gap;			// Интевал между макетами
	Opt<qreal>	bleed;			// Выход за обрез
	QSizeF		maxIndents;		// Максимальные отступы от края листа
	Postpress	postpress;		// Постпечатная обработка

	bool IsSpreads() const;
	void SetDefaultPrepSheets();
	void LayoutPages();
	void CalcCost();
	void CalcSheets();
	QString GetName(bool bTrueSimpleName = false) const;
	bool IsOwnBackAllowed() const;

	Imposition() : signatures(this) {
	}

	Imposition(EImpType eType) : eType(eType), signatures(this) {
	}

	inline Product * GetOwner() const {
		return owner;
	}

	inline void SetOwner(Product * product) {
		Q_ASSERT(product);
		owner = product;
	}

	PROP_RW(Product *, product, Owner)

	inline auto IsOwnBack() const {
		return ownBack.value_or(false); //eOwnBack == EOwnBack::Forced;
	}

	inline auto IsOwnBackForbidden() const {
		return !ownBack.has_value(); //eOwnBack == EOwnBack::Forbidden;
	}

	inline auto GetWPlaces() const {
		Q_ASSERT(*placesInWidth < INT_MAX);
		return *placesInWidth;
	}


	inline auto GetHPlaces() const {
		Q_ASSERT(*placesInHeight < INT_MAX);
		return SizeT(*placesInHeight);
	}

	inline auto GetTotalPlaces() const {
		Q_ASSERT(*totalPlaces < INT_MAX);
		return SizeT(*totalPlaces);
	}

	inline auto & GetBleedSize() const {
		return bleedSize;
	}

	inline auto & GetPrintField() const {
		return printField;
	}

	inline auto GetCost() const {
		return *cost;
	}

	inline auto & GetLog() const {
		return log;
	}

	inline auto IsNoPrint() const {
		return ePress == EPress::WrongPress;
	}

	inline auto IsDuplex() const {
		return bool(inks.back);
	}

	inline auto IsSimple() const {
		return eType == EImpType::Simple;
	}

	inline auto IsOffsetPress() const {
		return	ePress == EPress::Ryobi			or
				ePress == EPress::Roland		or
				ePress == EPress::Heidelberg;
	}

	inline void SetTotalPlaces(size_t newPlaces) {
		Q_ASSERT(newPlaces > 0 and newPlaces < 5000);
		totalPlaces = newPlaces;
		placesInWidth.reset();
		placesInHeight.reset();
	}

	auto operator ==(const Imposition & other) const {
		return
			eType			== other.eType				and
			paper			== other.paper				and
			inks			== other.inks				and
			ePress			== other.ePress				and
			prepSheets		== other.prepSheets			and
			signatures		== other.signatures			and
			bAutoImposition	== other.bAutoImposition	and
			bAutoIndent		== other.bAutoIndent		and
			horizIndent		== other.horizIndent		and
			vertIndent		== other.vertIndent			and
			bAutoGap		== other.bAutoGap			and
			gap				== other.gap				and
			bleed			== other.bleed				and
			maxIndents		== other.maxIndents			and
			postpress		== other.postpress;
	}

	friend QDataStream & operator <<(QDataStream &, const Imposition &);
	friend QDataStream & operator >>(QDataStream &, Imposition &);
};


class Impositions : protected QMap<EImpType, Imposition *> {
public:
	void Add(Imposition * imp);

	inline auto isEmpty() const {
		return QMap::isEmpty();
	}

	inline auto begin() const {
		return QMap::begin();
	}

	inline auto begin() {
		return QMap::begin();
	}

	inline auto end() const {
		return QMap::end();
	}

	inline auto end() {
		return QMap::end();
	}

	inline auto count() const {
		return QMap::count();
	}

	inline auto contains(EImpType type) const {
		return QMap::contains(type);
	}

	inline void clear() {
		QMap::clear();
	}

	inline bool remove(EImpType type) {
//		Q_ASSERT(contains(type));
		return QMap::remove(type);
	}

	inline Imposition & GetRefImp(EImpType type) const {
		Q_ASSERT(contains(type));
		return *value(type);
	}

	inline Imposition * GetImp(EImpType type) {
		return value(type, nullptr);
	}

	void SetOwner(Product * owner) {
		for (auto imp : qAsConst(*this))
			imp->product = owner;
	}

	auto operator ==(const Impositions & other) const {
		if (!(isEmpty() and other.isEmpty())) {
			if (count() != other.count())
				return false;
			for (auto it = cbegin(), oit = other.cbegin();
				 it != cend() and oit != cend();
				 ++it, ++oit)
			{
				if (*it != *oit)
					return false;
			}
		}
		return true;
	}
};


struct ProductKind {
	size_t copies =
#ifdef _DEBUG
	500;
#else
	1;
#endif // _DEBUG
	size_t packs = 1;
	Opt<QString> name;

	auto operator ==(const ProductKind & other) const {
		return
			copies		== other.copies		and
			packs		== other.packs		and
			name		== other.name;
	}
};


struct ProductKinds : public QList<ProductKind> {
	size_t GetCopies() const;
	size_t GetPacks() const;
	size_t GetEvery() const;

	inline bool IsOnlyOneKind() const {
		return count() == 1;
	}

	inline auto operator ==(const ProductKinds & other) const {
		return QList::operator ==(other);
	}
};


// Изделие
class Product {
	// Суммарные значения для всех спусков изделия
	struct Totals {
		SizeT	platesB3;		// Всего форм A3
		SizeT	platesB2;		// Всего форм A2
		SizeT	platesSetsB3;	// Комплекты форм A3
		SizeT	platesSetsB2;	// Комплекты форм A2
		SizeT	sheetsOffset;	// Печатные листы на офсетные машины
		SizeT	sheetsRiso;		// Печатные листы на ризограф
		SizeT	sheetsKonica;	// Печатные листы на Конику
		qreal	hoursFC;		// Часы печати полноцвет (3-4 краски)
		qreal	hoursBW;		// Часы печати ЧБ (1-2 краски)
		SizeT	platesPriceB3;	// Цена форм B3
		SizeT	platesPriceB2;	// Цена форм B2
		SizeT	printPriceFC;	// Цена печати полноцвет (3-4 краски)
		SizeT	printPriceBW;	// Цена печати ЧБ (1-2 краски)

		auto operator ==(const Totals & other) const {
			return
				platesB3		== other.platesB3		and
				platesB2		== other.platesB2		and
				platesSetsB3	== other.platesSetsB3	and
				platesSetsB2	== other.platesSetsB2	and
				sheetsOffset	== other.sheetsOffset	and
				sheetsRiso		== other.sheetsRiso		and
				sheetsKonica	== other.sheetsKonica	and
				hoursFC			== other.hoursFC		and
				hoursBW			== other.hoursBW		and
				platesPriceB3	== other.platesPriceB3	and
				platesPriceB2	== other.platesPriceB2	and
				printPriceFC	== other.printPriceFC	and
				printPriceBW	== other.printPriceBW;
		}
	} totals;

	size_t			cost = WRONG;
	QList<QString>	log;

public:
	class Size {
		EProductSize	eps;
		SizeT			w;		// ширина в мм
		SizeT			h;		// высота в мм

	public:
		Size() {
			SetSize(DEFAULT_PRODUCT_SIZE);
		}

		void SetSize(EProductSize eProductSize,
					 SizeT width = 0,
					 SizeT height = 0);

		inline void SetESize(EProductSize eProductSize) {
			SetSize(eProductSize);
		}

		inline EProductSize GetESize() const {
			return eps;
		}

		inline QSize GetSize() const {
			return { int(w), int(h) };
		}

		inline SizeT GetWidth() const {
			return w;
		}

		inline void SetWidth(SizeT newWidth) {
			ASSERT_RANGE(newWidth, PRODUCT_SIZE);
			Q_ASSERT(eps == EProductSize::CustomSize);
			w = newWidth;
		}

		inline SizeT GetHeight() const {
			return h;
		}

		inline void SetHeight(size_t newHeight) {
			ASSERT_RANGE(newHeight, PRODUCT_SIZE);
			Q_ASSERT(eps == EProductSize::CustomSize);
			h = newHeight;
		}

		PROP_RW(EProductSize,	eSize,		ESize)
		PROP_RO(QSize,			size,		Size)
		PROP_RW(SizeT,			width,		Width)
		PROP_RW(SizeT,			height,		Height)

		inline auto operator ==(const Size & other) const {
			return eps == other.eps and w == other.w and h  == other.h;
		}

		friend QDataStream & operator <<(QDataStream &, const Size &);
		friend QDataStream & operator >>(QDataStream &, Size &);
	};

	SizeT			number;			// Номер изделия
	EProductKind	eKind			// Вид изделия (простое или многостраничное)
		{ EProductKind::Simple };
	QString			sName;			// Название изделия
	Size			size;			// Размер изделия
	ProductKinds	kinds;			// Список видов изделия
	EBinding		eBinding		// Переплет
		{ EBinding::NoBinding };
	qreal			bindDiam		// Диаметр болтов или колец
		{ 0.0 };
	qreal			bindPrice		// Цена единицы переплета
		{ 0.0 };
	SizeT			nBindCount		// Кол-во болтов или колец
		{ 1	};
	bool			bUserBindPrice	// Цена переплета установлена пользователем
		{ false };
	bool			bLongSideBind	// Переплет по длинной стороне
		{ true };
	SizeT			nPagesCover		// Кол-во полос обложки
		{ 0 };
	SizeT			nPagesBlock		// Кол-во полос блока
		{ 0 };
	SizeT			nPagesBottom	// Кол-во полос подложки
		{ 0 };
	SizeT			nPagesInsert	// Кол-во полос вставки
		{ 0 };
	bool			bIdenticalPages	// Полосы блока одинаковые
		{ false };
	Impositions		impositions;	// Список спусков
	QString			notes;			// Примечания к изделию

	void CalcCost();
//	void UpdateTotals();
	const QSize GetSpreadSize(EImpType impType = EImpType::Simple) const;

	inline void SetImpositionsOwner() {
		impositions.SetOwner(this);
	}

	inline const auto & GetTotals() const {
		return totals;
	}

	inline auto IsSimple() const {
		return eKind == EProductKind::Simple;
	}

	inline auto IsPortrait() const {
		return size.GetWidth() < size.GetHeight();
	}

	inline auto GetCost() const {
		return cost;
	}

	inline const auto & GetLog() const {
		return log;
	}

	inline auto IsSizeLargerB3() const {
		return Fit({ 520, 360 }, GetSpreadSize()) == EFit::NotFit;
	}

	inline auto IsSizeLargerDigital() const {
		return Fit({ 480, 320 }, GetSpreadSize()) == EFit::NotFit;
	}

	inline auto IsSizeLargerA3() const {
		return Fit({ 420, 297 }, GetSpreadSize()) == EFit::NotFit;
	}

	auto operator ==(const Product & other) const {
		return
			totals			== other.totals				and
			cost			== other.cost				and
			eKind			== other.eKind				and
			sName			== other.sName				and
			size			== other.size				and
			kinds			== other.kinds				and
			eBinding		== other.eBinding			and
			bindDiam		== other.bindDiam			and
			bindPrice		== other.bindPrice			and
			nBindCount		== other.nBindCount			and
			bUserBindPrice	== other.bUserBindPrice		and
			bLongSideBind	== other.bLongSideBind		and
			nPagesCover		== other.nPagesCover		and
			nPagesBlock		== other.nPagesBlock		and
			nPagesBottom	== other.nPagesBottom		and
			nPagesInsert	== other.nPagesInsert		and
			bIdenticalPages	== other.bIdenticalPages	and
			impositions		== other.impositions		and
			notes			== other.notes;
	}


	friend QDataStream & operator <<(QDataStream &, const Product &);
	friend QDataStream & operator >>(QDataStream &, Product &);
};


class Products : protected QList<Product *> {
public:
	inline void clear() {
		QList::clear();
	}

	inline auto isEmpty() const {
		return QList::isEmpty();
	}

	inline auto count() const {
		return QList::count();
	}

	inline auto begin() const noexcept {
		return QList::begin();
	}

	inline auto end() const noexcept {
		return QList::end();
	}

	auto IsIndexValid(qsizetype index) {
		return index > 0 and index <= count();
	}

	inline void Insert(Product * product, QList::size_type index = -1) {
		Q_ASSERT(product and !product->impositions.isEmpty());
		Q_ASSERT(index == -1 or (index != -1 and IsIndexValid(index)));
		Q_ASSERT(!contains(product));
		product->SetImpositionsOwner();
		insert(index == -1 ? count() : index, product);
		QList::size_type i = 0;
		while (i < count()) {
			auto p = (*this)[i];
			p->number = ++i;
		}
	}

	inline void Remove(Product * product) {
		const auto found = indexOf(product);
		Q_ASSERT(found != -1);
		remove(found);
	}

	inline auto GetProduct(QList::size_type index) {
		Q_ASSERT(IsIndexValid(index));
		return (*this)[index];
	}
};


// Цены основных операций
struct OperationPrice {
	struct Price {
		size_t	limit;	// Порог значения цены (напр., печ. листы)
		qreal	price;	// Цена при данном пороге
	};

	struct Lam {
		bool			isGloss;	// Глянцевая или матовая
		size_t			thickness;	// Толщина в мкм
		EProductSize	eSize;		// Формат (например, А4)
	};

	size_t			setupCost;	// Стоимость приладки
	QList<Price>	prices;		// Список цен по порогам
	Opt<Lam>		lam;		// Ламинация (необязательно)

	void clear() {
		setupCost = 0;
		prices.clear();
		if (lam)
			lam.reset();
	}

	qreal GetPrice(size_t amount) const;
};


class OperationPrices : protected QMultiMap<EOperation, OperationPrice> {
public:
	void clear() {
		for (auto & op : *this)
			op.clear();
		QMultiMap::clear();
	}

	auto isEmpty() {
		return QMultiMap::isEmpty();
	}

	auto contains(EOperation eOp) const {
		return QMultiMap::contains(eOp);
	}

	void Add(EOperation eOp, const OperationPrice & op) {
		QMultiMap::insert(eOp, op);
	}

	auto GetSetupCost(EOperation eOp) const {
		Q_ASSERT(contains(eOp));
		return value(eOp).setupCost;
	}

	auto GetPrice(EOperation eOp, size_t amount) const {
		Q_ASSERT(contains(eOp));
		return value(eOp).GetPrice(amount);
	}

	auto GetPrice(EOperation eOp,
				  size_t amount,
				  bool isGloss,
				  size_t thickness,
				  Opt<EProductSize> eSize = { }) const;
};


_CONST(auto,	PRODUCT_SIZES_COUNT,	enum_count<EProductSize>()	)
_CONST(auto,	BINDINGS_COUNT,			enum_count<EBinding>()		)
_CONST(auto,	PAPER_TYPES_COUNT,		enum_count<EPaperType>()	)
_CONST(auto,	PAPER_FORMATS_COUNT,	enum_count<EPaperFormat>()	)
_CONST(auto,	SHEET_PORTIONS_COUNT,	enum_count<EPortion>()		)
_CONST(auto,	PRESSES_COUNT,			enum_count<EPress>()		)
_CONST(auto,	IMP_TYPES_COUNT,		enum_count<EImpType>()		)
_CONST(auto,	OPERATIONS_COUNT,		enum_count<EOperation>()	)


using MapProductKind		= QHash<QString,	EProductKind>;
using MapBindingNames		= QMap<EBinding,	QString>;
using MapBindings			= QMap<EBinding,	size_t>;
using MapPortionNames		= QMap<EPortion,	QString>;
using MapPressNames			= QMap<EPress,		QString>;
using MapLamNames			= QMap<ELamType,	QString>;
using MapImpNames			= QMap<EImpType,	QString>;
using MapOperationNames		= QHash<QString,	EOperation>;
using MapLamThickness		= QMap<ELamType,	int>;

extern QString &				sDefaultProductsName;
extern QList<QString> &			simpleProductsNames;
extern MapProductKind &			mapProductKind;
extern MapImpNames &			stdImpNames;
extern MapBindingNames &		stdBindingNames;
extern QList<decltype(Paper::weight)> &
								stdPaperWeights;
extern MapPortionNames &		stdPortionNames;
extern MapPressNames &			pressNames;
// Стоимость стандартных бумаг за лист
extern PaperPricesByType &		paperPrices;
extern MapLamNames &			stdLamNames;
// Стандартные толщины ламинации
extern QList<SizeT> &			stdLamThickness;
// Толщины ламинации по умолчанию
extern MapLamThickness &		defaultLamThickness;
// Толщины пакетной ламинации
extern QList<SizeT> &			lamPackageThickness;
// Толщины цифровой ламинации
extern QList<SizeT> &			lamDigitalThickness;
extern MapOperationNames &		mapOperationNames;
extern OperationPrices &		operationPrices;
extern Products &				products;
extern QList<QString> &			orderPrefixes;

extern bool IsLamOperation(EOperation);
extern EOperation LamOperation(ELamType);
