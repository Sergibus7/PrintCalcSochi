#include <ranges>
#include "Product.h"


bool IsLamOperation(EOperation eOp) {
	using enum EOperation;
	return	eOp == LamOffset	or
			eOp == LamUltraBond	or
			eOp == LamDigital	or
			eOp == LamPacket;
}


EOperation LamOperation(ELamType eLamType) {
	switch (eLamType) {
		case ELamType::Offset:		return EOperation::LamOffset;
		case ELamType::Digital:		return EOperation::LamDigital;
		case ELamType::UltraBond:	return EOperation::LamUltraBond;
		case ELamType::Packet:		return EOperation::LamPacket;
		case ELamType::LamCustom:	return EOperation::LamPacket;
		default:
			Q_ASSERT(false);
			return EOperation::WrongOperation;
	}
}


auto OperationPrices::GetPrice(EOperation eOp,
							   size_t amount,
							   bool isGloss,
							   size_t thickness,
							   Opt<EProductSize> eSize) const
{
	Q_ASSERT(IsLamOperation(eOp));
	Q_ASSERT(eOp != EOperation::LamPacket or
			 (eOp == EOperation::LamPacket and eSize));
	for (const auto & lamOp : values(eOp)) {
		const auto & lam = lamOp.lam;
		Q_ASSERT(lam);
		if (lam->isGloss	== isGloss		and
			lam->thickness	== thickness	and
			((eOp != EOperation::LamPacket) or
			(eOp == EOperation::LamPacket and lam->eSize == eSize)))
			return lamOp.GetPrice(amount);
	}
	return 0.0;
}


qreal OperationPrice::GetPrice(size_t amount) const {
	if (prices.count() == 1)
		return prices.first().price;

	for (const auto p : prices | std::views::reverse)
		if (amount >= p.limit)
			return p.price;

	return 0.0;
}


QString Imposition::GetName(bool bTrueSimpleName) const {
	return (IsSimple() and !bTrueSimpleName) ? S("Спуск") : stdImpNames[eType];
}


bool Imposition::IsSpreads() const {
	return product->eBinding == EBinding::Staples or
		   (product->eBinding == EBinding::SoftCover and
			eType == EImpType::Cover);
}


void Imposition::SetDefaultPrepSheets() {
	if (IsNoPrint() or ePress == EPress::Konica) {
		prepSheets = 0;
		return;
	}

	if (ePress == EPress::Riso) {
		prepSheets = IsDuplex() ? 20 : 10;
		return;
	}

	prepSheets = inks.face < 3 ? 50 : 100;
	if (IsDuplex())
		*prepSheets += inks.back < 3 ? 50 : 100;
}


void Imposition::LayoutPages() {
	printField = bleedSize = { 0.0, 0.0 };

	if (!bAutoImposition) {
		if (totalPlaces == -1)
			totalPlaces = 1;
		return;
	}

	const auto & printSize = paper.printSize;
	// Вычисляем отступы
	if (!IsNoPrint() and bAutoIndent) {
		switch (ePress) {
			case EPress::Konica:
				switch (paper.format.eFormat) {
					case EPaperFormat::SRA3pc:
						horizIndent	= ceil((450 - 405) / 2.0);
						vertIndent	= ceil((320 - 288) / 2.0);
						break;

					case EPaperFormat::_330x487pc:
						horizIndent	= ceil((487 - 442) / 2.0);
						vertIndent	= ceil((330 - 298) / 2.0);
						break;

					default:
						horizIndent	= 4.0;
						vertIndent	= printSize.height() > 320 ? 5.0 : 4.0;
						break;
				}
				gap = 3.0;
				bleed = 2.0;
				break;

			case EPress::Riso:
				horizIndent = vertIndent = bleed = gap = 0.0;
				break;

			case EPress::Ryobi:
				if (paper.format == EPaperFormat::A3)
					horizIndent = vertIndent = bleed = gap = 0.0;
				else {
					if (paper.format == EPaperFormat::_50x70pc) {
						Q_ASSERT(paper.ePortion == EPortion::Portion2);
						horizIndent	= ceil((500 - 2 - 455) / 2.0);
						vertIndent	= ceil((350 - 2 - 314) / 2.0);
					} else {
						horizIndent	= 4.0;
						vertIndent	= 5.0;
					}
					gap		= 2.0;
					bleed	= 2.0;
				}
				break;

			default:
				if (paper.format == EPaperFormat::_50x70pc) {
					Q_ASSERT(paper.ePortion == EPortion::Portion1);
					horizIndent	= ceil((500 - 2 - 455) / 2.0);
					vertIndent	= ceil((700 - 2 - 314 * 2) / 2.0);
				} else {
					horizIndent	= 4.0;
					vertIndent	= 5.0;
				}
				horizIndent	= 4.0;
				vertIndent	= 7.0;
				gap			= 3.0;
				bleed		= 2.0;
				break;
		}
	} else {
		if (bAutoIndent)
			horizIndent = vertIndent = 0.0;
		if (bAutoGap)
			gap = bleed = 0.0;
	}

	Q_ASSERT(horizIndent >= 0.0 and vertIndent >= 0.0);
	// Вычисляем печатное поле
	printField.rwidth()	 = printSize.width() - qCeil(2 * *horizIndent);
	printField.rheight() = printSize.height() - qCeil(2 * *vertIndent);

	// Вычисляем размер макетов с учетом выхода за обрез
	const auto doubleBleed = 2.0 * *bleed;
	bleedSize = product->GetSpreadSize(eType) +
				QSizeF(doubleBleed, doubleBleed);
	if (printField.width() < bleedSize.width() or
		printField.height() < bleedSize.height())
	{
		bleedSize.transpose();
		if (!(ePress == EPress::Riso or
			  paper.eType == EPaperType::NewsPaper))
		{
			// 1 мм для меток реза
			if (printField.width() - 1 < bleedSize.width() or
				printField.height() - 1 < bleedSize.height())
				return;
		}
	}

	// Вычисляем макс. отступы от края печ. листа
	const auto wBleed = bleedSize.width();
	const auto hBleed = bleedSize.height();
	const auto bPortrait = wBleed < hBleed;
	const auto hIndent1 = (printSize.width() - wBleed) / 2.0;
	const auto vIndent1 = (printSize.height() - hBleed) / 2.0;
	const auto hIndent2 = (printSize.width() - hBleed) / 2.0;
	const auto vIndent2 = (printSize.height() - wBleed) / 2.0;
	const auto hMinIndent = qMin(hIndent1, hIndent2);
	const auto hMaxIndent = qMax(hIndent1, hIndent2);
	const auto vMinIndent = qMin(vIndent1, vIndent2);
	const auto vMaxIndent = qMax(vIndent1, vIndent2);
	Q_ASSERT(horizIndent <= hMaxIndent and vertIndent <= vMaxIndent);

 	maxIndents.rwidth() =
			(vertIndent > vMinIndent and !bPortrait) ? hMinIndent : hMaxIndent;
	maxIndents.rheight() =
			(horizIndent > hMinIndent and bPortrait) ? vMinIndent : vMaxIndent;
	Q_ASSERT(maxIndents.isValid());

	auto CalcPlaces = [gap = gap, bleed = bleed]
					  (const qreal & printField, const qreal & bleedSize)
	{
		Q_ASSERT(printField > 10.0 and bleedSize > 10.0);
		auto places = qFloor(printField / bleedSize);
		if (gap > 0.0) {
			const auto bleedAll =
					(places + 1) * bleedSize - places * (2.0 * *bleed - *gap);
			if (bleedAll <= printField)
				++places;
		}
		return places;
	};

	// Попытка разместить места на спуске в книжной ориентации
	const auto wPlaces1 = CalcPlaces(printField.width(), bleedSize.width());
	const auto hPlaces1 = CalcPlaces(printField.height(), bleedSize.height());
	const auto places1 = wPlaces1 * hPlaces1;

	// Попытка разместить места на спуске в альбомной ориентации
	const auto &bleedSizeRotated = bleedSize.transposed();
	const auto wPlaces2 = CalcPlaces(printField.width(),
									 bleedSizeRotated.width());
	const auto hPlaces2 = CalcPlaces(printField.height(),
									 bleedSizeRotated.height());
	const auto places2 = wPlaces2 * hPlaces2;

	if (!places1 and !places2)
		return;

	if (places1 > places2) {
		placesInWidth = wPlaces1;
		placesInHeight = hPlaces1;
	} else {
		placesInWidth = wPlaces2;
		placesInHeight = hPlaces2;
		bleedSize = bleedSizeRotated;
	}

	totalPlaces = *placesInWidth * *placesInHeight;

	if (IsNoPrint()) {
		Q_ASSERT(qFuzzyIsNull(*gap) and qFuzzyIsNull(*bleed));
		printField.rwidth() = bleedSize.width() * *placesInWidth;
		printField.rheight() = bleedSize.height() * *placesInHeight;
	}
}


