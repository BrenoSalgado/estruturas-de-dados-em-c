#ifndef ARVORE_AVL_H
#define ARVORE_AVL_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Árvore AVL: BST autobalanceada.
 *
 * Em todo nó, a diferença de altura entre as subárvores (fator de balanceamento)
 * fica entre -1 e +1. Depois de cada inserção/remoção, rotações simples ou duplas
 * restauram essa propriedade, garantindo altura O(log n) e operações O(log n)
 * mesmo no pior caso.
 */

typedef struct ArvoreAVL ArvoreAVL;

ArvoreAVL *avl_criar(void);
void avl_destruir(ArvoreAVL *a);

bool avl_inserir(ArvoreAVL *a, int chave);
bool avl_buscar(const ArvoreAVL *a, int chave);
bool avl_remover(ArvoreAVL *a, int chave);

size_t avl_tamanho(const ArvoreAVL *a);
int avl_altura(const ArvoreAVL *a);
void avl_em_ordem(const ArvoreAVL *a, int *vetor);

/* Confere se a árvore é uma BST válida, balanceada e com alturas corretas (usada nos testes). */
bool avl_valida(const ArvoreAVL *a);

#endif
