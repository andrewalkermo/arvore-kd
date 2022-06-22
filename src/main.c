#include "main.h"

int main() {
  No *raiz = prepara_arvore();
  processa_comandos(raiz);
  return 0;
}

void processa_comandos(No *raiz) {
  char comando;
  while (scanf("%c", &comando)) {
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

// verifica se ja existe o arquivo de registros, caso ja exista, 
// inicializa a arvore com os registros do arquivo como indice da arvore e salva a arvore no arquivo
// se nao existir cria o arquivo pra salvar os registros que serão utilizados como indice da arvore,
// inicializa arvore e salva a arvore no arquivo

No *prepara_arvore() {
  No *raiz;
  FILE *arquivo;

  if ((arquivo = fopen(NOME_ARQUIVO, "r"))) {
    raiz = estrutura_arvore_atraves_de_arquivo(arquivo);
    fclose(arquivo);
  }
  else {
    raiz = inicializa_arvore_e_arquivo_com_entradas();
  }

  return raiz;
}

// le a quantidade de registros a serem inseridos para a criacao da arvore, cria o arquivo
//  e salva no cabeçalho do arquivo a quantidade de registros
// inicializa a arvore com os indices e as paginas com registros vazios
No *inicializa_arvore_e_arquivo_com_entradas() {
  No *raiz = cria_no_vazio(TIPO_NO_AUTOR);
  CabecalhoArquivo cabecalhoArquivo;
  scanf("%u", &cabecalhoArquivo.qtdNos);
  cabecalhoArquivo.qtdPaginas = cabecalhoArquivo.qtdNos + 1;

  // criacao do arquivo e escrita do cabeçalho com a quantidade de nós
  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "w+");
  fwrite(&cabecalhoArquivo, sizeof(CabecalhoArquivo), 1, arquivo);

  // loop que le os registros e insere na arvore
  for (unsigned int i = 0; i < cabecalhoArquivo.qtdNos; i++) {
    Obra *obra = le_obra_da_entrada();
    cria_no_e_insere_na_arvore(raiz, obra);
    fwrite(obra, sizeof(Obra), 1, arquivo);
  }

  // loop que cria e seta as páginas com a quantidade maxima suportada de registros em cada página
  // as paginas sao setadas com registros vazios
  for (unsigned int i = 0; i < cabecalhoArquivo.qtdPaginas; i++) {
    Pagina pagina = cria_pagina_vazia();
    fwrite(&pagina, sizeof(Pagina), 1, arquivo);
  }

  int indice = 0;
  adiciona_indices_das_paginas_na_arvore(raiz, &indice);
  
  // loop que insere os registros no arquivo
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

void adiciona_indices_das_paginas_na_arvore(No *no, int *indice) {
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

// faz a leitura dos registros que estao no arquivo
// inicializa a arvore com os registros do arquivo como indice da arvore e salva a arvore no arquivo
No *estrutura_arvore_atraves_de_arquivo(FILE *arquivo) {
  No *raiz = cria_no_vazio(TIPO_NO_AUTOR);
  CabecalhoArquivo cabecalhoArquivo = le_cabecalho_do_arquivo(arquivo);

  // loop que insere os registros na arvore
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
  Obra *obra = le_obra_da_entrada();
  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r+");
  insere_obra_no_arquivo(raiz, arquivo, obra);
  fclose(arquivo);
  printf("inserido registro com nome: %s\n", obra->autor);
}

// faz a leitura do valor da consulta, verifica se existe registros que correspondam a consulta
// imprime os registros encontrados e caso nao tenha nenhum registro encontrado, imprime mensagem de nao encontrado 
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

// faz a leitura dos valores da consulta, verifica se existem registros que correspondam ao intervalo da consulta
// imprime os registros encontrados e caso nao tenha nenhum registro encontrado, imprime mensagem de nao encontrado 
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

// faz a leitura dos valores da consulta, verifica se existem registros que correspondam ao intervalo da consulta
// imprime os registros encontrados e caso nao tenha nenhum registro encontrado, imprime mensagem de nao encontrado 
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

// faz a leitura dos valores da consulta, verifica se existem registros que correspondam ao intervalo da consulta
// imprime os registros encontrados e caso nao tenha nenhum registro encontrado, imprime mensagem de nao encontrado 
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

//faz a leitura do conteudo da pagina no arquivo e imprime todas as obras
// caso tenha referencia para outra pagina, o conteudo dessa pagina tambem sera impressa
// ate que acabem as paginas referenciadas
void processa_comando_imprime_pagina(No *raiz) {
  int indicePagina;
  scanf("%d", &indicePagina);
  printf("pagina: %d\n", indicePagina);
  Pagina pagina;
  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r");
  do {
    pagina = le_pagina_do_arquivo(arquivo, indicePagina);
    for (int j = 0; j < NREGSPORPAGINA; j++) {
      if (pagina.registros[j].ocupado) {
        printf("%s\n", pagina.registros[j].obra.autor);
        printf("%s\n", pagina.registros[j].obra.nome);
        printf("%u\n", pagina.registros[j].obra.ano);
        printf("%s\n", pagina.registros[j].obra.arquivo);
      }
    }
    indicePagina = pagina.proxima;
  } while (pagina.proxima != -1);
  fclose(arquivo);
}

// le o conteudo da pagina no arquivo e compara a obra com os valores da consulta
// serao impressas todas as obras que sejam iguais ou estejam dentro do intervalo de valores da consulta
void imprime_registros_da_pagina(int indicePagina, Consulta *consulta) {
  Pagina pagina;
  FILE *arquivo = abre_arquivo(NOME_ARQUIVO, "r");
  do {
    pagina = le_pagina_do_arquivo(arquivo, indicePagina);
    // loop para ler todos as obras da pagina
    for (int j = 0; j < NREGSPORPAGINA; j++) {
      if (pagina.registros[j].ocupado && compara_obra_com_consulta(&pagina.registros[j].obra, consulta)) {
        (*consulta).qtdResultados++;
        if (consulta->tipo == CONSULTA_SIMPLES) {
            printf("nome: ");
        }
        printf("%s\n", pagina.registros[j].obra.autor);
        printf("%s\n", pagina.registros[j].obra.nome);
        printf("%u\n", pagina.registros[j].obra.ano);
        printf("%s\n", pagina.registros[j].obra.arquivo);
      }
    }
    indicePagina = pagina.proxima;
  } while (pagina.proxima != -1);
  fclose(arquivo);
}

void imprime_registros_que_correspondem_a_consulta(No *no, Consulta *consulta) {
  // verifica se o no e menor igual ou maior ao valor da consulta pra definir em qual direcao deve-se percorrer a arvore
  int resultado = compara_no_com_consulta(no, consulta);
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
