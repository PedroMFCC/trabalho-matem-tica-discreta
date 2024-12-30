#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fornecedor.h"

void regforn() {
    Forn fornecedor;
    
    printf("Insira o código do fornecedor: ");
    scanf("%d", &fornecedor.codigo);
    getchar();
    printf("Insira o nome fantasia: ");
    scanf("%59[^\n]", fornecedor.nomeFts);
    getchar();
    printf("Insira o CNPJ: ");
    scanf("%19[^\n]", fornecedor.Cnpj);
    getchar();
    printf("Insira a razão social: ");
    scanf("%150[^\n]", fornecedor.razSc);
    getchar();
    printf("Insira a inscrição estadual: ");
    scanf("%19[^\n]", fornecedor.insEE);
    getchar();
    printf("Insira o endereço: ");
    scanf("%129[^\n]", fornecedor.endCp);
    getchar();
    printf("Insira o telefone: ");
    scanf("%19[^\n]", fornecedor.Telef);
    getchar();
    printf("Insira o email: ");
    scanf("%149[^\n]", fornecedor.email);
    getchar();

    int formaReg;
    FILE *formaArqv = fopen("arquivos/formato.bin", "rb");
    if (formaArqv == NULL) {
        printf("Erro no formato do arquivo escolhido!");
        return;
    }
    fread(&formaReg, sizeof(int), 1, formaArqv);

    switch (formaReg) {
        case 1:
            regfornBIN(fornecedor);
            break;
        case 2:
            regfornTXT(fornecedor);
            break;
    }

    fclose(formaArqv);
}

void editforn() {
    int formaReg;

    FILE *formaArqv = fopen("arquivos/formato.bin", "rb");
    if (formaArqv == NULL) {
        printf("Erro no formato do arquivo escolhido!");
        return;
    }
    fread(&formaReg, sizeof(int), 1, formaArqv);

    switch (formaReg) {
        case 1:
            editfornBIN();
            break;
        case 2:
            editfornTXT();
            break;
    }
}

void regfornBIN(Forn fornecedor) {
    FILE *bin = fopen("arquivos/fornecedores.bin", "ab");
    if (bin == NULL) {
        perror("Erro ao abrir o arquivo!!");
        return;
    }
    fwrite(&fornecedor, sizeof(Forn), 1, bin);
    fclose(bin);
}

