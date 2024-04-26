#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Definição da estrutura para representar um ponto no plano
typedef struct {
    float x;
    float y;
} Ponto;

// Função para calcular a área de um triângulo com base nas coordenadas dos vértices
float calcularAreaTriangulo(Ponto *A, Ponto *B, Ponto *C) {
    // Calcula a área usando o método do produto cruzado
    float area = 0.5 * ((B->x - A->x) * (C->y - A->y) - (C->x - A->x) * (B->y - A->y));
    return fabs(area); // Retorna o valor absoluto da área
}

int main() {
    FILE *arquivo;
    system("cls");

    arquivo = fopen("vertices.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    int numVertices;
    fscanf(arquivo, "%d\n", &numVertices);

    Ponto vertices[numVertices];

    if (numVertices < 3)
        printf("Poligono incompativel. (menos que 3 vertices)\n");
    else {
        printf("Calculando a area de um poligono com %d vertices\n", numVertices);
        int contVertices = 0;
        float areaTotal = 0;
        char linha[256];

        // Lendo as coordenadas dos vértices a partir do arquivo
        while (fgets(linha, sizeof(linha), arquivo)) {
            sscanf(linha, "%f %f", &vertices[contVertices].x, &vertices[contVertices].y);
            printf("Vertice %d: %.2f \t%.2f\n", contVertices + 1, vertices[contVertices].x, vertices[contVertices].y);
            contVertices++;
        }

        if (numVertices == 3) {
            areaTotal = calcularAreaTriangulo(&vertices[0], &vertices[1], &vertices[2]);
            printf("A area do poligono eh: %.2f\n", areaTotal);
        } else {
            for (int i = 1; i < numVertices - 1; i++) {
                float areaTriangulo = calcularAreaTriangulo(&vertices[0], &vertices[i], &vertices[i + 1]);
                areaTotal += areaTriangulo;
            }
            printf("A area total do poligono eh: %.2f\n", areaTotal);
        }
    }

    fclose(arquivo);

    return 0;
}