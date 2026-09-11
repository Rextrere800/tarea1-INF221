# Documentación

## Entrega

La entrega se realiza vía **aula.usm.cl** en formato `.zip`.

## Requisitos

- **C++17** o superior (`g++` con soporte para `<filesystem>`).
- **Python 3** con `numpy` y `matplotlib` instalados:
```bash
  pip install numpy matplotlib
```

## Comandos en Makefile
Tanto en ordenamiento de arreglos como en multiplicacion de matrices el makefile usa los mismos comandos:
| Comando | Descripción |
|---|---|
| `make` / `make all` | Compila el programa principal |
| `make generate` | Ejecuta el script de Python que genera los datos de entrada |
| `make run` | Compila (si es necesario) y ejecuta el benchmark |
| `make plots` | Ejecuta el generador de gráficos sobre los measurements existentes |
| `make report` | `run` + `plots` en un solo paso |
| `make clean` | Elimina el ejecutable |
| `make clean-data` | Elimina outputs, measurements, plots y CSVs generados |

## Multiplicación de matrices
- **Naive** (`algorithms/naive.hpp`): multiplicación por fuerza bruta, O(n³).
  Fuente: [Dev-XYS/Algorithms](https://github.com/Dev-XYS/Algorithms/blob/master/Matrix-Multiplication(Naive).cpp).
  Se reemplazaron las matrices globales de tamaño fijo del original por
  `vector<vector<int>>` dimensionado dinámicamente, ya que el original
  producía memoria corrupta para n=256 y n=1024.

- **Strassen** (`algorithms/strassen.hpp`): O(n^log2(7)) ≈ O(n^2.807).
  Fuente: [Strassen's Matrix Multiplication Algorithm](https://medium.com/@shashikantsolanki042/strassens-matrix-multiplication-algorithm-b6cc3b2287df).
  Se cambió el paso de matrices por valor a `const&` para evitar copias
  innecesarias, y se eliminó el `main()` interactivo del original para
  dejarlo como librería header-only. Requiere matrices cuadradas de
  tamaño potencia de 2 (no hay padding automático).

### Programa principal

Recorre `data/matrix_input/`, empareja los archivos `{n}_{tipo}_{dominio}_{muestra}_1.txt`
y `..._2.txt`, y para cada par ejecuta ambos algoritmos midiendo tiempo
(`std::chrono`) y memoria pico (mediante sobrecarga global de `operator new`/`delete`).
Escribe el resultado de la multiplicación en `data/matrix_output/` y las
mediciones (`{tiempo_us} {memoria_bytes}`) en `data/measurements/`.

**n = 1024 (2^10) excluido**: por indicación del curso, se omitió este
tamaño de las mediciones dado el tiempo de ejecución de Strassen sin
caso base optimizado.

### Scripts
- `scripts/matrix_generator.py`: genera las matrices de entrada según
  `n ∈ {16, 64, 256, 1024}`, `tipo ∈ {densa, diagonal, dispersa}`,
  `dominio ∈ {D0, D10}`, `muestra ∈ {a, b, c}`.
- `scripts/plot_generator.py`: lee `data/measurements/`, promedia sobre
  las 3 muestras y genera gráficos log-log de tiempo y memoria por
  combinación (tipo, dominio) en `data/plots/`, además de las tablas
  resumen `resumen_tiempo.csv` y `resumen_memoria.csv`.

## Ordenamiento de arreglo unidimensional

- **std::sort** (`algorithms/sort.cpp`): provisto por el enunciado.
- **MergeSort** (`algorithms/mergesort.cpp`): implementación estándar, O(n log n).
  Fuente: [GeeksforGeeks - Merge Sort](https://www.geeksforgeeks.org/dsa/merge-sort/).
- **QuickSort** (`algorithms/quicksort.cpp`): O(n log n) esperado.
  Fuente: [GeeksforGeeks - QuickSort](https://www.geeksforgeeks.org/cpp/cpp-program-for-quicksort/).
  Se agregó selección de pivote por *mediana de tres* (primero, medio,
  último) en vez de usar siempre el último elemento, ya que este último
  degenera a O(n²) en arreglos ya ordenados asc/descendente (justo los
  casos que pide el enunciado, con n=10⁷). Fuente de la técnica:
  [cplusplus.com - mediana de tres](https://cplusplus.com/faq/sequences/sequencing/sort-algorithms/quicksort/#pivot-median-of-three).
- **PatienceSort** (`algorithms/patiencesort.cpp`): O(n log n).
  Fuente: [GeeksforGeeks - Patience Sorting](https://www.geeksforgeeks.org/dsa/patience-sorting/).
  Se reemplazaron las búsquedas lineales del original (O(n) por
  inserción) por `upper_bound` sobre los topes de las pilas (O(log k))
  y un min-heap para el merge final (O(log k) por extracción), dado que
  los topes de las pilas se mantienen siempre ordenados. Complejidad
  total O(n log n). Se corrigió además un caso borde de arreglo vacío
  que causaba segmentation fault en el original.
### Programa principal
Recorre `data/array_input/`, y para cada archivo `{n}_{tipo}_{dominio}_{muestra}.txt`
ejecuta los 4 algoritmos midiendo tiempo y memoria pico (mismo mecanismo
de sobrecarga de `operator new`/`delete` que en matrices). Escribe el
arreglo ordenado en `data/array_output/` y las mediciones en
`data/measurements/`.

### Scripts
- `scripts/array_generator.py`: genera los arreglos de entrada según
  `n ∈ {10, 10³, 10⁵, 10⁷}`, `tipo ∈ {ascendente, descendente, aleatorio}`,
  `dominio ∈ {D1, D7}`, `muestra ∈ {a, b, c}`.
- `scripts/plot_generator.py`: análogo al de matrices — promedia sobre
  las 3 muestras y genera gráficos log-log de tiempo y memoria por
  combinación (tipo, dominio), además de los CSV resumen.