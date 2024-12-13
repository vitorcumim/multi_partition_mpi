#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "chrono.h"
#include "verifica_particoes.h"

#define NTIMES 10

// Definindo o número total de elementos como variável global 
int nTotalElements;

// Gera números aleatórios long long
long long geraAleatorioLL() {
    int a = rand();
    int b = rand();
    return (long long)a * 100 + b;
}

// Comparador para qsort
int cmpfunc(const void *a, const void *b) {
    return (*(long long *)a - *(long long *)b);
}

// Função multi_partition_mpi
void multi_partition_mpi(long long *input, int local_size, long long *P, int np, long long *output, int *nO) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // 1. Particionar localmente
    int *send_counts = (int *)calloc(np, sizeof(int));
    long long **local_bins = (long long **)malloc(np * sizeof(long long *));
    for (int i = 0; i < np; i++) {
        local_bins[i] = (long long *)malloc(local_size * sizeof(long long));
    }

    // Particionamento dos dados
    for (int i = 0; i < local_size; i++) {
        for (int j = 0; j < np; j++) {
            if (input[i] <= P[j]) {
                local_bins[j][send_counts[j]++] = input[i];
                break;
            }
        }
    }

    // Preparar buffers de envio
    int *send_displs = (int *)calloc(np, sizeof(int));
    send_displs[0] = 0;
    for (int i = 1; i < np; i++) {
        send_displs[i] = send_displs[i - 1] + send_counts[i - 1];
    }

    long long *send_buffer = (long long *)malloc(local_size * sizeof(long long));
    int offset = 0;
    for (int i = 0; i < np; i++) {
        memcpy(&send_buffer[offset], local_bins[i], send_counts[i] * sizeof(long long));
        offset += send_counts[i];
    }

    // 2. Preparar para redistribuição
    int *recv_counts = (int *)malloc(np * sizeof(int)); // Para receber contagens de elementos
    int *recv_displs = (int *)malloc(np * sizeof(int));

    MPI_Alltoall(send_counts, 1, MPI_INT, recv_counts, 1, MPI_INT, MPI_COMM_WORLD);

    recv_displs[0] = 0;
    int total_recv = recv_counts[0];
    for (int i = 1; i < np; i++) {
        recv_displs[i] = recv_displs[i - 1] + recv_counts[i - 1];
        total_recv += recv_counts[i];
    }

    long long *recv_buffer = (long long *)malloc(total_recv * sizeof(long long));

    // 3. Redistribuir os dados
    MPI_Alltoallv(
        send_buffer, send_counts, send_displs, MPI_LONG_LONG,
        recv_buffer, recv_counts, recv_displs, MPI_LONG_LONG,
        MPI_COMM_WORLD
    );

    // 4. Copiar para o vetor de saída
    memcpy(output, recv_buffer, total_recv * sizeof(long long));
    *nO = total_recv;

    // Limpeza de memória temporária
    for (int i = 0; i < np; i++) {
        free(local_bins[i]);
    }
    free(local_bins);
    free(send_counts);
    free(send_displs);
    free(recv_counts);
    free(recv_displs);
    free(send_buffer);
    free(recv_buffer);
}

// Program Principal
int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    // Criando cronometro.
    chronometer_t parallelReductionTime;

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // Inicialização da semente aleatória
    srand(2024 * 100 + rank);

    // Configuração do tamanho total dos elementos e local
    nTotalElements = atoi(argv[1]);
    int local_size = nTotalElements / nprocs;

    // Geração do vetor Input
    long long *input = (long long *)malloc(local_size * sizeof(long long));
    for (int i = 0; i < local_size; i++) {
        input[i] = geraAleatorioLL();
    }

    // Geração do vetor de particionamento P
    long long *P = (long long *)malloc(nprocs * sizeof(long long));
    if (rank == 0) {
        for (int i = 0; i < nprocs - 1; i++) {
            P[i] = geraAleatorioLL();
        }
        P[nprocs - 1] = LLONG_MAX;
        qsort(P, nprocs, sizeof(long long), cmpfunc);
    }

    // Distribuir P para todos os processos
    MPI_Bcast(P, nprocs, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // Buffer de saída e contagem
    long long *output = (long long *)malloc(nTotalElements * sizeof(long long));
    int nO;

    // Iniciando o cronometro.
    chrono_reset(&parallelReductionTime);
    chrono_start(&parallelReductionTime);

    for (int i = 0; i < NTIMES; i++) {
        // Chama a função multi_partition_mpi
        multi_partition_mpi(input, local_size, P, nprocs, output, &nO);
    }

    // Para o cronometro e imprime o tempo.
    chrono_stop(&parallelReductionTime);

    double total_time_in_seconds = (double)chrono_gettotal(&parallelReductionTime) / ((double)1000*1000*1000);
    printf("total_time_in_seconds: %lf s\n", total_time_in_seconds);

    // Verifica se o particionamento está correto
    verifica_particoes(input,local_size,P,nprocs,output,&nO);

    // Exibe informações sobre o processo
    printf("Rank %d recebeu %d elementos.\n", rank, nO);

    free(input);
    free(output);
    free(P);

    MPI_Finalize();
    return 0;
}
