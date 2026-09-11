#ifndef NAIVE_HPP
#define NAIVE_HPP

// Fuente del algoritmo: https://github.com/Dev-XYS/Algorithms/blob/master/Matrix-Multiplication(Naive).cpp
//
// Modificacion respecto al original: el archivo original usaba matrices
// globales de tamano fijo (int A[100][100]), lo que produce memoria
// corrupta (undefined behavior) para los tamanos que pide el enunciado
// (n = 256 y n = 1024, ya que 1024 > 100). Se reemplazo por
// vector<vector<int>> dimensionado dinamicamente segun el tamano real de
// las matrices de entrada. La logica de la multiplicacion (triple for) es
// la misma del original.
//
// Se extrajo a header-only (namespace naive) para poder incluirla tanto
// desde el benchmark como desde cualquier otro archivo del proyecto sin
// duplicar codigo ni arrastrar un main().

#include <vector>

namespace naive {

// Multiplica A (m x p) por B (p x n) y retorna la matriz resultado (m x n).
// Complejidad: O(m * n * p), o O(n^3) para matrices cuadradas de lado n.
inline std::vector<std::vector<int>> multiply(const std::vector<std::vector<int>>& A,
                                               const std::vector<std::vector<int>>& B) {
    const int m = static_cast<int>(A.size());
    const int p = static_cast<int>(A[0].size());
    const int n = static_cast<int>(B[0].size());

    std::vector<std::vector<int>> R(m, std::vector<int>(n, 0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < p; k++) {
                R[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return R;
}

}  // namespace naive

#endif  // NAIVE_HPP
