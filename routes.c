#include "routes.h"

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

bool theCityHasRoute (City *city, unsigned routeId) {

    for (int i=0; i<city->numberOfRoads; i++) {
        Road *road = city->tabOfRoads[i];
        ListOfRoutes *listOfRoutes = road->listOfRoutes;
        while (listOfRoutes != NULL) {
            if (listOfRoutes->routeId == routeId) {
                return true;
            }
            listOfRoutes = listOfRoutes->next;
        }
    }
    return false;
}

bool push(List **priorityQueue, City *city) {

    List *element = malloc(sizeof(List));
    if (element == NULL)
        return ERROR;
    element->city = city;
    element->next = NULL;

    List *copyOfPriorityQueue = *priorityQueue;

    if (*priorityQueue != NULL && (*priorityQueue)->city->distance > element->city->distance) {
        element->next = *priorityQueue;
        *priorityQueue = element;
        return CORRECT;
    }

    while (*priorityQueue != NULL && (*priorityQueue)->next != NULL &&
           (((*priorityQueue)->next)->city)->distance < (element->city)->distance) {
        *priorityQueue = (*priorityQueue)->next;
    }
    if (*priorityQueue == NULL) {
        *priorityQueue = element;
    }
    else if ((*priorityQueue)->next == NULL) {
        (*priorityQueue)->next = element;
        *priorityQueue = copyOfPriorityQueue;
    }
    else {
        element->next = (*priorityQueue)->next;
        (*priorityQueue)->next = element;
        *priorityQueue = copyOfPriorityQueue;
    }
    return CORRECT;
}

City *pop(List **queue) {

    City *firstCity = (*queue)->city;
    List *pom = *queue;
    *queue = (*queue)->next;
    free(pom);
    return firstCity;
}

bool empty(List *queue) {

    if (queue == NULL)
        return EMPTY;
    else
        return NOT_EMPTY;
}

bool dijkstra (Map *map, City *firstCity, unsigned routeId) {
    for (int i=0; i<map->numberOfCities; i++) {
        (map->tabOfCities[i])->distance = INT_MAX;
        (map->tabOfCities[i])->visited = NOT_VISITED;
    }

    firstCity->distance = 0;

    List *priorityQueue = NULL;

    if (!push(&priorityQueue, firstCity))
        return ERROR;

    while (!empty(priorityQueue)) {

        City *city = pop(&priorityQueue);
        if (city->visited == VISITED)
            continue;
        city->visited = VISITED;
        bool thisCityHasThisRouteYet = theCityHasRoute(city, routeId);
        if (thisCityHasThisRouteYet && city != firstCity)
            continue;
        for (int i=0; i<city->numberOfRoads; i++) {
            Road *road = city->tabOfRoads[i];
            if (road->length == INT_MAX)
                continue;
            if (city->distance + (int)road->length < (road->secondCity)->distance) {
                road->secondCity->distance = city->distance + road->length;
                if (!push(&priorityQueue, road->secondCity))
                    return ERROR;
            }
        }
    }
    return CORRECT;
}

bool addRouteToRoad (Road *road, unsigned routeId) {

    ListOfRoutes *element = malloc(sizeof(ListOfRoutes));
    if (element == NULL)
        return ERROR;

    element->routeId = routeId;
    element->next = road->listOfRoutes;
    road->listOfRoutes = element;
    return CORRECT;
}

bool addRouteToWay (Way **way, unsigned routeId) {

    while (*way != NULL) {
        Road *road = (*way)->road;
        Way *help = *way;
        *way = (*way)->next;
        free(help);
        if (!addRouteToRoad(road, routeId))
            return ERROR;
        if (!addRouteToRoad(road->alterRoad, routeId))
            return ERROR;
    }
    return CORRECT;
}

bool addRouteToMap (Map *map, City *firstCity, City *secondCity, unsigned routeId) {

    RoutesOnMap *element = malloc(sizeof(struct RoutesOnMap));
    if (element == NULL)
        return ERROR;

    element->routeId = routeId;
    element->firstCity = firstCity;
    element->secondCity = secondCity;
    element->next = map->routesOnMap;
    map->routesOnMap = element;
    return CORRECT;
}

void deleteRouteFromMap (Map *map, unsigned routeId) {

    RoutesOnMap *copy = map->routesOnMap;

    if (map->routesOnMap == NULL)
        return;
    if (map->routesOnMap->routeId == routeId) {
        RoutesOnMap *help = map->routesOnMap;
        map->routesOnMap = map->routesOnMap->next;
        free(help);
        return;
    }

    while (map->routesOnMap->next != NULL && map->routesOnMap->next->routeId != routeId)
        map->routesOnMap = map->routesOnMap->next;

    if (map->routesOnMap->next != NULL) {
        RoutesOnMap *help = map->routesOnMap->next;
        map->routesOnMap->next = help->next;
        free(help);
    }
    map->routesOnMap = copy;
}

