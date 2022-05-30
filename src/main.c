#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
void inicializa_arvore_e_arquivo_com_entradas();
void estrutura_arvore_atraves_de_arquivo(FILE *arquivo);
void cria_no_e_insere_na_arvore(No *no, Obra *obra);
FILE *abre_arquivo(char *nomeArquivo, char *modo);
No *cria_no_vazio(TipoNo tipo);
bool no_esta_vazio(No *no);
Obra *le_obra_da_entrada();
Pagina cria_pagina_vazia();
Registro cria_registro_vazio();
Obra cria_obra_vazia();
void inserir_obra_na_pagina(Pagina *pagina, Obra *obra);
void adiciona_pagina_no_arquivo(Pagina *pagina, unsigned int indice);


Arvore *arvore;

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

void prepara_arvore() {
  arvore = (Arvore*) malloc(sizeof(Arvore));
  arvore->qtdNos = 0;
  arvore->raiz = cria_no_vazio(AUTOR_NO);

  FILE *arquivo;
  if (arquivo = fopen(NOME_ARQUIVO, "r")) {
    estrutura_arvore_atraves_de_arquivo(arquivo);
    fclose(arquivo);
  }
  else {
    inicializa_arvore_e_arquivo_com_entradas();
  }
}

void inicializa_arvore_e_arquivo_com_entradas() {
  
  unsigned int qtdEntradas;
  scanf("%u", &qtdEntradas);
  
  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "w+");
  for (unsigned int i = 0; i < qtdEntradas + 1; i++) {
    Pagina pagina = cria_pagina_vazia();
    fwrite(&pagina, sizeof(Pagina), 1, arquivo);
  }
  fclose(arquivo);

  for (unsigned int i = 0; i < qtdEntradas; i++) {
    Obra *obra = le_obra_da_entrada();

    cria_no_e_insere_na_arvore(arvore->raiz, obra);
    printf("%s\n", arvore->raiz->autor);
  }
  printf("arvore k-d gerada\n");
}

void cria_no_e_insere_na_arvore(No *no, Obra *obra) {
  if (no_esta_vazio(no)) {
    arvore->qtdNos++;
    if (no->tipo == AUTOR_NO) {
      strcpy(no->autor, obra->autor);
    }
    else {
      no->ano = obra->ano;
    }
    Pagina pagina = cria_pagina_vazia();
    inserir_obra_na_pagina(&pagina, obra);
    adiciona_pagina_no_arquivo(&pagina, arvore->qtdNos);
    no->indicePaginaEsquerda = arvore->qtdNos;

    pagina = cria_pagina_vazia();
    adiciona_pagina_no_arquivo(&pagina, arvore->qtdNos);
    no->indicePaginaDireita = arvore->qtdNos + 1;
  }
}

void estrutura_arvore_atraves_de_arquivo(FILE *arquivo) {
  //TODO
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

FILE *abre_arquivo(char *nomeArquivo, char *modo){
  FILE *arquivo;
  if (!(arquivo = fopen(nomeArquivo, modo))) {
    printf("Erro na tentativa de abrir arquivo \"%s\".\n", nomeArquivo);
    exit(-1);
  }
  return arquivo;
}

No *cria_no_vazio(TipoNo tipo) {
  No *no = (No*) malloc(sizeof(No));
  no->tipo = tipo;
  no->noEsquerdo = NULL;
  no->noDireito = NULL;
  no->indicePaginaEsquerda = 0;
  no->indicePaginaDireita = 0;

  if (tipo == AUTOR_NO) {
    no->autor[0] = '\0';
  }
  else {
    no->ano = 0;
  }
  return no;
}

void adiciona_pagina_no_arquivo(Pagina *pagina, unsigned int indice) {
  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r+");
  fseek(arquivo, indice * sizeof(Pagina), SEEK_SET);
  fwrite(pagina, sizeof(Pagina), 1, arquivo);
}

bool no_esta_vazio(No *no) {
  return no->tipo == AUTOR_NO && no->autor[0] == '\0' ||
         no->tipo == ANO_NO && no->ano == 0;
}

Obra *le_obra_da_entrada() {
  Obra *obra = (Obra*) malloc(sizeof(Obra));
  getchar();
  fgets(obra->autor, TAMANHO_NOME, stdin);
  fgets(obra->nome, TAMANHO_NOME, stdin);
  scanf("%u", &obra->ano);
  scanf("%s", obra->arquivo);
  return obra;
}

Pagina cria_pagina_vazia() {
  Pagina pagina;
  pagina.qtdRegistros = 0;
  for (int i = 0; i < NREGSPORPAGINA; i++) {
    pagina.registros[i] = cria_registro_vazio();
  }
  return pagina;
}

Registro cria_registro_vazio() {
  Registro registro;
  registro.ocupado = false;
  registro.obra = cria_obra_vazia();
  return registro;
}

Obra cria_obra_vazia() {
  Obra obra;
  obra.autor[0] = '\0';
  obra.nome[0] = '\0';
  obra.ano = 0;
  obra.arquivo[0] = '\0';
  return obra;
}

void inserir_obra_na_pagina(Pagina *pagina, Obra *obra) {
  pagina->registros[pagina->qtdRegistros].obra = *obra;
  pagina->registros[pagina->qtdRegistros].ocupado = true;
  pagina->qtdRegistros++;
}