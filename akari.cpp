#include "akari.h"
#include <queue>
using namespace std;

/*
输入数据由若干文件组成，每个文件描述一个Akari问题的初始状态，编写程序读入此文件，根据初始状态求解。有的有一个解，有的有多个解，我们保证有解。

文件由若干行组成，第一行为两个整数 n，m，代表棋盘的行数和列数。之后的 n 行每行有 m 个整数表示棋盘的每个格子的状态，若它为 -2，则表示是白格子，若它为 -1，则表示是没有数字的黑格子，若它为 0-4，则表示是数字 0-4 的黑格子。若你想把灯泡放在白色格子上面，则需要将 -2 改为 5，因为 5 表示有灯泡的格子。

你需要在右侧代码编辑框给出的函数中编写你的代码，函数的参数为我们给出的light up矩阵，你需要在该函数中返回相同大小的结果矩阵。对于有多个解的light up，你可以返回其中的任意一组解，我们将对你返回的矩阵进行检查，若结果正确，提示The answer is right!，否则提示其它。

6表示被点亮
<-2表示被点亮
 */

/*

7 7
-2 1 -2 -2 -2 -2 -2
-2 -2 3 -2 -2 -2 0
-2 -2 -2 -2 -2 1 -2
-2 -2 -2 -1 -2 -2 -2
-2 1 -2 -2 -2 -2 -2
0 -2 -2 -2 2 -2 -2
-2 -2 -2 -2 -2 0 -2 */

/*
7 7
-2 1 -2 -2 -2 -2 -2
-2 -2 -2 -2 -2 -2 1
-2 -2 1 -2 4 -2 -2
-2 -2 -2 -2 -2 -2 -2
-2 -2 0 -2 -1 -2 -2
1 -2 -2 -2 -2 -2 -2
-2 -2 -2 -2 -2 0 -2  */

/*
7 7
-2 -2 -2 -2 -1 -2 -2
-2 2 -2 -2 -2 4 -2
-1 -2 -2 -1 -2 -2 -2
-2 -2 2 -1 1 -2 -2
-2 -2 -2 -1 -2 -2 1
-2 2 -2 -2 -2 -1 -2
-2 -2 2 -2 -2 -2 -2  */

/*
10 10
2 -2 -2 -2 2 -2 -2 -2 -2 -1
-2 -1 -2 -2 -2 -2 -2 -2 -1 -2
-2 -2 -2 2 -2 -1 -2 -2 -2 -2
-2 -2 -2 -2 -2 -2 -2 2 -2 -2
-2 -2 2 -2 -2 -2 -2 -2 -2 1
-1 -2 -2 -2 -2 -2 -2 0 -2 -2
-2 -2 2 -2 -2 -2 -2 -2 -2 -2
-2 -2 -2 -2 1 -2 -1 -2 -2 -2
-2 -1 -2 -2 -2 -2 -2 -2 1 -2
1 -2 -2 -2 -2 1 -2 -2 -2 0  */


class NumCell {
  public:
    int num;
    int y;
    int x;
    NumCell() = default;
    NumCell(int num, int y, int x) : num(num), y(y), x(x) {}
    friend bool operator<(const NumCell& a, const NumCell& b);
};

inline bool operator<(const NumCell& a, const NumCell& b) {
    return a.num < b.num;
}

vector<vector<int>> comb(int n, int m) {
    vector<vector<int>> res;
    vector<int> arr(m);
    for (int i = 0; i < m; i++)
        arr[i] = i;
    while (1) {
        res.push_back(arr);
        int j = m - 1;
        while (arr[j] == n - m + j)
            j--;
        if (j < 0)
            break;
        arr[j]++;
        for (int i = j + 1; i < m; i++)
            arr[i] = arr[i - 1] + 1;
    }
    return res;
}

namespace aka {

inline int count_neighbor(const vector<vector<int>>& g, int y, int x, int neighbor) {
    int cnt = 0;
    cnt += y > 0 ? g[y - 1][x] == neighbor : 0;
    cnt += y < g.size() - 1 ? g[y + 1][x] == neighbor : 0;
    cnt += x > 0 ? g[y][x - 1] == neighbor : 0;
    cnt += x < g[y].size() - 1 ? g[y][x + 1] == neighbor : 0;
    return cnt;
}

inline void light_up(vector<vector<int>>& g, int y, int x) {
    g[y][x] = 5;
    for (int i = x - 1; i >= 0; i--) {
        if (-1 <= g[y][i] && g[y][i] <= 4)
            break;
        g[y][i] = 6;
    }
    for (int i = x + 1; i < g[y].size(); i++) {
        if (-1 <= g[y][i] && g[y][i] <= 4)
            break;
        g[y][i] = 6;
    }
    for (int i = y - 1; i >= 0; i--) {
        if (-1 <= g[i][x] && g[i][x] <= 4)
            break;
        g[i][x] = 6;
    }
    for (int i = y + 1; i < g.size(); i++) {
        if (-1 <= g[i][x] && g[i][x] <= 4)
            break;
        g[i][x] = 6;
    }
}

vector<NumCell> get_blank_cells(const vector<vector<int>>& g) {
    vector<NumCell> blank_cells;
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[i].size(); j ++) {
            if (g[i][j] == -2)
                blank_cells.push_back(NumCell(-2, i, j));
        }
    }
    return blank_cells;
}

