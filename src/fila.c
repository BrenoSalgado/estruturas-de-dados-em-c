#include "fila.h"

#include <stdlib.h>

#define CAPACIDADE_INICIAL 8

struct Fila {
    int *dados;
    size_t inicio;     /* índice do primeiro elemento */
    size_t tamanho;
    size_t capacidade;
};

Fila *fila_criar(void) {
    Fila *f = malloc(sizeof *f);
    if (!f) abort();
    f->dados = malloc(CAPACIDADE_INICIAL * sizeof *f->dados);
    if (!f->dados) abort();
    f->inicio = 0;
    f->tamanho = 0;
    f->capacidade = CAPACIDADE_INICIAL;
    return f;
}

void fila_destruir(Fila *f) {
    if (!f) return;
    free(f->dados);
    free(f);
}

/* Copia os elementos em ordem para um vetor maior, "desenrolando" o círculo. */
static void crescer(Fila *f) {
    size_t nova = f->capacidade * 2;
    int *dados = malloc(nova * sizeof *dados);
    if (!dados) abort();
    for (size_t i = 0; i < f->tamanho; i++) {
        dados[i] = f->dados[(f->inicio + i) % f->capacidade];
    }
    free(f->dados);
    f->dados = dados;
    f->inicio = 0;
    f->capacidade = nova;
}

void fila_enfileirar(Fila *f, int valor) {
    if (f->tamanho == f->capacidade) crescer(f);
    f->dados[(f->inicio + f->tamanho) % f->capacidade] = valor;
    f->tamanho++;
}

bool fila_desenfileirar(Fila *f, int *saida) {
    if (f->tamanho == 0) return false;
    if (saida) *saida = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return true;
}

bool fila_frente(const Fila *f, int *saida) {
    if (f->tamanho == 0) return false;
    *saida = f->dados[f->inicio];
    return true;
}

size_t fila_tamanho(const Fila *f) { return f->tamanho; }
bool fila_vazia(const Fila *f) { return f->tamanho == 0; }
