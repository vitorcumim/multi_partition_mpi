// Feito por:
// Vitor Lorenzo Cumim - GRR20224757
// Rafael Urbanek Laurentino - GRR20224381

#ifndef VERIFICA_PARTICOES_H
#define VERIFICA_PARTICOES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <mpi.h>

typedef long long ll;  // Definindo um alias para o tipo long long

// Função responsável por verificar se o vetor foi particionado corretamente
void verifica_particoes(long long *Input, int n, long long *P, int np, long long *Output, int *nO);

#endif // VERIFICA_PARTICOES_H
