#ifndef FORNECEDOR_H
#define FORNECEDOR_H

typedef struct {
    int codigo;
    char nomeFts[60]; // Nome Fantasia
    char Cnpj[20];
    char razSc[150]; // Razão Social
    char insEE[20]; // Inscrição Estadual
    char endCp[150]; // Endereço Completo
    char Telef[20];
    char email[150];

} Forn;

void regfornBIN();
void editfornBIN();
void lerfornBIN();

void regfornTXT();
void editfornTXT();
void lerfornTXT();

void regforn();
void editforn();
void lerforn();
void menufornecedor();

void excluirReg();
void excluirFornecedor();

#endif