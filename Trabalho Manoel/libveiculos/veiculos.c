#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "veiculos.h"

// Função para registrar veículo em formato binário
void regveiculobin(Veiculo veiculo) {
    
    FILE *arquivo = fopen("arquivos/veiculos.bin", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para registro binário.\n");
        return;
    }


    fwrite(&veiculo, sizeof(Veiculo), 1, arquivo);
    fclose(arquivo);
    printf("Veículo registrado com sucesso em formato binário!\n");
}
// Função para registrar veículo em formato texto
void regveiculotxt(Veiculo veiculo) {

    FILE *arquivo = fopen("arquivos/veiculos.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para registro em texto.\n");
        return;
    }


    fprintf(arquivo, "%s %s %s %d %s %s\n",
            veiculo.placa, veiculo.modelo, veiculo.marca,
            veiculo.anoFabricacao, veiculo.chassi, veiculo.proprietario);

    fclose(arquivo);
    printf("Veiculos registrado com sucesso!");
}
// Função de registro (escolhe o formato)
void regveiculo() {
    Veiculo veiculo;
    int formato;
    FILE *arq = fopen("arquivos/formato.bin", "rb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de formato. Configure o formato primeiro.\n");
        return;
    }
    fread(&formato, sizeof(int), 1, arq);
    fclose(arq);

    printf("Digite a placa do veículo: ");
    scanf(" %9s", veiculo.placa);
    printf("Digite o modelo do veículo: ");
    scanf(" %49[^\n]", veiculo.modelo);
    printf("Digite a marca do veículo: ");
    scanf(" %49[^\n]", veiculo.marca);
    printf("Digite o ano de fabricação do veículo: ");
    scanf("%d", &veiculo.anoFabricacao);
    printf("Digite o chassi do veículo: ");
    scanf(" %24[^\n]", veiculo.chassi);
    printf("Digite o nome do proprietário: ");
    scanf(" %99[^\n]", veiculo.proprietario);

    if (formato == 1) {
        regveiculobin(veiculo);
    } else if (formato == 2) {
        regveiculotxt(veiculo);
    } else {
        printf("Formato desconhecido.\n");
    }
}


