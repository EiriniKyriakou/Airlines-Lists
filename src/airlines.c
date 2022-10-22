/***************************************************
 *                                                 *
 * file: twitter.c                                 *
 *                                                 *
 * @Author  Iacovos G. Kolokasis                   *
 * @Version 25-02-2020                             *
 * @email   kolokasis@csd.uoc.gr                   *
 *                                                 *
 * @brief   Implementation of airlines.h            *
 *                                                 *
 ***************************************************
 */

#include "airlines.h"


int print2(){
    _airlines_l *last = airlines_p;
    _airplane *cur = last->p_root;
    int i=1;
    printf("\tAirline %d = ",i);
    while(cur != NULL){
        if (cur->next == NULL){
            printf("%d:%d ", cur->pId, last->aId);
        }else{
            printf("%d:%d, ", cur->pId, last->aId);
        }
        cur = cur->next;
    }
    printf("\n");
    while(last->next != airlines_p){
        i++;
        last = last->next;
        printf("\tAirline %d = ", i);
        cur = last->p_root;
        while(cur != NULL){
            if (cur->next == NULL){
                printf("%d:%d ", cur->pId, last->aId);
            }else{
                printf("%d:%d, ", cur->pId, last->aId);
            }
            cur = cur->next;
        }
        printf("\n");
    }
    /*printf("DONE\n");*/
    return 1;
}

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize() {
    return 1;
}

/**
 * @brief Register airline
 *
 * @param aId The airline's id
 *
 * @return 1 on success
 *         0 on failure
 */
int register_airline(int aId) {
    _airlines_l *new_airline;
    new_airline = malloc(sizeof(_airlines_l));
    new_airline->aId = aId;
    new_airline->p_root = NULL;
    if (airlines_p == NULL){
        new_airline->next = new_airline;
        new_airline->prev = new_airline;
        airlines_p = new_airline;
    }else{
        _airlines_l *last = airlines_p;
        while(last->next != airlines_p){
            last = last->next;
        }
        new_airline->next = airlines_p;
        airlines_p->prev = new_airline;
        new_airline->prev = last;
        last->next = new_airline;

    }

    /*Print*/
    _airlines_l *last = airlines_p;
    /*printf("R %d\n", aId,);*/
    printf("\tAirlines = %d", last->aId);
    while(last->next != airlines_p){
        last = last->next;
        printf(", %d",last->aId);
    }
    printf("\n");
    /*printf("DONE\n");*/
    return 1;
}

/**
 * @brief Insert new airplane to the list of the certain airline with aId
 *
 * @param aId         The airlines id
 * @param pId         The airplanes id
 * @param dest        Destination Id [0-9]
 * @param depart_time Departure time
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_airplanes(int aId, int pId, int dest, int depart_time) {
    _airplane *new_airplane;
    new_airplane = malloc(sizeof(_airplane));
    new_airplane->pId = pId;
    new_airplane->dest = dest;
    new_airplane->depart_time = depart_time;
    _airlines_l *last = airlines_p;

    /*Finds the right node in airline, the last*/
    if(last->aId != aId){
        last = last->next;
    }
    while((last->aId != aId) && (last != airlines_p)){
        last = last->next;
    }
    if(last->aId == aId){
        /*Finds where to put airplane*/
        if (last->p_root == NULL){
            last->p_root = new_airplane;
            new_airplane->next = NULL;
        }else{
            _airplane *cur = last->p_root;
            _airplane *pre = NULL;
            if (pId < cur->pId){
                pre = new_airplane;
                pre->next = cur;
                last->p_root = pre;
            } else{
                while( (cur != NULL) && (pId > cur->pId)){
                    pre = cur;
                    cur = cur->next;
                }
                pre->next = new_airplane;
                new_airplane->next = cur;
            }
        }
        /*Finds where to put it in dest table*/
        _flights_t *new_flight;
        new_flight = malloc(sizeof(_flights_t));
        new_flight->pId = pId;
        new_flight->depart_time = depart_time;

        if(destTable[dest].flight_p == NULL){
            _flights_t *sentinel;
            destTable[dest].flight_p = new_flight;
            sentinel = malloc(sizeof(_flights_t));
            sentinel->pId = pId;
            sentinel->depart_time = depart_time;
            sentinel->next = new_flight;
            destTable[dest].sentinel_p = sentinel;
            new_flight->next = sentinel;

        }else{
            _flights_t *cur = destTable[dest].flight_p;
            _flights_t *pre = NULL;
            if (depart_time < cur->depart_time){
                pre = new_flight;
                pre->next = cur;
                destTable[dest].flight_p = pre;
            }else{
                if(depart_time > destTable[dest].sentinel_p->depart_time){
                    cur = destTable[dest].sentinel_p->next;
                }
                while( (cur->next != destTable[dest].sentinel_p ) && (depart_time >= cur->next->depart_time)){
                    if(depart_time == cur->depart_time){
                        while((cur->next != destTable[dest].sentinel_p ) && (pId > cur->next->pId)){
                            pre = cur;
                            cur = cur->next;
                        }
                    }else{
                        pre = cur;
                        cur = cur->next;
                    }
                }
                /*
                pre->next = new_flight;
                new_flight->next = cur;
                */
                new_flight->next = cur->next;
                cur->next = new_flight;
            }
            destTable[dest].sentinel_p->pId = pId;
            destTable[dest].sentinel_p->depart_time = depart_time;
            destTable[dest].sentinel_p->next = new_flight;

        }
    }

    /*printf("I %d %d %d %d\n", aId, pId, dest, depart_time);*/
    print2();
    return 1;
}

