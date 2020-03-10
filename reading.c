#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "reading.h"
#include "map.h"
#include "cities_and_roads.h"
#include "routes.h"

#define ERROR false
#define CORRECT true
#define ROAD_NOT_EXIST -1
#define CITY_NOT_EXIST 0

const char *readCityName (const char *line, int lineLength, int *position) {

    char *cityName = NULL;
    int allocated = 0;
    int start = *position;
    int i = 0;

    while (start+i < lineLength && line[start+i] != ';' && (line[start+i]) >= ' ') {

        if (allocated <= i) {
            cityName = realloc(cityName, (allocated+50)*sizeof(char));
            if (cityName == NULL) return NULL;
            allocated += 50;
        }
        cityName[i] = line[start+i];
        i++;
    }

    //if the name is empty
    if (i == 0) return NULL;

    if (start+i < lineLength && line[start+i] < ' ') {
        free(cityName);
        return NULL;
    }

    cityName[i] = '\0';
    *position = start+i;
    return cityName;
}

int readNumber (const char *line, int length, int *position) {

    int number = 0;
    bool minus = false;

    if (line[*position] == '-') {
        minus = true;
        (*position)++;
    }

    if (line[*position] == '0'){
        *position = length;
        return 0;
    }

    while (*position < length && line[*position] != ';') {

        if (line[*position] < '0' || line[*position] > '9')
            return 0;

        number *= 10;
        number += (int)(line[*position]) - '0';
        (*position)++;
    }

    if (minus)
        number = -number;

    return number;
}

bool isPatternCorrect (char **pattern, int patternLength, const char *line, int lineLength) {

    if (lineLength <= patternLength) return ERROR;

    for (int i=0; i<patternLength; i++) {
        if (line[i] != *(*pattern + i))
            return ERROR;
    }
    return CORRECT;
}

bool isAddRoadCorrect (Map *map, char *line, int lineLength) {

    char *pattern = "addRoad;";
    int patternLength = 8;
    if (isPatternCorrect(&pattern, patternLength, line, lineLength) == ERROR) return ERROR;
    int position = patternLength;

    const char *city1 = readCityName(line, lineLength, &position);
    if (city1 == NULL || position >= lineLength-1 || line[position] != ';') {
        if (city1 != NULL) free((void*)city1);
        return ERROR;
    }
    position++;

    const char *city2 = readCityName(line, lineLength, &position);
    if (city2 == NULL || position >= lineLength-1 || line[position] != ';') {
        free((void*)city1);
        if (city2 != NULL) free((void*)city2);
        return ERROR;
    }
    position++;

    int roadLengthInt = readNumber(line, lineLength, &position);
    if (roadLengthInt <= 0 || position >= lineLength-1 || line[position] != ';') {
        free((void*)city1); free((void*)city2);
        return ERROR;
    }
    position++;
    unsigned roadLengthUnsigned = (unsigned)(roadLengthInt);

    int builtYear = readNumber(line, lineLength, &position);
    if (builtYear == 0 || position != lineLength) {
        free((void*)city1); free((void*)city2);
        return ERROR;
    }

    bool isRealizationCorrect = addRoad(map, city1, city2, roadLengthUnsigned, builtYear);
    free((void*)city1); free((void*)city2);
    return isRealizationCorrect;
}

bool isRepairRoadCorrect (Map *map, char *line, int lineLength) {

    char *pattern = "repairRoad;";
    int patternLength = 11;
    if (isPatternCorrect(&pattern, patternLength, line, lineLength) == ERROR) return ERROR;
    int position = patternLength;

    const char *city1 = readCityName(line, lineLength, &position);
    if (city1 == NULL || position >= lineLength-1 || line[position] != ';') {
        if (city1 != NULL) free((void*)city1);
        return ERROR;
    }
    position++;

    const char *city2 = readCityName(line, lineLength, &position);
    if (city2 == NULL || position >= lineLength-1 || line[position] != ';') {
        free((void*)city1);
        if (city2 != NULL) free((void*)city2);
        return ERROR;
    }
    position++;

    int repairYear = readNumber(line, lineLength, &position);
    if (repairYear == 0 || position != lineLength) {
        free((void*)city1); free((void*)city2);
        return ERROR;
    }

    bool isRealizationCorrect = repairRoad(map, city1, city2, repairYear);
    free((void*)city1); free((void*)city2);
    return isRealizationCorrect;
}

bool isGetRouteDescriptionCorrect (Map *map, char *line, int lineLength) {

    char *pattern = "getRouteDescription;";
    int patternLength = 20;
    if (isPatternCorrect(&pattern, patternLength, line, lineLength) == ERROR) return ERROR;
    int position = patternLength;

    int routeIdInt = readNumber(line, lineLength, &position);
    if (routeIdInt < 0 || position != lineLength) return ERROR;
    unsigned routeId = (unsigned)(routeIdInt);

    const char *description = getRouteDescription(map,routeId);
    if (description == NULL) return ERROR;

    printf("%s\n", description);
    free((void*)description);
    return CORRECT;
}

