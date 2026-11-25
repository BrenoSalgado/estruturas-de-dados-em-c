#include "lista.h"

#include <stdlib.h>

typedef struct No {
    int valor;
    struct No *anterior;
    struct No *proximo;
} No;

struct Lista {
    No *inicio;
    No *fim;
    size_t tamanho;
};

static No *no_criar(int valor) {
    No *no = malloc(sizeof *no);
    if (!no) abort();
    no->valor = valor;
    no->anterior = no->proximo = NULL;
    return no;
}

Lista *lista_criar(void) {
    Lista *l = calloc(1, sizeof *l);
    if (!l) abort();
    return l;
}

void lista_destruir(Lista *l) {
    if (!l) return;
    No *atual = l->inicio;
    while (atual) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    free(l);
}

size_t lista_tamanho(const Lista *l) { return l->tamanho; }
bool lista_vazia(const Lista *l) { return l->tamanho == 0; }

void lista_inserir_inicio(Lista *l, int valor) {
    No *no = no_criar(valor);
    no->proximo = l->inicio;
    if (l->inicio) l->inicio->anterior = no;
    else l->fim = no;
    l->inicio = no;
    l->tamanho++;
}

void lista_inserir_fim(Lista *l, int valor) {
    No *no = no_criar(valor);
    no->anterior = l->fim;
    if (l->fim) l->fim->proximo = no;
    else l->inicio = no;
    l->fim = no;
    l->tamanho++;
}

/* Percorre a partir da ponta mais próxima da posição pedida. */
static No *no_na_posicao(const Lista *l, size_t posicao) {
    No *atual;
    if (posicao < l->tamanho / 2) {
        atual = l->inicio;
        for (size_t i = 0; i < posicao; i++) atual = atual->proximo;
    } else {
        atual = l->fim;
        for (size_t i = l->tamanho - 1; i > posicao; i--) atual = atual->anterior;
    }
    return atual;
}

bool lista_inserir_em(Lista *l, size_t posicao, int valor) {
    if (posicao > l->tamanho) return false;
    if (posicao == 0) { lista_inserir_inicio(l, valor); return true; }
    if (posicao == l->tamanho) { lista_inserir_fim(l, valor); return true; }

    No *depois = no_na_posicao(l, posicao);
    No *antes = depois->anterior;
    No *no = no_criar(valor);
    no->anterior = antes;
    no->proximo = depois;
    antes->proximo = no;
    depois->anterior = no;
    l->tamanho++;
    return true;
}

static void desligar_no(Lista *l, No *no) {
    if (no->anterior) no->anterior->proximo = no->proximo;
    else l->inicio = no->proximo;
    if (no->proximo) no->proximo->anterior = no->anterior;
    else l->fim = no->anterior;
    free(no);
    l->tamanho--;
}

bool lista_remover_inicio(Lista *l, int *saida) {
    if (!l->inicio) return false;
    if (saida) *saida = l->inicio->valor;
    desligar_no(l, l->inicio);
    return true;
}

bool lista_remover_fim(Lista *l, int *saida) {
    if (!l->fim) return false;
    if (saida) *saida = l->fim->valor;
    desligar_no(l, l->fim);
    return true;
}

bool lista_remover_valor(Lista *l, int valor) {
    for (No *atual = l->inicio; atual; atual = atual->proximo) {
        if (atual->valor == valor) {
            desligar_no(l, atual);
            return true;
        }
    }
    return false;
}

bool lista_obter(const Lista *l, size_t posicao, int *saida) {
    if (posicao >= l->tamanho) return false;
    *saida = no_na_posicao(l, posicao)->valor;
    return true;
}

long lista_buscar(const Lista *l, int valor) {
    long i = 0;
    for (No *atual = l->inicio; atual; atual = atual->proximo, i++) {
        if (atual->valor == valor) return i;
    }
    return -1;
}

void lista_inverter(Lista *l) {
    No *atual = l->inicio;
    while (atual) {
        No *proximo = atual->proximo;
        atual->proximo = atual->anterior;
        atual->anterior = proximo;
        atual = proximo;
    }
    No *antigo_inicio = l->inicio;
    l->inicio = l->fim;
    l->fim = antigo_inicio;
}

void lista_para_vetor(const Lista *l, int *vetor) {
    size_t i = 0;
    for (No *atual = l->inicio; atual; atual = atual->proximo) vetor[i++] = atual->valor;
}
