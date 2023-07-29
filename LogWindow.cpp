#include <QLayout>
#include <QCloseEvent>
#include "MsgBox.h"
#include "LogWindow.h"
#include "ui_LogWindow.h"


LogWindow::LogWindow(QWidget * parent) :
	QDialog(parent), ui(new Ui::LogWindow)
{
	ui->setupUi(this);

	textEdit = new QTextEdit(this);
	textEdit->setReadOnly(true);
	textEdit->setLineWrapMode(QTextEdit::NoWrap);

	QHBoxLayout * layout = new QHBoxLayout(this);
	layout->addWidget(textEdit);
	setLayout(layout);
}


LogWindow::~LogWindow() {
	delete ui;
}


void LogWindow::closeEvent(QCloseEvent * event) {
	if (event->spontaneous()) {
		MsgBox mb(this);
		const auto result = mb.YesNoPrompt("Закрыть также основной диалог?");
		switch (result) {
			case MsgBox::Yes: {
				const auto parent = parentWidget();
				Q_ASSERT(parent);
				if (!parent->close()) {
					event->ignore();
					return;
				}
			} break;

			case MsgBox::No:
				if (mb.YesNoPrompt("Вы точно уверены, что хотите"
								   " закрыть это окно\n"
								   "(это действие не закроет"
								   " основной диалог)?")
					!= MsgBox::Yes)
					{
						event->ignore();
						return;
					}
				break;

			default:
				event->ignore();
				return;
		}
	}
	event->accept();
}


void LogWindow::ShowText(const QList<QString> & lines) {
	textEdit->clear();
	for (const auto & s : lines)
		textEdit->append(s);
}


void LogWindow::Pin(QDialog * dlgToPin, bool moveParent, bool bToRight) {
	auto width = frameGeometry().size().width();
	auto posToPin = dlgToPin->pos();
	if (moveParent) {
		auto offset = bToRight ? posToPin.x() - width / 2 :
								 posToPin.x() + width / 2;
		dlgToPin->move(offset, posToPin.y());
	}
	resize(size().width(), dlgToPin->size().height());
	posToPin = dlgToPin->pos();
	auto leftPos = bToRight ? posToPin.x() + dlgToPin->frameSize().width() :
							  posToPin.x() - width;
	move(leftPos, posToPin.y());
}


void LogWindow::UpdateTitle(const QString & additionalText) {
	setWindowTitle(S("%1 %2").arg(sDefaultTitle, additionalText));
}
