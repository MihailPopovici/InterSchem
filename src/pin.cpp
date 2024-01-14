#include "pin.h"
#include "nodeComponents.h"

#include "raylib.h"

bool LineLine(const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& D, Vector2& intersection) {
	float uA = ((D.x - C.x) * (A.y - C.y) - (D.y - C.y) * (A.x - C.x)) / ((D.y - C.y) * (B.x - A.x) - (D.x - C.x) * (B.y - A.y));
	float uB = ((B.x - A.x) * (A.y - C.y) - (B.y - A.y) * (A.x - C.x)) / ((D.y - C.y) * (B.x - A.x) - (D.x - C.x) * (B.y - A.y));
	if (uA > 0 && uA < 1 && uB > 0 && uB < 1) { // add an '=' if you want to detect margins too
		intersection.x = A.x + (uA * (B.x - A.x));
		intersection.y = A.y + (uA * (B.y - A.y));
		return true;
	}
	return false;
}

std::vector<Vector2> CreateRoad(const Vector2 A, const Vector2 B, void* firstNode, void* secondNode) {
	//TODO: change this block of for's
	std::vector<void*> obstacles;
	if (nodes.startNode != nullptr) {
		obstacles.push_back(nodes.startNode);
	}
	for (auto& p : nodes.readNodes) {
		obstacles.push_back(p);
	}
	for (auto& p : nodes.writeNodes) {
		obstacles.push_back(p);
	}
	for (auto& p : nodes.assignNodes) {
		obstacles.push_back(p);
	}
	for (auto& p : nodes.decisionNodes) {
		obstacles.push_back(p);
	}
	for (auto& p : nodes.stopNodes) {
		obstacles.push_back(p);
	}
	std::vector<Vector2> points;
	std::vector<bool> solved(3);
	if (A.y < B.y) {
		points.push_back(A);
		points.push_back({ B.x, A.y });
		points.push_back(B);
	}
	else {
		points.push_back(A);
		points.push_back({ A.x, B.y });
		points.push_back(B);
	}
	size_t pos = 1;
	solved[0] = true;
	while (pos < points.size()) {
		if (solved[pos]) {
			pos++;
			continue;
		}

		for (void*& node : obstacles) {
			int* nodePtr = (int*)node;
			Rectangle rec = { (float)(*nodePtr), (float)(*(nodePtr + 1)), (float)(*(nodePtr + 2)), (float)(*(nodePtr + 3)) };
			Vector2 intLeft, intTop, intBottom, intRight;
			Vector2 topLeft{ rec.x, rec.y };
			Vector2 topRight{ rec.x + rec.width, rec.y };
			Vector2 bottomRight{ rec.x + rec.width, rec.y + rec.height };
			Vector2 bottomLeft{ rec.x, rec.y + rec.height };
			bool left = LineLine(points[pos - 1], points[pos], topLeft, bottomLeft, intLeft);
			bool top = LineLine(points[pos - 1], points[pos], topLeft, topRight, intTop);
			bool right = LineLine(points[pos - 1], points[pos], topRight, bottomRight, intRight);
			bool bottom = LineLine(points[pos - 1], points[pos], bottomRight, bottomLeft, intBottom);
			if (left ^ top ^ right ^ bottom) {
				if (left) {
					if (node == firstNode) {
						points[1] = bottomLeft;
						points.insert(points.begin() + 2, { bottomLeft.x, points[2].y });
						solved.insert(solved.begin() + 2, true);
						pos++;
						break;
					}
					else {
						points[pos].x = bottomLeft.x;
						points.insert(points.begin() + pos + 1, bottomLeft);
						points.insert(points.begin() + pos + 2, { points[pos + 2].x, bottomLeft.y });
						solved.insert(solved.begin() + 1, true);
						solved.insert(solved.begin() + 2, true);
					}
				}
				else if (right) {
					if (node == firstNode) {
						points[1] = bottomRight;
						points.insert(points.begin() + 2, { bottomRight.x, points[2].y });
						solved.insert(solved.begin() + 2, true);
						pos++;
						break;
					}
					else {
						points[pos].x = bottomRight.x;
						points.insert(points.begin() + pos + 1, bottomRight);
						points.insert(points.begin() + pos + 2, { points[pos + 2].x, bottomRight.y });
						solved.insert(solved.begin() + 1, true);
						solved.insert(solved.begin() + 2, true);
					}
				}
				else if (top) {
					if (node == firstNode) {
						if (A.x < B.x) {
							points[1].x = bottomRight.x;
							points.insert(points.begin() + 1, bottomRight);
							solved.insert(solved.begin() + 1, true);
							break;
						}
						else {
							points[1].x = bottomLeft.x;
							points.insert(points.begin() + 1, bottomLeft);
							solved.insert(solved.begin() + 1, true);
							break;
						}
					}
					else {
						// TODO: what else?
					}
				}
				else {
					if (node == secondNode) {
						if (points[pos].x < points[pos + 1].x) {
							points[pos].y = bottomLeft.y;
							points.insert(points.begin() + pos + 1, bottomLeft);
							points.insert(points.begin() + pos + 2, topLeft);
						}
						else {
							points[pos].y = bottomRight.y;
							points.insert(points.begin() + pos + 1, bottomRight);
							points.insert(points.begin() + pos + 2, topRight);
						}
						solved.insert(solved.begin() + 1, true);
						solved.insert(solved.begin() + 2, true);
						pos = points.size() - 1;
						break;
					}
					points[pos].y = intBottom.y;
					if (points[pos].x < points[pos + 1].x) {
						points.insert(points.begin() + pos + 1, bottomRight);
						points.insert(points.begin() + pos + 2, { bottomRight.x, points[pos + 2].y });
					}
					else {
						points.insert(points.begin() + pos + 1, bottomLeft);
						points.insert(points.begin() + pos + 2, { bottomLeft.x, points[pos + 2].y });
					}
					solved.insert(solved.begin() + pos + 1, true);
					solved.insert(solved.begin() + pos + 2, true);
				}
				continue;
			}
			if (top && bottom) {
				float ratio = (intTop.x - rec.x) / rec.width;
				if (points[0].y < points[points.size() - 1].y) {
					points.insert(points.begin() + pos, intTop);
					if (ratio < 0.5f) {
						points.insert(points.begin() + pos + 1, topLeft);
						points.insert(points.begin() + pos + 2, bottomLeft);
					}
					else {
						points.insert(points.begin() + pos + 1, topRight);
						points.insert(points.begin() + pos + 2, bottomRight);
					}
					points.insert(points.begin() + pos + 3, intBottom);
				}
				else {
					points.insert(points.begin() + pos, intBottom);
					if (ratio < 0.5f) {
						points.insert(points.begin() + pos + 1, bottomLeft);
						points.insert(points.begin() + pos + 2, topLeft);
					}
					else {
						points.insert(points.begin() + pos + 1, bottomRight);
						points.insert(points.begin() + pos + 2, topRight);
					}
					points.insert(points.begin() + pos + 3, intTop);
				}

				solved.insert(solved.begin() + pos, false);
				solved.insert(solved.begin() + pos + 1, true);
				solved.insert(solved.begin() + pos + 2, true);
				solved.insert(solved.begin() + pos + 3, true);
				continue;
			}
			if (left && right) {
				float ratio = (intLeft.y - rec.y) / rec.height;
				points.insert(points.begin() + pos, intLeft);
				if (ratio < 0.5f) {
					points.insert(points.begin() + pos + 1, topLeft);
					points.insert(points.begin() + pos + 2, topRight);
				}
				else {
					points.insert(points.begin() + pos + 1, bottomLeft);
					points.insert(points.begin() + pos + 2, bottomRight);
				}
				points.insert(points.begin() + pos + 3, intRight);
				
				solved.insert(solved.begin() + pos, false);
				solved.insert(solved.begin() + pos + 1, true);
				solved.insert(solved.begin() + pos + 2, true);
				solved.insert(solved.begin() + pos + 3, true);
				if (points[pos].x < points[pos - 1].x) {
					std::swap(points[pos], points[pos + 3]);
					std::swap(points[pos + 1], points[pos + 2]);
				}
				continue;
			}
		}
		pos++;
	}
	return points;
}

