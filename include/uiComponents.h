#pragma once
#include <cstring>
#include <raylib.h>
struct Button {
	char* label;
	int fontSize, padding;
	Color bgColor, textColor;
	float x, y, width, height;
	void (*func)();
};

struct Window {

};

struct Popup {

};

struct SingleLineText {

};

struct MultiLinetext {

};

Button* NewButton() {
	Button* p = new Button;
	p->label = nullptr;
	p->fontSize = -1;
	p->padding = -1;
	p->x = -1;
	p->y = -1;
	p->width = -1;
	p->height = -1;
	p->func = nullptr;
	return p;
}

void SetButtonColors(Button* p, Color bgColor, Color textColor) {
	p->bgColor = bgColor;
	p->textColor = textColor;
}

void SetButtonLabel(Button* p, const char* label, int fontSize, int padding) {
	p->label = new char[strlen(label) + 1]; //TODO:de verificat daca new face curat
	strcpy(p->label, label);
	p->fontSize = fontSize;
	p->padding = padding;
	p->width = MeasureText(label, fontSize) + 2 * padding;
	p->height = fontSize + 2 * padding;
}

void SetButtonPosition(Button* p, float x, float y) {
	p->x = x;
	p->y = y;
}

void SetButtonOnClick(Button* p, void (*func)()) {
	p->func = func;
}

bool IsButtonClicked(Button* p) {
	int mx = GetMouseX(), my = GetMouseY();
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mx >= p->x && mx <= p->x + p->width && my >= p->y && my <= p->y + p->height;
}

void DrawButton(Button* p) {
	DrawRectangle(p->x, p->y, p->width, p->height, p->bgColor);
	DrawText(p->label, p->x + p->padding, p->y + p->padding, p->fontSize, p->textColor);
}

void OnButtonClick(Button* p) {
	p->func();
}