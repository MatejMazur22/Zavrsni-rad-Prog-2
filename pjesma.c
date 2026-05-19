#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "pjesma.h"
#include "playlist.h"

void  ponavljanje_ispisa(PJESMA* niz, int ID, int n) {

	if (ID >= n) return;

	printf("%s - %s (%.2f)\n", niz[ID].izvodac, niz[ID].ime, niz[ID].trajanje);

	ponavljanje_ispisa(niz, ID + 1, n);

}

PJESMA* ucitajPjesme(int id, int* n) {


	char file[50];
	sprintf(file, "playlist_%d.txt", id); // ispis kao printf, samo ispisuje u string

	FILE* fp = fopen(file, "r");

	if (fp == NULL) {

		perror("Greska pri otvaranju playliste");
		*n = 0;
		return NULL;

	}

	int count = 0;

	char ime[MAX_IME];
	char izvodac[MAX_IME];
	float trajanje;

	while (fscanf(fp, "%99[^;]°;%99[^;];%f\n", ime, izvodac, &trajanje) == 3) {

		count++;

	} // Preborajavanje pjesama, ako ima sva tri parametra (ime, izvodac, trajanje) onda se count povecava za 1

	if (count == 0) {

		fclose(fp);
		*n = 0;
		return NULL;

	}

	PJESMA* niz = malloc(count * sizeof(PJESMA));

	if (niz == NULL) {

		perror("Neuspjesno zauzeta memorija!");
		fclose(fp);
		*n = 0;
		return NULL;

	}

	rewind(fp); // vraca pok na pocetak file fp

	int i = 0;
	while (fscanf(fp, "%99[^;];%99[^;];%f\n", niz[i].ime, niz[i].izvodac, &niz[i].trajanje) == 3) {

		i++;

	}

	if (ferror(fp)) {

		perror("Greska pri citanju datoteke.");

	}


	fclose(fp);

	*n = count;
	return niz;


}

void ispisiPjesme(int id) {

	int n;
	PJESMA* niz = ucitajPjesme(id, &n);

	if (niz == NULL || n == 0) {

		printf("Nema pjesama ili playlista ne postoji.\n");
		return;

	}

	printf("\n--Pjesme--\n");

	ponavljanje_ispisa(niz, 0, n);

	free(niz);


}

int usporedi_Naziv(const void* a, const void* b) {

	return strcmp((*(PJESMA*)a).ime, (*(PJESMA*)b).ime);

}

int usporedi_Izvodaca(const void* a, const void* b) {

	return strcmp((*(PJESMA*)a).izvodac, (*(PJESMA*)b).izvodac);

}

int usporedi_Trajanje(const void* a, const void* b) {

	float t1 = (*(PJESMA*)a).trajanje;
	float t2 = (*(PJESMA*)b).trajanje;

	if (t1 > t2) return 1;
	if (t1 < t2) return -1;

	return 0;

}

void sortirajPjesme(int id) {


	int n;

	PJESMA* niz = ucitajPjesme(id, &n);

	if (niz == NULL || n == 0) {

		printf("Nema pjesama.");

		return;

	}

	int izbor;

	printf("1. Naziv");
	printf("2. Izvodac");
	printf("3. Trajanje");
	printf("Odabir: ");
	scanf("%d", &izbor);

	switch (izbor) {

		case SORT_NAZIV:

			qsort(niz, n, sizeof(PJESMA), usporedi_Naziv);
			break;

		case SORT_IZVODAC:

			qsort(niz, n, sizeof(PJESMA), usporedi_Izvodaca);
			break;

		case SORT_TRAJANJE:

			qsort(niz, n, sizeof(PJESMA), usporedi_Trajanje);

		default:

			printf("Pogresno unesen odabir. Pokusajte ponovno!");
			safeFree((void**)&niz);
			return;

	}

	printf("----SORTIRANE PJESME----");

	for (int i = 0; i < n; i++) {

		printf("%s - %s (%.2f)", niz[i].izvodac, niz[i].ime, niz[i].trajanje);

	}

	safeFree((void**)&niz);


}

int compareSearch(const void* key, const void* element) {

	return strcmp((char*)key, (*(PJESMA*)element).ime);

}

void pretraziPjesmu(int id, char* trazeno) {

	int n;

	PJESMA* niz = ucitajPjesme(id, &n);

	if (niz == NULL || n == 0) {

		printf("Nema pjesama.");

		return;

	}

	qsort(niz, n, sizeof(PJESMA), usporedi_Naziv);

	PJESMA* rezultat = bsearch(trazeno, niz, n, sizeof(PJESMA), compareSearch);

	if (rezultat == NULL) { 

		printf("Pjesma nije pronadena.");

	} else {

		printf("Pronadena pjesma: ");
		printf("%s - %s (%.2f)", (*rezultat).izvodac, (*rezultat).ime, (*rezultat).trajanje);

	}

	safeFree((void**)&niz);

}

void obrisiPjesmu() {



}

void prebaciPjesmu() {

	int izvorID, odredID;

	printf("Izvor playlist ID: ");

	if (scanf("%d", &izvorID) != 1) {
		printf("Pogresan unos!\n");
		CLEAR_BUFFER();
		return;
	}

	printf("Odredisni playlist ID: ");

	if (scanf("%d", &odredID) != 1) {
		printf("Pogresan unos!\n");
		CLEAR_BUFFER();
		return;
	}

	CLEAR_BUFFER();

	char izvorFile[50], odredFile[50];
	sprintf(izvorFile, "playlist_%d.txt", izvorID);
	sprintf(odredFile, "playlist_%d.txt", odredID);

	FILE* izvor = fopen(izvorFile, "r");
	FILE* odred = fopen(odredFile, "a");
	FILE* temp = fopen("temp.txt", "w");

	if (!izvor || !odred || !temp) {
		printf("Greska kod datoteka.\n");
		return;
	}

	char trazeno[MAX_IME];

	printf("Naziv pjesme za prebacivanje: ");
	fgets(trazeno, MAX_IME, stdin);
	trazeno[strcspn(trazeno, "\n")] = 0;

	PJESMA p;

	int pronadjena = 0;

	while (fscanf(izvor,
		"%99[^;];%99[^;];%f\n",
		p.ime,
		p.izvodac,
		&p.trajanje) == 3) {

		if (strcmp(p.ime, trazeno) == 0) {

			// PREBACI u drugu playlistu
			fprintf(odred,
				"%s;%s;%.2f\n",
				p.ime,
				p.izvodac,
				p.trajanje);

			pronadjena = 1;
		}
		else {

			// ostaje u izvornoj
			fprintf(temp,
				"%s;%s;%.2f\n",
				p.ime,
				p.izvodac,
				p.trajanje);
		}
	}

	fclose(izvor);
	fclose(odred);
	fclose(temp);

	remove(izvorFile);
	rename("temp.txt", izvorFile);

	if (pronadjena) {

		printf("Pjesma prebacena.\n");

	} else {

		printf("Pjesma nije pronadena.\n");

	}

}


void obrisiSvePjesme() {

	int id;

	printf("ID playliste: ");

	if (scanf("%d", &id) != 1) {
		printf("Pogresan unos!\n");
		CLEAR_BUFFER();
		return;
	}

	char file[50];
	sprintf(file, "playlist_%d.txt", id);

	FILE* fp = fopen(file, "w");

	if (fp == NULL) {
		printf("Greska kod otvaranja datoteke.\n");
		return;
	}

	// prepisuje prazno -> briše sve pjesme
	fclose(fp);

	printf("Sve pjesme obrisane iz playliste.\n");

}