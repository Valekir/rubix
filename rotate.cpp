#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

void print_matrix(vector<vector <int>>& matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

template <typename T>
void reflect_vert(vector<vector <T>>& matrix, int n) {
    for (int i = 0; i < n / 2; ++i) {
        for (int j = 0; j < n; ++j) {
            swap(matrix[i][j], matrix[n - 1 - i][j]);
        }
    }
}
template <typename T>
void reflect_hor(vector<vector<T>>& matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n / 2; ++j) {
            swap(matrix[i][j], matrix[i][n - 1 - j]);
        }
    }
}

int main() {
    vector <vector <int>> matrix;
    int n;
    char command;
    cin >> n;
    vector <int> temp;
    for (int i = 0; i < n; i++)  {
        for (int j = 0; j < n; j++) {
            temp.push_back(3*i+j+1);
        }
        matrix.push_back(temp);
        temp.clear();
    }
    cout << "Original:" << endl;
    print_matrix(matrix, n);

    while (cin >> command) {
        if (command == 'H')
            reflect_hor(matrix, n);
        else if (command == 'V')
            reflect_vert(matrix, n);
        else
            break;
        print_matrix(matrix, n);
        cout << '\n';
    }

    return 0;
}