/**
 * @brief Cancel flight
 *
 * @param aId         The airlines id
 * @param pId         The airplanes id
 * @param dest        Destination Id [0-9]
 *
 * @return 1 on success
 *         0 on failure
 */
int cancel_flight(int aId, int pId, int dest) {
    /*Delete from airplane list*/
    _airlines_l *airline_aId = airlines_p;
    _airplane *airline_pId;
    _airplane *pre;
    if(airline_aId->aId != aId){
        airline_aId = airline_aId->next;
    }
    while((airline_aId != airlines_p) && (airline_aId->aId != aId)){
        airline_aId = airline_aId->next;
    }
    if(airline_aId->aId == aId){
        airline_pId = airline_aId->p_root;
        if(airline_pId->pId == pId){
            airline_aId->p_root = airline_pId->next;
            free(airline_pId);
        }else{
            while((airline_pId->next != NULL) && (airline_pId->pId != pId)){
                pre = airline_pId;
                airline_pId = airline_pId->next;
            }
            pre->next = airline_pId->next;
            free(airline_pId);
        }

        /*Delete from destination table*/
        _flights_t *curr = destTable[dest].flight_p;
        _flights_t *prev;
        if (curr->pId == pId){ /*Case for first in list*/
            destTable[dest].flight_p = curr->next;
            destTable[dest].sentinel_p->pId = curr->next->pId;
            destTable[dest].sentinel_p->depart_time = curr->next->depart_time;
            destTable[dest].sentinel_p->next = curr->next;

        }else{
            while((curr->next != destTable[dest].sentinel_p) && (curr->pId != pId)){
                prev = curr;
                curr = curr->next;
            }
            destTable[dest].sentinel_p->pId = prev->pId;
            destTable[dest].sentinel_p->depart_time = prev->depart_time;
            destTable[dest].sentinel_p->next = prev;
            prev->next = curr->next;
        }
        free(curr);
    }

    /*print*/
    printf("\n");
    /*printf("C %d %d %d\n", aId, pId, dest);*/
    print2();
    return 1;
}