void Imposition::CalcCost() {
	log.clear();
	const auto & totals = product->GetTotals();
	const auto sigCount = signatures.count();
	const auto sPrint S("печать: ");
	const auto sBullet = sSpaces + S("• ");
	size_t totalPaperCost = 0;
	size_t totalPlatesCost = 0;
	size_t totalPrintCost = 0;
	size_t printCost;
	qreal printPrice;

	QString sImpName;
	if (eType != EImpType::Simple)
		sImpName = S(" (%1)").arg(stdImpNames[eType]);

	log << S("Спуск: %1%2").arg(product->sName, sImpName);

	auto LogPressName = [=] () -> void {
		log << "";
		log << S("Печатная машина: %1").arg(pressNames.value(ePress));
	};

	auto LogPaper = [=] (size_t sheets) -> size_t {
		log << S("Бумага:");
		const auto paperCost = Ceil(sheets * paper.price);
		log << S("%1%2 исх. л. х %L3 р. = %4 р.")
					.arg(sBullet,
						 t(sheets),
						 t(paper.price, 2),
						 t(size_t(paperCost)));
		return paperCost;
	};

	auto LogPrintSheet = [=] (size_t printSheets,
							  qreal printPrice,
							  size_t printCost) -> void
	{
		log << S("%1%2 %3 печ. л. х %L4 р. = %5 р.")
				   .arg(sBullet,
						sPrint,
						t(printSheets),
						t(printPrice, 2),
						t(printCost));
	};

	auto LogMultipleSignature = [=] (size_t index) -> void {
		log << "";
		log << S("Печатный лист %1 (%2 сигнатура)")
				   .arg(t(index + 1),
						signatures[index].isPartial ? S("частичная") :
													  S("полная"));
	};

	switch (ePress) {
		case EPress::WrongPress: {
			Q_ASSERT(!inks.face and !inks.back);
			totalPaperCost = LogPaper(signatures.sourceSheets);
			log << "";
			log << S("Без печати");
		} break;

		case EPress::Konica: {
#ifdef _DEBUG
			if (inks.face == 4)
				Q_ASSERT((QList<size_t> { 0, 1, 4 }).contains(inks.back));
			else
				if (inks.face == 1)
					Q_ASSERT(inks.back == 0 or inks.back == 1);
				else {
					static const QList<size_t> illegal { 2, 3 };
					Q_ASSERT(!illegal.contains(inks.face) and
							 !illegal.contains(inks.back));
				}
#endif // _DEBUG

			const auto & printSheets = totals.sheetsKonica;
			const auto eop =
					IsDuplex() ? (inks.face > 1 ? EOperation::Konica_4_4 :
												  EOperation::Konica_1_1) :
								 (inks.face > 1 ? EOperation::Konica_4_0 :
												  EOperation::Konica_1_0);
			printPrice = operationPrices.GetPrice(eop, printSheets);
			Q_ASSERT(printPrice > 0.5 and printPrice < 10000);
			totalPaperCost = LogPaper(signatures.sourceSheets);
			LogPressName();

			for (qsizetype i = 0; i < sigCount; ++i) {
				if (sigCount > 1)
					LogMultipleSignature(i);
				const auto printSheets = signatures[i].prepSheets +
										 signatures[i].printSheets;
				printCost = Ceil(printSheets * printPrice);
				LogPrintSheet(printSheets, printPrice, printCost);
				totalPrintCost += printCost;
			}
		} break;

		case EPress::Riso: {
			Q_ASSERT(inks.face == 1 and (inks.back == 1 or inks.back == 0));
			const auto eop = IsDuplex() ? EOperation::Riso_1_1 :
										  EOperation::Riso_1_0;
			printPrice = operationPrices.GetPrice(eop, totals.sheetsRiso);
			totalPaperCost = LogPaper(signatures.sourceSheets);
			LogPressName();
			for (qsizetype i = 0; i < sigCount; ++i) {
				if (sigCount > 1)
					LogMultipleSignature(i);
				const auto printSheets = signatures[i].ActualPrintSheets();
				printCost = Ceil(printSheets * printPrice);
				LogPrintSheet(printSheets, printPrice, printCost);
				totalPrintCost += printCost;
			}
		} break;

		default: {
			Q_ASSERT(inks.face >= 1 and inks.face <= 4 and
					 inks.back >= 0 and inks.back <= 4 and
					 inks.face >= inks.back);

			totalPaperCost = LogPaper(signatures.sourceSheets);
			LogPressName();

			static const QString sPrintFC	{ S("полноцв."	) };
			static const QString sPrintBW	{ S("ч/б"		) };
			static const QString sB3		{ S("B3"		) };
			static const QString sB2		{ S("B2"		) };
			const auto isB3 = ePress == EPress::Ryobi;
			const size_t platePrice =
					isB3 ? totals.platesPriceB3 : totals.platesPriceB2;
			const size_t plates =
					isB3 ? signatures.platesB3 : signatures.platesB2;

			totalPlatesCost = plates * platePrice;

			const auto isFC = static_cast<bool>(signatures.hoursFC);
			const auto isBW = static_cast<bool>(signatures.hoursBW);
			Q_ASSERT(isFC or isBW);
			const size_t printCostFC =
					isFC ? Ceil(signatures.hoursFC * totals.printPriceFC) : 0;
			const size_t printCostBW =
					isBW ? Ceil(signatures.hoursBW * totals.printPriceBW) : 0;
			totalPrintCost = printCostFC + printCostBW;

			QString sMinimum;
			EOperation eMin = inks.face > 2 ?
								  IsDuplex() ? EOperation::RyobiMin_4_4 :
											   EOperation::Ryobi_4_0 :
								  EOperation::Ryobi_1_0;
			qreal minPrintCost = operationPrices.GetPrice(eMin, 0);
			bool isMinimum = (inks.face > 2 ?
								  totals.hoursFC * totals.printPriceFC :
								  totals.hoursBW * totals.printPriceBW)
							 < minPrintCost;
			if (isMinimum) {
				totalPrintCost = minPrintCost;
				sMinimum = S(" (минимум)");
			}

			using OptSignature = Opt<const Signature *>;
			auto LogPrintHours =
					[=] (bool isBW, OptSignature sig = { }) -> void {
				static auto s =
						S("%1часы печати: %2 печ. л. / 5000 = %3 ч (%4)");
				size_t sheets;
				qreal hours;
				if (sig) {
					sheets = (*sig)->printSheets;
					hours = isBW ? (*sig)->hoursBW : (*sig)->hoursFC;
				} else {
					sheets = totals.sheetsOffset;
					hours = isBW ? totals.hoursBW : totals.hoursFC;
				}
				log << s.arg(sBullet,
							 t(sheets),
							 t(hours),
							 isBW ? sPrintBW : sPrintFC);
			};

			if (sigCount == 1) {
				auto LogPrintCost = [=] (bool isBW) -> void {
					static auto s = S("%1%2 %3 ч х %4 р. = %5 р. (%6)");
					log << s.arg(sBullet,
								 sPrint,
								 t(isBW ? signatures.hoursBW :
										  signatures.hoursFC),
								 t(isBW ? totals.printPriceBW :
										  totals.printPriceFC),
								 t(isBW ? printCostBW : printCostFC),
								 isBW ? sPrintBW : sPrintFC);
				};

				log << S("%1формы %2: %3 шт. x %4 р. = %5 р.").
							arg(sBullet,
								isB3 ? sB3 : sB2,
								t(plates),
								t(platePrice),
								t(totalPlatesCost));
				if (isMinimum)
					log << S("%1%2%3 р.%4").arg(sBullet,
												sPrint,
												t(totalPrintCost),
												sMinimum);
				else {
					auto optSig = /*std::make_optional<const Signature *>
									(*/&signatures.first()/*)*/;
					if (isBW)
						LogPrintHours(true, optSig);
					if (isFC)
						LogPrintHours(false, optSig);
					if (isBW)
						LogPrintCost(true);
					if (isFC)
						LogPrintCost(false);
				}

				log << S("%1Итого формы + печать %2 р.").
						   arg(sSpaces, t(totalPlatesCost + totalPrintCost));
			} else {
				for (qsizetype i = 0; i < sigCount; ++i) {
					const auto & sig = signatures[i];
					LogMultipleSignature(i);
					log << S("%1формы %2: %3 шт.").
							arg(sBullet,
								isB3 ? sB3 : sB2,
								t(isB3 ? sig.platesB3 : sig.platesB2));

					if (isFC)
						LogPrintHours(false, /*std::make_optional
									  <const Signature *>(*/&sig/*)*/);
					if (isBW)
						LogPrintHours(true, /*std::make_optional
									  <const Signature *>(*/&sig/*)*/);
				}

				log << "";
				log << S("Итого формы %1 шт. х %2 р. = %3 р.").
						arg(t(plates),
							t(platePrice),
							t(plates * platePrice));
				const auto & sTotalPrintTemplate =
						S("Итого печать %1 ч х %2 р. = %3 р.%4");

				if (isFC)
					log << sTotalPrintTemplate.
							arg(t(signatures.hoursFC),
								t(totals.printPriceFC),
								t(Ceil(signatures.hoursFC *
									   totals.printPriceFC)),
								sMinimum);
				if (isBW)
					log << sTotalPrintTemplate.arg(t(signatures.hoursBW),
												   t(totals.printPriceBW),
												   t(totalPrintCost),
												   sMinimum);
			}
		}
		break;
	}

	const auto & pp = postpress;
	const auto & lam = pp.lamination;
	const auto copies = product->kinds.GetCopies();
	SizeT lamCost = 0;
	if (lam) {
		log << "";

		SizeT lamSheets;
		if (lam->eType == ELamType::Packet) {
			Q_ASSERT(!product->IsSizeLargerA3());
			// Считаем ламинацию по кол-ву экз. изделия
			lamSheets = copies;
		} else {
			// Считаем ламинацию по кол-ву печ. листов
			lamSheets = signatures.printSheets;
			if (lam->bBack)
				lamSheets *= 2;
			if (paper.IsPrintSizeLargerB3())
				// Формат печатного листа B2, пересчитываем в А3
				lamSheets *= 2;
		}

		lamCost = Ceil(*lam->price * lamSheets);
		const auto lamSetUp =
				operationPrices.GetSetupCost(LamOperation(lam->eType));
		lamCost += lamSetUp;

		auto s = stdLamNames[lam->eType];
		if (lam->eType == ELamType::LamCustom)
			s += S(": %1").arg(lam->sCustomName);
		log << S("Ламинация: %1").arg(s);
		log << S("%1прил. %2 р. + %3 шт. x %4 р. = %5 р.").
				arg(sSpaces,
					t(lamSetUp),
					t(lamSheets),
					t(*lam->price, 2),
					t(lamCost));
	}

	SizeT foilCostFace = 0, foilCostBack = 0;
	const auto & foil = pp.foil;
	if (foil) {
		log << "";
		log << S("Фольгирование:");
		const auto foilSetUp =
				operationPrices.GetSetupCost(EOperation::FoilSqMeter);
		Q_ASSERT(foil->face);
		const auto sqMetersFace = foil->face->GetSquareMeters() * copies;
		const auto sqMetersBack =
			(foil->back ? foil->back->GetSquareMeters() : 0) * copies;
		const auto sqMetersTotal = sqMetersFace + sqMetersBack;
		const auto foilPriceSqMeters =
				operationPrices.GetPrice(EOperation::FoilSqMeter,
										 sqMetersTotal);
		const auto foilPriceExtra =
				operationPrices.GetPrice(EOperation::FoilExtra, copies);
		foilCostFace = foilSetUp +
					   Ceil(foilPriceSqMeters * sqMetersFace) +
					   Ceil(copies * foilPriceExtra);

		auto LogFoil = [=] (bool face) -> void {
			const auto & sqMeters = face ? sqMetersFace : sqMetersBack;
			auto sNum = loc.toString(sqMeters, 'f');
			auto dotPos = sNum.indexOf(loc.decimalPoint());
			Q_ASSERT(dotPos != -1);
			auto nonZeroPos = sNum.length() - 1;
			for ( ; nonZeroPos > dotPos; --nonZeroPos)
				if (sNum.at(nonZeroPos) != '0')
					break;
			if (nonZeroPos == dotPos)
				++nonZeroPos;
			sNum.truncate(nonZeroPos + 1);
			const auto dblSpaces = sSpaces + sSpaces;
			log << S("%1%2:").arg(sSpaces, (face ? sFace : sBack));
			log << S("%1прил. %2 р. +").arg(dblSpaces, t(foilSetUp));
			log << S("%1%2 кв. м х %3 р. +")
					.arg(dblSpaces,
						 sNum,
						 t(size_t(foilPriceSqMeters)));
			log << S("%1%2 экз. х %3 р. = %4 р.")
				   .arg(dblSpaces,
						t(copies),
						t(foilPriceExtra),
						t(face ? foilCostFace : foilCostBack));
		};

		LogFoil(true);
		if (foil->back) {
			foilCostBack = foilSetUp +
						   Ceil(foilPriceSqMeters * sqMetersBack) +
						   Ceil(copies * foilPriceExtra);
			LogFoil(false);
		}
	}

	// Вычисление стоимости операций постпечати
	auto CalcPP = [=] (EOperation eOp, size_t amount) -> size_t {
		if (!amount)
			return 0;

		const auto & units = (eOp == EOperation::DoubleSidedTape5mm or
							  eOp == EOperation::DoubleSidedTape9mm) ?
									S("м") : S("шт.");
		size_t	totalCost = 0,	// Полная стоимость операции
				runs,			// Кол-во выполнений операции
				setupCost;		// Стоимость приладки операции
		qreal	quantity,		// Кол-во операций в одном изделии
				operationPrice;	// Цена операции

		Q_ASSERT(eType != EImpType::Cover or
				 (eType == EImpType::Cover and product->nPagesCover <= 4));
		Q_ASSERT(eType != EImpType::Bottom or
				 (eType == EImpType::Bottom and product->nPagesBottom <= 4));

		if (eOp == EOperation::Numbering)
			quantity = 1;
		else
			if (eOp == EOperation::DieCutting)
				quantity = pp.dieCutting->count;
			else
				quantity = amount;

		if (!(eType == EImpType::Block or eType == EImpType::Insert)) {
			runs = copies * amount;
			if (eOp == EOperation::DoubleSidedTape5mm or
				eOp == EOperation::DoubleSidedTape9mm)
				runs = Ceil((runs * pp.dblSidedTape->length) / 1000.0);
		} else {
			size_t spreads = eType == EImpType::Block ?
								product->nPagesBlock : product->nPagesInsert;
			if (product->eBinding == EBinding::Staples)
				if (IsDuplex())
					spreads /= 4;
				else
					spreads /= 2;
			else
				if (IsDuplex())
					spreads /= 2;
			Q_ASSERT(spreads);
			runs = copies * spreads;
		}

		operationPrice = operationPrices.GetPrice(eOp, runs);
		setupCost = operationPrices.GetSetupCost(eOp);
		totalCost = setupCost + quantity * runs * operationPrice;

		log << "";
		log << S("%1:").arg(mapOperationNames.key(eOp));
		log << S("%1прил. %2 р. + %3 x %4 %5 x %6 р. = %7 р.")
				.arg(sSpaces,
					 t(setupCost),
					 t(quantity),
					 t(copies),
					 units,
					 t(operationPrice, 2),
					 t(totalCost));

		return totalCost;
	};

	const auto creasingCost		= pp.creasings ? 
			CalcPP(EOperation::Creasing, *pp.creasings) :
			0;
	const auto foldingCost		= pp.foldings ?
			CalcPP(pp.foldings->foldByHand ?
						EOperation::FoldByHand : EOperation::FoldAuto,
				   pp.foldings->count) :
			0;
	const auto numerationCost	= pp.nums ?
			CalcPP(EOperation::Numbering, pp.nums->end - pp.nums->start + 1) :
			0;
	const auto perforationCost	= pp.perforations ?
			CalcPP(EOperation::Perforation, *pp.perforations) : 0;
	const auto roundingCost		= pp.rndCorners ?
			CalcPP(EOperation::Rounding, pp.rndCorners->count) : 0;
	const auto eyeletCost		= pp.eyelets ?
			CalcPP(EOperation::Eyelet, pp.eyelets->count) : 0;
	const auto holesCost		= pp.holes ?
			CalcPP(EOperation::Hole, pp.holes->count) : 0;
	const auto gluePointCost	= pp.gluePoints ?
			CalcPP(EOperation::GluePoint, *pp.gluePoints) : 0;
	const auto scratchStripCost	= pp.scrathStrips ?
			CalcPP(EOperation::ScratchStrip, *pp.scrathStrips) : 0;
	const auto dblSidedTapeCost		= pp.dblSidedTape ?
			CalcPP(pp.dblSidedTape->width == 5 ?
						   EOperation::DoubleSidedTape5mm :
						   EOperation::DoubleSidedTape9mm,
				   pp.dblSidedTape->count)
			: 0;
	const auto dieCuttingCost	= pp.dieCutting ?
			CalcPP(EOperation::DieCutting, pp.dieCutting->count) : 0;

	const auto postPressCost =
		lamCost + foilCostFace + foilCostBack + foldingCost +
		creasingCost + numerationCost + perforationCost +
		roundingCost + eyeletCost + holesCost + gluePointCost +
		scratchStripCost + dblSidedTapeCost + dieCuttingCost;

	cost = roundCost.R(totalPaperCost +
					   totalPlatesCost +
					   totalPrintCost +
					   postPressCost);
	log << "";
	if (sigCount > 1) {
		QString s;
		if (ePress != EPress::WrongPress and
			ePress != EPress::Konica and
			ePress != EPress::Riso)
			s = S("формы + ");
		log << S("Итого %1печать %2 р.").
				arg(s, t(totalPlatesCost + totalPrintCost));
		log << "";
	}
	log << S("Общая стоимость спуска %1 р.").arg(t(*cost));
}


