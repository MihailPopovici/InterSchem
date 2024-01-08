#include "grid.h"

#include "button.h"

Grid* NewGrid() {
	Grid* p = new Grid{};
	return p;
}
void SetGridPosition(Grid* grid, int x, int y) {
	int dx = x - grid->x, dy = y - grid->y;
	grid->x += dx;
	grid->y += dy;
	for (auto& e : grid->elements) {
		*(int*)e += dx;
		*((int*)e + 1) += dy;
	}
}
void SetGridPadding(Grid* grid, int padding) {
	for (auto& e : grid->elements) {
		int* pos = (int*)e;
		*pos -= grid->padding - padding;
		*(pos + 1) -= grid->padding - padding;
		if (*(pos + 2) > grid->cellWidth) {
			grid->cellWidth = *(pos + 2);
		}
		if (*(pos + 3) > grid->cellHeight) {
			grid->cellHeight = *(pos + 3);
		}
	}
	grid->padding = padding;
	size_t rows = grid->elements.size() / grid->cols + (grid->elements.size() % grid->cols != 0);
	if (grid->elements.size() >= grid->cols) {
		grid->width = 2 * grid->padding + (grid->cols - 1) * grid->spacing + grid->cols * grid->cellWidth;
	}
	else if (!grid->elements.empty()) {
		grid->width = 2 * grid->padding + ((int)grid->elements.size() - 1) * grid->spacing + grid->cols * grid->cellWidth;
	}
	else {
		grid->width = 2 * grid->padding;
	}
	if (rows > 0) {
		grid->height = 2 * grid->padding + ((int)rows - 1) * grid->spacing + (int)rows * grid->cellHeight;
	}
	else {
		grid->height = 2 * grid->padding;
	}
}
void SetGridSpacing(Grid* grid, int spacing) {
	size_t i = 0, j = 0;
	size_t rows = grid->elements.size() / grid->cols + (grid->elements.size() % grid->cols != 0);
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < grid->cols && grid->cols * i + j < grid->elements.size(); j++) {
			void* ptr = grid->elements[i * grid->cols + j];
			int* pos = (int*)ptr;
			*pos = grid->x + grid->padding + (int)j * (grid->cellWidth + spacing);
			*(pos + 1) = grid->y + grid->padding + (int)i * (grid->cellHeight + spacing);
			if (*(pos + 2) > grid->cellWidth) {
				grid->cellWidth = *(pos + 2);
			}
			if (*(pos + 3) > grid->cellHeight) {
				grid->cellHeight = *(pos + 3);
			}
		}
	}
	grid->spacing = spacing;
	if (grid->elements.size() >= grid->cols) {
		grid->width = 2 * grid->padding + (grid->cols - 1) * grid->spacing + grid->cols * grid->cellWidth;
	}
	else if (!grid->elements.empty()) {
		grid->width = 2 * grid->padding + ((int)grid->elements.size() - 1) * grid->spacing + grid->cols * grid->cellWidth;
	}
	else {
		grid->width = 2 * grid->padding;
	}
	if (rows > 0) {
		grid->height = 2 * grid->padding + ((int)rows - 1) * grid->spacing + (int)rows * grid->cellHeight;
	}
	else {
		grid->height = 2 * grid->padding;
	}
}
void SetGridColor(Grid* grid, Color bgColor) {
	grid->bgColor = bgColor;
}
void SetGridSize(Grid* grid, int cols) {
	grid->cols = cols;
	size_t rows = grid->elements.size() / grid->cols + (grid->elements.size() % grid->cols != 0);
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < grid->cols && grid->cols * i + j < grid->elements.size(); j++) {
			void* ptr = grid->elements[i * grid->cols + j];
			int* pos = (int*)ptr;
			*pos = grid->x + grid->padding + (int)j * (grid->cellWidth + grid->spacing);
			*(pos + 1) = grid->y + grid->padding + (int)i * (grid->cellHeight + grid->spacing);
			if (*(pos + 2) > grid->cellWidth) {
				grid->cellWidth = *(pos + 2);
			}
			if (*(pos + 3) > grid->cellHeight) {
				grid->cellHeight = *(pos + 3);
			}
		}
	}
	if (grid->elements.size() >= grid->cols) {
		grid->width = 2 * grid->padding + (grid->cols - 1) * grid->spacing + grid->cols * grid->cellWidth;
	}
	else if (!grid->elements.empty()) {
		grid->width = 2 * grid->padding + ((int)grid->elements.size() - 1) * grid->spacing + grid->cols * grid->cellWidth;
	}
	else {
		grid->width = 2 * grid->padding;
	}
	if (rows > 0) {
		grid->height = 2 * grid->padding + ((int)rows - 1) * grid->spacing + (int)rows * grid->cellHeight;
	}
	else {
		grid->height = 2 * grid->padding;
	}
}
void SetGridVisible(Grid* grid, bool visible) {
	grid->visible = visible;
	for (auto& e : grid->elements) {
		*(bool*)((int*)e + 6) = visible;
	}
}
bool IsGridHovered(Grid* grid) {
	int mx = GetMouseX(), my = GetMouseY();
	return grid->visible && mx >= grid->x && mx <= grid->x + grid->width && my >= grid->y && my <= grid->y + grid->height;
}
bool IsGridClicked(Grid* grid) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsGridHovered(grid);
}
void GridAddElement(Grid* grid, void* e) {
	grid->elements.push_back(e);
	size_t rows = grid->elements.size() / grid->cols + (grid->elements.size() % grid->cols != 0);
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < grid->cols && grid->cols * i + j < grid->elements.size(); j++) {
			void* ptr = grid->elements[i * grid->cols + j];
			int* pos = (int*)ptr;
			*pos = grid->x + grid->padding + (int)j * (grid->cellWidth + grid->spacing);
			*(pos + 1) = grid->y + grid->padding + (int)i * (grid->cellHeight + grid->spacing);
			if (*(pos + 2) > grid->cellWidth) {
				grid->cellWidth = *(pos + 2);
			}
			if (*(pos + 3) > grid->cellHeight) {
				grid->cellHeight = *(pos + 3);
			}
		}
	}
	if (grid->elements.size() >= grid->cols) {
		grid->width = 2 * grid->padding + (grid->cols - 1) * grid->spacing + grid->cols * grid->cellWidth;
	}
	else if (!grid->elements.empty()) {
		grid->width = 2 * grid->padding + ((int)grid->elements.size() - 1) * grid->spacing + grid->cols * grid->cellWidth;
	}
	else {
		grid->width = 2 * grid->padding;
	}
	if (rows > 0) {
		grid->height = 2 * grid->padding + ((int)rows - 1) * grid->spacing + (int)rows * grid->cellHeight;
	}
	else {
		grid->height = 2 * grid->padding;
	}
}
void* GetGridClickedElement(Grid* grid) {
	if (!grid->visible) {
		return nullptr;
	}
	for (auto& e : grid->elements) {
		int* pos = (int*)e;
		int x = *pos, y = *(pos + 1), width = *(pos + 2), height = *(pos + 3);
		int mx = GetMouseX(), my = GetMouseY();
		if (mx >= x && mx <= x + width && my >= y && my <= y + height) {
			return e;
		}
	}
	return nullptr;
}
void DrawGrid(Grid* grid) {
	if (!grid->visible) {
		return;
	}
	// TODO: just buttons for now..
	DrawRectangle(grid->x, grid->y, grid->width, grid->height, grid->bgColor);
	for (auto& e : grid->elements) {
		DrawButton((Button*)e);
	}
}
void GridCleanup(Grid* grid) {
	for (auto& e : grid->elements) {
		delete (Button*)e;
	}
	grid->elements.clear();
	grid->cellWidth = grid->cellHeight = 0;
	grid->width = grid->height = 2 * grid->padding;
}
