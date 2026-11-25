#include "tabela_hash.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_INICIAL 16
#define CARGA_MAXIMA_NUM 3   /* fator de carga máximo = 3/4 */
#define CARGA_MAXIMA_DEN 4

typedef struct Entrada {
    char *chave;
    int valor;
    struct Entrada *proxima;
} Entrada;

struct TabelaHash {
    Entrada **buckets;
    size_t capacidade;
    size_t tamanho;
};

/* FNV-1a de 64 bits: simples, rápida e com boa dispersão para strings. */
static uint64_t fnv1a(const char *s) {
    uint64_t h = 14695981039346656037ULL;
    for (; *s; s++) {
        h ^= (unsigned char)*s;
        h *= 1099511628211ULL;
    }
    return h;
}

static size_t indice(const char *chave, size_t capacidade) {
    return (size_t)(fnv1a(chave) % capacidade);
}

static char *copiar(const char *s) {
    size_t n = strlen(s) + 1;
    char *c = malloc(n);
    if (!c) abort();
    memcpy(c, s, n);
    return c;
}

TabelaHash *hash_criar(void) {
    TabelaHash *t = malloc(sizeof *t);
    if (!t) abort();
    t->buckets = calloc(CAPACIDADE_INICIAL, sizeof *t->buckets);
    if (!t->buckets) abort();
    t->capacidade = CAPACIDADE_INICIAL;
    t->tamanho = 0;
    return t;
}

void hash_destruir(TabelaHash *t) {
    if (!t) return;
    for (size_t i = 0; i < t->capacidade; i++) {
        Entrada *e = t->buckets[i];
        while (e) {
            Entrada *proxima = e->proxima;
            free(e->chave);
            free(e);
            e = proxima;
        }
    }
    free(t->buckets);
    free(t);
}

static void redimensionar(TabelaHash *t, size_t nova_capacidade) {
    Entrada **novos = calloc(nova_capacidade, sizeof *novos);
    if (!novos) abort();
    for (size_t i = 0; i < t->capacidade; i++) {
        Entrada *e = t->buckets[i];
        while (e) {
            Entrada *proxima = e->proxima;
            size_t j = indice(e->chave, nova_capacidade);
            e->proxima = novos[j];
            novos[j] = e;
            e = proxima;
        }
    }
    free(t->buckets);
    t->buckets = novos;
    t->capacidade = nova_capacidade;
}

static Entrada *encontrar(const TabelaHash *t, const char *chave) {
    for (Entrada *e = t->buckets[indice(chave, t->capacidade)]; e; e = e->proxima) {
        if (strcmp(e->chave, chave) == 0) return e;
    }
    return NULL;
}

void hash_inserir(TabelaHash *t, const char *chave, int valor) {
    Entrada *existente = encontrar(t, chave);
    if (existente) {
        existente->valor = valor;
        return;
    }
    if ((t->tamanho + 1) * CARGA_MAXIMA_DEN > t->capacidade * CARGA_MAXIMA_NUM) {
        redimensionar(t, t->capacidade * 2);
    }
    Entrada *e = malloc(sizeof *e);
    if (!e) abort();
    e->chave = copiar(chave);
    e->valor = valor;
    size_t i = indice(chave, t->capacidade);
    e->proxima = t->buckets[i];
    t->buckets[i] = e;
    t->tamanho++;
}

bool hash_obter(const TabelaHash *t, const char *chave, int *saida) {
    Entrada *e = encontrar(t, chave);
    if (!e) return false;
    *saida = e->valor;
    return true;
}

bool hash_contem(const TabelaHash *t, const char *chave) { return encontrar(t, chave) != NULL; }

bool hash_remover(TabelaHash *t, const char *chave) {
    Entrada **link = &t->buckets[indice(chave, t->capacidade)];
    while (*link) {
        if (strcmp((*link)->chave, chave) == 0) {
            Entrada *e = *link;
            *link = e->proxima;
            free(e->chave);
            free(e);
            t->tamanho--;
            return true;
        }
        link = &(*link)->proxima;
    }
    return false;
}

size_t hash_tamanho(const TabelaHash *t) { return t->tamanho; }
size_t hash_capacidade(const TabelaHash *t) { return t->capacidade; }
