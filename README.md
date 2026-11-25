<div align="center">

# Estruturas de Dados em C

Implementações do zero das estruturas de dados e algoritmos clássicos,
com testes automatizados, verificação de memória e benchmark.

[![CI](https://github.com/BrenoSalgado/estruturas-de-dados-em-c/actions/workflows/ci.yml/badge.svg)](https://github.com/BrenoSalgado/estruturas-de-dados-em-c/actions/workflows/ci.yml)
![C11](https://img.shields.io/badge/C-C11-00599C?logo=c&logoColor=white)
![Sem dependências](https://img.shields.io/badge/depend%C3%AAncias-nenhuma-brightgreen)
[![Licença MIT](https://img.shields.io/badge/licen%C3%A7a-MIT-blue)](LICENSE)

</div>

---

## Sobre

Repositório de estudos de **Estruturas de Dados** e **Algoritmos**. Cada estrutura
tem uma interface pública (`.h`) com tipo opaco, a implementação (`.c`) comentada e
testes que cobrem os casos normais e de borda.

- Código em **C11 puro**, sem bibliotecas externas
- Compila sem avisos com `-Wall -Wextra -Wpedantic -Werror`
- **CI no GitHub Actions**: testes com gcc e clang, **AddressSanitizer/UBSan** e **Valgrind** (nenhum vazamento de memória)
- Mini framework de testes próprio em [`tests/teste.h`](tests/teste.h)

## Conteúdo

| Estrutura | Arquivo | Operações principais | Complexidade |
|---|---|---|---|
| Lista duplamente encadeada | [`lista`](src/lista.c) | inserir/remover nas pontas, inserir em posição, buscar, inverter | O(1) nas pontas · O(n) busca |
| Pilha (vetor dinâmico) | [`pilha`](src/pilha.c) | empilhar, desempilhar, topo + validador de `()[]{}` | O(1) amortizado |
| Fila (buffer circular) | [`fila`](src/fila.c) | enfileirar, desenfileirar, frente | O(1) amortizado |
| Árvore binária de busca | [`arvore_bst`](src/arvore_bst.c) | inserir, buscar, remover, mín/máx, 4 percursos | O(h) — O(log n) médio, O(n) pior |
| Árvore AVL | [`arvore_avl`](src/arvore_avl.c) | inserir e remover com rotações simples e duplas | O(log n) garantido |
| Heap de mínimo | [`heap`](src/heap.c) | inserir, remover mínimo, construir em O(n) | O(log n) |
| Tabela hash | [`tabela_hash`](src/tabela_hash.c) | inserir/atualizar, obter, remover; FNV-1a + encadeamento + rehash | O(1) médio |
| Grafo (lista de adjacência) | [`grafo`](src/grafo.c) | BFS, DFS, Dijkstra, ordenação topológica (Kahn) | O(V + E) · Dijkstra O(V²) |

### Ordenação e busca — [`ordenacao`](src/ordenacao.c)

| Algoritmo | Melhor | Médio | Pior | Memória extra | Estável |
|---|---|---|---|---|:---:|
| Bubble sort | O(n) | O(n²) | O(n²) | O(1) | ✅ |
| Selection sort | O(n²) | O(n²) | O(n²) | O(1) | ❌ |
| Insertion sort | O(n) | O(n²) | O(n²) | O(1) | ✅ |
| Merge sort | O(n log n) | O(n log n) | O(n log n) | O(n) | ✅ |
| Quick sort (mediana de 3) | O(n log n) | O(n log n) | O(n²) | O(log n) | ❌ |
| Heap sort | O(n log n) | O(n log n) | O(n log n) | O(1) | ❌ |
| Counting sort | O(n + k) | O(n + k) | O(n + k) | O(k) | ✅ |
| Busca binária | O(1) | O(log n) | O(log n) | O(1) | — |

## Como usar

Requisitos: `gcc` (ou `clang`) e `make`.

```bash
git clone https://github.com/BrenoSalgado/estruturas-de-dados-em-c.git
cd estruturas-de-dados-em-c
make test    # compila e roda todos os testes
make bench   # compara os algoritmos de ordenação
```

Saída dos testes (resumida):

```text
Árvores (BST e AVL)
  ok   bst_percursos
  ok   bst_remocao_dos_tres_casos
  ok   avl_continua_balanceada_com_entrada_ordenada
  ok   avl_operacoes_aleatorias
árvores: 20032 verificações, 0 falha(s)
...
```

Exemplo de uso de uma estrutura:

```c
#include "src/tabela_hash.h"

TabelaHash *idades = hash_criar();
hash_inserir(idades, "ana", 21);
hash_inserir(idades, "bruno", 34);

int idade;
if (hash_obter(idades, "ana", &idade)) printf("ana tem %d anos\n", idade);

hash_destruir(idades);
```

## Benchmark de ordenação

Vetores de inteiros aleatórios, mesmo vetor para todos os algoritmos
(`make bench`, gcc 16 `-O2`, Windows 11). Os O(n²) não rodam acima de 10.000 elementos.

| Algoritmo | n = 1.000 | n = 10.000 | n = 100.000 | n = 1.000.000 |
|---|---:|---:|---:|---:|
| Bubble sort | 0,46 ms | 55,74 ms | — | — |
| Selection sort | 0,77 ms | 80,95 ms | — | — |
| Insertion sort | 0,08 ms | 7,73 ms | — | — |
| Merge sort | 0,04 ms | 0,53 ms | 6,10 ms | 69,98 ms |
| Quick sort | 0,08 ms | 0,53 ms | 5,66 ms | 53,53 ms |
| Heap sort | 0,03 ms | 0,31 ms | 4,90 ms | 64,06 ms |
| Counting sort | 0,04 ms | 0,10 ms | 0,37 ms | 1,70 ms |

O que dá para tirar daí: de 1.000 para 10.000 elementos (10×) os quadráticos ficam ~100× mais
lentos, enquanto os O(n log n) ficam só ~7 a 13×. O counting sort é imbatível aqui porque o intervalo
de valores (k = 10⁶) é da ordem de n.

## Destaques de implementação

- **Tipos opacos**: o usuário só enxerga `typedef struct Lista Lista;` — os campos ficam escondidos no `.c` (encapsulamento em C).
- **Ponteiro para ponteiro** na BST e na tabela hash: inserção e remoção sem tratar a raiz/cabeça como caso especial.
- **AVL validada nos testes**: `avl_valida()` confere ordem, alturas e balanceamento após 20.000 operações aleatórias.
- **Fila circular** que cresce mesmo com o início no meio do vetor ("desenrola" o círculo ao copiar).
- **Heapify em O(n)** em `heap_de_vetor`, descendo a partir do último nó interno.
- **Quick sort** com mediana de três e recursão só na menor partição (pilha O(log n)).
- **Busca binária** com `meio = ini + (fim - ini) / 2` para evitar overflow.
- A BST tem um teste mostrando que ela **degenera em lista** com entrada ordenada (altura 100 para 100 chaves) — o motivo da AVL existir.

## Estrutura do projeto

```text
.
├── src/                  # interface (.h) e implementação (.c) de cada estrutura
├── tests/                # um executável de teste por módulo + teste.h
├── exemplos/             # benchmark de ordenação
├── .github/workflows/    # CI: gcc, clang, sanitizers e valgrind
└── Makefile
```

## Licença

[MIT](LICENSE)
