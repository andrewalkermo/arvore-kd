#pragma once

#include "../globais/configuracao.h"

typedef struct Obra Obra;

struct Obra{
  char autor[TAMANHO_NOME];
  char nome[TAMANHO_NOME];
  unsigned int ano;
  char arquivo[TAMANHO_NOME];
};