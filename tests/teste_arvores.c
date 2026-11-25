#include "../src/arvore_avl.h"
#include "../src/arvore_bst.h"
#include "teste.h"

#include <stdlib.h>

/*
 *          50
 *        /    \
 *      30      70
 *     /  \    /  \
 *   20   40  60   80
 */
static ArvoreBST *bst_exemplo(void) {
    ArvoreBST *a = bst_criar();
    int chaves[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) bst_inserir(a, chaves[i]);
    return a;
}

TESTE(bst_percursos) {
    ArvoreBST *a = bst_exemplo();
    int v[7];

    int em[] = {20, 30, 40, 50, 60, 70, 80};
    bst_em_ordem(a, v);
    VERIFICA_VETOR(em, v, 7);

    int pre[] = {50, 30, 20, 40, 70, 60, 80};
    bst_pre_ordem(a, v);
    VERIFICA_VETOR(pre, v, 7);

    int pos[] = {20, 40, 30, 60, 80, 70, 50};
    bst_pos_ordem(a, v);
    VERIFICA_VETOR(pos, v, 7);

    int largura[] = {50, 30, 70, 20, 40, 60, 80};
    bst_em_largura(a, v);
    VERIFICA_VETOR(largura, v, 7);

    VERIFICA_IGUAL(3, bst_altura(a));
    bst_destruir(a);
}

TESTE(bst_busca_min_max_e_repetidos) {
    ArvoreBST *a = bst_exemplo();
    VERIFICA(bst_buscar(a, 60));
    VERIFICA(!bst_buscar(a, 65));
    VERIFICA(!bst_inserir(a, 40));
    VERIFICA_IGUAL(7u, bst_tamanho(a));
    int x;
    VERIFICA(bst_minimo(a, &x) && x == 20);
    VERIFICA(bst_maximo(a, &x) && x == 80);
    bst_destruir(a);

    ArvoreBST *vazia = bst_criar();
    VERIFICA(!bst_minimo(vazia, &x));
    VERIFICA_IGUAL(0, bst_altura(vazia));
    bst_destruir(vazia);
}

TESTE(bst_remocao_dos_tres_casos) {
    ArvoreBST *a = bst_exemplo();
    int v[7];

    VERIFICA(bst_remover(a, 20));            /* folha */
    bst_inserir(a, 65);
    VERIFICA(bst_remover(a, 60));            /* um filho (65) */
    VERIFICA(bst_remover(a, 50));            /* dois filhos: raiz vira o sucessor 65 */
    VERIFICA(!bst_remover(a, 50));

    int esperado[] = {30, 40, 65, 70, 80};
    bst_em_ordem(a, v);
    VERIFICA_VETOR(esperado, v, 5);
    bst_pre_ordem(a, v);
    VERIFICA_IGUAL(65, v[0]);
    VERIFICA_IGUAL(5u, bst_tamanho(a));
    bst_destruir(a);
}

TESTE(bst_degenera_com_entrada_ordenada) {
    ArvoreBST *a = bst_criar();
    for (int i = 1; i <= 100; i++) bst_inserir(a, i);
    VERIFICA_IGUAL(100, bst_altura(a)); /* vira uma "lista": motivo de existir a AVL */
    bst_destruir(a);
}

TESTE(avl_continua_balanceada_com_entrada_ordenada) {
    ArvoreAVL *a = avl_criar();
    for (int i = 1; i <= 1000; i++) avl_inserir(a, i);
    VERIFICA(avl_valida(a));
    VERIFICA_IGUAL(1000u, avl_tamanho(a));
    VERIFICA(avl_altura(a) <= 11); /* 1,44 * log2(1000) ≈ 14; na prática fica em 10 */
    avl_destruir(a);
}

TESTE(avl_rotacoes_duplas) {
    /* Esquerda-direita: 30, 10, 20  -> raiz 20 */
    ArvoreAVL *a = avl_criar();
    avl_inserir(a, 30); avl_inserir(a, 10); avl_inserir(a, 20);
    VERIFICA(avl_valida(a));
    VERIFICA_IGUAL(2, avl_altura(a));
    avl_destruir(a);

    /* Direita-esquerda: 10, 30, 20 -> raiz 20 */
    a = avl_criar();
    avl_inserir(a, 10); avl_inserir(a, 30); avl_inserir(a, 20);
    VERIFICA(avl_valida(a));
    VERIFICA_IGUAL(2, avl_altura(a));
    avl_destruir(a);
}

TESTE(avl_operacoes_aleatorias) {
    ArvoreAVL *a = avl_criar();
    bool presente[2000] = {false};
    size_t esperado = 0;
    bool sempre_valida = true;
    srand(42);

    for (int passo = 0; passo < 20000; passo++) {
        int chave = rand() % 2000;
        if (rand() % 3) {
            bool inseriu = avl_inserir(a, chave);
            VERIFICA_IGUAL(!presente[chave], inseriu);
            if (inseriu) { presente[chave] = true; esperado++; }
        } else {
            bool removeu = avl_remover(a, chave);
            VERIFICA_IGUAL(presente[chave], removeu);
            if (removeu) { presente[chave] = false; esperado--; }
        }
        if (passo % 500 == 0) sempre_valida = sempre_valida && avl_valida(a);
    }
    VERIFICA(sempre_valida);
    VERIFICA(avl_valida(a));
    VERIFICA_IGUAL(esperado, avl_tamanho(a));

    bool busca_ok = true;
    for (int k = 0; k < 2000; k++) busca_ok = busca_ok && avl_buscar(a, k) == presente[k];
    VERIFICA(busca_ok);
    avl_destruir(a);
}

int main(void) {
    printf("Árvores (BST e AVL)\n");
    RODAR(bst_percursos);
    RODAR(bst_busca_min_max_e_repetidos);
    RODAR(bst_remocao_dos_tres_casos);
    RODAR(bst_degenera_com_entrada_ordenada);
    RODAR(avl_continua_balanceada_com_entrada_ordenada);
    RODAR(avl_rotacoes_duplas);
    RODAR(avl_operacoes_aleatorias);
    return RESUMO("árvores");
}
