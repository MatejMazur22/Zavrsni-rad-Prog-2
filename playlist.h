#ifndef PLAYLIST_H
#define PLAYLIST_H


#define MAX_NAZIV 100

#define CLEAR_BUFFER() while(getchar() != '\n');

extern const char* POPIS_PLAYLISTI;

typedef struct {

	int id;
	char naziv[MAX_NAZIV];

} PLAYLISTA;

void kreirajPlaylistu();
void ispisiPlayliste();
void obrisiPlaylistu();
int postojiLiPlaylista();
void preimenujPlaylistu();


#endif
