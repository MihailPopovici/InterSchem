#include "pin.h"

#include "raylib.h"

void DrawLink(Pin& from, Pin* to) {
	if (to == nullptr) return;
	if (from.y <= to->y) {
		DrawLineEx({ from.x, from.y }, { from.x, (from.y + to->y) / 2.0f }, 2.0f, GRAY);
		DrawLineEx({ from.x, (from.y + to->y) / 2.0f }, { to->x, (from.y + to->y) / 2.0f }, 2.0f, GRAY);
		DrawLineEx({ to->x, (from.y + to->y) / 2.0f }, { to->x, to->y }, 2.0f, GRAY);

		DrawTriangle({ from.x, from.y }, { from.x - 4, from.y + 8 }, { from.x + 4, from.y + 8 }, GRAY);
		DrawTriangle({ to->x, to->y }, { to->x + 4, to->y - 8 }, { to->x - 4, to->y - 8 }, GRAY);
	}
	else {
		DrawLineEx({ from.x, from.y }, { (from.x + to->x) / 2.0f, from.y }, 2.0f, GRAY);
		DrawLineEx({ (from.x + to->x) / 2.0f, from.y }, { (from.x + to->x) / 2.0f, to->y }, 2.0f, GRAY);
		DrawLineEx({ (from.x + to->x) / 2.0f, to->y }, { to->x, to->y }, 2.0f, GRAY);

		DrawTriangle({ from.x, from.y }, { from.x + 8, from.y + 4 }, { from.x + 8, from.y - 4 }, GRAY);
		DrawTriangle({ to->x, to->y }, { to->x - 8, to->y - 4 }, { to->x - 8,to->y + 4 }, GRAY);
	}
}
void NewLink(Pin*& destination, Pin& source) {
	destination = &source;
}