void DrawLink(Pin& from, Pin* to) {
	if (to == nullptr) return;
	float fromX = (float)from.x, fromY = (float)from.y, toX = (float)to->x, toY = (float)to->y;

	if ((fromX - toX) * (fromX - toX) + (fromY - toY) * (fromY - toY) <= 256) {
		DrawLineEx({ fromX, fromY }, { toX, toY }, 2.0f, GRAY);
	}

	auto points = CreateRoad({ fromX, fromY }, { toX, toY }, from.ownerPtr, to->ownerPtr);
	for (size_t i = 0; i < points.size() - 1; i++) {
		DrawLineEx(points[i], points[i + 1], 2.0f, GRAY);
		//DrawCircleV(points[i], 2.0f, RED);
	}
	if (points[points.size() - 1].y == points[points.size() - 2].y) {
		if (points[points.size() - 1].x > points[points.size() - 2].x) {
			DrawTriangle({ toX, toY }, { toX - 8.0f, toY - 4.0f }, { toX - 8.0f, toY + 4.0f }, GRAY);
		}
		else {
			DrawTriangle({ toX, toY }, { toX + 8.0f, toY + 4.0f }, { toX + 8.0f, toY - 4.0f }, GRAY);
		}
	}
	else {
		DrawTriangle({ toX, toY }, { toX + 4.0f, toY - 8.0f }, { toX - 4.0f, toY - 8.0f }, GRAY);
	}
}
void DrawGhostLink(Pin* pin, int mx, int my) {
	float x = pin->x, y = pin->y;
	if (y < my) {
		DrawLineEx({ x, y }, { (float)mx, y }, 2.0f, GRAY);
		DrawLineEx({ (float)mx, y }, { (float)mx, (float)my }, 2.0f, GRAY);
	}
	else {
		DrawLineEx({ x, y }, { x, (float)my }, 2.0f, GRAY);
		DrawLineEx({ x, (float)my }, { (float)mx, (float)my }, 2.0f, GRAY);
	}
}
void NewLink(Pin*& first, Pin& second) {
	first = &second;
}