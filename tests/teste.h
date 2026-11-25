#ifndef TESTE_H
#define TESTE_H

/* Mini framework de testes: sem dependências, só macros. */

#include <stdio.h>
#include <string.h>

static int teste_falhas = 0;
static int teste_verificacoes = 0;

#define VERIFICA(cond)                                                          \
    do {                                                                        \
        teste_verificacoes++;                                                   \
        if (!(cond)) {                                                          \
            teste_falhas++;                                                     \
            fprintf(stderr, "  FALHOU %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        }                                                                       \
    } while (0)

#define VERIFICA_IGUAL(esperado, obtido) VERIFICA((esperado) == (obtido))

#define VERIFICA_VETOR(esperado, obtido, n) \
    VERIFICA(memcmp((esperado), (obtido), (n) * sizeof *(esperado)) == 0)

#define TESTE(nome) static void nome(void)

#define RODAR(nome)                         \
    do {                                    \
        int antes = teste_falhas;           \
        nome();                             \
        printf("  %s %s\n", teste_falhas == antes ? "ok  " : "FALHA", #nome); \
    } while (0)

#define RESUMO(modulo)                                                              \
    (printf("%s: %d verificações, %d falha(s)\n\n", (modulo), teste_verificacoes, \
            teste_falhas),                                                          \
     teste_falhas == 0 ? 0 : 1)

#endif
