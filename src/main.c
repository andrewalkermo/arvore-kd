#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "types.h"

void processa_comandos(No *raiz);
void processa_comando_insere_registro(No *raiz);
void processa_comando_consulta_simples(No *raiz);
void processa_comando_consulta_por_faixa_de_nomes_de_autores(No *raiz);
void processa_comando_consulta_por_faixa_de_anos(No *raiz);
void processa_comando_consulta_por_faixa_de_nomes_de_autores_e_anos(No *raiz);
void processa_comando_imprime_indice_da_arvore(No *raiz);
void processa_comando_imprime_pagina(No *raiz);

void cria_no_e_insere_na_arvore(No *no, Obra *obra);
void inserir_obra_na_pagina(Pagina *pagina, Obra *obra);
void adiciona_indices_das_paginas_na_arvore(No *no, unsigned int *indice);

No *prepara_arvore();
No *inicializa_arvore_e_arquivo_com_entradas();
No *estrutura_arvore_atraves_de_arquivo(FILE *arquivo);
No *cria_no_vazio(TipoNo tipo);

bool no_esta_vazio(No *no);
Obra *le_obra_da_entrada();
Obra cria_obra_vazia();
Pagina cria_pagina_vazia();
Registro cria_registro_vazio();
FILE *abre_arquivo(char *nomeArquivo, char *modo);


void teste_imprimir_paginas();
void teste_imprimir_obras();


int main() {
  No *raiz = prepara_arvore();
  processa_comandos(raiz);
  return 0;
}

void processa_comandos(No *raiz) {
  char comando;
  while (scanf("%s", &comando)) {
    switch (comando) {
      case INSERE_REGISTRO:
        processa_comando_insere_registro(raiz);
        break;
      case CONSULTA_SIMPLES:
        processa_comando_consulta_simples(raiz);
        break;
      case CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES:
        processa_comando_consulta_por_faixa_de_nomes_de_autores(raiz);
        break;
      case CONSULTA_POR_FAIXA_DE_ANOS:
        processa_comando_consulta_por_faixa_de_anos(raiz);
        break;
      case CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES_E_ANOS:
        processa_comando_consulta_por_faixa_de_nomes_de_autores_e_anos(raiz);
        break;
      case IMPRIME_INDICE_DA_ARVORE:
        processa_comando_imprime_indice_da_arvore(raiz);
        break;
      case IMPRIME_PAGINA:
        processa_comando_imprime_pagina(raiz);
        break;
      case TERMINO_DA_SEQUENCIA_DE_COMANDOS:
        return;
    }
  }
}

No *prepara_arvore() {
  No *raiz;

  FILE *arquivo;
  if (arquivo = fopen(NOME_ARQUIVO, "r")) {
    raiz = estrutura_arvore_atraves_de_arquivo(arquivo);
    fclose(arquivo);
  }
  else {
    raiz = inicializa_arvore_e_arquivo_com_entradas();
  }

  printf("%sIE %d ID %d\n", raiz->autor, raiz->indicePaginaEsquerda, raiz->indicePaginaDireita);
  printf("%u\nIE %d ID %d\n", raiz->noFilhoEsquerdo->ano, raiz->noFilhoEsquerdo->indicePaginaEsquerda, raiz->noFilhoEsquerdo->indicePaginaDireita);
  printf("%u\nIE %d ID %d\n", raiz->noFilhoDireito->ano, raiz->noFilhoDireito->indicePaginaEsquerda, raiz->noFilhoDireito->indicePaginaDireita);
  printf("%sIE %d ID %d\n", raiz->noFilhoEsquerdo->noFilhoEsquerdo->autor, raiz->noFilhoEsquerdo->noFilhoEsquerdo->indicePaginaEsquerda, raiz->noFilhoEsquerdo->noFilhoEsquerdo->indicePaginaDireita);
  printf("%sIE %d ID %d\n", raiz->noFilhoDireito->noFilhoEsquerdo->autor, raiz->noFilhoDireito->noFilhoEsquerdo->indicePaginaEsquerda, raiz->noFilhoDireito->noFilhoEsquerdo->indicePaginaDireita);
  printf("%sIE %d ID %d\n", raiz->noFilhoDireito->noFilhoDireito->autor, raiz->noFilhoDireito->noFilhoDireito->indicePaginaEsquerda, raiz->noFilhoDireito->noFilhoDireito->indicePaginaDireita);
  printf("%u\nIE %d ID %d\n", raiz->noFilhoEsquerdo->noFilhoEsquerdo->noFilhoDireito->ano, raiz->noFilhoEsquerdo->noFilhoEsquerdo->noFilhoDireito->indicePaginaEsquerda, raiz->noFilhoEsquerdo->noFilhoEsquerdo->noFilhoDireito->indicePaginaDireita);

  return raiz;
}

No *inicializa_arvore_e_arquivo_com_entradas() {
  No *raiz = cria_no_vazio(TIPO_NO_AUTOR);
  CabecalhoArquivo cabecalhoArquivo;
  scanf("%u", &cabecalhoArquivo.qtdNos);
  cabecalhoArquivo.qtdPaginas = cabecalhoArquivo.qtdNos + 1;

  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "w+");
  fwrite(&cabecalhoArquivo, sizeof(CabecalhoArquivo), 1, arquivo);

  for (unsigned int i = 0; i < cabecalhoArquivo.qtdNos; i++) {
    Obra *obra = le_obra_da_entrada();

    cria_no_e_insere_na_arvore(raiz, obra);
    fwrite(obra, sizeof(Obra), 1, arquivo);
  }

  for (unsigned int i = 0; i < cabecalhoArquivo.qtdNos + 1; i++) {
    Pagina pagina = cria_pagina_vazia();
    fwrite(&pagina, sizeof(Pagina), 1, arquivo);
  }
  fclose(arquivo);

  int indice = 0;
  adiciona_indices_das_paginas_na_arvore(raiz, &indice);

  printf("arvore k-d gerada\n");
  return raiz;
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

No *estrutura_arvore_atraves_de_arquivo(FILE *arquivo) {
  No *raiz = cria_no_vazio(TIPO_NO_AUTOR);
  CabecalhoArquivo cabecalhoArquivo;
  fseek(arquivo, 0, SEEK_SET);
  fread(&cabecalhoArquivo, sizeof(CabecalhoArquivo), 1, arquivo);

  for(int i = 0; i < cabecalhoArquivo.qtdNos; i++) {
    Obra obra;
    fread(&obra, sizeof(Obra), 1, arquivo);
    cria_no_e_insere_na_arvore(raiz, &obra);
  }

  int indice = 0;
  adiciona_indices_das_paginas_na_arvore(raiz, &indice);

  return raiz;
}

void processa_comando_insere_registro(No *raiz) {
  printf("insere_registro");
}

void processa_comando_consulta_simples(No *raiz) {
  printf("consulta_simples");
}

void processa_comando_consulta_por_faixa_de_nomes_de_autores(No *raiz) {
  printf("consulta_por_faixa_de_nomes_de_autores");
}

void processa_comando_consulta_por_faixa_de_anos(No *raiz) {
  printf("consulta_por_faixa_de_anos");
}

void processa_comando_consulta_por_faixa_de_nomes_de_autores_e_anos(No *raiz) {
  printf("consulta_por_faixa_de_nomes_de_autores_e_anos");
}

void processa_comando_imprime_indice_da_arvore(No *raiz) {
  printf("imprime_indice_da_arvore");
}

void processa_comando_imprime_pagina(No *raiz) {
  printf("imprime_pagina");
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