void Imposition::CalcSheets() {
	Q_ASSERT(totalPlaces	and	*totalPlaces	>= 0 and
			 placesInWidth	and	*placesInWidth	>= 0 and
			 placesInHeight	and	*placesInHeight	>= 0);
	signatures.clear();
	if (!*totalPlaces)
		return;

	const auto & p = *product;
	decltype(ProductKind::copies) copies = p.kinds.GetCopies();
	Q_ASSERT(copies and paper.ePortion != EPortion::WrongPortion);
	if (!prepSheets)
		SetDefaultPrepSheets();

	decltype(Product::nPagesBlock) pages;
	switch (eType) {
		using enum EImpType;
		case Block:		pages = p.nPagesBlock;	break;
		case Bottom:	pages = p.nPagesBottom;	break;
		case Cover:		pages = p.nPagesCover;	break;
		case Insert:	pages = p.nPagesInsert;	break;
		default:		pages = 2;				break;
	}

	/* Если переплет скобы или это обложка КБС,
	 * считаем листы как кол-во разворотов */
	const auto sheets = IsSpreads() ? pages / 4 : pages / 2;
	Q_ASSERT(sheets >= 1);

	if (IsSimple() or (eType == EImpType::Block and p.bIdenticalPages)) {
		signatures.Add(copies * sheets,
					   false,
					   IsOwnBack() /*eOwnBack == EOwnBack::Forced*/);
		return;
	}

//	auto IsOwnBack = [this] () -> bool {
//		if (!IsDuplex())
//			return false;
//
//		switch (eOwnBack) {
//			case EOwnBack::Forced:
//				Q_ASSERT(IsOwnBackAllowed());
//				return true;
//
//			case EOwnBack::Allowed:
//				return IsOwnBackAllowed();
//
//			default:
//				return false;
//		}
//	};

	Q_ASSERT(pages >= 2 and totalPlaces != -1);

#ifdef _DEBUG
	if (IsSpreads())
		Q_ASSERT(pages % 4 == 0);
	else
		Q_ASSERT(pages % 2 == 0);
#endif // _DEBUG

	size_t fullSigCount = totalPlaces ? sheets / *totalPlaces : 0;
	size_t restSheets = sheets - fullSigCount * *totalPlaces;
	if (restSheets < 0)
		restSheets = 0;

	bool isRestSheetsOwnBack;
	if (fullSigCount) {
		size_t fullSigCopies = fullSigCount ? copies * *totalPlaces : 1;
		bool fullSigOwnBack =
			fullSigCount > 1 or restSheets ? false : IsOwnBack();
		// Записываем все полные сигнатуры
		for (size_t i = 0; i < fullSigCount; i++)
			signatures.Add(fullSigCopies, false, fullSigOwnBack);
		isRestSheetsOwnBack = IsOwnBack();
	} else
		isRestSheetsOwnBack = IsOwnBack(); // eOwnBack == EOwnBack::Forced;

	// Остаток листов для спуска
	while (restSheets) {
		// Ищем наибольшее кол-во листов для спуска, кратное totalPlaces
		size_t maxRestPlaces = *totalPlaces / 2;
		while (restSheets < maxRestPlaces)
			maxRestPlaces /= 2;
		const auto partialSheets =
			Ceil(copies / (*totalPlaces / qreal(maxRestPlaces)));
		signatures.Add(*totalPlaces * partialSheets,
					   true,
					   isRestSheetsOwnBack);
		restSheets -= maxRestPlaces;
	}
}


