#ifndef PECAS_H
#define PECAS_H

typedef struct{
    int codpec;
    char nomepec[20];    
    char descpec[60];
    char fabpec[30];
    char fornpec[30];
    float custpeca;
    float vendpeca;
    int npeca;
    int minpeca;
}dadopec;

float calculo(float custo);

void regpecabin(dadopec peca);
void regpecatxt(dadopec peca);
void regpecapath(dadopec peca);
void regpeca();

void editpecabin();
void editpecatxt();
void editpeca();

void lerpecabin();
void lerpecatxt();
void lerpeca();

void removerpecabin();
void removerpecatxt();
void removerpeca();

#endif