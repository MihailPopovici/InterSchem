#include "button.h"

Button* NewButton() {
	Button* p = new Button;
	p->label = "";
	p->fontSize = -1;
	p->padding = -1;
	p->x = -1;
	p->y = -1;
	p->width = -1;
	p->height = -1;
	return p;
}
void SetButtonColors(Button* p, Color bgColor, Color textColor) {
	p->bgColor = bgColor;
	p->textColor = textColor;
}
void SetButtonLabel(Button* p, const char* label, int fontSize, int padding) { // TODO: replace const char* with string& ?? 
	p->label = label;
	p->fontSize = fontSize;
	p->padding = padding;
	p->width = MeasureText(label, fontSize) + 2 * padding;
	p->height = fontSize + 2 * padding;
}
void SetButtonPosition(Button* p, float x, float y) {
	p->x = x;
	p->y = y;
}
bool IsButtonClicked(Button* p) {
	int mx = GetMouseX(), my = GetMouseY();
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mx >= p->x && mx <= p->x + p->width && my >= p->y && my <= p->y + p->height;
}
void DrawButton(Button* p) {
	DrawRectangle(p->x, p->y, p->width, p->height, p->bgColor);
	DrawText(p->label.c_str(), p->x + p->padding, p->y + p->padding, p->fontSize, p->textColor);
}