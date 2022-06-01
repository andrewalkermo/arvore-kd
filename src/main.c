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
void adiciona_indices_das_paginas_na_arvore(No *no, unsigned int *indice);

void insere_obra_no_arquivo(No *no, FILE *arquivo, Obra *obra);
void insere_obra_na_pagina(FILE *arquivo, int indicePagina, Obra *obra);
void escreve_pagina_no_arquivo(FILE *arquivo, int indicePagina, Pagina pagina);
void atualiza_cabecalho_do_arquivo(FILE *arquivo, CabecalhoArquivo cabecalho);

No *prepara_arvore();
No *inicializa_arvore_e_arquivo_com_entradas();
No *estrutura_arvore_atraves_de_arquivo(FILE *arquivo);
No *cria_no_vazio(TipoNo tipo);

bool no_esta_vazio(No *no);
Obra *le_obra_da_entrada();
Obra cria_obra_vazia();
Pagina cria_pagina_vazia();
Pagina le_pagina_do_arquivo(FILE *arquivo, int indicePagina);
Registro cria_registro_vazio();
FILE *abre_arquivo(char *nomeArquivo, char *modo);
CabecalhoArquivo le_cabecalho_do_arquivo(FILE *arquivo);

void teste_imprimir_paginas();
void teste_imprimir_obras_que_criam_nos_no_arquivo();


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

  for (unsigned int i = 0; i < cabecalhoArquivo.qtdPaginas; i++) {
    Pagina pagina = cria_pagina_vazia();
    fwrite(&pagina, sizeof(Pagina), 1, arquivo);
  }

  int indice = 0;
  adiciona_indices_das_paginas_na_arvore(raiz, &indice);
  
  for (unsigned int i = 0; i < cabecalhoArquivo.qtdNos; i++) {
    Obra obra;
    fseek(arquivo, sizeof(CabecalhoArquivo) + i * sizeof(Obra), SEEK_SET);
    fread(&obra, sizeof(Obra), 1, arquivo);
    insere_obra_no_arquivo(raiz, arquivo, &obra);
  }

  fclose(arquivo);
  printf("arvore k-d gerada\n");
  return raiz;
}

