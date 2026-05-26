#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "playlist.h"
#include "pjesma.h"

const char* POPIS_PLAYLISTI = "playliste.txt";

int generirajID() {

	FILE* fp = fopen(POPIS_PLAYLISTI, "r");

	if (fp == NULL) {

		return 1;

	}


	PLAYLISTA p;
	int zadnjiID = 0;

	while (fscanf(fp, "%d;%99[^\n]\n", &p.id, p.naziv) == 2) {

		zadnjiID = p.id;

	}

	fclose(fp);

	return zadnjiID + 1;


}


int postojiLiPlaylista() {

	FILE* fp = fopen(POPIS_PLAYLISTI, "r");

	if (fp == NULL) {

		return 0;

	}

	char redak[100];

	if (fgets(redak, sizeof(redak), fp) != NULL) {

		fclose(fp);
		return 1;

	}

	fclose(fp);

	return 0;

}


void kreirajPlaylistu() {

	PLAYLISTA p;

	p.id = generirajID();

	getchar();

	printf("Unesite naziv playliste: ");

	fgets(p.naziv, MAX_NAZIV, stdin);


	p.naziv[strcspn(p.naziv, "\n")] = 0;

	char imeDatoteke[50];

	sprintf(imeDatoteke, "playlist_%d.txt", p.id);

	FILE* fp = fopen(imeDatoteke, "w");

	if (fp == NULL) {

		printf("Greska pri kreiranju Playliste.\n");
		return;

	}

	fclose(fp);

	FILE* popis = fopen(POPIS_PLAYLISTI, "a");

	if (popis == NULL) {

		printf("Greska pri spremanju playliste.\n");
		return;

	}

	fprintf(popis, "%d;%s\n", p.id, p.naziv);

	fclose(popis);

	printf("\nPlaylista je uspjesno kreirana.\n");
	printf("ID: %d, Naziv: %s", p.id, p.naziv);


}


void ispisiPlayliste() {


	FILE* fp = fopen(POPIS_PLAYLISTI, "r");

	if (fp == NULL) {

		printf("\nNema spremljenih playlista.\n");
		return;

	}

	PLAYLISTA p;

	int postojiPlaylista = 0;

	while (fscanf(fp, "%d;%99[^\n]\n", &p.id, &p.naziv) == 2) {

		if (postojiPlaylista == 0) {

			printf("\n ----- LISTA PLAYLISTI -----\n");

			postojiPlaylista = 1;

		}

		printf("ID: %d | Naziv: %s\n", p.id, p.naziv);

	}

	if (postojiPlaylista == 0) {

		printf("\nNema spremljenih playlisti!\n");

	}

	fclose(fp);


}


void obrisiPlaylistu() {

	ispisiPlayliste();

	int trazeniID;


	printf("\nUnesite ID playliste za brisanje: ");
	scanf("%d", &trazeniID);

	FILE* fp = fopen(POPIS_PLAYLISTI, "r");
	FILE* temp = fopen("temp.txt", "w");

	if (fp == NULL || temp == NULL) {

		printf("\nGreska pri otvaranju datoteka.\n");
		return;

	}

	PLAYLISTA p;

	while (fscanf(fp, "%d;%99[^\n]\n", &p.id, &p.naziv) == 2) {

		if (p.id != trazeniID) {

			fprintf(temp, "%d; %s\n", p.id, p.naziv);

		}

	}

	fclose(fp);
	fclose(temp);
	remove(POPIS_PLAYLISTI);
	rename("temp.txt", POPIS_PLAYLISTI);

	char imeDatoteke[50];

	sprintf(imeDatoteke, "playlist_%d.txt", trazeniID);

	remove(imeDatoteke);

	printf("\nPlaylista je obrisana.\n");


}


void preimenujPlaylistu() {

	int id;
	char novoIme[MAX_NAZIV];

	

	CLEAR_BUFFER();

	printf("\n---- Sve trenutne playliste ----\n");
	ispisiPlayliste();

	printf("\nUnesite ID playliste: ");
	if (scanf("%d", &id) != 1) {

		printf("\nPogresan unos!\n");
		CLEAR_BUFFER();
		return;

	}

	printf("\nUnesite novo ime playliste: ");
	fgets(novoIme, MAX_NAZIV, stdin);
	novoIme[strcspn(novoIme, "\n")] = 0;

	FILE* fp = fopen(POPIS_PLAYLISTI, "r");
	FILE* temp = fopen("temp.txt", "w");

	if (!fp || !temp) {

		printf("\nGreska pri otvaranju datoteka!\n");
		if (fp) fclose(fp);

		if (temp) fclose(temp);

		return;

	}

	PLAYLISTA p;
	int found = 0;

	while (fscanf(fp, "%d;%99[^\n]\n", &p.id, p.naziv) == 2) {

		if (p.id == id) {

			strcpy(p.naziv, novoIme);
			found = 1;

		}

		fprintf(temp, "%d;%s\n", p.id, p.naziv);

	}

	fclose(fp);
	fclose(temp);

	remove(POPIS_PLAYLISTI);
	rename("temp.txt", POPIS_PLAYLISTI);

	if (found) {

		printf("\nPlaylista je preimenovana!\n");

		printf("\n---- Nova lista playlisti ----\n");
		ispisiPlayliste();

	} else {

		printf("\nPlaylista nije pronadena.\n");

	}

}