#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "user32.lib")

#include <QButtonGroup>
#include <QApplication>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QLocale>
#include <QComboBox>
#include <QSettings>
#include <shlobj_core.h>
#include <WinReg.hpp>
#include "Globals.h"
#include "MsgBox.h"
#include "MainWindow.h"
#include "ImpWizard.h"

bool		ok;
QSettings *	settings		{ nullptr	};
bool		bPathToCSVSaved	{ false		};
HWND *		pBuf			{ nullptr	};
const QChar	chTab			{ '\t'		};

GLOBAL(QString,					sPathToCSV				)
GLOBAL(QString,					sNewline				)
GLOBAL(QString,					sPathToCSVKeyName		)
GLOBAL(QString,					sCSVFileNamePaper		)
GLOBAL(QString,					sCSVFileNameOperations	)
GLOBAL(QString,					sCSVFileNameStates		)
GLOBAL(QString,					sRoundCostKeyName		)
GLOBAL(QString,					sCoatedGlossy			)
GLOBAL(QString,					sFace					)
GLOBAL(QString,					sBack					)
GLOBAL(QString,					sSpaces					)
GLOBAL(QString,					sFontName				)
GLOBAL(QString,					sFileExtention			)
GLOBAL(QString,					sProgramName			)
GLOBAL(QString,					sDefaultProductsName	)
GLOBAL(QString,					sDateFormat				)
GLOBAL(QString,					sTimeFormat				)
GLOBAL(QString,					sErrorNotFit			)
GLOBAL(QLocale,					loc						)
GLOBAL(QList<QString>,			simpleProductsNames		)
GLOBAL(MapProductKind,			mapProductKind			)
GLOBAL(QSet<EProductSize>,		stdConvertSizes			)
GLOBAL(StdProductSizes,			stdProductSizes			)
GLOBAL(MapImpNames,				stdImpNames				)
GLOBAL(MapBindingNames,			stdBindingNames			)
GLOBAL(StdPaperTypes,			stdPaperTypes			)
GLOBAL(StdPaperFormats,			stdPaperFormats			)
GLOBAL(QList<SizeT>,			stdPaperWeights			)
GLOBAL(MapPortionNames,			stdPortionNames			)
GLOBAL(MapPressNames,			pressNames				)
GLOBAL(PaperPricesByType,		paperPrices				)
GLOBAL(MapOperationNames,		mapOperationNames		)
GLOBAL(OperationPrices,			operationPrices			)
GLOBAL(MapLamNames,				stdLamNames				)
GLOBAL(QList<SizeT>,			stdLamThickness			)
GLOBAL(MapLamThickness,			defaultLamThickness		)
GLOBAL(QList<SizeT>,			lamPackageThickness		)
GLOBAL(QList<SizeT>,			lamDigitalThickness		)
GLOBAL(Products,				products				)
GLOBAL(RoundCost,				roundCost				)

// из ImpWizard
GLOBAL(MapIndexPaperFormat,		mapIndexPaperFormat		)
GLOBAL(MapIndexSheetPortion,	mapIndexSheetPortion	)
GLOBAL(MapIndexPaperWeight,		mapIndexPaperWeight		)
GLOBAL(MapIndexPress,			mapIndexPress			)
GLOBAL(QList<State *>,			statesTable				)



QString t(qreal f,
		  decltype(enum_integer(QLocale::FloatingPointShortest)) prec)
{
	auto s = loc.toString(f, 'f', prec);
	if (f > 1000.0 and f < 10000.0) {
        const auto length = s.length();
        s.remove(QChar(160));
		Q_ASSERT(length > s.length());
	}
	if (prec == QLocale::FloatingPointShortest) {
		const auto decimalPos = s.lastIndexOf(loc.decimalPoint());
		if (decimalPos == -1)
			// целое число
			return s;
		if (!decimalPos) {
			if (qFuzzyIsNull(/* выделяем дробную часть */ f - size_t(f)))
				s += S(",0");
			return s;
		}
		_CONST(qsizetype, MAX_FRACTION_LENGTH, 3)
		if (s.length() - decimalPos - 1 > MAX_FRACTION_LENGTH) {
			auto nonZeroPos = decimalPos + MAX_FRACTION_LENGTH;
			do {
				if (s.at(nonZeroPos) != u'0')
					break;
			} while (--nonZeroPos > decimalPos);
			s.truncate(nonZeroPos + 1);
		}
	}
	return s;
}


