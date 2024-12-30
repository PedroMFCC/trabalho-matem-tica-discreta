#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionarios.h"

// Função para registrar funcionário em formato binário
void regfuncionariosbin(Funcionario funcionario) {
    //abre o arquivo referente
    FILE *arquivo = fopen("arquivos/funcionarios.bin", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para registro binário.\n");
        return;
    }
    

    fwrite(&funcionario, sizeof(Funcionario), 1, arquivo);
    fclose(arquivo);
    printf("Funcionário registrado com sucesso em formato binário!\n");
}
// Função para registrar funcionário em formato texto
void regfuncionariosTXT(Funcionario funcionario) {
    //abre o arquivo referente
    FILE *arquivo = fopen("arquivos/funcionarios.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para registro em texto.\n");
        return;
    }

    fprintf(arquivo, "%s,%s,%s,%.2f\n", funcionario.nome, funcionario.cpf, funcionario.cargo, funcionario.salario);

    fclose(arquivo);
    printf("Funcionário registrado com sucesso em formato texto!");
}
// Função de registro (escolhe o formato)
void regfuncionario() {
    int formato;
    Funcionario funcionario;
    FILE *arq = fopen("arquivos/formato.bin", "rb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de formato. Configure o formato primeiro.\n");
        return;
    }
    fread(&formato, sizeof(int), 1, arq);
    fclose(arq);

    printf("Digite o nome do funcionário: ");
    scanf(" %99[^\n]", funcionario.nome);
    printf("Digite o CPF do funcionário: ");
    scanf(" %14s", funcionario.cpf);
    printf("Digite o cargo do funcionário: ");
    scanf(" %49[^\n]", funcionario.cargo);
    printf("Digite o salário do funcionário: ");
    scanf("%f", &funcionario.salario);

    if (formato == 1) {
        regfuncionariosbin(funcionario);
    } else if (formato == 2) {
        regfuncionariosTXT(funcionario);
    } else {
        printf("Formato desconhecido.\n");
    }
}


// Função para editar funcionário no formato binário
void editfuncionariosbin() {
    char cpf[15];
    int encontrado = 0;
    Funcionario funcionario;
    //abre os arquivos referentes
    FILE *arquivo = fopen("arquivos/funcionarios.bin", "rb");
    FILE *temp = fopen("arquivos/temp_funcionarios.bin", "wb");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos para edição binária.\n");
        if (arquivo) fclose(arquivo);
        if (temp) fclose(temp);
        return;
    }

    printf("Digite o CPF do funcionário a ser editado: ");
    scanf(" %14s", cpf);

    //Compara o valor do arquivo para edição
    while (fread(&funcionario, sizeof(Funcionario), 1, arquivo)) {
        if (strcmp(funcionario.cpf, cpf) == 0) {
            encontrado = 1;
            printf("Funcionário encontrado! Digite os novos dados.\n");
            printf("Nome: ");
            scanf(" %99[^\n]", funcionario.nome);
            printf("Cargo: ");
            scanf(" %49[^\n]", funcionario.cargo);
            printf("Salário: ");
            scanf("%f", &funcionario.salario);
        }
        // Escreve no arquivo temporário (seja o registro atualizado ou o original)
        fwrite(&funcionario, sizeof(Funcionario), 1, temp);
    }

    fclose(arquivo);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    remove("arquivos/funcionarios.bin");
    rename("arquivos/temp_funcionarios.bin", "arquivos/funcionarios.bin");

    if (encontrado) {
        printf("Funcionário editado com sucesso!\n");
    } else {
        printf("Funcionário com CPF %s não encontrado.\n", cpf);
    }
}
// Função para editar funcionário no formato texto
void editfuncionariosTXT() {
    char cpf[15];
    int encontrado = 0;
    Funcionario funcionario;
    char linha[200]; 

    FILE *arquivo = fopen("arquivos/funcionarios.txt", "r");
    FILE *temp = fopen("arquivos/temp.txt", "w");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos para edição em texto.\n");
        return;
    }

    printf("Digite o CPF do funcionário a ser editado: ");
    scanf(" %14s", cpf);

    // Compara o valor do arquivo para edição
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        
        if (sscanf(linha, " %99[^,], %14[^,], %49[^,], %f", funcionario.nome, funcionario.cpf, funcionario.cargo, &funcionario.salario) == 4) {
            if (strcmp(funcionario.cpf, cpf) == 0) {
                encontrado = 1;
                printf("Funcionário encontrado! Digite os novos dados.\n");
                printf("Nome: ");
                scanf(" %99[^\n]", funcionario.nome);
                printf("Cargo: ");
                scanf(" %49[^\n]", funcionario.cargo);
                printf("Salário: ");
                scanf("%f", &funcionario.salario);
            }
            // Escreve no arquivo temporário (seja o registro atualizado ou o original)
            fprintf(temp, "%s, %s, %s, %.2f\n", funcionario.nome, funcionario.cpf, funcionario.cargo, funcionario.salario);
        }
    }

    fclose(arquivo);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    remove("arquivos/funcionarios.txt");
    rename("arquivos/temp.txt", "arquivos/funcionarios.txt");

    if (encontrado) {
        printf("Funcionário editado com sucesso!\n");
    } else {
        printf("Funcionário com CPF %s não encontrado.\n", cpf);
    }
}

