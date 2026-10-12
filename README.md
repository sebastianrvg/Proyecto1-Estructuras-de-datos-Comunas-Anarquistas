# Comunas Anarquistas

Proyecto 1 del curso de Estructuras de Datos. Simulación en C de una sociedad solarpunk organizada en comunas anarquistas que gestionan personas, bienes y servicios, y que se ayudan mediante trueque y regalo, sin usar dinero. El éxito de la sociedad se mide con la satisfacción de las personas.

- **Integrantes:** Gustavo López y Sebastián Rodríguez
- **Repositorio:** https://github.com/sebastianrvg/Proyecto1-Estructuras-de-datos-Comunas-Anarquistas

## Cómo compilar y ejecutar

Requiere gcc y  make. El programa lee los archivos comunas, personas, bienes y servicios del **directorio actual**, así que hay que ejecutarlo desde la carpeta del proyecto.

```
make          # compila la simulación completa (ejecutable: prog)
./prog        # ejecuta la simulación
make demo     # compila el demo de las estructuras (ejecutable: demo)
./demo        # ejecuta el demo
make clean    # borra los ejecutables
```

Al iniciar, el programa pide cuántas comunas (2 a 50), cuántos bienes (1 a 25) y cuántos servicios (1 a 25) tendrá la sociedad. Después muestra un menú que se repite cada día:

1. Hacer un trueque
2. Pasar al día siguiente
3. Ver comunas (necesidad y satisfacción)
4. Ver comunas en emergencia
5. Salir

Ejemplo de un trueque (los números cambian en cada ejecución porque hay azar):

```
Propuesta de trueque de bienes:
  Conte Burica recibe 1 de Agua de parte de Meleruk.
  Meleruk recibe 5 de Arroz de parte de Conte Burica.
  Bono de reciprocidad: Conte Burica +1 y Meleruk +2.
-----------------------------------------------------------------------------------------------------
Desea continuar con el intercambio? (1 = si, 2 = no):
Trueque exitoso: Meleruk le dio 1 de Agua a Conte Burica, y recibio 5 de Arroz.
```

Para una demostración se recomienda usar entre 5 y 10 comunas con 6 bienes y 6 servicios.

## Archivos de datos

| Archivo   | Contenido              | Cantidad |
| --------- | ---------------------- | -------- |
| personas  | un nombre por línea   | 500      |
| comunas   | un nombre por línea   | 50       |
| bienes    | un bien por línea     | 25       |
| servicios | un servicio por línea | 25       |

Los nombres de línea no deben pasar de 48 caracteres, y los archivos no llevan extensión.

## Organización del código

| Archivo           | Qué contiene                                                                                                                                           |
| ----------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Nodos.c/.h        | Las estructuras (Persona,Recursos,Servicios,Comuna), insertar, recorrer, leer los archivos y avanzarComunas.                                            |
| Sociedad.c/.h     | crearSociedad: pide las cantidades, elige las comunas, reparte las personas, calcula los máximos y libera la memoria (liberarComunas).                 |
| Indices.c/.h      | Fórmulas de necesidad y satisfacción, y actualizarIndices.                                                                                            |
| Consumo.c/.h      | Consumo diario de bienes.                                                                                                                               |
| Emergencias.c/.h  | Emergencias aleatorias, regalo tras una emergencia y la lista de comunas en emergencia.                                                                 |
| Trueque.c/.h      | Trueque de bienes y de servicios, y el bono de reciprocidad.                                                                                            |
| Busqueda.c/.h     | Búsquedas en las listas, cálculo de puntos, verificación de si una comuna puede dar algo sin quedar en riesgo, traslado de personas y confirmación. |
| PasarDia.c/.h     | pasarTurno: lo que ocurre al pasar un día.                                                                                                             |
| DemoEstructuras.c | Programa aparte que demuestra la lista simple y la lista circular.                                                                                      |
| Colores.h         | Códigos de color para la terminal.                                                                                                                     |
| main.c            | Menú principal y menú del trueque.                                                                                                                    |

## Concepto del proyecto

### ¿Por qué esos nombres de comunas?

Los nombres de las comunas corresponden a territorios y comunidades indígenas reales de Costa Rica (Talamanca Bribri, Talamanca Cabécar, Boruca, Salitre, Ujarrás, Térraba, Guatuso, Matambú, entre otros). Se escogieron a propósito, por dos razones.

**Coherencia temática.** El proyecto simula una sociedad organizada bajo una economía de cooperación, sin dinero, basada en el trueque y en la ayuda mutua ante emergencias. Esa forma de organización tiene mucho en común con la manera en que históricamente han funcionado muchas comunidades indígenas: decisiones colectivas, trabajo comunitario y reciprocidad en lugar de acumulación individual. Usar nombres de territorios reales conecta la simulación con un contexto concreto en vez de dejarla como un ejercicio abstracto.

**Una posición planteada con cuidado.** Hay que separar tres niveles distintos:

