#pragma once

#include "raylib.h"
#include <string>

struct SingleLineText {
	int x, y, width, height;
	int fontSize, padding;
	bool visible, focused;
	Color bgColor, fontColor;
	std::string str;
};

SingleLineText* NewSingleLineText();
void SetSingleLineTextPosition(SingleLineText* stext, int x, int y);
void SetSingleLineTextColors(SingleLineText* stext, Color bgColor, Color fontColor);
void SetSingleLineTextFontSize(SingleLineText* stext, int fontSize);
void SetSingleLineTextPadding(SingleLineText* stext, int padding);
void SetSingleLineTextString(SingleLineText* stext, std::string str);
bool IsSingleLineTextHovered(SingleLineText* stext);
bool IsSingleLineTextClicked(SingleLineText* stext);
void ResizeSingleLineText(SingleLineText* stext);
void InsertCharSingleLineText(SingleLineText* stext, char c);
void EraseCharSingleLineText(SingleLineText* stext);
void ClearStrSingleLineText(SingleLineText* stext);
void DrawSingleLineText(SingleLineText* stext);