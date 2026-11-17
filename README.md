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

## Avance 3

En este avance el programa:

- Lee los swings de `swings.csv` y los guarda en una **lista doblemente ligada**
  (`lista.h`), en el mismo orden en que los pegué, y en un **árbol binario de
  búsqueda** (`arbol.h`) ordenado por distancia.
- Muestra todos los swings o solo los últimos que pegué.
- Ordena los swings por distancia, de mayor a menor, usando **quicksort**.
- Busca los swings de un palo y saca la distancia promedio, la velocidad
  promedio y el mejor tiro con ese palo.
- Deja agregar swings nuevos y borrar swings, y guarda los cambios en
  `swings.csv`.
- Me recomienda qué palo usar según las yardas que me quedan.

### Cambios desde el Avance 2

- Los swings que se agregan o se borran ya se guardan en `swings.csv`.
- Agregué el árbol y la opción 7 para que me recomiende un palo.
- Si un renglón del archivo tiene datos que no sirven, el programa se lo salta y
  avisa en vez de cerrarse.
- Agregué 20 swings más, ahora son 60.
- Completé el análisis de complejidad con el mejor caso, el caso promedio y el
  peor caso.

### Cambios desde el Avance 1

- Los swings ya no se guardan en un `vector`, ahora están en mi propia lista
  doblemente ligada (`ListaSwings` en `lista.h`).
