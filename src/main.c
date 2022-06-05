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

void adiciona_indices_das_paginas_na_arvore(No *no, unsigned int *indice);
void cria_no_e_insere_na_arvore(No *no, Obra *obra);
void insere_obra_no_arquivo(No *no, FILE *arquivo, Obra *obra);
void insere_obra_na_pagina(FILE *arquivo, int indicePagina, Obra *obra);
void atualiza_cabecalho_do_arquivo(FILE *arquivo, CabecalhoArquivo cabecalho);
void escreve_pagina_no_arquivo(FILE *arquivo, int indicePagina, Pagina pagina);
void escreve_registro_no_arquivo(FILE *arquivo, int indicePagina, int indiceRegistro, Registro registro);
void insere_pagina_vazia_no_final_do_arquivo(FILE *arquivo);
void imprime_obra_por_autor(No *no, char *autor, int *qtdEncontrados);

void imprime_registros_da_pagina(int indicePagina, Consulta *consulta);
void imprime_registros_que_correspondem_a_consulta(No *no, Consulta *consulta);
void imprime_indice_da_arvore(No *no);

No *prepara_arvore();
No *inicializa_arvore_e_arquivo_com_entradas();
No *estrutura_arvore_atraves_de_arquivo(FILE *arquivo);
No *cria_no_vazio(TipoNo tipo);

CabecalhoArquivo le_cabecalho_do_arquivo(FILE *arquivo);
Pagina cria_pagina_vazia();
Pagina le_pagina_do_arquivo(FILE *arquivo, int indicePagina);
Registro cria_registro_vazio();
Registro le_registro_do_arquivo(FILE *arquivo, int indicePagina, int indiceRegistro);
int compara_no_com_consulta(No *no, Consulta *consulta);
bool compara_obra_com_consulta(Obra *obra, Consulta *consulta);
bool no_esta_vazio(No *no);
Obra *le_obra_da_entrada();
Obra cria_obra_vazia();
FILE *abre_arquivo(char *nomeArquivo, char *modo);

CabecalhoArquivo le_cabecalho_do_arquivo(FILE *arquivo);
char *le_nome_da_entrada();

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
  cabecalhoArquivo.qtdPaginas = 0;

  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "w+");
  fwrite(&cabecalhoArquivo, sizeof(CabecalhoArquivo), 1, arquivo);

  for (unsigned int i = 0; i < cabecalhoArquivo.qtdNos; i++) {
    Obra *obra = le_obra_da_entrada();
    cria_no_e_insere_na_arvore(raiz, obra);
    fwrite(obra, sizeof(Obra), 1, arquivo);
  }

  for (unsigned int i = 0; i < cabecalhoArquivo.qtdNos + 1; i++) {
    insere_pagina_vazia_no_final_do_arquivo(arquivo);
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
  printf("inserido registro com nome: %s\n", obra->nome);
}

void processa_comando_consulta_simples(No *raiz) {
  Consulta consulta;
  strcpy(consulta.nomeInicial, le_nome_da_entrada());
  consulta.qtdResultados = 0;
  consulta.tipo = CONSULTA_SIMPLES;

  imprime_registros_que_correspondem_a_consulta(raiz, &consulta);
  if (consulta.qtdResultados == 0) {
    printf("nao foi encontrado registro com nome: %s\n", consulta.nomeInicial);
  }
}

void processa_comando_consulta_por_faixa_de_nomes_de_autores(No *raiz) {
  Consulta consulta;
  strcpy(consulta.nomeInicial, le_nome_da_entrada());
  strcpy(consulta.nomeFinal, le_nome_da_entrada());
  consulta.qtdResultados = 0;
  consulta.tipo = CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES;

  imprime_registros_que_correspondem_a_consulta(raiz, &consulta);
  if (consulta.qtdResultados == 0) {
    printf("nao foi encontrado registro com nome entre: \"%s\" e \"%s\"\n", consulta.nomeInicial, consulta.nomeFinal);
  }
}

