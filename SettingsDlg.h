#pragma once

#include <QDialog>

namespace Ui {
class SettingsDlg;
}

class SettingsDlg : public QDialog {
	Q_OBJECT

	Ui::SettingsDlg * ui;

public:
	explicit SettingsDlg(QWidget * parent);
	~SettingsDlg();
	int exec() override;
};
