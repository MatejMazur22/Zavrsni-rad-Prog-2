#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "playlist.h"
#include "pjesma.h"

int main() {

    int izbor;

    do {

        printf("\nPLAYLIST MENU\n");

        printf("1. Kreiraj playlistu\n");
        printf("2. Ispisi playliste\n");
        printf("3. Obrisi playlistu\n");
        printf("4. Dodaj pjesmu\n");
        printf("5. Obrisi pjesmu\n");
        printf("6. Prebaci pjesmu\n");
        printf("0. Izlaz\n");

        printf("Odabir: ");
        scanf("%d", &izbor);

        switch (izbor) {

        case 1:
            kreirajPlaylistu();
            break;

        case 2:
            ispisiPlayliste();
            break;

        case 3:
            obrisiPlaylistu();
            break;

        case 4:
            dodajPjesmu();
            break;

        case 5:
            obrisiPjesmu();
            break;

        case 6:
            prebaciPjesmu();
            break;
        }

    } while (izbor != 0);

    return 0;
}