// Fuente del algoritmo: https://github.com/Dev-XYS/Algorithms/blob/master/Matrix-Multiplication(Naive).cpp
//
// Modificacion respecto al original: el archivo original usaba matrices
// globales de tamano fijo (int A[100][100]), lo que produce memoria
// corrupta (undefined behavior) para los tamanos que pide el enunciado
// (n = 256 y n = 1024, ya que 1024 > 100). Se reemplazo por vector<vector<int>>
// dimensionado dinamicamente segun el tamano real de las matrices de entrada.
// La logica de la multiplicacion (triple for) es la misma del original.

#include <iostream>
#include <vector>
using namespace std;

// Multiplica A (m x p) por B (p x n) y retorna la matriz resultado (m x n)
vector<vector<int>> matrix_multiplication(const vector<vector<int>>& A,
                                           const vector<vector<int>>& B) {
    int m = A.size();
    int p = A[0].size();
    int n = B[0].size();

    vector<vector<int>> R(m, vector<int>(n, 0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < p; k++) {
                R[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return R;
}