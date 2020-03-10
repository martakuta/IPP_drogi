/** @file
 * Interfejs klasy przechowującej funkcje związane z tworzeniem i edytowaniem
 * dróg krajowych
 *
 * @author Marta Markocka <m.markocka@student.uw.edu.pl>
 */

#ifndef DROGI_ROUTES_H
#define DROGI_ROUTES_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "cities_and_roads.h"

typedef struct ListOfRoutes ListOfRoutes; /**< @struct ListOfRoutes cities_and_routes.h */
typedef struct Road Road; /**< @struct Road cities_and_routes.h */
typedef struct City City; /**< @struct City cities_and_routes.h */
typedef struct RoutesOnMap RoutesOnMap; /**< @struct RoutesOnMap cities_and_routes.h */
typedef struct Map Map; /**< @struct Map cities_and_routes.h */
typedef struct List List; /**< @struct List cities_and_routes.h */
typedef struct Way Way; /**< @struct Way cities_and_routes.h */

/** @brief Sprawdza czy przez miasto przechodzi dana droga krajowa.
 * Sprawdza dla kolejnych dróg należących do miasta czy przechodzi przez nie droga krajowa o zadanym numerze
 * @param city - wskaźnik na miasto
 * @param routeId - numer danej drogi krajowej
 * @return @p true jeśli przez miasto przechodzi zadana droga krajowa. Wartość @p false wpp.
 */
bool theCityHasRoute (City *city, unsigned routeId);

/** @brief Znajduje najkrótszą drogę z miasta początkowego do każdego z miast na mapie
 * Wyznacza długość najkrótszej drogi z miasta początkowego do każdego z miast na mapie.
 * Ignoruje przy tym możliwość przejścia przez miasta, przez które przechodzi droga krajowa
 * o zadanym numerze. Znalezione wartości zapisuje w strukturach poszczególnych miast. Jeśli
 * do miasta nie da się dotrzeć będzie w nim ustawiona wartość @p INT_MAX
 * @param map - wskaźnik na mapę
 * @param firstCity - wskaźnik na miasto początkowe
 * @param routeId - numer drogi krajowej, której należy unikać
 * @return Wartość @p true jeśli udało się znaleźć drogi do miast, wystąpił błąd alokacji pamięci.
 * Wartość @p false wpp.
 */
bool dijkstra (Map *map, City *firstCity, unsigned routeId);

/** @brief Dodaje drogę krajową do danej drogi
 * Dodaje numer drogi krajowej do listy dróg należącej do danej drogi
 * @param road - wskaźnik na drogę
 * @param routeId - numer drogi krajowej
 * @return @p true jeśli udało się zaalokować miejsce i dodać drogę krajową. Wartość @p false wpp.
 */
bool addRouteToRoad (Road *road, unsigned routeId);

/** @brief Dodaje drogę krajową do wszystkich dróg na ścieżce.
 * Dodaje numer drogi krajowej do każdej kolejnej drogi należącej do ścieżki.
 * @param way - wskaźnik na ścieżkę
 * @param routeId - numer drogi krajowej
 * @return @p true jeśli udało się zaalokować miejsce i dodać drogi krajowe. Wartość @p false wpp.
 */
bool addRouteToWay (Way **way, unsigned routeId);

/**@brief Dodaje do mapy nową drogę krajową.
 * Dodaje do mapy nową drogę krajową. Alokuje na nią pamięć na liście dróg krajowych na mapie i tworzy
 * nowy element listy.
 * @param map - wskaźnik na mapę
 * @param firstCity - wskaźnik na miasto początkowe nowej drogi krajowej
 * @param secondCity - wskaźnik na miasto końcowe nowej drogi krajowej
 * @param routeId - numer nowej drogi krajowej
 * @return @p true jeśli udało się zaalokować pamięć i dodać drogę. Wartość @p false wpp.
 */
bool addRouteToMap (Map *map, City *firstCity, City *secondCity, unsigned routeId);

/** @brief Usuwa z mapy drogę krajową o zadanym numerze
 * Wyszukuje na mapie drogę krajową o zadanym numerze i usuwa ją.
 * @param map - wskaźnik na mapę
 * @param routeId - numer drogi krajowej
 */
void deleteRouteFromMap (Map *map, unsigned routeId);

/**@brief Zwalnia pamięć zaalokowaną na ścieżkę.
 * Przchodzi przez kolejne elementy ścieżki i zwalnia zaalokowaną na nie pamięć.
 * @param way - wskaźnik na ścieżkę
 */
void freeTheWay (Way **way);

