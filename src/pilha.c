#include "pilha.h"

#include <stdlib.h>

#define CAPACIDADE_INICIAL 8

struct Pilha {
    int *dados;
    size_t tamanho;
    size_t capacidade;
};

Pilha *pilha_criar(void) {
    Pilha *p = malloc(sizeof *p);
    if (!p) abort();
    p->dados = malloc(CAPACIDADE_INICIAL * sizeof *p->dados);
    if (!p->dados) abort();
    p->tamanho = 0;
    p->capacidade = CAPACIDADE_INICIAL;
    return p;
}

void pilha_destruir(Pilha *p) {
    if (!p) return;
    free(p->dados);
    free(p);
}

void pilha_empilhar(Pilha *p, int valor) {
    if (p->tamanho == p->capacidade) {
        size_t nova = p->capacidade * 2;
        int *dados = realloc(p->dados, nova * sizeof *dados);
        if (!dados) abort();
        p->dados = dados;
        p->capacidade = nova;
    }
    p->dados[p->tamanho++] = valor;
}

bool pilha_desempilhar(Pilha *p, int *saida) {
    if (p->tamanho == 0) return false;
    p->tamanho--;
    if (saida) *saida = p->dados[p->tamanho];
    return true;
}

bool pilha_topo(const Pilha *p, int *saida) {
    if (p->tamanho == 0) return false;
    *saida = p->dados[p->tamanho - 1];
    return true;
}

size_t pilha_tamanho(const Pilha *p) { return p->tamanho; }
bool pilha_vazia(const Pilha *p) { return p->tamanho == 0; }

bool expressao_balanceada(const char *expressao) {
    Pilha *p = pilha_criar();
    bool ok = true;

    for (const char *c = expressao; *c && ok; c++) {
        if (*c == '(' || *c == '[' || *c == '{') {
            pilha_empilhar(p, *c);
        } else if (*c == ')' || *c == ']' || *c == '}') {
            int aberto;
            char esperado = *c == ')' ? '(' : *c == ']' ? '[' : '{';
            ok = pilha_desempilhar(p, &aberto) && aberto == esperado;
        }
    }

    ok = ok && pilha_vazia(p);
    pilha_destruir(p);
    return ok;
}
