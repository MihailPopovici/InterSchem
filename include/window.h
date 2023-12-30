#pragma once

#include <vector>
#include <raylib.h>

enum WindowElementType {
	WindowElementTypeButton,
	WindowElementTypeDictionary, // only <string, int> for now
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
	int dx, dy;
	bool dragging;
};

Window* NewWindow();
void SetWindowColor(Window* win, Color bgColor);
void SetWindowPosition(Window* win, int x, int y);
void SetWindowPadding(Window* win, float padding);
void SetWindowSpacing(Window* win, float spacing);
void AddElementToWindow(Window* win, WindowElement el);
void DrawWindow(Window* win);
bool IsWindowFocused(Window* win);
bool IsWindowHovered(Window* win);
bool IsWindowClicked(Window* win);
bool IsWindowElementClicked(Window* win);
void ResizeWindow(Window* win);
void DragWindow(Window* win);