bool Imposition::IsOwnBackAllowed() const {
	if (!IsDuplex() or ePress == EPress::Konica)
		return false;
	Q_ASSERT(ePress != EPress::WrongPress or
			 (ePress == EPress::WrongPress and !inks.face and !inks.back));
	using enum EImpType;
	const auto & p = *product;
	if (p.eBinding == EBinding::Staples) {
		if (*placesInWidth % 2 != 0)
			return false;
		if (eType == Block)
			return (p.nPagesBlock % 4) / *placesInWidth >= 2;
		else
			if (eType == Insert)
				return (p.nPagesInsert % 4) / *placesInWidth >= 2;
	} else {
		const bool isNotepadIdenticalPages =
				p.eKind == EProductKind::Notepad and
				p.bIdenticalPages and
				(eType == Block or eType == Insert);
		if ((IsSimple() or isNotepadIdenticalPages) and
			totalPlaces >= 2 and
			placesInWidth >= 2 and
			*placesInWidth % 2 == 0)
			return true;
	}
	return false;
}


void Impositions::Add(Imposition * imp) {
	Q_ASSERT(imp);
	Q_ASSERT((imp->IsSimple() and (isEmpty() or contains(EImpType::Simple))) or
			 (!imp->IsSimple()) and !contains(EImpType::Simple));
	insert(imp->eType, imp);
	Q_ASSERT(count() < IMP_TYPES_COUNT);
}


////////////////////////////////  Load / Save  ////////////////////////////////

