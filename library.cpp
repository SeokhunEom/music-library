#include "library.h"

#define NUM_CHARS 256
#define SIZE_INDEX_TABLE 100
#define BUFFER_LENGTH 200

Artist *artist_directory[NUM_CHARS];
SNode *index_directory[SIZE_INDEX_TABLE];
int num_index = 0;

void initilize() {
  for (int i = 0; i < NUM_CHARS; i++) {
    artist_directory[i] = NULL;
  }

  for (int i = 0; i < SIZE_INDEX_TABLE; i++) {
    index_directory[i] = NULL;
  }
}

void load(FILE *fp) {
  char buffer[BUFFER_LENGTH];
  char *name, *title, *path;

  while (1) {
    if (read_line(fp, buffer, BUFFER_LENGTH) <= 0) {
      break;
    }

    name = strtok(buffer, "#");
    if (strcmp(name, " ") == 0) {
      name = NULL;
    } else {
      name = strdup(name);
    }

    title = strtok(NULL, "#");
    if (strcmp(title, " ") == 0) {
      title = NULL;
    } else {
      title = strdup(title);
    }

    path = strtok(NULL, "#");
    if (strcmp(path, " ") == 0) {
      path = NULL;
    } else {
      path = strdup(path);
    }

    add_song(name, title, path);
  }
}

void save(FILE *fp) {
  for (int i = 0; i < NUM_CHARS; i++) {
    Artist *p = artist_directory[i];
    while (p != NULL) {
      save_artist(p, fp);
      p = p->next;
    }
  }
}

Artist *create_artist_instance(char *name) {
  Artist *ptr_artist = (Artist *)malloc(sizeof(Artist));
  ptr_artist->name = name;
  ptr_artist->head = NULL;
  ptr_artist->tail = NULL;
  ptr_artist->next = NULL;
  return ptr_artist;
}

Song *create_song_instance(Artist *ptr_artist, char *title, char *path) {
  Song *ptr_song = (Song *)malloc(sizeof(Song));
  ptr_song->artist = ptr_artist;
  ptr_song->title = title;
  ptr_song->path = path;
  ptr_song->index = num_index++;

  return ptr_song;
}

Artist *add_artist(char *name) {
  Artist *ptr_artist = create_artist_instance(name);
  Artist *p = artist_directory[(unsigned char)name[0]];
  Artist *q = NULL;

  while (p != NULL && strcmp(p->name, name) < 0) {
    q = p;
    p = p->next;
  }

  if (p == NULL && q == NULL) {
    artist_directory[(unsigned char)name[0]] = ptr_artist;
  } else if (q == NULL) {
    ptr_artist->next = artist_directory[(unsigned char)name[0]];
    artist_directory[(unsigned char)name[0]] = ptr_artist;
  } else {
    ptr_artist->next = p;
    q->next = ptr_artist;
  }

  return ptr_artist;
}

void add_song(char *artist, char *title, char *path) {
  Artist *ptr_artist = find_artist(artist);
  if (ptr_artist == NULL) {
    ptr_artist = add_artist(artist);
  }

  Song *ptr_song = create_song_instance(ptr_artist, title, path);
  SNode *ptr_snode = (SNode *)malloc(sizeof(SNode));
  ptr_snode->song = ptr_song;
  ptr_snode->next = NULL;
  ptr_snode->prev = NULL;

  insert_node(ptr_artist, ptr_snode);
  insert_to_index_directory(ptr_song);
}

void insert_to_index_directory(Song *ptr_song) {
  SNode *ptr_snode = (SNode *)malloc(sizeof(SNode));
  ptr_snode->song = ptr_song;
  ptr_snode->next = NULL;
  ptr_snode->prev = NULL;  // unused

  int index = ptr_song->index % SIZE_INDEX_TABLE;

  SNode *p = index_directory[index];
  SNode *q = NULL;
  while (p != NULL && strcmp(p->song->title, ptr_snode->song->title) < 0) {
    q = p;
    p = p->next;
  }
  if (q == NULL) {
    ptr_snode->next = p;
    index_directory[index] = ptr_snode;
  } else {
    ptr_snode->next = p;
    q->next = ptr_snode;
  }
}

void insert_node(Artist *ptr_artist, SNode *ptr_snode) {
  SNode *p = ptr_artist->head;

  while (p != NULL && strcmp(p->song->title, ptr_snode->song->title) < 0) {
    p = p->next;
  }

  if (ptr_artist->head == NULL) {
    ptr_artist->head = ptr_snode;
    ptr_artist->tail = ptr_snode;
  } else if (p == ptr_artist->head) {
    ptr_snode->next = ptr_artist->head;
    ptr_artist->head->prev = ptr_snode;
    ptr_artist->head = ptr_snode;
  } else if (p == NULL) {
    ptr_snode->prev = ptr_artist->tail;
    ptr_artist->tail->next = ptr_snode;
    ptr_artist->tail = ptr_snode;
  } else {
    ptr_snode->next = p;
    ptr_snode->prev = p->prev;
    p->prev->next = ptr_snode;
    p->prev = ptr_snode;
  }
}

