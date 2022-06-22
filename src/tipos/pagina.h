#pragma once

#include "registro.h"
#include "../globais/configuracao.h"

typedef struct Pagina Pagina;

struct Pagina{
    unsigned int qtdRegistros;
    int proxima;
    Registro registros[NREGSPORPAGINA];
};
