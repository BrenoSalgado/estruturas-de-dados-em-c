/*
 * Compara o tempo dos algoritmos de ordenação em vetores aleatórios de tamanhos diferentes.
 * Os algoritmos O(n^2) são pulados nos tamanhos grandes para o programa terminar rápido.
 *
 * Uso: make bench
 */
#include "../src/ordenacao.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef void (*Ordenador)(int *, size_t);

/* timespec_get (C11) tem resolução bem melhor que clock() no Windows. */
static double agora_ms(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
}

static const struct {
    const char *nome;
    Ordenador funcao;
    bool quadratico;
} ALGORITMOS[] = {
    {"Bubble sort", bubble_sort, true},       {"Selection sort", selection_sort, true},
    {"Insertion sort", insertion_sort, true}, {"Merge sort", merge_sort, false},
    {"Quick sort", quick_sort, false},        {"Heap sort", heap_sort, false},
    {"Counting sort", counting_sort, false},
};

static const size_t TAMANHOS[] = {1000, 10000, 100000, 1000000};
#define LIMITE_QUADRATICO 10000

int main(void) {
    size_t qtd_alg = sizeof ALGORITMOS / sizeof *ALGORITMOS;
    size_t qtd_tam = sizeof TAMANHOS / sizeof *TAMANHOS;
    size_t maior = TAMANHOS[qtd_tam - 1];

    int *original = malloc(maior * sizeof *original);
    int *copia = malloc(maior * sizeof *copia);
    if (!original || !copia) return 1;
    srand(123);
    for (size_t i = 0; i < maior; i++) original[i] = rand() % 1000000;

    printf("| Algoritmo | ");
    for (size_t t = 0; t < qtd_tam; t++) printf("n = %zu | ", TAMANHOS[t]);
    printf("\n|---|");
    for (size_t t = 0; t < qtd_tam; t++) printf("---:|");
    printf("\n");

    for (size_t a = 0; a < qtd_alg; a++) {
        printf("| %s | ", ALGORITMOS[a].nome);
        for (size_t t = 0; t < qtd_tam; t++) {
            size_t n = TAMANHOS[t];
            if (ALGORITMOS[a].quadratico && n > LIMITE_QUADRATICO) {
                printf("— | ");
                continue;
            }
            memcpy(copia, original, n * sizeof *copia);
            double inicio = agora_ms();
            ALGORITMOS[a].funcao(copia, n);
            double ms = agora_ms() - inicio;
            if (!esta_ordenado(copia, n)) {
                fprintf(stderr, "%s não ordenou corretamente!\n", ALGORITMOS[a].nome);
                return 1;
            }
            printf("%.2f ms | ", ms);
        }
        printf("\n");
    }

    free(original);
    free(copia);
    return 0;
}
