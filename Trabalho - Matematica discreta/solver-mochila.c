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

void escreverArquivoSaida(const char* displayArquivo, const char* nomeArquivo, Universo* universo, const char Metodo) {
    float tempo_gasto;
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de saída");
        exit(EXIT_FAILURE);
    }

    float TotalBeneficio = 0;
    float TotalPeso = 0;
    for (int i = 0; i < universo->Cardinalidade; i++) {
        if(universo->MapaDeBits[i] == 1){
            TotalBeneficio += universo->Beneficio[i];
            TotalPeso += universo->Peso[i];
        }
    }

    fprintf(arquivo,     "Instancia  : %s\n", displayArquivo);
    fprintf(arquivo,     "N          : %d\n", universo->Cardinalidade);
    fprintf(arquivo,     "K          : %.1f\n", universo->Capacidade);
    printf( "Instancia  : %s\n", nomeArquivo);
    printf( "N          : %d\n", universo->Cardinalidade);
    printf( "K          : %.1f\n", universo->Capacidade);
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
    fprintf(arquivo,    "Tempo      : %.3f segundos\n", tempo_gasto);
    fprintf(arquivo,    "Peso       : %.1f\n", TotalPeso);
    fprintf(arquivo,    "Beneficio  : %.1f\n\n", TotalBeneficio);
    printf( "Tempo      : %.3f segundos\n", tempo_gasto);
    printf( "Peso       : %.1f\n", TotalPeso);
    printf( "Beneficio  : %.1f\n\n", TotalBeneficio);

    int con = 0;
    for (int i = 0; i < universo->Cardinalidade; i++) {
        if(universo->MapaDeBits[i] == 1){
            fprintf(arquivo, "%s (w = %.2f, c = %.2f)\n", universo->NomeItens[i], universo->Peso[i], universo->Beneficio[i]);
            printf("%s (w = %.2f, c = %.2f)\n", universo->NomeItens[i], universo->Peso[i], universo->Beneficio[i]);
            con = 1;
        }
    }
    if(con == 0){
        printf("Nao ha itens que caibam no parametro da mochila!\n");
    }
    printf("\nFim do Processamento!\n");

    for (int i = 0; i < universo->Cardinalidade; i++) {
        free(universo->NomeItens[i]);
    }
    free(universo->NomeItens);
    free(universo->Peso);
    free(universo->Beneficio);
    free(universo->Razao);
    free(universo->MapaDeBits);

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
    
    float melhor_beneficio = 0;
    float melhor_peso = 0;
    int *melhor_comb = (int *)malloc(universo->Cardinalidade * sizeof(int));
    for (int i = 0; i < universo->Cardinalidade; i++) {
        melhor_comb[i] = -1;  
    }

    
    for (int i = 1; i < (1 << universo->Cardinalidade); i++) {
        float peso_total = 0, beneficio_total = 0;
        int *combinacao_atual = (int *)malloc(universo->Cardinalidade * sizeof(int));
        int num_itens_selecionados = 0;

        
        for (int j = 0; j < universo->Cardinalidade; j++) {
            if (i & (1 << j)) { 
                peso_total += universo->Peso[j]; 
                beneficio_total += universo->Beneficio[j];
                combinacao_atual[num_itens_selecionados++] = j; 
            }
        }
        

        //Se o peso total e a capacidade e o benefício bater ele muda
        if (peso_total <= universo->Capacidade && beneficio_total > melhor_beneficio) {
            melhor_beneficio = beneficio_total;
            melhor_peso = peso_total;
            //Caso troque, salva a combinação que gerou o melhor beneficio
            for (int j = 0; j < num_itens_selecionados; j++) {
                melhor_comb[j] = combinacao_atual[j];  //Salva o índice dos itens
            }
        }
        //Liberar memória alocada para a combinação atual
        free(combinacao_atual);
    }
    
    for(int j = 0; j<universo->Cardinalidade;j++){
        universo->MapaDeBits[j] = 0;
    }

    float ComparaCapacidade, auxCapacidade = universo->Capacidade;
    for (int i = 0; i < universo->Cardinalidade; i++) {        
        ComparaCapacidade = auxCapacidade - universo->Peso[melhor_comb[i]];
        
        if(ComparaCapacidade < 0){
            ComparaCapacidade = auxCapacidade;
            universo->MapaDeBits[melhor_comb[i]] = 0;
        }
        else{
            auxCapacidade = ComparaCapacidade;
            universo->MapaDeBits[melhor_comb[i]] = 1;
        }
    }
    //Liberar memória alocada para a melhor combinação
    free(melhor_comb);

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

    escreverArquivoSaida(StrEntrada, StrSaida, &universo, Metodo);

    exit(0);
}