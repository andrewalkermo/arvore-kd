#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../globais/comandos.h"
#include "../tipos/consulta.h"
#include "../tipos/no.h"
#include "../tipos/obra.h"

int compara_no_com_consulta(No *no, Consulta *consulta);
bool compara_obra_com_consulta(Obra *obra, Consulta *consulta);
