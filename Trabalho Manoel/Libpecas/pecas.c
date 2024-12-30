#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pecas.h"

//Subrotina para o cálculo de preço da venda de peça
float calculo(float custo){
    float result = 0, lucropeca;
    FILE *lucro = fopen("arquivos/lucro.bin", "rb");
    if (lucro == NULL){
        printf("erro lerlucro");
        return 1;
    }

    fread(&lucropeca, sizeof(float), 1, lucro);

    result = (custo * (lucropeca/100)) + custo; //Cálculo provisório para custo de venda

    fclose(lucro); //Fecha arquivo do lucro

    return result;
}


//Subrotina para registro em binário
void regpecabin(dadopec peca){ 
    FILE *bin = fopen("arquivos/peca.bin", "ab");
    if(bin == NULL){
        printf("Erro ao abrir o arquivo! regbin");
        return;
    }

    fwrite(&peca, sizeof(dadopec), 1, bin);

    fclose(bin);
}
//Subrotina para registro em texto
void regpecatxt(dadopec peca){
    FILE *txt = fopen("arquivos/peca.txt", "a");
    if(txt == NULL){
        printf("erro ao abrir  arquivo! regtxt");
        return;
    }

    fprintf(txt,
    "%d\n%s\n%s\n%s\n%s\n%f\n%f\n%d\n%d\n",
    peca.codpec, peca.nomepec, peca.descpec, peca.fabpec, peca.fornpec, peca.custpeca, peca.vendpeca, peca.npeca, peca.minpeca);


    fclose(txt);
    printf("Peça registrada com sucesso!");
}
//Subrotina para encaminhar os dados das variáveis para as formas de registro disponíveis
void regpecapath(dadopec peca){
    int formatoReg;
    FILE *formatoArq = fopen("arquivos/formato.bin", "rb");
    if (formatoArq == NULL){
        printf("Erro na interpretação do formato do arquivo!");
        return;
    }
    fread(&formatoReg, sizeof(int), 1, formatoArq);

    //separa entre os formatos definidos no arquivo de formato
    if (formatoReg == 1){
        regpecabin(peca);
    }
    else{
        regpecatxt(peca);
    }

    fclose(formatoArq);
}
//Subrotina para registro das variáveis
void regpeca(){
    dadopec peca;
    
    printf("\nInsira o código da peça:");
    scanf("%d", &peca.codpec);
    getchar();
    printf("Insira o nome da peça:");
    scanf("%19[^\n]", peca.nomepec);
    getchar();
    printf("Insira uma descrição breve da peça:");
    scanf("%59[^\n]", peca.descpec);
    getchar();
    printf("Insira o Fabricante das peças:");
    scanf("%29[^\n]", peca.fabpec);
    getchar();
    printf("Insira o Fornecedor das peças:");
    scanf("%29[^\n]", peca.fornpec);
    getchar();
    printf("Insira o custo das peças em R$:");
    scanf("%f", &peca.custpeca);
    printf("Insira o número disponível em estoque da peça:");
    scanf("%d", &peca.npeca);
    printf("Insira o número mínimo que essa peça deve ter em estoque:");
    scanf("%d", &peca.minpeca);
    getchar();

//----------------------
    peca.vendpeca = calculo(peca.custpeca);
//----------------------

    regpecapath(peca);
}