/**
 * @brief Delete airline
 *
 * @param aId         The airlines id
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_airline(int aId) {
    _airlines_l *del_airline = airlines_p;
    _airlines_l *prev_1 = NULL;
    if(aId != del_airline->aId){
        prev_1 = del_airline;
        del_airline = del_airline->next;
    }
    while ((del_airline != airlines_p) &&(aId != del_airline->aId)){
        prev_1 = del_airline;
        del_airline = del_airline->next;
    }
    if (aId == del_airline->aId){
        while(del_airline->p_root != NULL){
            _airplane *c = del_airline->p_root;
            _flights_t *curr = destTable[c->dest].flight_p;
            _flights_t *prev;
            if (curr->pId == c->pId){ /*Case for first in list*/
                destTable[c->dest].flight_p = curr->next;
                destTable[c->dest].sentinel_p->pId = curr->next->pId;
                destTable[c->dest].sentinel_p->depart_time = curr->next->depart_time;
                destTable[c->dest].sentinel_p->next = curr->next;
                free(curr);
            }else{
                while((curr/*->next*/ != destTable[c->dest].sentinel_p) && (curr->pId != c->pId)){
                    prev = curr;
                    curr = curr->next;
                }
                if(curr != destTable[c->dest].sentinel_p){
                    prev->next = curr->next;
                    destTable[c->dest].sentinel_p->pId = prev->pId;
                    destTable[c->dest].sentinel_p->depart_time = prev->depart_time;
                    destTable[c->dest].sentinel_p->next = prev;
                    free(curr);
                }
            }
            del_airline->p_root = c->next;
            free(c);
        }
        free(del_airline->p_root);
        if((del_airline->next == airlines_p) && (del_airline->prev == airlines_p)){
            airlines_p = NULL;
        }else if(del_airline == airlines_p ){
            prev_1 = airlines_p->prev;
            airlines_p = airlines_p->next;
            prev_1->next = airlines_p;
            airlines_p->prev = prev_1;
        }else if(del_airline->next == airlines_p){
            prev_1->next = airlines_p;
            airlines_p->prev = prev_1;
        }else{
            _airlines_l *tmp = del_airline->next;
            prev_1->next = tmp;
            tmp->prev = prev_1;
        }
        free(del_airline);
    }

    /*Print*/
    /*printf("D %d\n", aId);*/
    print2();
    return 1;
}

/**
 * @brief Acquisition airline
 *
 * @param aId1         The first airline id
 * @param aId2         The second airline id
 *
 * @return 1 on success
 *         0 on failure
 */
int acquisition_airline(int aId1, int aId2) {
    _airlines_l *aId1_airline = airlines_p;
    _airlines_l *aId2_airline = airlines_p;
    _airlines_l *prev_1 = aId1_airline;
    _airplane *aId1_airplane;
    _airplane *aId2_airplane;
    _airplane *pr2 = NULL;
    _airplane *pr1 = NULL;
    if (aId1_airline->aId != aId1){
        prev_1 = aId1_airline;
        aId1_airline = aId1_airline->next;
    }
    while ((aId1_airline != airlines_p) && (aId1_airline->aId != aId1)){
        prev_1 = aId1_airline;
        aId1_airline = aId1_airline->next;
    }
    if (aId2_airline->aId != aId2){
        aId2_airline = aId2_airline->next;
    }
    while ((aId2_airline != airlines_p) && (aId2_airline->aId != aId2)){
        aId2_airline = aId2_airline->next;
    }
    if ((aId1_airline->aId == aId1) && (aId2_airline->aId == aId2)){
        aId1_airplane = aId1_airline->p_root;
        aId2_airplane = aId2_airline->p_root;
        while (aId1_airplane != NULL ){
            if((aId1_airline->aId == aId1) && (aId2_airline->aId == aId2)){
                if (aId2_airline->p_root == NULL){ /*Case for airline2 is empty*/
                    aId2_airline->p_root = aId1_airplane;
                    aId1_airline->p_root = NULL;
                }else if(aId2_airplane == NULL){ /*End of airline2*/
                    pr2->next = aId1_airplane;
                    aId1_airline->p_root = NULL;
                }else if ((aId2_airplane->pId == aId2_airline->p_root->pId) && (aId1_airplane->pId < aId2_airplane->pId)){
                    /*Change in the first airplane of airline2*/
                    pr2 = aId1_airplane;
                    aId1_airline->p_root = aId1_airplane->next;
                    pr2->next = aId2_airplane;
                    aId2_airline->p_root = pr2;
                    aId1_airplane = aId1_airline->p_root;
                    aId2_airplane = aId2_airline->p_root;
                }else{
                    while( (aId2_airplane->next != NULL) && (aId1_airplane->pId > aId2_airplane->next->pId)){
                        pr2 = aId2_airplane;
                        aId2_airplane = aId2_airplane->next;
                    }
                    pr1 = aId1_airplane;
                    aId1_airline->p_root = aId1_airplane->next;
                    pr1->next = aId2_airplane->next;
                    aId2_airplane/*pr2*/->next = pr1;
                }
                aId1_airplane = aId1_airline->p_root;
            }
        }
        free(aId1_airline->p_root);
        if((aId1_airline->next == airlines_p) && (aId1_airline->prev == airlines_p)){
            airlines_p = NULL;
        }else if(aId1_airline == airlines_p ){
            prev_1 = airlines_p->prev;
            airlines_p = airlines_p->next;
            prev_1->next = airlines_p;
            airlines_p->prev = prev_1;
        }else if(aId1_airline->next == airlines_p){
            prev_1->next = airlines_p;
            airlines_p->prev = prev_1;
        }else{
            _airlines_l *tmp = aId1_airline->next;
            prev_1->next = tmp;
            tmp->prev = prev_1;
        }
        free(aId1_airline);
    }

    /*Print*/
    /*printf("A %d %d\n", aId1, aId2);*/
    print2();
    return 1;
}

