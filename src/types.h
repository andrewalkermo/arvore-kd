
#include <stdbool.h>

#define TAMANHO_NOME 21

#define NREGSPORPAGINA 3

#define INSERE_REGISTRO 'i'
#define CONSULTA_SIMPLES 'c'
#define CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES 'n'
#define CONSULTA_POR_FAIXA_DE_ANOS 'a'
#define CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES_E_ANOS 'q'
#define IMPRIME_INDICE_DA_ARVORE 't'
#define IMPRIME_PAGINA 'p'
#define TERMINO_DA_SEQUENCIA_DE_COMANDOS 'e'

typedef struct Obra Obra;
typedef struct ArvoreKB ArvoreKB;

struct Obra{
  char autor[TAMANHO_NOME];
  char nome[TAMANHO_NOME];
  unsigned int ano;
  char arquivo[TAMANHO_NOME];
};

struct ArvoreKB{

};

struct NoAutor{
  char nome[TAMANHO_NOME];

};

struct NoAno{
  unsigned int ano;
};

struct Pagina{
  unsigned int qtdRegistros;
  unsigned int proxima;
  Obra registros[NREGSPORPAGINA];
};

struct Registro{
  bool ocupado;
  Obra obra;

};