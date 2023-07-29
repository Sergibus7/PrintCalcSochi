#pragma once

#include <QDialog>
#include "Product.h"


namespace Ui {
class DeleteImpDlg;
}


class DeleteImpDlg : public QDialog {
	Q_OBJECT

	Ui::DeleteImpDlg * ui;
	Impositions & imps;

public:
	explicit DeleteImpDlg(Impositions & impositions, QWidget * parent);
	~DeleteImpDlg();
	int exec() override;
};
