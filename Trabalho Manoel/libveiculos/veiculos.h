#ifndef VEICULOS_H
#define VEICULOS_H

typedef struct {
    char placa[10];
    char modelo[50];
    char marca[50];
    int anoFabricacao;
    char chassi[25];
    char proprietario[100];
} Veiculo;

void regveiculobin(Veiculo veiculo);
void regveiculotxt(Veiculo veiculo);
void regveiculo();

void editveiculobin();
void editveiculotxt();
void editveiculo();

void lerveiculobin();
void lerveiculotxt();
void lerveiculo();

void excluirVeiculo();


#endif
