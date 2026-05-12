#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "playlist.h"

int generirajNoviID() {

    FILE* fp = fopen("playliste.txt", "r");

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

void kreirajPlaylistu() {

    PLAYLISTA p;

    p.id = generirajNoviID();

    getchar();

    printf("Unesite naziv playliste: ");

    fgets(p.naziv, MAX_NAZIV, stdin);

    p.naziv[strcspn(p.naziv, "\n")] = 0;

    char imeDatoteke[50];

    sprintf(imeDatoteke, "playlist_%d.txt", p.id);

    FILE* fp = fopen(imeDatoteke, "w");

    if (fp == NULL) {

        printf("Greska kod kreiranja playliste.\n");
        return;
    }

    fprintf(fp, "PLAYLISTA: %s\n", p.naziv);

    fclose(fp);

    FILE* popis = fopen("playliste.txt", "a");

    if (popis == NULL) {

        printf("Greska kod spremanja playliste.\n");
        return;
    }

    fprintf(popis, "%d;%s\n", p.id, p.naziv);

    fclose(popis);

    printf("\nPlaylista uspjesno kreirana.\n");
    printf("Dodijeljeni ID: %d\n", p.id);
}

void ispisiPlayliste() {

    FILE* fp = fopen("playliste.txt", "r");

    if (fp == NULL) {

        printf("Nema spremljenih playlista.\n");
        return;
    }

    PLAYLISTA p;

    int postojiPlaylista = 0;

    while (fscanf(fp, "%d;%99[^\n]\n", &p.id, p.naziv) == 2) { // ako se nigdje u datoteci ne pronadju id i naziv playliste (playlista ne postoji) -> ne postoji ni jedna playlista ˇ

        if (postojiPlaylista == 0) {

            printf("\n===== POSTOJECE PLAYLISTE =====\n");

            postojiPlaylista = 1;
        }

        printf("ID: %d | Naziv: %s\n", p.id, p.naziv);
    }

    if (postojiPlaylista == 0) {

        printf("Nema spremljenih playlista.\n");
    }

    fclose(fp);
}

void obrisiPlaylistu() {

    ispisiPlayliste();

    int trazeniID;

    printf("\nUnesite ID playliste za brisanje: ");
    scanf("%d", &trazeniID);

    FILE* fp = fopen("playliste.txt", "r");
    FILE* temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) {

        printf("Greska kod datoteka.\n");
        return;
    }

    PLAYLISTA p;

    while (fscanf(fp, "%d;%99[^\n]\n", &p.id, p.naziv) == 2) {

        if (p.id != trazeniID) {

            fprintf(temp, "%d;%s\n", p.id, p.naziv);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("playliste.txt");
    rename("temp.txt", "playliste.txt");

    char imeDatoteke[50];

    sprintf(imeDatoteke, "playlist_%d.txt", trazeniID);

    remove(imeDatoteke);

    printf("Playlista obrisana.\n");
}