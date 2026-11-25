#include "arvore_avl.h"

#include <limits.h>
#include <stdlib.h>

typedef struct No {
    int chave;
    int altura;
    struct No *esquerda;
    struct No *direita;
} No;

struct ArvoreAVL {
    No *raiz;
    size_t tamanho;
};

static int altura(const No *no) { return no ? no->altura : 0; }
static int maior(int a, int b) { return a > b ? a : b; }

static void atualizar_altura(No *no) {
    no->altura = 1 + maior(altura(no->esquerda), altura(no->direita));
}

static int fator(const No *no) { return altura(no->esquerda) - altura(no->direita); }

/*
 *      y                x
 *     / \             /   \
 *    x   C   --->    A     y
 *   / \                   / \
 *  A   B                 B   C
 */
static No *rotacao_direita(No *y) {
    No *x = y->esquerda;
    y->esquerda = x->direita;
    x->direita = y;
    atualizar_altura(y);
    atualizar_altura(x);
    return x;
}

static No *rotacao_esquerda(No *x) {
    No *y = x->direita;
    x->direita = y->esquerda;
    y->esquerda = x;
    atualizar_altura(x);
    atualizar_altura(y);
    return y;
}

/* Restaura o balanceamento de um nó cujas subárvores já estão balanceadas. */
static No *balancear(No *no) {
    atualizar_altura(no);
    int f = fator(no);
    if (f > 1) {                                   /* pesado à esquerda */
        if (fator(no->esquerda) < 0)               /* caso esquerda-direita */
            no->esquerda = rotacao_esquerda(no->esquerda);
        return rotacao_direita(no);
    }
    if (f < -1) {                                  /* pesado à direita */
        if (fator(no->direita) > 0)                /* caso direita-esquerda */
            no->direita = rotacao_direita(no->direita);
        return rotacao_esquerda(no);
    }
    return no;
}

ArvoreAVL *avl_criar(void) {
    ArvoreAVL *a = calloc(1, sizeof *a);
    if (!a) abort();
    return a;
}

static void destruir_nos(No *no) {
    if (!no) return;
    destruir_nos(no->esquerda);
    destruir_nos(no->direita);
    free(no);
}

void avl_destruir(ArvoreAVL *a) {
    if (!a) return;
    destruir_nos(a->raiz);
    free(a);
}

static No *inserir(No *no, int chave, bool *inseriu) {
    if (!no) {
        No *novo = calloc(1, sizeof *novo);
        if (!novo) abort();
        novo->chave = chave;
        novo->altura = 1;
        *inseriu = true;
        return novo;
    }
    if (chave < no->chave) no->esquerda = inserir(no->esquerda, chave, inseriu);
    else if (chave > no->chave) no->direita = inserir(no->direita, chave, inseriu);
    else return no; /* chave repetida */
    return balancear(no);
}

bool avl_inserir(ArvoreAVL *a, int chave) {
    bool inseriu = false;
    a->raiz = inserir(a->raiz, chave, &inseriu);
    if (inseriu) a->tamanho++;
    return inseriu;
}

bool avl_buscar(const ArvoreAVL *a, int chave) {
    const No *atual = a->raiz;
    while (atual) {
        if (chave == atual->chave) return true;
        atual = chave < atual->chave ? atual->esquerda : atual->direita;
    }
    return false;
}

static No *remover_minimo(No *no, No **minimo) {
    if (!no->esquerda) {
        *minimo = no;
        return no->direita;
    }
    no->esquerda = remover_minimo(no->esquerda, minimo);
    return balancear(no);
}

static No *remover(No *no, int chave, bool *removeu) {
    if (!no) return NULL;
    if (chave < no->chave) {
        no->esquerda = remover(no->esquerda, chave, removeu);
    } else if (chave > no->chave) {
        no->direita = remover(no->direita, chave, removeu);
    } else {
        *removeu = true;
        No *esquerda = no->esquerda, *direita = no->direita;
        free(no);
        if (!direita) return esquerda;
        /* Substitui pelo sucessor (menor da subárvore direita). */
        No *sucessor;
        direita = remover_minimo(direita, &sucessor);
        sucessor->esquerda = esquerda;
        sucessor->direita = direita;
        return balancear(sucessor);
    }
    return balancear(no);
}

bool avl_remover(ArvoreAVL *a, int chave) {
    bool removeu = false;
    a->raiz = remover(a->raiz, chave, &removeu);
    if (removeu) a->tamanho--;
    return removeu;
}

size_t avl_tamanho(const ArvoreAVL *a) { return a->tamanho; }
int avl_altura(const ArvoreAVL *a) { return altura(a->raiz); }

static void em_ordem(const No *no, int *vetor, size_t *i) {
    if (!no) return;
    em_ordem(no->esquerda, vetor, i);
    vetor[(*i)++] = no->chave;
    em_ordem(no->direita, vetor, i);
}

void avl_em_ordem(const ArvoreAVL *a, int *vetor) {
    size_t i = 0;
    em_ordem(a->raiz, vetor, &i);
}

/* Retorna a altura real da subárvore, ou -1 se alguma propriedade for violada. */
static int validar(const No *no, long long minimo, long long maximo) {
    if (!no) return 0;
    if (no->chave <= minimo || no->chave >= maximo) return -1;
    int e = validar(no->esquerda, minimo, no->chave);
    int d = validar(no->direita, no->chave, maximo);
    if (e < 0 || d < 0) return -1;
    if (e - d > 1 || d - e > 1) return -1;
    int h = 1 + maior(e, d);
    return h == no->altura ? h : -1;
}

bool avl_valida(const ArvoreAVL *a) {
    return validar(a->raiz, (long long)INT_MIN - 1, (long long)INT_MAX + 1) >= 0;
}
