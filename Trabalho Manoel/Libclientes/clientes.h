#ifndef CLIENTES_H
#define CLIENTES_H

typedef struct {
    char nome[50];
    char cpfCnpj[20];
    char endereco[100];
    char telefone[20];
    char email[50];

} cliDados;

void regcliBIN();
void editcliBIN();
void lercliBIN();

void regcliTXT();
void editcliTXT();
void lercliTXT();

void regcli();
void editcli();
void lercli();
void menucliente();

void excluirRegistro();
void excluirCliente();

#endif