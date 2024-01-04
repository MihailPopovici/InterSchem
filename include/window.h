#pragma once

#include <vector>
#include <raylib.h>
#include <string>

struct Button;

enum WindowElementType {
	WindowElementTypeButton,
	WindowElementTypeDictionary, // only <string, int> for now
	WindowElementTypeSingleLineText,
	WindowElementTypeMultiLineText,
};

struct WindowElement {
	void* ptr;
	WindowElementType type;
};

struct Window {
	int x, y, width, height; // TODO: int/int?
	int padding, spacing;
	std::vector<WindowElement> elements;
	Color bgColor;
	bool focused, visible;
	int dx, dy;
	bool dragging;
	std::string title;
	int fontSize;
	Color fontColor;
	Button* close;
};

Window* NewWindow();
void SetWindowTitle(Window* win, std::string title, int fontSize, Color fontColor);
void SetWindowColor(Window* win, Color bgColor);
void SetWindowPosition(Window* win, int x, int y);
void SetWindowPadding(Window* win, int padding);
void SetWindowSpacing(Window* win, int spacing);
void AddElementToWindow(Window* win, WindowElement el);
void DrawWindow(Window* win);
bool IsWindowHovered(Window* win);
bool IsWindowClicked(Window* win);
bool IsWindowElementClicked(Window* win);
void ResizeWindow(Window* win);
void UpdateWindow(Window* win);
bool WindowShouldClose(Window* win);
void WindowSetVisible(Window* win, bool visible);