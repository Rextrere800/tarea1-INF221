"""
Fuente propia (sin referencia externa). Genera graficos y tablas resumen
a partir de los archivos de medidas producidos por sorting.cpp.

Formato esperado de cada archivo en ./data/measurements:
    nombre:    {algoritmo}_{n}_{tipo}_{dominio}_{muestra}.txt
    contenido: "{tiempo_microsegundos} {memoria_pico_bytes}"

Donde:
    algoritmo in {sort, quicksort, mergesort, patiencesort}
    n         in {10, 1000, 100000, 10000000}
    tipo      in {ascendente, descendente, aleatorio}
    dominio   in {D1, D7}
    muestra   in {a, b, c}

Salida:
    ./data/plots/tiempo_{tipo}_{dominio}.png   (log-log, 1 curva por algoritmo)
    ./data/plots/memoria_{tipo}_{dominio}.png  (log-log, 1 curva por algoritmo)
    ./data/plots/resumen_tiempo.csv            (tabla para el informe)
    ./data/plots/resumen_memoria.csv           (tabla para el informe)
"""

import re
import csv
from pathlib import Path
from collections import defaultdict

import matplotlib
matplotlib.use("Agg")  # no necesita display, sirve para correr en CI/consola
import matplotlib.pyplot as plt
import numpy as np

# Rutas relativas a la RAIZ del proyecto (carpeta que contiene "data/"),
# calculadas a partir de la ubicacion de este archivo -> funciona sin
# importar si se ejecuta desde la raiz o desde scripts/.
RAIZ_PROYECTO = Path(__file__).resolve().parent.parent
RUTA_MEDIDAS = RAIZ_PROYECTO / "data" / "measurements"
RUTA_PLOTS = RAIZ_PROYECTO / "data" / "plots"

ALGORITMOS = ["sort", "quicksort", "mergesort", "patiencesort"]
TIPOS = ["ascendente", "descendente", "aleatorio"]
DOMINIOS = ["D1", "D7"]

# Nombre bonito para leyendas/titulos
NOMBRE_ALGO = {
    "sort": "std::sort",
    "quicksort": "QuickSort",
    "mergesort": "MergeSort",
    "patiencesort": "PatienceSort",
}

# Patron de nombre de archivo: algoritmo_n_tipo_dominio_muestra.txt
PATRON = re.compile(
    r"^(?P<algo>sort|quicksort|mergesort|patiencesort)_"
    r"(?P<n>\d+)_"
    r"(?P<tipo>ascendente|descendente|aleatorio)_"
    r"(?P<dominio>D1|D7)_"
    r"(?P<muestra>[abc])_out\.txt$"
)


def leer_medidas(ruta_carpeta: Path):
    """Recorre ./data/measurements y devuelve una lista de dicts, uno por archivo."""
    if not (ruta_carpeta.exists() and ruta_carpeta.is_dir()):
        print(f"La ruta {ruta_carpeta} no existe o no es un directorio valido.")
        return []

    filas = []
    for archivo in sorted(ruta_carpeta.iterdir()):
        if not archivo.is_file():
            continue

        m = PATRON.match(archivo.name)
        if not m:
            print(f"Aviso: se ignora archivo con nombre inesperado: {archivo.name}")
            continue

        try:
            contenido = archivo.read_text().strip()
            partes = contenido.split()
            if len(partes) < 2:
                print(f"Aviso: {archivo.name} no tiene el formato esperado, se ignora.")
                continue
            tiempo_us, memoria_bytes = int(partes[0]), int(partes[1])
        except (ValueError, IOError) as e:
            print(f"Error al procesar el archivo {archivo.name}: {e}")
            continue

        filas.append({
            "algoritmo": m.group("algo"),
            "n": int(m.group("n")),
            "tipo": m.group("tipo"),
            "dominio": m.group("dominio"),
            "muestra": m.group("muestra"),
            "tiempo_us": tiempo_us,
            "memoria_bytes": memoria_bytes,
        })

    return filas


def agrupar_promedios(filas):
    """
    Agrupa por (algoritmo, tipo, dominio, n) promediando sobre las muestras
    a/b/c. Devuelve dict: (algoritmo, tipo, dominio) -> lista de
    (n, tiempo_prom_us, tiempo_std_us, mem_prom_bytes, mem_std_bytes, n_muestras)
    ordenada por n.
    """
    acumulado = defaultdict(list)
    for fila in filas:
        clave = (fila["algoritmo"], fila["tipo"], fila["dominio"], fila["n"])
        acumulado[clave].append((fila["tiempo_us"], fila["memoria_bytes"]))

    agrupado = defaultdict(dict)
    for (algo, tipo, dominio, n), valores in acumulado.items():
        tiempos = np.array([v[0] for v in valores], dtype=float)
        memorias = np.array([v[1] for v in valores], dtype=float)
        agrupado[(algo, tipo, dominio)][n] = (
            tiempos.mean(), tiempos.std(),
            memorias.mean(), memorias.std(),
            len(valores),
        )

    resultado = {}
    for clave, por_n in agrupado.items():
        ns = sorted(por_n.keys())
        resultado[clave] = [(n, *por_n[n]) for n in ns]
    return resultado


