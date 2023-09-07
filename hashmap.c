#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
  Pair **buckets;
  long size;     // cantidad de datos/pairs en la tabla
  long capacity; // capacidad de la tabla
  long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
  Pair *new = (Pair *)malloc(sizeof(Pair));
  new->key = key;
  new->value = value;
  return new;
}

long hash(char *key, long capacity) {
  unsigned long hash = 0;
  char *ptr;
  for (ptr = key; *ptr != '\0'; ptr++) {
    hash += hash * 32 + tolower(*ptr);
  }
  return hash % capacity;
}

int is_equal(void *key1, void *key2) {
  if (key1 == NULL || key2 == NULL)
    return 0;
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

void insertMap(HashMap *map, char *key, void *value) {
  long i = hash(key, map->capacity);
  // obtener un contador para moverse por el arreglo, usando la funcion hash

  // while avanza hasta encontrar espacio disponible
  while (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
    // se usa strcmp para comparar dos strings y si son iguales retorna 0
    if (strcmp(map->buckets[i]->key, key) == 0) {
      return;
    }
    i = (i + 1) % map->capacity;
  }
  // luego de hallarse el espacio, se ingresan los valores a un par nuevo
  Pair *newPair = createPair(key, value);
  map->buckets[i] = newPair;
  // y se ajustan las variables del mapa
  map->size++;
  map->current = i;
}

void enlarge(HashMap *map) {
  enlarge_called = 1; // no borrar (testing purposes)
}

HashMap *createMap(long capacity) {
  // reservar memoria para el mapa y los buckets
  HashMap *newMap = (HashMap *)malloc(sizeof(HashMap));
  newMap->buckets = (Pair **)malloc(sizeof(Pair *) * capacity);
  // inicializar variables dentro del mapa
  newMap->size = 0;
  newMap->capacity = capacity;
  newMap->current = -1;

  return newMap;
}

void eraseMap(HashMap *map, char *key) {}

Pair *searchMap(HashMap *map, char *key) {
  // se hace casi lo mismo que en insert
  long i = hash(key, map->capacity);
  // en el while cuando vemos que las kkeys son la misma, se detiene el while
  while (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
    if (strcmp(map->buckets[i]->key, key) == 0) {
      break;
    }
    i = (i + 1) % map->capacity;
  }
  // y solo basta actualizar el current y retornar el elemento buscado
  map->current = i;

  return map->buckets[i];
}

Pair *firstMap(HashMap *map) { return NULL; }

Pair *nextMap(HashMap *map) { return NULL; }
