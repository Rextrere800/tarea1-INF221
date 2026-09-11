#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cstddef> // Asegura la definición de size_t

#include "algorithms/sort.hpp"
#include "algorithms/quicksort_adapter.hpp"
#include "algorithms/mergesort_adapter.hpp"
#include "algorithms/patiencesort.hpp"

// Se puede crear un alias para acortar el código
namespace fs = std::filesystem;


// Variables usadas para medicion de memoria
size_t memoria_actual = 0;
size_t pico_global = 0; 

// Sobrecarga global de 'new'
void* operator new(size_t size) {
    memoria_actual += size;
    if (memoria_actual > pico_global) {
        pico_global = memoria_actual;
    }
    return malloc(size);
}

// Sobrecarga global de 'delete'
void operator delete(void* memory, size_t size) noexcept {
    memoria_actual -= size;
    free(memory);
}

int main() {

    fs::path ruta_carpeta = "./data/array_input"; 
    fs::path output_carpeta = "./data/array_output";
    fs::path medidas_carpeta = "./data/measurements";

    // Verificar si la ruta existe y realmente es un directorio
    if (fs::exists(ruta_carpeta) && fs::is_directory(ruta_carpeta)) {
        std::cout << "Archivos en " << ruta_carpeta << ":\n";
        
        // Iterar sobre los elementos del directorio
        for (const auto& entrada : fs::directory_iterator(ruta_carpeta)) {
            // .path().filename() obtiene solo el nombre, no la ruta completa
            std::ifstream archivo(entrada.path());
            std::cout << "- " << entrada.path().filename().string() << "\n";
            int numero;
            if (!archivo.is_open()){
                std::cout << "No se pudo abrir el archivo";
                return 1;
            }
            std::vector<int> arreglo_desordenado;
            while (archivo >> numero)
                arreglo_desordenado.push_back(numero);
            archivo.close();

            // Aqui se empieza a trabajar en las salidas
            // Sort
            size_t memoria_inicio = 0;
            std::string nombre_sort = "sort_" + entrada.path().filename().string().substr(0,entrada.path().filename().string().size()-4) + "_out.txt";
            std::ofstream salidaSort(output_carpeta / (nombre_sort));
            pico_global = memoria_actual;
            memoria_inicio = memoria_actual;
            auto inicio = std::chrono::high_resolution_clock::now();
            std::vector<int> arreglo_sort = sortArray(arreglo_desordenado);
            auto fin = std::chrono::high_resolution_clock::now();
            size_t memoria_pico_sort = pico_global - memoria_inicio;
            auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio);
            std::ofstream medidasSort(medidas_carpeta / (nombre_sort));
            medidasSort << duracion.count() << " " << memoria_pico_sort;
            for(int valor_actual: arreglo_sort){
                salidaSort << valor_actual << " ";
            }
            salidaSort.close();
            medidasSort.close();
            arreglo_sort.clear();

            // QuickSort
            std::string nombre_quicksort = "quicksort_" + entrada.path().filename().string().substr(0,entrada.path().filename().string().size()-4) + "_out.txt";
            std::ofstream salidaQuickSort(output_carpeta / (nombre_quicksort));
            pico_global = memoria_actual;
            memoria_inicio = memoria_actual;
            inicio = std::chrono::high_resolution_clock::now();
            arreglo_sort = quickSortReal(arreglo_desordenado);
            fin = std::chrono::high_resolution_clock::now();
            size_t memoria_pico_quick = pico_global - memoria_inicio;
            duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio);
            std::ofstream medidasQuickSort(medidas_carpeta / (nombre_quicksort));
            medidasQuickSort << duracion.count() << " " << memoria_pico_quick;
            for(int valor_actual: arreglo_sort){
                salidaQuickSort << valor_actual << " ";
            }
            salidaQuickSort.close();
            medidasQuickSort.close();
            arreglo_sort.clear();

            // MergeSort
            std::string nombre_mergesort = "mergesort_" + entrada.path().filename().string().substr(0,entrada.path().filename().string().size()-4) + "_out.txt";
            std::ofstream salidaMergeSort(output_carpeta / (nombre_mergesort));
            pico_global = memoria_actual;
            memoria_inicio = memoria_actual;
            inicio = std::chrono::high_resolution_clock::now();
            arreglo_sort = mergeSortReal(arreglo_desordenado);
            fin = std::chrono::high_resolution_clock::now();
            size_t memoria_pico_merge = pico_global - memoria_inicio;
            duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio);
            std::ofstream medidasMergeSort(medidas_carpeta / (nombre_mergesort));
            medidasMergeSort << duracion.count() << " " << memoria_pico_merge;
            for(int valor_actual: arreglo_sort){
                salidaMergeSort << valor_actual << " ";
            }
            salidaMergeSort.close();
            medidasMergeSort.close();
            arreglo_sort.clear();

            // PatienceSort
            std::string nombre_patiencesort = "patiencesort_" + entrada.path().filename().string().substr(0,entrada.path().filename().string().size()-4) + "_out.txt";
            std::ofstream salidaPatienceSort(output_carpeta / (nombre_patiencesort));
            pico_global = memoria_actual;
            memoria_inicio = memoria_actual;
            inicio = std::chrono::high_resolution_clock::now();
            arreglo_sort = patienceSorting(arreglo_desordenado);
            fin = std::chrono::high_resolution_clock::now();
            size_t memoria_pico_patience = pico_global - memoria_inicio;
            duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio);
            std::ofstream medidasPatienceSorting(medidas_carpeta / (nombre_patiencesort));
            medidasPatienceSorting << duracion.count() << " " << memoria_pico_patience;
            for(int valor_actual: arreglo_sort){
                salidaPatienceSort << valor_actual << " ";
            }
            salidaPatienceSort.close();
            medidasPatienceSorting.close();
            arreglo_sort.clear();
        }
    } else {
        std::cerr << "La ruta no existe o no es un directorio válido.\n";
    }

    return 0;
}