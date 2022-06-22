#pragma once

#include <stdbool.h>

#include "obra.h"

typedef struct Registro Registro;

struct Registro{
    bool ocupado;
    Obra obra;
};