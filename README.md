# Registro de swings de golf

Proyecto integral de TC1031

Sebastian Villegas Olaya - A01714719

## Descripción del proyecto

Juego golf y cuando voy al range apunto mis swings: con qué palo pegué, a qué
distancia llegó la bola y a qué velocidad salió. La idea del proyecto es tener un
programa donde guardar esos swings y poder consultarlos, por ejemplo ver cuáles
fueron los más largos o cuánto le pego en promedio con cada palo.

Los datos están en `swings.csv`, que funciona como la base de datos del
programa. Cada renglón es un swing, en el orden en que los fui pegando:

```
palo,distancia,velocidad
Driver,238,148
```

## Avance 2

En este avance el programa:

- Lee los swings de `swings.csv` y los guarda en una **lista doblemente ligada**
  (`lista.h`), en el mismo orden en que los pegué.
- Muestra todos los swings o solo los últimos que pegué.
- Ordena los swings por distancia, de mayor a menor, usando **quicksort**.
- Busca los swings de un palo y saca la distancia promedio, la velocidad
  promedio y el mejor tiro con ese palo.
- Deja agregar swings nuevos y borrar swings.

### Cambios desde el Avance 1

- Los swings ya no se guardan en un `vector`, ahora están en mi propia lista
  doblemente ligada (`ListaSwings` en `lista.h`).
- Cambié bubble sort por quicksort. Lalo, en la retroalimentación del Avance 1
  me dijo que bubble sort se tarda mucho cuando hay muchos datos, y como ahora
  se pueden ir agregando swings, la lista va a crecer. Por eso borré `sorts.h` y
  el ordenamiento ahora está dentro de la lista.
- El menú ya no se queda en un ciclo infinito si escriben algo que no es número o
  si se acaba la entrada.

### Cómo correrlo

```
g++ main.cpp -o golf
./golf
```

El archivo `swings.csv` tiene que estar en la misma carpeta. Al empezar, el
programa dice cuántos swings cargó y muestra este menú:

| Opción | Qué hace |
|---|---|
| 1. Ver mis swings | Muestra todos los swings con su número (#), en el orden en que los pegué. |
| 2. Ver mis últimos swings | Pregunta cuántos y los muestra del más reciente al más viejo. |
| 3. Ordenar por distancia | Muestra los swings del más largo al más corto. No cambia el orden de la lista. |
| 4. Buscar por palo | Pide un palo (por ejemplo `Driver` o `hierro7`, da igual si es mayúscula o minúscula) y muestra sus swings, cuántos son, los promedios y el mejor tiro. |
| 5. Agregar un swing | Pide el palo, la distancia y la velocidad y lo agrega al final, como el swing más reciente. |
| 6. Borrar un swing | Pide el número del swing (el # de la opción 1) y lo borra. |
| 0. Salir | Termina el programa. |

Por ahora los swings que se agregan o se borran solo cambian en memoria. Guardar
los cambios en `swings.csv` lo dejo para el Avance 3.

## SICT0302: Toma decisiones

### Selecciona una estructura de datos adecuada al problema y la usa correctamente

Usé una lista doblemente ligada (`ListaSwings`, `lista.h` línea 34). Cada nodo
(`Nodo`, línea 19) guarda un swing y apunta al swing anterior (`ant`) y al
siguiente (`sig`). `head` es el primer swing que registré y `tail` el más
reciente.

La elegí porque mis swings son un registro que va creciendo en orden: cada vez
que voy al range pego más y se agregan al final. Con eso:

- Agregar un swing es inmediato, porque tengo `tail` y no tengo que recorrer la
  lista (`agregarFinal`, línea 101).
- Para ver mis últimos swings empiezo en `tail` y me voy para atrás con `ant`,
  así solo visito los swings que quiero ver y no toda la lista
  (`mostrarUltimos`, línea 165). Esto es lo que más uso, porque casi siempre
  quiero ver cómo me fue en la última sesión.
- Para borrar un swing, si está en la primera mitad lo busco desde `head` y si
  está en la segunda desde `tail`, así nunca recorro más de la mitad
  (`eliminar`, línea 116). Al borrar solo cambio dos apuntadores; en un vector
  tendría que recorrer todos los elementos que están después.
- No necesito saber cuántos swings voy a tener, la lista crece de uno en uno.

Lo que no puedo hacer rápido con la lista es ir directo a una posición, pero en
mi programa siempre recorro los swings en orden, así que no lo necesito.

### Selecciona un algoritmo de ordenamiento adecuado al problema

Usé quicksort para ordenar los swings por distancia de mayor a menor
(`quickSort` en `lista.h` línea 74 y `particion` en la línea 49). Se usa en la
opción 3 del menú, desde `mostrarPorDistancia` (línea 181).

Lo elegí porque en el caso promedio es O(n log n), mucho más rápido que bubble
sort cuando hay muchos swings, y lo puedo hacer directo sobre la lista ligada sin
pasarla a un arreglo:

- `particion` toma como pivote la distancia del último nodo y recorre los nodos
  con `sig`. Los swings más largos que el pivote los va pasando al principio, y
  al final pone el pivote en su lugar.
- `quickSort` ordena con recursión la parte de antes del pivote (usando `ant`
  para saber dónde termina) y la de después (usando `sig`).
- No muevo nodos, solo cambio los swings que guardan, así no tengo que arreglar
  los apuntadores.

Ordeno una copia de la lista y no la lista original, porque si la ordenara
perdería el orden en que pegué los swings y la opción 2 ya no mostraría los
últimos de verdad.

El peor caso de quicksort pasa cuando el pivote siempre es el más largo o el
más corto, por ejemplo si los swings ya vinieran ordenados por distancia. En mi
caso es poco probable, porque en el range voy cambiando de palo y las distancias
quedan revueltas en el orden en que las pego.

## SICT0301: Evalúa los componentes

### Análisis de complejidad

n es el número de swings en la lista y k el número de swings que se quieren ver
en la opción 2.

#### Lista doblemente ligada

| Función | Complejidad | Por qué |
|---|---|---|
| `agregarFinal` | O(1) | Usa `tail` y solo cambia dos apuntadores, no recorre nada. |
| `eliminar` | O(n) | Para llegar al swing recorre desde `head` o desde `tail`, el que esté más cerca, así que a lo mucho son n/2 pasos, que sigue siendo O(n). Ya que lo encuentra, desconectarlo es O(1). |
| `mostrar` | O(n) | Pasa una vez por cada swing. |
| `mostrarUltimos` | O(k) | Empieza en `tail` y solo avanza k nodos hacia atrás. Si k es más grande que la lista, es O(n). |
| `buscarPalo` | O(n) | Tiene que revisar todos los swings, porque los de un mismo palo pueden estar en cualquier lugar de la lista. Los promedios y el mejor tiro los saca en el mismo recorrido, sin volver a recorrer. |
| `getTam` | O(1) | La lista lleva la cuenta en `tam`. |
| Destructor | O(n) | Borra cada nodo una vez. |

Espacio: la lista usa O(n), un nodo por swing.

#### Ordenamiento con quicksort

- Mejor caso: O(n log n), cuando el pivote parte la lista más o menos a la mitad.
  Se hacen log n niveles de recursión y en cada nivel `particion` pasa por los n
  swings.
- Caso promedio: O(n log n), que es lo que pasa con distancias revueltas como
  las mías.
- Peor caso: O(n²), cuando el pivote siempre queda en un extremo (la lista ya
  ordenada o al revés). Cada llamada solo quita un swing y hay n niveles.

`mostrarPorDistancia` además copia la lista antes de ordenarla (O(n)) y la
imprime (O(n)), así que en total la opción 3 es O(n log n) en promedio y O(n²)
en el peor caso. Usa O(n) de espacio extra por la copia, más la pila de la
recursión: O(log n) en promedio y O(n) en el peor caso.

#### Lectura del archivo

`leerSwings` es O(n): lee una vez cada renglón del archivo y lo agrega con
`agregarFinal`, que es O(1).

#### Programa completo

| Opción del menú | Complejidad |
|---|---|
| Cargar `swings.csv` al empezar | O(n) |
| 1. Ver mis swings | O(n) |
| 2. Ver mis últimos swings | O(k) |
| 3. Ordenar por distancia | O(n log n) promedio, O(n²) peor caso |
| 4. Buscar por palo | O(n) |
| 5. Agregar un swing | O(1) |
| 6. Borrar un swing | O(n) |

Lo más costoso del programa es ordenar por distancia (opción 3), con
O(n log n) en el caso promedio.

## SICT0303: Implementa acciones científicas

### Implementa mecanismos para consultar información de las estructuras correctos

El programa tiene estas consultas sobre la lista:

- Opción 1: ver todos los swings en orden (`mostrar`, `lista.h` línea 152).
- Opción 2: ver los últimos k swings, del más reciente al más viejo
  (`mostrarUltimos`, línea 165).
- Opción 3: ver los swings ordenados por distancia con quicksort
  (`mostrarPorDistancia`, línea 181).
- Opción 4: ver los swings de un palo con su distancia promedio, velocidad
  promedio y mejor distancia (`buscarPalo`, línea 200). Compara el palo en
  minúsculas para que `driver` y `Driver` encuentren lo mismo.

En todas las consultas se muestra el número (#) de cada swing, que es el mismo
que pide la opción 6 para borrar. Se llaman desde el menú en `main.cpp`, de la
línea 69 a la 112.

### Implementa mecanismos de lectura de archivos para cargar datos a las estructuras de manera correcta

Los swings se cargan de `swings.csv` al iniciar el programa con la función
`leerSwings` (`main.cpp` línea 9). Se salta el renglón de los títulos, separa
cada renglón por comas en palo, distancia y velocidad, convierte los números con
`stoi` y agrega cada swing al final de la lista con `agregarFinal`, así la lista
queda en el mismo orden que el archivo. Si el archivo no existe avisa en
pantalla, y si hay renglones vacíos se los salta. Al terminar de cargar, el
programa dice cuántos swings leyó.

## Siguientes avances

- Avance 3: usar un árbol para buscar swings y guardar los swings agregados o
  borrados en `swings.csv`.
