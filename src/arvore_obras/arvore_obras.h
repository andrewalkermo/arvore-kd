#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../operacoes_arquivo/operacoes_arquivo.h"
#include "../globais/configuracao.h"
#include "../tipos/obra.h"
#include "../tipos/no.h"

void cria_no_e_insere_na_arvore(No *no, Obra *obra);
bool no_esta_vazio(No *no);
No *cria_no_vazio(TipoNo tipo);
void imprime_indice_da_arvore(No *no);
