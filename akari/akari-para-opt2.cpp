#include "akari.h"
#include <algorithm>
#include <time.h>
#include <iostream>
#include <thread>
#include <functional>
using namespace std;


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

inline bool is_num_cell(int val);
inline bool can_light_up(int val);
inline int count_neighbor(const vector<vector<int>>& g, int y, int x, int val);
inline bool has_numcell_neighbor(const vector<vector<int>>& g, int y, int x);
inline void light_up(vector<vector<int>>& g, int y, int x);
inline void unlight_up(vector<vector<int>>& g, int y, int x);
vector<NumCell> get_blank_cells(const vector<vector<int>>& g);
inline bool neighbor_valid(const vector<vector<int>>& g, const NumCell& bulb, const NumCell& center);
bool handle_left_seq(vector<vector<int>>& g, const vector<NumCell>& blank_cells, const int level);
bool akari_para(vector<vector<int>>& g, const vector<NumCell>& all_num_cells, const int level);
void akari_task(vector<vector<int>>& g, const vector<NumCell>& all_num_cells, const int level, const vector<NumCell>& cells_avail, const vector<int>&one_comb, vector<bool>& valid, int i);

vector<vector<int> > solveAkari(vector<vector<int> > & g) {
    clock_t start = clock();
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
            }
        }
    }
    sort(all_num_cells.rbegin(), all_num_cells.rend());
    if (akari_para(g, all_num_cells, 0)) {
        for (int i = 0; i < g.size(); i++) {
            for (int j = 0; j < g[i].size(); j ++) {
                if (g[i][j] < -2 || g[i][j] >= 6)
                    g[i][j] = -2;
            }
        }
    }
    cout << "time: " << ((double)(clock() - start) / CLOCKS_PER_SEC * 1000) << endl;
    return g;
}

bool akari_para(vector<vector<int>>& g, const vector<NumCell>& all_num_cells, const int level) {
    if (level >= all_num_cells.size()) {
        vector<NumCell> blank_cells = get_blank_cells(g);
        if (blank_cells.empty())
            return true;
        sort(blank_cells.begin(), blank_cells.end());
        return handle_left_seq(g, blank_cells, 0);
    }
    const NumCell& cell = all_num_cells[level];
    const int set_light_num = cell.num - count_neighbor(g, cell.y, cell.x, 5);
    if (set_light_num == 0)
        return akari_para(g, all_num_cells, level + 1);
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
    if (cell.num == 3) {
        vector<vector<vector<int>>> gs(combs.size(), g);
        vector<bool> success(combs.size(), false);
        vector<thread> threads(combs.size());
        for (int i = 0; i < combs.size(); i++)
            threads[i] = thread(akari_task, ref(gs[i]), cref(all_num_cells), level, cref(cells_avail), cref(combs[i]), ref(success), i);
        for (int i = 0; i < combs.size(); i++)
            threads[i].join();
        for (int i = 0; i < combs.size(); i++) {
            if (success[i]) {
                g = gs[i];
                return true;
            }
        }
    } else {
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
            if (akari_para(g, all_num_cells, level + 1))
                return true;
            else {
                for (int idx : one_comb)
                    unlight_up(g, cells_avail[idx].y, cells_avail[idx].x);
            }
        }
    }
    return false;
}

void akari_task(vector<vector<int>>& g, const vector<NumCell>& all_num_cells, const int level, const vector<NumCell>& cells_avail, const vector<int>&one_comb, vector<bool>& valid, int i) {
    // valid default false
    const NumCell& cell = all_num_cells[level];
    for (int idx : one_comb) {
        light_up(g, cells_avail[idx].y, cells_avail[idx].x);
        if (!neighbor_valid(g, cells_avail[idx], cell))
            return;
    }
    if (akari_para(g, all_num_cells, level + 1))
        valid[i] = true;
    return;
}


bool handle_left_seq(vector<vector<int>>& g, const vector<NumCell>& blank_cells, const int level) {
    for (int i = level; i < blank_cells.size(); i++) {
        if (g[blank_cells[i].y][blank_cells[i].x] == 6)
            break;
        if (g[blank_cells[i].y][blank_cells[i].x] != -2 || has_numcell_neighbor(g, blank_cells[i].y, blank_cells[i].x))
            continue;
        light_up(g, blank_cells[i].y, blank_cells[i].x);
        bool finish = true;
        bool has_empty_cell = false;
        for (int j = 0; j < blank_cells.size(); j++) {
            if (g[blank_cells[j].y][blank_cells[j].x] == -2) {
                finish = false;
                has_empty_cell = true;
                break;
            } else if (g[blank_cells[j].y][blank_cells[j].x] == 6)
                finish = false;
        }
        if (finish)
            return true;
        if (!has_empty_cell) {
            unlight_up(g, blank_cells[i].y, blank_cells[i].x);
            continue;
        }
        if (handle_left_seq(g, blank_cells, i + 1))
            return true;
        else
            unlight_up(g, blank_cells[i].y, blank_cells[i].x);
    }
    return false;
}

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

}