Artist *find_artist(char *name) {
  Artist *p = artist_directory[(unsigned char)name[0]];
  while (p != NULL && strcmp(p->name, name) < 0) {
    p = p->next;
  }

  if (p != NULL && strcmp(p->name, name) == 0) {
    return p;
  } else {
    return NULL;
  }
}

void status() {
  for (int i = 0; i < NUM_CHARS; i++) {
    Artist *p = artist_directory[i];
    while (p != NULL) {
      print_artist(p);
      p = p->next;
    }
  }
}

void print_artist(Artist *p) {
  printf("%s\n", p->name);
  SNode *ptr_snode = p->head;
  while (ptr_snode != NULL) {
    print_song(ptr_snode->song);
    ptr_snode = ptr_snode->next;
  }
}

void print_song(Song *ptr_song) {
  printf("  %d: %s, %s\n", ptr_song->index, ptr_song->title, ptr_song->path);
}

void search_song(char *artist, char *title) {
  Artist *ptr_artist = find_artist(artist);
  if (ptr_artist == NULL) {
    printf("No such artist exists.\n");
    return;
  }

  SNode *ptr_snode = find_snode(ptr_artist, title);
  if (ptr_snode != NULL) {
    printf("Found:\n");
    print_song(ptr_snode->song);
  } else {
    printf("No such song exists.\n");
    return;
  }
}

void search_song(char *artist) {
  Artist *ptr_artist = find_artist(artist);
  if (ptr_artist == NULL) {
    printf("No such artist exists.\n");
    return;
  }
  printf("Found:\n");
  print_artist(ptr_artist);
}

SNode *find_snode(Artist *ptr_artist, char *title) {
  SNode *ptr_snode = ptr_artist->head;
  while (ptr_snode != NULL && strcmp(ptr_snode->song->title, title) < 0) {
    ptr_snode = ptr_snode->next;
  }

  if (ptr_snode != NULL && strcmp(ptr_snode->song->title, title) == 0) {
    return ptr_snode;
  } else {
    return NULL;
  }
}

SNode *find_song(int index) {
  SNode *ptr_snode = index_directory[index % SIZE_INDEX_TABLE];
  while (ptr_snode != NULL && ptr_snode->song->index != index) {
    ptr_snode = ptr_snode->next;
  }
  return ptr_snode;
}

void play(int index) {
  SNode *ptr_snode = find_song(index);
  if (ptr_snode == NULL) {
    printf("No such song exists.\n");
    return;
  }
  printf("Found the song: %s\n", ptr_snode->song->title);
}

void save_artist(Artist *p, FILE *fp) {
  SNode *ptr_snode = p->head;
  while (ptr_snode != NULL) {
    save_song(ptr_snode->song, fp);
    ptr_snode = ptr_snode->next;
  }
}

void save_song(Song *ptr_song, FILE *fp) {
  if (ptr_song->artist->name != NULL) {
    fprintf(fp, "%s#", ptr_song->artist->name);
  } else {
    fprintf(fp, " #");
  }

  if (ptr_song->title != NULL) {
    fprintf(fp, "%s#", ptr_song->title);
  } else {
    fprintf(fp, " #");
  }

  if (ptr_song->path != NULL) {
    fprintf(fp, "%s#\n", ptr_song->path);
  } else {
    fprintf(fp, " #\n");
  }
}

void remove_song(int index) {
  SNode *p = index_directory[index % SIZE_INDEX_TABLE];
  SNode *q = NULL;
  while (p != NULL && p->song->index != index) {
    q = p;
    p = p->next;
  }

  if (p == NULL) {
    printf("No such song exists.\n");
    return;
  }

  Song *ptr_song = p->song;
  if (q == NULL) {
    index_directory[index % SIZE_INDEX_TABLE] = p->next;
  } else {
    q->next = p->next;
  }
  free(p);

  Artist *ptr_artist = ptr_song->artist;
  SNode *ptr_snode = find_snode(ptr_artist, ptr_song->title);
  if (ptr_snode == NULL) {
    printf("Not found snode - something wrong\n");
    return;
  }
  remove_snode(ptr_artist, ptr_snode);
  destory_song(ptr_song);
}

void remove_snode(Artist *ptr_artist, SNode *ptr_snode) {
  SNode *first = ptr_artist->head;
  SNode *last = ptr_artist->tail;

  if (first == ptr_snode && last == ptr_snode) {
    ptr_artist->head = NULL;
    ptr_artist->tail = NULL;
  } else if (first == ptr_snode) {
    ptr_artist->head = ptr_snode->next;
    ptr_snode->next->prev = NULL;
  } else if (last == ptr_snode) {
    ptr_artist->tail = ptr_snode->prev;
    ptr_snode->prev->next = NULL;
  } else {
    ptr_snode->prev->next = ptr_snode->next;
    ptr_snode->next->prev = ptr_snode->prev;
  }
  free(ptr_snode);
}

void destory_song(Song *ptr_song) {
  if (ptr_song->title != NULL) {
    free(ptr_song->title);
  }
  if (ptr_song->path != NULL) {
    free(ptr_song->path);
  }
  free(ptr_song);
}