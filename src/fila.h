#ifndef FILA_H
#define FILA_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Fila (FIFO) em buffer circular que cresce sob demanda.
 *
 * Os índices de início e fim "dão a volta" no vetor (módulo da capacidade),
 * então enfileirar e desenfileirar são O(1) sem precisar deslocar elementos.
 */

typedef struct Fila Fila;

Fila *fila_criar(void);
void fila_destruir(Fila *f);

void fila_enfileirar(Fila *f, int valor);
bool fila_desenfileirar(Fila *f, int *saida);
bool fila_frente(const Fila *f, int *saida);

size_t fila_tamanho(const Fila *f);
bool fila_vazia(const Fila *f);

#endif
