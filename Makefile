# Script de compilação
# Feito por:
# Vitor Lorenzo Cumim - GRR20224757
# Rafael Urbanek Laurentino - GRR20224381

# Nome dos arquivos
PARTICIONA = particiona
SRC = particiona.c
LIB = chrono.c verifica_particoes.c

# Compilador
CC = mpicc

# Opções de compilação
CFLAGS = -g -Wall -O3

# Arquivos objeto
OBJ_SRC = particiona.o
OBJ_LIB = chrono.o verifica_particoes.o
OBJ = $(OBJ_SRC) $(OBJ_LIB)

# Regra principal para compilar o executável
all: $(PARTICIONA)

# Regra para compilar o executável
$(PARTICIONA): $(OBJ)
	$(CC) $(OBJ) -o $(PARTICIONA)

# Regras para compilar os arquivos objeto
particiona.o: particiona.c
	$(CC) $(CFLAGS) -c particiona.c -o particiona.o

chrono.o: chrono.c
	$(CC) $(CFLAGS) -c chrono.c -o chrono.o

verifica_particoes.o: verifica_particoes.c
	$(CC) $(CFLAGS) -c verifica_particoes.c -o verifica_particoes.o

# Limpeza dos arquivos gerados
clean:
	rm -f $(PARTICIONA) $(OBJ)