- *Lo que está documentado:* existen tradiciones indígenas de reciprocidad y trabajo colectivo (en la región andina, el *ayni* o la *minka*) que históricamente han organizado la vida económica sin depender de dinero ni de mercado (Alberti y Mayer, 1974). También está documentado que los territorios indígenas de Costa Rica enfrentan hoy carencias reales de acceso a salud, agua, electricidad y educación, y que gran parte de la ayuda que reciben viene de fundaciones externas y no de un sistema económico propio (Jiménez Monge, 2023). Y en Costa Rica han existido iniciativas de economía social solidaria basadas en trueque, como la Feria Orgánica El Trueque, que mostró tanto capacidad de generar prácticas de solidaridad como límites frente a la lógica de mercado dominante (Castañeda et al., 2015).
- *Lo que es posible pero no está demostrado:* que un sistema sin dinero, basado en trueque y regalo entre comunidades, sea la intervención adecuada para aliviar las carencias actuales de las comunidades indígenas costarricenses. Las tradiciones y las carencias documentadas no prueban por sí solas que formalizar un sistema así mejoraría sus condiciones de vida; son cosas distintas.
- *Lo que haría falta para comprobarlo:* que las propias comunidades indígenas participen directamente en el diseño y la evaluación de una propuesta así, y determinen si les sirve, en qué forma y bajo qué condiciones. Ninguna simulación puede sustituir ese proceso, y la propuesta es, en el mejor de los casos, una hipótesis plausible y algo utópica.

Por eso los nombres no se usan para afirmar que el proyecto resuelve algo, sino como un gesto de conectar la simulación con una tradición económica real (la reciprocidad indígena) y con una necesidad real.

### ¿Por qué esos bienes y servicios?

Los bienes (agua, arroz, frijoles, maíz, papa, leche, huevos, medicamentos, ropa, semillas, etc.) y los servicios (educación, atención médica, agricultura, carpintería, electricidad, plomería, cuidado de personas, etc.) se escogieron para que fueran reconocibles dentro de la vida cotidiana costarricense, en particular la de zonas rurales: son los que sostienen a una comunidad pequeña y autosuficiente.

Esto conecta con el solarpunk, un movimiento que imagina futuros sostenibles donde la tecnología y la comunidad conviven con el cuidado del entorno. Aquí el solarpunk no está en tecnología futurista, sino en la organización social: comunidades autosuficientes, con producción local y cooperación, sin depender de un mercado centralizado. El anarquismo, como se usa en este proyecto, no es caos sino ausencia de una autoridad centralizada o de dinero que controle el intercambio: cada comuna decide con quién comerciar, qué pedir a cambio y a quién ayudar cuando hay una emergencia.

### ¿Por qué esa economía (trueque y regalo, sin dinero)?

Se descartó usar dinero o un valor numérico fijo para los bienes porque la especificación lo pide, y porque eso sería otra vez la economía de mercado que el proyecto busca evitar. Es una decisión de diseño, no una afirmación de que un sistema sin dinero sea superior o inferior por sí solo.

- **El trueque** modela la cooperación negociada. Una comuna pide algo y la que provee decide qué pedir a cambio: el bien o servicio del que ella tiene menos, para repartir su propia escasez. Cada trueque exitoso le da a ambas comunas un **bono de reciprocidad** del 30 % de lo que cada una recibe, redondeado hacia arriba. Este bono no es una ganancia monetaria, sino la representación de que la cooperación genera más valor comunitario que el simple intercambio. Si una comuna ya está cerca de su máximo, el bono se limita a lo que le cabe.
- **El regalo tras una emergencia** modela la solidaridad espontánea. Cuando una comuna sufre una emergencia hay un 25 % de probabilidad de que le regalen entre 5 y 10 unidades del bien que ella tiene en menor cantidad, sin pedir nada a cambio. Lo regala la primera comuna que pueda hacerlo sin poner en riesgo a su propia gente: se revisan una por una, empezando por la siguiente en la lista circular. Si ninguna puede, no pasa nada. Solo se regalan bienes, nunca servicios.

Los dos mecanismos reflejan dos formas complementarias de solidaridad económica sin mercado: una pactada (el trueque) y una espontánea (el regalo).

## Parte técnica

### Estructuras de datos

- **Lista enlazada simple** para las personas, los bienes y los servicios. Solo necesitan recorrerse hacia adelante, insertarse y, a veces, sacar un nodo del medio (por ejemplo al trasladar una persona de una comuna a otra en un trueque de servicios). No hace falta volver atrás ni una referencia circular.
- **Lista enlazada circular** para las comunas. Las comunas tienen una noción de vecindad: la satisfacción de una comuna depende en parte de sus vecinas, y las emergencias graves afectan a la comuna anterior y a la siguiente. En una lista circular, la que sigue a la última es la primera, sin casos especiales. La función avanzarComunas recibe cuántos pasos avanzar y el total de comunas, y calcula la posición final con un módulo (%), así que funciona sin importar cuántas vueltas se den.

