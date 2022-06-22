#include "consultas.h"

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

    //esperado que retorne antes de chegar neste ponto
    printf("Erro na consulta");
    exit(-1);
}

// Primeiro verifica-se o tipo de consulta
// Na consulta simples, retorna true se houver registros na árvore com o valor de nome indicado
// Na consulta por faixa de nome de autores e verificado se o nome do autor for maior ou igual ao nome do autor nomeInicial e menor ao nome do autor nomeFinal, considerando-se a ordem alfabetica
// Na consulta por faixa de nome de ano e verificado se o ano da obra é maior ou igual ao nome do ano anoInicial e menor ao ano anoFinal
// Na consulta por faixa de nome e de autores e verificado se a obra se encontra em ambos os intervalos das consultas descritas acima

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