# PD04
* Mauricio Bernuy
* Claudia Noche
* Anthony Aguilar

[ToC]

## Ejercicio 1: Normalización

<details>
  <summary>Expandir Código</summary>
    
```cpp
void normaliza(double A[N][N])
{
  double suma = 0.0, factor;
  #pragma omp parallel
  {
  #pragma omp for /*collapse(2)*/ reduction(+ \
                                          : suma)
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        suma += A[i][j] * A[i][j];
      }
    }
    factor = 1.0 / sqrt(suma);
  #pragma omp for /*collapse(2)*/
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        A[i][j] = factor * A[i][j];
      }
    }
  }
}
```
</details>

###### > código base


### Item a)
#### Paralelice la función de normalización con OpenMP usando dos regiones paralelas *(#pragma omp parallel)*

<details>
  <summary>Expandir Código</summary>
    
```cpp
void normaliza(double A[N][N])
{
  double suma = 0.0, factor;
#pragma omp parallel for /*collapse(2)*/ reduction(+ \
                                                   : suma)
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      suma += A[i][j] * A[i][j];
    }
  }
  factor = 1.0 / sqrt(suma);
#pragma omp parallel for /*collapse(2)*/
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      A[i][j] = factor * A[i][j];
    }
  }
}
```
</details>

###### > preg1_1.cpp

Se paralelizó esta función haciendo uso de las directivas *#pragma omp for* en cada uno de los dobles for loops, usando una *reduction* del operador *+* para la suma del primer doble for loop. También se realizaron pruebas utilizando collapse(2) para distribuir la carga, pero debido a que ambos loops son del mismo tamaño, no se aprecia una mejora al usarlo (el tiempo de ejecucion incluso sube al usarse). Mediante pruebas de tiempo, se aprecia que la paralelización mejora considerablemente el tiempo de ejecución, pero solo se logra apreciar cuando el numero N de elementos sube de alrededor de 10000. Se podria decir que el numero de elementos debe ser bastante mayor que el numero de hilos para que se aprecie una mejora en eficiencia **(p << N)**.

### Item b)
#### Ahora englobe a todos los bucles con solo una región paralela. En este caso, ¿tendría sentido utilizar la cláusula *nowait*?


<details>
  <summary>Expandir Código</summary>
    
```cpp
// nowait y sincronizar no afectan o aplican ningun beneficio
void normaliza(double A[N][N])
{
  double suma = 0.0, factor;
  #pragma omp parallel
  {
  #pragma omp for /*nowait*/ reduction(+ \
                                  : suma)
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        suma += A[i][j] * A[i][j];
      }
    }
  // #pragma omp barrier
    factor = 1.0 / sqrt(suma);


  #pragma omp for /*nowait*/
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        A[i][j] = factor * A[i][j];
      }
    }
  }
}

```
</details>

###### > preg1_2.cpp

Para el diseño de este algoritmo, usar nowait no seria ideal, incluso despues de aplicar medidas de sincronización como barriers o utilizando locks. Esto es debido al cálculo del factor, el cual necesita tener la suma completa de los valores de la matriz para poder calcular su valor y aplicarlo al siguiente for. Un fuerte rediseño del algoritmo sería necesario para aplicar esta cláusula.

---

## Ejercicio 2: Quicksort

<details>
    <summary>Expandir Código</summary>
    
```cpp
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
void swap(int *x, int *y)
{
  int temp = *x;
  *x = *y;
  *y = temp;
}
int separate(int *x, int low, int high)
{
  int i, pivot, last;
  pivot = x[low];
  swap(x + low, x + high);
  last = low;
  for (i = low; i < high; i++)
  {
    if (x[i] <= pivot)
    {
      swap(x + last, x + i);
      last += 1;
    }
  }
  swap(x + last, x + high);
  return last;
}
void qs(int *z, int zstart, int zend, int firstcall)
{
  {
    int pivot;
    if (firstcall == 1)
    {
      qs(z, 0, zend, 0);
    }
    else
    {
      if (zstart < zend)
      {
        pivot = separate(z, zstart, zend);
        qs(z, zstart, pivot - 1, 0);
        qs(z, pivot + 1, zend, 0);
      }
    }
  }
}
int main(int argc, char **argv)
{
  int i, n, *w;
  n = atoi(argv[1]);
  w = malloc(n * sizeof(int));
  for (i = 0; i < n; i++)
    w[i] = rand() % 100;
  qs(w, 0, n - 1, 1);
  if (n < 25)
    for (i = 0; i < n; i++)
      printf("%d\n", w[i]);
  return 0;
}
```
</details>
    