void freeTheWay (Way **way) {

    while (*way != NULL) {
        Way *help = *way;
        *way = (*way)->next;
        free(help);
    }
}

RoutesOnMap *findTheRouteViaId (Map *map, unsigned routeId) {

    RoutesOnMap *routesOnMap = map->routesOnMap;
    while (routesOnMap != NULL && routesOnMap->routeId != routeId) {
        routesOnMap = routesOnMap->next;
    }
    return routesOnMap;
}

Way *findWayOfRoute (unsigned routeId, City *previousCity, City *currentCity, City *lastCity) {

    //when the route exist and i want to have its description
    if (currentCity == lastCity) {
        return NULL;
    }

    for (int i=0; i<currentCity->numberOfRoads; i++) {

        Road *road = currentCity->tabOfRoads[i];
        if (road->secondCity == previousCity)
            continue;
        ListOfRoutes *listOfRoutes = road->listOfRoutes;

        while (listOfRoutes != NULL) {
            if (listOfRoutes->routeId == routeId) {

                Way *currentElement = malloc(sizeof(Way));
                if (currentElement == NULL)
                    return ERROR;
                currentElement->road = road;
                currentElement->next = findWayOfRoute(routeId, currentCity, road->secondCity, lastCity);
                return currentElement;
            }
            listOfRoutes = listOfRoutes->next;
        }
    }
    return NULL;
}

int findYear (Way *way) {

    int theOldestYear = INT_MAX;

    while (way != NULL) {
        Road *road = way->road;
        if (road->repairYear != 0) {
            if (road->repairYear < theOldestYear)
                theOldestYear = road->repairYear;
        }
        else {
            if (road->builtYear < theOldestYear)
                theOldestYear = road->builtYear;
        }
        way = way->next;
    }
    return theOldestYear;
}

void chooseNewElement (Road *road, Way *way, int ways, int *winningYear,
        Way **winningWay, int *numberOfWinningWays)  {

    Way *currentElement = malloc(sizeof(Way));
    currentElement->road = road;
    currentElement->next = way;

    int year = findYear(currentElement);

    if (year > *winningYear) {
        *winningYear = year;
        freeTheWay(winningWay);
        *winningWay = currentElement;
        *numberOfWinningWays = ways;
    }
    else if (year == *winningYear) {
        *numberOfWinningWays += ways;
        freeTheWay(&currentElement);
    }
    else {
        freeTheWay(&currentElement);
    }
}

Way *findTheBestWay(City *beginningCity, City *currentCity, Way *previousWay, int *howManyWays, unsigned routeId) {

    if (currentCity == beginningCity) {
        *howManyWays = 1;
        return previousWay;
    }

    int winningYear = -INT_MAX;
    Way *winningWay = NULL;
    int numberOfWinningWays = 0;

    for (int i=0; i<currentCity->numberOfRoads; i++) {
        Road *road = currentCity->tabOfRoads[i];

        if ((road->secondCity)->distance + (int)road->length == currentCity->distance) {

            bool thisCityHasThisRouteYet = theCityHasRoute(road->secondCity, routeId);
            if (thisCityHasThisRouteYet && road->secondCity != beginningCity)
                continue;

            int ways;
            Way *way = findTheBestWay(beginningCity, road->secondCity, previousWay, &ways, routeId);

           chooseNewElement(road, way, ways, &winningYear, &winningWay, &numberOfWinningWays);
        }
    }
    *howManyWays = numberOfWinningWays;
    return winningWay;
}

bool repairBreaksInRoutes(Map *map, City *firstCity, City *secondCity,
                ListOfRoutes *listOfRoutes, ListOfWays **listOfWays) {
    while (listOfRoutes != NULL) {
        unsigned routeId = listOfRoutes->routeId;
        //printf("route nr %u\n", routeId);
        if (!dijkstra(map, firstCity, routeId)) return ERROR;
        if (secondCity->distance == INT_MAX)  return ERROR;


        Way *theNewestRoute = NULL;
        int howManyWays;
        theNewestRoute = findTheBestWay(firstCity, secondCity, theNewestRoute, &howManyWays, routeId);

        if (howManyWays > 1) {
            freeTheWay(&theNewestRoute);
            return ERROR;
        }
        else {
            freeTheWay(&theNewestRoute);
            if (!dijkstra(map, secondCity, routeId)) return ERROR;
            theNewestRoute = findTheBestWay(secondCity, firstCity, theNewestRoute, &howManyWays, routeId);
            if (howManyWays > 1) {
                freeTheWay(&theNewestRoute);
                return ERROR;
            }
        }

        ListOfWays *element = malloc(sizeof(ListOfWays));
        if (element == NULL) return ERROR;
        element->way = theNewestRoute;
        element->routeId = routeId;
        element->next = *listOfWays;
        *listOfWays = element;

        listOfRoutes = listOfRoutes->next;
    }
    return CORRECT;
}

