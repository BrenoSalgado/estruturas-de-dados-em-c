#ifndef ARVORE_BST_H
#define ARVORE_BST_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Árvore binária de busca (BST) de inteiros, sem chaves repetidas.
 *
 * Invariante: para todo nó, as chaves da subárvore esquerda são menores e as da
 * direita são maiores. As operações custam O(h), onde h é a altura — O(log n)
 * em média, mas O(n) no pior caso (inserções já ordenadas). A AVL resolve isso.
 */

typedef struct ArvoreBST ArvoreBST;

ArvoreBST *bst_criar(void);
void bst_destruir(ArvoreBST *a);

/* Retorna false se a chave já existir. */
bool bst_inserir(ArvoreBST *a, int chave);
bool bst_buscar(const ArvoreBST *a, int chave);
bool bst_remover(ArvoreBST *a, int chave);

bool bst_minimo(const ArvoreBST *a, int *saida);
bool bst_maximo(const ArvoreBST *a, int *saida);

size_t bst_tamanho(const ArvoreBST *a);
/* Altura de uma árvore vazia é 0; de uma árvore com um nó é 1. */
int bst_altura(const ArvoreBST *a);

/* Percursos: preenchem vetor (com espaço para bst_tamanho(a) itens). */
void bst_em_ordem(const ArvoreBST *a, int *vetor);
void bst_pre_ordem(const ArvoreBST *a, int *vetor);
void bst_pos_ordem(const ArvoreBST *a, int *vetor);
void bst_em_largura(const ArvoreBST *a, int *vetor);

#endif
