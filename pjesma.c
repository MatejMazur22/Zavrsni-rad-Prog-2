#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pjesma.h"
#include "playlist.h"

void dodajPjesmu() {

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

    fgets(p.izvodac, MAX_IZVODAC, stdin);

    p.izvodac[strcspn(p.izvodac, "\n")] = 0;

    printf("Trajanje: ");
    scanf("%f", &p.trajanje);

    char imeDatoteke[50];

    sprintf(imeDatoteke, "playlist_%d.txt", id);

    FILE* fp = fopen(imeDatoteke, "a");

    if (fp == NULL) {

        printf("Playlista ne postoji.\n");
        return;
    }

    fprintf(fp,
        "%s;%s;%.2f\n",
        p.ime,
        p.izvodac,
        p.trajanje);

    fclose(fp);

    printf("Pjesma uspjesno dodana.\n");
}

void obrisiPjesmu() {

    int id;

    printf("ID playliste: ");
    scanf("%d", &id);

    getchar();

    char trazenaPjesma[MAX_IME];

    printf("Ime pjesme za brisanje: ");

    fgets(trazenaPjesma, MAX_IME, stdin);

    trazenaPjesma[strcspn(trazenaPjesma, "\n")] = 0;

    char imeDatoteke[50];

    sprintf(imeDatoteke, "playlist_%d.txt", id);

    FILE* fp = fopen(imeDatoteke, "r");
    FILE* temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) {

        printf("Greska kod datoteka.\n");
        return;
    }

    char linija[256];

    while (fgets(linija, sizeof(linija), fp)) {

        if (strstr(linija, trazenaPjesma) == NULL) {

            fputs(linija, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(imeDatoteke);
    rename("temp.txt", imeDatoteke);

    printf("Pjesma obrisana.\n");
}

void prebaciPjesmu() {

    int izvorID;
    int odredisteID;

    printf("Izvorna playlista ID: ");
    scanf("%d", &izvorID);

    printf("Nova playlista ID: ");
    scanf("%d", &odredisteID);

    getchar();

    char trazenaPjesma[MAX_IME];

    printf("Ime pjesme: ");

    fgets(trazenaPjesma, MAX_IME, stdin);

    trazenaPjesma[strcspn(trazenaPjesma, "\n")] = 0;

    char izvorDat[50];
    char odredDat[50];

    sprintf(izvorDat, "playlist_%d.txt", izvorID);
    sprintf(odredDat, "playlist_%d.txt", odredisteID);

    FILE* izvor = fopen(izvorDat, "r");
    FILE* odred = fopen(odredDat, "a");
    FILE* temp = fopen("temp.txt", "w");

    if (izvor == NULL || odred == NULL || temp == NULL) {

        printf("Greska kod datoteka.\n");
        return;
    }

    char linija[256];

    while (fgets(linija, sizeof(linija), izvor)) {

        if (strstr(linija, trazenaPjesma) != NULL) {

            fputs(linija, odred);
        }
        else {

            fputs(linija, temp);
        }
    }

    fclose(izvor);
    fclose(odred);
    fclose(temp);

    remove(izvorDat);
    rename("temp.txt", izvorDat);

    printf("Pjesma prebacena.\n");
}