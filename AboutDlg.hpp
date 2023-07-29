#ifndef ABOUTDLG_HPP
#define ABOUTDLG_HPP

#include <QDialog>


namespace Ui {
class AboutDlg;
}

class AboutDlg : public QDialog {
	Q_OBJECT
	Ui::AboutDlg * ui;

public:
	explicit AboutDlg(QWidget *);
	~AboutDlg();
};

#endif // ABOUTDLG_HPP
