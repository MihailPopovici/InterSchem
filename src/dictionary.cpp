#include "dictionary.h"

#include "window.h"

DictionaryRow* NewDictionaryRow() {
	DictionaryRow* p = new DictionaryRow;
	p->key = "";
	p->value = 0;
	p->x = 0;
	p->y = 0;
	p->width = 0;
	p->height = 0;
	p->padding = 0;
	p->bgColor = BLACK;
	p->fontColor = WHITE;
	return p;
}
void SetDictionaryRowData(DictionaryRow* drow, std::string key, int value, int fontSize, int padding) {
	drow->dict = nullptr;
	drow->key = key;
	drow->value = value;
	drow->fontSize = fontSize;
	drow->padding = padding;
	std::string strval = std::to_string(value);
	drow->width = 2 * padding + 10 + MeasureText(key.c_str(), fontSize) + MeasureText(strval.c_str(), fontSize);
	drow->height = 2 * padding + fontSize;
}
void SetDictionaryRowPosition(DictionaryRow* drow, int x, int y) {
	drow->x = x;
	drow->y = y;
}
void SetDictionaryRowColors(DictionaryRow* drow, Color bgColor, Color fontColor) {
	drow->bgColor = bgColor;
	drow->fontColor = fontColor;
}
bool IsDictionaryRowHovered(DictionaryRow* drow) {
	int mx = GetMouseX(), my = GetMouseY();
	return mx >= drow->x && mx <= drow->x + drow->width && my >= drow->y && my <= drow->y + drow->height;
}
bool IsDictionaryRowClicked(DictionaryRow* drow) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsDictionaryRowHovered(drow);
}
DictionaryRowHalf GetClickedDictionaryRowHalf(DictionaryRow* drow) {
	if (drow == nullptr) {
		return { DictionaryRowHalfType_None, nullptr };
	}

	int mx = GetMouseX(), my = GetMouseY();
	int kx = drow->x, ky = drow->y;
	int kwidth = drow->padding + MeasureText(drow->key.c_str(), drow->fontSize) + 10 / 2, kheight = drow->y + 2 * drow->padding;
	if (mx >= kx && mx <= kx + kwidth && my >= ky && my <= ky + kheight) {
		return { DictionaryRowHalfType_Key, &drow->key };
	}
	else {
		return { DictionaryRowHalfType_Value, &drow->value };
	}
	/*int vx = kx + kwidth, vy = ky;
	int vwidth = drow->padding + MeasureText(std::to_string(drow->value).c_str(), drow->fontSize) + 10 / 2, vheight = kheight;
	if (mx >= vx && mx <= vx + vwidth && my >= vy && my <= vy + vheight) {
		return { DictionaryRowHalfType_Value, &drow->value };
	}*/
}
void ResizeDictionaryRow(DictionaryRow* drow) {
	drow->width = 2 * drow->padding + MeasureText(drow->key.c_str(), drow->fontSize) + MeasureText(std::to_string(drow->value).c_str(), drow->fontSize) + 10;
	ResizeDictionary(drow->dict);
}
void SetDictionaryRowKey(DictionaryRow* drow, std::string key) {
	int oldKeyWidth = MeasureText(drow->key.c_str(), drow->fontSize);
	int newKeyWidth = MeasureText(key.c_str(), drow->fontSize);
	drow->width = drow->width - oldKeyWidth + newKeyWidth;
	int index = FindKeyInDictionary(drow->dict, drow->key);
	drow->key = key;
	ReorderDictionary(drow->dict, index);
	ResizeDictionary(drow->dict);
}
void SetDictionaryRowValue(DictionaryRow* drow, int value) {
	int oldValWidth = MeasureText(std::to_string(drow->value).c_str(), drow->fontSize);
	int newValWidth = MeasureText(std::to_string(value).c_str(), drow->fontSize);
	drow->width = drow->width - oldValWidth + newValWidth;
	drow->value = value;
	ResizeDictionary(drow->dict);
}
void DrawDictionaryRow(DictionaryRow* drow) {
	std::string strval = std::to_string(drow->value);
	int keyWidth = MeasureText(drow->key.c_str(), drow->fontSize);
	DrawRectangle(drow->x, drow->y, drow->width, drow->height, drow->bgColor);
	DrawText(drow->key.c_str(), drow->x + drow->padding, drow->y + drow->padding, drow->fontSize, drow->fontColor);
	DrawText(strval.c_str(), drow->x + drow->padding + keyWidth + 10, drow->y + drow->padding, drow->fontSize, drow->fontColor);
}