/** @brief Wyszukuje drogę krajową na mapie.
 * Na podstawie @p routeId wyszukuje na mapie drogę krajową o takim numerze
 * @param map - wskaźnik na mapę
 * @param routeId - numer drogi krajowej
 * @return wskaźnik na strukturę drogi krajowej na mapie, jeśli istnieje.
 * Wartość @p NULL wpp.
 */
RoutesOnMap *findTheRouteViaId (Map *map, unsigned routeId);

/** @brief Rekurencyjnie wyszukuje przebieg istniejącej drogi krajowej.
 * W każdym kolejnym wywołaniu rekurencyjnym wyszukuje wśród dróg należących do aktualnego miasta
 * te, przez które przebiega droga krajowa o zadanym numerze @p routeId. Następnie wywołuje się
 * dla miasta znajdującego się na drugim końcu znalezionej drogi. Kolejne drogi dodaje do ścieżki (Way).
 * Kończy wywoływanie gdy dotrze do zadanego miasta @p lastCity
 * @param routeId - numer szukanej drogi krajowej
 * @param previousCity - wskaźnik na poprzednio odwiedzone miasto (aby do niego nie zawrócić)
 * @param currentCity - wskaźnik na aktualnie rozpatrywane miasto
 * @param lastCity - wskaźnik na miasto, do którego należy dotrzeć
 * @return wskażnik na listę przechowującą dotychczas znalezioną ścieżkę.
 */
Way *findWayOfRoute (unsigned routeId, City *previousCity, City *currentCity, City *lastCity);

/** @brief Rekurencyjnie znajduje najlepszą ścieżkę między dwoma miastami
 * Rekurencyjnie znajduje najlepszą ścieżkę miedzy dwoma miastami. Najlepszą, czyli najkrótszą.
 * A jeśli jest więcej niż jedna najkrótsza wybiera z każdej rok budowy lub ostatniej naprawy najstarszego
 * odcinka i spośród wszystkich wybiera najmłodszy. Wówczas ścieżka, do której należy jest najlepszą ścieżką.
 * Omija miasta, których drogi należą do drogi krajowej o zadanym numerze.
 * @param beginningCity - wskaźnik na miasto początkowe
 * @param currentCity - wskaźnik na miasto aktualne, spośród jego dróg funkcja wywoła się rekurencyjnie dla tych,
 * które ze względu na swoją długośc i odległość miast na ich końcach od początku są potencjalnie najlepszymi.
 * @param previousWay - wskaźnik na dotychczas znalezioną drogę
 * @param howManyWays - wskaźnik na licznik ile dróg spośród dotychczas znalezionych jest najlepszych
 * @param routeId - numer omijanej drogi krajowej
 * @return
 */
Way *findTheBestWay(City *beginningCity, City *currentCity, Way *previousWay, int *howManyWays, unsigned routeId);

/** @brief Sprawdza czy da się zastąpić usuwaną drogę innymi dla wszystkich dróg krajowych.
 * Dla wszystkich dróg krajowych należących do usuwanej drogi sprawdza czy da się ją zastąpić inną ścieżką,
 * tak aby uzupełniany odcinek był możliwie najkrótszy, a droga krajowa nie wracała do miast w których już była.
 * @param map - wskaźnik na mapę
 * @param firstCity - wskaźnik na jedno z miast na końcach usuwanego odcinka
 * @param secondCity - wskaźnik na drugie z miast na końcach usuwanego odcinka
 * @param listOfRoutes - wskaźnik na listę dróg krajowych które należy uzupełnić
 * @param listOfWays - wskaźnik na wskaźnik na listę ścieżek, które w przypadku powodzenia zostaną dodane
 * @return @p true jeśli udało się znaleźć uzupełniające ścieżki dla wszystkich dróg krajowych z listy.
 * Wartość @p false gdy nie udało się uzupełnić choć jednej drogi krajowej.
 */
bool repairBreaksInRoutes(Map *map, City *firstCity, City *secondCity,
                          ListOfRoutes *listOfRoutes, ListOfWays **listOfWays);

