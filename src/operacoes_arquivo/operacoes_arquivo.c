#include "operacoes_arquivo.h"

FILE *abre_arquivo(char *nomeArquivo, char *modo){
  FILE *arquivo;
  if (!(arquivo = fopen(nomeArquivo, modo))) {
    printf("Erro na tentativa de abrir arquivo \"%s\".\n", nomeArquivo);
    exit(-1);
  }
  return arquivo;
}


CabecalhoArquivo le_cabecalho_do_arquivo(FILE *arquivo) {
  CabecalhoArquivo cabecalhoArquivo;
  fseek(arquivo, 0, SEEK_SET);
  fread(&cabecalhoArquivo, sizeof(CabecalhoArquivo), 1, arquivo);
  return cabecalhoArquivo;
}

void escreve_pagina_no_arquivo(FILE *arquivo, int indicePagina, Pagina pagina) {
    CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
    long int posicao = 1 * sizeof(CabecalhoArquivo) + cabecalho.qtdNos * sizeof(Obra) + indicePagina * sizeof(Pagina);
    fseek(arquivo, posicao, SEEK_SET);
    fwrite(&pagina, sizeof(Pagina), 1, arquivo);
}

// le a pagina e a retorna
Pagina le_pagina_do_arquivo(FILE *arquivo, int indicePagina) {
    CabecalhoArquivo cabecalho = le_cabecalho_do_arquivo(arquivo);
    long int posicao = 1 * sizeof(CabecalhoArquivo) + cabecalho.qtdNos * sizeof(Obra) + indicePagina * sizeof(Pagina);
    fseek(arquivo, posicao, SEEK_SET);
    Pagina pagina;
    fread(&pagina, sizeof(Pagina), 1, arquivo);
    return pagina;
}


void atualiza_cabecalho_do_arquivo(FILE *arquivo, CabecalhoArquivo cabecalho) {
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&cabecalho, sizeof(CabecalhoArquivo), 1, arquivo);
}

// faz a leitura da pagina no arquivo e caso a pagina nao esteja cheia, adiciona a obra na pagina e incrementa a quantidade de registros na pagina
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

// verifica o tipo de no a ser inserido
// faz o comparativo de autor e de ano como descrito para a funcao cria_no_e_insere_na_arvore
// e insere a obra na pagina
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
