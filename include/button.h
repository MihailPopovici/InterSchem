#pragma once

#include <string>
#include "raylib.h"

struct Button {
	int x, y, width, height;
	int fontSize, padding;
	bool visible;
	Color bgColor, textColor;
	std::string label;
};

Button* NewButton();
void SetButtonColors(Button* p, Color bgColor, Color textColor);
void SetButtonLabel(Button* p, std::string label, int fontSize, int padding);
void SetButtonPosition(Button* p, int x, int y);
bool IsButtonClicked(Button* p);
void DrawButton(Button* p);