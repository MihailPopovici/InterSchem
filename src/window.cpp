#include "window.h"

#include "button.h"
#include "dictionary.h"
#include "multilinetext.h"
#include "grid.h"

Window* NewWindow() {
	Window* p = new Window{};
	p->visible = true;
	p->close = NewButton();
	SetButtonColors(p->close, { 255, 0, 0, 100 } , BLANK);
	SetButtonLabel(p->close, "", 0, 8);
	SetButtonPosition(p->close, p->x + p->width - p->close->width, p->y);
	return p;
}
void SetWindowTitle(Window* win, std::string title, int fontSize, Color fontColor) {
	win->title = title;
	win->fontSize = fontSize;
	win->fontColor = fontColor;
	if (MeasureText(title.c_str(), fontSize) + 2 * win->padding > win->width) {
		win->width = MeasureText(title.c_str(), fontSize) + 2 * win->padding;
	}
	win->height = win->close->height + fontSize + win->padding + win->spacing;
	// TODO: resize and etc
}
void SetWindowColor(Window* win, Color bgColor) {
	win->bgColor = bgColor;
}
void SetWindowPosition(Window* win, int x, int y) {
	int dx = x - win->x, dy = y - win->y;
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementType_Button) {
			Button* b = (Button*)e.ptr;
			SetButtonPosition(b, b->x + dx, b->y + dy);
		}
		else if (e.type == WindowElementType_Dictionary) {
			Dictionary* d = (Dictionary*)e.ptr;
			SetDictionaryPosition(d, d->x + dx, d->y + dy);
		}
		else if (e.type == WindowElementType_MultiLineText) {
			MultiLineText* m = (MultiLineText*)e.ptr;
			MultiLineTextSetPosition(m, m->x + dx, m->y + dy);
		}
		else if (e.type == WindowElementType_Grid) {
			Grid* g = (Grid*)e.ptr;
			SetGridPosition(g, g->x + dx, g->y + dy);
		}
	}
	SetButtonPosition(win->close, win->close->x + dx, win->close->y + dy);
	win->x = x;
	win->y = y;
}
void SetWindowPadding(Window* win, int padding) {
	win->width -= 2 * win->padding;
	win->width += 2 * padding;
	int height = padding;
	if (!win->title.empty()) {
		height += win->fontSize + win->spacing;
	}
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementType_Button) {
			Button* b = (Button*)e.ptr;
			SetButtonPosition(b, b->x - win->padding + padding, win->y + height);
			height += b->height + win->spacing;
		}
		else if (e.type == WindowElementType_Dictionary) {
			Dictionary* d = (Dictionary*)e.ptr;
			SetDictionaryPosition(d, d->x - win->padding + padding, win->y + height);
			height += d->height + win->spacing;
		}
		else if (e.type == WindowElementType_MultiLineText) {
			MultiLineText* m = (MultiLineText*)e.ptr;
			MultiLineTextSetPosition(m, m->x - win->padding + padding, win->y + height);
			height += m->height + win->spacing;
		}
		else if (e.type == WindowElementType_Grid) {
			Grid* g = (Grid*)e.ptr;
			SetGridPosition(g, g->x - win->padding + padding, win->y + padding);
			height += g->height + win->spacing;
		}
	}
	if (!win->elements.empty()) {
		height -= win->spacing;
	}
	height += padding;
	win->height = height;
	win->padding = padding;
	SetButtonPosition(win->close, win->x + win->width - win->close->width, win->y);
}
void SetWindowSpacing(Window* win, int spacing) {
	int height = win->padding;
	if (!win->title.empty()) {
		height += win->fontSize + spacing;
	}
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementType_Button) {
			Button* b = (Button*)e.ptr;
			SetButtonPosition(b, b->x, win->y + height);
			height += b->height + spacing;
		}
		else if (e.type == WindowElementType_Dictionary) {
			Dictionary* d = (Dictionary*)e.ptr;
			SetDictionaryPosition(d, d->x, win->y + height);
			height += d->height + spacing;
		}
		else if (e.type == WindowElementType_MultiLineText) {
			MultiLineText* m = (MultiLineText*)e.ptr;
			MultiLineTextSetPosition(m, m->x, win->y + height);
			height += m->height + spacing;
		}
		else if (e.type == WindowElementType_Grid) {
			Grid* g = (Grid*)e.ptr;
			SetGridPosition(g, g->x, win->y + height);
			height += g->height + spacing;
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
	if (el.type == WindowElementType_Button) {
		Button* b = (Button*)el.ptr;
		if (!win->elements.empty()) {
			SetButtonPosition(b, win->x + win->padding, win->y + win->height - win->padding + win->spacing);
		}
		else {
			SetButtonPosition(b, win->x + win->padding, win->y + win->height - win->padding) ;
		}
		if (b->width + 2 * win->padding > win->width) {
			win->width = b->width + 2 * win->padding;
		}
		if (!win->elements.empty()) {
			win->height += win->spacing;
		}
		win->height += b->height;
	}
	else if (el.type == WindowElementType_Dictionary) {
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
	else if (el.type == WindowElementType_MultiLineText) {
		MultiLineText* m = (MultiLineText*)el.ptr;
		m->window = win;
		if (!win->elements.empty()) {
			MultiLineTextSetPosition(m, win->x + win->padding, win->y + win->height - win->padding + win->spacing);
		}
		else {
			MultiLineTextSetPosition(m, win->x + win->padding, win->y + win->height - win->padding);
		}
		if (m->width + 2 * win->padding > win->width) {
			win->width = m->width + 2 * win->padding;
		}
		if (!win->elements.empty()) {
			win->height += win->spacing;
		}
		win->height += m->height;
	}
	else if (el.type == WindowElementType_Grid) {
		Grid* g = (Grid*)el.ptr;
		g->window = win;
		if (!win->elements.empty()) {
			SetGridPosition(g, win->x + win->padding, win->y + win->height - win->padding + win->spacing);
		}
		else {
			SetGridPosition(g, win->x + win->padding, win->y + win->height - win->padding);
		}
		if (g->width + 2 * win->padding > win->width) {
			win->width = g->width + 2 * win->padding;
		}
		if (!win->elements.empty()) {
			win->height += win->spacing;
		}
		win->height += g->height;
	}
	win->elements.push_back(el); //TODO: validate
	SetButtonPosition(win->close, win->x + win->width - win->close->width, win->y);
}
void DrawWindow(Window* win) {
	if (!win->visible) {
		return;
	}
	DrawRectangle(win->x, win->y, win->width, win->height, win->bgColor);
	int titleWidth = MeasureText(win->title.c_str(), win->fontSize);
	DrawButton(win->close);
	DrawText(win->title.c_str(), win->x + win->padding + (win->width - 2 * win->padding - titleWidth) / 2, win->y + win->padding + win->close->height, win->fontSize, win->fontColor);
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementType_Button) {
			DrawButton((Button*)e.ptr);
		}
		else if (e.type == WindowElementType_Dictionary) {
			DrawDictionary((Dictionary*)e.ptr);
		}
		else if (e.type == WindowElementType_MultiLineText) {
			MultiLineTextDraw((MultiLineText*)e.ptr);
		}
		else if (e.type == WindowElementType_Grid) {
			DrawGrid((Grid*)e.ptr);
		}
	}
}
bool IsWindowHovered(Window* win) {
	int mx = GetMouseX(), my = GetMouseY();
	return win->visible && mx >= win->x && mx <= win->x + win->width && my >= win->y && my <= win->y + win->height;
}
bool IsWindowClicked(Window* win) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsWindowHovered(win);
}
bool IsWindowElementClicked(Window* win) {
	if (!win->visible) {
		return false;
	}
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementType_Button) {
			if (IsButtonClicked((Button*)e.ptr)) {
				return true;
			}
		}
		else if (e.type == WindowElementType_Dictionary) {
			if (IsDictionaryClicked((Dictionary*)e.ptr)) {
				return true;
			}
		}
		else if (e.type == WindowElementType_MultiLineText) {
			if (MultiLineTextIsClicked((MultiLineText*)e.ptr)) {
				return true;
			}
		}
		else if (e.type == WindowElementType_Grid) {
			if (IsGridClicked((Grid*)e.ptr)) {
				return true;
			}
		}
	}
	return false;
}
void ResizeWindow(Window* win) {
	int height = win->padding + win->close->height;
	if (!win->title.empty()) {
		height += win->fontSize + win->spacing;
	}
	int maxWidth = MeasureText(win->title.c_str(), win->fontSize);
	for (WindowElement& el : win->elements) {
		if (el.type == WindowElementType_Button) {
			Button* b = (Button*)el.ptr;
			SetButtonPosition(b, b->x, win->y + height);
			height += b->height;
			if (b->width > maxWidth) {
				maxWidth = b->width;
			}
		}
		else if (el.type == WindowElementType_Dictionary) {
			Dictionary* d = (Dictionary*)el.ptr;
			SetDictionaryPosition(d, d->x, win->y + height);
			height += d->height;
			if (d->width > maxWidth) {
				maxWidth = d->width;
			}
		}
		else if (el.type == WindowElementType_MultiLineText) {
			MultiLineText* m = (MultiLineText*)el.ptr;
			MultiLineTextSetPosition(m, m->x, win->y + height);
			height += m->height;
			if (m->width > maxWidth) {
				maxWidth = m->width;
			}
		}
		else if (el.type == WindowElementType_Grid) {
			Grid* g = (Grid*)el.ptr;
			SetGridPosition(g, g->x, win->y + height);
			height += g->height;
			if (g->width > maxWidth) {
				maxWidth = g->width;
			}
		}
		height += win->spacing;
	}
	height += win->padding;
	height -= win->spacing;
	win->height = height;
	win->width = maxWidth + 2 * win->padding;
	SetButtonPosition(win->close, win->x + win->width - win->close->width, win->y);
}
void UpdateWindow(Window* win) {
	if (!win->visible) {
		return;
	}
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
	if (IsButtonClicked(win->close)) {
		win->dragging = false;
		win->dx = win->dy = 0;
		WindowSetVisible(win, false);
	}
}
bool WindowShouldClose(Window* win) {
	return !win->visible;
}
void WindowSetVisible(Window* win, bool visible) {
	win->visible = visible;
	win->close->visible = visible;
	for (auto& e : win->elements) {
		if (e.type == WindowElementType_Grid) {
			SetGridVisible((Grid*)e.ptr, visible);
		}
		int* i = (int*)e.ptr;
		i += 6;
		bool* b = (bool*)i;
		*b = visible;
		//*((bool*)((int*)e.ptr) + 6) = visible;
	}
}
