#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Grafo ponderado com lista de adjacência (vértices numerados de 0 a n-1).
 *
 * Lista de adjacência usa O(V + E) de memória, ideal para grafos esparsos.
 * Algoritmos: BFS, DFS, Dijkstra e ordenação topológica (Kahn).
 */

#define GRAFO_INFINITO (-1L)

typedef struct Grafo Grafo;

Grafo *grafo_criar(size_t vertices, bool direcionado);
void grafo_destruir(Grafo *g);

/* Em grafos não direcionados, a aresta é adicionada nos dois sentidos. */
bool grafo_adicionar_aresta(Grafo *g, size_t origem, size_t destino, int peso);
size_t grafo_vertices(const Grafo *g);
size_t grafo_arestas(const Grafo *g);

/*
 * Busca em largura / profundidade a partir de origem.
 * Preenche ordem com os vértices na ordem de visita e retorna quantos foram visitados.
 * Os vizinhos são visitados na ordem em que as arestas foram adicionadas.
 */
size_t grafo_bfs(const Grafo *g, size_t origem, size_t *ordem);
size_t grafo_dfs(const Grafo *g, size_t origem, size_t *ordem);

/* Distâncias mínimas a partir de origem (pesos não negativos). Inalcançáveis = GRAFO_INFINITO. */
void grafo_dijkstra(const Grafo *g, size_t origem, long *distancias);

/* Ordenação topológica (apenas grafos direcionados). Retorna false se houver ciclo. */
bool grafo_ordenacao_topologica(const Grafo *g, size_t *ordem);

#endif
