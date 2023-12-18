#include "pin.h"

#include "raylib.h"

void DrawLink(Pin& from, Pin* to) {
	if (to == nullptr) return;
	DrawLineEx({ from.x, from.y }, { to->x, to->y }, 2.0f, GRAY);
}
void NewLink(Pin*& destination, Pin& source) {
	destination = &source;
}