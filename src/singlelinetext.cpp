#include "singlelinetext.h"

SingleLineText* NewSingleLineText() {
	SingleLineText* p = new SingleLineText{ 0 };
	return p;
}
void SetSingleLineTextPosition(SingleLineText* stext, int x, int y) {
	stext->x = x;
	stext->y = y;
}
void SetSingleLineTextColors(SingleLineText* stext, Color bgColor, Color fontColor) {
	stext->bgColor = bgColor;
	stext->fontColor = fontColor;
}
void SetSingleLineTextFontSize(SingleLineText* stext, int fontSize) {
	int width = MeasureText(stext->str.c_str(), fontSize) + 2 * stext->padding;
	int height = fontSize + 2 * stext->padding;
	stext->width = width;
	stext->height = height;
	stext->fontSize = fontSize;
}
void SetSingleLineTextPadding(SingleLineText* stext, int padding) {
	stext->width = stext->width - 2 * stext->padding + 2 * padding;
	stext->height = stext->height - 2 * stext->padding + 2 * padding;
	stext->padding = padding;
}
void SetSingleLineTextString(SingleLineText* stext, std::string str) {
	stext->width = MeasureText(str.c_str(), stext->fontSize) + 2 * stext->padding;
	stext->str = str;
}
bool IsSingleLineTextHovered(SingleLineText* stext) {
	int mx = GetMouseX(), my = GetMouseY();
	return mx >= stext->x && mx <= stext->x + stext->width && my >= stext->y && my <= stext->y + stext->height;
}
bool IsSingleLineTextClicked(SingleLineText* stext) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsSingleLineTextHovered(stext);
	//stext->focused = true; // TODO:
}
bool IsSingleLineTextFocused(SingleLineText* stext) { //TODO: unnecessary?
	return stext->focused;
}
void ResizeSingleLineText(SingleLineText* stext) {
	stext->width = MeasureText(stext->str.c_str(), stext->fontSize) + 2 * stext->padding;
}
void InsertCharSingleLineText(SingleLineText* stext, char c) { //TODO: where pos
	stext->str += c;
	ResizeSingleLineText(stext);
}
void EraseCharSingleLineText(SingleLineText* stext) { //TODO: where pos
	if (stext->str.empty()) {
		return;
	}
	stext->str.erase(stext->str.end() - 1);
	ResizeSingleLineText(stext);
}
void ClearStrSingleLineText(SingleLineText* stext) {
	stext->str.clear();
	ResizeSingleLineText(stext);
}
void DrawSingleLineText(SingleLineText* stext) {
	DrawRectangle(stext->x, stext->y, stext->width, stext->height, stext->bgColor);
	DrawText(stext->str.c_str(), stext->x + stext->padding, stext->y + stext->padding, stext->fontSize, stext->fontColor);
}