inline QString t(const QDate & date) {
	return loc.toString(date);
}


inline QString t(const QTime & time) {
	return loc.toString(time);
}


void _CriticalError(const QString & sErrorInfo) {
	Q_ASSERT(false);
	MsgBox mb(qApp->activeWindow());
	mb.Error(sErrorInfo + u'!');
	exit(EXIT_FAILURE);
}


size_t RoundCost::R(SizeT value) const {
	Q_ASSERT(value >= 0);
	const auto n = mapRounds[eRound];
	if (!n)
		return size_t(value);
	Q_ASSERT(n == 10 or n == 100 or n == 1000);
	return n * Ceil(value / qreal(n));
}


void EnableButtonGroup(const QButtonGroup * bg, bool enable) {
	Q_ASSERT(bg);
	for (const auto button : bg->buttons())
		if (button->isEnabled() != enable)
			button->setEnabled(enable);
};


inline void DisableButtonGroup(const QButtonGroup * bg) {
	EnableButtonGroup(bg, false);
}


size_t GetDataFromCombobox(const QComboBox * cb) {
	const size_t result = cb->currentData().toULongLong(&ok);
	return ok ? result : WRONG;
}


void SetIndexByDataFromCombobox(const QComboBox * cb, size_t data) {
	Q_ASSERT(cb);
	const auto found = cb->findData(data);
	Q_ASSERT(found != -1);
	const_cast <QComboBox *> (cb)->setCurrentIndex(found);
}


EFit Fit(const QSize & threshold, const QSize & size) {
	const auto & w = size.width();
	const auto & h = size.height();
	const auto & t = threshold;
	const auto & r = threshold.transposed();

	if ((w == t.width()	 or w == r.width()) and
		(h == t.height() or h == r.height()))
		return EFit::Equal;

	if ((w < t.width() and h < t.height()) or
		(w < r.width() and h < r.height()))
		return EFit::Fit;

	return EFit::NotFit;
}


inline bool IsCheckedBtn(const QButtonGroup * bg, int id) {
	const auto btn = bg->button(id);
	Q_ASSERT(btn);
	return btn->isChecked();
}


void SetBtnChecked(const QButtonGroup * bg, int id, bool checked) {
	const auto btn = bg->button(id);
	Q_ASSERT(btn);
	if (btn->isChecked() != checked)
		btn->setChecked(checked);
}


inline void SetBtnEnabled(const QButtonGroup * bg, int id, bool enable) {
	bg->button(id)->setEnabled(enable);
}


