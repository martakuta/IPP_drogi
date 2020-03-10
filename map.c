#define ERROR 0
#define CORRECT 1
#define CITY_NOT_EXIST 0
#define EXIST 1
#define ROAD_NOT_EXIST -1

#include <wordexp.h>
#include "map.h"

Map *newMap(void) {

    Map *map = malloc(sizeof(Map));
    if (map == NULL)
        return map;
    map->numberOfCities = 0;
    map->allocatedCities = 0;
    map->tabOfCities = NULL;
    map->routesOnMap = NULL;
    return map;
}

void deleteMap(Map *map) {

    if (map == NULL)
        return;

    for (int i=0; i<map->numberOfCities; i++) {
        City *city = map->tabOfCities[i];

        for (int j=0; j<city->numberOfRoads; j++) {
            Road *road = city->tabOfRoads[j];

            ListOfRoutes *listOfRoutes = road->listOfRoutes;
            while (listOfRoutes != NULL) {
                ListOfRoutes *help = listOfRoutes;
                listOfRoutes = listOfRoutes->next;
                free(help);
            }
            free(road);
        }
        free((void*)city->name);
        free(city->tabOfRoads);
        free(city);
    }

    RoutesOnMap *routesOnMap = map->routesOnMap;
    while (routesOnMap != NULL) {
        RoutesOnMap *help = routesOnMap;
        routesOnMap = routesOnMap->next;
        free(help);
    }

    free(map->tabOfCities);
    free(map);
}

bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear) {

    if (map == NULL || strcmp(city1, city2) == 0 || builtYear == 0 || length == 0) return ERROR;

    City *firstCity = NULL, *secondCity = NULL;
    if (findCity(map, city1, &firstCity) == CITY_NOT_EXIST) {
        if (createCity(map, &firstCity, city1) == ERROR)
            return ERROR;
    }
    if (findCity(map, city2, &secondCity) == CITY_NOT_EXIST) {
        if (createCity(map, &secondCity, city2) == ERROR)
            return ERROR;
    }

    if (findRoad(firstCity, secondCity) != ROAD_NOT_EXIST) return ERROR;
    if (findRoad(secondCity, firstCity) != ROAD_NOT_EXIST) return ERROR;

    if (createRoad(firstCity, secondCity, length, builtYear) == ERROR) return ERROR;
    if (createRoad(secondCity, firstCity, length, builtYear) == ERROR) return ERROR;

    Road *road1 = firstCity->tabOfRoads[firstCity->numberOfRoads-1];
    Road *road2 = secondCity->tabOfRoads[secondCity->numberOfRoads-1];
    road1->alterRoad = road2;
    road2->alterRoad = road1;

    return CORRECT;
}

bool repairRoad (Map *map, const char *city1, const char *city2, int repairYear) {

    if (map == NULL || repairYear == 0 || strcmp(city1, city2) == 0) return ERROR;

    City *firstCity = NULL, *secondCity = NULL;
    if (findCity(map, city1, &firstCity) == CITY_NOT_EXIST ||
        findCity(map, city2, &secondCity) == CITY_NOT_EXIST)
        return ERROR;

    int firstRoadId = findRoad(firstCity, secondCity);
    int secondRoadId = findRoad(secondCity, firstCity);
    if (firstRoadId == ROAD_NOT_EXIST || secondRoadId == ROAD_NOT_EXIST)
        return ERROR;

    Road *firstRoad = firstCity->tabOfRoads[firstRoadId];
    Road *secondRoad = secondCity->tabOfRoads[secondRoadId];

    if (repairYear < firstRoad->builtYear || (firstRoad->repairYear != 0 &&
        repairYear < firstRoad->repairYear))
        return ERROR;
    if (repairYear < secondRoad->builtYear || (secondRoad->repairYear != 0 &&
        repairYear < secondRoad->repairYear))
        return ERROR;

    firstRoad->repairYear = repairYear;
    secondRoad->repairYear = repairYear;

    return CORRECT;
}

bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {

    if (map == NULL || strcmp(city1, city2) == 0 || routeId >= 1000 || routeId <= 0) return ERROR;
    if (findTheRouteViaId(map, routeId) != NULL) return ERROR;

    City *firstCity = NULL, *secondCity = NULL;
    if (findCity(map, city1, &firstCity) == CITY_NOT_EXIST ||
        findCity(map, city2, &secondCity) == CITY_NOT_EXIST) return ERROR;

    if (!dijkstra(map, firstCity, routeId)) return ERROR;
    if (secondCity->distance == INT_MAX)  return ERROR; //way doesn't exist

    Way *theNewestRoute = NULL;
    int howManyWays;
    theNewestRoute = findTheBestWay(firstCity, secondCity, theNewestRoute, &howManyWays, routeId);

    if (howManyWays > 1) { //there are more than one way with the best length and the best age
        freeTheWay(&theNewestRoute);
        return ERROR;
    }
    else {
        freeTheWay(&theNewestRoute);
        if (!dijkstra(map, secondCity, routeId)) return ERROR;
        theNewestRoute = findTheBestWay(secondCity, firstCity, theNewestRoute, &howManyWays, routeId);

        if (howManyWays > 1) { //there are more than one way with the best length and the best age
            freeTheWay(&theNewestRoute);
            return ERROR;
        }
    }

    if (!addRouteToMap(map, firstCity, secondCity, routeId)) {
        freeTheWay(&theNewestRoute);
        return ERROR;
    }
    if (!addRouteToWay(&theNewestRoute, routeId)) {
        deleteRouteFromMap(map, routeId);
        freeTheWay(&theNewestRoute);
        return ERROR;
    }

    return CORRECT;
}