bool caseTheSameDistancesExtend (Map *map, int *distanceFromCity1, int *distanceFromCity2, City *newCity,
                           RoutesOnMap **route, unsigned routeId, Way **theNewestRoute, int *howManyWays) {

    if (*distanceFromCity1 == INT_MAX)
        return ERROR;

    int howManyWays1, howManyWays2;
    Way *secondWay = NULL;
    secondWay = findTheBestWay((*route)->secondCity, newCity, secondWay, &howManyWays2, routeId);
    int year2 = findYear(secondWay);

    if (!dijkstra(map, (*route)->firstCity, routeId)) return ERROR;
    Way *firstWay = NULL;
    firstWay = findTheBestWay((*route)->firstCity, newCity, firstWay, &howManyWays1, routeId);
    int year1 = findYear(firstWay);

    if (year1 > year2) {
        //first Way win
        *theNewestRoute = firstWay;
        *howManyWays = howManyWays1;
        (*distanceFromCity2)++;
        freeTheWay(&secondWay);
    }
    else if (year1 < year2) {
        //second Way win
        *theNewestRoute = secondWay;
        *howManyWays = howManyWays2;
        (*distanceFromCity1)++;
        freeTheWay(&firstWay);
    }
    else {
        freeTheWay(&firstWay);
        freeTheWay(&secondWay);
        return ERROR;
    }
    return CORRECT;
}

bool checkCaseMoreThanOneWayExtend (Map *map, int distanceFromCity1, int distanceFromCity2, City *newCity,
                              RoutesOnMap **route, unsigned routeId, Way **theNewestRoute,  int howManyWays) {

    if (howManyWays > 1) {
        freeTheWay(theNewestRoute);
        return ERROR;
    }
    else {
        freeTheWay(theNewestRoute);
        if (!dijkstra(map, newCity, routeId)) return ERROR;
        if (distanceFromCity1 < distanceFromCity2)
            *theNewestRoute = findTheBestWay(newCity, (*route)->firstCity, *theNewestRoute, &howManyWays, routeId);
        else if (distanceFromCity1 > distanceFromCity2)
            *theNewestRoute = findTheBestWay(newCity, (*route)->secondCity, *theNewestRoute, &howManyWays, routeId);
        if (howManyWays > 1) {
            freeTheWay(theNewestRoute);
            return ERROR;
        }
    }
    return CORRECT;
}

void removeRoadFromCity (City *city, int roadId) {

    int lastId = city->numberOfRoads-1; //last index in table od roads

    if (roadId == lastId) {
        city->tabOfRoads[lastId] = NULL;
        city->numberOfRoads --;
    }
    else {
        city->tabOfRoads[roadId] = city->tabOfRoads[lastId];
        city->tabOfRoads[lastId] = NULL;
        city->numberOfRoads --;
    }
}

bool undoRemoveChanges (ListOfWays **listOfWays, Road *road1, Road *road2,
                        unsigned length1, unsigned length2) {

    while (*listOfWays != NULL) {
        Way *way = (*listOfWays)->way;
        freeTheWay(&way);
        ListOfWays *help = *listOfWays;
        *listOfWays = (*listOfWays)->next;
        free(help);
    }
    road1->length = length1;
    road2->length = length2;
    return ERROR;
}

bool doRemoveChanges (ListOfWays **listOfWays, City *firstCity, City *secondCity,
                      Road **road1, Road **road2, int firstRoadId, int secondRoadId,
                      ListOfRoutes **listOfRoutes1, ListOfRoutes **listOfRoutes2) {

    while (*listOfWays!= NULL) {
        Way *way = (*listOfWays)->way;
        if (!addRouteToWay(&way, (*listOfWays)->routeId)) return ERROR;

        ListOfWays *help = *listOfWays;
        *listOfWays = (*listOfWays)->next;
        free(help);
    }

    removeRoadFromCity(firstCity, firstRoadId);
    removeRoadFromCity(secondCity, secondRoadId);
    free(*road1);
    free(*road2);

    while (*listOfRoutes1 != NULL) {
        ListOfRoutes *help = *listOfRoutes1;
        *listOfRoutes1 = (*listOfRoutes1)->next;
        free(help);
    }
    while (*listOfRoutes2 != NULL) {
        ListOfRoutes *help = *listOfRoutes2;
        *listOfRoutes2 = (*listOfRoutes2)->next;
        free(help);
    }
    return CORRECT;
}