###### > qsort.c


### Item a)
#### Paralelice con OpenMP el código adjunto *(qsort.c)*, tomando encuenta que el algoritmo recursivo *qs()* del ejemplo puede ser ejecutado en tareas *(tasks)*, dentro de un constructor *parallel*. Los valores iniciales a ser ordenados serán generados aleatoriamente con valores entre 1 y 99


<details>
    <summary>Expandir Código</summary>
    
```cpp
void qs(int *z, int zstart, int zend)
{
  // printf("hilo en qs: %i, nivel: %i, size: %i\n",
  //        omp_get_thread_num(), omp_get_active_level(), (zend - zstart));
  int pivot;
  {
    {
      if (zstart < zend)
      {
        pivot = separate(z, zstart, zend);

#pragma omp task shared(z) if ((zend - zstart) > N / THREADS)
        qs(z, zstart, pivot - 1);

#pragma omp task shared(z) if ((zend - zstart) > N / THREADS)
        qs(z, pivot + 1, zend);

#pragma omp taskwait
      }
    }
  }
}

void pqs(int *z, int zend)
{
#pragma omp parallel
  {
#pragma omp single
    qs(z, 0, zend - 1);
  }
}
```
</details>
    
###### > pqs.cpp

Se logró paralelizar el algoritmo de quicksort mediante el uso de *#pragma omp task* en cada llamada recursiva a la función *qs()*. Se empieza la ejecución en paralelo en la función *pqs()* desde un *#pragma omp single* en la primera llamada a la función *qs()*. Haciendo print a *omp_get_threadnum()* y *omp_get_active_level()*, se verifica la distribución de tasks a diferentes threads y su nivel de recursión.
| ![](https://i.imgur.com/piOYpOe.png) | 
|:--:| 
| *Output de consola, N=10, p=8* |
 
Se usa un script de Python para correr 100 iteraciones para un N y p definido y obtener el tiempo tiempos de cálculo del algoritmo.
| ![](https://i.imgur.com/wrQbRVF.png) | 
|:--:| 
| *Tiempos de ejecución, N=100000, p=1:8* |
    
Para permitir el paralelismo dentro de la recursión, es importante llamar a la función *omp_set_nested(1)* en la función main, la cual por defecto se encuentra desactivada.
Además, se utiliza *#pragma omp task if* para limitar la creación de tasks durante la recursión cuando el tamaño del problema se haya reducido lo suficiente *(en este caso, cuando baja de N/THREADS)*.
### Item b)
#### Grafique tiempos de cálculo para *n=106* vs. *p=1,2,4,8, ...* Haga un análisis cuantitativo del *speedup* y *eficiencia* del algoritmo y derive una expresión analítica en función a *n* y *p* para estas métricas.

Se genera la siguiente tabla, con los tiempos promedio de ejecución (100 pruebas):
    
| processors | average(s) |
| -------- | -------- |
|  1      |   0.00009758000000000000 |
|  2      |   0.00055327999999999996 |
|  3      |   0.00101010000000000010 |
|  4      |   0.00101479000000000044 |
|  5      |   0.00128855999999999979 |
|  6      |   0.00160773000000000036 |
|  7      |   0.00195014999999999967 |
|  8      |   0.00266137999999999878 |

###### >(Para este valor reducido de N, el overhead de la creación y manejo de tasks paralelas supera el beneficio que provee la ejecución en paralelo del programa)
    
![](https://i.imgur.com/w2ouJb1.png)

Se realizó un análisis del tiempo de ejecución en base al teorema de Brent, el cual explica:
    
$$T_p \leq \frac{T_1 - T_\infty}{p} + T_\infty $$
    
Se obtiene que la complejidad del algoritmo de quicksort paralelizado es:
    
$$ T(n,p) = O(\frac{nlog^2n}{p}) $$

Se calcula el Speedup:
    
$$ S(p) = \frac{T_1 (n)}{T_p (n)} = \frac{O (nlog (n))}{\frac{nlog^2n}{p}} = \frac{p}{log(n)} $$
    
Y la eficiencia:
$$ E(p) = \frac{\frac{p}{log(n)}}{p} = \frac{1}{log(n)} $$
    
Por lo que, en base a los resultados previos, se obtiene las siguientes expresiones:
    
$$ p = \frac{1}{log(n)}  $$
    
$$ n = 10^p$$

    
Utilizando valores mayores de N, se aprecia un claro incremento en el desempeño del algoritmo
|![](https://i.imgur.com/LWWnghL.png)| 
|:--:| 
| N=100000 |

### Item c)
#### Compare teoría con experimento y analice en que medida ambos coinciden (se obtiene la complejidad teorica esperada)

![](https://i.imgur.com/S5vAxY6.png)

Se puede ver que, como la eficiencia no posee $p$ entre sus variables, se concluye que no escala con la cantidad de procesos, por lo que se tiene un algotirmo **no escalable**.
    
Esto contrasta con la escalabilidad débil (weak scaling) mostrada con un dataset más grande en la figura previamente ilustrada.
    
Se obtienen las métricas esperadas y se demuestra la escalabilidad débil del problema resuelto. 
    
---

## Ejercicio 3: Arrays con OMP
Dada la siguiente función que realiza operaciones con los arrays *A*, *B* y *C*:

<details>
    <summary>Expandir Código</summary>
    
```cpp
double OpArr(double A[], double B[], double C[], int n)
{
  int i, j;
  double s1, s2, a, res;
  suma_de_prefijos(A, n); // obtiene el array de suma de prefijos de A
  quicksort(B, n);        // ordena el array B
  scan_left(C + x, n);    // acumula los valores de elementos de C mas
  // una constante x
  for (i = 0; i < n; i++)
  { /* primer bucle for */
    s1 = 0;
    for (j = 0; j < n; j++)
      s1 += A[j] * B[j];
    for (j = 0; j < n; j++)
      A[j] *= s1;
  }
  for (i = 0; i < n; i++)
  { /* segundo bucle for */
    s2 = 0;
    for (j = 0; j < n; j++)
      s2 += B[j] * C[j];
    for (j = 0; j < n; j++)
      C[j] *= s2;
  }
  /* calculo final */
  a = s1 / s2;
  res = 0;
  for (i = 0; i < n; i++)
    res += a * C[i];
  return res;
}

```
</details>
    
###### > código base


### Item a)
#### Identifique las tareas y sus dependencias y diagrame el DAG correspondiente

Se analizaron las posibles dependencias de las arrays *A*, *B*, *C* dentro de las operaciones en paralelo, y en base a esto se diagrama el siguiente *DAG*:    


| ![](https://i.imgur.com/6iMSicg.png) | 
|:--:| 
| Diagrama DAG simplificado, se obvia el comportamiento de las dependencias dentro de las funciones. |

### Item b)
#### Paralelice la función con *OMP*. Utilice tasks con dependencias.


Se utilizó la función paralela de quicksort creada en la anterior pregunta, y se implementó las siguientes funciones paralelas para suma de prefijos y scan left:
    
<details>
    <summary>Expandir Código</summary>
    
```cpp
void suma_de_prefijos(double *A, int n)
{
  double og_last = A[n-1]; 

  // sweep up
  long dmax = log2(n - 1);
  for (long d = 0; d <= dmax; d++)
  {
  #pragma omp parallel for
    for (long long k = 0; k < n; k += (1 << (d + 1)))
    {
      A[k + (1 << (d + 1)) - 1] = A[k + (1 << d) - 1] + A[k + (1 << (d + 1)) - 1];
    }
  }
  
  // sweep down
  A[n - 1] = 0;
  for (long d = dmax; d >= 0; d--)
  {
  #pragma omp parallel for
    for (long long k = 0; k < n; k += (1 << (d + 1)))
    {
      auto temp = A[k + (1 << d) - 1];
      A[k + (1 << d) - 1] = A[k + (1 << (d + 1)) - 1];
      A[k + (1 << (d + 1)) - 1] += temp;
    }
  }
  
  for (int i = 0; i != sizeof(A) - 1; i++) {
   *(A + i) = *(A + i + 1);
  }
  A[n-1] = A[n-1] + og_last;

  printf("finished prefix_sum in %i\n",omp_get_thread_num());

}
```
</details>

###### > prefix_sum paralelizada
      
<details>
    <summary>Expandir Código</summary>
    
```cpp
void scan_left(double *C, int n, double &x)
{
  // printf("before scan:\n");
  // for (int i = 0; i < n; i++)
  // {
  //   printf("%f,", C[i]);
  // }
  // printf("\n");

  long dmax = log2(n - 1);
  for (long d = 0; d <= dmax; d++)
  {
  #pragma omp parallel for
    for (long long k = 0; k < n; k += (1 << (d + 1)))
    {
      C[k + (1 << (d + 1)) - 1] = C[k + (1 << d) - 1] + C[k + (1 << (d + 1)) - 1];
    }
  }

  // printf("after scan:\n");
  // for (int i = 0; i < n; i++)
  // {
  //   printf("%f,", C[i]);
  // }
  // printf("\n");

  x += C[n - 1];
  printf("finished scan_left in %i\n",omp_get_thread_num());
}
```
</details>

###### > scan_left paralelizada
    
También se paraleliza los fors sobre la matriz *A* y *C* creando la siguiente función. Importante denotar que para evitar el overflow de los arrays de doubles, se modificó las operaciones para aplicar un *logaritmo base 10* a los valores, y se cambió la multiplicación del segundo for por una suma. 
###### Este error de overflow estaba ya presente en la versión secuencial, y se realizó este cambio para poder analizar correctamente los algoritmos.
    
<details>
    <summary>Expandir Código</summary>
    
```cpp
double multMod(double *A, double *B, int n)
{
  printf("hilo en multmod: %i\n",
        omp_get_thread_num());

  double s1;
  for (int i = 0; i < n; i++)
  { 
    s1 = 0;
  #pragma omp parallel for reduction(+ \
                                   : s1)
    for (int j = 0; j < n; j++){
      s1 += (log10(A[j] * B[j])); 
    }
    // barrera implicita
  #pragma omp parallel for
    for (int j = 0; j < n; j++)
      A[j] += (log10(s1));
  }

  return s1;
}
```
</details>
    
###### > multmod paralelizada

Finalmente se ejecuta la función *OpArr()* de manera paralelizada, dividiendo la ejecución del código en tasks, empezando por ejecutar las funciones *sumadeprefijos()*, *pqs()* y *scanleft()* con las arrays *A*, *B*, *C* respectivamente en paralelo, cada uno en una thread distinta *(cada función crea subthreads durante su ejecución)*. Las siguientes tasks tienen una dependencia sobre los arrays *A*, *B* para *s1* y *C*, *B* para *s2*, las cuales seran computadas en dos threads distintas una vez cumplidas las dependencias, de la misma manera creandose subthreads durante su ejecucion independiente. Luego de esto, se ejecuta la division *s1/s2* y el parallel for final como tasks tambien con dependencias.
    
Para las primeras tres tasks, estas pueden correr de manera totalmente independiente, pues cada una modifica solamente uno de los tres arrays durante su ejecución. De esta misma manera, es importante mantener estas dependencias en las siguientes dos tareas paralelas, ya que estas dependen de una array para la escritura y otra para la lectura. 
    
<details>
    <summary>Expandir Código</summary>
    
```cpp
double OpArr(double A[], double B[], double C[], int n)
{
  int i, j;
  double s1, s2, a, res;
  double x = 1.0;
  #pragma omp parallel default(shared)
  {
    #pragma omp single nowait
    {
      
      #pragma omp task depend(out:A)
        suma_de_prefijos(A, n); // obtiene el array de suma de prefijos de A
      #pragma omp task depend(out:B)
        pqs(B, n); // ordena el array B
      #pragma omp task depend(out:C)
        scan_left(C, n, x); // acumula los valores de elementos de C mas una constante x
      
      #pragma omp task depend(in:A,B) depend(out:s1)
          s1 = multMod(A, B, n);
      #pragma omp task depend(in:C,B) depend(out:s2)
          s2 = multMod(C, B, n);

      #pragma omp task depend(in:s1,s2) depend(out:a)
      {
        a = s1 / s2;
      }
      
      #pragma omp task depend(in:a)
      {
        res = 0;
        #pragma omp parallel for reduction(+ \
                                          : res)
        for (int i = 0; i < n; i++){
          res += a + C[i];
        }
      }

    }
  }
  return res;
}
```
</details>
    
###### > OpArr paralelizada
                
                        
### Item c)
#### Grafique tiempos de cálculo para *n=106* vs *p=1,2,4,8, ...* Haga un análisis cuantitativo del speedup y eficiencia del algoritmo y derive una expresión analítica en función a *n* y *p* para estas métricas.

Se genera la siguiente tabla, con los tiempos promedio de ejecución (100 pruebas):
    
|   processors   |        average(s)  | 
-------------------|-----------------
|         1      |   0.00297451999999999959 |
|         2      |   0.02452973999999999780 | 
|         3      |   0.04648878000000000732 |
|         4      |   0.07961444999999998939 |
|         5      |   0.11913736999999999244 |
|         6      |   0.17209273000000005482 |
|         7      |   0.23209903000000001172 |
|         8      |   0.28122575999999993579 |
    
![](https://i.imgur.com/qCfvg9m.png)
    
Con la implementación actual de la función *OpArr()*, el running time del algoritmo parece incrementar conforme se añaden threads. Esto parece depender completamente de la función *multMod()*, la cual es muy pesada y resulta dificil paralelizarla correctamente. Si movemos la función afuera de los tasks y las ejecutamos secuencialmente (igual con tasks dentro), el algoritmo presenta un mejor desempeño conforme crecen las threads, como se ve en la siguientes tablas:

| ![](https://i.imgur.com/XkQqvu1.png) | 
|:--:| 
| N=106 |

| ![](https://i.imgur.com/xjisfe4.png) | 
|:--:| 
| N=4096 |

|![](https://i.imgur.com/6Q6JVz0.png)| 
|:--:| 
| cambios en OpArray() |

###### > En el codigo adjunto, esta version cambiada se encuentra adicional como comentario.

Complejidad secuencial:
$$T_1 = O(n^2)$$

Se calculó la complejidad en base al teorema de Brent:
    
$$T_p = O(\frac {n^2 log^2 n} {p}) $$
 
Se calculó el Speedup
    
$$ S(p) = \frac{T_1 (n)}{T_p (n)} = \frac{ n^2}{\frac {n^2 log^2 n} {p}} = \frac{p}{log^2(n)} $$
    
Y la eficiencia:
$$ E(p) = \frac{\frac{p}{log^2(n)}}{p} = \frac{1}{log^2(n)} $$
    
    
### Item d)
#### Compare teoría con experimento y analice en que medida ambos coinciden (se obtiene la complejidad teorica esperada)

Como se puede ver la eficiencia no escala con p, por lo que se tiene un algoritmo no escalable.
    
Por otro lado, como se puede ver en la gráfica mostrada, el algoritmo en ejecución escala sin problemas para el valor de n = 106 que fue establecido. Esto muestra un algoritmo con escalabilidad fuerte, pues mientras más procesos se le añaden, disminuye el tiempo de ejecución.
    
Esto concuerda con lo calculado en el Speedup y en la Eficiencia, última fórmula dependiendo únicamente del valor de N, mas no de p.
    