QDataStream & operator <<(QDataStream & ds, const Color & c) {
	ds	<< c.eColor
		<< c.customColorName;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Color & c) {
	ds	>> c.eColor
		>> c.customColorName;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Paper & paper) {
	Q_ASSERT(paper.IsValid());
	ds << paper.eType;
	ds << paper.format;
	ds << paper.ePortion;
	ds << paper.bCuttingEdge;
	ds << paper.sCustomName;
//	ds << paper.color;
	ds << paper.price;
	ds << paper.weight;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Paper & paper) {
	ds >> paper.eType;
	ds >> paper.format;
	ds >> paper.ePortion;
	ds >> paper.bCuttingEdge;
	ds >> paper.sCustomName;
//	ds >> paper.color;
	ds >> paper.price;
	ds >> paper.weight;
	Q_ASSERT(paper.IsValid());
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Paper::Format & format) {
	ds << format.epf;
	ds << format.w;
	ds << format.h;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Paper::Format & format) {
	ds >> format.epf;
	ds >> format.w;
	ds >> format.h;
	return ds;
}


//_CONST(auto, INVALID_LAM_PRICE, -1.0)

QDataStream & operator <<(QDataStream & ds, const Lamination & lam) {
//#ifdef _DEBUG
//	Q_ASSERT(lam.IsValid());
//#endif // _DEBUG
	ds	<< lam.eType
		<< lam.bGloss
		<< lam.sCustomName
		<< lam.price/*.value_or(INVALID_LAM_PRICE)*/
		<< lam.thickness
		<< lam.bBack;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Lamination & lam) {
	ds	>> lam.eType
		>> lam.bGloss
		>> lam.sCustomName
		>> lam.price
		>> lam.thickness
		>> lam.bBack;

//#ifdef _DEBUG
//	Q_ASSERT(lam.IsValid());
//	if (!qFuzzyCompare(*lam.price, INVALID_LAM_PRICE)) {
//		Q_ASSERT(*lam.price > 0.0 and *lam.price <= MAX_LAM_PRICE);
//	}
//#endif // _DEBUG

	return ds;
}


QDataStream & operator <<(QDataStream & ds, const FoilSide & foilSide) {
	ds	<< foilSide.width
		<< foilSide.height
		<< foilSide.color;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, FoilSide & foilSide) {
	ds	>> foilSide.width
		>> foilSide.height
		>> foilSide.color;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Foil & foil) {
	ds	<< foil.face
		<< foil.back;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Foil & foil) {
	ds	>> foil.face
		>> foil.back;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Postpress::Folding & dc) {
	ds << dc.count;
	ds << dc.foldByHand;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Postpress::Folding & dc) {
	ds >> dc.count;
	ds >> dc.foldByHand;
	return ds;
}


QDataStream & operator <<(QDataStream & ds,
						  const Postpress::DblSidedTape & dc)
{
	ds << dc.count;
	ds << dc.length;
	ds << dc.width;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Postpress::DblSidedTape & dc) {
	ds >> dc.count;
	ds >> dc.length;
	ds >> dc.width;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Postpress::RndCorners & dc) {
	ds << dc.count;
	ds << dc.radius;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Postpress::RndCorners & dc) {
	ds >> dc.count;
	ds >> dc.radius;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Postpress::Eyelets & dc) {
	ds << dc.count;
	ds << dc.diam;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Postpress::Eyelets & dc) {
	ds >> dc.count;
	ds >> dc.diam;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Postpress::Holes & dc) {
	ds << dc.count;
	ds << dc.diam;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Postpress::Holes & dc) {
	ds >> dc.count;
	ds >> dc.diam;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Postpress::Numbering & dc) {
	ds << dc.start;
	ds << dc.end;
	ds << dc.digits;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Postpress::Numbering & dc) {
	ds >> dc.start;
	ds >> dc.end;
	ds >> dc.digits;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Postpress::DieCutting & dc) {
	ds << dc.count;
	ds << dc.sName;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Postpress::DieCutting & dc) {
	ds >> dc.count;
	ds >> dc.sName;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Postpress & pp) {
	ds << pp.lamination;
	ds << pp.foil;
	ds << pp.creasings;
	ds << pp.foldings;
	ds << pp.perforations;
	ds << pp.gluePoints;
	ds << pp.dblSidedTape;
	ds << pp.scrathStrips;
	ds << pp.rndCorners;
	ds << pp.eyelets;
	ds << pp.holes;
	ds << pp.nums;
	ds << pp.dieCutting;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Postpress & pp) {
	ds >> pp.lamination;
	ds >> pp.foil;
	ds >> pp.creasings;
	ds >> pp.foldings;
	ds >> pp.perforations;
	ds >> pp.gluePoints;
	ds >> pp.dblSidedTape;
	ds >> pp.scrathStrips;
	ds >> pp.rndCorners;
	ds >> pp.eyelets;
	ds >> pp.holes;
	ds >> pp.nums;
	ds >> pp.dieCutting;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Imposition & imp) {
	ds << imp.eType;
	ds << imp.paper;
	ds << imp.prepSheets;
	ds << imp.inks.face;
	ds << imp.inks.back;
	ds << imp.ownBack; //eOwnBack;
	ds << imp.ePress;
	ds << imp.bAutoImposition;
	ds << imp.bAutoIndent;
	ds << imp.bAutoGap;
	ds << imp.placesInWidth;
	ds << imp.placesInHeight;
	ds << imp.totalPlaces;
	ds << imp.vertIndent;
	ds << imp.horizIndent;
	ds << imp.bleed;
	ds << imp.gap;
	ds << imp.postpress;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Imposition & imp) {
	ds >> imp.eType;
	ds >> imp.paper;
	ds >> imp.prepSheets;
	ds >> imp.inks.face;
	ds >> imp.inks.back;
	ds >> imp.ownBack; //eOwnBack;
	ds >> imp.ePress;
	ds >> imp.bAutoImposition;
	ds >> imp.bAutoIndent;
	ds >> imp.bAutoGap;
	ds >> imp.placesInWidth;
	ds >> imp.placesInHeight;
	ds >> imp.totalPlaces;
	ds >> imp.vertIndent;
	ds >> imp.horizIndent;
	ds >> imp.bleed;
	ds >> imp.gap;
	ds >> imp.postpress;
	//Q_ASSERT(imp.IsValid());
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Impositions & imps) {
	ds << (quint8) imps.count();
	for (const auto imp : imps)
		ds << *imp;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Impositions & imps) {
	quint8 cnt;
	ds >> cnt;
	size_t impsCount = cnt;
	Q_ASSERT(impsCount >= 0 and impsCount < IMP_TYPES_COUNT);
	for (size_t i = 0; i < impsCount; i++) {
		auto imp = new Imposition;
		ds >> *imp;
		imps.Add(imp);
	}
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const ProductKind & kind) {
	ds << kind.copies;
	ds << kind.packs;
	ds << kind.name.value_or(QString());
	return ds;
}

QDataStream & operator >>(QDataStream & ds, ProductKind & kind) {
	ds >> kind.copies;
	ds >> kind.packs;
	QString s;
	ds >> s;
	if (!s.isEmpty())
		kind.name = s;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const ProductKinds & kinds) {
	ds << (quint8) kinds.count();
	for (const auto & kind : kinds)
		ds << kind;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, ProductKinds & kinds) {
	quint8 cnt;
	ds >> cnt;
	size_t kindsCount = cnt;
	Q_ASSERT(kindsCount >= 0 and kindsCount <= 100);
	kinds.clear();
	kinds.resize(kindsCount);
	for (auto & kind : kinds)
		ds >> kind;
	return ds;
}

