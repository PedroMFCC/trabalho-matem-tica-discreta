#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servicos.h"

// Função para registrar funcionário em formato binário
void regservicobin(Servico servico) {
    FILE *arquivo = fopen("arquivos/servicos.bin", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para registro binário.\n");
        return;
    }

    fwrite(&servico, sizeof(Servico), 1, arquivo);
    fclose(arquivo);
        printf("Serviço registrado com sucesso!\n================================================================================\n");
}
// Função para registrar funcionário em formato texto
void regservicotxt(Servico servico) {

    FILE *arquivo = fopen("arquivos/servicos.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para registro em texto.\n");
        return;
    }
   
    fprintf(arquivo, "%d %s %.2f %.2f\n", servico.codigo, servico.descricao, servico.preco, servico.comissao);
    
    fclose(arquivo);
    printf("Serviço registrado com sucesso em formato texto!\n");
}
// Função de registro (escolhe o formato)
void regservico() {
    int formato;
    Servico servico;
    FILE *arq = fopen("arquivos/formato.bin", "rb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de formato. Configure o formato primeiro.\n");
        return;
    }
    fread(&formato, sizeof(int), 1, arq);
    fclose(arq);

    
    printf("Digite o código do serviço: ");
    scanf("%d", &servico.codigo);
    printf("Digite a descrição do serviço: ");
    scanf(" %99[^\n]", servico.descricao);
    printf("Digite o preço do serviço: ");
    scanf("%f", &servico.preco);
    printf("Digite a comissão do serviço: ");
    scanf("%f", &servico.comissao);

    if (formato == 1) {
        regservicobin(servico);
    } else if (formato == 2) {
        regservicotxt(servico);
    } else {
        printf("Formato desconhecido.\n");
    }
}
// Função para editar veículo no formato binário
void editservicobin() {
    int codigo, encontrado = 0;
    Servico servico;
    FILE *arquivo = fopen("arquivos/servicos.bin", "rb");
    FILE *temp = fopen("arquivos/temp_servicos.bin", "wb");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos para edição binária.\n");
        if (arquivo) fclose(arquivo);
        if (temp) fclose(temp);
        return;
    }

    printf("Digite o código do serviço a ser editado: ");
    scanf("%d", &codigo);

    //Compara o valor do arquivo para edição
    while (fread(&servico, sizeof(Servico), 1, arquivo)) {
        if (servico.codigo == codigo) {
            encontrado = 1;
            printf("Serviço encontrado! Digite os novos dados.\n");
            printf("Descrição: ");
            scanf(" %99[^\n]", servico.descricao);
            printf("Preço: ");
            scanf("%f", &servico.preco);
            printf("Comissão: ");
            scanf("%f", &servico.comissao);
        }
        // Escreve no arquivo temporário o registro (seja editado ou original)
        fwrite(&servico, sizeof(Servico), 1, temp);
    }

    fclose(arquivo);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    remove("arquivos/servicos.bin");
    rename("arquivos/temp_servicos.bin", "arquivos/servicos.bin");

    if (encontrado) {
        printf("Serviço editado com sucesso!\n");
    } else {
        printf("Serviço com código %d não encontrado.\n", codigo);
    }
}
// Função para editar veículo no formato texto
void editservicotxt() {
    int codigo, encontrado = 0;
    char confirmacao;
    Servico servico;

    printf("Digite o código do serviço a ser editado: ");
    scanf("%d", &codigo);

    FILE *arquivo = fopen("arquivos/servicos.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura em texto!\n");
        return;
    }

    FILE *temp = fopen("arquivos/temp.txt", "w");
    if (temp == NULL) {
        printf("Erro ao abrir o arquivo temporário para escrita em texto!\n");
        fclose(arquivo);
        return;
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0; 

        
        char descricao[100];
        if (sscanf(linha, "%d %99[^0-9] %f %f", &servico.codigo, descricao, &servico.preco, &servico.comissao) == 4) {
            
            for (int i = strlen(descricao) - 1; i >= 0 && descricao[i] == ' '; i--) {
                descricao[i] = '\0';
            }

            if (servico.codigo == codigo) {
                encontrado = 1;
                printf("Serviço encontrado! Digite os novos dados.\n");
                printf("Descrição: ");
                scanf(" %99[^\n]", descricao);
                printf("Preço: ");
                scanf("%f", &servico.preco);
                printf("Comissão: ");
                scanf("%f", &servico.comissao);

                fprintf(temp, "%d %s %.2f %.2f\n", servico.codigo, descricao, servico.preco, servico.comissao);
            } else {
                fprintf(temp, "%s\n", linha);
            }
        } else {
            printf("Erro ao processar linha: '%s'\n", linha);
            fprintf(temp, "%s\n", linha);
        }
    }

    fclose(arquivo);
    fclose(temp);
    remove("arquivos/servicos.txt");
    rename("arquivos/temp.txt", "arquivos/servicos.txt");

    if (encontrado) {
        printf("Serviço editado com sucesso!\n");
    } else {
        printf("Serviço com código %d não encontrado.\n", codigo);
    }
}
//Função de edição (escolhe o formato)
void editservico() {
   int formatoReg;
    FILE *formatoArq = fopen("arquivos/formato.bin", "rb");
    if (formatoArq == NULL){
        printf("Erro na interpretação do formato do arquivo!");
        return;
    }
    fread(&formatoReg, sizeof(int), 1, formatoArq);

    if (formatoReg == 1){
        editservicobin();
    }
    else{
        editservicotxt();
    }

    fclose(formatoArq);
}
// Função para leitura de veículos no formato binário
void lerservicobin() {
    Servico servico;
    FILE *bin = fopen("arquivos/servicos.bin", "rb");
    if (bin == NULL) {
        printf("Erro ao abrir o arquivo binário.\n");
        return;
    }

    printf("================================== Lista de Serviços ==================================\n");
    while (fread(&servico, sizeof(Servico), 1, bin)) {
    
        printf("Código: %d\nDescrição: %s\nPreço: %.2f\nComissão: %.2f\n", 
               servico.codigo, servico.descricao, servico.preco, servico.comissao);
        printf("--------------------------------------------------------------------------------------\n");
    }
    
    fclose(bin);
}
// Função para leitura de veículos no formato texto
void lerservicotxt() {
    Servico servico;
    FILE *arquivo = fopen("arquivos/servicos.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura em texto!\n");
        return;
    }

    char linha[256];  
    printf("================================== Lista de Serviços ==================================\n");

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0;

        
        char descricao[100];
        if (sscanf(linha, "%d %99[^0-9] %f %f", &servico.codigo, descricao, &servico.preco, &servico.comissao) == 4) {
            
            for (int i = strlen(descricao) - 1; i >= 0 && descricao[i] == ' '; i--) {
                descricao[i] = '\0';
            }

            // Exibe os dados no formato desejado
            printf("Código: %d\nDescrição: %s\nPreço: %.2f\nComissão: %.2f\n",
                   servico.codigo, descricao, servico.preco, servico.comissao);
            strcpy(servico.descricao, descricao); 
        } else {
            printf("Erro ao processar linha: '%s'\n", linha);
        }
        printf("--------------------------------------------------------------------------------------\n");
    }

    fclose(arquivo);
}
// Função de leitura (escolhe o formato)
void lerservico() {
    int formatoReg;
    FILE *formatoArq = fopen("arquivos/formato.bin", "rb");
    if (formatoArq == NULL) {
        printf("Erro na interpretação do formato do arquivo!\n");
        return;
    }
    fread(&formatoReg, sizeof(int), 1, formatoArq);

    // Dependendo do formato, chama a função de leitura apropriada
    if (formatoReg == 1) {
        lerservicobin();  // Leitura em binário
    } else {
        lerservicotxt();  // Leitura em texto
    }

    fclose(formatoArq);
}
//Função de exclusão, exclui em binário ou txt
void excluirServico() {
    int codigo, encontrado = 0;
    char confirmacao;
    int formato;

    // Lê o formato do registro
    FILE *formatoFile = fopen("arquivos/formato.bin", "rb");
    if (formatoFile == NULL) {
        printf("Erro ao abrir o arquivo de formato. Certifique-se de que ele existe!\n");
        return;
    }
    fread(&formato, sizeof(int), 1, formatoFile);
    fclose(formatoFile);

    printf("Digite o código do serviço a ser excluído: ");
    scanf("%d", &codigo);

    if (formato == 1) {
        
        FILE *arquivo = fopen("arquivos/servicos.bin", "rb");
        FILE *temp = fopen("arquivos/temp.bin", "wb");

        if (arquivo == NULL || temp == NULL) {
            printf("Erro ao abrir os arquivos necessários.\n");
            if (arquivo) fclose(arquivo);
            if (temp) fclose(temp);
            return;
        }

        Servico servico;

        while (fread(&servico, sizeof(Servico), 1, arquivo)) {
            if (servico.codigo == codigo) {
                encontrado = 1;
                printf("Serviço com código %d encontrado. Deseja realmente excluí-lo? (S/N): ", codigo);
                scanf(" %c", &confirmacao);
                getchar(); 

                if (confirmacao == 'S' || confirmacao == 's') {
                    printf("Serviço excluído com sucesso!\n");
                    continue; 
                } else {
                    printf("Operação cancelada.\n");
                    fwrite(&servico, sizeof(Servico), 1, temp); 
                }
            } else {
                fwrite(&servico, sizeof(Servico), 1, temp); 
            }
        }

        fclose(arquivo);
        fclose(temp);
        remove("arquivos/servicos.bin");
        rename("arquivos/temp.bin", "arquivos/servicos.bin");

    } else if (formato == 2) {
        // Exclusão em arquivo texto
        FILE *arquivo = fopen("arquivos/servicos.txt", "r");
        FILE *temp = fopen("arquivos/temp.txt", "w");

        if (arquivo == NULL || temp == NULL) {
            printf("Erro ao abrir os arquivos necessários.\n");
            if (arquivo) fclose(arquivo);
            if (temp) fclose(temp);
            return;
        }

        char linha[1024];
        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
            // Lê os dados da linha
            Servico servico;
            sscanf(linha, "%d %99s %f %f", &servico.codigo, servico.descricao, &servico.preco, &servico.comissao);

            if (servico.codigo == codigo) {
                encontrado = 1;
                printf("Serviço com código %d encontrado. Deseja realmente excluí-lo? (S/N): ", codigo);
                scanf(" %c", &confirmacao);
                getchar(); 
                if (confirmacao == 'S' || confirmacao == 's') {
                    printf("Serviço excluído com sucesso!\n");
                    continue; 
                } else {
                    printf("Operação cancelada.\n");
                    fprintf(temp, "%s", linha); 
                }
            } else {
                fprintf(temp, "%s", linha); 
            }
        }

        fclose(arquivo);
        fclose(temp);
        remove("arquivos/servicos.txt");
        rename("arquivos/temp.txt", "arquivos/servicos.txt");

    } else {
        printf("Formato desconhecido. Não foi possível realizar a exclusão.\n");
    }

    if (!encontrado) {
        printf("Serviço com código %d não encontrado.\n", codigo);
    }
}