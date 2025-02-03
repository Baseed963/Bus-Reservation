#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUSES 10
#define SEATS_PER_BUS 32

typedef struct {
    int seat_number;
    char passenger_name[50];
} Seat;

typedef struct {
    char bus_number[10];
    char driver[50];
    char arrival[10];
    char departure[10];
    char from[50];
    char to[50];
    Seat seats[SEATS_PER_BUS];
} Bus;

Bus buses[MAX_BUSES];
int bus_count = 0;

void load_buses() {
    FILE *file = fopen("buses.dat", "rb");
    if (file != NULL) {
        fread(&bus_count, sizeof(int), 1, file);
        fread(buses, sizeof(Bus), bus_count, file);
        fclose(file);
    }
}

void save_buses() {
    FILE *file = fopen("buses.dat", "wb");
    if (file != NULL) {
        fwrite(&bus_count, sizeof(int), 1, file);
        fwrite(buses, sizeof(Bus), bus_count, file);
        fclose(file);
    }
}

void add_bus() {
    if (bus_count >= MAX_BUSES) {
        printf("Cannot add more buses.\n");
        return;
    }
    Bus *bus = &buses[bus_count];

    printf("Enter bus number: ");
    scanf("%9s", bus->bus_number);

    printf("Enter driver's name: ");
    scanf(" %49[^\n]", bus->driver); 

    printf("Enter arrival time: ");
    scanf("%9s", bus->arrival);

    printf("Enter departure time: ");
    scanf("%9s", bus->departure);

    printf("Enter origin: ");
    scanf(" %49[^\n]", bus->from);

    printf("Enter destination: ");
    scanf(" %49[^\n]", bus->to);

    for (int i = 0; i < SEATS_PER_BUS; i++) {
        bus->seats[i].seat_number = i + 1;
        strcpy(bus->seats[i].passenger_name, "Empty");
    }

    bus_count++;
    save_buses();
    printf("Bus added successfully.\n");
}

void view_buses() {
    if (bus_count == 0) {
        printf("No buses available.\n");
        return;
    }
    for (int i = 0; i < bus_count; i++) {
        Bus *bus = &buses[i];
        printf("\nBus Number: %s\n", bus->bus_number);
        printf("Driver: %s\n", bus->driver);
        printf("Arrival Time: %s\n", bus->arrival);
        printf("Departure Time: %s\n", bus->departure);
        printf("From: %s\n", bus->from);
        printf("To: %s\n", bus->to);
    }
}

Bus* find_bus(char *bus_number) {
    for (int i = 0; i < bus_count; i++) {
        if (strcmp(buses[i].bus_number, bus_number) == 0) {
            return &buses[i];
        }
    }
    return NULL;
}

void book_seat() {
    char bus_number[10];
    printf("Enter bus number: ");
    scanf("%9s", bus_number);
    Bus *bus = find_bus(bus_number);
    if (bus == NULL) {
        printf("Bus not found.\n");
        return;
    }
    int seat_number;
    printf("Enter seat number (1-%d): ", SEATS_PER_BUS);
    scanf("%d", &seat_number);

    if (seat_number < 1 || seat_number > SEATS_PER_BUS) {
        printf("Invalid seat number.\n");
        return;
    }

    Seat *seat = &bus->seats[seat_number - 1];
    if (strcmp(seat->passenger_name, "Empty") != 0) {
        printf("Seat already booked.\n");
        return;
    }

    printf("Enter passenger name: ");
    scanf(" %49[^\n]", seat->passenger_name);

    save_buses();
    printf("Seat booked successfully.\n");
}

void cancel_reservation() {
    char bus_number[10];
    printf("Enter bus number: ");
    scanf("%9s", bus_number);
    Bus *bus = find_bus(bus_number);
    if (bus == NULL) {
        printf("Bus not found.\n");
        return;
    }

    int seat_number;
    printf("Enter seat number (1-%d): ", SEATS_PER_BUS);
    scanf("%d", &seat_number);

    if (seat_number < 1 || seat_number > SEATS_PER_BUS) {
        printf("Invalid seat number.\n");
        return;
    }

    Seat *seat = &bus->seats[seat_number - 1];
    if (strcmp(seat->passenger_name, "Empty") == 0) {
        printf("Seat is already empty.\n");
        return;
    }

    strcpy(seat->passenger_name, "Empty");
    save_buses();
    printf("Reservation cancelled successfully.\n");
}

void view_seats() {
    char bus_number[10];
    printf("Enter bus number: ");
    scanf("%9s", bus_number);
    Bus *bus = find_bus(bus_number);
    if (bus == NULL) {
        printf("Bus not found.\n");
        return;
    }

    printf("Seat Number\tPassenger Name\n");
    for (int i = 0; i < SEATS_PER_BUS; i++) {
        Seat *seat = &bus->seats[i];
        printf("%d\t\t%s\n", seat->seat_number, seat->passenger_name);
    }
}

int main() {
    load_buses();
    int choice;
    while (1) {
        printf("\nBus Reservation System\n");
        printf("1. Add Bus\n");
        printf("2. View Buses\n");
        printf("3. Book Seat\n");
        printf("4. Cancel Reservation\n");
        printf("5. View Seats\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }

        switch (choice) {
            case 1:
                add_bus();
                break;
            case 2:
                view_buses();
                break;
            case 3:
                book_seat();
                break;
            case 4:
                cancel_reservation();
                break;
            case 5:
                view_seats();
                break;
            case 6:
                printf("Exiting the system.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
