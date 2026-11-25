#include "../src/lista.h"
#include "teste.h"

TESTE(insercoes_nas_pontas_e_no_meio) {
    Lista *l = lista_criar();
    VERIFICA(lista_vazia(l));
    lista_inserir_fim(l, 2);
    lista_inserir_inicio(l, 1);
    lista_inserir_fim(l, 4);
    VERIFICA(lista_inserir_em(l, 2, 3));
    VERIFICA(!lista_inserir_em(l, 10, 99));

    int esperado[] = {1, 2, 3, 4}, obtido[4];
    lista_para_vetor(l, obtido);
    VERIFICA_IGUAL(4u, lista_tamanho(l));
    VERIFICA_VETOR(esperado, obtido, 4);
    lista_destruir(l);
}

TESTE(remocoes) {
    Lista *l = lista_criar();
    for (int i = 1; i <= 5; i++) lista_inserir_fim(l, i);

    int x;
    VERIFICA(lista_remover_inicio(l, &x) && x == 1);
    VERIFICA(lista_remover_fim(l, &x) && x == 5);
    VERIFICA(lista_remover_valor(l, 3));
    VERIFICA(!lista_remover_valor(l, 42));

    int esperado[] = {2, 4}, obtido[2];
    lista_para_vetor(l, obtido);
    VERIFICA_VETOR(esperado, obtido, 2);

    VERIFICA(lista_remover_inicio(l, NULL));
    VERIFICA(lista_remover_inicio(l, NULL));
    VERIFICA(!lista_remover_inicio(l, &x));
    VERIFICA(!lista_remover_fim(l, &x));
    VERIFICA(lista_vazia(l));
    lista_destruir(l);
}

TESTE(busca_e_acesso_por_posicao) {
    Lista *l = lista_criar();
    for (int i = 0; i < 10; i++) lista_inserir_fim(l, i * 10);

    int x;
    VERIFICA(lista_obter(l, 0, &x) && x == 0);
    VERIFICA(lista_obter(l, 7, &x) && x == 70); /* percorre a partir do fim */
    VERIFICA(!lista_obter(l, 10, &x));
    VERIFICA_IGUAL(3, lista_buscar(l, 30));
    VERIFICA_IGUAL(-1, lista_buscar(l, 31));
    lista_destruir(l);
}

TESTE(inverter) {
    Lista *l = lista_criar();
    lista_inverter(l); /* lista vazia não deve quebrar */
    for (int i = 1; i <= 4; i++) lista_inserir_fim(l, i);
    lista_inverter(l);

    int esperado[] = {4, 3, 2, 1}, obtido[4];
    lista_para_vetor(l, obtido);
    VERIFICA_VETOR(esperado, obtido, 4);

    /* As pontas também precisam estar corretas depois de inverter. */
    lista_inserir_fim(l, 0);
    int x;
    VERIFICA(lista_remover_inicio(l, &x) && x == 4);
    VERIFICA(lista_remover_fim(l, &x) && x == 0);
    lista_destruir(l);
}

int main(void) {
    printf("Lista duplamente encadeada\n");
    RODAR(insercoes_nas_pontas_e_no_meio);
    RODAR(remocoes);
    RODAR(busca_e_acesso_por_posicao);
    RODAR(inverter);
    return RESUMO("lista");
}
