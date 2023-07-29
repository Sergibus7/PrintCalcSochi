#pragma once

#include <QDialog>
#include "Product.h"
#include "LogWindow.h"

namespace Ui {
class ProductDlg;
}

class QSpinBox;

class ProductDlg : public QDialog {
	Q_OBJECT

	Ui::ProductDlg * ui;

	const QString sOne			{ S("Спуск удалён!"					) };
	const QString sMany			{ S("Спуски удалены!"				) };
	const QString sAddNotes		{ S("Добавить примечания..."		) };
	const QString sEditNotes	{ S("Редактировать примечания..."	) };

	bool			canChange;
	bool			bPacksCanChange = true;
	EProductSize	oldESize		= DEFAULT_PRODUCT_SIZE;
	QSize			oldSize			=
		stdProductSizes.GetSize(DEFAULT_PRODUCT_SIZE);
	LogWindow *		logWindow		= nullptr;
	Impositions *	imps;
	ProductKinds *	kinds;

	void UpdateProductSize();
	void EnableImpButtons();
	void DeleteImposition(EImpType type = EImpType::Simple);
	void DeleteAllImpositions();
	void CheckAndSetPagesCount(EImpType impType = EImpType::Simple);
	//void CheckAndToggleConverts();
	void CheckCalendar();
	void SetDefaultPages();
	void SetBindings(EBinding binding, bool setByDefault = true);
	void SetDefaultBinding();
	void ChangeCopies(size_t newCopies = WRONG);
	void CheckNotes();
	void CheckNotepad();
	void CalcTotalPages(bool reflectToProduct = false);
	void SetStepForPages();
	void SetValueSilent(QSpinBox * sb, size_t value);
	void RecalcCost();

public:
	//bool isModified;
	Product * p;

	explicit ProductDlg(Product * prod, QWidget * parent);
	int exec() override;
	// return true if accepted, false if rejected or closed
	static bool ExecEditImposition(Product * product,
								   EImpType eImpType,
								   bool bAdd,
								   QWidget * parent);
	~ProductDlg() override;
};
