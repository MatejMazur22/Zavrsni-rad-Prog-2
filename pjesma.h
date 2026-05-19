#ifndef PJESME_H
#define PJESME_H

#define MAX_IME 100

static inline void safeFree(void** ptr) {

	if (*ptr != NULL) {

		free(*ptr);
		*ptr = NULL;

	}

	// Funkcija oslobadja din. zauzetu memoriju

}


typedef enum {

	SORT_NAZIV = 1,
	SORT_IZVODAC,
	SORT_TRAJANJE
	
} SORTIRANJE;


typedef struct {

	char ime[MAX_IME];
	char izvodac[MAX_IME];
	float trajanje;

} PJESMA;

void dodajPjesmu();
void obrisiPjesmu();
void prebaciPjesmu();
void ispisiPjesme(int id);
void sortirajPjesme(int id);
void pretraziPjesmu(int id, char* trazeno);
void obrisiSvePjesme();

PJESMA* ucitajPjesme(int id, int* n);

void ponavljanje_ispisa(PJESMA* niz, int ID, int n);

#endif