bool extendRoute(Map *map, unsigned routeId, const char *city) {

    if (map == NULL || routeId >= 1000 || routeId <= 0) return ERROR;

    City *newCity = NULL;
    if (findCity(map, city, &newCity) == CITY_NOT_EXIST) return ERROR;
    if (theCityHasRoute(newCity, routeId)) return ERROR;
    RoutesOnMap *route = findTheRouteViaId(map, routeId);
    if (route == NULL) return ERROR;

    if (!dijkstra(map, route->firstCity, routeId)) return ERROR;
    int distanceFromCity1 = newCity->distance;
    if (!dijkstra(map, route->secondCity, routeId)) return ERROR;
    int distanceFromCity2 = newCity->distance;

    Way *theNewestRoute = NULL;
    int howManyWays;

    if (distanceFromCity1 == distanceFromCity2) {
        if (!caseTheSameDistancesExtend(map, &distanceFromCity1, &distanceFromCity2, newCity,
                &route, routeId, &theNewestRoute, &howManyWays)) return ERROR;
    }
    else if (distanceFromCity1 < distanceFromCity2) {
        if (!dijkstra(map, route->firstCity, routeId)) return ERROR;
        theNewestRoute = findTheBestWay(route->firstCity, newCity, theNewestRoute, &howManyWays, routeId);
    }
    else {
        theNewestRoute = findTheBestWay(route->secondCity, newCity, theNewestRoute, &howManyWays, routeId);
    }

    if (!checkCaseMoreThanOneWayExtend(map, distanceFromCity1, distanceFromCity2, newCity,
            &route, routeId, &theNewestRoute, howManyWays)) return ERROR;

    if (distanceFromCity1 < distanceFromCity2)
        route->firstCity = newCity;
    else
        route->secondCity = newCity;

    if (!addRouteToWay(&theNewestRoute, routeId)) return ERROR;

    return CORRECT;
}

bool removeRoad(Map *map, const char *city1, const char *city2) {

    if (map == NULL || strcmp(city1, city2) == 0) return ERROR;

    City *firstCity = NULL, *secondCity = NULL;
    if (findCity(map, city1, &firstCity) == CITY_NOT_EXIST ||
        findCity(map, city2, &secondCity) == CITY_NOT_EXIST)
        return ERROR;

    int firstRoadId = findRoad(firstCity, secondCity);
    int secondRoadId = findRoad(secondCity, firstCity);
    if (firstRoadId == ROAD_NOT_EXIST || secondRoadId == ROAD_NOT_EXIST) return ERROR;

    Road *road1 = firstCity->tabOfRoads[firstRoadId];
    Road *road2 = secondCity->tabOfRoads[secondRoadId];
    ListOfRoutes *listOfRoutes1 = road1->listOfRoutes;
    ListOfRoutes *listOfRoutes2 = road2->listOfRoutes;

    unsigned length1 = road1->length;
    road1->length = INT_MAX;
    unsigned length2 = road2->length;
    road2->length = INT_MAX;

    ListOfWays *listOfWays = NULL;
    if (repairBreaksInRoutes(map, firstCity, secondCity, listOfRoutes1, &listOfWays) == ERROR)
         return undoRemoveChanges(&listOfWays, road1, road2, length1, length2);
    else
        return doRemoveChanges(&listOfWays, firstCity, secondCity, &road1, &road2,
                firstRoadId, secondRoadId, &listOfRoutes1, &listOfRoutes2);
}

char const* getRouteDescription(Map *map, unsigned routeId) {

    if (map == NULL) return NULL;
    RoutesOnMap *route = findTheRouteViaId(map, routeId);
    if (route == NULL) {
        char *answer = malloc(sizeof(char));
        if (answer == NULL) return NULL;
        answer[0] = '\0';
        return  answer;
    }

    Way *way = NULL;
    way = findWayOfRoute(routeId, route->firstCity, route->firstCity, route->secondCity);
    Way *copyOfWay = way;

    const char *semicolon;
    char *description = addBeginningToDescription(route, routeId, &semicolon);
    if (description == NULL) return NULL;
    int allocated = 100;

    while (way != NULL) {

        if (allocated - strlen(description) < 50) {
            allocated += 100;
            description = realloc(description, allocated * sizeof(char));
            if (description == NULL) return NULL;
        }
        Road *road = way->road;
        addRoadDetailsToDescription(road, &description, semicolon);
        way = way->next;
    }
    freeTheWay(&copyOfWay);
    return description;
}