Dictionary* NewDictionary() {
	Dictionary* p = new Dictionary;
	p->x = 0;
	p->y = 0;
	p->width = 0;
	p->height = 0;
	p->padding = 0;
	p->spacing = 0;
	return p;
}
void SetDictionaryPosition(Dictionary* dict, int x, int y) {
	int dx = x - dict->x, dy = y - dict->y;
	dict->x = x;
	dict->y = y;
	for (DictionaryRow* r : dict->rows) {
		SetDictionaryRowPosition(r, r->x + dx, r->y + dy);
	}
}
int AddDictionaryRow(Dictionary* dict, DictionaryRow* drow) {
	drow->dict = dict;
	if (dict->rows.empty()) {
		dict->height += drow->height;
		SetDictionaryRowPosition(drow, dict->x + dict->padding, dict->y + dict->padding);
		dict->rows.push_back(drow);
		return 0;
	}

	size_t n = dict->rows.size();
	int left = 0, right = n - 1;
	int pos = 0;
	while (left <= right) {
		int middle = (left + right) / 2;
		pos = middle;
		if (drow->key.compare(dict->rows[middle]->key) < 0) {
			right = middle - 1;
		}
		else if (drow->key.compare(dict->rows[middle]->key) > 0) {
			left = middle + 1;
		}
		else {
			return -1; // TODO: throw error or smth, can t have two identical keys
			/*
			or replace the values
			delete dict->data[middle];
			dict->data[middle] = drow;
			*/
		}
	}
	// 1 4 5 6 8 10
	// 0 1 2 3 4  5
	// l   m      r
	//       l m  r
	//            b
	if (drow->key.compare(dict->rows[pos]->key) < 0) {
		SetDictionaryRowPosition(drow, dict->rows[pos]->x, dict->rows[pos]->y);
		for (int i = pos; i < n; i++) {
			SetDictionaryRowPosition(dict->rows[i], dict->rows[i]->x, dict->rows[i]->y + drow->height + dict->spacing);
		}
		dict->rows.insert(dict->rows.begin() + pos, drow);
	}
	else {
		for (int i = pos + 1; i < n; i++) {
			SetDictionaryRowPosition(dict->rows[i], dict->rows[i]->x, dict->rows[i]->y + drow->height + dict->spacing);
		}
		SetDictionaryRowPosition(drow, dict->x + dict->padding, dict->y + dict->height - dict->padding + dict->spacing);
		dict->rows.insert(dict->rows.begin() + pos + 1, drow);
	}
	if (drow->width + 2 * dict->padding > dict->width) {
		dict->width = drow->width + 2 * dict->padding;
	}
	dict->height += drow->height + dict->spacing;
	return pos;
}
int FindKeyInDictionary(Dictionary* dict, std::string key) {
	size_t n = dict->rows.size();
	int left = 0, right = n - 1;
	while (left <= right) {
		int middle = (left + right) / 2;
		if (key.compare(dict->rows[middle]->key) < 0) {
			right = middle - 1;
		}
		else if (key.compare(dict->rows[middle]->key) > 0) {
			left = middle + 1;
		}
		else {
			return middle;
		}
	}
	return -1;
}
int GetValueFromDictionary(Dictionary* dict, int index) {
	if (index != -1) {
		return dict->rows[index]->value;
	}
	return -1;
}
int GetValueFromDictionary(Dictionary* dict, std::string key){
	size_t n = dict->rows.size();
	int left = 0, right = n - 1;
	while (left <= right) {
		int middle = (left + right) / 2;
		if (key.compare(dict->rows[middle]->key) < 0) {
			right = middle - 1;
		}
		else if (key.compare(dict->rows[middle]->key) > 0) {
			left = middle + 1;
		}
		else {
			return dict->rows[middle]->value;
		}
	}
	//TODO:trebuie sa existe variabila in dictionar
	return 0;
}
bool IsDictionaryHovered(Dictionary* dict) {
	int mx = GetMouseX(), my = GetMouseY();
	return mx >= dict->x && mx <= dict->x + dict->width && my >= dict->y && my <= dict->y + dict->height;
}
bool IsDictionaryClicked(Dictionary* dict) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsDictionaryHovered(dict);
}
bool IsDictionaryRowClicked(Dictionary* dict) {
	for (DictionaryRow* r : dict->rows) {
		if (IsDictionaryRowClicked(r)) {
			return true;
		}
	}
	return false;
}
DictionaryRow* GetClickedDictionaryRow(Dictionary* dict) {
	for (DictionaryRow* r : dict->rows) {
		if (IsDictionaryRowClicked(r)) {
			return r;
		}
	}
	return nullptr;
}
DictionaryRow* GetDictionaryRow(Dictionary* dict, std::string key) {
	size_t n = dict->rows.size();
	int left = 0, right = n - 1;
	while (left <= right) {
		int middle = (left + right) / 2;
		if (key.compare(dict->rows[middle]->key) < 0) {
			right = middle - 1;
		}
		else if (key.compare(dict->rows[middle]->key) > 0) {
			left = middle + 1;
		}
		else {
			return dict->rows[middle];
		}
	}
	return nullptr;
}
void SetDictionaryPadding(Dictionary* dict, int padding) {
	dict->width -= 2 * dict->padding;
	dict->width += 2 * padding;
	int height = padding;
	for (DictionaryRow* r : dict->rows) {
		SetDictionaryRowPosition(r, r->x - dict->padding + padding, dict->y + height);
		height += r->height + dict->spacing;
	}
	if (!dict->rows.empty()) {
		height -= dict->spacing;
	}
	height += padding;
	dict->height = height;
	dict->padding = padding;
}
void SetDictionarySpacing(Dictionary* dict, int spacing) {
	float height = dict->padding;
	for (DictionaryRow* r : dict->rows) {
		SetDictionaryRowPosition(r, r->x, dict->y + height);
		height += r->height + spacing;
	}
	if (!dict->rows.empty()) {
		height -= spacing;
	}
	height += dict->padding;
	dict->height = height;
	dict->spacing = spacing;
}
void ReorderDictionary(Dictionary* dict, int index) {
	size_t n = dict->rows.size();
	while (index - 1 >= 0 && dict->rows[index]->key.compare(dict->rows[index - 1]->key) < 0) {
		std::swap(dict->rows[index], dict->rows[index - 1]);
		dict->rows[index - 1]->y = dict->rows[index]->y;
		dict->rows[index]->y = dict->rows[index - 1]->y + dict->rows[index - 1]->height + dict->spacing;
		index--;
	}
	while (index + 1 < n && dict->rows[index]->key.compare(dict->rows[index + 1]->key) > 0) {
		std::swap(dict->rows[index], dict->rows[index + 1]);
		dict->rows[index]->y = dict->rows[index + 1]->y;
		dict->rows[index + 1]->y = dict->rows[index]->y + dict->rows[index]->height + dict->spacing;
		index++;
	}
}
void ResizeDictionary(Dictionary* dict) {
	int maxWidth = 0;
	for (DictionaryRow* r : dict->rows) {
		if (r->width > maxWidth) {
			maxWidth = r->width;
		}
	}
	dict->width = 2 * dict->padding + maxWidth;
	ResizeWindow(dict->window);
}
void DrawDictionary(Dictionary* dict) {
	DrawRectangle(dict->x, dict->y, dict->width, dict->height, RAYWHITE); // TODO:
	for (DictionaryRow* r : dict->rows) {
		DrawDictionaryRow(r);
	}
}