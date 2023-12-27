#pragma once

#include <cstring>
#include <raylib.h>
#include <string>
#include <vector>

struct Button {
	std::string label;
	int fontSize, padding;
	Color bgColor, textColor;
	float x, y, width, height;
};

enum WindowElementType {
	WindowElementTypeButton
};

struct WindowElement {
	void* ptr;
	WindowElementType type;
};

struct Window {
	int x, y, width, height; // TODO: int/float?
	float padding, spacing;
	std::vector<WindowElement> elements;
	Color bgColor;
	bool focused;
};

struct Popup {

};

struct SingleLineText {

};

struct MultiLinetext {

};

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

Window* NewWindow() {
	Window* p = new Window;
	p->x = 0;
	p->y = 0;
	p->width = 0;
	p->height = 0;
	p->padding = 0.0f;
	p->spacing = 0.0f;
	p->bgColor = WHITE;
	p->focused = false;
	return p;
}
void SetWindowColor(Window* window, Color bgColor) {
	window->bgColor = bgColor;
}
void SetWindowPosition(Window* window, int x, int y) {
	int dx = x - window->x, dy = y - window->y;
	for (WindowElement& e : window->elements) {
		if (e.type == WindowElementTypeButton) {
			Button* b = (Button*)e.ptr;
			SetButtonPosition(b, b->x + dx, b->y + dy);
		}
	}
	window->x = x;
	window->y = y;
}
void SetWindowPadding(Window* window, float padding) {
	window->width -= 2 * window->padding;
	window->width += 2 * padding;
	float height = padding;
	for (WindowElement& e : window->elements) {
		if (e.type == WindowElementTypeButton) {
			Button* b = (Button*)e.ptr;
			SetButtonPosition(b, b->x - window->padding + padding, window->y + height);
			height += b->height + window->spacing;
		}
	}
	if (!window->elements.empty()) {
		height -= window->spacing;
	}
	height += padding;
	window->height = height;
	window->padding = padding;
}
void SetWindowSpacing(Window* window, float spacing) {
	float height = window->padding;
	for (WindowElement& e : window->elements) {
		if (e.type == WindowElementTypeButton) {
			Button* b = (Button*)e.ptr;
			SetButtonPosition(b, b->x, window->y + height);
			height += b->height + spacing;
		}
	}
	if (!window->elements.empty()) {
		height -= spacing;
	}
	height += window->padding;
	window->height = height;
	window->spacing = spacing;
}
void AddElementToWindow(Window* window, WindowElement el) {
	if (el.type == WindowElementTypeButton) {
		Button* b = (Button*)el.ptr;
		if (!window->elements.empty()) {
			SetButtonPosition(b, window->x + window->padding, window->y + window->height - window->padding + window->spacing);
		}
		else {
			SetButtonPosition(b, window->x + window->padding, window->y + window->height - window->padding);
		}
		if (b->width + 2 * window->padding > window->width) {
			window->width = b->width + 2 * window->padding;
		}
		if (!window->elements.empty()) {
			window->height += window->spacing;
		}
		window->height += b->height;
	}
	window->elements.push_back(el); //TODO: validate
}
void DrawWindow(Window* window) {
	DrawRectangle(window->x, window->y, window->width, window->height, window->bgColor);
	for (WindowElement& e : window->elements) {
		if (e.type == WindowElementTypeButton) {
			DrawButton((Button*)e.ptr);
		}
	}
}
bool IsWindowFocused(Window* window) {
	return window->focused;
}
bool IsWindowHovered(Window* window) {
	int mx = GetMouseX(), my = GetMouseY();
	return mx >= window->x && mx <= window->x + window->width && my >= window->y && my <= window->y + window->height;
}
bool IsWindowClicked(Window* window) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsWindowHovered(window);
}