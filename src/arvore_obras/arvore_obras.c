#include "arvore_obras.h"

// primeiro verifica o tipo de no a ser iserido
// caso o tipo de no seja autor, faz a verificação com o auxilio da função srtcmp que compara a srings em ordem alfabetica,
// se o nome do autor da obra é menor ou igual ao nome do autor do nó a inserção ocorre a esquerda, e caso seja maior, ocorre a direita
// caso o tipo de de no seja do tipo ano, faz a comparação entre inteiros, caso o ano da obra seja menor ou igual ao ano do nó a inserção ocorre à esquerda e caso seja maior, ocorre a direita
// a insercao ocorre de forma recursiva, percorrendo a arvore ate que seja encontrado o nó esquerdo ou direto que seja NULL
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

bool no_esta_vazio(No *no) {
  return (no->tipo == TIPO_NO_AUTOR && no->autor[0] == '\0') ||
         (no->tipo == TIPO_NO_ANO && no->ano == 0);
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

// imprime todos os indices da arvore
// comecando na raiz e descendo na arvore imprimindo os nos da esquerda e depois da direita para cada no
// e caso o no corresponda a uma página, imprime a informacao de pagina
void imprime_indice_da_arvore(No *no) {
    if (no->noFilhoEsquerdo != NULL) {
        imprime_indice_da_arvore(no->noFilhoEsquerdo);
    }
    if (no->tipo == TIPO_NO_AUTOR) {
        printf("nome: %s ", no->autor);
        if (no->noFilhoEsquerdo == NULL) {
            printf("fesq: pagina ");
        } else {
            printf("fesq: %u ", no->noFilhoEsquerdo->ano);
        }
        // verifica se e um no folha
        if (no->noFilhoDireito == NULL) {
            printf("fdir: pagina");
        } else {
            printf("fdir: %u", no->noFilhoDireito->ano);
        }
        printf("\n");
    }
    else {
        printf("ano: %u ", no->ano);
        // verifica se e um no folha
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
    if (no->noFilhoDireito != NULL) {
        imprime_indice_da_arvore(no->noFilhoDireito);
    }
}
