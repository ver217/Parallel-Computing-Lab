#include <iostream>
#include <vector>
using namespace std;

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

int main() {
    int n,  m;
    cin >> n >> m;
    vector<vector<int>> cs = comb(n, m);
    for (auto arr : cs) {
        for (auto elem : arr)
            cout << elem << ' ';
        cout << endl;
    }
}