void lerfornBIN() {
    Forn fornecedor;
    
    FILE *bin = fopen("arquivos/fornecedores.bin", "rb");
    if (bin == NULL) {
        printf("Nenhum fornecedor registrado ainda!!\n");
        return;
    }

    printf("================================== Lista de Fornecedores ==================================\n");
    while (fread(&fornecedor, sizeof(Forn), 1, bin)) {
        printf("Código do fornecedor: %d\n", fornecedor.codigo);
        printf("Nome fantasia: %s\n", fornecedor.nomeFts);
        printf("Razão Social: %s\n", fornecedor.razSc);
        printf("Inscrição Estadual: %s\n", fornecedor.insEE);
        printf("CNPJ: %s\n", fornecedor.Cnpj);
        printf("Endereço: %s\n", fornecedor.endCp);
        printf("Telefone: %s\n", fornecedor.Telef);
        printf("Email: %s\n", fornecedor.email);
        printf("--------------------------------------------------------------------------------------\n");
    }
    fclose(bin);
}
/* vai editar os fornecedores em binário e localizaa o fornecedor atraves do código*/ 
void editfornBIN() {
    int codigoBusca;

    printf("Insira o código do fornecedor que deseja editar: ");
    scanf("%d", &codigoBusca); 
    getchar(); // Para consumir o '\n' deixado pelo scanf

    FILE *bin = fopen("arquivos/fornecedores.bin", "rb");
    if (bin == NULL) {
        printf("Erro: Nenhum fornecedor registrado ainda ou arquivo inexistente.\n");
        return;
    }

    FILE *temp = fopen("arquivos/temp.bin", "wb");
    if (temp == NULL) {
        printf("Erro: Não foi possível criar um arquivo temporário.\n");
        fclose(bin);
        return;
    }

    Forn f;
    int encontrado = 0;

    while (fread(&f, sizeof(Forn), 1, bin)) {
        if (f.codigo == codigoBusca) {
            encontrado = 1;
            printf("Fornecedor encontrado: %s\n", f.nomeFts);
            printf("Insira os novos dados do fornecedor:\n");

            printf("Novo nome fantasia: ");
            fgets(f.nomeFts, sizeof(f.nomeFts), stdin);
            f.nomeFts[strcspn(f.nomeFts, "\n")] = '\0';

            printf("Nova razão social: ");
            fgets(f.razSc, sizeof(f.razSc), stdin);
            f.razSc[strcspn(f.razSc, "\n")] = '\0';

            printf("Nova inscrição estadual: ");
            fgets(f.insEE, sizeof(f.insEE), stdin);
            f.insEE[strcspn(f.insEE, "\n")] = '\0';

            printf("Novo CNPJ: ");
            fgets(f.Cnpj, sizeof(f.Cnpj), stdin);
            f.Cnpj[strcspn(f.Cnpj, "\n")] = '\0';

            printf("Novo endereço: ");
            fgets(f.endCp, sizeof(f.endCp), stdin);
            f.endCp[strcspn(f.endCp, "\n")] = '\0';

            printf("Novo telefone: ");
            fgets(f.Telef, sizeof(f.Telef), stdin);
            f.Telef[strcspn(f.Telef, "\n")] = '\0';

            printf("Novo email: ");
            fgets(f.email, sizeof(f.email), stdin);
            f.email[strcspn(f.email, "\n")] = '\0';

            // Escrever os dados atualizados no arquivo temporário
            fwrite(&f, sizeof(Forn), 1, temp);
        } else {
            // Copiar os dados originais para o arquivo temporário
            fwrite(&f, sizeof(Forn), 1, temp);
        }
    }

    fclose(bin);
    fclose(temp);

    if (encontrado) {
        remove("arquivos/fornecedores.bin");
        if (rename("arquivos/temp.bin", "arquivos/fornecedores.bin") == 0) {
            printf("Fornecedor atualizado com sucesso!\n");
        } else {
            printf("Erro ao substituir o arquivo original.\n");
        }
    } else {
        remove("arquivos/temp.bin");
        printf("Fornecedor com código %d não encontrado.\n", codigoBusca);
    }
}
// registro dos fornecedores em texto-------------------------------------------------------------------------------------------------------------
void regfornTXT(Forn fornecedor) {
    FILE *txt = fopen("arquivos/fornecedores.txt", "a");
    if (txt == NULL) {
        printf("Erro ao abrir o arquivo!!\n");
        return;
    }

    fprintf(txt, "%d,%s,%s,%s,%s,%s,%s,%s\n", fornecedor.codigo,
            fornecedor.nomeFts, fornecedor.razSc, fornecedor.insEE, fornecedor.Cnpj, fornecedor.endCp, fornecedor.Telef, fornecedor.email);

    fclose(txt);
    printf("Fornecedor registrado com sucesso!");
}