// Função para editar veículo no formato binário
void editveiculobin() {
    char placa[10];
    int encontrado = 0;
    Veiculo veiculo;
    FILE *arquivo = fopen("arquivos/veiculos.bin", "rb");
    FILE *temp = fopen("arquivos/temp_veiculos.bin", "wb");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos para edição binária.\n");
        if (arquivo) fclose(arquivo);
        if (temp) fclose(temp);
        return;
    }

    printf("Digite a placa do veículo a ser editado: ");
    scanf(" %9s", placa);

    //Compara o valor do arquivo para edição
    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo)) {
        if (strcmp(veiculo.placa, placa) == 0) {
            encontrado = 1;
            printf("Veículo encontrado! Digite os novos dados.\n");
            printf("Modelo: ");
            scanf(" %49[^\n]", veiculo.modelo);
            printf("Marca: ");
            scanf(" %49[^\n]", veiculo.marca);
            printf("Ano de Fabricação: ");
            scanf("%d", &veiculo.anoFabricacao);
            printf("Chassi: ");
            scanf(" %24[^\n]", veiculo.chassi);
            printf("Proprietário: ");
            scanf(" %99[^\n]", veiculo.proprietario);
        }
        // Escreve no arquivo temporário o registro (seja ele editado ou original)
        fwrite(&veiculo, sizeof(Veiculo), 1, temp);
    }

    fclose(arquivo);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    remove("arquivos/veiculos.bin");
    rename("arquivos/temp_veiculos.bin", "arquivos/veiculos.bin");

    if (encontrado) {
        printf("Veículo editado com sucesso!\n");
    } else {
        printf("Veículo com placa %s não encontrado.\n", placa);
    }
}
// Função para editar veículo no formato texto
void editveiculotxt() {
    char placa[10];
    int encontrado = 0;
    Veiculo veiculo;
    FILE *arquivo = fopen("arquivos/veiculos.txt", "r");
    FILE *temp = fopen("arquivos/temp.txt", "w");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos para edição em texto.\n");
        return;
    }

    printf("Digite a placa do veículo a ser editado: ");
    scanf(" %9s", placa);

    //Compara o valor do arquivo para edição
    while (fscanf(arquivo, "%s %s %s %d %s %[^\n]",
                  veiculo.placa, veiculo.modelo, veiculo.marca,
                  &veiculo.anoFabricacao, veiculo.chassi, veiculo.proprietario) != EOF) {
        if (strcmp(veiculo.placa, placa) == 0) {
            encontrado = 1;
            printf("Veículo encontrado! Digite os novos dados.\n");
            printf("Modelo: ");
            scanf(" %49[^\n]", veiculo.modelo);
            printf("Marca: ");
            scanf(" %49[^\n]", veiculo.marca);
            printf("Ano de Fabricação: ");
            scanf("%d", &veiculo.anoFabricacao);
            printf("Chassi: ");
            scanf(" %24[^\n]", veiculo.chassi);
            printf("Proprietário: ");
            scanf(" %99[^\n]", veiculo.proprietario);
        }
        // Escreve no arquivo temporário (seja o registro atualizado ou o original)
        fprintf(temp, "%s %s %s %d %s %s\n",
                veiculo.placa, veiculo.modelo, veiculo.marca,
                veiculo.anoFabricacao, veiculo.chassi, veiculo.proprietario);
    }

    fclose(arquivo);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    remove("arquivos/veiculos.txt");
    rename("arquivos/temp.txt", "arquivos/veiculos.txt");

    if (encontrado) {
        printf("Veículo editado com sucesso!\n");
    } else {
        printf("Veículo com placa %s não encontrado.\n", placa);
    }
}
//Função de edição (escolhe o formato)
void editveiculo() {
    int formatoReg;
    FILE *formatoArq = fopen("arquivos/formato.bin", "rb");
    if (formatoArq == NULL){
        printf("Erro na interpretação do formato do arquivo!");
        return;
    }
    fread(&formatoReg, sizeof(int), 1, formatoArq);

    if (formatoReg == 1){
        editveiculobin();
    }
    else{
        editveiculotxt();
    }

    fclose(formatoArq);
}


// Função para leitura de veículos no formato binário
void lerveiculobin() {
    Veiculo veiculo;
    FILE *arquivo = fopen("arquivos/veiculos.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura binária.\n");
        return;
    }

    printf("================================== Lista de Veiculos ==================================\n");
    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo)) {
        printf("Placa: %s, Modelo: %s, Marca: %s, Ano: %d, Chassi: %s, Proprietário: %s\n",
               veiculo.placa, veiculo.modelo, veiculo.marca, veiculo.anoFabricacao, veiculo.chassi, veiculo.proprietario);
        printf("--------------------------------------------------------------------------------------\n");
    }
    fclose(arquivo);
}
// Função para leitura de veículos no formato texto
void lerveiculotxt() {
    Veiculo veiculo;
    FILE *arquivo = fopen("arquivos/veiculos.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura em texto.\n");
        return;
    }
    printf("================================== Lista de Veiculos ==================================\n");
    while (fscanf(arquivo, "%s %s %s %d %s %[^\n]",
                  veiculo.placa, veiculo.modelo, veiculo.marca,
                  &veiculo.anoFabricacao, veiculo.chassi, veiculo.proprietario) != EOF) {
        printf("Placa: %s\nModelo: %s\nMarca: %s\nAno: %d\nChassi: %s\nProprietário: %s\n",
               veiculo.placa, veiculo.modelo, veiculo.marca, veiculo.anoFabricacao, veiculo.chassi, veiculo.proprietario);
        printf("--------------------------------------------------------------------------------------\n");
    }
    
    fclose(arquivo);
}
// Função de leitura (escolhe o formato)
void lerveiculo() {
    int formato;
    FILE *arq = fopen("arquivos/formato.bin", "rb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de formato. Configure o formato primeiro.\n");
        return;
    }
    fread(&formato, sizeof(int), 1, arq);
    fclose(arq);

    if (formato == 1) {
        lerveiculobin();
    } else if (formato == 2) {
        lerveiculotxt();
    } else {
        printf("Formato desconhecido.\n");
    }
}