void processa_comando_consulta_por_faixa_de_anos(No *raiz) {
  Consulta consulta;
  scanf("%u", &consulta.anoInicial);
  scanf("%u", &consulta.anoFinal);
  consulta.qtdResultados = 0;
  consulta.tipo = CONSULTA_POR_FAIXA_DE_ANOS;

  imprime_registros_que_correspondem_a_consulta(raiz, &consulta);
  if (consulta.qtdResultados == 0) {
    printf("nao foi encontrado registro com ano entre: %d e %d\n", consulta.anoInicial, consulta.anoFinal);
  }
}

void processa_comando_consulta_por_faixa_de_nomes_de_autores_e_anos(No *raiz) {
  Consulta consulta;
  strcpy(consulta.nomeInicial, le_nome_da_entrada());
  strcpy(consulta.nomeFinal, le_nome_da_entrada());
  scanf("%u", &consulta.anoInicial);
  scanf("%u", &consulta.anoFinal);
  consulta.qtdResultados = 0;
  consulta.tipo = CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES_E_ANOS;

  imprime_registros_que_correspondem_a_consulta(raiz, &consulta);
  if (consulta.qtdResultados == 0) {
    printf("nao foi encontrado registro com nome entre: \"%s\" e \"%s\" e ano entre: %d e %d\n", consulta.nomeInicial, consulta.nomeFinal, consulta.anoInicial, consulta.anoFinal);
  }
}

void processa_comando_imprime_indice_da_arvore(No *raiz) {
  imprime_indice_da_arvore(raiz);
}

void processa_comando_imprime_pagina(No *raiz) {
  int indicePagina;
  scanf("%d", &indicePagina);
  printf("pagina: %d\n", indicePagina);
  Pagina pagina;
  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r");
  do {
    pagina = le_pagina_do_arquivo(arquivo, indicePagina);
    for (int j = 0; j < NREGSPORPAGINA; j++) {
      Registro registro = le_registro_do_arquivo(arquivo, indicePagina, j);
      if (registro.ocupado) {
        printf("%s\n", registro.obra.autor);
        printf("%s\n", registro.obra.nome);
        printf("%u\n", registro.obra.ano);
        printf("%s\n", registro.obra.arquivo);
      }
    }
    indicePagina = pagina.proxima;
  } while (pagina.proxima != -1);
  fclose(arquivo);
}

void imprime_registros_da_pagina(int indicePagina, Consulta *consulta) {
  Pagina pagina;
  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r");

  do {
    pagina = le_pagina_do_arquivo(arquivo, indicePagina);
    for (int i = 0; i < NREGSPORPAGINA; i++) {
      Registro registro = le_registro_do_arquivo(arquivo, indicePagina, i);
      if (registro.ocupado && compara_obra_com_consulta(&registro.obra, consulta)) {
        (*consulta).qtdResultados++;
        printf("nome: %s\n", registro.obra.autor);
        printf("%s\n", registro.obra.nome);
        printf("%u\n", registro.obra.ano);
        printf("%s\n", registro.obra.arquivo);
      }
    }
    // printf("LOG - pagina: %d\n", indicePagina);
    // printf("LOG - proxima: %d\n", pagina.proxima);
    // return;
    indicePagina = pagina.proxima;
  } while (pagina.proxima != -1);

  fclose(arquivo);
}

bool compara_obra_com_consulta(Obra *obra, Consulta *consulta) {
  if (
    (
      consulta->tipo == CONSULTA_SIMPLES 
      && strcmp(obra->autor, consulta->nomeInicial) == 0
    )
    || (
      consulta->tipo == CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES
      && strcmp(obra->autor, consulta->nomeInicial) >= 0
      && strcmp(obra->autor, consulta->nomeFinal) <= 0
    )
    || (
      consulta->tipo == CONSULTA_POR_FAIXA_DE_ANOS
      && obra->ano >= consulta->anoInicial
      && obra->ano <= consulta->anoFinal
    )
    || (
      consulta->tipo == CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES_E_ANOS
      && strcmp(obra->autor, consulta->nomeInicial) >= 0
      && strcmp(obra->autor, consulta->nomeFinal) <= 0
      && obra->ano >= consulta->anoInicial
      && obra->ano <= consulta->anoFinal
    )
  ) {
    return true;
  }
  return false;
}

