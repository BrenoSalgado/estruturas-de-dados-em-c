#ifndef PILHA_H
#define PILHA_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Pilha (LIFO) sobre vetor dinâmico.
 *
 * O vetor dobra de capacidade quando enche, então empilhar custa O(1) amortizado.
 */

typedef struct Pilha Pilha;

Pilha *pilha_criar(void);
void pilha_destruir(Pilha *p);

void pilha_empilhar(Pilha *p, int valor);
bool pilha_desempilhar(Pilha *p, int *saida);
bool pilha_topo(const Pilha *p, int *saida);

size_t pilha_tamanho(const Pilha *p);
bool pilha_vazia(const Pilha *p);

/* Aplicação clássica: verifica se (), [] e {} estão balanceados na expressão. */
bool expressao_balanceada(const char *expressao);

#endif