//Função de exclusão, exclui em binário ou txt
void excluirVeiculo() {
    char placa[10];
    char confirmacao;
    int encontrado = 0;
    int formato;

    // Lê o formato do registro
    FILE *formatoFile = fopen("arquivos/formato.bin", "rb");
    if (formatoFile == NULL) {
        printf("Erro ao abrir o arquivo de formato. Certifique-se de que ele existe!\n");
        return;
    }
    fread(&formato, sizeof(int), 1, formatoFile);
    fclose(formatoFile);

    printf("Digite a placa do veículo a ser excluído: ");
    scanf(" %9s", placa);

    if (formato == 1) {
        // Exclusão em arquivo binário
        FILE *arquivo = fopen("arquivos/veiculos.bin", "rb");
        FILE *temp = fopen("arquivos/temp.bin", "wb");

        if (arquivo == NULL || temp == NULL) {
            printf("Erro ao abrir os arquivos necessários.\n");
            if (arquivo) fclose(arquivo);
            if (temp) fclose(temp);
            return;
        }

        Veiculo veiculo;

        while (fread(&veiculo, sizeof(Veiculo), 1, arquivo)) {
            if (strcmp(veiculo.placa, placa) == 0) {
                encontrado = 1;
                printf("Veículo com placa %s encontrado. Deseja realmente excluí-lo? (S/N): ", placa);
                scanf(" %c", &confirmacao);

                if (confirmacao == 'S' || confirmacao == 's') {
                    printf("Veículo excluído com sucesso!\n");
                    continue;
                } else {
                    printf("Operação cancelada.\n");
                }
            }
            fwrite(&veiculo, sizeof(Veiculo), 1, temp); // Escreve no arquivo temporário
        }

        fclose(arquivo);
        fclose(temp);
        remove("arquivos/veiculos.bin");
        rename("arquivos/temp.bin", "arquivos/veiculos.bin");

    } else if (formato == 2) {
        // Exclusão em arquivo texto
        FILE *arquivo = fopen("arquivos/veiculos.txt", "r");
        FILE *temp = fopen("arquivos/temp.txt", "w");

        if (arquivo == NULL || temp == NULL) {
            printf("Erro ao abrir os arquivos necessários.\n");
            if (arquivo) fclose(arquivo);
            if (temp) fclose(temp);
            return;
        }

        Veiculo veiculo;

        while (fscanf(arquivo, "%9s %49[^\n] %49[^\n] %d %19[^\n] %99[^\n]", 
                      veiculo.placa, veiculo.modelo, veiculo.marca, 
                      &veiculo.anoFabricacao, veiculo.chassi, veiculo.proprietario) != EOF) {
            if (strcmp(veiculo.placa, placa) == 0) {
                encontrado = 1;
                printf("Veículo com placa %s encontrado. Deseja realmente excluí-lo? (S/N): ", placa);
                scanf(" %c", &confirmacao);

                if (confirmacao == 'S' || confirmacao == 's') {
                    printf("Veículo excluído com sucesso!\n");
                    continue;
                } else {
                    printf("Operação cancelada.\n");
                }
            }
            fprintf(temp, "%s %s %s %d %s %s\n", 
                    veiculo.placa, veiculo.modelo, veiculo.marca, 
                    veiculo.anoFabricacao, veiculo.chassi, veiculo.proprietario);
        }

        fclose(arquivo);
        fclose(temp);
        remove("arquivos/veiculos.txt");
        rename("arquivos/temp.txt", "arquivos/veiculos.txt");
    } else {
        printf("Formato desconhecido. Não foi possível realizar a exclusão.\n");
    }

    if (!encontrado) {
        printf("Veículo com placa %s não encontrado.\n", placa);
    }
}

