#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../tipos/no.h"
#include "../utils/utils.h"
#include "../tipos/obra.h"
#include "../tipos/pagina.h"
#include "../globais/configuracao.h"
#include "../tipos/cabecalho_arquivo.h"

FILE *abre_arquivo(char *nomeArquivo, char *modo);
CabecalhoArquivo le_cabecalho_do_arquivo(FILE *arquivo);
Pagina le_pagina_do_arquivo(FILE *arquivo, int indicePagina);
void escreve_pagina_no_arquivo(FILE *arquivo, int indicePagina, Pagina pagina);
void atualiza_cabecalho_do_arquivo(FILE *arquivo, CabecalhoArquivo cabecalho);
void insere_obra_na_pagina(FILE *arquivo, int indicePagina, Obra *obra);
void insere_obra_no_arquivo(No *no, FILE *arquivo, Obra *obra);
