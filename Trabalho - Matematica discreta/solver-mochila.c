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

    int *MapaDeBits;
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
    universo->MapaDeBits = malloc(numItens * sizeof(int));

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
        if(universo->MapaDeBits == 1){
            TotalBeneficio += universo->Beneficio[i];
            TotalPeso += universo->Peso[i];
        }
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

    int con = 0;
    for (int i = 0; i < universo->Cardinalidade; i++) {
        if(universo->MapaDeBits == 1){
            fprintf(arquivo, "%s (w = %.2f, c = %.2f)\n", universo->NomeItens[i], universo->Peso[i], universo->Beneficio[i]);
            printf("%s (w = %.2f, c = %.2f)\n", universo->NomeItens[i], universo->Peso[i], universo->Beneficio[i]);
            con = 1;
        }
    }
    if(con == 0){
        printf("Nao ha itens que caim no parametro da mochila!\n");
    }
    printf("\nFim do Processamento!\n");

    fclose(arquivo);
}


void Heuristica(Universo* universo){    

    int* indice = malloc(universo->Cardinalidade * sizeof(int));

    for(int j = 0; j<universo->Cardinalidade;j++){
        universo->MapaDeBits[j] = 0;
        universo->Razao[j] = universo->Beneficio[j]/universo->Peso[j];
        indice[j] = j;
    }
    int pos_maior;
    for(int i = 0; i<universo->Cardinalidade; i++){
        pos_maior = i;
        for(int j = i + 1;j < universo->Cardinalidade;j++){ 
            if(universo->Razao[indice[j]] > universo->Razao[indice[pos_maior]]){
                pos_maior = j;
            }
        }
        int aux = indice[i];
        indice[i] = indice[pos_maior];
        indice[pos_maior] = aux;
    }
    float ComparaCapacidade, auxCapacidade = universo->Capacidade;
    for (int i = 0; i < universo->Cardinalidade; i++) {        
        ComparaCapacidade = auxCapacidade - universo->Peso[indice[i]];
        
        if(ComparaCapacidade < 0){
            ComparaCapacidade = auxCapacidade;
            universo->MapaDeBits[indice[i]] = 0;
        }
        else{
            auxCapacidade = ComparaCapacidade;
            universo->MapaDeBits[indice[i]] = 1;
        }
    }
    free(indice);
}
void Exaustiva(Universo* universo){

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
            Exaustiva(&universo);
        break;

        case 'H':case 'h':
            Heuristica(&universo);
        break;
    }

    escreverArquivoSaida(StrSaida, &universo, Metodo);

    exit(0);
}