QDataStream & operator <<(QDataStream & ds, const Product & p) {
	ds << p.eKind;
	ds << p.sName;
	ds << p.size;
	ds << p.eBinding;
	ds << p.bindDiam;
	ds << p.nBindCount;
	ds << p.bindColor;
	ds << p.bUserBindPrice;
	ds << p.bLongSideBind;
	ds << p.nPagesCover;
	ds << p.nPagesBlock;
	ds << p.nPagesBottom;
	ds << p.nPagesInsert;
	ds << p.bIdenticalPages;
	ds << p.bCurcor;
	ds << p.cursorColor;
	ds << p.bRigel;
	ds << p.rigelColor;
	ds << p.kinds;
	ds << p.impositions;
	ds << p.notes;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Product & p) {
	ds >> p.eKind;
	ds >> p.sName;
	ds >> p.size;
	ds >> p.eBinding;
	ds >> p.bindDiam;
	ds >> p.nBindCount;
	ds >> p.bindColor;
	ds >> p.bUserBindPrice;
	ds >> p.bLongSideBind;
	ds >> p.nPagesCover;
	ds >> p.nPagesBlock;
	ds >> p.nPagesBottom;
	ds >> p.nPagesInsert;
	ds >> p.bIdenticalPages;
	ds >> p.bCurcor;
	ds >> p.cursorColor;
	ds >> p.bRigel;
	ds >> p.rigelColor;
	ds >> p.kinds;
	ds >> p.impositions;
	ds >> p.notes;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Product::Size & s) {
	ds << s.eSize;
	ds << s.width;
	ds << s.height;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Product::Size & s) {
	ds >> s.eps;
	ds >> s.w;
	ds >> s.h;
	return ds;
}


QDataStream & operator <<(QDataStream & ds, const Order & order) {
	ds << order.sPrefix;
	ds << order.num;
	ds << order.customer.name;
	ds << order.customer.phone;
	ds << order.customer.delivery.sDestination;
	ds << order.customer.delivery.sRecipient;
	ds << order.customer.delivery.sRecipientPhone;
	ds << order.customer.delivery.sNotes;
	ds << order.dateOpen;
	ds << order.dateDead;
	ds << order.timeDead;
	ds << order.sNotes;
	ds << order.sPathToFiles;
	return ds;
}

QDataStream & operator >>(QDataStream & ds, Order & order) {
	ds >> order.sPrefix;
	ds >> order.num;
	ds >> order.customer.name;
	ds >> order.customer.phone;
	ds >> order.customer.delivery.sDestination;
	ds >> order.customer.delivery.sRecipient;
	ds >> order.customer.delivery.sRecipientPhone;
	ds >> order.customer.delivery.sNotes;
	ds >> order.dateOpen;
	ds >> order.dateDead;
	ds >> order.timeDead;
	ds >> order.sNotes;
	ds >> order.sPathToFiles;
	return ds;
}


QSize Paper::GetPrintSize() const {
	Q_ASSERT(format.IsCustom()							or
			 (!bCuttingEdge and format.IsDigital())		or
			 (bCuttingEdge and !format.IsDigital()));

	auto printSize = format.size;
	if (printSize.width() < printSize.height())
		printSize.transpose();

	switch (ePortion) {
		case EPortion::Portion1:
			break;

		case EPortion::Portion2:
			printSize.rwidth() /= 2;
			break;

		//case EPortion::Portion3:
		//	printSize.rwidth() /= 3;
		//	break;

		case EPortion::Portion6:
			printSize.rwidth() /= 2;
			printSize.rheight() /= 3;
			break;

		default:
			printSize.rwidth() /= 2;
			printSize.rheight() /= 2;
			break;
	}

	if (bCuttingEdge) {
		printSize.rwidth() -= 2;
		printSize.rheight() -= 2;
	}

	if (printSize.width() < printSize.height())
		printSize.transpose();

	return printSize;
}


bool Paper::IsStdWeight(decltype(weight) weight) {
	return stdPaperWeights.contains(weight);
}


#ifdef _DEBUG

using TestPair = std::tuple<size_t /*amount*/, qreal /*expected result*/>;

void TestFunc(EOperation eOp, const QString & sTitle, QList<TestPair> pairs) {
	qDebug().noquote() << sTitle;
	for (const auto & pair : pairs) {
		const auto & [amount, expected] = pair;
		qreal price;
		if (IsLamOperation(eOp)) {
			if (eOp == EOperation::LamDigital)
				price = operationPrices.GetPrice(eOp, amount, true, 75);
			else
				if (eOp == EOperation::LamOffset)
					price = operationPrices.GetPrice(eOp, amount, true, 30);
				else
					Q_ASSERT(false);
		} else
			price = operationPrices.GetPrice(eOp, (eOp == EOperation::PlateB3 or
												   eOp == EOperation::PlateB2) ?
														amount / 4 : amount);
		qDebug().noquote() <<
			S("Value = %1  price = %2 (exp. %3)  --> %4")
				.arg(amount, 7)
				.arg(price, 7, 'f', 2)
				.arg(expected, 7, 'f', 2)
				.arg(qFuzzyCompare(expected, price) ? "passed" : "FAILED");
	}
	qDebug();
};
#endif // _DEBUG


//void Product::UpdateTotals() {
//	auto GetPlatePrice = [&] (bool isB3) -> size_t {
//		const auto sets = isB3 ? totals.platesSetsB3 : totals.platesSetsB2;
//		if (!sets)
//			return 0;
//		const auto op = isB3 ? EOperation::PlateB3 : EOperation::PlateB2;
//		return operationPrices.GetPrice(op, sets);
//	};
//
//	auto GetPrintPrice = [&] (bool isBW) -> size_t {
//		const auto & hours = isBW ? totals.hoursBW : totals.hoursFC;
//		const auto op = isBW ? EOperation::Ryobi_1_0 : EOperation::Ryobi_4_0;
//		return operationPrices.GetPrice(op, hours);
//	};
//
//	totals.hoursFC			= totals.hoursBW		=
//	totals.sheetsOffset		= totals.sheetsRiso		= totals.sheetsKonica =
//	totals.platesB3			= totals.platesB2		=
//	totals.platesSetsB3		= totals.platesSetsB2	= 
//	totals.platesPriceB3	= totals.platesPriceB2	= 
//	totals.printPriceFC		= totals.printPriceBW	= 0;
//
//	for (const auto imp : qAsConst(impositions)) {
//		const auto & s = imp->signatures;
//		switch (imp->ePress) {
//			case EPress::WrongPress:
//				break;
//
//			case EPress::Riso:
//				totals.sheetsRiso += s.ActualPrintSheets();
//				break;
//
//			case EPress::Konica:
//				totals.sheetsKonica += s.ActualPrintSheets();
//				break;
//
//			default:
//				totals.platesB3			+= s.platesB3;
//				totals.platesB2			+= s.platesB2;
//				totals.platesSetsB3		+= s.platesSetsB3;
//				totals.platesSetsB2		+= s.platesSetsB2;
//				totals.hoursFC			+= s.hoursFC;
//				totals.hoursBW			+= s.hoursBW;
//				totals.sheetsOffset		+= s.printSheets;
//				totals.platesPriceB3	= GetPlatePrice(true);
//				totals.platesPriceB2	= GetPlatePrice(false);
//				totals.printPriceFC		= GetPrintPrice(false);
//				totals.printPriceBW		= GetPrintPrice(true);
//				break;
//		}
//	}
//}


