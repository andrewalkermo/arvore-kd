#pragma once

#include "../globais/configuracao.h"

typedef struct No No;
typedef enum TipoNo TipoNo;

enum TipoNo {
    TIPO_NO_AUTOR,
    TIPO_NO_ANO,
};

struct No{
    enum TipoNo tipo;
    union {
        char autor[TAMANHO_NOME];
        unsigned int ano;
    };
    No* noFilhoEsquerdo;
    No* noFilhoDireito;
    int indicePaginaEsquerda;
    int indicePaginaDireita;
};