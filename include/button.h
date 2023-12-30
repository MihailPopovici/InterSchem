#pragma once

#include <string>
#include "raylib.h"

struct Button {
	std::string label;
	int fontSize, padding;
	Color bgColor, textColor;
	float x, y, width, height;
};

Button* NewButton();
void SetButtonColors(Button* p, Color bgColor, Color textColor);
void SetButtonLabel(Button* p, const char* label, int fontSize, int padding);
void SetButtonPosition(Button* p, float x, float y);
bool IsButtonClicked(Button* p);
void DrawButton(Button* p);