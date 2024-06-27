#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_tools.h"

#define NUM_CHARS 256
#define SIZE_INDEX_TABLE 100
#define BUFFER_LENGTH 200

typedef struct song Song;
typedef struct snode SNode;
typedef struct artist Artist;

struct song {
  Artist *artist;
  char *title;
  char *path;
  int index;
};

struct snode {
  SNode *next, *prev;
  Song *song;
};

struct artist {
  char *name;
  Artist *next;
  SNode *head, *tail;
};

void initilize();
void load(FILE *fp);
void save(FILE *fp);
Artist *create_artist_instance(char *name);
Song *create_song_instance(Artist *ptr_artist, char *title, char *path);
Artist *add_artist(char *name);
void add_song(char *artist, char *title, char *path);
void insert_to_index_directory(Song *ptr_song);
void insert_node(Artist *ptr_artist, SNode *ptr_snode);
Artist *find_artist(char *name);
void status();
void print_artist(Artist *p);
void print_song(Song *ptr_song);
void search_song(char *artist, char *title);
void search_song(char *artist);
SNode *find_snode(Artist *ptr_artist, char *title);
SNode *find_song(int index);
void play(int index);
void save_artist(Artist *p, FILE *fp);
void save_song(Song *ptr_song, FILE *fp);
void remove_song(int index);
void remove_snode(Artist *ptr_artist, SNode *ptr_snode);
void destory_song(Song *ptr_song);

#endif