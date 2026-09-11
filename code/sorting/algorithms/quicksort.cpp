//Fuente: https://www.geeksforgeeks.org/cpp/cpp-program-for-quicksort/
//
// Modificacion respecto al original: se agrego seleccion de pivote por
// "mediana de tres" (primero, medio, ultimo) en vez de usar siempre el
// ultimo elemento como pivote. El enunciado exige probar arreglos ya
// ordenados ascendente/descendente, que es exactamente el peor caso O(n^2)
// de un pivote fijo en el ultimo elemento (con n=10^7 no termina en tiempo
// razonable). Tecnica de mediana de tres:
// Fuente: https://cplusplus.com/faq/sequences/sequencing/sort-algorithms/quicksort/#pivot-median-of-three

#include <bits/stdc++.h>
using namespace std;

// Deja en vec[high] la mediana entre vec[low], vec[mid] y vec[high],
// para usarla como pivote y evitar el peor caso en arreglos ya ordenados.
void medianOfThree(vector<int>& vec, int low, int high) {
    int mid = low + (high - low) / 2;
    if (vec[mid] < vec[low]) swap(vec[mid], vec[low]);
    if (vec[high] < vec[low]) swap(vec[high], vec[low]);
    if (vec[high] < vec[mid]) swap(vec[high], vec[mid]);
    swap(vec[mid], vec[high]);
}

void partition3(vector<int>& vec, int low, int high, int& lt, int& gt) {
    int pivot = vec[high];  // la mediana quedó acá
    int i = low;
    lt = low;
    gt = high - 1; // dejamos vec[high] afuera del barrido, ya es el pivote
    while (i <= gt) {
        if (vec[i] < pivot) {
            swap(vec[lt], vec[i]);
            lt++; i++;
        } else if (vec[i] > pivot) {
            swap(vec[i], vec[gt]);
            gt--;
        } else {
            i++;
        }
    }
    swap(vec[gt + 1], vec[high]); // coloca el pivote en su posicion final
    gt = gt + 1; // gt ahora apunta a donde quedó el pivote
}

int partition(vector<int>& vec, int low, int high) {

    // Selecting the last element as the pivot
    int pivot = vec[high];

    // Index of the element smaller than the pivot
    int i = low - 1;

    for (int j = low; j < high; j++) {

        // If the current element is smaller than
        // or equal to the pivot
        if (vec[j] <= pivot) {
            i++;
            swap(vec[i], vec[j]);
        }
    }

    // Place the pivot in its correct position
    swap(vec[i + 1], vec[high]);

    // Return the partition index
    return i + 1;
}


void quickSort(vector<int>& vec, int low, int high) {

    // Base case
    if (low < high) {

        // Si hay al menos 3 elementos, elegir pivote por mediana de tres
        if (high - low >= 2) {
            medianOfThree(vec, low, high);
        }

        int lt,gt;
        // Partition the array
        partition3(vec, low, high, lt, gt);

        // Sort elements before the pivot
        quickSort(vec, low, lt -1);

        // Sort elements after the pivot
        quickSort(vec, gt + 1, high);
    }
}
