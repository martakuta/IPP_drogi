#include "cities_and_roads.h"

#define ERROR 0
#define CORRECT 1
#define NEW_SIZE 2
#define EXIST 1
#define NOT_EXIST 0
#define EMPTY 1
#define NOT_EMPTY 0
#define NOT_VISITED 0
#define VISITED 1
#define ROAD_NOT_EXIST -1

/*void printMap (Map *m) {

    printf("na mapie jest tyle miast: %d\n", m->numberOfCities);

    for (int j=0; j<m->numberOfCities; j++) {

        City *city = m->tabOfCities[j];
        printf("\n%s:\n", city->name);
        for (int i=0; i<city->numberOfRoads; i++) {
            Road *road = city->tabOfRoads[i];

            printf("%s, ", ((road)->secondCity)->name);
            printf("%d, ", (road)->length);
            printf("%d, ", (road)->builtYear);
            printf("%d, ", (road)->repairYear);

            ListOfRoutes *copyOfListOfRoutes = road->listOfRoutes;
            printf("Routes: ");
            while (copyOfListOfRoutes != NULL) {
                printf("%u, ", copyOfListOfRoutes->routeId);
                copyOfListOfRoutes = copyOfListOfRoutes->next;
            }
            printf("\n");
        }
    }
}*/

City *newCity(const char *cityName) {

    City *city = malloc(sizeof(City));
    if (city == NULL) return NULL;

    city->name = malloc(strlen(cityName)+1);
    if (city->name == NULL) return NULL;
    strcpy((char*)city->name, cityName);

    city->numberOfRoads = 0;
    city->allocatedRoads = 0;
    city->tabOfRoads = NULL;
    city->visited = NOT_VISITED;
    city->distance = INT_MAX;
    return city;
}

bool correctSizeOfCitiesTab (Map *map) {

    if (map->allocatedCities <= map->numberOfCities) {
        map->allocatedCities = NEW_SIZE*(map->allocatedCities)+1;
        map->tabOfCities = realloc(map->tabOfCities, (map->allocatedCities)*sizeof(City));
    }

    if (map->tabOfCities == NULL)
        return ERROR;
    else
        return CORRECT;
}

bool createCity (Map *map, City **city, const char *cityName) {

    if (!correctSizeOfCitiesTab(map)) return ERROR;
    *city = newCity(cityName);
    if (*city == NULL) return ERROR;

    map->tabOfCities[map->numberOfCities] = *city;
    map->numberOfCities ++;

    return CORRECT;
}

bool findCity(Map *map, const char *cityName, City **city) {

    for (int i=0; i < map->numberOfCities; i++) {

        City *anotherCity = map->tabOfCities[i];
        if (strcmp(anotherCity->name, cityName) == 0) {
            *city = anotherCity;
            return EXIST;
        }
    }
    return NOT_EXIST;
}

bool correctSizeOfRoadsTab (City *city) {

    if (city->allocatedRoads <= city->numberOfRoads) {
        city->allocatedRoads = NEW_SIZE*(city->allocatedRoads)+1;
        city->tabOfRoads = realloc(city->tabOfRoads, (city->allocatedRoads)*sizeof(Road));
    }

    if (city->tabOfRoads == NULL)
        return ERROR;
    else
        return CORRECT;
}

bool createRoad (City *firstCity, City *secondCity, unsigned length, int builtYear) {

    Road *newRoad = malloc(sizeof(Road));
    if (newRoad == NULL) return ERROR;
    newRoad->secondCity = secondCity;
    newRoad->length = length;
    newRoad->builtYear = builtYear;
    newRoad->repairYear = 0;
    newRoad->alterRoad = NULL;
    newRoad->listOfRoutes = NULL;

    if (correctSizeOfRoadsTab(firstCity) == ERROR) return ERROR;
    int howManyRoads = firstCity->numberOfRoads;
    firstCity->tabOfRoads[howManyRoads] = newRoad;
    (firstCity->numberOfRoads)++;

    return CORRECT;
}

int findRoad (City *firstCity, City *secondCity) {

    for (int i=0; i<firstCity->numberOfRoads; i++) {
        Road *road = firstCity->tabOfRoads[i];

        if (secondCity == road->secondCity)
            return i;
    }
    return ROAD_NOT_EXIST;
}

char *reverseNumberTab (int number, int *finalPosition) {

    char *reverseNumber = malloc(50* sizeof(char));
    if (reverseNumber == NULL) return NULL;
    int position = 0;

    bool minus = false;
    if (number < 0) {
        minus = true;
        number = -number;
    }

    while (number > 0) {
        char sign = (char)(number%10) + (char)('0');
        reverseNumber[position] = sign;
        position++;
        number /= 10;
    }

    if (minus) {
        reverseNumber[position] = '-';
        position++;
    }
    *finalPosition = position;
    return reverseNumber;
}

const char *convertNumberToString (int number) {

    int position = 0;
    char *reverseNumber = reverseNumberTab(number, &position);
    if (reverseNumber == NULL) return NULL;

    char *correctNumber = malloc((position+1)*sizeof(char));
    if (correctNumber == NULL) return NULL;

    for (int i=0; i<position; i++)
        correctNumber[i] = reverseNumber[position-1-i];
    correctNumber[position] = '\0';
    free((void*)reverseNumber);

    return correctNumber;
}

char *addBeginningToDescription (RoutesOnMap *route, unsigned routeId, const char **semicolon) {

    char *description = malloc(100*sizeof(char));
    if (description == NULL) return NULL;
    description[0] = '\0';
    const char *routeIdString = convertNumberToString((int)(routeId));
    strcat(description, routeIdString);
    free((void*)routeIdString);

    char *help = ";";
    *semicolon = help;
    strcat(description, *semicolon);

    strcat(description, (route->firstCity)->name);
    return description;
}

void addRoadDetailsToDescription (Road *road, char **description, const char *semicolon) {

    strcat(*description, semicolon);
    const char *lengthString = convertNumberToString((int)(road->length));
    strcat(*description, lengthString);
    free((void*)lengthString);

    strcat(*description, semicolon);
    const char *yearString;
    if (road->repairYear != 0)
        yearString = convertNumberToString(road->repairYear);
    else
        yearString = convertNumberToString(road->builtYear);
    strcat(*description, yearString);
    free((void*)yearString);

    strcat(*description, semicolon);
    strcat(*description, (road->secondCity)->name);
}