//Subrotina para edição de peças em binário
void editpecabin(){
    int codBusca;
    printf("Digite o código da peça que deseja editar: ");
    scanf("%d", &codBusca);

    FILE *bin = fopen("arquivos/peca.bin", "rb");
    if (bin == NULL) {
        printf("Erro ao abrir o arquivo binário para leitura!\n");
        return;
    }

    FILE *temp = fopen("arquivos/temp.bin", "wb");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(bin);
        return;
    }

    dadopec peca;
    int encontrado = 0;


    //função que separa o arquivo localizado para edição
    while (fread(&peca, sizeof(dadopec), 1, bin)) { 
        if (peca.codpec == codBusca) {
            encontrado = 1;
                printf("\nInsira o novo código da peça:");
                scanf("%d", &peca.codpec);
                getchar();
                printf("Insira o novo nome da peça:");
                scanf("%19[^\n]", peca.nomepec);
                getchar();
                printf("Insira uma n descrição breve da peça:");
                scanf("%59[^\n]", peca.descpec);
                getchar();
                printf("Insira o novo Fabricante das peças:");
                scanf("%29[^\n]", peca.fabpec);
                getchar();
                printf("Insira o novo Fornecedor das peças:");
                scanf("%29[^\n]", peca.fornpec);
                getchar();
                printf("Insira o novo custo das peças em R$:");
                scanf("%f", &peca.custpeca);
                printf("Insira o novo número disponível em estoque da peça:");
                scanf("%d", &peca.npeca);
                printf("Insira o novo número mínimo que essa peça deve ter em estoque:");
                scanf("%d", &peca.minpeca);
                getchar();

            //----------------------
                peca.vendpeca = calculo(peca.custpeca);
            //----------------------

        }
        //Registra as variáveis modificadas ou não no arquivo temporário
        fwrite(&peca, sizeof(dadopec), 1, temp);
    }

    fclose(bin);
    fclose(temp);

    //processo de mudança do arquivo temporário para o principal
    if (encontrado) {
        remove("arquivos/peca.bin");
        rename("arquivos/temp.bin", "arquivos/peca.bin");
        printf("Peça editada com sucesso!\n");
    } else {
        remove("arquivos/temp.bin");
        printf("Peça com código %d não encontrada!\n", codBusca);
    }
}
//Subrotina para edição de peças em texto
void editpecatxt(){
    int codBusca;
    printf("Digite o código da peça que deseja editar: ");
    scanf("%d", &codBusca);

    FILE *txt = fopen("arquivos/peca.txt", "r");
    if (txt == NULL) {
        printf("Erro ao abrir o arquivo texto para leitura!\n");
        return;
    }

    FILE *temp = fopen("arquivos/temp.txt", "w");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(txt);
        return;
    }

    dadopec peca;
    int encontrado = 0;

    //função que separa o arquivo localizado para edição
    while (fscanf(txt, 
                "%d\n%19[^\n]\n%59[^\n]\n%29[^\n]\n%29[^\n]\n%f\n%f\n%d\n%d\n", 
                &peca.codpec, peca.nomepec, peca.descpec, peca.fabpec, peca.fornpec, 
                &peca.custpeca, &peca.vendpeca, &peca.npeca, &peca.minpeca) == 9) {
        if (peca.codpec == codBusca) {
            encontrado = 1;

            printf("\nInsira o novo código da peça:");
            scanf("%d", &peca.codpec);
            getchar();
            printf("Insira o novo nome da peça:");
            scanf("%19[^\n]", peca.nomepec);
            getchar();
            printf("Insira uma nova descrição breve da peça:");
            scanf("%59[^\n]", peca.descpec);
            getchar();
            printf("Insira o novo Fabricante das peças:");
            scanf("%29[^\n]", peca.fabpec);
            getchar();
            printf("Insira o novo Fornecedor das peças:");
            scanf("%29[^\n]", peca.fornpec);
            getchar();
            printf("Insira o novo custo das peças em R$:");
            scanf("%f", &peca.custpeca);
            printf("Insira o novo número disponível em estoque da peça:");
            scanf("%d", &peca.npeca);
            printf("Insira o novo número mínimo que essa peça deve ter em estoque:");
            scanf("%d", &peca.minpeca);
            getchar();

            //----------------------
            peca.vendpeca = calculo(peca.custpeca);
            //----------------------

            // Salvar peça atualizada no arquivo temporário
            fprintf(temp,
                    "%d\n%s\n%s\n%s\n%s\n%f\n%f\n%d\n%d\n",
                    peca.codpec, peca.nomepec, peca.descpec, peca.fabpec, peca.fornpec, 
                    peca.custpeca, peca.vendpeca, peca.npeca, peca.minpeca);
        } else {
            // Copiar peça não alterada para o arquivo temporário
            fprintf(temp,
                    "%d\n%s\n%s\n%s\n%s\n%f\n%f\n%d\n%d\n",
                    peca.codpec, peca.nomepec, peca.descpec, peca.fabpec, peca.fornpec, 
                    peca.custpeca, peca.vendpeca, peca.npeca, peca.minpeca);
        }
    }

    fclose(txt);
    fclose(temp);


    //processo de mudança do arquivo temporário para o principal
    if (encontrado) {
        remove("arquivos/peca.txt");
        rename("arquivos/temp.txt", "arquivos/peca.txt");
        printf("Peça editada com sucesso!\n");
    } else {
        remove("arquivos/temp.txt");
        printf("Peça com código %d não encontrada!\n", codBusca);
    }
}
//Subrotina para encaminhar o tipo de edição a ser realizada
void editpeca(){
    int formatoReg;
    FILE *formatoArq = fopen("arquivos/formato.bin", "rb");
    if (formatoArq == NULL){
        printf("Erro na interpretação do formato do arquivo!");
        return;
    }
    fread(&formatoReg, sizeof(int), 1, formatoArq);

    //separa entre os formatos definidos no arquivo de formato
    if (formatoReg == 1){
        editpecabin();
    }
    else{
        editpecatxt();
    }

    fclose(formatoArq);
}


