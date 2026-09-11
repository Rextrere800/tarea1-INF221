#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cstddef> // Asegura la definición de size_t

#include "algorithms/naive.hpp"
#include "algorithms/strassen.hpp"

// Se puede crear un alias para acortar el código
namespace fs = std::filesystem;


// Variables usadas para medicion de memoria
size_t memoria_actual = 0;
size_t pico_global = 0; // Registra el máximo absoluto histórico si lo necesitas

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

    // Definir la ruta de la carpeta (puede ser absoluta o relativa)
    fs::path ruta_carpeta = "./data/matrix_input"; 
    fs::path output_carpeta = "./data/matrix_output";
    fs::path medidas_carpeta = "./data/measurements";

    std::vector<std::vector<int>> matriz1, matriz2;
    bool matrix1_calculable = false,matrix2_calculable = false;
    // Verificar si la ruta existe y realmente es un directorio
    if (fs::exists(ruta_carpeta) && fs::is_directory(ruta_carpeta)) {
        std::cout << "Archivos en " << ruta_carpeta << ":\n";
        
        // Iterar sobre los elementos del directorio
        for (const auto& entrada : fs::directory_iterator(ruta_carpeta)) {
            // .path().filename() obtiene solo el nombre, no la ruta completa
            std::ifstream archivo(entrada.path());
            int counter = 0;
            int numero;
            std::cout << "- " << entrada.path().filename().string() << "\n";
            int n;
            std::string nombre_archivo = entrada.path().filename().string();
            for (int i = 0; i < static_cast<int>(nombre_archivo.size())-5; i++){
                if (nombre_archivo[i] == '_'){
                    n = stoi(nombre_archivo.substr(0,i));
                }
            }
            if (!archivo.is_open()){
                std::cout << "No se pudo abrir el archivo";
                return 1;
            }
            if (nombre_archivo[nombre_archivo.size()-5] == '1'){
                matriz1.assign(n, std::vector<int>(n, 0));
                matrix1_calculable = true;
                while (archivo >> numero){
                    matriz1[counter/n][counter%n] = numero;
                    counter++;
                }
            } else if (nombre_archivo[nombre_archivo.size()-5] == '2'){
                matriz2.assign(n, std::vector<int>(n, 0));
                matrix2_calculable = true;
                while (archivo >> numero){
                    matriz2[counter/n][counter%n] = numero;
                    counter++;
                }
            }
            //Aca se mide tiempo y memoria de cada algoritmo
            if(matrix1_calculable && matrix2_calculable && !(n >= 1024)){
                size_t memoria_inicio = 0;
                std::string nombre_salida_naive = "naive_" + entrada.path().filename().string().substr(0,entrada.path().filename().string().size()-5) + "out.txt";
                std::ofstream salidaNaive(output_carpeta / (nombre_salida_naive));
                pico_global = memoria_actual;
                memoria_inicio = memoria_actual;
                auto inicio = std::chrono::high_resolution_clock::now();
                std::vector<std::vector<int>> naive = naive::multiply(matriz1, matriz2);
                auto fin = std::chrono::high_resolution_clock::now();
                size_t memoria_pico_naive = pico_global - memoria_inicio;
                auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio);
                std::ofstream medidasNaive(medidas_carpeta / (nombre_salida_naive));
                medidasNaive << duracion.count() << " " << memoria_pico_naive;
                for(std::vector<int> fila_actual: naive){
                    for (int valor_actual: fila_actual)
                    salidaNaive << valor_actual << " ";
                }
                salidaNaive.close();
                medidasNaive.close();
                naive.clear();
                memoria_inicio = 0;
                std::string nombre_salida_strassen = "strassen_" + entrada.path().filename().string().substr(0,entrada.path().filename().string().size()-6) + "_out.txt";
                std::ofstream salidaStrassen(output_carpeta / (nombre_salida_strassen));
                pico_global = memoria_actual;
                memoria_inicio = memoria_actual;
                inicio = std::chrono::high_resolution_clock::now();
                std::vector<std::vector<int>> strassen = strassen::detail::strassenMultiply(matriz1, matriz2, n);
                fin = std::chrono::high_resolution_clock::now();
                size_t memoria_pico_strassen = pico_global - memoria_inicio;
                duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio);
                std::ofstream medidasStrassen(medidas_carpeta / (nombre_salida_strassen));
                medidasStrassen << duracion.count() << " " << memoria_pico_strassen;
                for(std::vector<int> fila_actual: naive){
                    for (int valor_actual: fila_actual)
                    salidaStrassen << valor_actual << " ";
                }
                salidaStrassen.close();
                medidasStrassen.close();
                strassen.clear();
                matrix1_calculable = false;
                matrix2_calculable = false;
            } else if (matrix1_calculable && matrix2_calculable && n >= 1024){
                matrix1_calculable = false;
                matrix2_calculable = false;
            }
        }
    } else {
        std::cerr << "La ruta no existe o no es un directorio válido.\n";
    }

    return 0;
}