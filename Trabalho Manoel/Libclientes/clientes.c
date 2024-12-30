#include <stdio.h>
#include <string.h>
#include "clientes.h"


void regcli() {
    cliDados cliente;

getchar();
printf("Insira o nome: ");
fgets(cliente.nome, sizeof(cliente.nome), stdin);
cliente.nome[strcspn(cliente.nome, "\n")] = '\0';

printf("Insira o CPF/CNPJ: ");
fgets(cliente.cpfCnpj, sizeof(cliente.cpfCnpj), stdin);
cliente.cpfCnpj[strcspn(cliente.cpfCnpj, "\n")] = '\0';

printf("Insira o endereço: ");
fgets(cliente.endereco, sizeof(cliente.endereco), stdin);
cliente.endereco[strcspn(cliente.endereco, "\n")] = '\0';

printf("Insira o telefone: ");
fgets(cliente.telefone, sizeof(cliente.telefone), stdin);
cliente.telefone[strcspn(cliente.telefone, "\n")] = '\0';

printf("Insira o email: ");
fgets(cliente.email, sizeof(cliente.email), stdin);
cliente.email[strcspn(cliente.email, "\n")] = '\0';

int formaReg;
FILE *formaArqv = fopen("arquivos/formato.bin", "rb");
if (formaArqv == NULL) {
    printf("Erro no formato do arquivo escolhido!");
    return;
}
fread(&formaReg, sizeof(int), 1, formaArqv);

switch (formaReg)
{
case 1:
    regcliBIN(cliente);
    break;

case 2:
    regcliTXT(cliente);
    break;
}

fclose(formaArqv);
}

void editcli() {
    int formaReg;

    FILE *formaArqv = fopen("arquivos/formato.bin", "rb");
    if (formaArqv == NULL) {
        printf("Erro no formato do arquivo escolhido!");
        return;
    }
    fread(&formaReg, sizeof(int), 1, formaArqv);

    switch (formaReg)
    {
    case 1:
        editcliBIN();
        break;
    
    case 2:
        editcliTXT();
        break;
    }
}

void regcliBIN(cliDados cliente ) {
  
    FILE *bin = fopen("arquivos/clientes.bin", "ab"); 
    if (bin == NULL) {
        perror("Erro ao abrir o arquivo!!");
        return;
}
    fwrite(&cliente, sizeof(cliDados), 1, bin);
    fclose(bin);
    
}

