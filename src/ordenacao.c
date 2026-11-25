#include "ordenacao.h"

#include <stdlib.h>
#include <string.h>

static void trocar(int *a, int *b) { int t = *a; *a = *b; *b = t; }

/* ---------- O(n^2) ---------- */

void bubble_sort(int *v, size_t n) {
    for (size_t fim = n; fim > 1; fim--) {
        bool trocou = false;
        for (size_t i = 1; i < fim; i++) {
            if (v[i - 1] > v[i]) { trocar(&v[i - 1], &v[i]); trocou = true; }
        }
        if (!trocou) return; /* já ordenado: melhor caso O(n) */
    }
}

void selection_sort(int *v, size_t n) {
    for (size_t i = 0; i + 1 < n; i++) {
        size_t menor = i;
        for (size_t j = i + 1; j < n; j++) if (v[j] < v[menor]) menor = j;
        if (menor != i) trocar(&v[i], &v[menor]);
    }
}

void insertion_sort(int *v, size_t n) {
    for (size_t i = 1; i < n; i++) {
        int atual = v[i];
        size_t j = i;
        while (j > 0 && v[j - 1] > atual) { v[j] = v[j - 1]; j--; }
        v[j] = atual;
    }
}

/* ---------- O(n log n) ---------- */

static void intercalar(int *v, int *aux, size_t ini, size_t meio, size_t fim) {
    size_t i = ini, j = meio, k = ini;
    while (i < meio && j < fim) aux[k++] = v[i] <= v[j] ? v[i++] : v[j++]; /* <= mantém estabilidade */
    while (i < meio) aux[k++] = v[i++];
    while (j < fim) aux[k++] = v[j++];
    memcpy(v + ini, aux + ini, (fim - ini) * sizeof *v);
}

static void merge_rec(int *v, int *aux, size_t ini, size_t fim) {
    if (fim - ini < 2) return;
    size_t meio = ini + (fim - ini) / 2;
    merge_rec(v, aux, ini, meio);
    merge_rec(v, aux, meio, fim);
    intercalar(v, aux, ini, meio, fim);
}

void merge_sort(int *v, size_t n) {
    if (n < 2) return;
    int *aux = malloc(n * sizeof *aux);
    if (!aux) abort();
    merge_rec(v, aux, 0, n);
    free(aux);
}

/* Mediana de três como pivô evita o pior caso O(n^2) em vetores já ordenados. */
static int mediana_de_tres(int *v, long ini, long fim) {
    long meio = ini + (fim - ini) / 2;
    if (v[meio] < v[ini]) trocar(&v[meio], &v[ini]);
    if (v[fim] < v[ini]) trocar(&v[fim], &v[ini]);
    if (v[fim] < v[meio]) trocar(&v[fim], &v[meio]);
    return v[meio];
}

static void quick_rec(int *v, long ini, long fim) {
    while (ini < fim) {
        int pivo = mediana_de_tres(v, ini, fim);
        /* Particionamento de Hoare */
        long i = ini, j = fim;
        while (i <= j) {
            while (v[i] < pivo) i++;
            while (v[j] > pivo) j--;
            if (i <= j) { trocar(&v[i], &v[j]); i++; j--; }
        }
        /* Recursão na parte menor e laço na maior: pilha de chamadas O(log n). */
        if (j - ini < fim - i) { quick_rec(v, ini, j); ini = i; }
        else { quick_rec(v, i, fim); fim = j; }
    }
}

void quick_sort(int *v, size_t n) {
    if (n > 1) quick_rec(v, 0, (long)n - 1);
}

static void descer_max(int *v, size_t n, size_t i) {
    for (;;) {
        size_t maior = i, e = 2 * i + 1, d = 2 * i + 2;
        if (e < n && v[e] > v[maior]) maior = e;
        if (d < n && v[d] > v[maior]) maior = d;
        if (maior == i) return;
        trocar(&v[i], &v[maior]);
        i = maior;
    }
}

void heap_sort(int *v, size_t n) {
    for (size_t i = n / 2; i-- > 0;) descer_max(v, n, i);   /* monta heap de máximo */
    for (size_t fim = n; fim > 1; fim--) {
        trocar(&v[0], &v[fim - 1]);                        /* maior vai para o final */
        descer_max(v, fim - 1, 0);
    }
}

/* ---------- O(n + k) ---------- */

void counting_sort(int *v, size_t n) {
    if (n < 2) return;
    int min = v[0], max = v[0];
    for (size_t i = 1; i < n; i++) {
        if (v[i] < min) min = v[i];
        if (v[i] > max) max = v[i];
    }
    size_t k = (size_t)((long long)max - min) + 1;
    size_t *contagem = calloc(k, sizeof *contagem);
    if (!contagem) abort();
    for (size_t i = 0; i < n; i++) contagem[(size_t)((long long)v[i] - min)]++;
    size_t pos = 0;
    for (size_t c = 0; c < k; c++)
        while (contagem[c]--) v[pos++] = (int)((long long)c + min);
    free(contagem);
}

bool esta_ordenado(const int *v, size_t n) {
    for (size_t i = 1; i < n; i++) if (v[i - 1] > v[i]) return false;
    return true;
}

long busca_binaria(const int *v, size_t n, int alvo) {
    size_t ini = 0, fim = n;           /* intervalo semiaberto [ini, fim) */
    while (ini < fim) {
        size_t meio = ini + (fim - ini) / 2;  /* evita overflow de (ini + fim) */
        if (v[meio] == alvo) return (long)meio;
        if (v[meio] < alvo) ini = meio + 1;
        else fim = meio;
    }
    return -1;
}
