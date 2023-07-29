#include "AboutDlg.hpp"
#include "ui_AboutDlg.h"


AboutDlg::AboutDlg(QWidget * parent) :
	QDialog(parent), ui(new Ui::AboutDlg)
{
	ui->setupUi(this);
}


AboutDlg::~AboutDlg() {
	delete ui;
}
