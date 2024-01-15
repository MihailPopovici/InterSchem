#include "multilinetext.h"

#include "raylib.h"
#include <iostream>

void MultiLineTextMergeLines(MultiLineText* mtext, size_t destination, size_t source) {
	mtext->text[destination] += mtext->text[source];
	mtext->text.erase(mtext->text.begin() + source);
}
void MultiLineTextToPreviousLine(MultiLineText* mtext) {
	if (mtext->lin > 0) {
		mtext->lin--;
		if (mtext->lin < mtext->firstLin) {
			mtext->firstLin = mtext->lin;
		}
		if (mtext->col > mtext->text[mtext->lin].size()) {
			mtext->col = mtext->text[mtext->lin].size();
			if (mtext->col < mtext->firstCol) {
				mtext->firstCol = mtext->col;
			}
		}
	}
}
void MultiLineTextToNextLine(MultiLineText* mtext) {
	if (mtext->lin < mtext->text.size() - 1) {
		mtext->lin++;
		if (mtext->lin >= mtext->firstLin + mtext->visLin) {
			mtext->firstLin++;
		}
		if (mtext->col > mtext->text[mtext->lin].size()) {
			mtext->col = mtext->text[mtext->lin].size();
			if (mtext->col < mtext->firstCol) {
				mtext->firstCol = mtext->col;
			}
		}
	}
}
void MultiLineTextToPreviousColumn(MultiLineText* mtext) {
	if (mtext->col > 0) {
		mtext->col--;
		if (mtext->col < mtext->firstCol) {
			mtext->firstCol = mtext->col;
		}
	}
	else if (mtext->lin > 0) {
		mtext->lin--;
		if (mtext->lin < mtext->firstLin) {
			mtext->firstLin = mtext->lin;
		}
		mtext->col = mtext->text[mtext->lin].size();
		if (mtext->col >= mtext->firstCol + mtext->visCol) {
			mtext->firstCol = mtext->col - mtext->visCol + 1;
		}
	}
}
void MultiLineTextToNextColumn(MultiLineText* mtext) {
	if (mtext->col < mtext->text[mtext->lin].size()) {
		mtext->col++;
		if (mtext->col >= mtext->firstCol + mtext->visCol) {
			mtext->firstCol++;
		}
	}
	else if (mtext->lin < mtext->text.size() - 1) {
		mtext->col = 0;
		mtext->firstCol = 0;
		mtext->lin++;
		if (mtext->lin >= mtext->firstLin + mtext->visLin) {
			mtext->firstLin++;
		}
	}
}
void MultiLineTextEnter(MultiLineText* mtext) {
	if (mtext->lin < mtext->limLin || (mtext->lin == mtext->limLin && mtext->col < mtext->limCol)) {
		return;
	}
	mtext->lin++;
	if (mtext->lin >= mtext->firstLin + mtext->visLin) {
		mtext->firstLin++;
	}
	MultiLineTextInsertLine(mtext, mtext->lin, mtext->text[mtext->lin - 1].substr(mtext->col));
	mtext->text[mtext->lin - 1].erase(mtext->col);
	mtext->col = 0;
	mtext->firstCol = 0;
}
void MultiLineTextBackspace(MultiLineText* mtext) {
	if (mtext->lin < mtext->limLin || (mtext->lin == mtext->limLin && mtext->col <= mtext->limCol)) {
		return;
	}
	if (mtext->col > 0) {
		mtext->col--;
		mtext->text[mtext->lin].erase(mtext->col, 1);
		if (mtext->col < mtext->firstCol) {
			mtext->firstCol--;
		}
	}
	else if (mtext->lin > 0) {
		mtext->lin--;
		if (mtext->lin < mtext->firstLin) {
			mtext->firstLin = mtext->lin;
		}
		mtext->col = mtext->text[mtext->lin].size();
		if (mtext->col >= mtext->firstCol + mtext->visCol) {
			mtext->firstCol = mtext->col - 1;
		}
		MultiLineTextMergeLines(mtext, mtext->lin, mtext->lin + 1);
	}
}
void MultiLineTextPaste(MultiLineText* mtext) {
	std::string clipboard = GetClipboardText();
	size_t n = clipboard.size();
	if (n == 0) return;

	std::vector<std::string> temp = MultiLineTextParseText(mtext, clipboard);
	mtext->lin = mtext->lin + temp.size() - 1;
	mtext->col = temp[temp.size() - 1].size();

	mtext->text = temp;
}
void MultiLineTextCopy(MultiLineText* mtext) {
	size_t sl = 0, sc = 0, el = mtext->text.size() - 1, ec = mtext->text[mtext->text.size() - 1].size(); // TODO: might break, would add size - 1

	if (sl == el) {
		SetClipboardText(mtext->text[sl].substr(sc, ec - sc).c_str());
	}
	else {
		std::string res;
		res += mtext->text[sl].substr(sc);
		res += "\r\n";
		for (size_t i = sl + 1; i < el; i++) {
			res += mtext->text[i];
			res += "\r\n";
		}
		res += mtext->text[el].substr(0, ec);
		if (mtext->text[el].size() == ec) {
			res += "\r\n";
		}
		SetClipboardText(res.c_str());
	}
}
MultiLineText* NewMultiLineText(int startX, int startY, size_t visibleLines, size_t visibleColumns, Font font, int fontSize, int padding, Color textColor, Color bgColor) {
	MultiLineText* mtext = new MultiLineText;
	mtext->visLin = visibleLines;
	mtext->visCol = visibleColumns;
	mtext->font = font;
	mtext->fontSize = fontSize;
	mtext->padding = padding;
	mtext->textColor = textColor;
	mtext->bgColor = bgColor;
	mtext->chWidth = (fontSize * font.recs->width / font.baseSize) + font.glyphPadding / 2.0f;

	mtext->col = mtext->lin = 0;
	mtext->text.push_back("");

	mtext->focused = false;

	mtext->firstLin = mtext->firstCol = 0;
	mtext->limLin = mtext->limCol = 0;
	// TODO: this won t work for any font
	mtext->x = startX;
	mtext->y = startY;
	mtext->width = visibleColumns * mtext->chWidth + 2.0f * padding;
	mtext->height = visibleLines * fontSize + 2.0f * padding;
	mtext->visible = false;

	return mtext;
}
void CleanupMultiLineText(MultiLineText* mtext) {

}
void MultiLineTextDraw(MultiLineText* mtext) {
	if (!mtext->visible) {
		return;
	}
	DrawRectangle(mtext->x, mtext->y, mtext->width, mtext->height, mtext->bgColor);

	for (size_t i = mtext->firstLin, posLin = 0; i < mtext->text.size() && i < mtext->firstLin + mtext->visLin; i++, posLin++) {
		//if (mtext->text[i].empty()) continue;
		if (mtext->text[i].size() < mtext->firstCol) continue;

		size_t n = (mtext->firstCol + mtext->visCol) > mtext->text[i].size() ? mtext->text[i].size() - mtext->firstCol + 1 : mtext->visCol;
		std::string cropped = mtext->text[i].substr(mtext->firstCol, n);

		DrawTextEx(mtext->font, cropped.c_str(), { (float)mtext->x + mtext->padding, (float)mtext->y + posLin * mtext->fontSize + mtext->padding }, mtext->fontSize, 2, mtext->textColor);
	}

	/*std::string cursor;
	cursor += std::to_string(mtext->lin);
	cursor += "/";
	cursor += std::to_string(mtext->text.size());
	cursor += ";";
	cursor += std::to_string(mtext->col);
	cursor += "/";
	cursor += std::to_string(mtext->text[mtext->lin].size());
	DrawText(cursor.c_str(), mtext->x + mtext->width + 5, mtext->y, fontSize, BLACK);*/

	if (mtext->focused) {
		// TODO: figure out how to calculate mtext->text width based on a given font
		DrawLineV({ mtext->x + mtext->padding + (mtext->col - mtext->firstCol) * mtext->chWidth, (float)mtext->y + mtext->padding + (mtext->lin - mtext->firstLin) * mtext->fontSize }, { mtext->x + mtext->padding + (mtext->col - mtext->firstCol) * mtext->chWidth, (float)mtext->y + mtext->padding + (mtext->lin - mtext->firstLin + 1) * mtext->fontSize }, mtext->textColor);
		DrawLineV({ mtext->x + mtext->padding + (mtext->limCol - mtext->firstCol) * mtext->chWidth, (float)mtext->y + mtext->padding + (mtext->limLin - mtext->firstLin) * mtext->fontSize }, { mtext->x + mtext->padding + (mtext->limCol - mtext->firstCol) * mtext->chWidth, (float)mtext->y + mtext->padding + (mtext->limLin - mtext->firstLin + 1) * mtext->fontSize }, RED);
	}
}
std::vector<std::string> MultiLineTextParseText(MultiLineText* mtext, std::string strToParse) {
	size_t n = strToParse.size();
	std::vector<std::string> temp(1);
	for (size_t i = 0; i < n; i++) {
		if (strToParse[i] == '\r') {
			strToParse.erase(i, 1);
		}
	}
	size_t prev = 0, num = 0;
	for (size_t i = 0; i < n; i++) {
		if (strToParse[i] == '\n') {
			temp[num] = strToParse.substr(prev, i - prev);
			temp.push_back("");
			num++;
			prev = i + 1;
		}
	}
	if (temp[0].empty()) {
		temp[0] += strToParse;
	}
	else if (strToParse[n - 1] != '\n') {
		temp.push_back(strToParse.substr(prev, n - prev)); // TODO: might break
	}

	for (std::string& str : temp) {
		size_t s = str.size();
		for (size_t i = 0; i < str.size();) {
			if (str[i] == '\t') {
				str.erase(i, 1);
				str.insert(i, 4, ' ');
				i += 4;
			}
			else {
				i++;
			}
		}
	}

	return temp;
}
void MultiLineTextInsertLine(MultiLineText* mtext, size_t pos, std::string line) {
	mtext->text.insert(mtext->text.begin() + pos, line);
}
void MultiLineTextPushLine(MultiLineText* mtext, std::string line) {
	mtext->text.push_back(line);
}
void MultiLineTextPushString(MultiLineText* mtext, std::string str) {
	auto lines = MultiLineTextParseText(mtext, str);
	size_t n = lines.size();
	mtext->text[mtext->limLin] += lines[0];
	for (size_t i = 1; i < n; i++) {
		mtext->text.push_back(lines[i]);
	}
}
void MultiLineTextOverrideLine(MultiLineText* mtext, size_t pos, std::string line) {
	mtext->text[pos] = line;
}
void MultiLineTextEdit(MultiLineText* mtext) {
	if (!mtext->visible) {
		return;
	}

	auto mpos = GetMousePosition();
	bool inBounds = (mpos.x >= mtext->x && mpos.x < mtext->x + mtext->width) && (mpos.y >= mtext->y && mpos.y < mtext->y + mtext->height);
	// TODO: maybe create a static member to avoid conflict
	//if (inBounds) {
	//	SetMouseCursor(MOUSE_CURSOR_IBEAM); // TODO: shouldn t set this that often
	//}
	//else {
	//	SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	//}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (inBounds) {
			mtext->focused = true;
		}
		else {
			mtext->focused = false;
		}
	}

	if (!mtext->focused) return;

	int chr = GetCharPressed();
	if (chr != 0) {
		if (mtext->lin > mtext->limLin || (mtext->lin == mtext->limLin && mtext->col >= mtext->limCol)) {
			mtext->text[mtext->lin].insert(mtext->col, 1, chr);
			if (mtext->text[mtext->lin].size() >= mtext->visCol) {
				mtext->firstCol++;
			}
			mtext->col++;
		}
	}

	int key = GetKeyPressed();
	if (key == 0) return;

	if (key == KEY_UP) {
		MultiLineTextToPreviousLine(mtext);
	}
	else if (key == KEY_DOWN) {
		MultiLineTextToNextLine(mtext);
	}
	else if (key == KEY_LEFT) {
		MultiLineTextToPreviousColumn(mtext);
	}
	else if (key == KEY_RIGHT) {
		MultiLineTextToNextColumn(mtext);
	}
	else if (key == KEY_ENTER) {
		MultiLineTextEnter(mtext);
	}
	else if (key == KEY_BACKSPACE) {
		MultiLineTextBackspace(mtext);
	}

	if (IsKeyDown(KEY_LEFT_CONTROL)) {
		if (IsKeyPressed(KEY_S)) {
			// TODO: save to file
			std::cout << "Command\n";
		}
		else if (IsKeyPressed(KEY_C)) {
			MultiLineTextCopy(mtext);
		}
		else if (IsKeyPressed(KEY_V)) {
			MultiLineTextPaste(mtext);
		}
	}
}
void MultiLineTextSetPosition(MultiLineText* mtext, int x, int y) {
	mtext->x = x;
	mtext->y = y;
}
bool MultiLineTextIsHovered(MultiLineText* mtext) {
	int mx = GetMouseX(), my = GetMouseY();
	return mtext->visible && mx >= mtext->x && mx <= mtext->x + mtext->width && my >= mtext->y && my <= mtext->y + mtext->height;
}
bool MultiLineTextIsClicked(MultiLineText* mtext) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && MultiLineTextIsHovered(mtext);
}
void MultiLineTextClear(MultiLineText* mtext) {
	mtext->lin = mtext->col = 0;
	mtext->firstLin = mtext->firstCol = 0;
	mtext->limLin = mtext->limCol = 0;
	mtext->text.clear();
	mtext->text.push_back("");
}
void MultiLineTextSetLimit(MultiLineText* mtext, size_t limLin, size_t limCol) {
	mtext->limLin = limLin;
	mtext->limCol = limCol;
	if (mtext->lin < mtext->limLin) {
		mtext->lin = mtext->limLin;
		if (mtext->lin >= mtext->firstLin + mtext->visLin) {
			mtext->firstLin = mtext->lin - mtext->visLin + 1;
		}
	}
	if (mtext->lin == mtext->limLin && mtext->col < mtext->limCol) {
		mtext->col = limCol;
		if (mtext->col >= mtext->firstCol + mtext->visCol) {
			mtext->firstCol = mtext->col - mtext->visCol + 1;
		}
	}
}
void MultiLineTextSetLimitMax(MultiLineText* mtext) {
	mtext->lin = mtext->limLin = mtext->text.size() - 1;
	if (mtext->lin >= mtext->firstLin + mtext->visLin) {
		mtext->firstLin = mtext->lin - mtext->visLin + 1;
	}
	mtext->col = mtext->limCol = mtext->text[mtext->limLin].size();
	if (mtext->col >= mtext->firstCol + mtext->visCol) {
		mtext->firstCol = mtext->col - mtext->visCol + 1;
	}
}
bool MultiLineTextGetNextNumber(MultiLineText* mtext, float& x) {
	bool reachedEnd = false;
	while (!isdigit(mtext->text[mtext->limLin][mtext->limCol]) && mtext->text[mtext->limLin][mtext->limCol] != '.') {
		if (mtext->limCol == mtext->text[mtext->limLin].size() - 1) {
			if (mtext->limLin < mtext->text.size() - 1) {
				mtext->limLin++;
				mtext->limCol = 0;
			}
			else {
				reachedEnd = true;
				break;
			}
		}
		else {
			mtext->limCol++;
		}
	}
	if (reachedEnd && !isdigit(mtext->text[mtext->limLin][mtext->limCol])) {
		return true;
	}
	std::string str = "";
	size_t cnt = 0;
	while (isdigit(mtext->text[mtext->limLin][mtext->limCol]) || mtext->text[mtext->limLin][mtext->limCol] == '.') {
		str += mtext->text[mtext->limLin][mtext->limCol];
		if (mtext->text[mtext->limLin][mtext->limCol] == '.') {
			cnt++;
		}
		if (mtext->limCol == mtext->text[mtext->limLin].size() - 1) {
			if (mtext->limLin < mtext->text.size() - 1) {
				mtext->limLin++;
				mtext->limCol = 0;
			}
			else {
				reachedEnd = true;
				break;
			}
		}
		else {
			mtext->limCol++;
		}
	}
	if (cnt > 1) {
		return true;
	}

	try {
		x = stof(str);
	}
	catch (...) {
		return true;
	}
	return false;
}