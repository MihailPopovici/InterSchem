#pragma once

#ifndef PIN_RADIUS
#define PIN_RADIUS 4.0f
#endif // !PIN_RADIUS

#include "nodeGeneral.h"

enum PinType {
	input, 
	output
};

struct Pin {
	int id; // TODO: int or unsigned?
	PinType type;
	float x, y;
	float radius;

	void* owner;
	NodeType ownerType;
};

void DrawLink(Pin& from, Pin* to);
void NewLink(Pin*& destination, Pin& source);