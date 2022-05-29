#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "types.h"

void insere_registro();
void consulta_simples();
void consulta_por_faixa_de_nomes_de_autores();
void consulta_por_faixa_de_anos();
void consulta_por_faixa_de_nomes_de_autores_e_anos();
void imprime_indice_da_arvore();
void imprime_pagina();
void termino_da_sequencia_de_comandos();
void prepara_arvore();
void processa_comandos();

int main() {
  prepara_arvore();
  processa_comandos();
  return 0;
}

void processa_comandos() {
  char comando;
  while (scanf("%s", &comando)) {
    switch (comando) {
      case INSERE_REGISTRO:
        insere_registro();
        break;
      case CONSULTA_SIMPLES:
        consulta_simples();
        break;
      case CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES:
        consulta_por_faixa_de_nomes_de_autores();
        break;
      case CONSULTA_POR_FAIXA_DE_ANOS:
        consulta_por_faixa_de_anos();
        break;
      case CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES_E_ANOS:
        consulta_por_faixa_de_nomes_de_autores_e_anos();
        break;
      case IMPRIME_INDICE_DA_ARVORE:
        imprime_indice_da_arvore();
        break;
      case IMPRIME_PAGINA:
        imprime_pagina();
        break;
      case TERMINO_DA_SEQUENCIA_DE_COMANDOS:
        return;
    }
  }
}

void insere_registro() {
  printf("insere_registro");
}

void consulta_simples() {
  printf("consulta_simples");
}

void consulta_por_faixa_de_nomes_de_autores() {
  printf("consulta_por_faixa_de_nomes_de_autores");
}

void consulta_por_faixa_de_anos() {
  printf("consulta_por_faixa_de_anos");
}

void consulta_por_faixa_de_nomes_de_autores_e_anos() {
  printf("consulta_por_faixa_de_nomes_de_autores_e_anos");
}

void imprime_indice_da_arvore() {
  printf("imprime_indice_da_arvore");
}

void imprime_pagina() {
  printf("imprime_pagina");
}

void termino_da_sequencia_de_comandos() {
  printf("termino_da_sequencia_de_comandos");
}
