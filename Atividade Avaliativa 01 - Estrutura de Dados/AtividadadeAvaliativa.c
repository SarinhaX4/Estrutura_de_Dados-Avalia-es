#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes
#define ERRO -1
#define SUCESSO 0
#define MAX_ALUNOS 100

// Estrutura de Dados
typedef struct {
    char nome[50];
    char telefone[20];
    char curso[50];
    float nota1;
    float nota2;
} Aluno;

// Função para ler o arquivo e preencher a estrutura Aluno
int lerArquivo(Aluno alunos[], int *numAlunos) {
    FILE *arquivo;
    char linha[100];
    char *pedaco;
    int contador = 0;

    arquivo = fopen("DadosEntrada.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo 'DadosEntrada.csv'\n");
        return ERRO;
    }

    // Ignorar a primeira linha do arquivo
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL && contador < MAX_ALUNOS) {
        pedaco = strtok(linha, ",");
        strcpy(alunos[contador].nome, pedaco);

        pedaco = strtok(NULL, ",");
        strcpy(alunos[contador].telefone, pedaco);

        pedaco = strtok(NULL, ",");
        strcpy(alunos[contador].curso, pedaco);

        pedaco = strtok(NULL, ",");
        alunos[contador].nota1 = atof(pedaco);

        pedaco = strtok(NULL, ",");
        alunos[contador].nota2 = atof(pedaco);

        contador++;
    }

    fclose(arquivo);
    *numAlunos = contador;
    return SUCESSO;
}

// Função para calcular a média
float calcularMedia(float nota1, float nota2) {
    return (nota1 + nota2) / 2.0;
}

// Função para calcular a situação do aluno e escrever no arquivo
void calcularSituacao(Aluno alunos[], int numAlunos) {
    FILE *arquivo;

    arquivo = fopen("SituacaoFinal.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo 'SituacaoFinal.csv'\n");
        return;
    }

    fprintf(arquivo, "Nome,Média,Situação\n");

    for (int i = 0; i < numAlunos; i++) {
        float media = calcularMedia(alunos[i].nota1, alunos[i].nota2);
        fprintf(arquivo, "%s, %.2f, %s\n", alunos[i].nome, media, (media >= 7.0 ? "Aprovado" : "Reprovado"));
    }

    fclose(arquivo);
}

// Função principal
int main() {
    Aluno alunos[MAX_ALUNOS];
    int numAlunos = 0;

    if (lerArquivo(alunos, &numAlunos) != SUCESSO) {
        printf("Erro ao ler o arquivo.\n");
        return ERRO;
    }

    calcularSituacao(alunos, numAlunos);

    printf("Processamento concluído. Verifique o arquivo 'SituacaoFinal.csv'\n");

    return SUCESSO;
}
