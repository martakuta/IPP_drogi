/** @file
 * Interfejs klasy przechowującej struktury oraz funcje pomocnicze do tworzenia
 * i naprawiania dróg oraz opisywania dróg krajowych
 *
 * @author Marta Markocka <m.markocka@student.uw.edu.pl>
 */

#ifndef DROGI_CITIES_AND_ROADS_H
#define DROGI_CITIES_AND_ROADS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/**
 * Struktura przechowująca miasto
 */
typedef struct City City;

/**
 * Struktura przechowująca listę identyfikatorów dróg krajowych, które przechodzą przez miasto
 */
typedef struct ListOfRoutes {
    unsigned routeId; /**< identyfikator drogi krajowej */
    struct ListOfRoutes *next; /**< wskaźnik na następną drogę krajową na liście */
} ListOfRoutes;

/**
 * Struktura przechowująca drogę. Droga łączy dwa miasta i jest jednokierunkowa, ale zawsze
 * trzyma wskaźnik na swoją drogę do pary, prowadzącą w drugą stronę.
 */
typedef struct Road {
    City *secondCity; /**< wskaźnik na miasto do którego prowadzi droga */
    unsigned length; /**< długość drogi krajowej */
    int builtYear; /**< rok budowy drogi */
    int repairYear; /**< rok ostatniej naprawy drogi */
    struct Road *alterRoad; /**< wskaźnik na drogę powrotną */
    ListOfRoutes *listOfRoutes; /**< wskaźnik na listę dróg krajowych przechodzących przez tę drogę */
} Road;

/**
 * Struktura przechowująca miasto
 */
typedef struct City {
    const char *name; /**< nazwa miasta */
    int numberOfRoads; /**< liczba dróg wychodzących z tego miasta */
    int allocatedRoads; /**< liczba zaalokowanych miejsc w tablicy dróg */
    Road **tabOfRoads; /**< wskaźnik na tablicę dróg prowadzących z tego miasta */
    bool visited; /**< robocze pole mówiące o tym czy miasto było odwiedzone */
    int distance; /**< robocze pole mówiące o odległości miasta od innego, wybranego */
} City;

/**
 * Struktura przechowująca listę dróg krajowych na mapie
 */
typedef struct RoutesOnMap {
    unsigned routeId; /**< identyfikator drogi krajowej */
    City *firstCity; /**< wskaźnik na miasto na początku drogi krajowej */
    City *secondCity; /**< wskaźnik na miasto na końcu drogi krajowej */
    struct RoutesOnMap *next; /**< wskaźnik na następny element listy dróg krajowych na mapie */
} RoutesOnMap;

/**
 * Struktura przechowująca mapę miast i dróg krajowych
 */
typedef struct Map {
    int numberOfCities; /**< liczba miast na mapie */
    int allocatedCities; /**< liczba zaalokowanych miejsc z tablicy miast */
    City **tabOfCities; /**< wskaźnik na tablicę miast na mapie istniejących na mapie */
    RoutesOnMap *routesOnMap; /**< wskaźnik na listę dróg krajowych istniejących na mapie */
} Map;

/**
 * Struktura przechowująca listę miast
 */
typedef struct List {
    City *city; /**< wskaźnik na miasto na liście */
    struct List *next; /**< wskaźnik na następny element listy */
} List;

/**
 * Struktura przechowująca listę dróg, czyli ścieżkę
 */
typedef struct Way {
    Road *road; /**< wskaźnik na drogę na liście */
    struct Way *next; /**< wskaźnik na następny element na liście */
} Way;

/**
 * Struktura przechowująca listę ścieżek
 */
typedef struct ListOfWays {
    Way *way; /**< wskaźnik na ścieżkę na liście */
    unsigned routeId; /**< numer drogi krajowej która prowadzi przez daną ścieżkę */
    struct ListOfWays *next; /**< wskaźnik na następny element na liście */
} ListOfWays;

/** @brief Tworzy nowe miasto i dodaje je do mapy.
 * Sprawdza czy jest wystarczająco dużo miejsca na mapie na nowe miasto.
 * Tworzy miasto i dodaje wskaźnik na nie do mapy.
 * @param map - wskaźnik na mapę
 * @param city - wskaźnik na wskaźnik na miasto, które jest tworzone
 * @param cityName - wskaźnik na napis reprezentujący nazwę miasta
 * @return wartość @p true jeśli udało się zaalokować pamięć i stworzyć miasto.
 * Wartość @p false wpp.
 */
bool createCity (Map *map, City **city, const char *cityName);

/** @brief Wyszukuje miasto o podanej nazwie
 * Na podstawie podanej nazwy miasta wyszukuje je w tablicy miast na mapie.
 * Przypisuje je do @p city.
 * @param map - wskaźnik na mapę
 * @param city - wskaźnik na wskaźnik na miasto
 * @param cityName - wskaźnik na napis reprezentujący nazwę miasta
 * @return wartość @p true jeśli miasto istnieje.
 * Wartość @p false wpp.
 */
bool findCity(Map *map, const char *cityName, City **city);

/** @brief Tworzy nową drogę i dodaje ją do mapy.
 * Sprawdza czy tablica dróg w mieście ma wystarczająco dużo miejsca na nową drogę.
 * Tworzy drogę i dodaje wskaźnik na nią do miasta.
 * @param firstCity - wskaźnik na miasto na początku drogi
 * @param secondCity - wskaźnik na miasto na końcu drogi
 * @param length - długość drogi
 * @param builtYear - rok budowy drogi
 * @return wartość @p true jeśli udało się zaalokować pamięć i stworzyć drogę.
 * Wartość @p false wpp.
 */
bool createRoad (City *firstCity, City *secondCity, unsigned length, int builtYear);

/** @brief Wyszukuje drogę o podanej nazwie
 * Na podstawie podanych miast na końcu i na początku drogi w tablicy dróg należącej
 * do początkowego miasta znajduje drogę prowadzącą z @p firstCity do @p secondCity.
 * @param firstCity - wskaźnik na miasto na początku drogi
 * @param secondCity - wskaźnik na miasto na końcu drogi
 * @return numer indeksu w tablicy dróg początkowego miasta, pod którym znajduje się
 * szukana droga. Wartość @p -1 jeśli droga nie isnieje
 */
int findRoad (City *firstCity, City *secondCity);

/** @brief Tworzy początek opisu drogi krajowej.
 * Alokuje pamięć na opis drogi krajowej. Dodaje do niego numer drogi krajowej, przekonwertowny
 * na napis oraz nazwę miasta, w którym zaczyna się droga krajowa. Tworzy napis reprezentujący średnik.
 * @param route - wskaźnik na strukturę drogi krajowej na mapie
 * @param routeId - numer drogi krajowej
 * @param semicolon - wskaźnik na wskaźnik na napis reprezentujący średnik
 * @return napis zawierający początek opisu drogi krajowej jeśli udało się zaalokować pamięć i go stworzyć.
 * Wartość @p NULL wpp.
 */
char *addBeginningToDescription (RoutesOnMap *route, unsigned routeId, const char **semicolon);

/** @brief Dodaje kolejną drogę do opisu.
 * Dodaje szczegółowe informacje na temat drogi do opisu.
 * @param road - wskaźnik na opisywaną drogę
 * @param description - wskaźnik na wskaźnik na edytowany przez funkcję opis drogi krajowej
 * @param semicolon - wskaźnik na napis reprezentujący średnik
 */
void addRoadDetailsToDescription (Road *road, char **description, const char *semicolon);


#endif //DROGI_CITIES_AND_ROADS_H
