#pragma once

#include "../globais/configuracao.h"

typedef struct Consulta Consulta;

struct Consulta{
    char tipo;
    char nomeInicial[TAMANHO_NOME];
    char nomeFinal[TAMANHO_NOME];
    unsigned int anoInicial;
    unsigned int anoFinal;
    unsigned int qtdResultados;
};