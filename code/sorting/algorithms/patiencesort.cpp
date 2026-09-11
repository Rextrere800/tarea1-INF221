/*
Fuente: https://www.geeksforgeeks.org/dsa/patience-sorting/

Modificaciones respecto al original:
1) patienceSorting({}) (arreglo vacio) causaba segmentation fault: con
    piles vacio, merge_piles intentaba hacer v[-1] al no encontrar ningun
    indice valido. Se agrego un chequeo temprano para ese caso.
2) Se elimino el "cout" que el original tenia dentro de patienceSorting():
    imprimia el resultado a stdout en cada llamada, lo cual no es
    necesario (ni practico con arreglos de hasta 10^7 elementos) ya que
    el resultado se debe escribir a un archivo desde el programa principal.
3) Reemplazo de las dos busquedas lineales del original (O(n^2) en el
    peor caso, por ejemplo con un arreglo ya ordenado ascendente, donde
    se crean n pilas de tamano 1) por:
    - binary search (upper_bound) sobre los topes de las pilas para
      ubicar cada carta -> O(log k) por insercion en vez de O(k).
    - min-heap (priority_queue) para el merge de las pilas -> O(log k)
      por extraccion en vez de O(k).
    Esto se puede hacer porque los topes de las pilas quedan SIEMPRE
    ordenados de forma ascendente de izquierda a derecha (invariante
    clasico de patience sorting, demostrable por induccion: si los topes
    estan ordenados antes de insertar una carta x, siguen ordenados
    despues, porque x se coloca en la primera pila cuyo tope la supera,
    y por construccion ese valor queda entre el tope anterior y el
    siguiente). Complejidad total: O(n log k) <= O(n log n).
*/

#include <bits/stdc++.h>
using namespace std;

// Funcion para mezclar las pilas en orden usando un min-heap sobre los
// topes actuales de cada pila, en vez de escanear todas las pilas en
// cada extraccion (como hacia el original).
vector<int> merge_piles(vector<vector<int>>& piles)
{
    using P = pair<int,int>; // (valor, indice de pila)
    priority_queue<P, vector<P>, greater<P>> pq;

    for (int i = 0; i < (int)piles.size(); i++) {
        pq.push({piles[i].back(), i});
    }

    vector<int> ans;
    while (!pq.empty()) {
        auto [val, idx] = pq.top();
        pq.pop();

        ans.push_back(val);
        piles[idx].pop_back();

        // Si la pila no quedo vacia, su nuevo tope vuelve al heap
        if (!piles[idx].empty()) {
            pq.push({piles[idx].back(), idx});
        }
    }
    return ans;
}

// Funcion para ordenar el arreglo usando patience sorting
vector<int> patienceSorting(vector<int> arr)
{
    // Caso borde agregado: arreglo vacio (el original no lo manejaba
    // y hacia segfault en merge_piles).
    if (arr.empty()) {
        return {};
    }

    vector<vector<int>> piles;
    vector<int> tops; // topes de cada pila, invariante: ordenados ascendente

    for (int x : arr) {
        // Primera pila cuyo tope es estrictamente mayor que x
        int idx = (int)(upper_bound(tops.begin(), tops.end(), x) - tops.begin());

        if (idx == (int)tops.size()) {
            // Ninguna pila sirve: se crea una nueva
            piles.push_back({x});
            tops.push_back(x);
        } else {
            // Se coloca sobre la pila encontrada
            piles[idx].push_back(x);
            tops[idx] = x;
        }
    }

    // Mezclar las pilas para obtener el arreglo ordenado
    vector<int> ans = merge_piles(piles);

    return ans;
}