- Cambié bubble sort por quicksort, porque bubble sort es O(n²) y como ahora se
  pueden ir agregando swings, la lista va a crecer. Por eso borré `sorts.h` y el
  ordenamiento ahora está dentro de la lista.
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
| 5. Agregar un swing | Pide el palo, la distancia y la velocidad, lo agrega al final como el swing más reciente y lo guarda en `swings.csv`. |
| 6. Borrar un swing | Pide el número del swing (el # de la opción 1), lo borra y actualiza `swings.csv`. |
| 7. Qué palo uso | Pide cuántas yardas te quedan, muestra tus swings que cayeron a 5 yardas o menos de esa distancia y te recomienda el palo con el que más veces has llegado ahí. |
| 0. Salir | Termina el programa. |

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
  (`mostrarUltimos`, línea 173). Esto es lo que más uso, porque casi siempre
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
opción 3 del menú, desde `mostrarPorDistancia` (línea 189).

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

### Usa un árbol adecuado para resolver un problema

Usé un árbol binario de búsqueda ordenado por distancia (`ArbolSwings`,
`arbol.h` línea 30). Los swings más cortos van a la izquierda y los de la misma
distancia o más largos a la derecha. Lo uso en la opción 7: si me quedan 150
yardas, busca mis swings entre 145 y 155 y me recomienda el palo con el que más
veces he llegado ahí (`recomendar`, línea 176).

Lo elegí porque con la lista tendría que revisar todos los swings, y en el árbol
`buscarRango` (línea 100) solo baja por las ramas donde puede haber swings de ese
rango, y además los va sacando ya ordenados. El árbol se actualiza cuando agrego
o borro un swing (`insertar`, línea 157, y `eliminar`, línea 163). Igual que con
quicksort, como las distancias entran revueltas el árbol no se degenera: con mis
60 swings tiene altura 9.

## SICT0301: Evalúa los componentes

### Análisis de complejidad

n es el número de swings, k el número de swings que se quieren ver en la
opción 2 y r los swings que caen en el rango de la opción 7.

#### Lista doblemente ligada

| Función | Mejor | Promedio | Peor | Por qué |
|---|---|---|---|---|
| `agregarFinal` | O(1) | O(1) | O(1) | Usa `tail` y solo cambia dos apuntadores, no recorre nada. |
| `eliminar` | O(1) | O(n) | O(n) | Recorre desde `head` o desde `tail`, el que esté más cerca. Si es el primero o el último no recorre nada; si no, son hasta n/2 pasos, que sigue siendo O(n). |
| `mostrar` | O(n) | O(n) | O(n) | Pasa una vez por cada swing. |
| `mostrarUltimos` | O(1) | O(k) | O(n) | Empieza en `tail` y solo avanza k nodos hacia atrás. Si k es más grande que la lista, es O(n). |
| `buscarPalo` | O(n) | O(n) | O(n) | Siempre revisa todos los swings, porque los de un mismo palo pueden estar en cualquier lugar de la lista. |
| `guardar` | O(n) | O(n) | O(n) | Escribe cada swing en el archivo una vez. |
| `getTam` | O(1) | O(1) | O(1) | La lista lleva la cuenta en `tam`. |
| Destructor | O(n) | O(n) | O(n) | Borra cada nodo una vez. |

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

#### Árbol binario de búsqueda

| Función | Mejor | Promedio | Peor | Por qué |
|---|---|---|---|---|
| `insertar` | O(1) | O(log n) | O(n) | Baja desde la raíz hasta su lugar. En el peor caso el árbol está degenerado y baja n niveles. |
| `eliminar` | O(1) | O(log n) | O(n) | Baja igual que `insertar` hasta encontrar el swing. Si tiene dos hijos, `quitarMinimo` baja por otro camino, que no cambia la complejidad. |
| `buscarRango` | O(log n + r) | O(log n + r) | O(n) | Baja hasta el rango y solo visita los r swings que caen ahí. |
| `masCercano` | O(log n) | O(log n) | O(n) | Baja por un solo camino de la raíz a una hoja. |
| `recomendar` | O(log n + r) | O(log n + r) | O(n) | Usa `buscarRango` y cuenta los r swings por palo. Si no hay ninguno, usa `masCercano`. |
| Destructor | O(n) | O(n) | O(n) | Borra cada nodo una vez. |

Espacio: el árbol usa O(n), un nodo por swing.

#### Lectura y escritura del archivo

- `leerSwings` lee cada renglón una vez y lo agrega a la lista (O(1)) y al árbol
  (O(log n)), así que es O(n log n) en el mejor caso y en promedio, y O(n²) en el
  peor caso, si el archivo viniera ordenado por distancia.
- `guardarNuevo` es O(1) siempre: solo escribe el renglón nuevo al final.
- `guardarTodos` es O(n) siempre: reescribe todos los swings.

#### Programa completo

| Opción del menú | Mejor | Promedio | Peor |
|---|---|---|---|
| Cargar `swings.csv` al empezar | O(n log n) | O(n log n) | O(n²) |
| 1. Ver mis swings | O(n) | O(n) | O(n) |
| 2. Ver mis últimos swings | O(1) | O(k) | O(n) |
| 3. Ordenar por distancia | O(n log n) | O(n log n) | O(n²) |
| 4. Buscar por palo | O(n) | O(n) | O(n) |
| 5. Agregar un swing | O(1) | O(log n) | O(n) |
| 6. Borrar un swing | O(n) | O(n) | O(n) |
| 7. Qué palo uso | O(log n + r) | O(log n + r) | O(n) |

La complejidad final del programa es O(n log n) en el mejor caso y en promedio,
y O(n²) en el peor caso, por cargar el archivo y ordenar por distancia. Todo lo
demás es O(n) o menos.

## SICT0303: Implementa acciones científicas

### Implementa mecanismos para consultar información de las estructuras correctos

El programa tiene estas consultas:

- Opción 1: ver todos los swings en orden (`mostrar`, `lista.h` línea 152).
- Opción 2: ver los últimos k swings, del más reciente al más viejo
  (`mostrarUltimos`, línea 173).
- Opción 3: ver los swings ordenados por distancia con quicksort
  (`mostrarPorDistancia`, línea 189).
- Opción 4: ver los swings de un palo con su distancia promedio, velocidad
  promedio y mejor distancia (`buscarPalo`, línea 208). Compara el palo en
  minúsculas para que `driver` y `Driver` encuentren lo mismo.
- Opción 7: ver qué palo me conviene según las yardas que me quedan
  (`recomendar`, `arbol.h` línea 176).

Se llaman desde el menú en `main.cpp`, de la línea 146 a la 217.

### Implementa mecanismos de lectura de archivos para cargar datos a las estructuras de manera correcta

Los swings se cargan de `swings.csv` al iniciar el programa con la función
`leerSwings` (`main.cpp` línea 26). Se salta el renglón de los títulos, separa
cada renglón por comas en palo, distancia y velocidad, convierte los números con
`stoi` y agrega cada swing al final de la lista y al árbol. Si el archivo no
existe avisa en pantalla, y si un renglón está vacío o tiene datos que no sirven
se lo salta (`esNumero`, línea 12). Al terminar de cargar, el programa dice
cuántos swings leyó.

### Implementa mecanismos de escritura de archivos para guardar los datos de las estructuras de manera correcta

Cuando agrego un swing, `guardarNuevo` (`main.cpp` línea 65) lo escribe al final
de `swings.csv`. Cuando borro uno, `guardarTodos` (`main.cpp` línea 100)
reescribe el archivo con los swings que quedan, usando `guardar` de la lista
(`lista.h` línea 165). Así la próxima vez que abro el programa los swings están
como los dejé.

## SEG0701: Cultura digital

### El producto genera valor para un usuario final y se ejecuta sin errores

Me sirve para llevar el registro de mis sesiones en el range y, con la opción 7,
para saber qué palo usar en la cancha según lo que de verdad me ha salido. Lo
probé agregando y borrando swings y volviendo a abrir el programa, con un
`swings.csv` con renglones malos, sin el archivo, con entradas que no son
números y con la lista vacía, y en ningún caso se cerró con error.
