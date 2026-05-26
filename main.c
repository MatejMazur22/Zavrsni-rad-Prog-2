#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "playlist.h"
#include "pjesma.h"

int main() {


	int izbor;

	do {

		printf("\nPLAYLIST MENU:");
		printf("PLAYLISTE\n");
		printf("1. Kreiraj playlistu\n");
		printf("2. Ispisi sve playliste\n");
		printf("3. Obrisi playlistu\n");
		printf("4. Preimenuj playlistu\n");
		printf("\nPJESME:\n");
		printf("5. Dodaj pjesme\n");
		printf("6. Ispisi pjesme\n");
		printf("7. Obrisi pjesmu\n");
		printf("8. Prebaci pjesmu\n");
		printf("9. Sortiraj pjesme\n");
		printf("10. Pretrazi pjesmu\n");
		printf("11. Obrisi sve pjesme\n");
		printf("12. Preimenuj pjesmu\n");

		printf("0. Izlaz\n");

		printf("Unesite svoj odabir: ");
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
			preimenujPlaylistu();
			break;
		case 5:
			dodajPjesmu();
			break;
		case 6:

			if (!postojiLiPlaylista()) {

				printf("Nema spremljenih playlista.\n");
				break;

			}

			int ID_ispis;

			ispisiPlayliste();
			printf("Unesite ID playliste koju zelite ispisati: ");
			if (scanf("%d", &ID_ispis) != 1) {

				printf("Pogresan unos, pokusajte ponovno.");
				CLEAR_BUFFER();
				break;

			}

			ispisiPjesme(ID_ispis);
			break;

		case 7:

			obrisiPjesmu();
			break;

		case 8:

			prebaciPjesmu();
			break;

		case 9: {

			int id_sort;

			ispisiPlayliste();

			printf("Unesite ID playliste: ");
			if (scanf("%d", &id_sort) != 1) {

				printf("Pogresan unos!\n");
				CLEAR_BUFFER();
				break;

			}


			sortirajPjesme(id_sort);
			break;
		}
		case 10: {

			int id_pret;
			char trazeno[MAX_IME];

			ispisiPlayliste();

			printf("\nUnesite id playliste za pretrazivanje: ");
			if (scanf("%d", &id_pret) != 1) {

				printf("Pogresan unos!\n");
				CLEAR_BUFFER();
				break;

			}

			CLEAR_BUFFER();

			ispisiPjesme(id_pret);

			printf("Unesite naziv pjesme za pretrazivanje: ");
			fgets(trazeno, MAX_IME, stdin);

			trazeno[strcspn(trazeno, "\n")] = 0;

			pretraziPjesmu(id_pret, trazeno);
			break;
		}
		case 11:

			obrisiSvePjesme();
			break;

		case 12:

			preimenujPjesmu();
			break;

		case 0: 

			printf("Izlazak iz programa.\n");
			break;

		default: 
			printf("Nepostojeci izbor!\n");
			break;

		}

	} while (izbor != 0);

	return 0;


}