bool handle_left_seq(vector<vector<int>>& g, const vector<NumCell>& blank_cells, const vector<NumCell>& all_num_cells) {
    if (blank_cells.empty())
        return true;
    vector<vector<int>> prev_g = g;
    for (int i = 0; i < blank_cells.size(); i++) {
        light_up(g, blank_cells[i].y, blank_cells[i].x);
        bool valid = true;
        for (const NumCell& cell : all_num_cells) {
            if (count_neighbor(g, cell.y, cell.x, 5) > cell.num) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            g = prev_g;
            continue;
        }
        vector<NumCell> new_blank_cells = get_blank_cells(g);
        if (handle_left_seq(g, new_blank_cells, all_num_cells))
            return true;
        else
            g = prev_g;
    }
    return false;
}

bool akari_seq(vector<vector<int>>& g, priority_queue<NumCell> num_cells, const vector<NumCell>& all_num_cells) {
    for (const NumCell& cell : all_num_cells) {
        if (count_neighbor(g, cell.y, cell.x, 5) > cell.num)
            return false;
    }
    if (num_cells.empty())
        return handle_left_seq(g, get_blank_cells(g), all_num_cells);
    NumCell cell = num_cells.top();
    num_cells.pop();
    const int set_light_num = cell.num - count_neighbor(g, cell.y, cell.x, 5);
    if (cell.num == 0 || set_light_num == 0)
        return akari_seq(g, num_cells, all_num_cells);
    vector<vector<int>> prev_g = g;
    vector<NumCell> cells_avail;
    if (cell.y > 0 && g[cell.y - 1][cell.x] == -2)
        cells_avail.push_back(NumCell(-2, cell.y - 1, cell.x));
    if (cell.y < g.size() - 1 && g[cell.y + 1][cell.x] == -2)
        cells_avail.push_back(NumCell(-2, cell.y + 1, cell.x));
    if (cell.x > 0 && g[cell.y][cell.x - 1] == -2)
        cells_avail.push_back(NumCell(-2, cell.y, cell.x - 1));
    if (cell.x < g[cell.y].size() - 1 && g[cell.y][cell.x + 1] == -2)
        cells_avail.push_back(NumCell(-2, cell.y, cell.x + 1));
    if (cells_avail.size() < set_light_num)
        return false;
    vector<vector<int>> combs = comb(cells_avail.size(), set_light_num);
    for (const vector<int>& one_comb : combs) {
        for (int idx : one_comb)
            light_up(g, cells_avail[idx].y, cells_avail[idx].x);
        if (akari_seq(g, num_cells, all_num_cells))
            return true;
        else
            g = prev_g;
    }
    return false;
}

vector<vector<int> > solveAkari(vector<vector<int> > & g) {
    priority_queue<NumCell> num_cells;
    vector<NumCell> all_num_cells;
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[i].size(); j++) {
            if (0 <= g[i][j] && g[i][j] <= 4) {
                NumCell cell(g[i][j], i, j);
                num_cells.push(cell);
                all_num_cells.push_back(cell);
            }
        }
    }
    if (akari_seq(g, num_cells, all_num_cells)) {
        for (int i = 0; i < g.size(); i++) {
            for (int j = 0; j < g[i].size(); j ++) {
                if (g[i][j] == 6)
                    g[i][j] = -2;
            }
        }
    }
    return g;
}

}

/*
function transfer(dom) {
    let divs = dom.children;
    let row = Math.sqrt(divs.length - 1);
    let cnt = row;
    let total = `${row} ${row}`;
    let line = '';
    for (let i = 1; i < divs.length; i++) {
        if (divs[i].classList.contains('wall'))
            line += '-1 ';
        else if (divs[i].classList.contains('cell-off'))
            line += '-2 ';
        else if (divs[i].classList.contains('light-up-task-cell'))
            line += divs[i].innerText + ' ';
        if (--cnt == 0) {
            total += `\n${line}`;
            line = '';
            cnt = row;
        }
    }
    return total;
} */