Cada estructura se usa porque lo que representa encaja con ella, y no solo para cumplir el requisito de tener dos tipos. make demo ejecuta un programa aparte que muestra las dos funcionando por separado.

### Sistema de oficios

Cada servicio tiene un número de oficio, que es su posición en el archivo servicios. Cada persona recibe al azar un número de oficio entre 1 y la cantidad de servicios elegidos. Así se puede saber cuántas personas de una comuna tienen un oficio comparando números en vez de texto. Esto es lo que hace posible el trueque de servicios: cuando una comuna da un servicio, en realidad presta personas con ese oficio, y el programa verifica que de verdad tenga suficientes antes de aceptar el trueque.

### Cantidades máximas e iniciales

Cada bien y servicio tiene una cantidad máxima que depende de las personas de la comuna: **60 por persona en los bienes y 2 por persona en los servicios**. Los bienes tienen un factor mucho mayor porque se consumen cada turno, mientras que los servicios solo bajan por emergencias. La cantidad inicial es un número al azar entre el 60 % y el 100 % del máximo.

### Consumo y emergencias

- **Consumo:** cada día, cada persona consume entre 1 y 5 unidades de cada bien (3 de promedio), así que cada comuna consume una cantidad distinta según su población.
- **Emergencias:** ocurren cada 4 a 7 turnos, un número al azar. Hay cinco tipos: incendio, inundación y sequía (leves), y terremoto y revolución (graves). Cada una afecta a un solo bien o servicio elegido al azar, con una pérdida de 5 % del máximo por cada punto de gravedad. Las graves afectan además a la comuna anterior y a la siguiente (si hay al menos 3 comunas).

### Fórmulas

**Necesidad.** Para los bienes se suman las cantidades actuales y las máximas de todos, y se calcula la fracción que falta para llegar al máximo; se hace igual con los servicios. La necesidad es 99 por el promedio de las dos fracciones, así que nunca llega a 100.

**Satisfacción propia.** Cada bien o servicio recibe puntos según cuánto tiene:

- *Bienes:* si tiene menos de lo que consumirían 2 turnos (con el consumo promedio) cuenta como emergencia; con el 90 % o más del máximo vale 2 puntos; con 4 turnos o más de reserva vale 1; y en el resto vale 0.
- *Servicios:* se compara con el reparto esperado (personas entre tipos de servicio). Menos del 20 % es emergencia; hasta el 60 % vale 0; menos del 100 % vale 1; y el 100 % o más vale 2.

Los puntos de lo que no está en emergencia se suman y se dividen entre el máximo posible (2 por ítem). El resultado se multiplica por un **tope**: 99 si no hay emergencias, y 50 menos 5 por cada emergencia adicional a la primera si las hay.

**Satisfacción final.** Es el 70 % de la satisfacción propia más el 30 % del promedio de las vecinas, hasta 4 (2 anteriores y 2 siguientes; menos si hay menos de 5 comunas).

Estas fórmulas no usan azar: dado el mismo estado de una comuna, siempre dan el mismo resultado. Esto es necesario porque la satisfacción se recalcula varias veces por turno (también para calcular la de las vecinas), y si dependiera del azar la misma situación podría mostrar resultados distintos sin que nada cambiara. El azar se usa en otros lugares: el consumo, las emergencias, el regalo y los valores iniciales.

## Extras

Logo de texto al iniciar, colores en la terminal para distinguir los mensajes (rojo para emergencias, verde para trueques exitosos, amarillo para avisos y opciones inválidas, cian para el encabezado de cada día, magenta para el logo y la salida), confirmación antes de cada trueque, traslado de personas con su oficio en los trueques de servicios, regalo tras una emergencia, y el programa aparte DemoEstructuras que demuestra las dos estructuras.

## Referencias

Alberti, G., y Mayer, E. (Eds.). (1974). *Reciprocidad e intercambio en los Andes peruanos* (Perú Problema No. 12). Instituto de Estudios Peruanos.

Boza Villarreal, A. (2018). Economía, Estado y comunidades indígenas en Talamanca, Costa Rica, 1927-1948. *Intercambio*, Universidad de Costa Rica, Centro de Investigación en Identidad y Cultura Latinoamericanas.

Castañeda, H., Chaves-Chang, L. D., y Segura-Vargas, X. (2015). Feria orgánica El Trueque: alcances de una economía social solidaria. *Trama. Revista de Ciencias Sociales y Humanidades, 4*(1-2), 20-60. https://doi.org/10.18845/tracs.v4i1-2.2473

Jiménez Monge, A. V. (2023, 27 de septiembre). Territorios indígenas sufren la desidia y el olvido de sus compatriotas. *Delfino.cr*.

*Nota sobre las fuentes:* la fuente de Boza Villarreal (2018) se cita de forma general porque solo se pudo acceder a la introducción del texto completo; no se usan de ella afirmaciones específicas que no estén respaldadas en esa parte. La fecha de publicación de Jiménez Monge (2023) se toma del byline del artículo tal como aparece publicado.
