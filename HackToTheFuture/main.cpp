#include <iostream>
#include <array>
#include <queue>
#include <string>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>

using namespace std;

const int N = 100;
const int Inf = 999999999;

template<class Type, size_t Width, size_t Height>
class FixedGrid {
private:

	using ContainerType = std::array<Type, Width * Height>;
	ContainerType m_data;

public:

	FixedGrid() = default;
	FixedGrid(const Type& v) { fill(v); }
	FixedGrid(const FixedGrid& other) = default;

	FixedGrid& operator=(const FixedGrid& other) = default;
	FixedGrid& operator=(FixedGrid&& other) = default;

	const Type* operator[](size_t y) const {
		return &m_data[y * Width];
	}
	Type* operator[](size_t y) {
		return &m_data[y * Width];
	}

	const Type& at(size_t x, size_t y) const {
		if (outside(x, y))
			throw std::out_of_range("FixedGrid::at");
		return m_data[y * Width + x];
	}
	Type& at(size_t x, size_t y) {
		if (outside(x, y))
			throw std::out_of_range("FixedGrid::at");
		return m_data[y * Width + x];
	}

	constexpr size_t width() const {
		return Width;
	}
	constexpr size_t height() const {
		return Height;
	}

	bool inside(size_t x, size_t y) const {
		return (0 <= x && x < Width && 0 <= y && y < Height);
	}
	bool outside(size_t x, size_t y) const {
		return (0 > x || x >= Width || 0 > y || y >= Height);
	}

	void fill(const Type& v) noexcept {
		m_data.fill(v);
	}

	void clear() {
		m_data.swap(ContainerType());
	}

};

using Grid = FixedGrid<int, N, N>;
using Answer = vector<string>;

struct Point {
	int x;
	int y;
	int h = 0;

	Point() {
		x = y = h = -1;
	}
	Point(int _x, int _y, int _h) {
		x = _x;
		y = _y;
		h = _h;
	}

	const bool operator<(const Point& other) const {
		return h < other.h;
	}

};

const string format(int x, int y, int h) {
	return to_string(x) + " " + to_string(y) + " " + to_string(h);
}
const int clamp(int v) {
	return min(N, max(1, v));
}
const bool inside(int v) {
	return (0 <= v && v < N);
}
const int range(int dx, int dy, int h) {

	int r = abs(dx) + abs(dy);

	return max(0, h - r);
}

void show(const Grid& grid) {

	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			cout << grid[y][x] << "\t";
		}
		cout << endl;
	}

}

const int directionX[] = { 0,-1,1,0 };
const int directionY[] = { -1,0,0,1 };

Grid subMountain(int x, int y, int h, const Grid& table) {

	Grid next(table);

	const int r = h - 1;

	for (int dy = -r; dy <= r; dy++)
	{
		for (int dx = -r; dx <= r; dx++)
		{
			int px = x + dx;
			int py = y + dy;

			if (inside(px) && inside(py))
			{
				next[py][px] -= range(dx, dy, h);
			}
		}
	}

	return next;
}

const int subPower(int x, int y, int h, const Grid& table, int turn) {

	const int power = clamp(h);

	const int r = power - 1;
	int diff = 0;

	const auto getScore = [&](int r) {

		//全体的な差分
		//小さいほど理想的になる
		//そのうちターン係数など入れるかも
		int score = 0;

		const double c = max((1000 - turn) / 20, 1);

		for (int yy = 0; yy < N; yy++)
		{
			for (int xx = 0; xx < N; xx++)
			{
				//差分を求める
				int sub = table[yy][xx] - range(x - xx, y - yy, r + 1);
				if (sub < 0) sub = static_cast<int>(sub*c);
				score += abs(sub);
			}
		}

		return score;
	};

	int minScore = getScore(r);
	int best = r;
	for (int p = r - 1; p >= 0; p--)
	{
		const int score = getScore(p);
		if (minScore > score)
		{
			minScore = score;
			best = p;
		}
	}

	return best + 1;
}

int main() {

	Grid input;

	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			cin >> input[y][x];
		}
	}

	Answer ans;

	auto next = input;
	for (int t = 0; t < 1000; t++)
	{
		array<int, N> diffx;
		array<int, N> diffy;

		diffx.fill(0);
		diffy.fill(0);

		for (int y = 0; y < N; y++)
		{
			for (int x = 0; x < N; x++)
			{
				const int px = (0 <= x - 1) ? next[y][x - 1] : 0;
				const int ax = (x - 1 < N) ? next[y][x + 1] : 0;
				const int py = (0 <= y - 1) ? next[y - 1][x] : 0;
				const int ay = (y - 1 < N) ? next[y + 1][x] : 0;

				//縦
				if (px <= next[y][x] && next[y][x] >= ax)
				{
					diffx[y]++;
				}
				//横
				if (py <= next[y][x] && next[y][x] >= ay)
				{
					diffy[x]++;
				}
			}
		}

		pair<int, bool> x1{ 0,false };
		pair<int, bool> x2{ 0,true };
		pair<int, bool> y1{ 0,false };
		pair<int, bool> y2{ 0,true };

		for (int i = 0; i < N; i++)
		{
			if (diffx[i] > 0)
			{
				if (!x1.second)
				{
					x1.first = i;
					x1.second = true;
				}
				x2.first = i;
			}
			if (diffy[i] > 0)
			{
				if (!y1.second)
				{
					y1.first = i;
					y1.second = true;
				}
				y2.first = i;
			}
		}

		Point top(0, 0, 0);

		for (int y = y1.first; y <= y2.first; y++)
		{
			for (int x = x1.first; x <= x2.first; x++)
			{
				if (top.h < next[y][x])
				{
					top.x = x;
					top.y = y;
					top.h = next[y][x];
				}
			}
		}

		if (top.h == 0) break;

		int power = subPower(top.x, top.y, top.h, next, t);

		ans.push_back(format(top.x, top.y, power));
		next = subMountain(top.x, top.y, power, next);
	}

	cout << ans.size() << endl;
	for (const auto& s : ans)
	{
		cout << s << endl;
	}

	return 0;
}
