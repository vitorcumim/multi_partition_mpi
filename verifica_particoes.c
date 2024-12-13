// Feito por:
// Vitor Lorenzo Cumim - GRR20224757
// Rafael Urbanek Laurentino - GRR20224381

#include "verifica_particoes.h"

void verifica_particoes(long long *Input, int n, long long *P, int np, long long *Output, int *nO) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obtém o rank do processo atual

    int erro = 0; // Flag de erro para o processo

    // Verifica se os valores em Output estão dentro dos limites de partições
    for (int i = 0; i < *nO; i++) {
        int found_partition = 0;
        for (int j = 0; j < np - 1; j++) {
            if (Output[i] <= P[j]) {
                found_partition = 1;
                break;
            }
            else if (Output[i] >= P[j] && Output[i] < P[j + 1]) {
                found_partition = 1;
                break;
            }
        }
        if (!found_partition) {
            erro = 1;
            break;
        }
    }

    // Imprime a mensagem dependendo do erro
    if (erro) {
        printf("Rank %d: ===> particionamento COM ERROS\n", rank);
    } else {
        printf("Rank %d: ===> particionamento CORRETO\n", rank);
    }
}
