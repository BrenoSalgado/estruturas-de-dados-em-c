ifeq ($(origin CC),default)
CC      := gcc
endif
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2
BUILD   := build

SRCS    := $(wildcard src/*.c)
OBJS    := $(patsubst src/%.c,$(BUILD)/%.o,$(SRCS))
TESTES  := $(patsubst tests/%.c,$(BUILD)/%,$(wildcard tests/teste_*.c))

.PHONY: all test bench clean

all: $(TESTES) $(BUILD)/benchmark_ordenacao

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: src/%.c src/%.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/teste_%: tests/teste_%.c tests/teste.h $(OBJS)
	$(CC) $(CFLAGS) $< $(OBJS) -o $@

$(BUILD)/benchmark_ordenacao: exemplos/benchmark_ordenacao.c $(OBJS)
	$(CC) $(CFLAGS) $< $(OBJS) -o $@

# Roda todos os testes e falha se qualquer um falhar.
test: $(TESTES)
	@status=0; for t in $(TESTES); do ./$$t || status=1; done; exit $$status

bench: $(BUILD)/benchmark_ordenacao
	./$(BUILD)/benchmark_ordenacao

clean:
	rm -rf $(BUILD)