/**
 * @brief Subsidiary company
 *
 * @param aId1         The first airline id
 * @param aId2         The second airline id
 * @param dest         Destination
 *
 * @return 1 on success
 *         0 on failure
 */
int subsidiary_airiline(int aId1, int aId2, int dest){
    _airlines_l *aId1_airline = airlines_p;
    _airlines_l *aId2_airline = airlines_p;

    if (airlines_p != NULL){
        if (aId1_airline->aId != aId1){
            aId1_airline = aId1_airline->next;
        }
        while ((aId1_airline->aId != aId1) && (aId1_airline != airlines_p)){
            aId1_airline = aId1_airline->next;
        }
        if (aId2_airline->aId != aId2){
            aId2_airline = aId2_airline->next;
        }
        while (aId2_airline->aId != aId2 && (aId2_airline != airlines_p)){
            aId2_airline = aId2_airline->next;
        }
        if((aId1_airline->aId == aId1) && (aId2_airline->aId == aId2)){ /*If it was found*/
            _airplane *aId1_airplane = aId1_airline->p_root;
            _airplane *aId2_airplane = aId2_airline->p_root;
            _airplane *pr2 = aId2_airplane;
            _airplane *pr1 = aId1_airplane;
            while (aId1_airplane != NULL){
                if(aId1_airplane->dest == dest){
                    if (aId1_airplane->pId == aId1_airline->p_root->pId){ /*Case for root1*/
                        if ((aId2_airline->p_root == NULL) || ((aId2_airplane->pId == aId2_airline->p_root->pId) && (aId1_airplane->pId < aId2_airplane->pId))){
                            /*Case for change in root2*/
                            aId2_airline->p_root = aId1_airplane;
                            aId1_airline->p_root = aId1_airplane->next;
                            aId1_airplane->next = aId2_airplane;
                            aId1_airplane = aId1_airline->p_root;
                            pr1 = aId1_airplane;
                            pr2 = aId2_airline->p_root;
                        }else{ /*Case for not in root2*/
                            while( (aId2_airplane != NULL) && (aId1_airplane->pId > aId2_airplane->pId)){
                                pr2 = aId2_airplane;
                                aId2_airplane = aId2_airplane->next;
                            }
                            pr2->next = aId1_airplane;
                            aId1_airline->p_root = aId1_airplane->next;
                            aId1_airplane->next = aId2_airplane;
                            aId1_airplane = aId1_airline->p_root;
                            pr1 = aId1_airplane;
                        }
                    }else { /*Case for not in root1*/
                        if ((aId2_airline->p_root == NULL) || ((aId2_airplane->pId == aId2_airline->p_root->pId) && (aId1_airplane->pId < aId2_airplane->pId))){
                            /*Case for change in root2*/
                            aId2_airline->p_root = aId1_airplane;
                            pr1->next = aId1_airplane->next;
                            aId1_airplane->next = pr2;
                            pr2 = aId2_airline->p_root;
                            aId1_airplane = pr1->next;
                        }else{ /*Case for not in root2*/
                            while( (aId2_airplane != NULL) && (aId1_airplane->pId > aId2_airplane->pId)){
                                pr2 = aId2_airplane;
                                aId2_airplane = aId2_airplane->next;
                            }
                            pr2->next = aId1_airplane;
                            pr1->next = aId1_airplane->next;
                            aId1_airplane->next = aId2_airplane;
                            aId1_airplane = pr1->next;
                        }
                    }
                }else{ /*Case for airplane1->dest != dest*/
                    pr1 = aId1_airplane;
                    aId1_airplane = pr1->next;
                }
            }
        }
    }

    /*Print*/
    /*printf("S %d %d %d\n", aId1, aId2, dest);*/
    print2();
    return 1;
}


