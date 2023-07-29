#pragma once

#include <Product.h>
#include <QDialog>

namespace Ui {
class SelectImpDlg;
}

class SelectImpDlg : public QDialog {
	Q_OBJECT

	Ui::SelectImpDlg * ui;
	bool addImp;
	const Product & p;

public:
	Opt<EImpType> selImpType;

	explicit SelectImpDlg(const Product & product,
						  bool addImp,
						  QWidget * parent);
	~SelectImpDlg() override;
	int exec() override;
	void EnableBtn(EImpType impType);
};
