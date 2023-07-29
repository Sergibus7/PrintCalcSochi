#include "MsgBox.h"

TCHAR text[BUF_SIZE];
TCHAR caption[CAPTION_SIZE];
//bool isCaptionFilled = false;

MsgBox::MsgBox(const QWidget * parent) {
	//Q_ASSERT(parent);
	hParent = parent ? HWND(parent->winId()) : NULL;
}


void MsgBox::FillBuf(const QString & s, TCHAR * buf) {
	size_t nullPos;
	Q_ASSERT(buf == text or buf == caption);
	const qsizetype MAX_LEN = buf == text ? MAX_BUF_LEN : MAX_CAPTION_LEN;
	if (s.length() > MAX_LEN) {
		auto sTruncated = s;
		sTruncated.truncate(MAX_LEN);
		sTruncated.toWCharArray(buf);
		nullPos = sTruncated.length();
	} else {
		s.toWCharArray(buf);
		nullPos = s.length();
	}
	buf[nullPos] = TCHAR(0);
}


int MsgBox::ShowMessage(EType type, const QString & sText, bool yesByDefault) {
	static QString sWarning S("Предупреждение");
	switch (type) {
		case EType::Info:
			FillBuf(S("Информация"), caption);
			FillBuf(sText);
			MessageBox(hParent, text, caption, MB_ICONINFORMATION);
			break;

		case EType::Error:
			FillBuf(sText);
			MessageBox(hParent, text, NULL, MB_ICONERROR);
			break;

		case EType::YesNoPrompt:
			FillBuf(S("Запрос"), caption);
			FillBuf(sText);
			return MessageBox(hParent, text, caption,
							  MB_ICONWARNING |
							  MB_YESNO |
							  (yesByDefault ? MB_DEFBUTTON1 : MB_DEFBUTTON2));

		case EType::SavePrompt:
			FillBuf(sWarning, caption);
			FillBuf(S("Имеются несохраненные данные.\nСохранить изменения?"));
			return MessageBox(hParent, text, caption,
							  MB_ICONWARNING |
							  MB_YESNOCANCEL |
							  MB_DEFBUTTON1);

		case EType::ExitPrompt: {
			FillBuf(sWarning, caption);
			FillBuf(S("Действительно завершить работу?"));
			return MessageBox(hParent, text, caption,
							  MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2);
		}
	}
	return Cancel;
}
