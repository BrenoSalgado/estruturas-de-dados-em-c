#include "../src/heap.h"
#include "../src/tabela_hash.h"
#include "teste.h"

#include <stdlib.h>

TESTE(heap_remove_em_ordem_crescente) {
    Heap *h = heap_criar();
    int valores[] = {5, 3, 8, 1, 9, 2, 7, 3};
    for (int i = 0; i < 8; i++) heap_inserir(h, valores[i]);

    int x;
    VERIFICA(heap_minimo(h, &x) && x == 1);
    int esperado[] = {1, 2, 3, 3, 5, 7, 8, 9}, obtido[8];
    for (int i = 0; i < 8; i++) heap_remover_minimo(h, &obtido[i]);
    VERIFICA_VETOR(esperado, obtido, 8);
    VERIFICA(heap_vazio(h));
    VERIFICA(!heap_remover_minimo(h, &x));
    heap_destruir(h);
}

TESTE(heap_construido_de_vetor) {
    int v[500];
    srand(7);
    for (int i = 0; i < 500; i++) v[i] = rand() % 1000 - 500;
    Heap *h = heap_de_vetor(v, 500);
    VERIFICA_IGUAL(500u, heap_tamanho(h));

    int anterior, atual;
    heap_remover_minimo(h, &anterior);
    bool ordem_ok = true;
    while (heap_remover_minimo(h, &atual)) {
        ordem_ok = ordem_ok && anterior <= atual;
        anterior = atual;
    }
    VERIFICA(ordem_ok);
    heap_destruir(h);

    Heap *vazio = heap_de_vetor(NULL, 0);
    heap_inserir(vazio, 4);
    VERIFICA(heap_minimo(vazio, &atual) && atual == 4);
    heap_destruir(vazio);
}

TESTE(hash_inserir_obter_atualizar_remover) {
    TabelaHash *t = hash_criar();
    hash_inserir(t, "ana", 21);
    hash_inserir(t, "bruno", 34);
    hash_inserir(t, "carla", 19);

    int x;
    VERIFICA(hash_obter(t, "bruno", &x) && x == 34);
    VERIFICA(!hash_obter(t, "daniel", &x));

    hash_inserir(t, "ana", 22); /* atualiza, não duplica */
    VERIFICA(hash_obter(t, "ana", &x) && x == 22);
    VERIFICA_IGUAL(3u, hash_tamanho(t));

    VERIFICA(hash_remover(t, "ana"));
    VERIFICA(!hash_remover(t, "ana"));
    VERIFICA(!hash_contem(t, "ana"));
    VERIFICA(hash_contem(t, "carla"));
    VERIFICA_IGUAL(2u, hash_tamanho(t));
    hash_destruir(t);
}

TESTE(hash_rehash_preserva_tudo) {
    TabelaHash *t = hash_criar();
    char chave[32];
    for (int i = 0; i < 10000; i++) {
        snprintf(chave, sizeof chave, "chave-%d", i);
        hash_inserir(t, chave, i * 2);
    }
    VERIFICA_IGUAL(10000u, hash_tamanho(t));
    /* fator de carga deve ter ficado abaixo de 0,75 */
    VERIFICA(hash_tamanho(t) * 4 <= hash_capacidade(t) * 3);

    bool tudo_certo = true;
    for (int i = 0; i < 10000; i++) {
        int x;
        snprintf(chave, sizeof chave, "chave-%d", i);
        tudo_certo = tudo_certo && hash_obter(t, chave, &x) && x == i * 2;
    }
    VERIFICA(tudo_certo);

    for (int i = 0; i < 10000; i += 2) {
        snprintf(chave, sizeof chave, "chave-%d", i);
        hash_remover(t, chave);
    }
    VERIFICA_IGUAL(5000u, hash_tamanho(t));
    VERIFICA(!hash_contem(t, "chave-0"));
    VERIFICA(hash_contem(t, "chave-1"));
    hash_destruir(t);
}

int main(void) {
    printf("Heap e Tabela Hash\n");
    RODAR(heap_remove_em_ordem_crescente);
    RODAR(heap_construido_de_vetor);
    RODAR(hash_inserir_obter_atualizar_remover);
    RODAR(hash_rehash_preserva_tudo);
    return RESUMO("heap/hash");
}