def graficar(agrupado):
    RUTA_PLOTS.mkdir(parents=True, exist_ok=True)

    for tipo in TIPOS:
        for dominio in DOMINIOS:
            # Grafico de tiempo
            fig, ax = plt.subplots(figsize=(7, 5))
            hubo_datos = False
            for algo in ALGORITMOS:
                clave = (algo, tipo, dominio)
                if clave not in agrupado or not agrupado[clave]:
                    continue
                hubo_datos = True
                puntos = agrupado[clave]
                ns = [p[0] for p in puntos]
                t_prom_s = [p[1] / 1e6 for p in puntos]      # a segundos
                t_std_s = [p[2] / 1e6 for p in puntos]
                ax.errorbar(ns, t_prom_s, yerr=t_std_s, marker="o",
                            capsize=3, label=NOMBRE_ALGO[algo])

            if hubo_datos:
                ax.set_xscale("log")
                ax.set_yscale("log")
                ax.set_xlabel("n (cantidad de elementos)")
                ax.set_ylabel("tiempo promedio (s)")
                ax.set_title(f"Tiempo de ordenamiento - {tipo} - dominio {dominio}")
                ax.legend()
                ax.grid(True, which="both", linestyle="--", alpha=0.4)
                fig.tight_layout()
                fig.savefig(RUTA_PLOTS / f"tiempo_{tipo}_{dominio}.png", dpi=150)
                print(f"Guardado: tiempo_{tipo}_{dominio}.png")
            plt.close(fig)

            # Grafico de memoria
            fig, ax = plt.subplots(figsize=(7, 5))
            hubo_datos = False
            for algo in ALGORITMOS:
                clave = (algo, tipo, dominio)
                if clave not in agrupado or not agrupado[clave]:
                    continue
                hubo_datos = True
                puntos = agrupado[clave]
                ns = [p[0] for p in puntos]
                m_prom_mb = [p[3] / (1024 * 1024) for p in puntos]  # a MB
                m_std_mb = [p[4] / (1024 * 1024) for p in puntos]
                ax.errorbar(ns, m_prom_mb, yerr=m_std_mb, marker="o",
                            capsize=3, label=NOMBRE_ALGO[algo])

            if hubo_datos:
                ax.set_xscale("log")
                ax.set_yscale("log")
                ax.set_xlabel("n (cantidad de elementos)")
                ax.set_ylabel("pico de memoria promedio (MB)")
                ax.set_title(f"Uso de memoria - {tipo} - dominio {dominio}")
                ax.legend()
                ax.grid(True, which="both", linestyle="--", alpha=0.4)
                fig.tight_layout()
                fig.savefig(RUTA_PLOTS / f"memoria_{tipo}_{dominio}.png", dpi=150)
                print(f"Guardado: memoria_{tipo}_{dominio}.png")
            plt.close(fig)


def escribir_tablas_csv(agrupado):
    RUTA_PLOTS.mkdir(parents=True, exist_ok=True)

    with open(RUTA_PLOTS / "resumen_tiempo.csv", "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["algoritmo", "tipo", "dominio", "n",
                    "tiempo_prom_s", "tiempo_std_s", "n_muestras"])
        for (algo, tipo, dominio), puntos in sorted(agrupado.items()):
            for n, t_prom, t_std, _, _, n_muestras in puntos:
                w.writerow([algo, tipo, dominio, n,
                            f"{t_prom/1e6:.6f}", f"{t_std/1e6:.6f}", n_muestras])

    with open(RUTA_PLOTS / "resumen_memoria.csv", "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["algoritmo", "tipo", "dominio", "n",
                    "memoria_prom_MB", "memoria_std_MB", "n_muestras"])
        for (algo, tipo, dominio), puntos in sorted(agrupado.items()):
            for n, _, _, m_prom, m_std, n_muestras in puntos:
                w.writerow([algo, tipo, dominio, n,
                            f"{m_prom/(1024*1024):.4f}", f"{m_std/(1024*1024):.4f}", n_muestras])

    print("Guardado: resumen_tiempo.csv, resumen_memoria.csv")


def main():
    filas = leer_medidas(RUTA_MEDIDAS)
    if not filas:
        print("No se encontraron archivos de medidas validos. Nada que graficar.")
        return

    print(f"Se leyeron {len(filas)} archivos de medidas.")
    agrupado = agrupar_promedios(filas)
    graficar(agrupado)
    escribir_tablas_csv(agrupado)


if __name__ == "__main__":
    main()
