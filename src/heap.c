#include "heap.h"

#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_INICIAL 8

struct Heap {
    int *dados;
    size_t tamanho;
    size_t capacidade;
};

static void trocar(int *a, int *b) { int t = *a; *a = *b; *b = t; }

static void subir(int *v, size_t i) {
    while (i > 0) {
        size_t pai = (i - 1) / 2;
        if (v[pai] <= v[i]) break;
        trocar(&v[pai], &v[i]);
        i = pai;
    }
}

static void descer(int *v, size_t n, size_t i) {
    for (;;) {
        size_t menor = i, e = 2 * i + 1, d = 2 * i + 2;
        if (e < n && v[e] < v[menor]) menor = e;
        if (d < n && v[d] < v[menor]) menor = d;
        if (menor == i) return;
        trocar(&v[i], &v[menor]);
        i = menor;
    }
}

static Heap *alocar(size_t capacidade) {
    Heap *h = malloc(sizeof *h);
    if (!h) abort();
    h->dados = malloc(capacidade * sizeof *h->dados);
    if (!h->dados) abort();
    h->tamanho = 0;
    h->capacidade = capacidade;
    return h;
}

Heap *heap_criar(void) { return alocar(CAPACIDADE_INICIAL); }

Heap *heap_de_vetor(const int *vetor, size_t n) {
    Heap *h = alocar(n > CAPACIDADE_INICIAL ? n : CAPACIDADE_INICIAL);
    if (n > 0) memcpy(h->dados, vetor, n * sizeof *vetor);
    h->tamanho = n;
    /* Folhas já são heaps; basta descer a partir do último nó interno. */
    for (size_t i = n / 2; i-- > 0;) descer(h->dados, n, i);
    return h;
}

void heap_destruir(Heap *h) {
    if (!h) return;
    free(h->dados);
    free(h);
}

void heap_inserir(Heap *h, int valor) {
    if (h->tamanho == h->capacidade) {
        size_t nova = h->capacidade * 2;
        int *dados = realloc(h->dados, nova * sizeof *dados);
        if (!dados) abort();
        h->dados = dados;
        h->capacidade = nova;
    }
    h->dados[h->tamanho] = valor;
    subir(h->dados, h->tamanho);
    h->tamanho++;
}

bool heap_remover_minimo(Heap *h, int *saida) {
    if (h->tamanho == 0) return false;
    if (saida) *saida = h->dados[0];
    h->dados[0] = h->dados[--h->tamanho];
    descer(h->dados, h->tamanho, 0);
    return true;
}

bool heap_minimo(const Heap *h, int *saida) {
    if (h->tamanho == 0) return false;
    *saida = h->dados[0];
    return true;
}

size_t heap_tamanho(const Heap *h) { return h->tamanho; }
bool heap_vazio(const Heap *h) { return h->tamanho == 0; }
