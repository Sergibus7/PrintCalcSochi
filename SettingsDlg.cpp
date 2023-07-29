#include <QFileDialog>
#include <QDir>
#include <QSettings>
#include "Globals.h"
#include "MsgBox.h"
#include "SettingsDlg.h"
#include "ui_SettingsDlg.h"


SettingsDlg::SettingsDlg(QWidget * parent) :
	QDialog(parent), ui(new Ui::SettingsDlg)
{
	ui->setupUi(this);

	connect(ui->btnOK, &QAbstractButton::clicked, this, [=] {
		MsgBox mb(this);
		const auto & path = ui->lePath->text();

		auto FileExist = [&mb, path] (const QString & sFileName) -> bool {
			if (!QFile::exists(path + '\\' + sFileName)) {
				mb.Error(S("Файл %1 по указанному пути не существует!\n"
						   "Выберите другую папку!").arg(sFileName));
				return false;
			}
			return true;
		};

		if (path.isEmpty() or !QDir(path).exists()) {
			mb.Error(S("Указан несуществующий путь!"));
			return;
		}

		if (!FileExist(sCSVFileNamePaper) or
			!FileExist(sCSVFileNameOperations))
		{
			mb.Error(S("Файлы CSV по указанному пути не существуют!\n"
					   "Укажите правильный путь."));
			return;
		}

		if (ui->leManagerName->text().isEmpty()) {
			mb.Error(S("Имя менеджера не заполнено!"));
			return;
		}

		if (ui->leManagerPhone->text().isEmpty()) {
			mb.Error(S("Телефон менеджера пуст!"));
			return;
		}

		sPathToCSV = path;
		roundCost.SetFromIndex(ui->horSlider->value());

		manager.sName = ui->leManagerName->text();
		manager.sPhone = ui->leManagerPhone->text();
		manager.sEmail = ui->leManagerEmail->text();
		Q_ASSERT(!manager.IsEmpty());

		while (!settings->group().isEmpty())
			settings->endGroup();

		settings->beginGroup(sManagerGroupName);
		settings->setValue(sManagerNameKey, manager.sName);
		settings->setValue(sManagerPhoneKey, manager.sPhone);
		settings->setValue(sManagerEmailKey, manager.sEmail);
		settings->endGroup();
		settings->sync();

		accept();
	});

	connect(ui->btnCancel, &QAbstractButton::clicked, this, [=] {
		reject();
	});

	connect(ui->pbBrowse, &QAbstractButton::clicked, this, [=] {
		const auto path =
			QFileDialog::getExistingDirectory(this,
											  "Выберите папку",
											  ui->lePath->text());
		ui->lePath->setText(QDir::toNativeSeparators(path));
	});

	connect(ui->horSlider, &QSlider::valueChanged, this, [=] (int newValue) {
		roundCost.SetFromIndex(newValue);
	});
}


SettingsDlg::~SettingsDlg() {
	delete ui;
}


int SettingsDlg::exec() {
	setFixedSize(size());
	ui->lePath->setText(sPathToCSV);
	ui->horSlider->setValue(roundCost);
	ui->leManagerName->setText(manager.sName);
	ui->leManagerPhone->setText(manager.sPhone);
	ui->leManagerEmail->setText(manager.sEmail);
	return QDialog::exec();
}
