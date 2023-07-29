#pragma once

#include <QDialog>
#include <QTextEdit>


namespace Ui {
	class LogWindow;
}


class LogWindow : public QDialog {
	Q_OBJECT

	Ui::LogWindow * ui;

	QTextEdit * textEdit = nullptr;
	const QString sDefaultTitle { "Журнал расчета" };

	void closeEvent(QCloseEvent * event) override;

public:
	explicit LogWindow(QWidget * parent);
	~LogWindow();
	void ShowText(const QList<QString> & lines);
	void UpdateTitle(const QString & additionalText);
	void Pin(QDialog * dlgToPin, bool moveParent, bool bToRight = true);
};

