#include <stdlib.h>
#include <stdio.h>
#include <time.h>

clock_t inicio, fim;

typedef struct{
    int Cardinalidade;

    char **NomeItens;

    float *Peso;
    float *Beneficio;

    float *Razao;

    float Capacidade;
}Universo;

void lerArquivoEntrada(const char* nomeArquivo, Universo* universo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    float capacidade;
    int numItens;


    fgets(linha, sizeof(linha), arquivo);
    sscanf(linha, "K %f", &capacidade);
    universo->Capacidade = capacidade;

    fgets(linha, sizeof(linha), arquivo);
    sscanf(linha, "N %d", &numItens);

    universo->Cardinalidade = numItens;
    universo->NomeItens = malloc(numItens * sizeof(char*));
    universo->Peso = malloc(numItens * sizeof(float));
    universo->Beneficio = malloc(numItens * sizeof(float));
    universo->Razao = malloc(numItens * sizeof(float));

    for (int i = 0; i < numItens; i++) {
        fgets(linha, sizeof(linha), arquivo);
        universo->NomeItens[i] = malloc(50 * sizeof(char));
        sscanf(linha, "I %s %f %f", universo->NomeItens[i], &universo->Peso[i], &universo->Beneficio[i]);
        universo->Razao[i] = universo->Beneficio[i] / universo->Peso[i];
    }

    fclose(arquivo);
}

void escreverArquivoSaida(const char* nomeArquivo, Universo* universo, const char Metodo) {
    float tempo_gasto;
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de saída");
        exit(EXIT_FAILURE);
    }

    float TotalBeneficio = 0;
    float TotalPeso = 0;
    for (int i = 0; i < universo->Cardinalidade; i++) {
        TotalBeneficio += universo->Beneficio[i];
        TotalPeso += universo->Peso[i];
    }

    fprintf(arquivo,     "Instancia : %s\n",
                         "N         : %d\n",
                         "K         : %.1f\n", nomeArquivo, universo->Cardinalidade, universo->Capacidade);
    printf( "Instancia : %s\n",
            "N         : %d\n",
            "K         : %.1f\n", nomeArquivo, universo->Cardinalidade, universo->Capacidade);
    if(Metodo == 'H'){
        fprintf(arquivo,"Metodo     : Heuristica\n");
        printf("Metodo     : Heuristica\n");
    }
    else if(Metodo == 'E'){
        fprintf(arquivo,"Metodo     : Exaustão\n");
        printf("Metodo     : Exaustão\n");
    }
    fim = clock();
    tempo_gasto = ((float)(fim - inicio)) / CLOCKS_PER_SEC; // Calcula o tempo em segundos
    fprintf(arquivo,    "Tempo      : %.3f segundos\n",
                        "Peso       : %.1f\n",
                        "Beneficio  : %.1f\n\n", tempo_gasto, TotalPeso, TotalBeneficio);
    printf( "Tempo      : %.3f segundos\n",
            "Peso       : %.1f\n",
            "Beneficio  : %.1f\n\n", tempo_gasto, TotalPeso, TotalBeneficio);


    for (int i = 0; i < universo->Cardinalidade; i++) {
        fprintf(arquivo, "%s (w = %.2f, c = %.2f)\n", universo->NomeItens[i], universo->Peso[i], universo->Beneficio[i]);
        printf("%s (w = %.2f, c = %.2f)\n", universo->NomeItens[i], universo->Peso[i], universo->Beneficio[i]);
    }
    printf("\nFim do Processamento!\n");

    fclose(arquivo);
}

int main(int argc, char **argv){
    inicio = clock();
    char *StrEntrada;
    char *StrSaida;
    char Metodo;

    if(argc != 4){
        printf("erro de sintaxe, insira todos os parâmetros necessários:\n");
        printf("Metodo:\n E - Solução Exaustiva\n H - Solução via Heurística construtiva\n");
        printf("Entrada:\n- Insira o arquivo, com seu caminho incluso, no qual serão extraidos as informações\n");
        printf("Saida:\n- Insira o arquivo, com seu caminho incluso, para o qual a saida será definida\n\n");
        printf("Dessa forma tente usar esse modelo: \n./solver-mochila.bin <metodo> <entrada> <saida>\n\n");
    }

    Metodo = argv[1][0];
    StrEntrada = argv[2];
    StrSaida = argv[3];

    Universo universo;
    lerArquivoEntrada(StrEntrada, &universo);

    switch (Metodo){
        case 'E':case 'e':
        resolverExaustivo();
        escreverArquivoSaida(StrSaida, &universo, Metodo);
        liberarMemoriaExaustivo();
        break;

        case 'H':case 'h':

        break;
    }

    escreverArquivoSaida(StrSaida, &universo, Metodo);

    exit(0);
}
