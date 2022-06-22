#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globais/configuracao.h"
#include "globais/comandos.h"
#include "arvore_obras/arvore_obras.h"
#include "consultas/consultas.h"
#include "operacoes_arquivo/operacoes_arquivo.h"
#include "utils/utils.h"

void processa_comandos(No *raiz);
void processa_comando_insere_registro(No *raiz);
void processa_comando_consulta_simples(No *raiz);
void processa_comando_consulta_por_faixa_de_nomes_de_autores(No *raiz);
void processa_comando_consulta_por_faixa_de_anos(No *raiz);
void processa_comando_consulta_por_faixa_de_nomes_de_autores_e_anos(No *raiz);
void processa_comando_imprime_indice_da_arvore(No *raiz);
void processa_comando_imprime_pagina(No *raiz);
void adiciona_indices_das_paginas_na_arvore(No *no, int *indice);
void imprime_registros_da_pagina(int indicePagina, Consulta *consulta);
void imprime_registros_que_correspondem_a_consulta(No *no, Consulta *consulta);

No *prepara_arvore();
No *inicializa_arvore_e_arquivo_com_entradas();
No *estrutura_arvore_atraves_de_arquivo(FILE *arquivo);
