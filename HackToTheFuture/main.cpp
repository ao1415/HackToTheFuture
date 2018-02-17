#include <iostream>
#include <array>
#include <queue>
#include <string>
#include <algorithm>
#include <vector>
#include <stack>

using namespace std;

const int N = 100;

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

const int directionX[] = { 0,-1,1,0 };
const int directionY[] = { -1,0,0,1 };

Grid labeling(const Grid& input) {

	Grid label(0);
	Grid check(0);

	int count = 1;
	for (int x = 0; x < N; x++)
	{
		for (int y = 0; y < N; y++)
		{
			if (check[y][x] == 0)
			{
				if (input[y][x] != 0)
				{
					stack<Point> sta;
					sta.push(Point(x, y, count));
					check[y][x] = 1;
					label[y][x] = count;
					count++;

					while (!sta.empty())
					{
						const auto pos = sta.top();
						sta.pop();


						for (int i = 0; i < 4; i++)
						{
							const int posx = pos.x + directionX[i];
							const int posy = pos.y + directionY[i];

							if (inside(posx) && inside(posy))
							{
								if (check[posy][posx] == 0 && input[posy][posx] != 0)
								{
									sta.push(Point(posx, posy, pos.h));
									check[posy][posx] = 1;
									label[posy][posx] = pos.h;
								}
							}

						}

					}

				}
			}
		}
	}

	return label;
}

Grid subMountain(int x, int y, int h, const Grid& map) {

	Grid next(map);

	const int r = h - 1;

	for (int dy = -r; dy <= r; dy++)
	{
		for (int dx = -r; dx <= r; dx++)
		{
			int px = x + dx;
			int py = y + dy;

			if (inside(px) && inside(py))
			{
				next[y][x] -= range(dx, dy, h);
			}
		}
	}

	return next;
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

	priority_queue<Point> que;

	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			que.push(Point(x, y, input[y][x]));
		}
	}

	const auto output = [&]() {

		Answer ans;
		for (int i = 0; i < 10; i++)
		{
			const auto cell = que.top();
			que.pop();

			int h = clamp(cell.h);

			ans.push_back(format(cell.x, cell.y, h));

		}

		cout << ans.size() << endl;
		for (const auto& s : ans)
		{
			cout << s << endl;
		}

	};

	//debug();
	output();

	return 0;
}
