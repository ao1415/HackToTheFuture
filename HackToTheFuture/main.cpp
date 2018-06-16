#include <iostream>
#include <array>
#include <queue>
#include <string>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>
#include <cmath>
#include <chrono>

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

/// <summary>
/// 一定時間の経過したか確認するクラス
/// </summary>
class Timer {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Timer() = default;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(ナノ秒)</param>
	Timer(const std::chrono::nanoseconds& _time) { type = Type::nanoseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(マイクロ秒)</param>
	Timer(const std::chrono::microseconds& _time) { type = Type::microseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	Timer(const std::chrono::milliseconds& _time) { type = Type::milliseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(秒)</param>
	Timer(const std::chrono::seconds& _time) { type = Type::seconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(分)</param>
	Timer(const std::chrono::minutes& _time) { type = Type::minutes; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(時)</param>
	Timer(const std::chrono::hours& _time) { type = Type::hours; time = _time.count(); }

	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(ナノ秒)</param>
	void set(const std::chrono::nanoseconds& _time) { type = Type::nanoseconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(マイクロ秒)</param>
	void set(const std::chrono::microseconds& _time) { type = Type::microseconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	void set(const std::chrono::milliseconds& _time) { type = Type::milliseconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(秒)</param>
	void set(const std::chrono::seconds& _time) { type = Type::seconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(分</param>
	void set(const std::chrono::minutes& _time) { type = Type::minutes; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(時)</param>
	void set(const std::chrono::hours& _time) { type = Type::hours; time = _time.count(); }

	/// <summary>
	/// タイマーを開始させる
	/// </summary>
	void start() { s = std::chrono::high_resolution_clock::now(); }

	inline long long diff() const {
		const auto e = std::chrono::high_resolution_clock::now();
		switch (type)
		{
		case Type::nanoseconds: return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();
		case Type::microseconds: return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count();
		case Type::milliseconds: return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
		case Type::seconds: return std::chrono::duration_cast<std::chrono::seconds>(e - s).count();
		case Type::minutes: return std::chrono::duration_cast<std::chrono::minutes>(e - s).count();
		case Type::hours: return std::chrono::duration_cast<std::chrono::hours>(e - s).count();
		default: return true;
		}
	}

	/// <summary>
	/// 設定時間経過したかを得る
	/// </summary>
	/// <returns>経過していれば true, それ以外は false</returns>
	inline const bool check() const {
		return diff() >= time;
	}

	/// <summary>
	/// 設定時間経過したかを得る
	/// </summary>
	/// <returns>経過していれば true, それ以外は false</returns>
	operator bool() const { return check(); }

private:

	enum class Type {
		nanoseconds,
		microseconds,
		milliseconds,
		seconds,
		minutes,
		hours
	};

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	long long time;
	Type type;

};

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

struct XorShift {
	unsigned int x;
	XorShift() : x(2463534242U) {}
	unsigned int rand() {
		x ^= (x << 13);
		x ^= (x >> 17);
		x ^= (x << 5);
		return x;
	}

};

using Grid = FixedGrid<int, N, N>;
using Answer = array<Point, 1000>;

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

class Annealing {
private:

	/// <summary>
	/// 実行時間(ms)
	/// </summary>
	const int T = 5500;

	const double TempStart = 10000.0;
	const double TempEnd = 0.0001;
	const double Time = T;
	const double TempDiff = (TempStart - TempEnd) / Time;

	XorShift random;
	int size;

	bool probability(const double& diff, const long long& t) {

		if (diff > 0) return true;

		const double temp = TempStart - TempDiff * t;

		const double p = exp(diff / temp) * 4294967295.0;

		return p > random.rand();
	}

	int result(const Grid& table, int t = 0) {

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

	void sub(const Point& pos, Grid& table) {

		const int r = pos.h - 1;

		for (int dy = -r; dy <= r; dy++)
		{
			for (int dx = -r; dx <= r; dx++)
			{
				int px = pos.x + dx;
				int py = pos.y + dy;

				if (inside(px) && inside(py))
				{
					table[py][px] -= range(dx, dy, pos.h);
				}
			}
		}
	}

	void move(Point& pos, const int direction, Grid& table) {

		const int r = pos.h - 1;

		switch (direction)
		{
			//上
		case 0:

			for (int dy = -r; dy <= 0; dy++)
			{
				for (int dx = -r; dx <= r; dx++)
				{
					int px = pos.x + dx + directionX[direction];
					int py = pos.y + dy + directionY[direction];

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
							table[py][px] -= 1;
					}
				}
			}

			for (int dy = 0; dy <= r; dy++)
			{
				for (int dx = -r; dx <= r; dx++)
				{
					int px = pos.x + dx;
					int py = pos.y + dy;

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
							table[py][px] += 1;
					}
				}
			}

			break;
			//左
		case 1:

			for (int dy = -r; dy <= r; dy++)
			{
				for (int dx = -r; dx <= 0; dx++)
				{
					int px = pos.x + dx + directionX[direction];
					int py = pos.y + dy + directionY[direction];

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
							table[py][px] -= 1;
					}
				}
			}

			for (int dy = -r; dy <= r; dy++)
			{
				for (int dx = 0; dx <= r; dx++)
				{
					int px = pos.x + dx;
					int py = pos.y + dy;

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
							table[py][px] += 1;
					}
				}
			}

			break;
			//右
		case 2:

			for (int dy = -r; dy <= r; dy++)
			{
				for (int dx = -r; dx <= 0; dx++)
				{
					int px = pos.x + dx;
					int py = pos.y + dy;

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
							table[py][px] += 1;
					}
				}
			}

			for (int dy = -r; dy <= r; dy++)
			{
				for (int dx = 0; dx <= r; dx++)
				{
					int px = pos.x + dx + directionX[direction];
					int py = pos.y + dy + directionY[direction];

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
							table[py][px] -= 1;
					}
				}
			}

			break;
			//下
		case 3:

			for (int dy = -r; dy <= 0; dy++)
			{
				for (int dx = -r; dx <= r; dx++)
				{
					int px = pos.x + dx;
					int py = pos.y + dy;

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
							table[py][px] += 1;
					}
				}
			}

			for (int dy = 0; dy <= r; dy++)
			{
				for (int dx = -r; dx <= r; dx++)
				{
					int px = pos.x + dx + directionX[direction];
					int py = pos.y + dy + directionY[direction];

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
							table[py][px] -= 1;
					}
				}
			}

			break;
		}

		pos.x += directionX[direction];
		pos.y += directionY[direction];

	}

	void inc(Point& pos, const int h, Grid& table) {

		const int r = pos.h + h - 1;

		for (int dy = -r; dy <= r; dy++)
		{
			for (int dx = -r; dx <= r; dx++)
			{
				int px = pos.x + dx;
				int py = pos.y + dy;

				if (inside(px) && inside(py))
				{
					if (abs(dx) + abs(dy) <= r)
						table[py][px] -= h;
				}
			}
		}

		pos.h += h;
	}

	int moveScore(const Point& pos, const int direction, const Grid& table) const {

		if (!(inside(pos.x + directionX[direction]) && inside(pos.y + directionY[direction]))) return Inf;

		const int r = pos.h - 1;
		int nowScore = 0;
		int nextScore = 0;

		switch (direction)
		{
			//上
		case 0:

			for (int dy = -r; dy <= 0; dy++)
			{
				for (int dx = -r; dx <= r; dx++)
				{
					int px = pos.x + dx + directionX[direction];
					int py = pos.y + dy + directionY[direction];

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
						{
							nowScore += abs(table[py][px]);
							nextScore += abs(table[py][px] - 1);
						}
					}
				}
			}

			for (int dy = 0; dy <= r; dy++)
			{
				for (int dx = -r; dx <= r; dx++)
				{
					int px = pos.x + dx;
					int py = pos.y + dy;

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
						{
							nowScore += abs(table[py][px]);
							nextScore += abs(table[py][px] + 1);
						}
					}
				}
			}

			break;
			//左
		case 1:

			for (int dy = -r; dy <= r; dy++)
			{
				for (int dx = -r; dx <= 0; dx++)
				{
					int px = pos.x + dx + directionX[direction];
					int py = pos.y + dy + directionY[direction];

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
						{
							nowScore += abs(table[py][px]);
							nextScore += abs(table[py][px] - 1);
						}
					}
				}
			}

			for (int dy = -r; dy <= r; dy++)
			{
				for (int dx = 0; dx <= r; dx++)
				{
					int px = pos.x + dx;
					int py = pos.y + dy;

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
						{
							nowScore += abs(table[py][px]);
							nextScore += abs(table[py][px] + 1);
						}
					}
				}
			}

			break;
			//右
		case 2:

			for (int dy = -r; dy <= r; dy++)
			{
				for (int dx = -r; dx <= 0; dx++)
				{
					int px = pos.x + dx;
					int py = pos.y + dy;

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
						{
							nowScore += abs(table[py][px]);
							nextScore += abs(table[py][px] + 1);
						}
					}
				}
			}

			for (int dy = -r; dy <= r; dy++)
			{
				for (int dx = 0; dx <= r; dx++)
				{
					int px = pos.x + dx + directionX[direction];
					int py = pos.y + dy + directionY[direction];

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
						{
							nowScore += abs(table[py][px]);
							nextScore += abs(table[py][px] - 1);
						}
					}
				}
			}