//Subrotina para leitura dos dados em binário
void lerpecabin(){
    dadopec peca;
    FILE *bin = fopen("arquivos/peca.bin", "rb");
    if (bin == NULL){
        printf("erro lerbin");
        return;
    }
    
    //função de leitura do arquivo
    printf("===================================== Lista de Peças =====================================\n");
    while (fread(&peca, sizeof(dadopec), 1, bin)) {
        printf("\nCodigo: %d\nNome: %s\nDescrição: %s\nFabricante: %s\nFornecedor: %s \nCusto: %.2f \nVenda: %.2f \nUnidades Disponiveis: %d\nQuantidade Mínima: %d",
         peca.codpec, peca.nomepec, peca.descpec, peca.fabpec, peca.fornpec, peca.custpeca, peca.vendpeca, peca.npeca, peca.minpeca);
         printf("--------------------------------------------------------------------------------------\n");
    }
    
    fclose(bin);

}
//Subrotina para leitura dos dados em texto
void lerpecatxt(){
    dadopec peca;
    FILE *txt = fopen("arquivos/peca.txt", "r");
    if (txt == NULL) {
        printf("Erro ao abrir o arquivo texto!\n");
        return;
    }

    //função de leitura do arquivo
    printf("===================================== Lista de Peças =====================================\n");
    while (fscanf(txt, "%d\n%19[^\n]\n%59[^\n]\n%29[^\n]\n%29[^\n]\n%f\n%f\n%d\n%d\n", 
                   &peca.codpec, peca.nomepec, peca.descpec, peca.fabpec, peca.fornpec, 
                   &peca.custpeca, &peca.vendpeca, &peca.npeca, &peca.minpeca) == 9) {
        printf("Código da peça: %d\n", peca.codpec);
        printf("Nome da peça: %s\n", peca.nomepec);
        printf("Descrição: %s\n", peca.descpec);
        printf("Fabricante: %s\n", peca.fabpec);
        printf("Fornecedor: %s\n", peca.fornpec);
        printf("Custo: R$ %.2f\n", peca.custpeca);
        printf("Preço de venda: R$ %.2f\n", peca.vendpeca);
        printf("Estoque disponível: %d\n", peca.npeca);
        printf("Estoque mínimo: %d\n", peca.minpeca);
        printf("--------------------------------------------------------------------------------------\n");
    }

    fclose(txt);
}
//Subrotina para encaminhar o tipo de leitura a ser realizada
void lerpeca(){
    int formatoReg;
    FILE *formatoArq = fopen("arquivos/formato.bin", "rb");
    if (formatoArq == NULL){
        printf("Erro na interpretação do formato do arquivo!");
        return;
    }
    fread(&formatoReg, sizeof(int), 1, formatoArq);

    //separa entre os formatos definidos no arquivo de formato
    if (formatoReg == 1){
        lerpecabin();
    }
    else{
        lerpecatxt();
    }

    fclose(formatoArq);
}


