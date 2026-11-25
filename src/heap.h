#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Heap binário de mínimo (fila de prioridade).
 *
 * Árvore binária completa guardada em vetor: os filhos de i estão em 2i+1 e 2i+2,
 * e todo pai é menor ou igual aos filhos. O menor elemento fica sempre em [0].
 */

typedef struct Heap Heap;

Heap *heap_criar(void);
/* Constrói o heap a partir de um vetor em O(n) (heapify de baixo para cima). */
Heap *heap_de_vetor(const int *vetor, size_t n);
void heap_destruir(Heap *h);

void heap_inserir(Heap *h, int valor);
bool heap_remover_minimo(Heap *h, int *saida);
bool heap_minimo(const Heap *h, int *saida);

size_t heap_tamanho(const Heap *h);
bool heap_vazio(const Heap *h);

#endif
