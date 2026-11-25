#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Tabela hash (dicionário string -> int) com encadeamento separado.
 *
 * A função de hash é a FNV-1a. Cada posição do vetor (bucket) guarda uma lista
 * encadeada das entradas que colidiram ali. Quando o fator de carga
 * (entradas / buckets) passa de 0,75, o vetor dobra de tamanho e tudo é
 * redistribuído (rehash), mantendo as operações em O(1) em média.
 */

typedef struct TabelaHash TabelaHash;

TabelaHash *hash_criar(void);
void hash_destruir(TabelaHash *t);

/* Insere ou atualiza. A chave é copiada internamente. */
void hash_inserir(TabelaHash *t, const char *chave, int valor);
bool hash_obter(const TabelaHash *t, const char *chave, int *saida);
bool hash_contem(const TabelaHash *t, const char *chave);
bool hash_remover(TabelaHash *t, const char *chave);

size_t hash_tamanho(const TabelaHash *t);
size_t hash_capacidade(const TabelaHash *t);

#endif
