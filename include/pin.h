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
	int x, y;
	int radius;
	PinType type;
	//AnyNodeType owner;
	void* ownerPtr;
	NodeType ownerType;
};

void DrawLink(Pin& from, Pin* to);
void DrawGhostLink(Pin* pin, int mx, int my);
void NewLink(Pin*& destination, Pin& source);