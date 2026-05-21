#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 5          
#define NAME_LEN  40

typedef struct Passenger {
    int  pnr;
    char name[NAME_LEN];
    int  age;
    char gender;             
    int  seatNo;            
    struct Passenger *next;
} Passenger;

Passenger *confirmedHead = NULL;  
Passenger *waitFront = NULL;      
Passenger *waitRear  = NULL;       

int seat[MAX_SEATS + 1];           
int pnrCounter = 1000;            
int confirmedCount = 0;

int generatePNR(void) { return ++pnrCounter; }

int firstFreeSeat(void) {
    int i;
    for (i = 1; i <= MAX_SEATS; i++)
        if (seat[i] == 0) return i;
    return -1;                      
}

Passenger *createPassenger(char *name, int age, char gender) {
    Passenger *p = (Passenger *) malloc(sizeof(Passenger));
    p->pnr    = generatePNR();
    strncpy(p->name, name, NAME_LEN - 1);
    p->name[NAME_LEN - 1] = '\0';
    p->age    = age;
    p->gender = gender;
    p->seatNo = 0;
    p->next   = NULL;
    return p;
}

void addConfirmed(Passenger *p) {
    p->next = NULL;
    if (confirmedHead == NULL) {
        confirmedHead = p;
    } else {
        Passenger *t = confirmedHead;
        while (t->next != NULL) t = t->next;
        t->next = p;
    }
    confirmedCount++;
}

void enqueueWaiting(Passenger *p) {
    p->next = NULL;
    if (waitRear == NULL) {
        waitFront = waitRear = p;
    } else {
        waitRear->next = p;
        waitRear = p;
    }
}

Passenger *dequeueWaiting(void) {
    if (waitFront == NULL) return NULL;
    Passenger *p = waitFront;
    waitFront = waitFront->next;
    if (waitFront == NULL) waitRear = NULL;
    p->next = NULL;
    return p;
}

void bookTicket(void) {
    char name[NAME_LEN], gender;
    int  age;

    printf("\nEnter passenger name : ");
    scanf(" %39[^\n]", name);
    printf("Enter age            : ");
    scanf("%d", &age);
    printf("Enter gender (M/F)   : ");
    scanf(" %c", &gender);

    Passenger *p = createPassenger(name, age, gender);
    int s = firstFreeSeat();

    if (s != -1) {                     
        seat[s] = p->pnr;
        p->seatNo = s;
        addConfirmed(p);
        printf("\n>>> Ticket CONFIRMED.\n");
        printf("    PNR: %d | Seat No: %d\n", p->pnr, p->seatNo);
    } else {                           
        enqueueWaiting(p);
        printf("\n>>> Coach full. Added to WAITING LIST.\n");
        printf("    PNR: %d\n", p->pnr);
    }
}

void cancelTicket(void) {
    int target;
    printf("\nEnter PNR to cancel : ");
    scanf("%d", &target);

    Passenger *cur = confirmedHead, *prev = NULL;
    while (cur != NULL && cur->pnr != target) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        printf("\n>>> PNR %d not found in confirmed list.\n", target);
        return;
    }

    int freedSeat = cur->seatNo;
    if (prev == NULL) confirmedHead = cur->next;
    else              prev->next = cur->next;
    seat[freedSeat] = 0;
    confirmedCount--;
    printf("\n>>> PNR %d (%s) cancelled. Seat %d freed.\n",
           cur->pnr, cur->name, freedSeat);
    free(cur);

    Passenger *next = dequeueWaiting();
    if (next != NULL) {
        seat[freedSeat] = next->pnr;
        next->seatNo = freedSeat;
        addConfirmed(next);
        printf(">>> Waiting passenger %s (PNR %d) auto-confirmed to seat %d.\n",
               next->name, next->pnr, freedSeat);
    }
}

void displayConfirmed(void) {
    printf("\n------------- CONFIRMED RESERVATION CHART -------------\n");
    if (confirmedHead == NULL) {
        printf("No confirmed bookings yet.\n");
    } else {
        printf("%-8s %-20s %-5s %-7s %-5s\n",
               "PNR", "NAME", "AGE", "GENDER", "SEAT");
        Passenger *t = confirmedHead;
        while (t != NULL) {
            printf("%-8d %-20s %-5d %-7c %-5d\n",
                   t->pnr, t->name, t->age, t->gender, t->seatNo);
            t = t->next;
        }
    }
    printf("Seats occupied: %d / %d\n", confirmedCount, MAX_SEATS);
    printf("-------------------------------------------------------\n");
}

void displayWaiting(void) {
    printf("\n----------------- WAITING LIST -----------------\n");
    if (waitFront == NULL) {
        printf("Waiting list is empty.\n");
    } else {
        printf("%-4s %-8s %-20s %-5s\n", "POS", "PNR", "NAME", "AGE");
        Passenger *t = waitFront;
        int pos = 1;
        while (t != NULL) {
            printf("%-4d %-8d %-20s %-5d\n", pos++, t->pnr, t->name, t->age);
            t = t->next;
        }
    }
    printf("------------------------------------------------\n");
}

void menu(void) {
    printf("\n========= RAILWAY RESERVATION SYSTEM =========\n");
    printf("1. Book Ticket\n");
    printf("2. Cancel Ticket\n");
    printf("3. Display Confirmed Chart\n");
    printf("4. Display Waiting List\n");
    printf("5. Exit\n");
    printf("==============================================\n");
    printf("Enter your choice : ");
}

int main(void) {
    int choice;
    while (1) {
        menu();
        if (scanf("%d", &choice) != 1) break;
        switch (choice) {
            case 1: bookTicket();      break;
            case 2: cancelTicket();    break;
            case 3: displayConfirmed();break;
            case 4: displayWaiting();  break;
            case 5: printf("\nThank you for using the system. Goodbye!\n");
                    return 0;
            default: printf("\nInvalid choice. Try again.\n");
        }
    }
    return 0;
}
