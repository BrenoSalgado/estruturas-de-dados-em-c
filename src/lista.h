#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Lista duplamente encadeada de inteiros.
 *
 * Cada nó aponta para o anterior e para o próximo, e a lista guarda ponteiros
 * para o início e o fim. Isso permite inserir e remover nas duas pontas em O(1).
 */

typedef struct Lista Lista;

Lista *lista_criar(void);
void lista_destruir(Lista *l);

size_t lista_tamanho(const Lista *l);
bool lista_vazia(const Lista *l);

void lista_inserir_inicio(Lista *l, int valor);
void lista_inserir_fim(Lista *l, int valor);
/* Insere na posição indicada (0 = início). Retorna false se a posição for inválida. */
bool lista_inserir_em(Lista *l, size_t posicao, int valor);

/* Removem e guardam o valor removido em *saida (se saida != NULL). */
bool lista_remover_inicio(Lista *l, int *saida);
bool lista_remover_fim(Lista *l, int *saida);
/* Remove a primeira ocorrência de valor. */
bool lista_remover_valor(Lista *l, int valor);

bool lista_obter(const Lista *l, size_t posicao, int *saida);
/* Retorna o índice da primeira ocorrência de valor, ou -1. */
long lista_buscar(const Lista *l, int valor);

void lista_inverter(Lista *l);
/* Copia os elementos para vetor (que deve ter espaço para lista_tamanho(l) itens). */
void lista_para_vetor(const Lista *l, int *vetor);

#endif
