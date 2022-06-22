
#include <stdbool.h>

#define NREGSPORPAGINA 3
#define TAMANHO_NOME 21
#define NOME_ARQUIVO "arvore.dat"

#define INSERE_REGISTRO 'i'
#define CONSULTA_SIMPLES 'c'
#define CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES 'n'
#define CONSULTA_POR_FAIXA_DE_ANOS 'a'
#define CONSULTA_POR_FAIXA_DE_NOMES_DE_AUTORES_E_ANOS 'q'
#define IMPRIME_INDICE_DA_ARVORE 't'
#define IMPRIME_PAGINA 'p'
#define TERMINO_DA_SEQUENCIA_DE_COMANDOS 'e'

typedef struct No No;
typedef struct Obra Obra;
typedef struct Registro Registro;
typedef struct Pagina Pagina;
typedef struct CabecalhoArquivo CabecalhoArquivo;
typedef struct Consulta Consulta;

typedef enum TipoNo TipoNo;

struct Obra{
  char autor[TAMANHO_NOME];
  char nome[TAMANHO_NOME];
  unsigned int ano;
  char arquivo[TAMANHO_NOME];
};

enum TipoNo {
  TIPO_NO_AUTOR,
  TIPO_NO_ANO,
};

struct No{
  enum TipoNo tipo;
  union {
    char autor[TAMANHO_NOME];
    unsigned int ano;
  };
  No* noFilhoEsquerdo;
  No* noFilhoDireito;
  int indicePaginaEsquerda;
  int indicePaginaDireita;
};

struct Registro{
  bool ocupado;
  Obra obra;
};

struct Pagina{
  unsigned int qtdRegistros;
  int proxima;
  Registro registros[NREGSPORPAGINA];
};

struct CabecalhoArquivo{
  unsigned int qtdNos;
  unsigned int qtdPaginas;
};

struct Consulta{
  char tipo;
  char nomeInicial[TAMANHO_NOME];
  char nomeFinal[TAMANHO_NOME];
  unsigned int anoInicial;
  unsigned int anoFinal;
  unsigned int qtdResultados;
};
