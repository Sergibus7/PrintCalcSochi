#include "MsgBox.h"
#include "DeleteImpDlg.h"
#include "ui_DeleteImpDlg.h"


DeleteImpDlg::DeleteImpDlg(Impositions & impositions, QWidget * parent) :
	QDialog(parent), ui(new Ui::DeleteImpDlg), imps(impositions)
{
	ui->setupUi(this);

	Q_ASSERT(!imps.isEmpty() and !imps.contains(EImpType::Simple));

	const auto & buttons = ui->bgImps->buttons();
	size_t i = 0;
	for (const auto type : enum_values<EImpType>()) {
		if (type == EImpType::Simple)
			continue;
		auto btn = buttons[i++];
		ui->bgImps->setId(btn, enum_integer(type));
		btn->setText(stdImpNames[type]);
	}

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [=] {
		QList<EImpType> checked;
		for (const auto btn : ui->bgImps->buttons())
			if (btn->isChecked())
				checked << EnumCast<EImpType>(ui->bgImps->id(btn));
		if (!checked.isEmpty()) {
			MsgBox mb(this);
			if (mb.YesNoPrompt(S("Действительно удалить выбранные спуски?"))
				!= MsgBox::Yes)
				return;
		}
		for (const auto type : checked)
			imps.remove(type);
		accept();
	});
}


DeleteImpDlg::~DeleteImpDlg() {
	delete ui;
}


int DeleteImpDlg::exec() {
	setFixedSize(size());
	for (auto btn : ui->bgImps->buttons()) {
		bool isExist = imps.contains(EnumCast<EImpType>(ui->bgImps->id(btn)));
		btn->setEnabled(isExist);
		btn->setChecked(isExist and imps.count() == 1);
	}
	return QDialog::exec();
}