void Product::CalcCost() {
#ifdef _DEBUG
	QList<TestPair> hoursPairs {
		{ trunc(0.3			),	2500 },
		{ trunc(0.5			),	2500 },
		{ trunc(2.5			),	2400 },
		{ trunc(3.0			),	2100 },
		{ trunc(3.3			),	2100 },
		{ trunc(4.99		),	2100 },
		{ trunc(5.0			),	1900 },
		{ trunc(7.2			),	1900 },
		{ trunc(10.0		),	1800 },
		{ trunc(10.00001	),	1800 },
		{ trunc(13.0		),	1800 },
		{ trunc(15.5		),	1600 },
		{ trunc(25.0		),	1500 },
		{ trunc(29.99999	),	1500 },
		{ trunc(30.0		),	1400 },
		{ trunc(49.0		),	1400 },
		{ trunc(10000.0		),	1400 },
	};

	QList<TestPair> lamDigitalPairs {
		{ 5,	50 },
		{ 10,	50 },
		{ 50,	35 },
		{ 90,	35 },
		{ 100,	27 },
		{ 150,	27 },
		{ 200,	24 },
		{ 299,	24 },
		{ 300,	22 },
		{ 500,	22 },
		{ 9999,	22 },
	};

	QList<TestPair> lamOffsetPairs {
		{ 30,		8.0 },
		{ 99,		8.0 },
		{ 100,		8.0 },
		{ 300,		7.0 },
		{ 500,		6.0 },
		{ 1000,		5.0 },
		{ 2000,		4.0 },
		{ 2500,		4.0 },
		{ 3000,		3.0 },
		{ 5000,		2.5 },
		{ 9999,		2.5 },
		{ 10000,	2.0 },
		{ 777777,	2.0 },
	};

	QList<TestPair> konicaPairs {
		{ 9,	40 },
		{ 24,	40 },
		{ 25,	30 },
		{ 49,	30 },
		{ 50,	28 },
		{ 90,	28 },
		{ 100,	23 },
		{ 250,	20 },
		{ 700,	18 },
		{ 1000,	16 },
		{ 1999,	16 },
		{ 2000,	14 },
		{ 9999,	14 },
	};

	QList<TestPair> platesB3Pairs {
		{ 1,	300 },
		{ 4,	300 },
		{ 16,	275 },
		{ 30,	275 },
		{ 40,	250 },
		{ 100,	250 },
		{ 900,	250 },
	};

	QList<TestPair> platesB2Pairs {
		{ 1,	600 },
		{ 4,	600 },
		{ 10,	600 },
		{ 16,	500 },
		{ 100,	500 },
	};

	QList<TestPair> foldPairs {
		{ 1,		0.3		},
		{ 5000,		0.3		},
		{ 10000,	0.2		},
		{ 20000,	0.2		},
		{ 50000,	0.15	},
		{ 150000,	0.15	},
	};

	TestFunc(EOperation::PlateB3	, "Формы B3"			, platesB3Pairs		);
	TestFunc(EOperation::PlateB2	, "Формы B2"			, platesB2Pairs		);
	TestFunc(EOperation::Ryobi_4_0	, "Часы печати"			, hoursPairs		);
	TestFunc(EOperation::Konica_4_0	, "Печать Коника"		, konicaPairs		);
	TestFunc(EOperation::LamOffset	, "Офсетная ламинация"	, lamOffsetPairs	);
	TestFunc(EOperation::LamDigital	, "Цифровая ламинация"	, lamDigitalPairs	);
	TestFunc(EOperation::FoldAuto	, "Машинная фальцовка"	, foldPairs			);
#endif // _DEBUG

	log.clear();
	size_t assemblyCost = 0, bindCost = 0, cursorCost = 0, rigelCost = 0;
	const auto copies = kinds.GetCopies();


	totals.hoursFC			= totals.hoursBW		=
	totals.sheetsOffset		= totals.sheetsRiso		= totals.sheetsKonica =
	totals.platesB3			= totals.platesB2		=
	totals.platesSetsB3		= totals.platesSetsB2	=
	totals.platesPriceB3	= totals.platesPriceB2	=
	totals.printPriceFC		= totals.printPriceBW	= 0;

	for (auto imp : impositions) {
		imp->LayoutPages();
		imp->CalcSheets();
	}

	auto GetPlatePrice = [&] (bool isB3) -> size_t {
		const auto sets = isB3 ? totals.platesSetsB3 : totals.platesSetsB2;
		if (!sets)
			return 0;
		const auto op = isB3 ? EOperation::PlateB3 : EOperation::PlateB2;
		return operationPrices.GetPrice(op, sets);
	};

	auto GetPrintPrice = [&] (bool isBW) -> size_t {
		const auto & hours = isBW ? totals.hoursBW : totals.hoursFC;
		const auto op = isBW ? EOperation::Ryobi_1_0 : EOperation::Ryobi_4_0;
		return operationPrices.GetPrice(op, hours);
	};

	for (const auto imp : qAsConst(impositions)) {
		const auto & sig = imp->signatures;
		switch (imp->ePress) {
			case EPress::WrongPress:
				break;

			case EPress::Riso:
				totals.sheetsRiso += sig.ActualPrintSheets();
				break;

			case EPress::Konica:
				totals.sheetsKonica += sig.ActualPrintSheets();
				break;

			default:
				totals.platesB3			+= sig.platesB3;
				totals.platesB2			+= sig.platesB2;
				totals.platesSetsB3		+= sig.platesSetsB3;
				totals.platesSetsB2		+= sig.platesSetsB2;
				totals.hoursFC			+= sig.hoursFC;
				totals.hoursBW			+= sig.hoursBW;
				totals.sheetsOffset		+= sig.printSheets;
				totals.platesPriceB3	= GetPlatePrice(true);
				totals.platesPriceB2	= GetPlatePrice(false);
				totals.printPriceFC		= GetPrintPrice(false);
				totals.printPriceBW		= GetPrintPrice(true);
				break;
		}
	}

//	UpdateTotals();

	if (IsSimple()) {
		auto imp = impositions.GetImp(EImpType::Simple);
		if (imp)
			imp->CalcCost();
		log << S("Стоимость спуска: %1 р.").arg(t(imp ? imp->GetCost() : 0));
	} else {
		bool isBlockDigital = false;
		for (auto imp : impositions) {
			if (imp->eType == EImpType::Block and
				imp->ePress == EPress::Konica)
				isBlockDigital = true;
			imp->CalcCost();
			log << S("Спуск: %1, стоимость: %2 р.")
				   .arg(imp->GetName(), t(imp->GetCost()));
			log << "";
		}

		// Стоимость сборки

		auto sBind = stdBindingNames[eBinding];
		if (eBinding == EBinding::NoBinding) {
			log << sBind;
			return;
		}

		if (!isBlockDigital and
			eBinding == EBinding::Staples and
			((eKind == EProductKind::Notepad and !bIdenticalPages) or
			 eKind != EProductKind::Notepad))
		{
			const size_t divider = eBinding == EBinding::Staples ? 4 : 2;
			const size_t spreads = nPagesCover	 / divider +
								nPagesBlock  / divider +
								nPagesBottom / divider +
								nPagesInsert / divider;
			const auto assemblySetUp =
					operationPrices.GetSetupCost(EOperation::ImpAssembly);
			const auto quantity = copies * spreads;
			const auto assemblyPrice =
					operationPrices.GetPrice(EOperation::ImpAssembly,
											 quantity);
			assemblyCost = assemblySetUp + quantity * assemblyPrice;

			log << S("Листоподборка:");
			log << S("%1прил. %2 р. + %3 экз. х %4 разв. х %5 р. = %6 р.").
					arg(sSpaces,
						t(assemblySetUp),
						t(copies),
						t(spreads),
						t(assemblyPrice, 2),
						t(assemblyCost));
			log << "";
		}

		EOperation eBind;
		if (eBinding == EBinding::SoftCover) {
			const auto w = size.GetWidth();
			const auto h = size.GetHeight();
			const auto length = bLongSideBind ? qMax(w, h) : qMin(w, h);
			// Если "длина" переплета превышает "середину" между А4 и А5, берём А4
			eBind = (length > 297 - (297 - 210) / 2) ?
						EOperation::BindSoftCoverA4 :
						EOperation::BindSoftCoverA5;
		} else
			switch (eBinding) {
				case EBinding::Bolts:	eBind = EOperation::BindBolts;	 break;
				case EBinding::Rings:	eBind = EOperation::BindRings;	 break;
				case EBinding::Staples:	eBind = EOperation::BindStaples; break;
				case EBinding::Wire:	eBind = EOperation::BindWire;	 break;
				default:
					eBind = EOperation::WrongOperation;
					Q_ASSERT(false);
			}

		const auto bindSetUp = operationPrices.GetSetupCost(eBind);
		auto bindPrice = operationPrices.GetPrice(eBind, kinds.GetCopies());
		bUserBindPrice = qFuzzyIsNull(bindPrice);
		if (bUserBindPrice)
			bindPrice = this->bindPrice;
		else
			this->bindPrice = bindPrice;
		bindCost = Ceil(bindSetUp + copies * nBindCount * bindPrice);

		if (eBinding != EBinding::SoftCover)
			sBind = loc.toLower(sBind);
		if (eBinding == EBinding::Wire)
			sBind.back() = u'у';

		log << S("Сборка на %1:").arg(sBind);
		log << S("%1прил. %2 р. + %3 экз. х %4 x %5 р. = %6 р.")
				.arg(sSpaces,
					 t(bindSetUp),
					 t(copies),
					 t(nBindCount),
					 t(bindPrice, 2),
					 t(bindCost));
	}

	if (bCurcor or bRigel) {
		const auto LogCost = [&] (bool isCursor) -> void {
			const auto eOp = isCursor ? EOperation::Cursor : EOperation::Rigel;
			auto & cost = isCursor ? cursorCost : rigelCost;
			const auto setUp = operationPrices.GetSetupCost(eOp);
			const auto price =
					operationPrices.GetPrice(eOp, kinds.GetCopies());
			cost = setUp + Ceil(copies * price);
			log << S("%1:").arg(mapOperationNames.key(eOp));
			log << S("%1прил. %2 р. + %3 экз. х %4 р. = %5 р.")
				.arg(sSpaces, t(setUp), t(copies), t(price, 2), t(cost));
		};

		if (bCurcor)
			LogCost(true);
		if (bRigel)
			LogCost(false);
	}

	size_t impsCost = 0;
	for (const auto & imp : qAsConst(impositions))
		impsCost += imp->GetCost();
	cost = roundCost.R(impsCost + assemblyCost + bindCost +
					   cursorCost + rigelCost);
	log << "";
	const qreal priceOneCopy = cost / (qreal) copies;
	log << S("Стоимость изделия: %1 р. (%2 р./экз.)").
		   arg(t(cost), t(priceOneCopy, 2));
}


