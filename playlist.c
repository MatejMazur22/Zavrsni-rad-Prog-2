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

	while (fscanf(fp, "%d;%99[^\n]\n", &p.id, p.naziv) == 2) { // cita indeks;naziv -> broj pa ; i onda maks 99 znakova dok ne dodje do '\n' (dok ne ucita oba dva parametra, i id i naziv)

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

	// Provjera ima li ikakav redak u datoteci popisa playlisti
	if (fgets(redak, sizeof(redak), fp) != NULL) {

		fclose(fp);
		return 1;

	}

	fclose(fp);
	// Datoteka postoji ali je prazna jer je datoteka fp == NULL
	return 0;

}


void kreirajPlaylistu() {

	PLAYLISTA p;

	p.id = generirajID();

	getchar(); // ako se dogodi slucajni '\n'

	printf("Unesite naziv playliste: ");

	fgets(p.naziv, MAX_NAZIV, stdin);


	p.naziv[strcspn(p.naziv, "\n")] = 0; // strcspn -> trazi 1. pojavu nekog znaka iz jednog stringa u drugom

	char imeDatoteke[50];

	sprintf(imeDatoteke, "playlist_%d.txt", p.id); // ispis kao printf, samo ispisuje u string

	FILE* fp = fopen(imeDatoteke, "w");

	if (fp == NULL) {

		printf("Greska pri kreiranju Playliste.\n");
		return;

	}

	fprintf(fp, "PLAYLISTA: %s\n", p.naziv);
	fclose(fp);

	FILE* popis = fopen(POPIS_PLAYLISTI, "a"); // "a" za pretrazivanje

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

		printf("Nema spremljenih playlista.\n");
		return;

	}

	PLAYLISTA p;

	int postojiPlaylista = 0;

	while (fscanf(fp, "%d;%99[^\n]\n", &p.id, &p.naziv) == 2) { // provjerava ako se nigdje u datoteci ne pronadje id i naziv neke playliste, znaci ne postoje.

		if (postojiPlaylista == 0) {

			printf("\n -----LISTA PLAYLISTI-----\n");

			postojiPlaylista = 1;

		}

		printf("ID: %d | Naziv: %s\n", p.id, p.naziv);

	}

	if (postojiPlaylista == 0) {

		printf("Nema spremljenih playlisti!\n");

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

		printf("Greska pri otvaranju datoteka.\n");
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

	printf("Playlista je obrisana.\n");

	/*

	Trazi se upis ID za brisanje
	Otvara se datoteka s popisom playlisti
	Traži se ako postoji ta playlista -> while petlja
	zamjenjuju se datoteke temp i playliste (temp postaje playliste.txt bez playliste s trazenim ID-em)
	brise se datoteka playliste s trazenim ID-em

	*/


}

void dodajPjesmu() {

	if (!postojiLiPlaylista()) {

		printf("Prvo morate kreirati barem jednu playlistu!\n");
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
	p.ime[strcspn(p.ime, "\n")] = 0; // prva pojava znaka '\n' postaje znak '\0' (kraj stringa)

	printf("Izvodac ");
	fgets(p.izvodac, MAX_IME, stdin);
	p.ime[strcspn(p.izvodac, "\n")] = 0;

	printf("Trajanje: ");
	scanf("%f", &p.trajanje);

	char imeDatoteke[50];

	sprintf(imeDatoteke, "playlist_%d.txt", id);

	FILE* fp = fopen(imeDatoteke, "a");

	if (fp == NULL) {

		printf("Playlista ne postoji.\n");
		return;

	}

	fprintf(fp, "%s;%s;%.2f\n", p.ime, p.izvodac, p.trajanje);

	fclose(fp);

	printf("Pjesma uspjesno dodana.\n");


}