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
    // se usa strcmp para comparar dos strings y si son iguales retorna
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

  Pair** old_buckets = map->buckets;
  long old_capacity = map->capacity;

  map->capacity = map->capacity * 2;

  map->size = 0;
  
  for(long i = 0; i<old_capacity;i++){
    if(old_buckets[i]!=NULL && old_buckets[i]->key!=NULL){
      insertMap(map, old_buckets[i]->key,old_buckets[i]->value);
    }
  }
  
  
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

void eraseMap(HashMap *map, char *key) {
  // se hace basicamente lo mismo que en los ejercicios anterioes, crear un
  // contador con la funcion hash, e ir recorriendo el areglo del mapa hasta
  // encontrar el elemento con la key correcta
  long i = hash(key, map->capacity);

  while (map->buckets[i] != NULL) {

    if (map->buckets[i]->key != NULL &&
        strcmp(map->buckets[i]->key, key) == 0) {
      map->buckets[i]->key = NULL;
      // se disminuye el tamaño del mapa
      map->size--;
      return;
    }
    i = (i + 1) % map->capacity;
  }
}

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

Pair *firstMap(HashMap *map) {
  // avanzando desde la primera clave posible(0), hasta cuando se encuentre el
  // primer pair valido
  for (long i = 0; i < map->capacity; i++) {
    // se busca primer pair que sea no nulo y que tenga key no nula
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
      // se acutaliza el current y se retorna el mismo
      map->current = i;
      return (map->buckets[i]);
    }
  }
  // se deja en caso de que no haya pair valido
  return NULL;
}

Pair *nextMap(HashMap *map) {
  // se hace lo mismo que con firstmap, pero en vez de empezar i desde cero se
  // comienza desde el current
  for (long i = map->current + 1; i < map->capacity; i++) {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
      map->current = i;
      return (map->buckets[i]);
    }
  }
  return NULL;
}