void insere_obra_na_pagina(FILE *arquivo, int indicePagina, Obra *obra) {

  CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
  Pagina pagina = le_pagina_do_arquivo(arquivo, indicePagina);
  while(pagina.proxima != -1) {
    indicePagina = pagina.proxima;
    pagina = le_pagina_do_arquivo(arquivo, indicePagina);
  }
  Registro registro;
  registro.ocupado = true;
  registro.obra = *obra;
  escreve_registro_no_arquivo(arquivo, indicePagina, pagina.qtdRegistros, registro);
  pagina.qtdRegistros++;
  // Se a pagina estiver cheia, vamos criar uma nova pagina vazia no final do arquivo e referenciar a nova pagina na pagina antiga, usando o contador
  if (pagina.qtdRegistros == NREGSPORPAGINA) {
    pagina.proxima = cabecalho.qtdPaginas;
    insere_pagina_vazia_no_final_do_arquivo(arquivo);
  }
  
  escreve_pagina_no_arquivo(arquivo, indicePagina, pagina);
}

void atualiza_cabecalho_do_arquivo(FILE *arquivo, CabecalhoArquivo cabecalho) {
  fseek(arquivo, 0, SEEK_SET);
  fwrite(&cabecalho, sizeof(CabecalhoArquivo), 1, arquivo);
}

void escreve_pagina_no_arquivo(FILE *arquivo, int indicePagina, Pagina pagina) {
  CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
  long int posicao = 1 * sizeof(CabecalhoArquivo) 
    + cabecalho.qtdNos * sizeof(Obra)
    + indicePagina * sizeof(Pagina)
    + indicePagina * NREGSPORPAGINA * sizeof(Registro);

  fseek(arquivo, posicao, SEEK_SET);
  fwrite(&pagina, sizeof(Pagina), 1, arquivo);
}

Pagina le_pagina_do_arquivo(FILE *arquivo, int indicePagina) {
  CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
  long int posicao = 1 * sizeof(CabecalhoArquivo)
    + cabecalho.qtdNos * sizeof(Obra)
    + indicePagina * sizeof(Pagina)
    + indicePagina * NREGSPORPAGINA * sizeof(Registro);
  Pagina pagina;
  fseek(arquivo, posicao, SEEK_SET);
  fread(&pagina, sizeof(Pagina), 1, arquivo);
  return pagina;
}

Registro le_registro_do_arquivo(FILE *arquivo, int indicePagina, int indiceRegistro) {
  CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
  
  long int posicao = 1 * sizeof(CabecalhoArquivo)
    + cabecalho.qtdNos * sizeof(Obra)
    + (indicePagina + 1) * sizeof(Pagina)
    + indicePagina * NREGSPORPAGINA * sizeof(Registro)
    + indiceRegistro * sizeof(Registro);

  Registro registro;
  fseek(arquivo, posicao, SEEK_SET);
  fread(&registro, sizeof(Registro), 1, arquivo);
  return registro;
}

void escreve_registro_no_arquivo(FILE *arquivo, int indicePagina, int indiceRegistro, Registro registro) {
  CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
  // printf("LOG escreve_registro_no_arquivo: indicePagina: %d, indiceRegistro: %d\n", indicePagina, indiceRegistro);
  long int posicao = 1 * sizeof(CabecalhoArquivo)
    + cabecalho.qtdNos * sizeof(Obra)
    + (indicePagina + 1) * sizeof(Pagina)
    + indicePagina * NREGSPORPAGINA * sizeof(Registro)
    + indiceRegistro * sizeof(Registro);

  fseek(arquivo, posicao, SEEK_SET);
  fwrite(&registro, sizeof(Registro), 1, arquivo);
}