//Função de edição (escolhe o formato)
void editfuncionario() {
    int formatoReg;
    //abre o arquivo referente
    FILE *formatoArq = fopen("arquivos/formato.bin", "rb");
    if (formatoArq == NULL){
        printf("Erro na interpretação do formato do arquivo!");
        return;
    }
    fread(&formatoReg, sizeof(int), 1, formatoArq);

    if (formatoReg == 1){
        editfuncionariosbin();
    }
    else{
        editfuncionariosTXT();
    }

    fclose(formatoArq);

}


// Função para leitura de funcionários no formato binário
void lerfuncionariosbin() {
    Funcionario funcionario;
    //abre o arquivo referente
    FILE *arquivo = fopen("arquivos/funcionarios.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura binária.\n");
        return;
    }
    printf("========================== Lista de Funcionarios ==========================\n");
    while (fread(&funcionario, sizeof(Funcionario), 1, arquivo)) {

        printf("Nome: %s\nCPF: %s\nCargo: %s\nSalário: %.2f\n",
               funcionario.nome, funcionario.cpf, funcionario.cargo, funcionario.salario);
        printf("--------------------------------------------------------------------------------------\n");
    }
    
    fclose(arquivo);
}

// Função para leitura de funcionários no formato texto
void lerfuncionariosTXT() {
    Funcionario funcionario;
    char linha[200]; 

    // abre o arquivo referente
    FILE *arquivo = fopen("arquivos/funcionarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura em texto.\n");
        return;
    }

    printf("========================== Lista de Funcionarios ==========================\n");
    while (fgets(linha, sizeof(linha), arquivo)) {
        
        (sscanf(linha, " %99[^,], %14[^,], %49[^,], %f", funcionario.nome, funcionario.cpf, funcionario.cargo, &funcionario.salario) == 4);
            printf("Nome: %s\nCPF: %s\nCargo: %s\nSalário: %.2f\n", 
                   funcionario.nome, funcionario.cpf, funcionario.cargo, funcionario.salario);
            printf("--------------------------------------------------------------------------------------\n");
    }

    fclose(arquivo);
}