const QSize Product::GetSpreadSize(EImpType impType) const {
	auto sz = size.GetSize();
	if (eBinding == EBinding::Staples or
		(eBinding == EBinding::SoftCover and impType == EImpType::Cover))
	{
		Q_ASSERT(sz.width() < INT_MAX / 2 and sz.height() < INT_MAX / 2);
		if (bLongSideBind)
			sz.rwidth() *= 2;
		else
			sz.rheight() *= 2;
	}
	return sz;
}


void PaperPricesByFormat::AddPrice(const QSize & size, size_t weight, qreal price) {
	Q_ASSERT(size.isValid() and size.width() < 1500 and size.height() < 1500);
	if (contains(size)) {
		auto & pw = (*this)[size];
		pw.AddPrice(weight, price);
	} else {
		PaperWeightPrices wp;
		wp.AddPrice(weight, price);
		insert(size, wp);
	}
}


qreal PaperPricesByFormat::GetPrice(const QSize & size, size_t weight) const {
	Q_ASSERT(size.isValid() and size.width() < 1500 and size.height() < 1500);
	return contains(size) ? value(size).GetPrice(weight) : 0.0;
}


void PaperPricesByType::AddPrice(EPaperType type, const QSize & size,
								 size_t weight, qreal price)
{
	Q_ASSERT(type != EPaperType::WrongPaper);
	if (contains(type))
		(*this)[type].AddPrice(size, weight, price);
	else {
		PaperPricesByFormat fp;
		fp.AddPrice(size, weight, price);
		insert(type, fp);
	}
}


qreal PaperPricesByType::GetPrice(EPaperType type,
								  const QSize & size,
								  size_t weight)
{
	Q_ASSERT(type != EPaperType::WrongPaper);
	return (contains(type)) ? value(type).GetPrice(size, weight) : 0.0;
}


size_t ProductKinds::GetCopies() const {
	if (IsOnlyOneKind())
		return first().copies;
	size_t copies = 0;
	for (const auto & kind : *this)
		copies += kind.copies;
	return copies;
}


size_t ProductKinds::GetPacks() const {
	if (IsOnlyOneKind())
		return first().packs;
	size_t packs = 0;
	for (const auto & kind : *this)
		packs += kind.packs;
	return packs;
}


size_t ProductKinds::GetEvery() const {
	Q_ASSERT(!isEmpty());
	Q_ASSERT(first().copies > 0 and first().packs > 0);
	const auto every = Ceil(first().copies / qreal(first().packs));
	return count() > 1 ? 0 : every;
}


void Lamination::FillPrice(size_t sheets) {
	Q_ASSERT(thickness);
	_CONST(auto, ENOSIZE, EProductSize::CustomSize)
	if (/*!IsLam() or */IsSpecial())
		price = 0.0;
	else
		price = operationPrices.GetPrice(LamOperation(eType),
										 sheets,
										 bGloss,
										 *thickness,
										 eType == ELamType::Packet ? eSize :
																	 ENOSIZE);
}


void Signatures::Add(size_t copies, bool isPartial, bool isOwnBack) {
	Q_ASSERT(!isOwnBack or (isOwnBack and imp->inks.back));

	Signature sig;
	sig.printSheets = Ceil(copies / qreal(imp->GetTotalPlaces()));
	const auto multi = Ceil(sig.printSheets / LIMIT_PER_PLATE);
	Q_ASSERT(multi >= 1 and multi <= 100);
	sig.prepSheets = *imp->prepSheets * multi;
	sig.sourceSheets = Ceil(sig.ActualPrintSheets() /
							qreal(imp->paper.ePortion));
	sig.isPartial = isPartial;
	sig.isOwnBack = isOwnBack;
	sig.platesB2 = sig.platesB3 = 0;
	sig.hoursFC = sig.hoursBW = 0.0;

	if (imp->IsOffsetPress()) {
		auto AddHours = [&] (auto hours, bool isFace) -> void {
			if ((isFace ? imp->inks.face : imp->inks.back) > 2)
				sig.hoursFC += hours + PREP_HOURS_FULL_COLOR;
			else
				sig.hoursBW += hours + PREP_HOURS_BW;
		};

		auto AddPlates = [&] (bool isFace, bool isB3) -> void {
			auto & plates = isB3 ? sig.platesB3 : sig.platesB2;
			plates += multi * (isFace ? imp->inks.face : imp->inks.back);
			++(isB3 ? platesSetsB3 : platesSetsB2);
		};

		bool isB3 = !imp->paper.IsPrintSizeLargerB3();
		AddPlates(true, isB3);
		const auto ps = isB3 ? sig.printSheets : sig.printSheets * 2;
		//										 ^^^^^^^^^^^^^^^^^^^
		// ________________ Если размер листа А2, пересчитываем в А3
		const auto hours = ps / SHEETS_PER_HOUR;
		AddHours(hours, true);
		if (imp->IsDuplex()) {
			AddHours(hours, false);
			if (!isOwnBack)
				AddPlates(false, isB3);
		}

		platesB3	+= sig.platesB3;
		platesB2	+= sig.platesB2;
		hoursFC		+= sig.hoursFC;
		hoursBW		+= sig.hoursBW;
	}

	append(sig);
	prepSheets		+= sig.prepSheets;
	printSheets		+= sig.printSheets;
	sourceSheets	+= sig.sourceSheets;
	++(isPartial ? partialCount : fullCount);
}


void Product::Size::SetSize(EProductSize eProductSize,
							SizeT width,
							SizeT height)
{
	eps = eProductSize;
	if (eps == EProductSize::CustomSize) {
		ASSERT_RANGE(w, PRODUCT_SIZE);
		ASSERT_RANGE(h, PRODUCT_SIZE);
		w = width;
		h = height;
	} else {
		const auto & size = stdProductSizes.GetSize(eps);
		Q_ASSERT(size.isValid());
		w = size.width();
		h = size.height();
	}
}


void Paper::Format::SetFormat(EPaperFormat eFormat,
							  SizeT width,
							  SizeT height)
{
	epf = eFormat;
	if (IsCustom()) {
		w = width;
		h = height;
	} else {
		const auto & size = stdPaperFormats.GetSize(epf);
		Q_ASSERT(size.isValid());
		w = size.width();
		h = size.height();
	}
	ASSERT_RANGE(w, PAPER_SIZE);
	ASSERT_RANGE(h, PAPER_SIZE);
}