int main(int argc, char * argv[]) {
	fesetround(FE_UPWARD);
	constexpr auto BUFFER_SIZE = 32767;
	DWORD bufLen = BUFFER_SIZE;
	LPTSTR buf = new TCHAR[BUFFER_SIZE];
	if (!GetUserName(buf, &bufLen))
		exit(1);
	Q_ASSERT(bufLen);

	const auto sKey { S("Local\\%1").arg(QString::fromWCharArray(buf)) };
	LPTSTR lpszName = new TCHAR[sKey.length()];
	sKey.toWCharArray(lpszName);

	auto ErrorMsg = [] (const TCHAR * msg) -> void {
		MessageBox(0, msg, 0, MB_ICONERROR);
	};

	auto hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,	// использование файла подкачки
		NULL,
		PAGE_READWRITE,
		0,
		SIZE_HWND,
		lpszName);

	if (!hMapFile) {
		ErrorMsg(L"Невозможно создать FileMapping!");
		exit(1);
	}

	const auto isSecondary = GetLastError() == ERROR_ALREADY_EXISTS;

	if (isSecondary) {
		hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, lpszName);
		if (!hMapFile) {
			ErrorMsg(L"Невозможно открыть FileMapping!");
			exit(1);
		}
	}

	pBuf = (HWND *) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS,
								  0, 0, SIZE_HWND);
	if (!pBuf) {
		ErrorMsg(L"Could not map view of file!");
		exit(1);
	}

	const auto isOpenFromShell = argc > 1;

	if (isSecondary) {
		const auto hWnd = *(static_cast <HWND *> (pBuf));
		if (isOpenFromShell) {
			COPYDATASTRUCT copyData {
				.cbData = static_cast <DWORD> (qstrlen(argv[1])),
				.lpData = argv[1],
			};
			SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM) &copyData);
		} else
            ErrorMsg(L"Единственно возможный экземпляр калькулятора\n"
                     "уже запущен!");
		SetForegroundWindow(hWnd);
		exit(1);
	}

	/////////////////////////// Инициализация данных //////////////////////////

	sNewline				= S("\r\n"								);
	sPathToCSVKeyName		= S("Путь к файлам CSV"					);
	sCSVFileNamePaper		= S("Бумага.csv"						);
	sCSVFileNameOperations	= S("Операции.csv"						);
	sCSVFileNameStates		= S("States.csv"						);
	sRoundCostKeyName		= S("Округление стоимости"				);
	sFileExtention			= S(".printcalc"						);
	sProgramName			= S("Полиграфический калькулятор"		);
	sSpaces					= S("     "								);
	sFontName				= S("Arial"								);
	sDefaultProductsName	= S("Листовка"							);
	sFace					= S("лицо"								);
	sBack					= S("оборот"							);
	sCoatedGlossy			= S("Мел. глянцевая"					);
	sErrorNotFit			= S("Изделие не помещается на бумагу!"	);
	sDateFormat				= S("dd.MM.yyyy"						);
	sTimeFormat				= S("hh:mm"								);

	orderPrefixes = {
		S("рз"	),
		S("п"	),
		S("дв"	),
		S("мв"	),
		S("кол"	),
	};

	simpleProductsNames = {
		sDefaultProductsName	 ,
		S("Плакат"				),
		S("Флаер"				),
		S("Буклет"				),
		S("Лифлет"				),
		S("Бланк"				),
		S("Визитная карточка"	),
		S("Наклейка (стикер)"	),
		S("Газета"				),
		S("Открытка"			),
		S("Карманный календарь"	),
		S("Флажок"				),
		S("Бейдж"				),
		S("Пригласительный"		),
		S("Билет"				),
		S("Пластиковая карта"	),
		S("Пакет"				),
		S("Ценник"				),
		S("Воблер"				),
		S("Папка"				),
		S("Этикетка"			),
	};

	mapProductKind = {
		{ S("Брошюра"				),	EProductKind::Brochure		},
		{ S("Каталог"				),	EProductKind::Brochure		},
		{ S("Журнал"				),	EProductKind::Brochure		},
		{ S("Книга"					),	EProductKind::Book			},
		{ S("Газета"				),	EProductKind::Newspaper		},
		{ S("Блокнот"				),	EProductKind::Notepad		},
		{ S("Меню"					),	EProductKind::Menu			},
		{ S("Квартальный календарь"	),	EProductKind::QuartCalend	},
		{ S("Настенный календарь"	),	EProductKind::WallCalend	},
	};

	stdProductSizes = StdProductSizes {
		{ EProductSize::SizeA7		,	{ S("А7"		),	{ 74,	105	} } },
		{ EProductSize::SizeA6		,	{ S("А6"		),	{ 105,	148	} } },
		{ EProductSize::SizeA5		,	{ S("А5"		),	{ 148,	210	} } },
		{ EProductSize::SizeA4		,	{ S("А4"		),	{ 210,	297	} } },
		{ EProductSize::SizeA3		,	{ S("А3"		),	{ 297,	420	} } },
		{ EProductSize::SizeA2		,	{ S("А2"		),	{ 420,	594	} } },
		{ EProductSize::Size210x98	,	{ S("Еврофлаер"	),	{ 98,	210	} } },
		{ EProductSize::Size90x50	,	{ S("Визитка"	),	{ 50,	90	} } },
	};
	Q_ASSERT(stdProductSizes.count() == PRODUCT_SIZES_COUNT - 1);

	stdBindingNames = {
		{ EBinding::NoBinding	, S("Без переплёта"	), },
		{ EBinding::SoftCover	, S("КБС"			), },
		{ EBinding::Staples		, S("Скобы"			), },
		{ EBinding::Wire		, S("Пружина"		), },
		{ EBinding::Rings		, S("Кольца"		), },
		{ EBinding::Bolts		, S("Болты"			), },
	};
	Q_ASSERT(stdBindingNames.count() == BINDINGS_COUNT);

	stdPaperTypes = {
		{ EPaperType::CoatedGlossy	, { sCoatedGlossy		 , false } },
		{ EPaperType::CoatedMatte	, {	S("Мел. матовая"	), false } },
		{ EPaperType::Offset		, {	S("Офсетная"		), false } },
		{ EPaperType::Adhesive		, {	S("Самоклеящаяся"	), false } },
		{ EPaperType::NewsPaper		, {	S("Газетная"		), false } },
		{ EPaperType::Carton		, {	S("Картон"			), false } },
		{ EPaperType::Labels		, {	S("Этикеточная"		), false } },
		{ EPaperType::Craft			, {	S("Крафт"			), false } },
		{ EPaperType::ColorCopy		, {	S("ColorCopy"		), true  } },
		{ EPaperType::Linen			, {	S("Лён"				), true  } },
		{ EPaperType::Shyne			, {	S("Shyne"			), true  } },
		{ EPaperType::SirioPearl	, {	S("Sirio Pearl"		), true  } },
		{ EPaperType::TouchCover	, {	S("TouchCover"		), true  } },
		{ EPaperType::Monotex		, {	S("Monotex"			), true  } },
		{ EPaperType::Raflatac		, {	S("Raflatac®"		), true  } },
		{ EPaperType::CustomPaper	, {	S("Другая"			), false } },
	};
	Q_ASSERT(stdPaperTypes.count() == PAPER_TYPES_COUNT - 1);

	using enum EPaperFormat;
	stdPaperFormats = {
		{ _64x90	, { S("64x90"		), { 640, 900  }, false } },
		{ _70x100	, { S("70x100"		), { 700, 1000 }, false } },
		{ _72x104	, { S("72x104"		), { 720, 1040 }, false } },
		{ _43x61	, { S("43x61"		), { 430, 610  }, false } },
		{ _50x70	, { S("50x70"		), { 500, 700  }, false } },
		{ _50x70pc	, { S("50x70"		), { 500, 700  }, false } },
		{ A3		, { S("A3"			), { 297, 420  }, true  } },
		{ SRA3		, { S("SRA3"		), { 320, 450  }, true  } },
		{ _330x487	, { S("330x487"		), { 330, 487  }, true  } },
		{ SRA3pc	, { S("SRA3 ПР"		), { 320, 450  }, true  } },
		{ _330x487pc	, { S("330x487 ПР"	), { 330, 487  }, true  } },
	};
	Q_ASSERT(stdPaperFormats.count() == PAPER_FORMATS_COUNT - 2);

	stdPortionNames = {
		{ EPortion::Portion1, S("1/1") },
		{ EPortion::Portion2, S("1/2") },
		{ EPortion::Portion4, S("1/4") },
		{ EPortion::Portion6, S("1/6") }
	};
	Q_ASSERT(stdPortionNames.count() == SHEET_PORTIONS_COUNT - 1);

	stdPaperWeights = {
		70,
		80,
		90,
		100,
		105,
		115,
		120,
		130,
		140,
		150,
		157,
		160,
		170,
		180,
		200,
		230,
		250,
		265,
		270,
		280,
		300,
	};

	pressNames = {
		{ EPress::Konica	, S("Konica Minolta") },
		{ EPress::Ryobi		, S("Ryobi"			) },
		{ EPress::Roland	, S("Roland"		) },
		{ EPress::Heidelberg, S("Heidelberg"	) },
		{ EPress::Riso		, S("Ризограф"		) },
	};
	Q_ASSERT(pressNames.count() == PRESSES_COUNT - 1);

	using enum EOperation;
	mapOperationNames.reserve(OPERATIONS_COUNT - 1);
	mapOperationNames = {
		{ S("Форма B3"						),	PlateB3				},
		{ S("Форма B2"						),	PlateB2				},
		{ S("Офс. печать 4+4 мин. стоим."	),	RyobiMin_4_4		},
		{ S("Офсетная печать 4+0"			),	Ryobi_4_0			},
		{ S("Офсетная печать 1+0"			),	Ryobi_1_0			},
		{ S("Ризограф 1+0"					),	Riso_1_0			},
		{ S("Ризограф 1+1"					),	Riso_1_1			},
		{ S("Konica 4+0"					),	Konica_4_0			},
		{ S("Konica 4+4"					),	Konica_4_4			},
		{ S("Konica 1+0"					),	Konica_1_0			},
		{ S("Konica 1+1"					),	Konica_1_1			},
		{ S("Фальцовка машинная"			),	FoldAuto			},
		{ S("Фальцовка ручная"				),	FoldByHand			},
		{ S("Биговка"						),	Creasing			},
		{ S("Нумерация"						),	Numbering			},
		{ S("Вырубка"						),	DieCutting			},
		{ S("Перфорация"					),	Perforation			},
		{ S("Скругление"					),	Rounding			},
		{ S("Люверс"						),	Eyelet				},
		{ S("Отверстие"						),	Hole				},
		{ S("Клеевая точка"					),	GluePoint			},
		{ S("Скретч-полоса"					),	ScratchStrip		},
		{ S("Двусторонний скотч 5 мм"		),	DoubleSidedTape5mm	},
		{ S("Двусторонний скотч 9 мм"		),	DoubleSidedTape9mm	},
		{ S("Листоподборка"					),	ImpAssembly			},
		{ S("Сборка на скобы"				),	BindStaples			},
		{ S("Сборка на болты"				),	BindBolts			},
		{ S("Сборка на кольца"				),	BindRings			},
		{ S("Сборка на пружину"				),	BindWire			},
		{ S("Сборка на КБС А5"				),	BindSoftCoverA5		},
		{ S("Сборка на КБС А4"				),	BindSoftCoverA4		},
		{ S("Курсор"						),	Cursor				},
		{ S("Ригель"						),	Rigel				},
		{ S("Фольгирование"					),	FoilSqMeter			},
		{ S("Фольгирование доп."			),	FoilExtra			},
		{ S("Ламинация офсетная"			),	LamOffset			},
		{ S("Ламинация Ultrabond"			),	LamUltraBond		},
		{ S("Ламинация цифровая"			),	LamDigital			},
		{ S("Ламинация пакетная"			),	LamPacket			},
	};
	Q_ASSERT(mapOperationNames.count() == OPERATIONS_COUNT - 2);

	stdLamNames = {
		{ ELamType::Offset,		S("Офсетная"		), },
		{ ELamType::Digital,	S("Цифровая"		), },
		{ ELamType::UltraBond,	S("UltraBond"		), },
		{ ELamType::Packet,		S("Пакетная"		), },
		{ ELamType::LamCustom,	S("Специальная"		), },
	};
	Q_ASSERT(stdLamNames.count() == enum_count<ELamType>());

	stdLamThickness = {
		30,
		40,
		75,
		125,
		175,
	};

	defaultLamThickness = {
		{ ELamType::Offset,		stdLamThickness[0],	},
		{ ELamType::Digital,	stdLamThickness[2],	},
		{ ELamType::UltraBond,	stdLamThickness[1],	},
		{ ELamType::Packet,		stdLamThickness[2],	},
		{ ELamType::LamCustom,	stdLamThickness[0],	},
	};

	lamPackageThickness = {
		stdLamThickness[2],
		stdLamThickness[3],
		stdLamThickness[4],
	};

	lamDigitalThickness = {
		stdLamThickness[2],
		stdLamThickness[3],
	};

	stdImpNames = {
		{ EImpType::Simple	, S("Простое изделие"	), },
		{ EImpType::Cover	, S("Обложка"			), },
		{ EImpType::Block	, S("Блок"				), },
		{ EImpType::Bottom	, S("Подложка"			), },
		{ EImpType::Insert	, S("Вставка"			), },
	};
	Q_ASSERT(stdImpNames.count() == IMP_TYPES_COUNT);

	using enum EIndexStates;

	mapIndexPaperFormat = {
		{ i64x90,			EPaperFormat::_64x90		, },
		{ i70x100,			EPaperFormat::_70x100		, },
		{ i72x104,			EPaperFormat::_72x104		, },
		{ i43x61,			EPaperFormat::_43x61		, },
		{ i50x70,			EPaperFormat::_50x70		, },
		{ i50x70pc,			EPaperFormat::_50x70pc		, },
		{ iA3,				EPaperFormat::A3			, },
		{ iSRA3,			EPaperFormat::SRA3			, },
		{ i330x487,			EPaperFormat::_330x487		, },
		{ iSRA3pc,			EPaperFormat::SRA3pc		, },
		{ i330x487pc,		EPaperFormat::_330x487pc		, },
	};
	Q_ASSERT(mapIndexPaperFormat.count() == PAPER_FORMATS_COUNT - 2);

	mapIndexSheetPortion = {
		{ iPortion1,	EPortion::Portion1, },
		{ iPortion2,	EPortion::Portion2, },
		{ iPortion4,	EPortion::Portion4, },
		{ iPortion6,	EPortion::Portion6, },
	};
	Q_ASSERT(mapIndexSheetPortion.count() == SHEET_PORTIONS_COUNT - 1);

	mapIndexPress = {
		{ iKonica,		EPress::Konica		},
		{ iRyobi,		EPress::Ryobi		},
		{ iRoland,		EPress::Roland		},
		{ iHeidelberg,	EPress::Heidelberg	},
		{ iRiso,		EPress::Riso		},
	};
	Q_ASSERT(mapIndexPress.count() == PRESSES_COUNT - 1);


	///////////////////////// Инициализация приложения ////////////////////////

	MyApplication a(argc, argv);
    a.setOrganizationName	(S("Сочи Пресс"     			));
    a.setOrganizationDomain	(S("sochipress.org"				));
	a.setApplicationName	(S("Полиграфический калькулятор"));

	Q_ASSERT(!settings);
	settings = new QSettings;

	const auto & sDefaultPathToCSV =
		QDir::toNativeSeparators(S("%1/CSV")
								 .arg(QDir::current().absolutePath()));
	sPathToCSV = settings->value(sPathToCSVKeyName,
								 sDefaultPathToCSV).toString();
	roundCost.SetFromInt(settings->value(sRoundCostKeyName).toLongLong());


	//////////////////// Регистрация ассоциации расширения ////////////////////

	using namespace winreg;

	RegKey rootKey(HKEY_CURRENT_USER, L"Software\\Classes");
	RegKey extKey(rootKey.Get(), sFileExtention.toStdWString());
	std::wstring sProgID { L"PrintCalc.1" };
	extKey.SetStringValue({ }, sProgID);
	RegKey progIDKey(rootKey.Get(), sProgID);
	progIDKey.SetStringValue({ }, L"Файл полиграфического калькулятора");
	const auto & sExeFileName =
		QDir::toNativeSeparators(qApp->applicationFilePath()).toStdWString();
	RegKey shellOpenKey(progIDKey.Get(), L"shell\\open\\command");
	shellOpenKey.SetStringValue({ }, L'"' + sExeFileName + LR"(" "%1")");
	RegKey defaultIconKey(progIDKey.Get(), L"DefaultIcon");
	defaultIconKey.SetStringValue({ }, sExeFileName);

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

	MainWindow w;
	w.WriteHWND();
	w.show();

	if (isOpenFromShell) {
		w.startOption = EStartOption::OpenFromShell;
		w.sOpenFileName = QString::fromLocal8Bit(argv[1]);
	}

	w.ParseCSVFiles();
	w.Init();

	return a.exec();
}


bool MyApplication::notify(QObject * receiver, QEvent * event) {
	if (event->type() == QEvent::KeyPress) {
		if (static_cast <QKeyEvent *> (event)->key() == Qt::Key_F1) {
			Help();
			return true;
		}
	}
	return QApplication::notify(receiver, event);
}


void MyApplication::Help() {
	QDir dir(S("./Help"));
	const auto & helpFileName =
			S("%1/%2").arg(dir.absolutePath(), S("index.html"));
	if (!dir.exists() or
		!QFileInfo::exists(helpFileName) or
		!QDesktopServices::openUrl(QUrl::fromLocalFile(helpFileName)))
	{
		MsgBox mb(activeWindow());
		mb.Error(S("Файлы справки не найдены"));
	}
}

