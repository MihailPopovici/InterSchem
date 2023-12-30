#include "window.h"

#include "button.h"
#include "dictionary.h"

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
	p->dx = 0;
	p->dy = 0;
	p->dragging = false;
	return p;
}
void SetWindowColor(Window* win, Color bgColor) {
	win->bgColor = bgColor;
}
void SetWindowPosition(Window* win, int x, int y) {
	int dx = x - win->x, dy = y - win->y;
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementTypeButton) {
			Button* b = (Button*)e.ptr;
			SetButtonPosition(b, b->x + dx, b->y + dy);
		}
		else if (e.type = WindowElementTypeDictionary) {
			Dictionary* d = (Dictionary*)e.ptr;
			SetDictionaryPosition(d, d->x + dx, d->y + dy);
		}
	}
	win->x = x;
	win->y = y;
}
void SetWindowPadding(Window* win, float padding) {
	win->width -= 2 * win->padding;
	win->width += 2 * padding;
	float height = padding;
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementTypeButton) {
			Button* b = (Button*)e.ptr;
			SetButtonPosition(b, b->x - win->padding + padding, win->y + height);
			height += b->height + win->spacing;
		}
		else if (e.type == WindowElementTypeDictionary) {
			Dictionary* d = (Dictionary*)e.ptr;
			SetDictionaryPosition(d, d->x - win->padding + padding, win->y + height);
			height += d->height + win->spacing;
		}
	}
	if (!win->elements.empty()) {
		height -= win->spacing;
	}
	height += padding;
	win->height = height;
	win->padding = padding;
}
void SetWindowSpacing(Window* win, float spacing) {
	float height = win->padding;
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementTypeButton) {
			Button* b = (Button*)e.ptr;
			SetButtonPosition(b, b->x, win->y + height);
			height += b->height + spacing;
		}
		else if (e.type == WindowElementTypeDictionary) {
			Dictionary* d = (Dictionary*)e.ptr;
			SetDictionaryPosition(d, d->x, win->y + height);
			height += d->height + spacing;
		}
	}
	if (!win->elements.empty()) {
		height -= spacing;
	}
	height += win->padding;
	win->height = height;
	win->spacing = spacing;
}
void AddElementToWindow(Window* win, WindowElement el) {
	if (el.type == WindowElementTypeButton) {
		Button* b = (Button*)el.ptr;
		if (!win->elements.empty()) {
			SetButtonPosition(b, win->x + win->padding, win->y + win->height - win->padding + win->spacing);
		}
		else {
			SetButtonPosition(b, win->x + win->padding, win->y + win->height - win->padding);
		}
		if (b->width + 2 * win->padding > win->width) {
			win->width = b->width + 2 * win->padding;
		}
		if (!win->elements.empty()) {
			win->height += win->spacing;
		}
		win->height += b->height;
	}
	else if (el.type == WindowElementTypeDictionary) {
		Dictionary* d = (Dictionary*)el.ptr;
		d->window = win;
		if (!win->elements.empty()) {
			SetDictionaryPosition(d, win->x + win->padding, win->y + win->height - win->padding + win->spacing);
		}
		else {
			SetDictionaryPosition(d, win->x + win->padding, win->y + win->height - win->padding);
		}
		if (d->width + 2 * win->padding > win->width) {
			win->width = d->width + 2 * win->padding;
		}
		if (!win->elements.empty()) {
			win->height += win->spacing;
		}
		win->height += d->height;
	}
	win->elements.push_back(el); //TODO: validate
}
void DrawWindow(Window* win) {
	DrawRectangle(win->x, win->y, win->width, win->height, win->bgColor);
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementTypeButton) {
			DrawButton((Button*)e.ptr);
		}
		else if (e.type == WindowElementTypeDictionary) {
			DrawDictionary((Dictionary*)e.ptr);
		}
	}
}
bool IsWindowFocused(Window* win) {
	return win->focused;
}
bool IsWindowHovered(Window* win) {
	int mx = GetMouseX(), my = GetMouseY();
	return mx >= win->x && mx <= win->x + win->width && my >= win->y && my <= win->y + win->height;
}
bool IsWindowClicked(Window* win) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsWindowHovered(win);
}
bool IsWindowElementClicked(Window* win) {
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementTypeButton) {
			if (IsButtonClicked((Button*)e.ptr)) {
				return true;
			}
		}
		else if (e.type == WindowElementTypeDictionary) {
			if (IsDictionaryClicked((Dictionary*)e.ptr)) {
				return true;
			}
		}
	}
	return false;
}
void ResizeWindow(Window* win) {
	int height = win->padding;
	int maxWidth = 0;
	for (WindowElement& el : win->elements) {
		if (el.type == WindowElementTypeButton) {
			Button* b = (Button*)el.ptr;
			SetButtonPosition(b, b->x, win->y + height);
			height += b->height;
			if (b->width > maxWidth) {
				maxWidth = b->width;
			}
		}
		else if (el.type == WindowElementTypeDictionary) {
			Dictionary* d = (Dictionary*)el.ptr;
			SetDictionaryPosition(d, d->x, win->y + height);
			height += d->height;
			if (d->width > maxWidth) {
				maxWidth = d->width;
			}
		}
		height += win->spacing;
	}
	height += win->padding;
	height -= win->spacing;
	win->height = height;
	win->width = maxWidth + 2 * win->padding;
}
void DragWindow(Window* win) {
	int mx = GetMouseX(), my = GetMouseY();
	if (IsWindowClicked(win) && !IsWindowElementClicked(win)) {
		win->dx = mx - win->x;
		win->dy = my - win->y;
		win->dragging = true;
	}
	else if (win->dragging && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		SetWindowPosition(win, mx - win->dx, my - win->dy);
	}
	else if (win->dragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		win->dragging = false;
	}
}