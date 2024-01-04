#include "button.h"

Button* NewButton() {
	Button* p = new Button{};
	return p;
}
void SetButtonColors(Button* p, Color bgColor, Color textColor) {
	p->bgColor = bgColor;
	p->textColor = textColor;
}
void SetButtonLabel(Button* p, std::string label, int fontSize, int padding) { // TODO: replace const char* with string& ?? 
	p->label = label;
	p->fontSize = fontSize;
	p->padding = padding;
	p->width = MeasureText(label.c_str(), fontSize) + 2 * padding;
	p->height = fontSize + 2 * padding;
}
void SetButtonPosition(Button* p, int x, int y) {
	p->x = x;
	p->y = y;
}
bool IsButtonClicked(Button* p) {
	int mx = GetMouseX(), my = GetMouseY();
	return p->visible && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mx >= p->x && mx <= p->x + p->width && my >= p->y && my <= p->y + p->height;
}
void DrawButton(Button* p) {
	if (!p->visible) {
		return;
	}
	DrawRectangle(p->x, p->y, p->width, p->height, p->bgColor);
	DrawText(p->label.c_str(), p->x + p->padding, p->y + p->padding, p->fontSize, p->textColor);
}