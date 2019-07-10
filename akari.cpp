#include "akari.h"
#include <algorithm>
#include <time.h>
#include <iostream>
using namespace std;

/*
输入数据由若干文件组成，每个文件描述一个Akari问题的初始状态，编写程序读入此文件，根据初始状态求解。有的有一个解，有的有多个解，我们保证有解。

文件由若干行组成，第一行为两个整数 n，m，代表棋盘的行数和列数。之后的 n 行每行有 m 个整数表示棋盘的每个格子的状态，若它为 -2，则表示是白格子，若它为 -1，则表示是没有数字的黑格子，若它为 0-4，则表示是数字 0-4 的黑格子。若你想把灯泡放在白色格子上面，则需要将 -2 改为 5，因为 5 表示有灯泡的格子。

你需要在右侧代码编辑框给出的函数中编写你的代码，函数的参数为我们给出的light up矩阵，你需要在该函数中返回相同大小的结果矩阵。对于有多个解的light up，你可以返回其中的任意一组解，我们将对你返回的矩阵进行检查，若结果正确，提示The answer is right!，否则提示其它。

<-2 || >6表示被点亮
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

int call_cnt = 0;
int call_cnt2 = 0;

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

inline bool is_num_cell(int val) {
    return 0 <= val && val <= 4;
}

inline bool can_light_up(int val) {
    return val <= -2 || val >= 6;
}

inline int count_neighbor(const vector<vector<int>>& g, int y, int x, int val) {
    int cnt = 0;
    cnt += y > 0 ? g[y - 1][x] == val : 0;
    cnt += y < g.size() - 1 ? g[y + 1][x] == val : 0;
    cnt += x > 0 ? g[y][x - 1] == val : 0;
    cnt += x < g[0].size() - 1 ? g[y][x + 1] == val : 0;
    return cnt;
}

inline bool has_numcell_neighbor(const vector<vector<int>>& g, int y, int x) {
    if (y > 0 && is_num_cell(g[y - 1][x]))
        return true;
    if (y < g.size() - 1 &&  is_num_cell(g[y + 1][x]))
        return true;
    if (x > 0 && is_num_cell(g[y][x - 1]))
        return true;
    if (x < g[0].size() - 1 && is_num_cell(g[y][x + 1]))
        return true;
    return false;
}

inline void light_up(vector<vector<int>>& g, int y, int x) {
    g[y][x] = 5;
    for (int i = x - 1; i >= 0; i--) {
        if (!can_light_up(g[y][i]))
            break;
        else if (g[y][i] <= -2)
            g[y][i]--;
        else
            g[y][i]++;
    }
    for (int i = x + 1; i < g[y].size(); i++) {
        if (!can_light_up(g[y][i]))
            break;
        else if (g[y][i] <= -2)
            g[y][i]--;
        else
            g[y][i]++;
    }
    for (int i = y - 1; i >= 0; i--) {
        if (!can_light_up(g[i][x]))
            break;
        else if (g[i][x] <= -2)
            g[i][x]--;
        else
            g[i][x]++;
    }
    for (int i = y + 1; i < g.size(); i++) {
        if (!can_light_up(g[i][x]))
            break;
        else if (g[i][x] <= -2)
            g[i][x]--;
        else
            g[i][x]++;
    }
}

inline void unlight_up(vector<vector<int>>& g, int y, int x) {
    g[y][x] = -2;
    for (int i = x - 1; i >= 0; i--) {
        if (!can_light_up(g[y][i]))
            break;
        else if (g[y][i] <= -2)
            g[y][i]++;
        else
            g[y][i]--;
    }
    for (int i = x + 1; i < g[y].size(); i++) {
        if (!can_light_up(g[y][i]))
            break;
        else if (g[y][i] <= -2)
            g[y][i]++;
        else
            g[y][i]--;
    }
    for (int i = y - 1; i >= 0; i--) {
        if (!can_light_up(g[i][x]))
            break;
        else if (g[i][x] <= -2)
            g[i][x]++;
        else
            g[i][x]--;
    }
    for (int i = y + 1; i < g.size(); i++) {
        if (!can_light_up(g[i][x]))
            break;
        else if (g[i][x] <= -2)
            g[i][x]++;
        else
            g[i][x]--;
    }
}

vector<NumCell> get_blank_cells(const vector<vector<int>>& g) {
    vector<NumCell> blank_cells;
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[0].size(); j ++) {
            if (g[i][j] == -2 || g[i][j] == 6)
                blank_cells.push_back(NumCell(g[i][j], i, j));
        }
    }
    return blank_cells;
}

inline bool all_shine(const vector<vector<int>>& g) {
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[0].size(); j ++) {
            if (g[i][j] == 6)
                return false;
        }
    }
    return true;
}

inline bool neighbor_valid(const vector<vector<int>>& g, const NumCell& bulb, const NumCell& center) {
    if (bulb.x > 0 && bulb.x - 1 != center.x && is_num_cell(g[bulb.y][bulb.x - 1]) && count_neighbor(g, bulb.y, bulb.x - 1, 5) > g[bulb.y][bulb.x - 1])
        return false;
    if (bulb.x + 1 < g[0].size() && bulb.x + 1 != center.x && is_num_cell(g[bulb.y][bulb.x + 1]) && count_neighbor(g, bulb.y, bulb.x + 1, 5) > g[bulb.y][bulb.x + 1])
        return false;
    if (bulb.y > 0 && bulb.y - 1 != center.y && is_num_cell(g[bulb.y - 1][bulb.x]) && count_neighbor(g, bulb.y - 1, bulb.x, 5) > g[bulb.y - 1][bulb.x])
        return false;
    if (bulb.y + 1 < g.size() && bulb.y + 1 != center.y && is_num_cell(g[bulb.y + 1][bulb.x]) && count_neighbor(g, bulb.y + 1, bulb.x, 5) > g[bulb.y + 1][bulb.x])
        return false;
    return true;
}

bool handle_left_seq(vector<vector<int>>& g, const vector<NumCell>& blank_cells) {
    // displayAkari(g);
    cout << "left: " << ++call_cnt2 << "\r" << flush;
    for (int i = 0; i < blank_cells.size(); i++) {
        if (blank_cells[i].num == 6 || has_numcell_neighbor(g, blank_cells[i].y, blank_cells[i].x))
            continue;
        light_up(g, blank_cells[i].y, blank_cells[i].x);
        vector<NumCell> new_blank_cells;
        new_blank_cells.reserve(blank_cells.size() - 1);
        for (int j = 0; j < blank_cells.size(); j++) {
            if (j == i)
                continue;
            if (g[blank_cells[j].y][blank_cells[j].x] == -2 || g[blank_cells[j].y][blank_cells[j].x] == 6)
                new_blank_cells.push_back(blank_cells[j]);
        }
        if (new_blank_cells.empty())
            return true;
        if (new_blank_cells[0].num == 6) {
            unlight_up(g, blank_cells[i].y, blank_cells[i].x);
            continue;
        }
        if (handle_left_seq(g, new_blank_cells))
            return true;
        else
            unlight_up(g, blank_cells[i].y, blank_cells[i].x);
    }
    return false;
}

bool akari_seq(vector<vector<int>>& g, const vector<NumCell>& all_num_cells, const int level) {
    cout << "akari: " << ++call_cnt <<  endl;
    if (level >= all_num_cells.size()) {
        vector<NumCell> blank_cells = get_blank_cells(g);
        if (blank_cells.empty())
            return true;
        sort(blank_cells.begin(), blank_cells.end());
        call_cnt2 = 0;
        return handle_left_seq(g, blank_cells);
    }
    const NumCell& cell = all_num_cells[level];
    const int set_light_num = cell.num - count_neighbor(g, cell.y, cell.x, 5);
    if (set_light_num == 0)
        return akari_seq(g, all_num_cells, level + 1);
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
        bool valid = true;
        for (int idx : one_comb) {
            light_up(g, cells_avail[idx].y, cells_avail[idx].x);
            if (!neighbor_valid(g, cells_avail[idx], cell)) {
                unlight_up(g, cells_avail[idx].y, cells_avail[idx].x);
                valid = false;
                break;
            }
        }
        if (!valid)
            continue;
        if (akari_seq(g, all_num_cells, level + 1))
            return true;
        else {
            for (int idx : one_comb)
                unlight_up(g, cells_avail[idx].y, cells_avail[idx].x);
        }
    }
    return false;
}

vector<vector<int> > solveAkari(vector<vector<int> > & g) {
    vector<NumCell> all_num_cells;
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[0].size(); j++) {
            if (g[i][j] == 0) {
                if (j > 0 && g[i][j - 1] == -2)
                    g[i][j - 1] = 6;
                if (j + 1 < g[0].size() && g[i][j + 1] == -2)
                    g[i][j + 1] = 6;
                if (i > 0 && g[i - 1][j] == -2)
                    g[i - 1][j] = 6;
                if (i + 1 < g.size() && g[i + 1][j] == -2)
                    g[i + 1][j] = 6;
            } else if (is_num_cell(g[i][j])) {
                NumCell cell(g[i][j], i, j);
                all_num_cells.push_back(cell);
            }
        }
    }
    bool greedy = true;
    while (greedy) {
        greedy = false;
        for (vector<NumCell>::iterator iter = all_num_cells.begin(); iter != all_num_cells.end(); iter++) {
            const NumCell& cell = *iter;
            const int set_light_num = cell.num - count_neighbor(g, cell.y, cell.x, 5);
            const int avail_num = count_neighbor(g, cell.y, cell.x, -2);
            if (set_light_num == 0) {
                all_num_cells.erase(iter--);
                continue;
            }
            if (avail_num < set_light_num)
                return g;
            else if (avail_num == set_light_num) {
                if (cell.y > 0 && g[cell.y - 1][cell.x] == -2)
                    light_up(g, cell.y - 1, cell.x);
                if (cell.y < g.size() - 1 && g[cell.y + 1][cell.x] == -2)
                    light_up(g, cell.y + 1, cell.x);
                if (cell.x > 0 && g[cell.y][cell.x - 1] == -2)
                    light_up(g, cell.y, cell.x - 1);
                if (cell.x < g[cell.y].size() - 1 && g[cell.y][cell.x + 1] == -2)
                    light_up(g, cell.y, cell.x + 1);
                greedy = true;
                all_num_cells.erase(iter--);
                continue;
            }
        }
    }
    displayAkari(g);
    sort(all_num_cells.rbegin(), all_num_cells.rend());
    clock_t start = clock();
    if (akari_seq(g, all_num_cells, 0)) {
        displayAkari(g);
        for (int i = 0; i < g.size(); i++) {
            for (int j = 0; j < g[i].size(); j ++) {
                if (g[i][j] < -2 || g[i][j] >= 6)
                    g[i][j] = -2;
            }
        }
    } else
        cout << "gg" << endl;
    cout << "time: " << ((double)(clock() - start) / CLOCKS_PER_SEC) << endl;
    return g;
}

}

/*
function transfer() {
    const dom = document.querySelector('.board-back');
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