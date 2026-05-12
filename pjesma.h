#ifndef PJESMA_H
#define PJESMA_H

#define MAX_IME 100
#define MAX_IZVODAC 100

typedef struct {

    char ime[MAX_IME];
    char izvodac[MAX_IZVODAC];
    float trajanje;

} PJESMA;

void dodajPjesmu();
void obrisiPjesmu();
void prebaciPjesmu();

#endif