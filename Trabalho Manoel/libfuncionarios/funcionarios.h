#ifndef FUNCIONARIOS_H
#define FUNCIONARIOS_H

typedef struct {
    char nome[100];
    char cpf[15];
    char cargo[50];
    float salario;
} Funcionario;


void regfuncionariosbin(Funcionario funcionario);
void regfuncionariosTXT(Funcionario funcionario);
void regfuncionario();

void editfuncionariosbin();
void editfuncionariosTXT();
void editfuncionario();

void lerfuncionariosbin();
void lerfuncionariosTXT();
void lerfuncionario();

void excluirFuncionario();


#endif
