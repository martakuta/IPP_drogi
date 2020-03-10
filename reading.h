/** @file
 * Interfejs klasy przechowującej funkcje potrzebne do wczytania danych
 * do programu z pliku zewnętrznego
 *
 * @author Marta Markocka <m.markocka@student.uw.edu.pl>
 */

#ifndef DROGI_READING_H
#define DROGI_READING_H

typedef struct Map Map; /**< @struct Map cities_and_routes.h */

/** @brief sprawdza czy polecenie addRoad ma poprawny format i je wywołuje
 * Sprawdza czy polecenie addRoad ma poprawny format tj. "addRoad;nazwa miasta;nazwa miasta;długość drogi;rok budowy"
 * Jeśli jest niepoprawne zwraca błąd. Jeśli jest poprawne wywołuje je.
 * @param map - wskaźnik na mapę, na której ma być wywołane polecenie
 * @param line - wskaźnik na napis, reprezentujący polecenie
 * @param lineLength - długośc napisu, reprezentującego polecenie
 * @return @p true jeśli format polecenia jest poprawny i udało się je wykonać. Wartość @p false wpp.
 */
bool isAddRoadCorrect (Map *map, char *line, int lineLength);

/** @brief sprawdza czy polecenie repairRoad ma poprawny format i je wywołuje
 * Sprawdza czy polecenie repairRoad ma poprawny format tj. "repairRoad;nazwa miasta;nazwa miasta;rok remontu"
 * Jeśli jest niepoprawne zwraca błąd. Jeśli jest poprawne wywołuje je.
 * @param map - wskaźnik na mapę, na której ma być wywołane polecenie
 * @param line - wskaźnik na napis, reprezentujący polecenie
 * @param lineLength - długośc napisu, reprezentującego polecenie
 * @return @p true jeśli format polecenia jest poprawny i udało się je wykonać. Wartość @p false wpp.
 */
bool isRepairRoadCorrect (Map *map, char *line, int lineLength);

/** @brief sprawdza czy polecenie getRouteDescription ma poprawny format i je wywołuje
 * Sprawdza czy polecenie getRouteDescription ma poprawny format tj. "getRouteDescription;numer drogi krajowej"
 * Jeśli jest niepoprawne zwraca błąd. Jeśli jest poprawne wywołuje je i wypisuje na standardowe wyjście wynik
 * wywołania funkcji getRouteDescription Funkcja nie sprawdza poprawności numeru drogi krajowej ani istnienia
 * drogi o danym numerze. Jeśli droga nie istnieje wypisana zostaje pusta linia.
 * @param map - wskaźnik na mapę, na której ma być wywołane polecenie
 * @param line - wskaźnik na napis, reprezentujący polecenie
 * @param lineLength - długośc napisu, reprezentującego polecenie
 * @return @p true jeśli format polecenia jest poprawny i udało się je wykonać. Wartość @p false wpp.
 */
bool isGetRouteDescriptionCorrect (Map *map, char *line, int lineLength);

/** @brief sprawdza czy polecenie tworzące nową drogę krajową ma poprawny format i je wywołuje
 * Sprawdza czy polecenie ma poprawny format tj. "numer drogi krajowej;nazwa miasta;długość drogi;
 * rok budowy lub ostatniego remontu;nazwa miasta;długość drogi;rok budowy lub ostatniego remontu;..;nazwa miasta"
 * Jeśli jakieś miasto lub odcinek drogi nie istnieje tworzy go. Jeśli odcinek drogi już istnieje, ale ma inną
 * długość lub późniejszy rok budowy lub ostatniego remontu zwraca błąd. Jeśli przebieg drogi jest poprawny
 * dokonuje zmian na mapie.
 * @param map - wskaźnik na mapę, na której ma zostać wykonane polecenie
 * @param line - wskaźnik na napis, reprezentujący polecenie
 * @param lineLength - długośc napisu, reprezentującego polecenie
 * @return @p true jeśli polecenie jest poprawne i udało się je wykonać. Wartość @p false wpp.
 */
bool isMakeNewRouteCorrect (Map *map, const char *line, int lineLength);

/** @brief w zależności od pierwszego znaku w linii rozpoznaje potencjalne polecenie lub błąd
 * Wczytuje pierwszy znak linii. Jeśli jest to jedna z liter 'a', 'r', 'g' lub cyfra przekierowuje
 * dalsze wczytywanie do odpowiednio funkcji @ref isAddRoadCorrect, @ref isRepairRoadCorrect,
 * @ref isGetRouteDescriptionCorrect, @ref isMakeNewRouteCorrect.
 * Jeśli polecenie jest niepoprawne lub zakończyło się błędem wypisuje na standardowe wyjście
 * diagnostyczne komunikat "ERROR" oraz numer linii, w której wystąpił błąd.
 * @param map - wskaźnik na mapę, na której ma być wykonane polecenie
 * @param line - wskaźnik na napis przechowujący zawartość aktualnej linii
 * @param lineLength - długość napisu pod wskaźnikiem @param line
 * @param numberOfLine - numer aktualnej linii
 */
void realizeLine (Map *map, char *line, int lineLength, int numberOfLine);

/** @brief wczytuje do końca linię będącą komentarzem
 * Wczytuje znaki aż do linukswowego znaku końca linii ('\n') i ignoruje je, bo są komentarzem.
 */
void readComment();

/** @brief wczytuje linię i zleca jej wykonanie
 * Wczytuje znaki aż do linukswowego znaku końca linii ('\n') i łączy je w polecenie.
 * Jeśli nie udało się zaalokować pamięci na wczytanie polecenia wypisuje na standardowe wyjście
 * diagnostyczne komunikat "ERROR" oraz numer linii, w której wystąpił błąd.
 * @param map - wskaźnik na mapę, na której ma zostać wykonane polecenie
 * @param numberOfLine - numer wczytywanej linii
 */
void readLine (Map *map, int numberOfLine);

/** @brief wczytuje plik wejsciowy i zleca realizację poleceń tam zawartych
 * Funkcja tworzy mapę, na której będzie operować, a następnie
 * wczytuje każdą kolejną linię. Jeśli zaczyna się od znaku '#' czyli jest komentarzem lub jest pusta
 * wczytuje ją i ignoruje. Wpp. zleca wykonenie polecenia.
 */
void readFile ();

#endif //DROGI_READING_H