bool isNextRoadCorrect (const char *line, int lineLength, int *position,
                        unsigned *roadLength, int *year, char **city2) {

    int roadLengthInt = readNumber(line, lineLength, position);
    if (roadLengthInt <= 0 || *position >= lineLength-1 || line[*position] != ';') return ERROR;
    (*position)++;
    *roadLength = (unsigned)(roadLengthInt);

    *year = readNumber(line, lineLength, position);
    if (*year == 0 || *position >= lineLength-1 || line[*position] != ';') return ERROR;
    (*position)++;

    *city2 = (char*)readCityName(line, lineLength, position);
    if (*city2 == NULL) return ERROR;

    return CORRECT;
}

bool findOrCreateCities (Map *map, const char *city1, const char *city2,
                            City **firstCity, City **secondCity) {

    if (findCity(map, city1, firstCity) == CITY_NOT_EXIST) {
        if (createCity(map, firstCity, city1) == ERROR)
            return ERROR;
    }
    if (findCity(map, city2, secondCity) == CITY_NOT_EXIST) {
        if (createCity(map, secondCity, city2) == ERROR)
            return ERROR;
    }
    return CORRECT;
}

bool findOrCreateRoad (City *firstCity, City *secondCity, unsigned length, int year, Road **road) {

    int roadId = findRoad(firstCity, secondCity);
    if (roadId == ROAD_NOT_EXIST) {
        if (createRoad(firstCity, secondCity, length, year) == ERROR)
            return ERROR;
        roadId = firstCity->numberOfRoads -1;
    }

    *road = firstCity->tabOfRoads[roadId];
    (*road)->repairYear = year;

    return CORRECT;
}

bool addRoadAndRoutes (Map *map, const char *city1, const char *city2,
                    unsigned length, int year, unsigned routeId) {

    City *firstCity = NULL, *secondCity = NULL;
    if (findOrCreateCities(map, city1, city2, &firstCity, &secondCity) == ERROR) return ERROR;

    Road *firstRoad = NULL, *secondRoad = NULL;
    if (findOrCreateRoad(firstCity, secondCity, length, year, &firstRoad) == ERROR ||
        findOrCreateRoad(secondCity, firstCity, length, year, &secondRoad) == ERROR) return ERROR;

    firstRoad->alterRoad = secondRoad;
    secondRoad->alterRoad = firstRoad;

    if (addRouteToRoad(firstRoad, routeId) == ERROR) return ERROR;
    if (addRouteToRoad(secondRoad, routeId) == ERROR) return ERROR;

    return CORRECT;
}

bool areLengthAndYearCorrect (Map *map, const char *city1, const char *city2, unsigned length, int year) {

    City *firstCity = NULL, *secondCity = NULL;
    if (findOrCreateCities(map, city1, city2, &firstCity, &secondCity) == ERROR) return ERROR;

    //if route makes a circle in meanwhile
    if (secondCity->visited == true)
        return ERROR;

    firstCity->visited = true;
    secondCity->visited = true;

    int roadId = findRoad(firstCity, secondCity);
    if (roadId == ROAD_NOT_EXIST) return CORRECT;

    Road *road = firstCity->tabOfRoads[roadId];
    if (road->length != length || road->builtYear > year ||
        (road->repairYear != 0 && road->repairYear > year)) return ERROR;

    roadId = findRoad(secondCity, firstCity);
    if (roadId == ROAD_NOT_EXIST) return CORRECT;

    road = secondCity->tabOfRoads[roadId];
    if (road->length != length || road->builtYear > year ||
        (road->repairYear != 0 && road->repairYear > year)) return ERROR;

    return CORRECT;
}

bool isRouteNumberCorrect (Map *map, unsigned *routeId, const char *line, int lineLength, int *position) {

    int routeIdInt = readNumber(line, lineLength, position);
    if (routeIdInt <= 0 || routeIdInt >= 1000 || *position >= lineLength-1 || line[*position] != ';')
        return ERROR;
    (*position)++;
    *routeId = (unsigned)(routeIdInt);

    RoutesOnMap *route = findTheRouteViaId(map, *routeId);
    if (route != NULL) return ERROR;

    return CORRECT;

}

const char *copyOfCityName (const char *city) {

    int lengthOfCity = (int)(strlen(city)) +1;
    char *beginningCity = malloc(lengthOfCity* sizeof(char));
    beginningCity = strcpy(beginningCity, city);

    return beginningCity;
}