void lercliBIN() {
    cliDados clientes;
    
    FILE *bin = fopen("arquivos/clientes.bin", "rb");
    if (bin == NULL) {
        printf("Nenhum cliente registrado ainda!!\n");
        return;
    }

    printf("================================== Lista de Clientes ==================================\n");
    while (fread(&clientes, sizeof(cliDados), 1, bin)) {
        printf("Nome: %s\n", clientes.nome);
        printf("CPF/CNPJ: %s\n", clientes.cpfCnpj);
        printf("Endereço: %s\n", clientes.endereco);
        printf("Telefone: %s\n", clientes.telefone);
        printf("Email: %s\n", clientes.email);
        printf("--------------------------------------------------------------------------------------\n");
    }
    fclose(bin);
}
/* vai editar os cliente em binario e localizar o fornecedor atraves do cpf ou cnpj*/ 
void editcliBIN() {
    char cpfCnpjBusca[20];

    printf("Digite o CPF/CNPJ do cliente que será editado: ");
    scanf("%20s", cpfCnpjBusca); // o %20s no scanf vai ler ate 20 caracteres
    getchar(); 

    FILE *arquivo = fopen("arquivos/clientes.bin", "rb+"); 
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    /* vai ler cada registro do arquivo binario e depois vai comprar o cpf e cnpj do cliente apos isso vai salvar a posição atual que tiver no arquivo*/
    cliDados c;
    int encontrado = 0;
    long posicao; // armazena a posição do registro

    while (fread(&c, sizeof(cliDados), 1, arquivo) == 1) { 
        if (strcmp(c.cpfCnpj, cpfCnpjBusca) == 0) { 
            encontrado = 1;

            posicao = ftell(arquivo) - sizeof(cliDados);

            printf("Cliente encontrado! Digite os novos dados:\n");

            printf("Nome: ");
            fgets(c.nome, sizeof(c.nome), stdin);
            c.nome[strcspn(c.nome, "\n")] = '\0';

            printf("Endereço: ");
            fgets(c.endereco, sizeof(c.endereco), stdin);
            c.endereco[strcspn(c.endereco, "\n")] = '\0';

            printf("Telefone: ");
            fgets(c.telefone, sizeof(c.telefone), stdin);
            c.telefone[strcspn(c.telefone, "\n")] = '\0';

            printf("Email: ");
            fgets(c.email, sizeof(c.email), stdin);
            c.email[strcspn(c.email, "\n")] = '\0';

            // Move o ponteiro do arquivo para a posição correta
            fseek(arquivo, posicao, SEEK_SET);
            fwrite(&c, sizeof(cliDados), 1, arquivo);

            printf("Cliente foi atualizado com sucesso!\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Cliente com CPF/CNPJ %s não encontrado.\n", cpfCnpjBusca);
    }

    fclose(arquivo);
}
// registro dos clientes em texto---------------------------------------------------------------------------
void regcliTXT(cliDados cliente) {

    FILE *txt = fopen("arquivos/clientes.txt", "a"); // Abre o arquivo em texto
    if (txt == NULL) {
        printf("Erro ao abrir o arquivo!!\n");
        return;
    }
    
    fprintf(txt, "%s,%s,%s,%s,%s\n",
    cliente.nome, cliente.cpfCnpj, cliente.endereco, cliente.telefone, cliente.email);

    fclose(txt);
    printf("Cliente registrado com sucesso!");
    
} 
void lercliTXT() {
    cliDados clientes;
    char linha[256];
    
    FILE *txt = fopen("arquivos/clientes.txt", "r");
    if (txt == NULL) {
        printf("Nenhum cliente registrado ainda!!\n");
        return;
    }

    printf("================================== Lista de Clientes ==================================\n");
    while (fgets(linha, sizeof(linha), txt)) {
        if (sscanf(linha, "%[^,],%[^,],%[^,],%[^,],%[^\n]", 
                   clientes.nome, 
                   clientes.cpfCnpj, 
                   clientes.endereco, 
                   clientes.telefone, 
                   clientes.email) == 5) {
            printf("Nome: %s\n", clientes.nome);
            printf("CPF/CNPJ: %s\n", clientes.cpfCnpj);
            printf("Endereço: %s\n", clientes.endereco);
            printf("Telefone: %s\n", clientes.telefone);
            printf("Email: %s\n", clientes.email);
            printf("--------------------------------------------------------------------------------------\n");
        } else {
            printf("Erro ao processar linha: %s\n", linha);
        }
    }
    fclose(txt);
}
/* vai editar os clientes em texto e localizar o cliente atraves do cpf ou cnpj*/
void editcliTXT() {
    char cpfCnpjBusca[20];

    printf("Insira o CPF/CNPJ do cliente que deseja editar: ");
    scanf("%20s", cpfCnpjBusca); 
    getchar(); 

    FILE *txt = fopen("arquivos/clientes.txt", "r");
    if (txt == NULL) {
        printf("Erro: Nenhum cliente registrado ainda ou arquivo inexistente.\n");
        return;
    }

    FILE *temp = fopen("arquivos/temp.txt", "w");
    if (temp == NULL) {
        printf("Erro: Não foi possível criar um arquivo temporário.\n");
        fclose(txt);
        return;
    }

    cliDados c;
    int encontrado = 0;
    char linha[256];

    while (fgets(linha, sizeof(linha), txt)) {
        
        if (sscanf(linha, "%[^,],%[^,],%[^,],%[^,],%[^\n]", c.nome, c.cpfCnpj, c.endereco, c.telefone, c.email) == 5) {
            if (strcmp(c.cpfCnpj, cpfCnpjBusca) == 0) {
                encontrado = 1;
                printf("Cliente encontrado: %s\n", c.nome);
                printf("Insira os novos dados do cliente:\n");

                printf("Novo nome: ");
                fgets(c.nome, sizeof(c.nome), stdin);
                c.nome[strcspn(c.nome, "\n")] = '\0';

                printf("Novo endereço: ");
                fgets(c.endereco, sizeof(c.endereco), stdin);
                c.endereco[strcspn(c.endereco, "\n")] = '\0';

                printf("Novo telefone: ");
                fgets(c.telefone, sizeof(c.telefone), stdin);
                c.telefone[strcspn(c.telefone, "\n")] = '\0';

                printf("Novo email: ");
                fgets(c.email, sizeof(c.email), stdin);
                c.email[strcspn(c.email, "\n")] = '\0';

                fprintf(temp, "%s,%s,%s,%s,%s\n", c.nome, c.cpfCnpj, c.endereco, c.telefone, c.email);
            } else {
                // vai copair os dados originais para o arquivo temporario
                fprintf(temp, "%s", linha);
            }
        }
    }

    fclose(txt);
    fclose(temp);

    if (encontrado) {
        remove("arquivos/clientes.txt");
        if (rename("arquivos/temp.txt", "arquivos/clientes.txt") == 0) {
            printf("Cliente atualizado com sucesso!\n");
        } else {
            printf("Erro ao substituir o arquivo original.\n");
        }
    } else {
        remove("arquivos/temp.txt");
        printf("Cliente com CPF/CNPJ %s não encontrado.\n", cpfCnpjBusca);
    }
}

void lercli() {
    int formaReg;
    FILE *formaArqv = fopen("arquivos/formato.bin", "rb");
    if (formaArqv == NULL) {
        printf("Erro ao ler o arquivo!!");
        return;
    }
    fread(&formaReg, sizeof(int), 1, formaArqv);
    switch (formaReg)
    {
    case 1:
        lercliBIN();
        break;
    
    case 2:
        lercliTXT();
        break;
    }

    fclose(formaArqv);
}

void excluirRegistro() {
    int formato; 
    char confirmacao;

    FILE *formatoFile = fopen("arquivos/formato.bin", "rb");
    if (formatoFile == NULL) {
        printf("Erro ao abrir o arquivo de formato. Certifique-se de que ele existe!\n");
        return;
    }
    fread(&formato, sizeof(int), 1, formatoFile);
    fclose(formatoFile);

    if (formato == 1) {
        printf("Você selecionou o formato binário. Deseja excluir o arquivo de registro BIN? (S/N): ");
    } else if (formato == 2) {
        printf("Você selecionou o formato texto. Deseja excluir o arquivo de registro TXT? (S/N): ");
    } else {
        printf("Formato de registro inválido no arquivo de configuração.\n");
        return;
    }

    scanf(" %c", &confirmacao);
    while (getchar() != '\n'); 

    if (confirmacao != 'S' && confirmacao != 's') {
        printf("Exclusão cancelada.\n");
        return;
    }

    if (formato == 1) {
        if (remove("arquivos/clientes.bin") == 0) {
            printf("Arquivo BIN de registro excluído com sucesso!\n");
        } else {
            printf("Erro ao excluir o arquivo BIN de registro. Verifique se ele existe.\n");
        }
    } else if (formato == 2) {
        if (remove("arquivos/clientes.txt") == 0) {
            printf("Arquivo TXT de registro excluído com sucesso!\n");
        } else {
            printf("Erro ao excluir o arquivo TXT de registro. Verifique se ele existe.\n");
        }
    }
}

void excluirCliente() {
    char cpfCnpjBusca[20];
    char confirmacao;
    int formato;
    int encontrado = 0;


    FILE *formatoFile = fopen("arquivos/formato.bin", "rb");
    if (formatoFile == NULL) {
        printf("Erro ao abrir o arquivo de formato. Certifique-se de que ele existe!\n");
        return;
    }
    fread(&formato, sizeof(int), 1, formatoFile);
    fclose(formatoFile);

    while (getchar() != '\n'); 

    printf("Digite o CPF/CNPJ do cliente a ser excluído: ");
    fgets(cpfCnpjBusca, sizeof(cpfCnpjBusca), stdin);
    cpfCnpjBusca[strcspn(cpfCnpjBusca, "\n")] = '\0';

    if (formato == 1) {
    
        FILE *arquivo = fopen("arquivos/clientes.bin", "rb");
        FILE *temp = fopen("arquivos/temp.bin", "wb");

        if (arquivo == NULL || temp == NULL) {
            printf("Erro ao abrir os arquivos necessários.\n");
            if (arquivo) fclose(arquivo);
            if (temp) fclose(temp);
            return;
        }

        cliDados cliente;

        while (fread(&cliente, sizeof(cliDados), 1, arquivo)) {
            if (strcmp(cliente.cpfCnpj, cpfCnpjBusca) == 0) {
                encontrado = 1;
                printf("Cliente com CPF/CNPJ %s encontrado. Deseja realmente excluí-lo? (S/N): ", cpfCnpjBusca);
                scanf(" %c", &confirmacao);
                while (getchar() != '\n'); 

                if (confirmacao == 'S' || confirmacao == 's') {
                    printf("Cliente excluído com sucesso!\n");
                    continue;
                } else {
                    printf("Operação cancelada.\n");
                }
            }
            fwrite(&cliente, sizeof(cliDados), 1, temp);
        }

        fclose(arquivo);
        fclose(temp);

        if (encontrado) {
            remove("arquivos/clientes.bin");
            rename("arquivos/temp.bin", "arquivos/clientes.bin");
        } else {
            remove("arquivos/temp.bin"); 
        }
    } else if (formato == 2) {
    
        FILE *arquivo = fopen("arquivos/clientes.txt", "r");
        FILE *temp = fopen("arquivos/temp.txt", "w");

        if (arquivo == NULL || temp == NULL) {
            printf("Erro ao abrir os arquivos necessários.\n");
            if (arquivo) fclose(arquivo);
            if (temp) fclose(temp);
            return;
        }

        char linha[256];

        while (fgets(linha, sizeof(linha), arquivo)) {
            char cpfCnpj[20];
            sscanf(linha, "%*[^,],%19[^,]", cpfCnpj);

            if (strcmp(cpfCnpj, cpfCnpjBusca) == 0) {
                encontrado = 1;
                printf("Cliente com CPF/CNPJ %s encontrado. Deseja realmente excluí-lo? (S/N): ", cpfCnpjBusca);
                scanf(" %c", &confirmacao);
                while (getchar() != '\n'); 

                if (confirmacao == 'S' || confirmacao == 's') {
                    printf("Cliente excluído com sucesso!\n");
                    continue; 
                } else {
                    printf("Operação cancelada.\n");
                }
            }
            fprintf(temp, "%s", linha); 
        }

        fclose(arquivo);
        fclose(temp);

        if (encontrado) {
            remove("arquivos/clientes.txt");
            rename("arquivos/temp.txt", "arquivos/clientes.txt");
        } else {
            remove("arquivos/temp.txt"); 
        }
    } else {
        printf("Formato inválido definido no arquivo de configuração.\n");
        return;
    }

    if (!encontrado) {
        printf("Cliente com CPF/CNPJ %s não encontrado.\n", cpfCnpjBusca);
    }
}