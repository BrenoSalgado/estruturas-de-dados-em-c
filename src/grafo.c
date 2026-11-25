#include "grafo.h"

#include <stdlib.h>

typedef struct Aresta {
    size_t destino;
    int peso;
    struct Aresta *proxima;
} Aresta;

/* Cada vértice guarda início e fim da lista para manter a ordem de inserção. */
typedef struct {
    Aresta *inicio;
    Aresta *fim;
} Adjacencia;

struct Grafo {
    Adjacencia *adj;
    size_t vertices;
    size_t arestas;
    bool direcionado;
};

Grafo *grafo_criar(size_t vertices, bool direcionado) {
    Grafo *g = malloc(sizeof *g);
    if (!g) abort();
    g->adj = calloc(vertices ? vertices : 1, sizeof *g->adj);
    if (!g->adj) abort();
    g->vertices = vertices;
    g->arestas = 0;
    g->direcionado = direcionado;
    return g;
}

void grafo_destruir(Grafo *g) {
    if (!g) return;
    for (size_t v = 0; v < g->vertices; v++) {
        Aresta *a = g->adj[v].inicio;
        while (a) {
            Aresta *proxima = a->proxima;
            free(a);
            a = proxima;
        }
    }
    free(g->adj);
    free(g);
}

static void anexar(Grafo *g, size_t origem, size_t destino, int peso) {
    Aresta *a = malloc(sizeof *a);
    if (!a) abort();
    a->destino = destino;
    a->peso = peso;
    a->proxima = NULL;
    if (g->adj[origem].fim) g->adj[origem].fim->proxima = a;
    else g->adj[origem].inicio = a;
    g->adj[origem].fim = a;
}

bool grafo_adicionar_aresta(Grafo *g, size_t origem, size_t destino, int peso) {
    if (origem >= g->vertices || destino >= g->vertices) return false;
    anexar(g, origem, destino, peso);
    if (!g->direcionado && origem != destino) anexar(g, destino, origem, peso);
    g->arestas++;
    return true;
}

size_t grafo_vertices(const Grafo *g) { return g->vertices; }
size_t grafo_arestas(const Grafo *g) { return g->arestas; }

size_t grafo_bfs(const Grafo *g, size_t origem, size_t *ordem) {
    if (origem >= g->vertices) return 0;
    bool *visitado = calloc(g->vertices, sizeof *visitado);
    size_t *fila = malloc(g->vertices * sizeof *fila);
    if (!visitado || !fila) abort();

    size_t inicio = 0, fim = 0, total = 0;
    visitado[origem] = true;
    fila[fim++] = origem;
    while (inicio < fim) {
        size_t v = fila[inicio++];
        ordem[total++] = v;
        for (Aresta *a = g->adj[v].inicio; a; a = a->proxima) {
            if (!visitado[a->destino]) {
                visitado[a->destino] = true;
                fila[fim++] = a->destino;
            }
        }
    }
    free(visitado);
    free(fila);
    return total;
}

static void dfs(const Grafo *g, size_t v, bool *visitado, size_t *ordem, size_t *total) {
    visitado[v] = true;
    ordem[(*total)++] = v;
    for (Aresta *a = g->adj[v].inicio; a; a = a->proxima) {
        if (!visitado[a->destino]) dfs(g, a->destino, visitado, ordem, total);
    }
}

size_t grafo_dfs(const Grafo *g, size_t origem, size_t *ordem) {
    if (origem >= g->vertices) return 0;
    bool *visitado = calloc(g->vertices, sizeof *visitado);
    if (!visitado) abort();
    size_t total = 0;
    dfs(g, origem, visitado, ordem, &total);
    free(visitado);
    return total;
}

/*
 * Dijkstra na versão O(V^2): a cada passo escolhe o vértice não fechado com menor
 * distância. Para grafos esparsos, uma fila de prioridade (heap) leva a O((V+E) log V).
 */
void grafo_dijkstra(const Grafo *g, size_t origem, long *distancias) {
    bool *fechado = calloc(g->vertices, sizeof *fechado);
    if (!fechado) abort();
    for (size_t v = 0; v < g->vertices; v++) distancias[v] = GRAFO_INFINITO;
    if (origem >= g->vertices) { free(fechado); return; }
    distancias[origem] = 0;

    for (size_t passo = 0; passo < g->vertices; passo++) {
        size_t u = g->vertices;
        for (size_t v = 0; v < g->vertices; v++) {
            if (!fechado[v] && distancias[v] != GRAFO_INFINITO &&
                (u == g->vertices || distancias[v] < distancias[u])) {
                u = v;
            }
        }
        if (u == g->vertices) break; /* restantes são inalcançáveis */
        fechado[u] = true;
        for (Aresta *a = g->adj[u].inicio; a; a = a->proxima) {
            long candidata = distancias[u] + a->peso;
            if (distancias[a->destino] == GRAFO_INFINITO || candidata < distancias[a->destino]) {
                distancias[a->destino] = candidata;
            }
        }
    }
    free(fechado);
}

bool grafo_ordenacao_topologica(const Grafo *g, size_t *ordem) {
    if (!g->direcionado) return false;
    size_t *grau_entrada = calloc(g->vertices, sizeof *grau_entrada);
    size_t *fila = malloc((g->vertices ? g->vertices : 1) * sizeof *fila);
    if (!grau_entrada || !fila) abort();

    for (size_t v = 0; v < g->vertices; v++)
        for (Aresta *a = g->adj[v].inicio; a; a = a->proxima) grau_entrada[a->destino]++;

    /* Algoritmo de Kahn: processa sempre vértices sem dependências pendentes. */
    size_t inicio = 0, fim = 0, total = 0;
    for (size_t v = 0; v < g->vertices; v++)
        if (grau_entrada[v] == 0) fila[fim++] = v;

    while (inicio < fim) {
        size_t v = fila[inicio++];
        ordem[total++] = v;
        for (Aresta *a = g->adj[v].inicio; a; a = a->proxima)
            if (--grau_entrada[a->destino] == 0) fila[fim++] = a->destino;
    }
    free(grau_entrada);
    free(fila);
    return total == g->vertices;
}
