#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 5
#define WMAX 5

struct Passenger {
    int id;
    char name[50];
    struct Passenger* next;
};

struct Stack {
    int data[100];
    int top;
};

struct Queue {
    int ids[WMAX];
    char names[WMAX][50];
    int front, rear;
};

struct Passenger* head = NULL;
int seatCount = 0;
int idCounter = 1;

struct Stack cancelStack;
struct Queue waitQueue;

void initStack() {
    cancelStack.top = -1;
}

void push(int id) {
    cancelStack.data[++cancelStack.top] = id;
}

void initQueue() {
    waitQueue.front = waitQueue.rear = -1;
}

int isQueueFull() {
    return (waitQueue.front == (waitQueue.rear + 1) % WMAX);
}

int isQueueEmpty() {
    return (waitQueue.front == -1);
}

void enqueue(int id, char name[]) {
    if (isQueueFull()) {
        printf("Waiting list is full!\n");
        return;
    }

    if (isQueueEmpty())
        waitQueue.front = 0;

    waitQueue.rear = (waitQueue.rear + 1) % WMAX;
    waitQueue.ids[waitQueue.rear] = id;
    strcpy(waitQueue.names[waitQueue.rear], name);

    printf("Added to waiting list.\n");
}

void dequeue(char name[]) {
    if (isQueueEmpty()) return;

    int id = waitQueue.ids[waitQueue.front];
    strcpy(name, waitQueue.names[waitQueue.front]);

    if (waitQueue.front == waitQueue.rear)
        waitQueue.front = waitQueue.rear = -1;
    else
        waitQueue.front = (waitQueue.front + 1) % WMAX;

    struct Passenger* newNode = (struct Passenger*)malloc(sizeof(struct Passenger));
    newNode->id = id;
    strcpy(newNode->name, name);
    newNode->next = head;
    head = newNode;

    seatCount++;
    printf("Waiting passenger %s got confirmed seat!\n", name);
}

void bookTicket() {
    char name[50];
    printf("Enter Name: ");
    scanf("%s", name);

    if (seatCount < MAX) {
        struct Passenger* newNode = (struct Passenger*)malloc(sizeof(struct Passenger));
        newNode->id = idCounter++;
        strcpy(newNode->name, name);
        newNode->next = head;
        head = newNode;

        seatCount++;
        printf("Ticket Booked! ID: %d\n", newNode->id);
    } else {
        enqueue(idCounter++, name);
    }
}

void cancelTicket() {
    int id;
    printf("Enter ID to cancel: ");
    scanf("%d", &id);

    struct Passenger *temp = head, *prev = NULL;

    while (temp != NULL) {
        if (temp->id == id) {
            if (prev == NULL)
                head = temp->next;
            else
                prev->next = temp->next;

            free(temp);
            seatCount--;

            push(id);
            printf("Ticket Cancelled!\n");

            char name[50];
            dequeue(name);
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    printf("Passenger not found!\n");
}

void displayPassengers() {
    struct Passenger* temp = head;

    if (!temp) {
        printf("No bookings yet.\n");
        return;
    }

    printf("\n--- Booked Passengers ---\n");
    while (temp != NULL) {
        printf("ID: %d | Name: %s\n", temp->id, temp->name);
        temp = temp->next;
    }
}

void displayWaiting() {
    if (isQueueEmpty()) {
        printf("No waiting list.\n");
        return;
    }

    printf("\n--- Waiting List ---\n");
    int i = waitQueue.front;

    while (1) {
        printf("ID: %d | Name: %s\n", waitQueue.ids[i], waitQueue.names[i]);
        if (i == waitQueue.rear) break;
        i = (i + 1) % WMAX;
    }
}

int main() {
    int choice;

    initStack();
    initQueue();

    while (1) {
        printf("\n--- Railway Reservation System ---\n");
        printf("1. Book Ticket\n");
        printf("2. Cancel Ticket\n");
        printf("3. Display Booked Tickets\n");
        printf("4. Display Waiting List\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: bookTicket(); break;
            case 2: cancelTicket(); break;
            case 3: displayPassengers(); break;
            case 4: displayWaiting(); break;
            case 5: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}