void adiciona_indices_das_paginas_na_arvore(No *no, unsigned int *indice) {
  if (no->noFilhoEsquerdo == NULL) {
    no->indicePaginaEsquerda = (*indice)++;
  } else {
    adiciona_indices_das_paginas_na_arvore(no->noFilhoEsquerdo, indice);
  }
  if (no->noFilhoDireito == NULL) {
    no->indicePaginaDireita = (*indice)++;
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
  CabecalhoArquivo cabecalhoArquivo = le_cabecalho_do_arquivo(arquivo);

  for(int i = 0; i < cabecalhoArquivo.qtdNos; i++) {
    Obra obra;
    fread(&obra, sizeof(Obra), 1, arquivo);
    cria_no_e_insere_na_arvore(raiz, &obra);
  }

  int indice = 0;
  adiciona_indices_das_paginas_na_arvore(raiz, &indice);

  return raiz;
}

CabecalhoArquivo le_cabecalho_do_arquivo(FILE *arquivo) {
  CabecalhoArquivo cabecalhoArquivo;
  fseek(arquivo, 0, SEEK_SET);
  fread(&cabecalhoArquivo, sizeof(CabecalhoArquivo), 1, arquivo);
  return cabecalhoArquivo;
}

void insere_obra_no_arquivo(No *no, FILE *arquivo, Obra *obra) {
  if (no->tipo == TIPO_NO_AUTOR) {
    if (strcmp(obra->autor, no->autor) <= 0) {
      if (no->noFilhoEsquerdo == NULL) {
        return insere_obra_na_pagina(arquivo, no->indicePaginaEsquerda, obra);
      } else {
        insere_obra_no_arquivo(no->noFilhoEsquerdo, arquivo, obra);
      }
    }
    else {
      if (no->noFilhoDireito == NULL) {
        return insere_obra_na_pagina(arquivo, no->indicePaginaDireita, obra);
      } else {
        insere_obra_no_arquivo(no->noFilhoDireito, arquivo, obra);
      }
    }
  }
  else {
    if (obra->ano <= no->ano) {
      if (no->noFilhoEsquerdo == NULL) {
        return insere_obra_na_pagina(arquivo, no->indicePaginaEsquerda, obra);
      } else {
        insere_obra_no_arquivo(no->noFilhoEsquerdo, arquivo, obra);
      }
    }
    else {
      if (no->noFilhoDireito == NULL) {
        return insere_obra_na_pagina(arquivo, no->indicePaginaDireita, obra);
      } else {
        insere_obra_no_arquivo(no->noFilhoDireito, arquivo, obra);
      }
    }
  }
}

void processa_comando_insere_registro(No *raiz) {
  Obra *obra = le_obra_da_entrada();
  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r+");
  insere_obra_no_arquivo(raiz, arquivo, obra);
  fclose(arquivo);
  printf("inserido registro com nome: %s", obra->nome);
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
  int indicePagina;
  scanf("%d", &indicePagina);
  Pagina pagina;
  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r");
  printf("pagina: %d\n", indicePagina);
  do {
    pagina = le_pagina_do_arquivo(arquivo, indicePagina);
    for (int j = 0; j < NREGSPORPAGINA; j++) {
      if (pagina.registros[j].ocupado) {
        printf("%s", pagina.registros[j].obra.autor);
        printf("%s", pagina.registros[j].obra.nome);
        printf("%u\n", pagina.registros[j].obra.ano);
        printf("%s\n", pagina.registros[j].obra.arquivo);
      }
    }
    indicePagina = pagina.proxima;
  } while (pagina.proxima != -1);
}

void insere_obra_na_pagina(FILE *arquivo, int indicePagina, Obra *obra) {

  CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
  Pagina pagina = le_pagina_do_arquivo(arquivo, indicePagina);
  
  while(pagina.proxima != -1) {
    indicePagina = pagina.proxima;
    pagina = le_pagina_do_arquivo(arquivo, indicePagina);
  }

  pagina.registros[pagina.qtdRegistros].obra = *obra;
  pagina.registros[pagina.qtdRegistros].ocupado = true;
  pagina.qtdRegistros++;

  // Se a pagina estiver cheia, vamos criar uma nova pagina vazia no final do arquivo e referenciar a nova pagina na pagina antiga, usando o contador
  if (pagina.qtdRegistros == NREGSPORPAGINA) {
    pagina.proxima = cabecalho.qtdPaginas;
    escreve_pagina_no_arquivo(arquivo, cabecalho.qtdPaginas, cria_pagina_vazia());
    cabecalho.qtdPaginas++;
    atualiza_cabecalho_do_arquivo(arquivo, cabecalho);
  }
  
  escreve_pagina_no_arquivo(arquivo, indicePagina, pagina);
}

void atualiza_cabecalho_do_arquivo(FILE *arquivo, CabecalhoArquivo cabecalho) {
  fseek(arquivo, 0, SEEK_SET);
  fwrite(&cabecalho, sizeof(CabecalhoArquivo), 1, arquivo);
}

void escreve_pagina_no_arquivo(FILE *arquivo, int indicePagina, Pagina pagina) {
  CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
  long int posicao = 1 * sizeof(CabecalhoArquivo) + cabecalho.qtdNos * sizeof(Obra) + indicePagina * sizeof(Pagina);
  fseek(arquivo, posicao, SEEK_SET);
  fwrite(&pagina, sizeof(Pagina), 1, arquivo);
}

Pagina le_pagina_do_arquivo(FILE *arquivo, int indicePagina) {
  CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
  long int posicao = 1 * sizeof(CabecalhoArquivo) + cabecalho.qtdNos * sizeof(Obra) + indicePagina * sizeof(Pagina);
  fseek(arquivo, posicao, SEEK_SET);
  Pagina pagina;
  fread(&pagina, sizeof(Pagina), 1, arquivo);
  return pagina;
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

void teste_imprimir_paginas() {

  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r");

  CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
  printf("LOG - Cabecalho:\n");
  printf("LOG - qtdNos: %u\n", cabecalho.qtdNos);
  printf("LOG - qtdPaginas: %u\n", cabecalho.qtdPaginas);
  // return;
  for (unsigned int i = 0; i < cabecalho.qtdNos + 1; i++) {
    printf("LOG - Pagina %u:\n", i);
    Pagina pagina;
    int indicePagina = i;
    do {
      pagina = le_pagina_do_arquivo(arquivo, indicePagina);
      for (int j = 0; j < NREGSPORPAGINA; j++) {
        if (pagina.registros[j].ocupado) {
          printf("LOG - %s", pagina.registros[j].obra.autor);
          printf("LOG - %s", pagina.registros[j].obra.nome);
          printf("LOG - %u\n", pagina.registros[j].obra.ano);
          printf("LOG - %s\n\n", pagina.registros[j].obra.arquivo);
        }
      }
      indicePagina = pagina.proxima;
    } while (pagina.proxima != -1);
  }
  fclose(arquivo);
}

void teste_imprimir_obras_que_criam_nos_no_arquivo() {

  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r");
  CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
  for (unsigned int i = 0; i < cabecalho.qtdNos; i++) {
    Obra obra;
    fread(&obra, sizeof(Obra), 1, arquivo);
    printf("LOG - %s", obra.autor);
    printf("LOG - %s", obra.nome);
    printf("LOG - %u\n", obra.ano);
    printf("LOG - %s\n\n", obra.arquivo);
  }
  fclose(arquivo);
}