#ifndef SERVICOS_H
#define SERVICOS_H

typedef struct {
    int codigo;
    char descricao[100];
    float preco;
    float comissao;
} Servico;

void regservicobin(Servico servico);
void regservicotxt(Servico servico);
void regservico();

void editservicobin();
void editservicotxt();
void editservico();

void lerservicobin();
void lerservicotxt();
void lerservico();

void excluirServico();



#endif
