#pragma once

#include <windows.h>
#include <QWidget>
#include <QApplication>
#include "Globals.h"

_CONST(size_t,	BUF_SIZE			, INT16_MAX - 8		)
_CONST(size_t,	MAX_BUF_LEN			, BUF_SIZE - 1		)
_CONST(size_t,	CAPTION_SIZE		, 256				)
_CONST(size_t,	MAX_CAPTION_LEN		, CAPTION_SIZE - 1	)

extern TCHAR text[BUF_SIZE];
extern TCHAR caption[CAPTION_SIZE];

class MsgBox {
	enum class EType {
		Info,
		Error,
		YesNoPrompt,
		SavePrompt,
		ExitPrompt,
	};

	HWND hParent;

	void FillBuf(const QString & s, TCHAR * buf = text);

	int ShowMessage(EType type,
					const QString & text = { },
					bool yesByDefault = false);

public:
	enum EResult : size_t {
		Yes		= size_t(IDYES		),
		No		= size_t(IDNO		),
		Cancel	= size_t(IDCANCEL	),
	};

	MsgBox(const QWidget * parent);

	inline void Info(const QString & information) {
		ShowMessage(EType::Info, information);
	}

	inline void Error(const QString & errorText) {
		ShowMessage(EType::Error, errorText);
	}

	inline auto YesNoPrompt(const QString & prompt, bool yesByDefault = false) {
		return EnumCast<EResult>(ShowMessage(EType::YesNoPrompt,
											 prompt,
											 yesByDefault));
	}

	inline auto SavePrompt() {
		return EnumCast<EResult>(ShowMessage(EType::SavePrompt));
	}

	inline auto ExitPrompt() {
		return EnumCast<EResult>(ShowMessage(EType::ExitPrompt));
	}
};
