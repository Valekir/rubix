#include <iostream>


int main() {
    // Транспонирование
    int n = 3;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                printf("1: (%d;%d) (%d;%d)\n", i, j, j, i);
                // swap(parts[i][j], parts[j][i]);
            }
        }
// Отражение по вертикали
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                printf("2: (%d;%d) (%d;%d)\n", i, j, i, n-j-1);
                // swap(parts[i][j], parts[i][n-j-1]);
            }
        }
// Поворот всех частей
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("3: (%d;%d)\n", i, j);
                // parts[i][j].rotate_piece('Z');
            }
        }
}