void lerfornTXT() {
    Forn fornecedor;
    char linha[256];
    
    FILE *txt = fopen("arquivos/fornecedores.txt", "r");
    if (txt == NULL) {
        printf("Nenhum fornecedor registrado ainda!!\n");
        return;
    }

    printf("================================== Lista de Fornecedores ==================================\n");
    while (fgets(linha, sizeof(linha), txt)) {

        if (sscanf(linha, "%d,%59[^,],%149[^,],%19[^,],%19[^,],%149[^,],%19[^,],%149[^\n]",
                   &fornecedor.codigo,
                   fornecedor.nomeFts,
                   fornecedor.razSc,
                   fornecedor.insEE,
                   fornecedor.Cnpj,
                   fornecedor.endCp,
                   fornecedor.Telef,
                   fornecedor.email) == 8) {
            printf("Código do fornecedor: %d\n", fornecedor.codigo);
            printf("Nome fantasia: %s\n", fornecedor.nomeFts);
            printf("Razão Social: %s\n", fornecedor.razSc);
            printf("Inscrição Estadual: %s\n", fornecedor.insEE);
            printf("CNPJ: %s\n", fornecedor.Cnpj);
            printf("Endereço: %s\n", fornecedor.endCp);
            printf("Telefone: %s\n", fornecedor.Telef);
            printf("Email: %s\n", fornecedor.email);
            printf("--------------------------------------------------------------------------------------\n");
        } else {
            printf("Erro ao processar linha: %s\n", linha);
        }
    }
    fclose(txt);
}
/* vai editar os fornecedores em texto e localizaa o fornecedor atraves do código*/ 
void editfornTXT() {
    int codigoBusca;

    printf("Insira o código do fornecedor que deseja editar: ");
    scanf("%d", &codigoBusca); 
    getchar(); 

    FILE *txt = fopen("arquivos/fornecedores.txt", "r");
    if (txt == NULL) {
        printf("Erro: Nenhum fornecedor registrado ainda ou arquivo inexistente.\n");
        return;
    }

    FILE *temp = fopen("arquivos/temp.txt", "w");
    if (temp == NULL) {
        printf("Erro: Não foi possível criar um arquivo temporário.\n");
        fclose(txt);
        return;
    }

    Forn f;
    int encontrado = 0;
    char linha[256];

    while (fgets(linha, sizeof(linha), txt)) {
        if (sscanf(linha, "%d,%59[^,],%149[^,],%19[^,],%19[^,],%149[^,],%19[^,],%149[^\n]",
                   &f.codigo,
                   f.nomeFts,
                   f.razSc,
                   f.insEE,
                   f.Cnpj,
                   f.endCp,
                   f.Telef,
                   f.email) == 8) {
            if (f.codigo == codigoBusca) {
                encontrado = 1;
                printf("Fornecedor encontrado: %s\n", f.nomeFts);
                printf("Insira os novos dados do fornecedor:\n");

                printf("Novo nome fantasia: ");
                fgets(f.nomeFts, sizeof(f.nomeFts), stdin);
                f.nomeFts[strcspn(f.nomeFts, "\n")] = '\0';

                printf("Nova razão social: ");
                fgets(f.razSc, sizeof(f.razSc), stdin);
                f.razSc[strcspn(f.razSc, "\n")] = '\0';

                printf("Nova inscrição estadual: ");
                fgets(f.insEE, sizeof(f.insEE), stdin);
                f.insEE[strcspn(f.insEE, "\n")] = '\0';

                printf("Novo CNPJ: ");
                fgets(f.Cnpj, sizeof(f.Cnpj), stdin);
                f.Cnpj[strcspn(f.Cnpj, "\n")] = '\0';

                printf("Novo endereço: ");
                fgets(f.endCp, sizeof(f.endCp), stdin);
                f.endCp[strcspn(f.endCp, "\n")] = '\0';

                printf("Novo telefone: ");
                fgets(f.Telef, sizeof(f.Telef), stdin);
                f.Telef[strcspn(f.Telef, "\n")] = '\0';

                printf("Novo email: ");
                fgets(f.email, sizeof(f.email), stdin);
                f.email[strcspn(f.email, "\n")] = '\0';

                // vai escrever os novos dados no arquivo temporário
                fprintf(temp, "%d,%s,%s,%s,%s,%s,%s,%s\n",
                        f.codigo,
                        f.nomeFts,
                        f.razSc,
                        f.insEE,
                        f.Cnpj,
                        f.endCp,
                        f.Telef,
                        f.email);
            } else {
                // vai copiar os dados originais para o arquivo temporário
                fprintf(temp, "%s", linha);
            }
        }
    }

    fclose(txt);
    fclose(temp);

    if (encontrado) {
        remove("arquivos/fornecedores.txt");
        if (rename("arquivos/temp.txt", "arquivos/fornecedores.txt") == 0) {
            printf("Fornecedor atualizado com sucesso!\n");
        } else {
            printf("Erro ao substituir o arquivo original.\n");
        }
    } else {
        remove("arquivos/temp.txt");
        printf("Fornecedor com código %d não encontrado.\n", codigoBusca);
    }
}
/* vai escolher a forma de resgitro do fornecedorr*/
void lerforn() {
    int formaReg;
    FILE *formaArqv = fopen("arquivos/formato.bin", "rb");
    if (formaArqv == NULL) {
        printf("Erro ao ler o arquivo!!");
        return;
    }
    fread(&formaReg, sizeof(int), 1, formaArqv);
    if (formaReg == 1) {
        lerfornBIN();
    } else {
        lerfornTXT();
    }

    fclose(formaArqv);
}

