#include "SelectImpDlg.h"
#include "ui_SelectImpDlg.h"


SelectImpDlg::SelectImpDlg(const Product & product,
						   bool addImp,
						   QWidget * parent) :
	QDialog(parent), ui(new Ui::SelectImpDlg), addImp(addImp), p(product)
{
	ui->setupUi(this);

	connect(ui->clbCover, &QAbstractButton::clicked, this, [=] {
		selImpType = EImpType::Cover;
		accept();
	});

	connect(ui->clbBottom, &QAbstractButton::clicked, this, [=] {
		selImpType = EImpType::Bottom;
		accept();
	});

	connect(ui->clbInsert, &QAbstractButton::clicked, this, [=] {
		selImpType = EImpType::Insert;
		accept();
	});

	connect(ui->clbBlock, &QAbstractButton::clicked, this, [=] {
		selImpType = EImpType::Block;
		accept();
	});
}


SelectImpDlg::~SelectImpDlg() {
	delete ui;
}


int SelectImpDlg::exec() {
	Q_ASSERT(!p.IsSimple());
	Q_ASSERT(p.nPagesCover	> 0 or
			 p.nPagesBottom	> 0 or
			 p.nPagesBlock	> 0 or
			 p.nPagesInsert	> 0);

	setFixedSize(size());
	EnableBtn(EImpType::Cover);
	EnableBtn(EImpType::Block);
	EnableBtn(EImpType::Bottom);
	EnableBtn(EImpType::Insert);

	Q_ASSERT(ui->clbCover->	isEnabled() or
			 ui->clbBlock->	isEnabled() or
			 ui->clbBottom->isEnabled() or
			 ui->clbInsert->isEnabled());

	return QDialog::exec();
}


void SelectImpDlg::EnableBtn(EImpType impType) {
	size_t pages;
	QCommandLinkButton * clb;
	switch (impType) {
		case EImpType::Cover:
			clb = ui->clbCover;
			pages = p.nPagesCover;
			break;

		case EImpType::Block:
			clb = ui->clbBlock;
			pages = p.nPagesBlock;
			break;

		case EImpType::Bottom:
			clb = ui->clbBottom;
			pages = p.nPagesBottom;
			break;

		case EImpType::Insert:
			clb = ui->clbInsert;
			pages = p.nPagesInsert;
			break;

		default:
			clb = nullptr;
			pages = -1;
			break;
	}

	Q_ASSERT(clb and pages != -1);
	bool isExist = p.impositions.contains(impType);
	clb->setEnabled(addImp ? pages and !isExist : isExist);
}
