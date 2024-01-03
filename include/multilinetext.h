#pragma once

#include <string>
#include <vector>
#include "raylib.h"

struct Window;

struct MultiLineText {
	Font font; // TODO: should be able to use more than one font
	float fontSize, chWidth;

	std::vector<std::string> text;
	size_t lin, col;
	size_t visLin, visCol;
	size_t firstLin, firstCol;
	size_t limLin, limCol;

	bool focused;

	int x, y, width, height;
	float padding;
	Color textColor, bgColor;

	Window* window;
};

void MultiLineTextMergeLines(MultiLineText* mtext, size_t destination, size_t source);
void MultiLineTextToPreviousLine(MultiLineText* mtext);
void MultiLineTextToNextLine(MultiLineText* mtext);
void MultiLineTextToPreviousColumn(MultiLineText* mtext);
void MultiLineTextToNextColumn(MultiLineText* mtext);
void MultiLineTextEnter(MultiLineText* mtext);
void MultiLineTextBackspace(MultiLineText* mtext);
void MultiLineTextPaste(MultiLineText* mtext);
void MultiLineTextCopy(MultiLineText* mtext);
MultiLineText* NewMultiLineText(int startX, int startY, size_t visibleLines, size_t visibleColumns, Font font, float fontSize, float padding, Color textColor, Color bgColor);
void CleanupMultiLineText(MultiLineText* mtext);
void MultiLineTextDraw(MultiLineText* mtext);
std::vector<std::string> MultiLineTextParseText(MultiLineText* mtext, std::string strToParse);
void MultiLineTextInsertLine(MultiLineText* mtext, size_t pos, std::string line);
void MultiLineTextPushLine(MultiLineText* mtext, std::string line = "");
void MultiLineTextPushString(MultiLineText* mtext, std::string str);
void MultiLineTextOverrideLine(MultiLineText* mtext, size_t pos, std::string line);
void MultiLineTextEdit(MultiLineText* mtext);
void MultiLineTextSetPosition(MultiLineText* mtext, int x, int y);
bool MultiLineTextIsHovered(MultiLineText* mtext);
bool MultiLineTextIsClicked(MultiLineText* mtext);
void MultiLineTextClear(MultiLineText* mtext);
void MultiLineTextSetLimit(MultiLineText* mtext, size_t limLin, size_t limCol);
void MultiLineTextSetLimitMax(MultiLineText* mtext);
int MultiLineTextGetNextInt(MultiLineText* mtext);