// Função de leitura (escolhe o formato)
void lerfuncionario() {
    int formato;
    //abre o arquivo referente
    FILE *arq = fopen("arquivos/formato.bin", "rb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de formato. Configure o formato primeiro.\n");
        return;
    }
    fread(&formato, sizeof(int), 1, arq);
    fclose(arq);

    if (formato == 1) {
        lerfuncionariosbin();
    } else if (formato == 2) {
        lerfuncionariosTXT();
    } else {
        printf("Formato desconhecido.\n");
    }
}
// Função para excluir um funcionário
void excluirFuncionario() {
    char cpf[15];
    char confirmacao;
    int encontrado = 0;
    int formato;

    // Abre o arquivo de formato para determinar se é binário ou texto
    FILE *formatoFile = fopen("arquivos/formato.bin", "rb");
    if (formatoFile == NULL) {
        printf("Erro ao abrir o arquivo de formato. Certifique-se de que ele existe!\n");
        return;
    }
    fread(&formato, sizeof(int), 1, formatoFile);
    fclose(formatoFile);

    printf("Digite o CPF do funcionário a ser excluído: ");
    scanf(" %14s", cpf);

    if (formato == 1) {
        // Binário
        FILE *arquivo = fopen("arquivos/funcionarios.bin", "rb");
        FILE *temp = fopen("arquivos/temp.bin", "wb");

        if (arquivo == NULL || temp == NULL) {
            printf("Erro ao abrir os arquivos necessários.\n");
            if (arquivo) fclose(arquivo);
            if (temp) fclose(temp);
            return;
        }

        Funcionario funcionario;
        int registrosMantidos = 0;

        // Leitura do arquivo binário
        while (fread(&funcionario, sizeof(Funcionario), 1, arquivo)) {
            if (strcmp(funcionario.cpf, cpf) == 0) {
                encontrado = 1;
                printf("Funcionário com CPF %s encontrado. Deseja realmente excluí-lo? (S/N): ", cpf);
                scanf(" %c", &confirmacao);

                if (confirmacao == 'S' || confirmacao == 's') {
                    printf("Funcionário excluído com sucesso!\n");
                    continue;
                } else {
                    printf("Operação cancelada.\n");
                }
            }
            fwrite(&funcionario, sizeof(Funcionario), 1, temp);
            registrosMantidos++;
        }

        fclose(arquivo);
        fclose(temp);

        if (registrosMantidos > 0) {
            remove("arquivos/funcionarios.bin");
            rename("arquivos/temp.bin", "arquivos/funcionarios.bin");
        } else {
            printf("Nenhum registro restante. O arquivo original será excluído.\n");
            remove("arquivos/funcionarios.bin");
            remove("arquivos/temp.bin");
        }

    } else if (formato == 2) {
        // Texto
        FILE *arquivo = fopen("arquivos/funcionarios.txt", "r");
        FILE *temp = fopen("arquivos/temp.txt", "w");

        if (arquivo == NULL || temp == NULL) {
            printf("Erro ao abrir os arquivos necessários.\n");
            if (arquivo) fclose(arquivo);
            if (temp) fclose(temp);
            return;
        }

        Funcionario funcionario;
        int registrosMantidos = 0;

        // Leitura do arquivo de texto
        while (fscanf(arquivo, " %99[^,], %14[^,], %49[^,], %f", 
                      funcionario.nome, funcionario.cpf, 
                      funcionario.cargo, &funcionario.salario) != EOF) {
            if (strcmp(funcionario.cpf, cpf) == 0) {
                encontrado = 1;
                printf("Funcionário com CPF %s encontrado. Deseja realmente excluí-lo? (S/N): ", cpf);
                scanf(" %c", &confirmacao);

                if (confirmacao == 'S' || confirmacao == 's') {
                    printf("Funcionário excluído com sucesso!\n");
                    continue; 
                } else {
                    printf("Operação cancelada.\n");
                }
            }
            fprintf(temp, "%s,%s,%s,%.2f\n", 
                    funcionario.nome, funcionario.cpf, 
                    funcionario.cargo, funcionario.salario);
            registrosMantidos++;
        }

        fclose(arquivo);
        fclose(temp);

        if (registrosMantidos > 0) {
            remove("arquivos/funcionarios.txt");
            rename("arquivos/temp.txt", "arquivos/funcionarios.txt");
        } else {
            printf("Nenhum registro restante. O arquivo original será excluído.\n");
            remove("arquivos/funcionarios.txt");
            remove("arquivos/temp.txt");
        }
    }

    if (!encontrado) {
        printf("Funcionário com CPF %s não encontrado.\n", cpf);
    }
}