bool isAllWayCorrect (Map *map, int *position, const char *line, int lineLength, const char **city1) {

    for (int i=0; i<map->numberOfCities; i++)
        map->tabOfCities[i]->visited = false;

    unsigned roadLength; int year; char *city2 = NULL;
    int copyOfPosition = *position;

    while (*position < lineLength) {
        if (isNextRoadCorrect(line, lineLength, position, &roadLength, &year, &city2) == ERROR ||
            (*position == lineLength-1 && line[*position] == ';')) {

            free((void*)(*city1)); free((void*)city2);
            return ERROR;
        }

        if (*position < lineLength && line[*position] == ';')
            (*position)++;

        if (areLengthAndYearCorrect(map, *city1, city2, roadLength, year) == ERROR) {
            free((void*)(*city1)); free((void*)city2);
            return ERROR;
        }

        free((void*)(*city1));
        *city1 = city2;
        city2 = NULL;
    }

    *position = copyOfPosition;
    free((void*)(*city1));
    return CORRECT;
}

bool makeChangesOnMap (Map *map, int position, const char *line, int lineLength,
                    const char **city1, unsigned routeId, const char *beginningCity) {

    unsigned roadLength; int year; char *city2 = NULL;
    while (position < lineLength) {

        if (isNextRoadCorrect(line, lineLength, &position, &roadLength, &year, &city2) == ERROR) {
            free((void*)(*city1)); free((void*)city2);
            return ERROR;
        }
        position++;

        if (addRoadAndRoutes(map, *city1, city2, roadLength, year, routeId) == ERROR) {
            free((void*)(*city1)); free((void*)city2);
            return ERROR;
        }

        free((void*)(*city1));
        *city1 = city2;
    }

    const char *endCity = *city1;
    City *firstCity = NULL, *secondCity = NULL;

    if (findCity(map, beginningCity, &firstCity) == CITY_NOT_EXIST ||
        findCity(map, endCity, &secondCity) == CITY_NOT_EXIST) {
        free((void*)endCity); free(firstCity);
        return ERROR;
    }

    if (addRouteToMap(map, firstCity, secondCity, routeId) == ERROR) {
        free(firstCity); free(secondCity); free((void*)endCity);
        return ERROR;
    }
    free((void*)endCity);

    return CORRECT;
}

bool isMakeNewRouteCorrect (Map *map, const char *line, int lineLength) {

    int position = 0;
    unsigned routeId = 0;
    if (isRouteNumberCorrect(map, &routeId, line, lineLength, &position) == ERROR) return ERROR;

    const char *city1 = readCityName(line, lineLength, &position);
    if (city1 == NULL || position >= lineLength-1 || line[position] != ';') {
        if (city1 != NULL) free((void*)city1);
        return ERROR;
    }
    position++;
    const char *beginningCity = copyOfCityName(city1);

    if (isAllWayCorrect(map, &position, line, lineLength, &city1) == ERROR) {
        free((void*)beginningCity);
        return ERROR;
    }
    city1 = beginningCity;
    beginningCity = copyOfCityName(city1);

    if (makeChangesOnMap(map, position, line, lineLength, &city1, routeId, beginningCity) == ERROR)
        return ERROR;
    free((void*)beginningCity);

    return CORRECT;
}

void realizeLine (Map *map, char *line, int lineLength, int numberOfLine) {

    if (line[0] == 'a') {
        if (isAddRoadCorrect(map, line, lineLength) == CORRECT)
            return;
    }
    else if (line[0] == 'r') {
        if (isRepairRoadCorrect(map, line, lineLength) == CORRECT)
            return;
    }
    else if (line[0] == 'g') {
        if (isGetRouteDescriptionCorrect(map, line, lineLength) == CORRECT)
            return;
    }
    else if ((int)(line[0]) >= '0' && (int)(line[0]) <= '9') {
        if (isMakeNewRouteCorrect(map, line, lineLength) == CORRECT)
            return;
    }

    //if the line isn't correct, function come here and print 'error'
    fprintf(stderr, "ERROR %d\n", numberOfLine);
}

void readComment() {

    int sign = getchar();

    while (sign != '\n' && sign != EOF) {
        sign = getchar();
    }
    if (sign == EOF)
        ungetc(sign, stdin);
}

void readLine (Map *map, int numberOfLine) {

    int sign = getchar();
    char *line = NULL;
    int allocated = 0;
    int read = 0; //how many signs have been read

    while (sign != '\n' && sign != EOF) {

        if (allocated <= read) {
            line = realloc(line, (allocated+50)*sizeof(char));
            if (line == NULL) {
                fprintf(stderr, "ERROR %d\n", numberOfLine);
                return;
            }
            allocated += 50;
        }

        line[read] = (char)(sign);
        read++;
        sign = getchar();
    }

    if (sign == EOF)
        ungetc(sign, stdin);

    realizeLine(map, line, read, numberOfLine);
    free(line);
}

void readFile () {

    Map *map = newMap();

    int sign;
    int howManyLines = 0;

    while ((sign=getchar()) != EOF) {
        howManyLines ++;

        if (sign != '\n' && sign != '#') {
            ungetc(sign, stdin);
            readLine(map, howManyLines);
        }
        else if (sign == '#') {
            readComment();
        }
        //else it was '\n' and program does nothing
    }

    deleteMap(map);
}
