#pragma once

#include "raylib.h"
#include <string>
#include <vector>

struct Window;

enum DictionaryRowHalfType {
	DictionaryRowHalfType_None,
	DictionaryRowHalfType_Key,
	DictionaryRowHalfType_Value
};

struct DictionaryRowHalf {
	DictionaryRowHalfType type;
	void* data;
};

struct Dictionary; // TODO: ew

struct DictionaryRow {
	Dictionary* dict;
	std::string key;
	int value;
	int x, y, width, height;
	int fontSize, padding;
	Color bgColor, fontColor;
};

struct Dictionary {
	int x, y, width, height;
	int padding, spacing;
	bool visible;
	Color bgColor;
	Window* window;
	std::vector<DictionaryRow*> rows; //TODO: or list, or map
};

DictionaryRow* NewDictionaryRow();
void SetDictionaryRowData(DictionaryRow* drow, std::string key, int value, int fontSize, int padding);
void SetDictionaryRowPosition(DictionaryRow* drow, int x, int y);
void SetDictionaryRowColors(DictionaryRow* drow, Color bgColor, Color fontColor);
bool IsDictionaryRowHovered(DictionaryRow* drow);
bool IsDictionaryRowClicked(DictionaryRow* drow);
DictionaryRowHalf GetClickedDictionaryRowHalf(DictionaryRow* drow);
void ResizeDictionaryRow(DictionaryRow* drow);
void SetDictionaryRowKey(DictionaryRow* drow, std::string key);
void SetDictionaryRowValue(DictionaryRow* drow, int value);
void DrawDictionaryRow(DictionaryRow* drow);

Dictionary* NewDictionary();
void SetDictionaryPosition(Dictionary* dict, int x, int y);
int AddDictionaryRow(Dictionary* dict, DictionaryRow* drow);
int FindKeyInDictionary(Dictionary* dict, std::string key);
int GetValueFromDictionary(Dictionary* dict, int index);
int GetValueFromDictionary(Dictionary* dict, std::string key);
bool IsDictionaryHovered(Dictionary* dict);
bool IsDictionaryClicked(Dictionary* dict);
bool IsDictionaryRowClicked(Dictionary* dict);
DictionaryRow* GetClickedDictionaryRow(Dictionary* dict);
DictionaryRow* GetDictionaryRow(Dictionary* dict, std::string key);
void SetDictionaryPadding(Dictionary* dict, int padding);
void SetDictionarySpacing(Dictionary* dict, int spacing);
void SetDictionaryColor(Dictionary* dict, Color bgColor);
void ReorderDictionary(Dictionary* dict, int index);
void ResizeDictionary(Dictionary* dict);
void DrawDictionary(Dictionary* dict);