			break;
			//下
		case 3:

			for (int dy = -r; dy <= 0; dy++)
			{
				for (int dx = -r; dx <= r; dx++)
				{
					int px = pos.x + dx;
					int py = pos.y + dy;

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
						{
							nowScore += abs(table[py][px]);
							nextScore += abs(table[py][px] + 1);
						}
					}
				}
			}

			for (int dy = 0; dy <= r; dy++)
			{
				for (int dx = -r; dx <= r; dx++)
				{
					int px = pos.x + dx + directionX[direction];
					int py = pos.y + dy + directionY[direction];

					if (inside(px) && inside(py))
					{
						if (abs(dx) + abs(dy) <= r)
						{
							nowScore += abs(table[py][px]);
							nextScore += abs(table[py][px] - 1);
						}
					}
				}
			}

			break;
		}

		return nowScore - nextScore;
	}

	int incScore(const Point& pos, const int h, const Grid& table) const {

		if (pos.h + h < 1 || pos.h + h > 100) return Inf;

		const int r = pos.h + h - 1;
		int nowScore = 0;
		int nextScore = 0;

		for (int dy = -r; dy <= r; dy++)
		{
			for (int dx = -r; dx <= r; dx++)
			{
				int px = pos.x + dx;
				int py = pos.y + dy;

				if (inside(px) && inside(py))
				{
					if (abs(dx) + abs(dy) <= r)
					{
						nowScore += abs(table[py][px]);
						nextScore += abs(table[py][px] - h);
					}
				}
			}
		}

		return nowScore - nextScore;
	}