/**
 * @brief Partition airplanes
 *
 * @param aId         The airline id
 *
 * @return 1 on success
 *         0 on failure
 */
int partition_airplanes(int aId1) {
    _airlines_l *airline = airlines_p;
    _airlines_l *prev_1 = NULL;
    _airlines_l *next;
    _airlines_l *prev;
    /*Finds the right node in airline*/
    if(airline->aId != aId1){
        airline = airline->next;
    }
    while((airline->aId != aId1) && (airline != airlines_p)){
        airline = airline->next;
    }
    if ((airline->aId == aId1) && (airline != airline->next)){
        next = airline;
        prev = airline;
        while(airline->p_root != NULL){
            _airplane *airplane_1 = airline->p_root;
            _airplane *airplane_2;
            _airplane *pre = NULL;

            next = next->next;
            airplane_2 = next->p_root;

            if (next->p_root == NULL){
                next->p_root = airplane_1;
                airline->p_root = airplane_1->next;
                airplane_1->next = NULL;
            }else if (airplane_2->pId > airplane_1->pId){
                next->p_root = airplane_1;
                airline->p_root = airplane_1->next;
                airplane_1->next = airplane_2;
            }else{
                while((airplane_2 != NULL) && (airplane_2->pId < airplane_1->pId)){
                    pre = airplane_2;
                    airplane_2 = airplane_2->next;
                }
                if (airplane_1 != airplane_2){
                pre->next = airplane_1;
                airline->p_root = airplane_1->next;
                airplane_1->next = airplane_2;
                }
            }

            airplane_1 = airline->p_root;
            if(airline->p_root != NULL){
                prev = prev->prev;
                airplane_2 = prev->p_root;
                if (prev->p_root == NULL){
                    prev->p_root = airplane_1;
                    airline->p_root = airplane_1->next;
                    airplane_1->next = NULL;
                }else if (prev->p_root->pId > airplane_1->pId){
                    prev->p_root = airplane_1;
                    airline->p_root = airplane_1->next;
                    airplane_1->next = airplane_2;
                }else{
                    while((airplane_2 != NULL) && (airplane_2->pId < airplane_1->pId)){
                        pre = airplane_2;
                        airplane_2 = airplane_2->next;
                    }
                    if(airplane_2 != airplane_1){
                    pre->next = airplane_1;
                    airline->p_root = airplane_1->next;
                    airplane_1->next = airplane_2;
                    }
                }
            }
        }
        free(airline->p_root);
        prev_1 = airline->prev;
        if(airline == airlines_p ){
            prev_1 = airlines_p->prev;
            airlines_p = airlines_p->next;
            prev_1->next = airlines_p;
            airlines_p->prev = prev_1;
        }else if(airline->next == airlines_p){
            prev_1->next = airlines_p;
            airlines_p->prev = prev_1;
        }else{
            _airlines_l *tmp = airline->next;
            prev_1->next = tmp;
            tmp->prev = prev_1;
        }
        free(airline);
    }
    /*Print*/
    /*printf("P %d\n", aId1);*/
    print2();
    return 1;
}


/**
 * @brief Travel
 *
 * @param dest         Destination
 * @param timestamp    Departure time
 *
 * @return 1 on success
 *         0 on failure
 */