//Subrotina para remover dados em binário
void removerpecatxt(){
    int codBusca;
    printf("Digite o código da peça que deseja remover: ");
    scanf("%d", &codBusca);

    FILE *txt = fopen("arquivos/peca.txt", "r");
    if (txt == NULL) {
        printf("Erro ao abrir o arquivo texto para leitura!\n");
        return;
    }

    FILE *temp = fopen("arquivos/temp.txt", "w");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(txt);
        return;
    }

    dadopec peca;
    int encontrado = 0;

    //Lê os dados a serem removidos
    while (fscanf(txt, 
                  "%d\n%19[^\n]\n%59[^\n]\n%29[^\n]\n%29[^\n]\n%f\n%f\n%d\n%d\n", 
                  &peca.codpec, peca.nomepec, peca.descpec, peca.fabpec, peca.fornpec, 
                  &peca.custpeca, &peca.vendpeca, &peca.npeca, &peca.minpeca) == 9) {
        if (peca.codpec == codBusca) {
            encontrado = 1;
            //Deixa o novo registro vazio
        } else {
            // Copiar peça não removida para o arquivo temporário
            fprintf(temp,
                    "%d\n%s\n%s\n%s\n%s\n%f\n%f\n%d\n%d\n",
                    peca.codpec, peca.nomepec, peca.descpec, peca.fabpec, peca.fornpec, 
                    peca.custpeca, peca.vendpeca, peca.npeca, peca.minpeca);
        }
    }

    fclose(txt);
    fclose(temp);

    if (encontrado) {
        remove("arquivos/peca.txt");
        rename("arquivos/temp.txt", "arquivos/peca.txt");
        printf("Peça removida com sucesso!\n");
    } else {
        remove("arquivos/temp.txt");
        printf("Peça com código %d não encontrada!\n", codBusca);
    }
}
//SUbrotina para remover dados em texto
void removerpecabin(){
    int codBusca;
    printf("Digite o código da peça que deseja remover: ");
    scanf("%d", &codBusca);

    FILE *bin = fopen("arquivos/peca.bin", "rb");
    if (bin == NULL) {
        printf("Erro ao abrir o arquivo binário para leitura!\n");
        return;
    }

    FILE *temp = fopen("arquivos/temp.bin", "wb");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(bin);
        return;
    }

    dadopec peca;
    int encontrado = 0;

    // Lendo os dados da peça do arquivo binário
    while (fread(&peca, sizeof(dadopec), 1, bin)) {
        if (peca.codpec == codBusca) {
            encontrado = 1;
            // Não escreve o registro no arquivo temporário, efetivamente removendo-o
        } else {
            // Copiar peça não removida para o arquivo temporário
            fwrite(&peca, sizeof(dadopec), 1, temp);
        }
    }

    fclose(bin);
    fclose(temp);

    if (encontrado) {
        remove("arquivos/peca.bin");
        rename("arquivos/temp.bin", "arquivos/peca.bin");
        printf("Peça removida com sucesso!\n");
    } else {
        remove("arquivos/temp.bin");
        printf("Peça com código %d não encontrada!\n", codBusca);
    }
}
//Subrotina para encaminhar o tipo de remoção a ser realizada
void removerpeca(){
    int formatoReg;
    FILE *formatoArq = fopen("arquivos/formato.bin", "rb");
    if (formatoArq == NULL){
        printf("Erro na interpretação do formato do arquivo!");
        return;
    }
    fread(&formatoReg, sizeof(int), 1, formatoArq);

    if (formatoReg == 1){
        removerpecabin();
    }
    else{
        removerpecatxt();
    }

    fclose(formatoArq);
}