#include <gtest/gtest.h>
#include "AStar.h"
#include <algorithm>
#include <cstdlib>

// ─── 辅助函数 ────────────────────────────────────────────────────────────────

static std::vector<std::vector<int>> MakeGrid(int rows, int cols) {
    return std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
}

// 检查路径中每对相邻节点是否四向相邻（无跳跃）
static bool IsContiguous(const std::vector<Vec2>& path) {
    for (size_t i = 1; i < path.size(); ++i) {
        int dx = std::abs(path[i].x - path[i-1].x);
        int dy = std::abs(path[i].y - path[i-1].y);
        if (dx + dy != 1) return false;
    }
    return true;
}

static bool Contains(const std::vector<Vec2>& path, Vec2 v) {
    return std::any_of(path.begin(), path.end(),
                       [&](const Vec2& p){ return p == v; });
}

// ─── Group 1: 基本正确性 ──────────────────────────────────────────────────────

// 空旷地图能找到路径；首元素是终点（路径反向存储：[goal, ..., first_step]）
TEST(AStarTest, FindsPathInOpenGrid) {
    auto grid = MakeGrid(10, 10);
    auto path = FindPath({0,0}, {9,9}, 10, 10, grid);
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.front(), (Vec2{9, 9}));
}

// 起点等于终点，无需移动，返回空路径
TEST(AStarTest, StartEqualsGoal) {
    auto grid = MakeGrid(5, 5);
    auto path = FindPath({2,2}, {2,2}, 5, 5, grid);
    EXPECT_TRUE(path.empty());
}

// 整列墙壁切断地图（x=2 全封），无路可走
TEST(AStarTest, FullyBlockedReturnsEmpty) {
    auto grid = MakeGrid(5, 5);
    for (int r = 0; r < 5; ++r) grid[r][2] = 1;
    auto path = FindPath({0,2}, {4,2}, 5, 5, grid);
    EXPECT_TRUE(path.empty());
}

// 部分墙迫使绕行，路径不经过任何障碍格
TEST(AStarTest, PathAvoidsWall) {
    auto grid = MakeGrid(5, 5);
    grid[0][1] = 1;   // (x=1,y=0)
    grid[1][1] = 1;   // (x=1,y=1)，y>=2 处有绕行缺口
    auto path = FindPath({0,0}, {2,0}, 5, 5, grid);
    EXPECT_FALSE(path.empty());
    for (const auto& p : path) {
        EXPECT_EQ(grid[p.y][p.x], 0)
            << "路径经过了障碍格 (" << p.x << "," << p.y << ")";
    }
}

// ─── Group 2: 路径结构不变量 ──────────────────────────────────────────────────

// 找到路径时 path.front() 必须等于终点
TEST(AStarStructureTest, PathFrontIsGoal) {
    auto grid = MakeGrid(6, 6);
    Vec2 goal{5, 3};
    auto path = FindPath({0,0}, goal, 6, 6, grid);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), goal);
}

// 路径不包含起点（起点不加入 cameFrom，因此不会出现在重建结果里）
TEST(AStarStructureTest, PathExcludesStart) {
    auto grid = MakeGrid(5, 5);
    Vec2 start{0, 0};
    auto path = FindPath(start, {4,4}, 5, 5, grid);
    ASSERT_FALSE(path.empty());
    EXPECT_FALSE(Contains(path, start));
}

// 即使有障碍绕行，路径中每对相邻节点仍应四向相邻
TEST(AStarStructureTest, PathIsContiguous) {
    auto grid = MakeGrid(8, 8);
    grid[2][3] = 1;
    grid[3][3] = 1;
    grid[4][3] = 1;   // 垂直障碍，迫使绕行
    auto path = FindPath({0,0}, {7,7}, 8, 8, grid);
    ASSERT_FALSE(path.empty());
    EXPECT_TRUE(IsContiguous(path));
}

// 空旷地图中 path.size() == 曼哈顿距离（最优路径验证）
// 路径不含起点，因此 size == 步数 == 曼哈顿距离
TEST(AStarStructureTest, PathLengthEqualsManhattanDistance) {
    auto grid = MakeGrid(10, 10);
    Vec2 start{1, 1}, goal{7, 5};
    int expected = std::abs(goal.x - start.x) + std::abs(goal.y - start.y);
    auto path = FindPath(start, goal, 10, 10, grid);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(static_cast<int>(path.size()), expected);
}

// ─── Group 3: 边界与特殊场景 ──────────────────────────────────────────────────

// 起点与终点相邻（1 步）→ 路径长度为 1，唯一节点是终点
TEST(AStarEdgeCaseTest, AdjacentStartAndGoal) {
    auto grid = MakeGrid(3, 3);
    auto path = FindPath({0,0}, {1,0}, 3, 3, grid);
    ASSERT_EQ(path.size(), 1u);
    EXPECT_EQ(path.front(), (Vec2{1, 0}));
}

// 终点本身是墙（grid[y][x]==1），永远无法进入，应返回空
TEST(AStarEdgeCaseTest, GoalIsWall) {
    auto grid = MakeGrid(5, 5);
    grid[4][4] = 1;
    auto path = FindPath({0,0}, {4,4}, 5, 5, grid);
    EXPECT_TRUE(path.empty());
}

// 终点被四面墙包围（自身可通行，但所有邻格是墙），无法到达
TEST(AStarEdgeCaseTest, IslandGoal) {
    auto grid = MakeGrid(5, 5);
    grid[1][2] = 1;   // 上 (x=2,y=1)
    grid[3][2] = 1;   // 下 (x=2,y=3)
    grid[2][1] = 1;   // 左 (x=1,y=2)
    grid[2][3] = 1;   // 右 (x=3,y=2)
    auto path = FindPath({0,0}, {2,2}, 5, 5, grid);
    EXPECT_TRUE(path.empty());
}

// 宽度为 1 的走廊，只有 y=1 这一行可通行
TEST(AStarEdgeCaseTest, NarrowCorridor) {
    // 布局（rows=3, cols=7）：
    //   # # # # # # #   (y=0 全堵)
    //   . . . . . . .   (y=1 唯一通道)
    //   # # # # # # #   (y=2 全堵)
    auto grid = MakeGrid(3, 7);
    for (int c = 0; c < 7; ++c) { grid[0][c] = 1; grid[2][c] = 1; }
    auto path = FindPath({0,1}, {6,1}, 7, 3, grid);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), (Vec2{6, 1}));
    for (const auto& p : path)
        EXPECT_EQ(p.y, 1) << "走廊路径偏离了 y=1，节点 (" << p.x << "," << p.y << ")";
}

// 单行地图（gridH=1）的水平直线路径
TEST(AStarEdgeCaseTest, SingleRowPath) {
    auto grid = MakeGrid(1, 6);
    auto path = FindPath({0,0}, {5,0}, 6, 1, grid);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), (Vec2{5, 0}));
    EXPECT_EQ(static_cast<int>(path.size()), 5);
}

// 50×50 大地图不应崩溃，路径应存在且终点正确
TEST(AStarEdgeCaseTest, LargeOpenGrid) {
    auto grid = MakeGrid(50, 50);
    auto path = FindPath({0,0}, {49,49}, 50, 50, grid);
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.front(), (Vec2{49, 49}));
}
