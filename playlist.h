#ifndef PLAYLIST_H
#define PLAYLIST_H

#define MAX_NAZIV 100

typedef struct {

    int id;
    char naziv[MAX_NAZIV];

} PLAYLISTA;

void kreirajPlaylistu();
void ispisiPlayliste();
void obrisiPlaylistu();

#endif