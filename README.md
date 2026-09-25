# Registro de tiros de golf

Proyecto integral de TC1031 - Avance 1

Sebastian Villegas Olaya - A01714719

## Descripción

Cuando voy al range apunto los tiros que hago: la fecha, el palo, la distancia y la
velocidad de la bola. El programa lee esos tiros de `tiros.csv` y permite verlos
ordenados por distancia (para ver cuáles fueron mis mejores tiros) o por palo
(para comparar los tiros que hice con el mismo palo).

## Cómo usarlo

Compilar:

```
g++ -std=c++17 main.cpp -o golf
```

Ejecutar (el archivo `tiros.csv` tiene que estar en la misma carpeta):

```
./golf
```

El menú tiene estas opciones:

1. Ver tiros
2. Ordenar por distancia (mayor a menor)
3. Ordenar por palo
0. Salir

## SICT0302: Toma decisiones

### Selecciona un algoritmo de ordenamiento adecuado al problema

Usé merge sort para ordenar los tiros por distancia y por palo. Lo elegí porque
siempre es O(n log n) sin importar cómo vengan los datos, y porque es estable:
cuando dos tiros tienen el mismo palo se quedan en el orden en el que estaban. Eso
sirve porque si primero ordeno por distancia y luego por palo, los tiros de cada
palo quedan de mayor a menor distancia.

Quicksort también es O(n log n) en promedio, pero no es estable y en el peor caso
es O(n²), así que no me servía para ordenar primero por distancia y luego por palo.

El ordenamiento está en `ordenamiento.h`: la función `merge` en la línea 18,
`mergeSort` en la línea 47 y `ordenar` en la línea 57. Se llama desde `main.cpp`
en las líneas 61 y 64.

## SICT0301: Evalúa los componentes

### Análisis de complejidad

**Leer el archivo** (`leerArchivo`): O(n), porque lee una línea por cada tiro.

**Mostrar los tiros** (`mostrar`): O(n), porque imprime cada tiro una vez.

**Merge sort** (`mergeSort`):
- Mejor caso: O(n log n)
- Caso promedio: O(n log n)
- Peor caso: O(n log n)

El arreglo se divide a la mitad hasta llegar a un elemento, lo que da log n
niveles. En cada nivel la función `merge` recorre los n elementos para juntarlos.
Por eso son n log n pasos en todos los casos.

En espacio usa O(n), porque `merge` necesita un vector auxiliar para ir juntando
las dos mitades.

## Próximos avances

- Avance 2: guardar los tiros en una estructura de datos lineal.
- Avance 3: agregar un árbol y guardar los tiros nuevos en el archivo.
