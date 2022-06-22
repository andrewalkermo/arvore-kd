#include "utils.h"

// cria a pagina e inicializa com registros vazios a quantidade maxima de registros
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

// aloca o espaço na memória para a struct de obra e faz leitura de registros da entrada
Obra *le_obra_da_entrada() {
    Obra *obra = (Obra*) malloc(sizeof(Obra));
    strcpy(obra->autor, le_nome_da_entrada());
    strcpy(obra->nome, le_nome_da_entrada());
    scanf("%u", &obra->ano);
    scanf("%s", obra->arquivo);
    return obra;
}
