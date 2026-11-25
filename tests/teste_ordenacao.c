#include "../src/ordenacao.h"
#include "teste.h"

#include <stdlib.h>

typedef void (*Ordenador)(int *, size_t);

static const struct {
    const char *nome;
    Ordenador funcao;
} ALGORITMOS[] = {
    {"bubble", bubble_sort},   {"selection", selection_sort}, {"insertion", insertion_sort},
    {"merge", merge_sort},     {"quick", quick_sort},         {"heap", heap_sort},
    {"counting", counting_sort},
};
#define QTD_ALGORITMOS (sizeof ALGORITMOS / sizeof *ALGORITMOS)

static int comparar(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

/* Ordena uma cópia com cada algoritmo e compara com o qsort da biblioteca padrão. */
static bool confere_todos(const int *original, size_t n) {
    int *referencia = malloc((n ? n : 1) * sizeof *referencia);
    int *copia = malloc((n ? n : 1) * sizeof *copia);
    if (n) memcpy(referencia, original, n * sizeof *original);
    qsort(referencia, n, sizeof *referencia, comparar);

    bool ok = true;
    for (size_t a = 0; a < QTD_ALGORITMOS; a++) {
        if (n) memcpy(copia, original, n * sizeof *original);
        ALGORITMOS[a].funcao(copia, n);
        if (n && memcmp(copia, referencia, n * sizeof *copia) != 0) {
            fprintf(stderr, "  %s_sort errou com n=%zu\n", ALGORITMOS[a].nome, n);
            ok = false;
        }
    }
    free(referencia);
    free(copia);
    return ok;
}

TESTE(casos_de_borda) {
    int um[] = {7};
    int dois[] = {2, 1};
    int iguais[] = {5, 5, 5, 5, 5};
    int negativos[] = {-3, 10, -7, 0, 2, -3};
    VERIFICA(confere_todos(NULL, 0));
    VERIFICA(confere_todos(um, 1));
    VERIFICA(confere_todos(dois, 2));
    VERIFICA(confere_todos(iguais, 5));
    VERIFICA(confere_todos(negativos, 6));
}

TESTE(vetores_ja_ordenados_e_invertidos) {
    int crescente[300], decrescente[300];
    for (int i = 0; i < 300; i++) { crescente[i] = i; decrescente[i] = 300 - i; }
    VERIFICA(confere_todos(crescente, 300));
    VERIFICA(confere_todos(decrescente, 300));
}

TESTE(vetores_aleatorios_de_varios_tamanhos) {
    srand(2025);
    bool ok = true;
    for (size_t n = 3; n <= 2000; n = n * 2 + 1) {
        int *v = malloc(n * sizeof *v);
        for (size_t i = 0; i < n; i++) v[i] = rand() % 200 - 100; /* muitos repetidos */
        ok = ok && confere_todos(v, n);
        free(v);
    }
    VERIFICA(ok);
}

TESTE(busca_binaria_encontra_e_nao_encontra) {
    int v[] = {1, 3, 5, 7, 9, 11};
    VERIFICA_IGUAL(0, busca_binaria(v, 6, 1));
    VERIFICA_IGUAL(3, busca_binaria(v, 6, 7));
    VERIFICA_IGUAL(5, busca_binaria(v, 6, 11));
    VERIFICA_IGUAL(-1, busca_binaria(v, 6, 4));
    VERIFICA_IGUAL(-1, busca_binaria(v, 6, 12));
    VERIFICA_IGUAL(-1, busca_binaria(v, 0, 1));
    VERIFICA(esta_ordenado(v, 6));
}

int main(void) {
    printf("Ordenação e busca\n");
    RODAR(casos_de_borda);
    RODAR(vetores_ja_ordenados_e_invertidos);
    RODAR(vetores_aleatorios_de_varios_tamanhos);
    RODAR(busca_binaria_encontra_e_nao_encontra);
    return RESUMO("ordenação");
}
