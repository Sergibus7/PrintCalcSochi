#pragma once

#include <QDateTime>
#include <QApplication>
#include <cfenv>
#include <limits>

//#define MAGIC_ENUM_RANGE_MIN -1
//#define MAGIC_ENUM_RANGE_MAX 128
#include <magic_enum.hpp>


//#define STATIC_EXT(TYPE, NAME) extern TYPE & NAME

#define GLOBAL(TYPE, NAME) \
	Q_GLOBAL_STATIC(TYPE, NAME##Ptr) \
	TYPE & NAME = *NAME##Ptr;

#define GLOBAL_ARGS(TYPE, NAME, ARGS) \
	Q_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME##Ptr, ARGS) \
	TYPE & NAME = *NAME##Ptr;

#define PROP_RO(Type, PropName, FuncName) \
	__declspec(property(get = Get##FuncName)) Type PropName;

#define PROP_RW(Type, PropName, FuncName) \
	__declspec(property(get = Get##FuncName, put = Set##FuncName)) \
	Type PropName;

#define _CONST(TYPE, NAME, VALUE) \
[[maybe_unused]] constexpr TYPE NAME = VALUE;


using SizeT = QList<int>::size_type /*qsizetype*/;

_CONST(SizeT, WRONG, LLONG_MIN)

template<typename T>
using Opt = std::optional<T>;

//template<typename T>
//QDataStream & operator <<(QDataStream &, Opt<const T> &);

//template<typename T>
//QDataStream & operator >>(QDataStream &, Opt<T> &);

template <class T>
inline bool CheckRange(const T & value, const T & min, const T & max) {
	return (value >= min and value <= max);
};


using namespace magic_enum;

//template<class Enum>
//constexpr Enum EnumValue(size_t index) {
////	Q_ASSERT(enum_contains<Enum>(index));
//	return enum_value<Enum>(index);
//}


template<class Enum>
constexpr Enum EnumCast(underlying_type_t<Enum> value) {
	Q_ASSERT(enum_cast<Enum>(value));
	return *enum_cast<Enum>(value);
}


template<class Enum>
constexpr Enum EnumCastOr(underlying_type_t<Enum> value, Enum orValue) {
	return enum_cast<Enum>(value).value_or(orValue);
}


template<class Enum>
constexpr underlying_type_t<Enum> EnumIndex(Enum value) {
	Q_ASSERT(enum_index<Enum>(value));
	return *enum_index<Enum>(value);
}


constexpr SizeT Ceil(std::floating_point auto value) {
	Q_ASSERT(value >= 0.0);
	return llrint(value);
}

bool constexpr CheckRange(SizeT value, SizeT MIN_CONST, SizeT MAX_CONST) {
	return value >= MIN_CONST and value <= MAX_CONST;
}


#define CHECK_RANGE(value, ConstName) \
	CheckRange(value, MIN_##ConstName, MAX_##ConstName)

#define ASSERT_RANGE(value, ConstName) \
	Q_ASSERT(CHECK_RANGE(value, ConstName))


class RoundCost {
	enum ERoundCost : size_t {
		NoRound		,
		Tens		,
		Hundreds	,
		Thousands	,
	} eRound = NoRound;

	const QMap<ERoundCost, size_t> mapRounds = {
		{ NoRound	,	0	, },
		{ Tens		,	10	, },
		{ Hundreds	,	100	, },
		{ Thousands	,	1000, },
	};

public:
	inline void SetFromIndex(size_t index) {
		Q_ASSERT(index < enum_count<ERoundCost>());
		eRound = enum_value<ERoundCost>(index);
	}

	inline void SetFromInt(size_t value) {
		eRound = EnumCastOr<ERoundCost>(value, NoRound);
	}

	operator QString() {
		return QString::number(mapRounds[eRound]);
	}

	//inline auto ToStr() const {
	//	return QString::number(mapRounds[eRound]);
	//}

	operator int() {
		return int(mapRounds[eRound]);
	}

	//inline auto ToInt() const {
	//	return mapRounds[eRound];
	//}

	size_t R(SizeT value) const;
};


// Опции запуска
enum class EStartOption: size_t {
	New,
	Open,
	OpenFromShell,
	CalcCostOnly,
	NormalOperation,
};


class MyApplication : public QApplication {
	bool notify(QObject * receiver, QEvent * event) override;

public:
	MyApplication(int & argc, char ** argv) : QApplication(argc, argv) { }
	void Help();
};


_CONST(auto, SIZE_HWND, sizeof(HWND))

class QSettings;
struct State;

extern bool				bPathToCSVSaved;
extern bool				ok;
extern HWND *			pBuf;
extern QSettings *		settings;
extern RoundCost &		roundCost;
extern QLocale &		loc;
extern const QChar		chTab;
extern QString &		sPathToCSV;
extern QString &		sNewline;
extern QString &		sPathToCSVKeyName;
extern QString &		sCSVFileNamePaper;
extern QString &		sCSVFileNameOperations;
extern QString &		sCSVFileNameStates;
extern QString &		sRoundCostKeyName;
extern QString &		sCoatedGlossy;
extern QString &		sFace;
extern QString &		sBack;
extern QString &		sSpaces;
extern QString &		sFileExtention;
extern QString &		sProgramName;
extern QString &		sDateFormat;
extern QString &		sTimeFormat;
extern QString &		sErrorNotFit;
extern QList<State *> & statesTable;

#define S(str) QStringLiteral(str)

// Текстовые функции
extern QString t(std::integral auto n) {
	return (n < 10000) ? QString::number(n) : loc.toString(n);
}
extern QString t(qreal f,
				 decltype(enum_integer(QLocale::FloatingPointShortest))
				 prec = QLocale::FloatingPointShortest);
extern inline QString t(const QDate & date);
extern inline QString t(const QTime & time);
extern void _CriticalError(const QString & sErrorInfo);

#define CRITICAL(str) _CriticalError(S(str))
#define _CRITICAL(str) _CriticalError(str)

enum class EFit : size_t {
	Fit,
	Equal,
	NotFit,
};

class QButtonGroup;
class QComboBox;

extern EFit Fit(const QSize & threshold, const QSize & size);
extern void EnableButtonGroup(const QButtonGroup * bg, bool enable = true);
extern void DisableButtonGroup(const QButtonGroup * bg);
extern size_t GetDataFromCombobox(const QComboBox * cb);
extern void SetIndexByDataFromCombobox(const QComboBox * cb, size_t data);
extern bool IsCheckedBtn(const QButtonGroup * bg, int id);
extern void SetBtnChecked(const QButtonGroup * bg, int id, bool checked = true);
extern void SetBtnEnabled(const QButtonGroup * bg, int id, bool enable = true);