public:

	Answer think(const Grid& grid) {

		Answer ans;

		XorShift random;

		for (auto& p : ans)
		{
			p.x = random.rand() % 100;
			p.y = random.rand() % 100;
			p.h = random.rand() % 100 + 1;
		}

		Grid table = grid;

		for (const auto& pos : ans)
		{
			sub(pos, table);
		}

		int score = result(table);
		int bestScore = score;
		Answer best = ans;

		Timer timer(chrono::milliseconds(this->T));

		int count = 0;

		timer.start();
		while (!timer)
		{
			const auto diff = timer.diff();
			count += 100;

			for (int i = 0; i < 100; i++)
			{
				const int index = random.rand() % 1000;

				const int patt = random.rand() % 6;

				int diffScore;

				if (patt < 4)
				{
					diffScore = moveScore(ans[index], patt, table);

					if (diffScore != Inf && probability(diffScore, diff))
					{
						move(ans[index], patt, table);

						score += diffScore;
						if (bestScore > score)
						{
							best = ans;
							bestScore = score;
						}

					}
				}
				else
				{
					const int h = (patt == 4 ? -1 : 1);
					diffScore = incScore(ans[index], h, table);

					if (diffScore != Inf && probability(diffScore, diff))
					{
						inc(ans[index], h, table);

						score += diffScore;
						if (bestScore > score)
						{
							best = ans;
							bestScore = score;
						}

					}
				}

			}

		}

		//cerr << count << endl;

		return best;
	}

};

int main() {

	Grid input;

	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			cin >> input[y][x];
		}
	}

	Annealing ai;

	const auto& ans = ai.think(input);

	cout << ans.size() << endl;

	for (const auto& p : ans)
	{
		cout << p.x << " " << p.y << " " << p.h << endl;
	}

	return 0;
}
