#include "arvore_bst.h"

#include <stdlib.h>

typedef struct No {
    int chave;
    struct No *esquerda;
    struct No *direita;
} No;

struct ArvoreBST {
    No *raiz;
    size_t tamanho;
};

ArvoreBST *bst_criar(void) {
    ArvoreBST *a = calloc(1, sizeof *a);
    if (!a) abort();
    return a;
}

static void destruir_nos(No *no) {
    if (!no) return;
    destruir_nos(no->esquerda);
    destruir_nos(no->direita);
    free(no);
}

void bst_destruir(ArvoreBST *a) {
    if (!a) return;
    destruir_nos(a->raiz);
    free(a);
}

bool bst_inserir(ArvoreBST *a, int chave) {
    /* Versão iterativa com ponteiro para ponteiro: evita tratar a raiz como caso especial. */
    No **link = &a->raiz;
    while (*link) {
        if (chave == (*link)->chave) return false;
        link = chave < (*link)->chave ? &(*link)->esquerda : &(*link)->direita;
    }
    No *no = calloc(1, sizeof *no);
    if (!no) abort();
    no->chave = chave;
    *link = no;
    a->tamanho++;
    return true;
}

bool bst_buscar(const ArvoreBST *a, int chave) {
    const No *atual = a->raiz;
    while (atual) {
        if (chave == atual->chave) return true;
        atual = chave < atual->chave ? atual->esquerda : atual->direita;
    }
    return false;
}

bool bst_remover(ArvoreBST *a, int chave) {
    No **link = &a->raiz;
    while (*link && (*link)->chave != chave) {
        link = chave < (*link)->chave ? &(*link)->esquerda : &(*link)->direita;
    }
    No *alvo = *link;
    if (!alvo) return false;

    if (alvo->esquerda && alvo->direita) {
        /* Dois filhos: copia o sucessor (menor da subárvore direita) e remove ele. */
        No **link_sucessor = &alvo->direita;
        while ((*link_sucessor)->esquerda) link_sucessor = &(*link_sucessor)->esquerda;
        No *sucessor = *link_sucessor;
        alvo->chave = sucessor->chave;
        *link_sucessor = sucessor->direita;
        free(sucessor);
    } else {
        /* Zero ou um filho: o filho (ou NULL) ocupa o lugar do nó. */
        *link = alvo->esquerda ? alvo->esquerda : alvo->direita;
        free(alvo);
    }
    a->tamanho--;
    return true;
}

bool bst_minimo(const ArvoreBST *a, int *saida) {
    const No *atual = a->raiz;
    if (!atual) return false;
    while (atual->esquerda) atual = atual->esquerda;
    *saida = atual->chave;
    return true;
}

bool bst_maximo(const ArvoreBST *a, int *saida) {
    const No *atual = a->raiz;
    if (!atual) return false;
    while (atual->direita) atual = atual->direita;
    *saida = atual->chave;
    return true;
}

size_t bst_tamanho(const ArvoreBST *a) { return a->tamanho; }

static int altura(const No *no) {
    if (!no) return 0;
    int e = altura(no->esquerda), d = altura(no->direita);
    return 1 + (e > d ? e : d);
}

int bst_altura(const ArvoreBST *a) { return altura(a->raiz); }

static void em_ordem(const No *no, int *vetor, size_t *i) {
    if (!no) return;
    em_ordem(no->esquerda, vetor, i);
    vetor[(*i)++] = no->chave;
    em_ordem(no->direita, vetor, i);
}

static void pre_ordem(const No *no, int *vetor, size_t *i) {
    if (!no) return;
    vetor[(*i)++] = no->chave;
    pre_ordem(no->esquerda, vetor, i);
    pre_ordem(no->direita, vetor, i);
}

static void pos_ordem(const No *no, int *vetor, size_t *i) {
    if (!no) return;
    pos_ordem(no->esquerda, vetor, i);
    pos_ordem(no->direita, vetor, i);
    vetor[(*i)++] = no->chave;
}

void bst_em_ordem(const ArvoreBST *a, int *vetor) { size_t i = 0; em_ordem(a->raiz, vetor, &i); }
void bst_pre_ordem(const ArvoreBST *a, int *vetor) { size_t i = 0; pre_ordem(a->raiz, vetor, &i); }
void bst_pos_ordem(const ArvoreBST *a, int *vetor) { size_t i = 0; pos_ordem(a->raiz, vetor, &i); }

void bst_em_largura(const ArvoreBST *a, int *vetor) {
    if (!a->raiz) return;
    /* Fila simples de ponteiros: cada nó entra exatamente uma vez. */
    const No **fila = malloc(a->tamanho * sizeof *fila);
    if (!fila) abort();
    size_t inicio = 0, fim = 0, i = 0;
    fila[fim++] = a->raiz;
    while (inicio < fim) {
        const No *no = fila[inicio++];
        vetor[i++] = no->chave;
        if (no->esquerda) fila[fim++] = no->esquerda;
        if (no->direita) fila[fim++] = no->direita;
    }
    free(fila);
}