FILE *abre_arquivo(char *nomeArquivo, char *modo){
  FILE *arquivo;
  if (!(arquivo = fopen(nomeArquivo, modo))) {
    printf("erro na tentativa de abrir arquivo \"%s\".\n", nomeArquivo);
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
  strcpy(obra->autor, le_nome_da_entrada());
  strcpy(obra->nome, le_nome_da_entrada());
  scanf("%u", &obra->ano);
  scanf("%s", obra->arquivo);
  return obra;
}

Pagina cria_pagina_vazia() {
  Pagina pagina;
  pagina.qtdRegistros = 0;
  pagina.proxima = -1;
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

void insere_pagina_vazia_no_final_do_arquivo(FILE *arquivo) {
  CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
  Pagina pagina = cria_pagina_vazia();

  escreve_pagina_no_arquivo(arquivo, cabecalho.qtdPaginas, pagina);
  for (int i = 0; i < NREGSPORPAGINA; i++) {
    Registro registro = cria_registro_vazio();
    escreve_registro_no_arquivo(arquivo, cabecalho.qtdPaginas, i, registro);
  }
  cabecalho.qtdPaginas++;
  atualiza_cabecalho_do_arquivo(arquivo, cabecalho);
}

void teste_imprimir_paginas() {

  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r");

  CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
  // return;
  for (unsigned int i = 0; i < cabecalho.qtdNos + 1; i++) {
    printf("LOG - Pagina %u:\n", i);
    Pagina pagina;
    int indicePagina = i;
    do {
      pagina = le_pagina_do_arquivo(arquivo, indicePagina);
      printf("LOG - teste_imprimir_paginas - qtdPaginas: %d\n", cabecalho.qtdPaginas);
      printf("LOG - teste_imprimir_paginas - qtdNos: %d\n", cabecalho.qtdNos);
      printf("LOG - teste_imprimir_paginas - pagina: %d\n", indicePagina);
      printf("LOG - teste_imprimir_paginas - qtdRegistros: %d\n", pagina.qtdRegistros);
      printf("LOG - teste_imprimir_paginas - proxima: %d\n", pagina.proxima);
      for (int j = 0; j < pagina.qtdRegistros; j++) {
        Registro registro = le_registro_do_arquivo(arquivo, indicePagina, j);
        if (registro.ocupado) {
          printf("LOG - %s", registro.obra.autor);
          printf("LOG - %s", registro.obra.nome);
          printf("LOG - %u\n", registro.obra.ano);
          printf("LOG - %s\n\n", registro.obra.arquivo);
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
  printf("LOG - Cabecalho:\n");
  printf("LOG - qtdNos: %u\n", cabecalho.qtdNos);
  printf("LOG - qtdPaginas: %u\n", cabecalho.qtdPaginas);
  for (unsigned int i = 0; i < cabecalho.qtdNos; i++) {
    Obra obra;
    fread(&obra, sizeof(Obra), 1, arquivo);
    printf("LOG - %s\n", obra.autor);
    printf("LOG - %s\n", obra.nome);
    printf("LOG - %u\n", obra.ano);
    printf("LOG - %s\n\n", obra.arquivo);
  }
  fclose(arquivo);
}

// compara no com a consulta e retorna e indica qual no deve ser seguido
// -1 deve ir para o no esquerdo
// 0 deve ir para os dois nos
// 1 deve ir para o no direito
int compara_no_com_consulta(No *no, Consulta *consulta) {
  if (no->tipo == TIPO_NO_AUTOR) {
    if (consulta->tipo == CONSULTA_SIMPLES) {
      if (strcmp(consulta->nomeInicial, no->autor) <= 0) {
        return -1;
      }
      else {
        return 1;
      }
    }
    else if (consulta->tipo == CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES || consulta->tipo == CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES_E_ANOS) {
      if (strcmp(no->autor, consulta->nomeInicial) >= 0 && strcmp(no->autor, consulta->nomeFinal) <= 0) {
        return 0;
      }
      else if (strcmp(no->autor, consulta->nomeInicial) < 0) {
        return 1;
      }
      else {
        return -1;
      }
    }
    else if (consulta->tipo == CONSULTA_POR_FAIXA_DE_ANOS) {
      return 0;
    }
  }
  else {
    if (consulta->tipo == CONSULTA_SIMPLES || consulta->tipo == CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES) {
      return 0;
    }
    else if (consulta->tipo == CONSULTA_POR_FAIXA_DE_ANOS || consulta->tipo == CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES_E_ANOS) {
      if (no->ano >= consulta->anoInicial && no->ano <= consulta->anoFinal) {
        return 0;
      }
      else if (no->ano < consulta->anoInicial) {
        return 1;
      }
      else {
        return -1;
      }
    }
  }
}

void imprime_registros_que_correspondem_a_consulta(No *no, Consulta *consulta) {

  int resultado = compara_no_com_consulta(no, consulta);
  // printf("LOG - Resultado da comparação: %d\n", resultado);
  if (resultado == -1) {
    if (no->noFilhoEsquerdo == NULL) {
      imprime_registros_da_pagina(no->indicePaginaEsquerda, consulta);
    }
    else {
      imprime_registros_que_correspondem_a_consulta(no->noFilhoEsquerdo, consulta);
    }
  }
  else if (resultado == 0) {
    if (no->noFilhoEsquerdo == NULL) {
      imprime_registros_da_pagina(no->indicePaginaEsquerda, consulta);
    }
    else {
      imprime_registros_que_correspondem_a_consulta(no->noFilhoEsquerdo, consulta);
    }
    if (no->noFilhoDireito == NULL) {
      imprime_registros_da_pagina(no->indicePaginaDireita, consulta);
    }
    else {
      imprime_registros_que_correspondem_a_consulta(no->noFilhoDireito, consulta);
    }
  }
  else {
    if (no->noFilhoDireito == NULL) {
      imprime_registros_da_pagina(no->indicePaginaDireita, consulta);
    }
    else {
      imprime_registros_que_correspondem_a_consulta(no->noFilhoDireito, consulta);
    }
  }
}

char *le_nome_da_entrada() {
  char *nome = (char *) malloc(TAMANHO_NOME * sizeof(char));
  fgets(nome, TAMANHO_NOME, stdin);
  
  if (nome[0] == '\n') {
    return le_nome_da_entrada();
  }

  if ((strlen(nome) > 0) && (nome[strlen (nome) - 1] == '\n')) {
    nome[strlen (nome) - 1] = '\0';
  }

  return nome;
}

void imprime_indice_da_arvore(No *no) {
  if (no->tipo == TIPO_NO_AUTOR) {
    printf("nome: %s ", no->autor);
    if (no->noFilhoEsquerdo == NULL) {
      printf("fesq: pagina ");
    } else {
      printf("fesq: %u ", no->noFilhoEsquerdo->ano);
    }
    if (no->noFilhoDireito == NULL) {
      printf("fdir: pagina");
    } else {
      printf("fdir: %u", no->noFilhoDireito->ano);
    }
    printf("\n");
  }
  else {
    printf("ano: %u ", no->ano);
    if (no->noFilhoEsquerdo == NULL) {
      printf("fesq: pagina ");
    } else {
      printf("fesq: %s ", no->noFilhoEsquerdo->autor);
    }
    if (no->noFilhoDireito == NULL) {
      printf("fdir: pagina");
    } else {
      printf("fdir: %s", no->noFilhoDireito->autor);
    }
    printf("\n");
  }
  if (no->noFilhoEsquerdo != NULL) {
    imprime_indice_da_arvore(no->noFilhoEsquerdo);
  }
  if (no->noFilhoDireito != NULL) {
    imprime_indice_da_arvore(no->noFilhoDireito);
  }
}
