# Registro de swings de golf

Proyecto integral de TC1031

Sebastian Villegas Olaya - A01714719

## Descripción del proyecto

Juego golf y cuando voy al range apunto mis swings: con qué palo pegué, a qué
distancia llegó la bola y a qué velocidad salió. La idea del proyecto es tener un
programa donde guardar esos swings y poder consultarlos, por ejemplo ver cuáles
fueron los más largos o cuánto le pego en promedio con cada palo.

Por ahora los datos están en `swings.csv`, que funciona como la base de datos del
programa. Cada renglón es un swing:

```
palo,distancia,velocidad
Driver,238,148
```

## Avance 1

En este avance el programa:

- Lee los 20 swings de `swings.csv`.
- Los muestra en pantalla.
- Los ordena por distancia, de mayor a menor, usando bubble sort.

### Cómo correrlo

```
g++ main.cpp -o golf
./golf
```

El archivo `swings.csv` tiene que estar en la misma carpeta.

## SICT0302: Toma decisiones

### Selecciona un algoritmo de ordenamiento adecuado al problema

Usé bubble sort para ordenar los swings por distancia. Lo elegí porque es fácil
de implementar y como por ahora solo tengo 20 swings, no se nota que sea más
lento que otros algoritmos. Le agregué una bandera para que se detenga si en una
pasada no hizo ningún cambio, así si la lista ya está ordenada termina rápido.

Está en `sorts.h` en la línea 10 y se llama desde `main.cpp` en la línea 57.

## SICT0301: Evalúa los componentes

### Análisis de complejidad

Bubble sort:

- Mejor caso: O(n), cuando los swings ya están ordenados. Hace una sola pasada y
  como no cambia nada, se sale.
- Caso promedio: O(n²)
- Peor caso: O(n²), cuando están al revés. Tiene que hacer n pasadas y en cada
  una compara casi todos los elementos.

Leer el archivo y mostrar los swings: O(n), porque pasa una vez por cada swing.

## Siguientes avances

- Avance 2: guardar los swings en una lista ligada y poder agregar swings nuevos.
- Avance 3: usar un árbol para buscar swings y guardar los cambios en el archivo.
- Si llego a tener muchos swings, cambiar bubble sort por merge sort.
