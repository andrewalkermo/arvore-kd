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
void adiciona_indices_das_paginas_na_arvore(No *no, unsigned int *indice);

void teste_imprimir_paginas();
void teste_imprimir_obras();

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
  arvore->raiz = cria_no_vazio(TIPO_NO_AUTOR);

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
  
  CabecalhoArquivo cabecalhoArquivo;
  scanf("%u", &cabecalhoArquivo.qtdNos);
  cabecalhoArquivo.qtdPaginas = cabecalhoArquivo.qtdNos + 1;

  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "w+");
  fwrite(&cabecalhoArquivo, sizeof(CabecalhoArquivo), 1, arquivo);

  for (unsigned int i = 0; i < cabecalhoArquivo.qtdNos; i++) {
    Obra *obra = le_obra_da_entrada();

    cria_no_e_insere_na_arvore(arvore->raiz, obra);
    fwrite(obra, sizeof(Obra), 1, arquivo);
  }

  for (unsigned int i = 0; i < cabecalhoArquivo.qtdNos + 1; i++) {
    Pagina pagina = cria_pagina_vazia();
    fwrite(&pagina, sizeof(Pagina), 1, arquivo);
  }
  fclose(arquivo);

  int indice = 0;
  adiciona_indices_das_paginas_na_arvore(arvore->raiz, &indice);

  // printf("%sIE %d ID %d\n", arvore->raiz->autor, arvore->raiz->indicePaginaEsquerda, arvore->raiz->indicePaginaDireita);
  // printf("%u\nIE %d ID %d\n", arvore->raiz->noFilhoEsquerdo->ano, arvore->raiz->noFilhoEsquerdo->indicePaginaEsquerda, arvore->raiz->noFilhoEsquerdo->indicePaginaDireita);
  // printf("%u\nIE %d ID %d\n", arvore->raiz->noFilhoDireito->ano, arvore->raiz->noFilhoDireito->indicePaginaEsquerda, arvore->raiz->noFilhoDireito->indicePaginaDireita);
  // printf("%sIE %d ID %d\n", arvore->raiz->noFilhoEsquerdo->noFilhoEsquerdo->autor, arvore->raiz->noFilhoEsquerdo->noFilhoEsquerdo->indicePaginaEsquerda, arvore->raiz->noFilhoEsquerdo->noFilhoEsquerdo->indicePaginaDireita);
  // printf("%sIE %d ID %d\n", arvore->raiz->noFilhoDireito->noFilhoEsquerdo->autor, arvore->raiz->noFilhoDireito->noFilhoEsquerdo->indicePaginaEsquerda, arvore->raiz->noFilhoDireito->noFilhoEsquerdo->indicePaginaDireita);
  // printf("%sIE %d ID %d\n", arvore->raiz->noFilhoDireito->noFilhoDireito->autor, arvore->raiz->noFilhoDireito->noFilhoDireito->indicePaginaEsquerda, arvore->raiz->noFilhoDireito->noFilhoDireito->indicePaginaDireita);
  // printf("%u\nIE %d ID %d\n", arvore->raiz->noFilhoEsquerdo->noFilhoEsquerdo->noFilhoDireito->ano, arvore->raiz->noFilhoEsquerdo->noFilhoEsquerdo->noFilhoDireito->indicePaginaEsquerda, arvore->raiz->noFilhoEsquerdo->noFilhoEsquerdo->noFilhoDireito->indicePaginaDireita);

  
  printf("arvore k-d gerada\n");
}

void adiciona_indices_das_paginas_na_arvore(No *no, unsigned int *indice) {
  if (no->noFilhoEsquerdo == NULL) {
    no->indicePaginaEsquerda = *indice;
    (*indice)++;
  } else {
    adiciona_indices_das_paginas_na_arvore(no->noFilhoEsquerdo, indice);
  }
  if (no->noFilhoDireito == NULL) {
    no->indicePaginaDireita = *indice;
    (*indice)++;
  }
  else {
    adiciona_indices_das_paginas_na_arvore(no->noFilhoDireito, indice);
  }
}

void cria_no_e_insere_na_arvore(No *no, Obra *obra) {
  if (no_esta_vazio(no)) {
    arvore->qtdNos++;
    if (no->tipo == TIPO_NO_AUTOR) {
      strcpy(no->autor, obra->autor);
    }
    else {
      no->ano = obra->ano;
    }
  }
  else {
    if (no->tipo == TIPO_NO_AUTOR) {
      if (strcmp(obra->autor, no->autor) <= 0) {
        if (no->noFilhoEsquerdo == NULL) {
          no->noFilhoEsquerdo = cria_no_vazio(TIPO_NO_ANO);
        }
        cria_no_e_insere_na_arvore(no->noFilhoEsquerdo, obra);
      }
      else {
        if (no->noFilhoDireito == NULL) {
          no->noFilhoDireito = cria_no_vazio(TIPO_NO_ANO);
        }
        cria_no_e_insere_na_arvore(no->noFilhoDireito, obra);
      }
    }
    else {
      if (obra->ano <= no->ano) {
        if (no->noFilhoEsquerdo == NULL) {
          no->noFilhoEsquerdo = cria_no_vazio(TIPO_NO_AUTOR);
        }
        cria_no_e_insere_na_arvore(no->noFilhoEsquerdo, obra);
      }
      else {
        if (no->noFilhoDireito == NULL) {
          no->noFilhoDireito = cria_no_vazio(TIPO_NO_AUTOR);
        }
        cria_no_e_insere_na_arvore(no->noFilhoDireito, obra);
      }
    }
  }
}

void estrutura_arvore_atraves_de_arquivo(FILE *arquivo) {
  teste_imprimir_obras();
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
  no->noFilhoEsquerdo = NULL;
  no->noFilhoDireito = NULL;
  no->indicePaginaEsquerda = -1;
  no->indicePaginaDireita = -1;

  if (tipo == TIPO_NO_AUTOR) {
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
  return no->tipo == TIPO_NO_AUTOR && no->autor[0] == '\0' ||
         no->tipo == TIPO_NO_ANO && no->ano == 0;
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
  pagina.proxima = -1;
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

void teste_imprimir_paginas() {

  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r");
  for (unsigned int i = 0; i < 8; i++)
  {
    printf("Pagina %u:\n", i);
    Pagina pagina;
    fread(&pagina, sizeof(Pagina), 1, arquivo);
    for (int j = 0; j < NREGSPORPAGINA; j++)
    {
      if (pagina.registros[j].ocupado) {
        printf("%s", pagina.registros[j].obra.autor);
        printf("%s", pagina.registros[j].obra.nome);
        printf("%u\n", pagina.registros[j].obra.ano);
        printf("%s\n\n", pagina.registros[j].obra.arquivo);
      }
    }
  }
  fclose(arquivo);
}

void teste_imprimir_obras() {

  unsigned int totalRegistros;
  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r");
  fread(&totalRegistros, sizeof(unsigned int), 1, arquivo);

  for (unsigned int i = 0; i < totalRegistros; i++)
  {
    printf("Obra %u:\n", i);
    Obra obra;
    fread(&obra, sizeof(Obra), 1, arquivo);
    printf("%s", obra.autor);
    printf("%s", obra.nome);
    printf("%u\n", obra.ano);
    printf("%s\n\n", obra.arquivo);
  }
}