int travel(int dest, int timestamp) {
    _flights_t *flight = destTable[dest].flight_p;
    /*printf("T %d %d\n",dest , timestamp);*/
    printf("\tDestination %d = ",dest+1);
    if (destTable[dest].flight_p != NULL){
    if (destTable[dest].sentinel_p->depart_time < timestamp){
        flight = destTable[dest].sentinel_p->next;
    }
    while((flight != /*NULL*/ destTable[dest].sentinel_p) && (flight->depart_time <= timestamp)){
        flight = flight->next;
    }
    while(flight != /*NULL*/ destTable[dest].sentinel_p){
        if (flight->next != /*NULL*/ destTable[dest].sentinel_p){
            printf("%d:%d, ",flight->pId, flight->depart_time);
        }else{
            printf("%d:%d",flight->pId, flight->depart_time);
        }
        flight = flight->next;
    }
    printf("\n");
    }
    /*printf("DONE\n");*/
    return 1;
}


/**
 * @brief Print airlines
 *
 * @return 1 on success
 *         0 on failure
 */
int print_airlines() {
    _airlines_l *last = airlines_p;
    int k=1;
    /* printf("X\n");*/
    if (airlines_p != NULL){
    printf("\tAirline 1 = %d\n", last->aId);
    printf("\t\tAirplanes 1 = ");
    _airplane *cur = last->p_root;
    while(cur != NULL){
        if (cur->next == NULL){
            printf("%d ", cur->pId);
        }else{
            printf("%d, ", cur->pId);
        }
        cur = cur->next;
    }
    printf("\n");
    while(last->next != airlines_p){
        last = last->next;
        k++;
        printf("\tAirline %d = %d\n", k, last->aId);
        printf("\t\tAirplanes %d = ",k);
        cur = last->p_root;
        while(cur != NULL){
            if (cur->next == NULL){
                printf("%d ", cur->pId);
            }else{
                printf("%d, ", cur->pId);
            }
            cur = cur->next;
        }
        printf("\n");
    }
    }
    /*printf("DONE\n");*/
    return 1;
}

/**
 * @brief Print destinations
 *
 * @return 1 on success
 *         0 on failure
 */
int print_destinations() {
    int i;
   /* printf("Y\n");*/
    for(i = 0; i < 10; i++){
        _flights_t *cur = destTable[i].flight_p;
        printf("\t Destination %d = ",i+1);
        while(cur != destTable[i].sentinel_p){
            if (cur->next == destTable[i].sentinel_p){
                printf("%d:%d ", cur->pId, cur->depart_time);
            }else{
                printf("%d:%d, ", cur->pId, cur->depart_time);
            }
            cur = cur->next;
        }
        printf("\n");
    }
    /* printf("DONE\n");*/
    return 1;
}

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int free_all(void) {
    while(airlines_p != NULL){
        _airlines_l *del_airline = airlines_p;
        _airlines_l *prev_1 = NULL;
        while(del_airline->p_root != NULL){
            _airplane *c = del_airline->p_root;
            _flights_t *curr = destTable[c->dest].flight_p;
            _flights_t *prev;
            if (curr->pId == c->pId){ /*Case for first in list*/
                destTable[c->dest].flight_p = curr->next;
                destTable[c->dest].sentinel_p->pId = curr->next->pId;
                destTable[c->dest].sentinel_p->depart_time = curr->next->depart_time;
                destTable[c->dest].sentinel_p->next = curr->next;
                free(curr);
            }else{
                while((curr/*->next*/ != destTable[c->dest].sentinel_p) && (curr->pId != c->pId)){
                    prev = curr;
                    curr = curr->next;
                }
                if(curr != destTable[c->dest].sentinel_p){
                    prev->next = curr->next;
                    destTable[c->dest].sentinel_p->pId = prev->pId;
                    destTable[c->dest].sentinel_p->depart_time = prev->depart_time;
                    destTable[c->dest].sentinel_p->next = prev;
                    free(curr);
                }
            }
            del_airline->p_root = c->next;
            free(c);
        }
        free(del_airline->p_root);
        if((del_airline->next == airlines_p) && (del_airline->prev == airlines_p)){
            airlines_p = NULL;
        }else {
            prev_1 = airlines_p->prev;
            airlines_p = airlines_p->next;
            prev_1->next = airlines_p;
            airlines_p->prev = prev_1;
        }
        free(del_airline);
    }
    int i;
    for (i=0; i<10; i++){
        if (destTable[i].sentinel_p != NULL){
            free(destTable[i].sentinel_p);
        }
    }
    return 1;
}
