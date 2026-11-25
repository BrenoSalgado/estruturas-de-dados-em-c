#include "../src/fila.h"
#include "../src/pilha.h"
#include "teste.h"

TESTE(pilha_lifo_e_crescimento) {
    Pilha *p = pilha_criar();
    for (int i = 0; i < 1000; i++) pilha_empilhar(p, i); /* força vários realloc */
    VERIFICA_IGUAL(1000u, pilha_tamanho(p));

    int x;
    VERIFICA(pilha_topo(p, &x) && x == 999);
    bool ordem_ok = true;
    for (int i = 999; i >= 0; i--) ordem_ok = ordem_ok && pilha_desempilhar(p, &x) && x == i;
    VERIFICA(ordem_ok);
    VERIFICA(pilha_vazia(p));
    VERIFICA(!pilha_desempilhar(p, &x));
    VERIFICA(!pilha_topo(p, &x));
    pilha_destruir(p);
}

TESTE(pilha_expressoes_balanceadas) {
    VERIFICA(expressao_balanceada(""));
    VERIFICA(expressao_balanceada("(a + b) * [c - {d / e}]"));
    VERIFICA(expressao_balanceada("{[()()]}"));
    VERIFICA(!expressao_balanceada("(]"));
    VERIFICA(!expressao_balanceada("(()"));
    VERIFICA(!expressao_balanceada("())"));
    VERIFICA(!expressao_balanceada("{[}]"));
}

TESTE(fila_fifo) {
    Fila *f = fila_criar();
    for (int i = 1; i <= 3; i++) fila_enfileirar(f, i);
    int x;
    VERIFICA(fila_frente(f, &x) && x == 1);
    VERIFICA(fila_desenfileirar(f, &x) && x == 1);
    VERIFICA(fila_desenfileirar(f, &x) && x == 2);
    VERIFICA(fila_desenfileirar(f, &x) && x == 3);
    VERIFICA(!fila_desenfileirar(f, &x));
    VERIFICA(fila_vazia(f));
    fila_destruir(f);
}

TESTE(fila_circular_dando_a_volta_e_crescendo) {
    Fila *f = fila_criar();
    int x, proximo_a_sair = 0, proximo_a_entrar = 0;
    bool ordem_ok = true;

    /* Entra 3, sai 2, repetidamente: os índices dão a volta várias vezes
       no buffer e ele precisa crescer com o início no meio do vetor. */
    for (int rodada = 0; rodada < 200; rodada++) {
        for (int k = 0; k < 3; k++) fila_enfileirar(f, proximo_a_entrar++);
        for (int k = 0; k < 2; k++) {
            ordem_ok = ordem_ok && fila_desenfileirar(f, &x) && x == proximo_a_sair;
            proximo_a_sair++;
        }
    }
    VERIFICA(ordem_ok);
    VERIFICA_IGUAL(200u, fila_tamanho(f));
    while (fila_desenfileirar(f, &x)) ordem_ok = ordem_ok && x == proximo_a_sair++;
    VERIFICA(ordem_ok);
    VERIFICA_IGUAL(600, proximo_a_sair);
    fila_destruir(f);
}

int main(void) {
    printf("Pilha e Fila\n");
    RODAR(pilha_lifo_e_crescimento);
    RODAR(pilha_expressoes_balanceadas);
    RODAR(fila_fifo);
    RODAR(fila_circular_dando_a_volta_e_crescendo);
    return RESUMO("pilha/fila");
}
