#ifndef STRASSEN_HPP
#define STRASSEN_HPP

// Fuente base: https://medium.com/@shashikantsolanki042/strassens-matrix-multiplication-algorithm-b6cc3b2287df
//
// Modificaciones respecto al original:
//   - Todas las funciones (add, subtract, strassenMultiply) recibian las
//     matrices por valor, copiando A y B completos en cada llamada
//     recursiva. Ahora reciben const&; el algoritmo no cambia, solo se
//     evitan copias que no aportan nada salvo costo de tiempo/memoria y
//     ensucian la comparacion contra naive en el benchmark.
//   - Se elimino bits/stdc++.h (dependencia especifica de GCC/competencia),
//     la funcion Print y el main() interactivo (freopen a input.txt/
//     output.txt, cin/cout) para dejar solo el algoritmo, reutilizable
//     como libreria header-only (namespace strassen).
//   - Se verificaron las 7 multiplicaciones (P..V) y el ensamblaje de
//     C11/C12/C21/C22 contra las formulas estandar de Strassen: coinciden,
//     el algoritmo original ya era correcto.
//
// Requisito importante: A y B deben ser cuadradas y de tamano potencia de
// 2 (no hay padding automatico). Alcanza para n = 256 y n = 1024 del
// enunciado. Si en algun momento se necesita soportar tamanos arbitrarios,
// hay que agregar padding con ceros hasta la siguiente potencia de 2 antes
// de llamar a multiply(), y despues recortar el resultado al tamano real.

#include <vector>

namespace strassen {

namespace detail {

inline std::vector<std::vector<int>> add(const std::vector<std::vector<int>>& A,
                                          const std::vector<std::vector<int>>& B,
                                          int size) {
    std::vector<std::vector<int>> C(size, std::vector<int>(size, 0));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

inline std::vector<std::vector<int>> subtract(const std::vector<std::vector<int>>& A,
                                               const std::vector<std::vector<int>>& B,
                                               int size) {
    std::vector<std::vector<int>> C(size, std::vector<int>(size, 0));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

inline std::vector<std::vector<int>> strassenMultiply(const std::vector<std::vector<int>>& A,
                                                        const std::vector<std::vector<int>>& B,
                                                        int size) {
    // Caso base: matriz 1x1.
    if (size == 1) {
        std::vector<std::vector<int>> C(1, std::vector<int>(1, 0));
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    std::vector<std::vector<int>> C(size, std::vector<int>(size, 0));
    const int k = size / 2;

    // Dividiendo las matrices en cuadrantes.
    std::vector<std::vector<int>> A11(k, std::vector<int>(k, 0));
    std::vector<std::vector<int>> A12(k, std::vector<int>(k, 0));
    std::vector<std::vector<int>> A21(k, std::vector<int>(k, 0));
    std::vector<std::vector<int>> A22(k, std::vector<int>(k, 0));
    std::vector<std::vector<int>> B11(k, std::vector<int>(k, 0));
    std::vector<std::vector<int>> B12(k, std::vector<int>(k, 0));
    std::vector<std::vector<int>> B21(k, std::vector<int>(k, 0));
    std::vector<std::vector<int>> B22(k, std::vector<int>(k, 0));

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][k + j];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][k + j];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    std::vector<std::vector<int>> P = strassenMultiply(add(A11, A22, k), add(B11, B22, k), k);
    std::vector<std::vector<int>> Q = strassenMultiply(add(A21, A22, k), B11, k);
    std::vector<std::vector<int>> R = strassenMultiply(A11, subtract(B12, B22, k), k);
    std::vector<std::vector<int>> S = strassenMultiply(A22, subtract(B21, B11, k), k);
    std::vector<std::vector<int>> T = strassenMultiply(add(A11, A12, k), B22, k);
    std::vector<std::vector<int>> U = strassenMultiply(subtract(A21, A11, k), add(B11, B12, k), k);
    std::vector<std::vector<int>> V = strassenMultiply(subtract(A12, A22, k), add(B21, B22, k), k);

    std::vector<std::vector<int>> C11 = add(P, add(subtract(S, T, k), V, k), k);
    std::vector<std::vector<int>> C12 = add(R, T, k);
    std::vector<std::vector<int>> C21 = add(Q, S, k);
    std::vector<std::vector<int>> C22 = add(P, add(subtract(R, Q, k), U, k), k);

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j] = C11[i][j];
            C[i][j + k] = C12[i][j];
            C[i + k][j] = C21[i][j];
            C[i + k][j + k] = C22[i][j];
        }
    }
    return C;
}

}  // namespace detail

// Multiplica A x B mediante el algoritmo de Strassen.
// Complejidad: O(n^log2(7)) ~ O(n^2.807).
// Requiere A y B cuadradas, de igual tamano, y ese tamano potencia de 2.
inline std::vector<std::vector<int>> multiply(const std::vector<std::vector<int>>& A,
                                               const std::vector<std::vector<int>>& B) {
    const int size = static_cast<int>(A.size());
    return detail::strassenMultiply(A, B, size);
}

}  // namespace strassen

#endif  // STRASSEN_HPP
