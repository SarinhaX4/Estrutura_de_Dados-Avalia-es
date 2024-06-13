#include "cidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função de comparação para ordenação das cidades
int comparaCidades(const void *cidade1, const void *cidade2) {
    return ((Cidade *)cidade1)->Posicao - ((Cidade *)cidade2)->Posicao;
}

// Função para ler as informações da estrada a partir de um arquivo
Estrada *carregaEstrada(const char *arquivo) {
    FILE *arquivoEstrada = fopen(arquivo, "r");
    if (!arquivoEstrada) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    Estrada *novaEstrada = (Estrada *)malloc(sizeof(Estrada));
    if (!novaEstrada) {
        perror("Erro de alocação de memória para a estrada");
        fclose(arquivoEstrada);
        return NULL;
    }

    fscanf(arquivoEstrada, "%d", &novaEstrada->T);
    fscanf(arquivoEstrada, "%d", &novaEstrada->N);

    novaEstrada->C = (Cidade *)malloc(novaEstrada->N * sizeof(Cidade));
    if (!novaEstrada->C) {
        perror("Erro de alocação de memória para as cidades");
        fclose(arquivoEstrada);
        free(novaEstrada);
        return NULL;
    }

    for (int i = 0; i < novaEstrada->N; i++) {
        if (fscanf(arquivoEstrada, "%d ", &novaEstrada->C[i].Posicao) != 1) {
            fprintf(stderr, "Erro ao ler a posição da cidade %d\n", i);
            fclose(arquivoEstrada);
            free(novaEstrada->C);
            free(novaEstrada);
            return NULL;
        }
        fgets(novaEstrada->C[i].Nome, 256, arquivoEstrada);
        novaEstrada->C[i].Nome[strcspn(novaEstrada->C[i].Nome, "\n")] = '\0';

        for (int j = 0; j < i; j++) {
            if (novaEstrada->C[i].Posicao == novaEstrada->C[j].Posicao) {
                fprintf(stderr, "Erro: posição duplicada para as cidades\n");
                fclose(arquivoEstrada);
                free(novaEstrada->C);
                free(novaEstrada);
                return NULL;
            }
        }
    }

    fclose(arquivoEstrada);
    return novaEstrada;
}

// Função para calcular a menor vizinhança entre as cidades
double menorDistanciaEntreCidades(const char *arquivo) {
    Estrada *estrada = carregaEstrada(arquivo);
    if (!estrada)
        return -1.0;

    qsort(estrada->C, estrada->N, sizeof(Cidade), comparaCidades);

    double menorDistancia = estrada->T;

    for (int i = 0; i < estrada->N; i++) {
        double inicio = (i == 0) ? 0 : (estrada->C[i].Posicao + estrada->C[i - 1].Posicao) / 2.0;
        double fim = (i == estrada->N - 1) ? estrada->T : (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        double vizinhanca = fim - inicio;

        if (vizinhanca < menorDistancia)
            menorDistancia = vizinhanca;
    }

    free(estrada->C);
    free(estrada);

    return menorDistancia;
}

// Função para encontrar a cidade com a menor vizinhança
char *encontraCidadeComMenorDistancia(const char *arquivo) {
    Estrada *estrada = carregaEstrada(arquivo);
    if (!estrada)
        return NULL;

    qsort(estrada->C, estrada->N, sizeof(Cidade), comparaCidades);

    double menorDistancia = estrada->T;
    char *cidadeMenorDistancia = NULL;

    for (int i = 0; i < estrada->N; i++) {
        double inicio = (i == 0) ? 0 : (estrada->C[i].Posicao + estrada->C[i - 1].Posicao) / 2.0;
        double fim = (i == estrada->N - 1) ? estrada->T : (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        double vizinhanca = fim - inicio;

        if (vizinhanca < menorDistancia) {
            menorDistancia = vizinhanca;

            cidadeMenorDistancia = malloc(strlen(estrada->C[i].Nome) + 1);
            if (!cidadeMenorDistancia) {
                perror("Erro de alocação de memória para o nome da cidade");
                free(estrada->C);
                free(estrada);
                return NULL;
            }

            strcpy(cidadeMenorDistancia, estrada->C[i].Nome);
        }
    }

    free(estrada->C);
    free(estrada);

    return cidadeMenorDistancia;
}

int main() {
    const char *arquivo = "caminhos.txt";

    double menorDistancia = menorDistanciaEntreCidades(arquivo);
    char *cidadeMenorDistancia = encontraCidadeComMenorDistancia(arquivo);

    printf("Menor vizinhança de estrada: %.2f\n", menorDistancia);
    if (cidadeMenorDistancia) {
        printf("Cidade com menor vizinhança: %s\n", cidadeMenorDistancia);
        free(cidadeMenorDistancia); 
    } else {
        printf("Nenhuma cidade encontrada.\n");
    }

    return 0;
}