void excluirReg() {
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
        if (remove("arquivos/fornecedores.bin") == 0) {
            printf("Arquivo BIN de registro excluído com sucesso!\n");
        } else {
            printf("Erro ao excluir o arquivo BIN de registro. Verifique se ele existe.\n");
        }
    } else if (formato == 2) {
        if (remove("arquivos/fornecedores.txt") == 0) {
            printf("Arquivo TXT de registro excluído com sucesso!\n");
        } else {
            printf("Erro ao excluir o arquivo TXT de registro. Verifique se ele existe.\n");
        }
    }
}

void excluirFornecedor() {
    int codigoBusca;
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

    printf("Digite o código do fornecedor a ser excluído: ");
    scanf("%d", &codigoBusca);
    getchar(); // Para limpar o buffer do teclado

    if (formato == 1) {
    
        FILE *arquivo = fopen("arquivos/fornecedores.bin", "rb");
        FILE *temp = fopen("arquivos/temp.bin", "wb");

        if (arquivo == NULL || temp == NULL) {
            printf("Erro ao abrir os arquivos necessários.\n");
            if (arquivo) fclose(arquivo);
            if (temp) fclose(temp);
            return;
        }

        Forn fornecedor;

        while (fread(&fornecedor, sizeof(Forn), 1, arquivo)) {
            if (fornecedor.codigo == codigoBusca) {
                encontrado = 1;
                printf("Fornecedor com código %d encontrado. Deseja realmente excluí-lo? (S/N): ", codigoBusca);
                scanf(" %c", &confirmacao);
                while (getchar() != '\n'); // Limpa o buffer

                if (confirmacao == 'S' || confirmacao == 's') {
                    printf("Fornecedor excluído com sucesso!\n");
                    continue;
                } else {
                    printf("Operação cancelada.\n");
                }
            }
            fwrite(&fornecedor, sizeof(Forn), 1, temp);
        }

        fclose(arquivo);
        fclose(temp);

        if (encontrado) {
            remove("arquivos/fornecedores.bin");
            rename("arquivos/temp.bin", "arquivos/fornecedores.bin");
        } else {
            remove("arquivos/temp.bin"); 
        }
    } else if (formato == 2) {
    
        FILE *arquivo = fopen("arquivos/fornecedores.txt", "r");
        FILE *temp = fopen("arquivos/temp.txt", "w");

        if (arquivo == NULL || temp == NULL) {
            printf("Erro ao abrir os arquivos necessários.\n");
            if (arquivo) fclose(arquivo);
            if (temp) fclose(temp);
            return;
        }

        char linha[256];

        while (fgets(linha, sizeof(linha), arquivo)) {
            int codigo;
            sscanf(linha, "%d,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,\n]", &codigo);

            if (codigo == codigoBusca) {
                encontrado = 1;
                printf("Fornecedor com código %d encontrado. Deseja realmente excluí-lo? (S/N): ", codigoBusca);
                scanf(" %c", &confirmacao);
                while (getchar() != '\n'); // Limpa o buffer

                if (confirmacao == 'S' || confirmacao == 's') {
                    printf("Fornecedor excluído com sucesso!\n");
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
            remove("arquivos/fornecedores.txt");
            rename("arquivos/temp.txt", "arquivos/fornecedores.txt");
        } else {
            remove("arquivos/temp.txt"); 
        }
    } else {
        printf("Formato inválido definido no arquivo de configuração.\n");
        return;
    }

    if (!encontrado) {
        printf("Fornecedor com código %d não encontrado.\n", codigoBusca);
    }
}