/**Rozpatruje przypadek gdy oba końce drogi krajowej są równoodległe od nowego miasta
 * Podczas próby przedłużenia drogi krajowej rozpatruje przypadek gdy początek i koniec drogi krajowej
 * są równo oddalone od nowego miasta. Sprawdza wiek dróg, a gdy to także nie jest decydujące informuje,
 * że nie da się przedłużyć drogi.
 * @param map - wskaźnik na mapę
 * @param distanceFromCity1 - wskaźnik na odległość od miasta początkowego
 * @param distanceFromCity2 - wskaźnik na odległość od miasta końcowego
 * @param newCity - misto do którego należy przedłużyć drogę krajową
 * @param route - wskaźnik na wskaźnik na strukturę zadanej drogi krajowej na mapie
 * @param routeId - numer zadanej drogi krajowej
 * @param theNewestRoute - wskaźnik na wskaźnik na znalezioną finalnie ścieżkę
 * @param howManyWays - wskażnik na licznik liczby równocześnie najlepszych ścieżek
 * @return @p true jeśli udało się wybrać najlepszą drogę. Wartość @p false wpp.
 */
bool caseTheSameDistancesExtend (Map *map, int *distanceFromCity1, int *distanceFromCity2, City *newCity,
                                 RoutesOnMap **route, unsigned routeId, Way **theNewestRoute, int *howManyWays);

/**@brief Sprawdza czy istnieje więcej niż jedna najlepsza ścieżka.
 * Podczas próby przedłużenia drogi krajowej sprawdza czy istnieje więcej niż jedna najlepsza
 * (tj. najkrótsza, a następnie najnowsza) ścieżka. Jeśli istnieje więcej niż jedna zwalnia pamięć
 * znalezionych ścieżek.
 * @param map - wskaźnik na mapę
 * @param distanceFromCity1 - odległość nowego miasta od początku drogi krajowej
 * @param distanceFromCity2 - odległość nowego miasta od końca drogi krajowej
 * @param newCity - wskaźnik na nowe miasto
 * @param route - wskaźnik na wskaźnik na strukturę danej drogi krajowej
 * @param routeId - numer danej drogi krajowej
 * @param theNewestRoute - wskaźnik na wskaźnik na znalezioną najlepszą ścieżkę
 * @param howManyWays - wskaźnik na licznik liczby równocześnie najlepszych ścieżek
 * @return @p true jeśli udało się znaleźć najlepszą ścieżkę. Wartość @p false jeśli nie da się
 * jednoznacznie wyznczyć najlepszej ścieżki.
 */
bool checkCaseMoreThanOneWayExtend (Map *map, int distanceFromCity1, int distanceFromCity2, City *newCity,
                                    RoutesOnMap **route, unsigned routeId, Way **theNewestRoute,  int howManyWays);

/**@ Cofa zmiany i zwalnia roboczą pamieć jeśli nie udało się usunać drogi.
 * Jeśli nie udało się usunąć drogi zwalnia pamięć wszystkich ścieżek oraz przywraca usuwanej
 * drodze jej długość (uprzednio zastąpionej przez INT_MAX)
 * @param listOfWays - wskaźnik na wskaźnik na listę ścieżek
 * @param road1 - wskaźnik na drogę w jedną stronę
 * @param road2 - wskaźnik na drogę w drugą stronę
 * @param length1 - długość drogi w jedną stronę
 * @param length2 - długość drogi w drugą stronę
 * @return @p false
 */
bool undoRemoveChanges (ListOfWays **listOfWays, Road *road1, Road *road2,
                        unsigned length1, unsigned length2);

/**@brief Realizuje usunięcie drogi kiedy wiadomo, że jest ono poprawne.
 * Kiedy wiadomo że usunięcie drogi jest poprawne usuwa ją, wpisuje do struktur nowe ścieżki dróg krajowych.
 * Zwalnia pamięć z listy ścieżek. Zwalnia pamięć z list dróg krajowych w obie strony.
 * @param listOfWays - wskaźnik na wskaźnik na listę ścieżek
 * @param firstCity - miasto na jednym końcu usuwanej drogi
 * @param secondCity - miasto na drugim końcu usuwanej drogi
 * @param road1 - wskaźnik na wskaźnik na usuwaną drogę w jedną stronę
 * @param road2 - wskaźnik na wskaźnik na usuwaną drogę w drugą stronę
 * @param firstRoadId - indeks w tablicy dróg miasta pierwszego pod którym znajduje się usuwana droga
 * @param secondRoadId - indeks w tablicy dróg miasta drugiego pod którym znajduje się usuwana droga
 * @param listOfRoutes1 - lista dróg krajowych w jedną stronę
 * @param listOfRoutes2 - lista dróg krajowych w drugą stronę
 * @return @p true
 */
bool doRemoveChanges (ListOfWays **listOfWays, City *firstCity, City *secondCity,
                      Road **road1, Road **road2, int firstRoadId, int secondRoadId,
                      ListOfRoutes **listOfRoutes1, ListOfRoutes **listOfRoutes2);


#endif //DROGI_ROUTES_H
