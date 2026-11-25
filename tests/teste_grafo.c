#include "../src/grafo.h"
#include "teste.h"

/*
 * Grafo não direcionado usado nos testes de busca:
 *
 *   0 --- 1 --- 3
 *   |     |
 *   2 --- 4     5 (isolado)
 */
static Grafo *grafo_exemplo(void) {
    Grafo *g = grafo_criar(6, false);
    grafo_adicionar_aresta(g, 0, 1, 1);
    grafo_adicionar_aresta(g, 0, 2, 1);
    grafo_adicionar_aresta(g, 1, 3, 1);
    grafo_adicionar_aresta(g, 1, 4, 1);
    grafo_adicionar_aresta(g, 2, 4, 1);
    return g;
}

TESTE(bfs_visita_por_niveis) {
    Grafo *g = grafo_exemplo();
    size_t ordem[6];
    size_t esperado[] = {0, 1, 2, 3, 4};
    VERIFICA_IGUAL(5u, grafo_bfs(g, 0, ordem));
    VERIFICA_VETOR(esperado, ordem, 5);
    VERIFICA_IGUAL(1u, grafo_bfs(g, 5, ordem)); /* isolado só visita a si mesmo */
    VERIFICA_IGUAL(5u, grafo_arestas(g));
    grafo_destruir(g);
}

TESTE(dfs_vai_fundo_primeiro) {
    Grafo *g = grafo_exemplo();
    size_t ordem[6];
    size_t esperado[] = {0, 1, 3, 4, 2};
    VERIFICA_IGUAL(5u, grafo_dfs(g, 0, ordem));
    VERIFICA_VETOR(esperado, ordem, 5);
    grafo_destruir(g);
}

TESTE(dijkstra_menores_caminhos) {
    /*
     *   0 -4-> 1 -1-> 3
     *   |      ^      ^
     *   1      2      5
     *   v      |      |
     *   2 -----+------+
     */
    Grafo *g = grafo_criar(5, true);
    grafo_adicionar_aresta(g, 0, 1, 4);
    grafo_adicionar_aresta(g, 0, 2, 1);
    grafo_adicionar_aresta(g, 2, 1, 2);
    grafo_adicionar_aresta(g, 1, 3, 1);
    grafo_adicionar_aresta(g, 2, 3, 5);

    long d[5];
    grafo_dijkstra(g, 0, d);
    long esperado[] = {0, 3, 1, 4, GRAFO_INFINITO};
    VERIFICA_VETOR(esperado, d, 5);
    grafo_destruir(g);
}

TESTE(ordenacao_topologica_de_disciplinas) {
    /* 0 Algoritmos I -> 1 Algoritmos II -> 3 Estruturas de Dados -> 4 Grafos
       2 Matemática Discreta -> 4 Grafos */
    Grafo *g = grafo_criar(5, true);
    grafo_adicionar_aresta(g, 0, 1, 1);
    grafo_adicionar_aresta(g, 1, 3, 1);
    grafo_adicionar_aresta(g, 3, 4, 1);
    grafo_adicionar_aresta(g, 2, 4, 1);

    size_t ordem[5], posicao[5];
    VERIFICA(grafo_ordenacao_topologica(g, ordem));
    for (size_t i = 0; i < 5; i++) posicao[ordem[i]] = i;
    VERIFICA(posicao[0] < posicao[1]);
    VERIFICA(posicao[1] < posicao[3]);
    VERIFICA(posicao[3] < posicao[4]);
    VERIFICA(posicao[2] < posicao[4]);

    grafo_adicionar_aresta(g, 4, 0, 1); /* cria um ciclo */
    VERIFICA(!grafo_ordenacao_topologica(g, ordem));
    grafo_destruir(g);
}

TESTE(arestas_invalidas) {
    Grafo *g = grafo_criar(2, false);
    VERIFICA(!grafo_adicionar_aresta(g, 0, 2, 1));
    VERIFICA_IGUAL(0u, grafo_arestas(g));
    size_t ordem[2];
    VERIFICA(!grafo_ordenacao_topologica(g, ordem)); /* não direcionado */
    grafo_destruir(g);
}

int main(void) {
    printf("Grafo\n");
    RODAR(bfs_visita_por_niveis);
    RODAR(dfs_vai_fundo_primeiro);
    RODAR(dijkstra_menores_caminhos);
    RODAR(ordenacao_topologica_de_disciplinas);
    RODAR(arestas_invalidas);
    return RESUMO("grafo");
}
