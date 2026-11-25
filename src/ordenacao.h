#ifndef ORDENACAO_H
#define ORDENACAO_H

#include <stdbool.h>
#include <stddef.h>

/* Algoritmos de ordenação (ordem crescente, in-place quando possível) e busca binária. */

void bubble_sort(int *v, size_t n);
void selection_sort(int *v, size_t n);
void insertion_sort(int *v, size_t n);
void merge_sort(int *v, size_t n);
void quick_sort(int *v, size_t n);
void heap_sort(int *v, size_t n);
/* Counting sort: O(n + k), onde k = max - min. Indicado quando o intervalo de valores é pequeno. */
void counting_sort(int *v, size_t n);

bool esta_ordenado(const int *v, size_t n);

/* Retorna o índice de alvo em v (ordenado), ou -1 se não existir. O(log n). */
long busca_binaria(const int *v, size_t n, int alvo);

#endif
