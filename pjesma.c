#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "pjesma.h"
#include "playlist.h"

void  ponavljanje_ispisa(PJESMA* niz, int ID, int n) {

	if (ID >= n) return;

	printf("%s - %s (%.2f)\n", 
		niz[ID].izvodac, 
		niz[ID].ime, 
		niz[ID].trajanje);

	ponavljanje_ispisa(niz, ID + 1, n);

}

PJESMA* ucitajPjesme(int id, int* n) {


	char file[50];
	sprintf(file, "playlist_%d.txt", id);

	FILE* fp = fopen(file, "r");

	if (fp == NULL) {

		perror("\nGreska pri otvaranju playliste!\n");
		*n = 0;
		return NULL;

	}

	int count = 0;

	char ime[MAX_IME];
	char izvodac[MAX_IME];
	float trajanje;

	while (fscanf(fp, "%99[^;];%99[^;];%f\n", ime, izvodac, &trajanje) == 3) {

		count++;

	}

	if (count == 0) {

		fclose(fp);
		*n = 0;
		return NULL;

	}

	PJESMA* niz = malloc(count * sizeof(PJESMA));

	if (niz == NULL) {

		perror("\nNeuspjesno zauzeta memorija!\n");
		fclose(fp);
		*n = 0;
		return NULL;

	}

	rewind(fp);

	int i = 0;
	while (fscanf(fp, "%99[^;];%99[^;];%f\n", niz[i].ime, niz[i].izvodac, &niz[i].trajanje) == 3) {

		i++;

	}

	if (ferror(fp)) {

		perror("\nGreska pri citanju datoteke.\n");

	}


	fclose(fp);

	*n = count;
	return niz;


}


void dodajPjesmu() {

	if (!postojiLiPlaylista()) {

		printf("\nPrvo morate kreirati barem jednu playlistu!\n");
		return;

	}

	ispisiPlayliste();

	int id;

	printf("\nUnesite ID playliste: ");
	scanf("%d", &id);

	getchar();

	PJESMA p;

	printf("Ime pjesme: ");
	fgets(p.ime, MAX_IME, stdin);
	p.ime[strcspn(p.ime, "\n")] = 0;

	printf("Izvodac: ");
	fgets(p.izvodac, MAX_IME, stdin);
	p.izvodac[strcspn(p.izvodac, "\n")] = 0;

	printf("\nTrajanje (npr. 2.15 za 2 min 15 sec): ");
	scanf("%f", &p.trajanje);
	getchar();

	char imeDatoteke[50];

	sprintf(imeDatoteke, "playlist_%d.txt", id);

	FILE* fp = fopen(imeDatoteke, "a");

	if (fp == NULL) {

		printf("\nPlaylista ne postoji.\n");
		return;

	}

	fprintf(fp, "%s;%s;%.2f\n", p.ime, p.izvodac, p.trajanje);

	fclose(fp);

	printf("\nPjesma uspjesno dodana.\n");


}


void ispisiPjesme(int id) {

	int n;
	PJESMA* niz = ucitajPjesme(id, &n);

	if (niz == NULL || n == 0) {

		printf("\nNema pjesama ili playlista ne postoji.\n");
		return;

	}

	printf("\n---- Pjesme ----\n");

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

		printf("\nNema pjesama.\n");

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

	int idPlaylist;
	int trazeno;

	ispisiPlayliste();


	printf("Unesite ID playliste: ");
	if (scanf("%d", &idPlaylist) != 1) {

		printf("Pogresan unos!\n");
		CLEAR_BUFFER();
		return;

	}

	
	printf("Unesite ID pjesme za brisanje: ");
	if (scanf("%d", &trazeno)) {

		printf("Pogresan unos!\n");
		CLEAR_BUFFER();
		return;

	}

	CLEAR_BUFFER();

	char file[50], tempFile[50];

	sprintf(file, "playlist_%d.txt", idPlaylist);
	sprintf(tempFile, "temp_%d.txt", idPlaylist);

	FILE* fp = fopen(file, "r");
	FILE* temp = fopen(tempFile, "w");

	if (!fp || !temp) {

		printf("Greska pri otvaranju datoteka.\n");
		if (fp) fclose(fp);
		if (temp) fclose(temp);
		return;

	}

	PJESMA p;
	int trenutno = 0;
	int pronadeno = 0;

	while (fscanf(fp, "%99[^;];%99[^;];%f\n", p.ime, p.izvodac, &p.trajanje) == 3) {

		trenutno++;

		if (trenutno == trazeno) {

			pronadeno = 1;
			continue;

		}

		fprintf(temp, "%s;%s;%.2f\n", p.ime, p.izvodac, p.trajanje);


	}

	fclose(fp);
	fclose(temp);

	remove(file);
	rename(tempFile, file);

	if (pronadeno) {

		printf("Pjesma je obrisana.\n");

	} else {

		printf("ID pjesme ne postoji.\n");

	}

}

void prebaciPjesmu() {

	int izvorID, odredID;

	ispisiPlayliste();

	printf("Odaberite ID playlist-e za prijenos: ");
	if (scanf("%d", &izvorID) != 1) {
		printf("Pogresan unos!\n");
		CLEAR_BUFFER();
		return;
	}

	printf("Odaberite ID playlist-e za odrediste: ");

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

	ispisiPjesme(izvorID);

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

			fprintf(odred,
				"%s;%s;%.2f\n",
				p.ime,
				p.izvodac,
				p.trajanje);

			pronadjena = 1;
		}
		else {

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

	ispisiPlayliste();

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

	fclose(fp);

	printf("Sve pjesme obrisane iz playliste.\n");

}


void preimenujPjesmu() {

	int id;
	char staroIme[MAX_IME];
	char novoIme[MAX_IME];

	ispisiPlayliste();

	printf("Unesite ID playliste: ");
	if (scanf("%d", &id) != 1) {

		printf("Pogresan unos!\n");
		CLEAR_BUFFER();
		return;

	}

	CLEAR_BUFFER();

	printf("\n ----Trenutne pjesme ----");
	ispisiPjesme(id);

	printf("/nUnesite trenutno ime pjesme: ");
	fgets(staroIme, MAX_IME, stdin);
	staroIme[strcspn(staroIme, "\n")] = 0;

	printf("/nUnesite novo ime pjesme: ");
	fgets(novoIme, MAX_IME, stdin);
	novoIme[strcspn(novoIme, "\n")] = 0;

	char file[50];
	sprintf(file, "playlist_%d.txt", id);

	FILE* fp = fopen(file, "r");
	FILE* temp = fopen("temp.txt", "w");

	if (!fp || !temp) {

		printf("\nGreska pri otvaranju datoteka!\n");
		if (fp) fclose(fp);

		if (temp) fclose(temp);
		
		return;

	}

	PJESMA p; 
	int found = 0;

	while (fscanf(fp, "%99[^;];%99[^;];%f\n", p.ime, p.izvodac, &p.trajanje) == 3) {

		if (strcmp(p.ime, staroIme) == 0) {

			strcpy(p.ime, novoIme);
			found = 1;

		}

		fprintf(temp, "%s;%s;%.2f\n", p.ime, p.izvodac, p.trajanje);

	}


	fclose(fp);
	fclose(temp);

	remove(file);
	rename("temp.txt", file);


	if (found) {

		printf("\nPjesma je uspjesno preimenovana.\n");

	} else {

		printf("\nPjesma nije pronadjena.\n");

	}

}