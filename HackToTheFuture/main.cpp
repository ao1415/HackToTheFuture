#include <iostream>
#include <array>
#include <queue>
#include <string>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>
#include <cmath>

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
	const bool operator==(const Point& o) const {
		return (x == o.x && y == o.y);
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
			cout << grid[y][x] << " ";
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

	const int mr = power - 1;
	int diff = 0;

	const auto getScore = [&](int r) {

		//全体的な差分
		//小さいほど理想的になる
		//そのうちターン係数など入れるかも
		double score = 0;

		const double c = max((1000 - turn) / 20.0, 1.0);

		for (int yy = 0; yy < N; yy++)
		{
			for (int xx = 0; xx < N; xx++)
			{
				//差分を求める
				double sub = table[yy][xx] - range(x - xx, y - yy, r + 1);

				if (r >= 65)
				{
					if (sub < 0) sub *= 100000;
					score += abs(sub);
				}
				else
				{
					if (sub < 0) sub *= c;
					score += abs(sub);
				}
			}
		}

		return score;
	};

	double minScore = getScore(mr);
	int best = mr;
	for (int p = mr - 1; p >= 0; p--)
	{
		const double score = getScore(p);
		if (minScore >= score)
		{
			minScore = score;
			best = p;
		}
	}

	return best + 1;
}

int result(const Grid& table, int t) {

	int score = 200000000;

	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			score -= abs(table[y][x]);
			//if (table[y][x] < -100) cout << t << ":" << table[y][x] << endl;
		}
	}

	return score;
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


	struct Data {
		int score;
		Grid table;
		Answer ans;

		const bool operator<(const Data& o) const {
			return score < o.score;
		}
	};

	priority_queue<Data> que;
	que.push(Data{ result(input,0),input ,Answer() });

	for (int t = 0; t < 1000; t++)
	{
		priority_queue<Data> next;

		for (int i = 0; i < 1; i++)
		{
			if (!que.empty())
			{
				const auto top = que.top();
				que.pop();

				priority_queue<Point> topList;

				Point max1(0, 0, 0);
				Point max2(0, 0, 0);
				for (int y = 0; y < N; y++)
				{
					for (int x = 0; x < N; x++)
					{
						if (max1.h < top.table[y][x])
						{
							max1.x = x;
							max1.y = y;
							max1.h = top.table[y][x];
						}
						if (max2.h <= top.table[y][x])
						{
							max2.x = x;
							max2.y = y;
							max2.h = top.table[y][x];
							topList.push(max2);
						}
					}
				}

				{
					vector<Point> list;
					while (!topList.empty())
					{
						if (topList.top().h != max1.h) break;

						list.push_back(topList.top());
						topList.pop();
					}
					max2 = list[list.size() / 2];
				}

				{
					const int h = subPower(max1.x, max1.y, max1.h, top.table, t);
					const auto& table = subMountain(max1.x, max1.y, h, top.table);
					const int score = result(table, t);
					Answer ans = top.ans;
					ans.push_back(format(max1.x, max1.y, h));
					next.push(Data{ score,table, ans });
				}
				if (!(max1 == max2)) {
					const int h = subPower(max2.x, max2.y, max2.h, top.table, t);
					const auto& table = subMountain(max2.x, max2.y, h, top.table);
					const int score = result(table, t);
					Answer ans = top.ans;
					ans.push_back(format(max2.x, max2.y, h));
					next.push(Data{ score,table, ans });
				}

			}
		}
		que.swap(next);
	}

	{
		const auto& ans = que.top().ans;

		cout << ans.size() << endl;
		for (const auto& s : ans)
		{
			cout << s << endl;
		}

	}

	return 0;
}
