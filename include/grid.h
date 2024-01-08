#pragma once

#include "raylib.h"
#include <vector>

struct Window;

struct Grid {
	int x, y, width, height;
	int padding, spacing;
	bool visible;
	Color bgColor;
	int cols;
	int cellWidth, cellHeight;
	std::vector<void*> elements; // TODO: should receive different types of elements
	Window* window;
};

Grid* NewGrid();
void SetGridPosition(Grid* grid, int x, int y);
void SetGridPadding(Grid* grid, int padding);
void SetGridSpacing(Grid* grid, int spacing);
void SetGridColor(Grid* grid, Color bgColor);
void SetGridSize(Grid* grid, int cols);
void SetGridVisible(Grid* grid, bool visible);
bool IsGridHovered(Grid* grid);
bool IsGridClicked(Grid* grid);
void GridAddElement(Grid* grid, void* e);
void* GetGridClickedElement(Grid* grid);
void DrawGrid(Grid* grid);
void GridCleanup(Grid* grid);
