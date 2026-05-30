/*
 * ============================================================
 *  RAILWAY STATION MANAGEMENT SYSTEM
 *  Course  : IN 1101 - Programming Fundamentals
 *  Project : Assignment 2 - Group Console Application
 * ============================================================
 *  MEMBER CONTRIBUTIONS
 *  Member 1 : Train Schedule & Route Management   (struct Train)
 *  Member 2 : Passenger Booking & Ticketing       (struct Passenger, struct Ticket)
 *  Member 3 : Platform & Track Assignment         (struct Platform)
 *  Member 4 : Staff & Crew Management             (struct Staff)
 *  Member 5 : Revenue & Report Generation         (struct Revenue)
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
   CONSTANTS
   ============================================================ */
#define MAX_TRAINS      50
#define MAX_PASSENGERS  100
#define MAX_TICKETS     100
#define MAX_PLATFORMS   10
#define MAX_STAFF       50
#define MAX_REVENUE     30

/* ============================================================
   STRUCTURE DEFINITIONS
   ============================================================ */

/* ---------- Member 1 ---------- */
struct Train {
    int  trainID;
    char trainName[50];
    char origin[30];
    char destination[30];
    char departureTime[6];   /* HH:MM */
    char arrivalTime[6];     /* HH:MM */
    int  totalSeats;
    int  availableSeats;
    int  isDelayed;          /* 0 = No, 1 = Yes */
    int  isCancelled;        /* 0 = No, 1 = Yes */
    int  delayMinutes;
};

/* ---------- Member 2 ---------- */
struct Passenger {
    int  passengerID;
    char name[50];
    char nic[15];
    char contactNo[15];
};

struct Ticket {
    int   ticketID;
    int   passengerID;
    int   trainID;
    char  seatClass[10];   /* "First", "Second", "Third" */
    float fare;
    char  bookingDate[12]; /* DD-MM-YYYY */
    int   isActive;        /* 0 = Cancelled, 1 = Active */
};

/* ---------- Member 3 ---------- */
struct Platform {
    int  platformNo;
    int  isOccupied;        /* 0 = Free, 1 = Occupied */
    int  assignedTrainID;   /* -1 if free */
    char status[20];        /* "Available" or "Occupied" */
};

/* ---------- Member 4 ---------- */
struct Staff {
    int  staffID;
    char name[50];
    char role[30];          /* "Driver", "Guard", "Cleaner", "Ticketer" */
    int  assignedTrainID;   /* -1 if unassigned */
    char shift[10];         /* "Morning", "Evening", "Night" */
    char contactNo[15];
};

/* ---------- Member 5 ---------- */
struct Revenue {
    char  date[12];         /* DD-MM-YYYY */
    float totalTicketSales;
    int   totalPassengers;
    float expenses;
    float netProfit;
};

/* ============================================================
   GLOBAL ARRAYS
   ============================================================ */
struct Train     trains[MAX_TRAINS];
struct Passenger passengers[MAX_PASSENGERS];
struct Ticket    tickets[MAX_TICKETS];
struct Platform  platforms[MAX_PLATFORMS];
struct Staff     staff[MAX_STAFF];
struct Revenue   revenues[MAX_REVENUE];

int trainCount     = 0;
int passengerCount = 0;
int ticketCount    = 0;
int platformCount  = 0;
int staffCount     = 0;
int revenueCount   = 0;

/* ============================================================
   UTILITY / HELPER FUNCTIONS
   ============================================================ */

/* Clear the console screen */
void clearScreen() {
    system("cls");
}

/* Print a divider line */
void printLine() {
    printf("============================================================\n");
}

/* Press enter to continue */
void pause() {
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
}

/* ============================================================
   MODULE 1 : TRAIN SCHEDULE & ROUTE MANAGEMENT
   Member 1
   ============================================================ */

/* 1.1 Add a new train */
void addTrain() {
    if (trainCount >= MAX_TRAINS) {
        printf("ERROR: Train storage is full!\n");
        return;
    }

    struct Train t;
    printf("\n--- ADD NEW TRAIN ---\n");

    t.trainID = trainCount + 1001; /* ID starts from 1001 */

    printf("Train Name       : "); scanf(" %[^\n]", t.trainName);
    printf("Origin Station   : "); scanf(" %[^\n]", t.origin);
    printf("Destination      : "); scanf(" %[^\n]", t.destination);
    printf("Departure (HH:MM): "); scanf(" %5s", t.departureTime);
    printf("Arrival   (HH:MM): "); scanf(" %5s", t.arrivalTime);
    printf("Total Seats      : "); scanf("%d", &t.totalSeats);

    t.availableSeats = t.totalSeats;
    t.isDelayed      = 0;
    t.isCancelled    = 0;
    t.delayMinutes   = 0;

    trains[trainCount++] = t;
    printf("\nSUCCESS: Train [%s] added with ID %d.\n", t.trainName, t.trainID);
}

/* 1.2 Display all trains */
void displayAllTrains() {
    if (trainCount == 0) {
        printf("\nNo trains available.\n");
        return;
    }

    printf("\n--- ALL TRAINS ---\n");
    printLine();
    printf("%-6s %-20s %-15s %-15s %-8s %-8s %-6s %-8s %-10s\n",
           "ID", "Name", "From", "To", "Depart", "Arrive",
           "Seats", "Avail", "Status");
    printLine();

    int i;
    for (i = 0; i < trainCount; i++) {
        char status[20];
        if (trains[i].isCancelled)
            strcpy(status, "CANCELLED");
        else if (trains[i].isDelayed)
            sprintf(status, "DELAYED+%dmin", trains[i].delayMinutes);
        else
            strcpy(status, "ON TIME");

        printf("%-6d %-20s %-15s %-15s %-8s %-8s %-6d %-8d %-10s\n",
               trains[i].trainID,
               trains[i].trainName,
               trains[i].origin,
               trains[i].destination,
               trains[i].departureTime,
               trains[i].arrivalTime,
               trains[i].totalSeats,
               trains[i].availableSeats,
               status);
    }
    printLine();
}

/* 1.3 Search train by ID */
void searchTrainByID() {
    int id, i, found = 0;
    printf("\nEnter Train ID to search: ");
    scanf("%d", &id);

    for (i = 0; i < trainCount; i++) {
        if (trains[i].trainID == id) {
            found = 1;
            printf("\n--- TRAIN DETAILS ---\n");
            printLine();
            printf("Train ID     : %d\n",   trains[i].trainID);
            printf("Name         : %s\n",   trains[i].trainName);
            printf("Origin       : %s\n",   trains[i].origin);
            printf("Destination  : %s\n",   trains[i].destination);
            printf("Departure    : %s\n",   trains[i].departureTime);
            printf("Arrival      : %s\n",   trains[i].arrivalTime);
            printf("Total Seats  : %d\n",   trains[i].totalSeats);
            printf("Avail Seats  : %d\n",   trains[i].availableSeats);
            printf("Delayed      : %s\n",   trains[i].isDelayed ? "Yes" : "No");
            printf("Cancelled    : %s\n",   trains[i].isCancelled ? "Yes" : "No");
            if (trains[i].isDelayed)
                printf("Delay (mins) : %d\n", trains[i].delayMinutes);
            printLine();
            break;
        }
    }
    if (!found)
        printf("ERROR: Train ID %d not found.\n", id);
}

/* 1.4 Sort trains by departure time (Bubble Sort) */
void sortTrainsByDeparture() {
    int i, j;
    struct Train temp;

    for (i = 0; i < trainCount - 1; i++) {
        for (j = 0; j < trainCount - i - 1; j++) {
            if (strcmp(trains[j].departureTime, trains[j + 1].departureTime) > 0) {
                temp        = trains[j];
                trains[j]   = trains[j + 1];
                trains[j+1] = temp;
            }
        }
    }
    printf("\nSUCCESS: Trains sorted by departure time.\n");
    displayAllTrains();
}

/* 1.5 Update train delay */
void updateTrainDelay() {
    int id, i, found = 0, mins;
    printf("\nEnter Train ID to update delay: ");
    scanf("%d", &id);

    for (i = 0; i < trainCount; i++) {
        if (trains[i].trainID == id) {
            found = 1;
            printf("Enter delay in minutes (0 = no delay): ");
            scanf("%d", &mins);
            if (mins <= 0) {
                trains[i].isDelayed    = 0;
                trains[i].delayMinutes = 0;
                printf("Train %d is now ON TIME.\n", id);
            } else {
                trains[i].isDelayed    = 1;
                trains[i].delayMinutes = mins;
                printf("Train %d delayed by %d minutes.\n", id, mins);
            }
            break;
        }
    }
    if (!found)
        printf("ERROR: Train ID %d not found.\n", id);
}

/* 1.6 Cancel a train */
void cancelTrain() {
    int id, i, found = 0;
    printf("\nEnter Train ID to cancel: ");
    scanf("%d", &id);

    for (i = 0; i < trainCount; i++) {
        if (trains[i].trainID == id) {
            found = 1;
            if (trains[i].isCancelled) {
                printf("Train %d is already cancelled.\n", id);
            } else {
                trains[i].isCancelled = 1;
                printf("SUCCESS: Train %d [%s] has been CANCELLED.\n",
                       id, trains[i].trainName);
            }
            break;
        }
    }
    if (!found)
        printf("ERROR: Train ID %d not found.\n", id);
}

/* Module 1 Sub-Menu */
void trainMenu() {
    int choice;
    do {
        clearScreen();
        printLine();
        printf("  MODULE 1 : TRAIN SCHEDULE & ROUTE MANAGEMENT\n");
        printLine();
        printf("  1. Add New Train\n");
        printf("  2. Display All Trains\n");
        printf("  3. Search Train by ID\n");
        printf("  4. Sort Trains by Departure Time\n");
        printf("  5. Update Train Delay\n");
        printf("  6. Cancel Train\n");
        printf("  0. Back to Main Menu\n");
        printLine();
        printf("  Enter choice: ");
        scanf("%d", &choice);

        clearScreen();
        switch (choice) {
            case 1: addTrain();              break;
            case 2: displayAllTrains();      break;
            case 3: searchTrainByID();       break;
            case 4: sortTrainsByDeparture(); break;
            case 5: updateTrainDelay();      break;
            case 6: cancelTrain();           break;
            case 0: break;
            default: printf("Invalid choice. Try again.\n");
        }
        if (choice != 0) pause();
    } while (choice != 0);
}

/* ============================================================
   MODULE 2 : PASSENGER BOOKING & TICKETING
   Member 2
   ============================================================ */

/* 2.1 Register a new passenger */
void registerPassenger() {
    if (passengerCount >= MAX_PASSENGERS) {
        printf("ERROR: Passenger storage is full!\n");
        return;
    }

    struct Passenger p;
    printf("\n--- REGISTER PASSENGER ---\n");

    p.passengerID = passengerCount + 2001;

    printf("Full Name    : "); scanf(" %[^\n]", p.name);
    printf("NIC Number   : "); scanf(" %14s", p.nic);
    printf("Contact No   : "); scanf(" %14s", p.contactNo);

    /* Validate NIC length */
    int nicLen = strlen(p.nic);
    if (nicLen != 9 && nicLen != 12) {
        printf("WARNING: NIC should be 9 or 12 characters. Saved anyway.\n");
    }

    passengers[passengerCount++] = p;
    printf("SUCCESS: Passenger [%s] registered with ID %d.\n",
           p.name, p.passengerID);
}

/* 2.2 Calculate fare based on class */
float calculateFare(int trainID, char *seatClass) {
    float baseFare = 500.0f; /* Base fare in LKR */
    int i;

    /* Adjust base fare by seat class */
    float multiplier = 1.0f;
    if (strcmp(seatClass, "First") == 0)       multiplier = 3.0f;
    else if (strcmp(seatClass, "Second") == 0) multiplier = 2.0f;
    else                                        multiplier = 1.0f; /* Third */

    /* Add distance factor (each train gets a unique factor from its ID) */
    for (i = 0; i < trainCount; i++) {
        if (trains[i].trainID == trainID) {
            baseFare += (trainID % 100) * 10.0f;
            break;
        }
    }

    return baseFare * multiplier;
}

/* 2.3 Book a ticket */
void bookTicket() {
    if (ticketCount >= MAX_TICKETS) {
        printf("ERROR: Ticket storage is full!\n");
        return;
    }

    int pID, tID, i;
    int passengerFound = 0, trainFound = 0, trainIndex = -1;

    printf("\n--- BOOK TICKET ---\n");
    printf("Enter Passenger ID : "); scanf("%d", &pID);
    printf("Enter Train ID     : "); scanf("%d", &tID);

    /* Validate passenger */
    for (i = 0; i < passengerCount; i++) {
        if (passengers[i].passengerID == pID) {
            passengerFound = 1;
            break;
        }
    }
    if (!passengerFound) { printf("ERROR: Passenger ID not found.\n"); return; }

    /* Validate train */
    for (i = 0; i < trainCount; i++) {
        if (trains[i].trainID == tID) {
            trainFound  = 1;
            trainIndex  = i;
            break;
        }
    }
    if (!trainFound) { printf("ERROR: Train ID not found.\n"); return; }

    /* Check cancellation */
    if (trains[trainIndex].isCancelled) {
        printf("ERROR: Train %d is cancelled. Cannot book.\n", tID);
        return;
    }

    /* Check seat availability */
    if (trains[trainIndex].availableSeats <= 0) {
        printf("ERROR: No available seats on this train.\n");
        return;
    }

    struct Ticket tk;
    tk.ticketID    = ticketCount + 3001;
    tk.passengerID = pID;
    tk.trainID     = tID;
    tk.isActive    = 1;

    printf("Seat Class (First/Second/Third): "); scanf(" %9s", tk.seatClass);

    /* Validate seat class */
    if (strcmp(tk.seatClass, "First")  != 0 &&
        strcmp(tk.seatClass, "Second") != 0 &&
        strcmp(tk.seatClass, "Third")  != 0) {
        printf("ERROR: Invalid seat class. Use First, Second, or Third.\n");
        return;
    }

    printf("Booking Date (DD-MM-YYYY)       : "); scanf(" %11s", tk.bookingDate);

    tk.fare = calculateFare(tID, tk.seatClass);
    trains[trainIndex].availableSeats--;

    tickets[ticketCount++] = tk;

    printf("\n--- BOOKING CONFIRMATION ---\n");
    printLine();
    printf("Ticket ID   : %d\n",    tk.ticketID);
    printf("Passenger ID: %d\n",    tk.passengerID);
    printf("Train ID    : %d\n",    tk.trainID);
    printf("Seat Class  : %s\n",    tk.seatClass);
    printf("Fare        : LKR %.2f\n", tk.fare);
    printf("Date        : %s\n",    tk.bookingDate);
    printLine();

    /* Update revenue for this booking */
    if (revenueCount > 0) {
        revenues[revenueCount - 1].totalTicketSales += tk.fare;
        revenues[revenueCount - 1].totalPassengers++;
        revenues[revenueCount - 1].netProfit =
            revenues[revenueCount - 1].totalTicketSales -
            revenues[revenueCount - 1].expenses;
    }
}

/* 2.4 Cancel a ticket */
void cancelTicket() {
    int tkID, i, found = 0;
    printf("\nEnter Ticket ID to cancel: ");
    scanf("%d", &tkID);

    for (i = 0; i < ticketCount; i++) {
        if (tickets[i].ticketID == tkID) {
            found = 1;
            if (!tickets[i].isActive) {
                printf("Ticket %d is already cancelled.\n", tkID);
            } else {
                tickets[i].isActive = 0;
                /* Return seat to train */
                int j;
                for (j = 0; j < trainCount; j++) {
                    if (trains[j].trainID == tickets[i].trainID) {
                        trains[j].availableSeats++;
                        break;
                    }
                }
                printf("SUCCESS: Ticket %d has been cancelled. Seat released.\n", tkID);
            }
            break;
        }
    }
    if (!found)
        printf("ERROR: Ticket ID %d not found.\n", tkID);
}

/* 2.5 View all booking history */
void viewBookingHistory() {
    if (ticketCount == 0) {
        printf("\nNo tickets booked yet.\n");
        return;
    }

    printf("\n--- BOOKING HISTORY ---\n");
    printLine();
    printf("%-8s %-10s %-8s %-8s %-8s %-12s %-10s\n",
           "TicketID", "PsgrID", "TrainID", "Class", "Fare", "Date", "Status");
    printLine();

    int i;
    for (i = 0; i < ticketCount; i++) {
        printf("%-8d %-10d %-8d %-8s %-8.2f %-12s %-10s\n",
               tickets[i].ticketID,
               tickets[i].passengerID,
               tickets[i].trainID,
               tickets[i].seatClass,
               tickets[i].fare,
               tickets[i].bookingDate,
               tickets[i].isActive ? "Active" : "Cancelled");
    }
    printLine();
}

/* Module 2 Sub-Menu */
void bookingMenu() {
    int choice;
    do {
        clearScreen();
        printLine();
        printf("  MODULE 2 : PASSENGER BOOKING & TICKETING\n");
        printLine();
        printf("  1. Register New Passenger\n");
        printf("  2. Book Ticket\n");
        printf("  3. Cancel Ticket\n");
        printf("  4. View Booking History\n");
        printf("  5. Calculate Fare (Preview)\n");
        printf("  0. Back to Main Menu\n");
        printLine();
        printf("  Enter choice: ");
        scanf("%d", &choice);

        clearScreen();
        switch (choice) {
            case 1: registerPassenger();   break;
            case 2: bookTicket();          break;
            case 3: cancelTicket();        break;
            case 4: viewBookingHistory();  break;
            case 5: {
                int tid; char cls[10];
                printf("Train ID: "); scanf("%d", &tid);
                printf("Seat Class (First/Second/Third): "); scanf(" %9s", cls);
                printf("Estimated Fare: LKR %.2f\n", calculateFare(tid, cls));
                break;
            }
            case 0: break;
            default: printf("Invalid choice. Try again.\n");
        }
        if (choice != 0) pause();
    } while (choice != 0);
}

/* ============================================================
   MODULE 3 : PLATFORM & TRACK ASSIGNMENT
   Member 3
   ============================================================ */

/* 3.1 Add a new platform */
void addPlatform() {
    if (platformCount >= MAX_PLATFORMS) {
        printf("ERROR: Platform storage is full!\n");
        return;
    }

    struct Platform p;
    printf("\n--- ADD PLATFORM ---\n");
    printf("Platform Number: "); scanf("%d", &p.platformNo);

    /* Check for duplicate */
    int i;
    for (i = 0; i < platformCount; i++) {
        if (platforms[i].platformNo == p.platformNo) {
            printf("ERROR: Platform %d already exists.\n", p.platformNo);
            return;
        }
    }

    p.isOccupied     = 0;
    p.assignedTrainID = -1;
    strcpy(p.status, "Available");

    platforms[platformCount++] = p;
    printf("SUCCESS: Platform %d added.\n", p.platformNo);
}

/* 3.2 Assign a train to a platform */
void assignTrainToPlatform() {
    int pNo, tID, i;
    int pFound = 0, tFound = 0;

    printf("\n--- ASSIGN TRAIN TO PLATFORM ---\n");
    printf("Enter Platform Number : "); scanf("%d", &pNo);
    printf("Enter Train ID        : "); scanf("%d", &tID);

    for (i = 0; i < platformCount; i++) {
        if (platforms[i].platformNo == pNo) {
            pFound = 1;
            if (platforms[i].isOccupied) {
                printf("ERROR: Platform %d is already occupied by Train %d.\n",
                       pNo, platforms[i].assignedTrainID);
                return;
            }
            /* Validate train */
            int j;
            for (j = 0; j < trainCount; j++) {
                if (trains[j].trainID == tID) { tFound = 1; break; }
            }
            if (!tFound) { printf("ERROR: Train ID not found.\n"); return; }

            platforms[i].isOccupied      = 1;
            platforms[i].assignedTrainID = tID;
            strcpy(platforms[i].status, "Occupied");
            printf("SUCCESS: Train %d assigned to Platform %d.\n", tID, pNo);
            break;
        }
    }
    if (!pFound)
        printf("ERROR: Platform %d not found.\n", pNo);
}

/* 3.3 Check platform availability */
void checkPlatformAvailability() {
    int pNo, i, found = 0;
    printf("\nEnter Platform Number: ");
    scanf("%d", &pNo);

    for (i = 0; i < platformCount; i++) {
        if (platforms[i].platformNo == pNo) {
            found = 1;
            printf("\nPlatform %d Status : %s\n", pNo, platforms[i].status);
            if (platforms[i].isOccupied)
                printf("Assigned Train ID  : %d\n", platforms[i].assignedTrainID);
            break;
        }
    }
    if (!found)
        printf("ERROR: Platform %d not found.\n", pNo);
}

/* 3.4 Display all platform statuses */
void displayPlatformStatus() {
    if (platformCount == 0) {
        printf("\nNo platforms registered.\n");
        return;
    }

    printf("\n--- PLATFORM STATUS BOARD ---\n");
    printLine();
    printf("%-12s %-15s %-15s\n", "Platform No", "Status", "Assigned Train");
    printLine();

    int i;
    for (i = 0; i < platformCount; i++) {
        char trainStr[20];
        if (platforms[i].isOccupied)
            sprintf(trainStr, "Train %d", platforms[i].assignedTrainID);
        else
            strcpy(trainStr, "----");

        printf("%-12d %-15s %-15s\n",
               platforms[i].platformNo,
               platforms[i].status,
               trainStr);
    }
    printLine();
}

/* 3.5 Release a platform */
void releasePlatform() {
    int pNo, i, found = 0;
    printf("\nEnter Platform Number to release: ");
    scanf("%d", &pNo);

    for (i = 0; i < platformCount; i++) {
        if (platforms[i].platformNo == pNo) {
            found = 1;
            if (!platforms[i].isOccupied) {
                printf("Platform %d is already free.\n", pNo);
            } else {
                printf("Releasing Train %d from Platform %d...\n",
                       platforms[i].assignedTrainID, pNo);
                platforms[i].isOccupied      = 0;
                platforms[i].assignedTrainID = -1;
                strcpy(platforms[i].status, "Available");
                printf("SUCCESS: Platform %d is now Available.\n", pNo);
            }
            break;
        }
    }
    if (!found)
        printf("ERROR: Platform %d not found.\n", pNo);
}

/* 3.6 Find the next available platform */
void findNextAvailablePlatform() {
    int i;
    for (i = 0; i < platformCount; i++) {
        if (!platforms[i].isOccupied) {
            printf("\nNext Available Platform: Platform %d\n",
                   platforms[i].platformNo);
            return;
        }
    }
    printf("\nNo available platforms at the moment.\n");
}

/* Module 3 Sub-Menu */
void platformMenu() {
    int choice;
    do {
        clearScreen();
        printLine();
        printf("  MODULE 3 : PLATFORM & TRACK ASSIGNMENT\n");
        printLine();
        printf("  1. Add Platform\n");
        printf("  2. Assign Train to Platform\n");
        printf("  3. Check Platform Availability\n");
        printf("  4. Display All Platform Status\n");
        printf("  5. Release Platform\n");
        printf("  6. Find Next Available Platform\n");
        printf("  0. Back to Main Menu\n");
        printLine();
        printf("  Enter choice: ");
        scanf("%d", &choice);

        clearScreen();
        switch (choice) {
            case 1: addPlatform();               break;
            case 2: assignTrainToPlatform();     break;
            case 3: checkPlatformAvailability(); break;
            case 4: displayPlatformStatus();     break;
            case 5: releasePlatform();           break;
            case 6: findNextAvailablePlatform(); break;
            case 0: break;
            default: printf("Invalid choice. Try again.\n");
        }
        if (choice != 0) pause();
    } while (choice != 0);
}

/* ============================================================
   MODULE 4 : STAFF & CREW MANAGEMENT
   Member 4
   ============================================================ */

/* 4.1 Add a new staff member */
void addStaff() {
    if (staffCount >= MAX_STAFF) {
        printf("ERROR: Staff storage is full!\n");
        return;
    }

    struct Staff s;
    printf("\n--- ADD STAFF MEMBER ---\n");

    s.staffID = staffCount + 4001;

    printf("Full Name              : "); scanf(" %[^\n]", s.name);
    printf("Role (Driver/Guard/Cleaner/Ticketer): "); scanf(" %29s", s.role);

    /* Validate role */
    if (strcmp(s.role, "Driver")   != 0 && strcmp(s.role, "Guard")   != 0 &&
        strcmp(s.role, "Cleaner")  != 0 && strcmp(s.role, "Ticketer") != 0) {
        printf("WARNING: Unrecognized role. Saved anyway.\n");
    }

    printf("Shift (Morning/Evening/Night): "); scanf(" %9s", s.shift);
    printf("Contact No             : "); scanf(" %14s", s.contactNo);

    s.assignedTrainID = -1; /* Unassigned by default */

    staff[staffCount++] = s;
    printf("SUCCESS: Staff [%s] added with ID %d.\n", s.name, s.staffID);
}

/* 4.2 Assign staff to a train */
void assignStaffToTrain() {
    int sID, tID, i;
    int sFound = 0, tFound = 0;

    printf("\n--- ASSIGN STAFF TO TRAIN ---\n");
    printf("Enter Staff ID : "); scanf("%d", &sID);
    printf("Enter Train ID : "); scanf("%d", &tID);

    for (i = 0; i < trainCount; i++) {
        if (trains[i].trainID == tID) { tFound = 1; break; }
    }
    if (!tFound) { printf("ERROR: Train ID not found.\n"); return; }

    for (i = 0; i < staffCount; i++) {
        if (staff[i].staffID == sID) {
            sFound = 1;
            staff[i].assignedTrainID = tID;
            printf("SUCCESS: Staff [%s] assigned to Train %d.\n",
                   staff[i].name, tID);
            break;
        }
    }
    if (!sFound)
        printf("ERROR: Staff ID %d not found.\n", sID);
}

/* 4.3 View all staff on duty */
void viewStaffOnDuty() {
    if (staffCount == 0) {
        printf("\nNo staff records available.\n");
        return;
    }

    printf("\n--- STAFF ON DUTY ---\n");
    printLine();
    printf("%-8s %-25s %-12s %-10s %-12s %-12s\n",
           "ID", "Name", "Role", "Shift", "Train ID", "Contact");
    printLine();

    int i;
    for (i = 0; i < staffCount; i++) {
        char trainStr[15];
        if (staff[i].assignedTrainID == -1)
            strcpy(trainStr, "Unassigned");
        else
            sprintf(trainStr, "%d", staff[i].assignedTrainID);

        printf("%-8d %-25s %-12s %-10s %-12s %-12s\n",
               staff[i].staffID,
               staff[i].name,
               staff[i].role,
               staff[i].shift,
               trainStr,
               staff[i].contactNo);
    }
    printLine();
}

/* 4.4 Search staff by ID */
void searchStaffByID() {
    int id, i, found = 0;
    printf("\nEnter Staff ID: ");
    scanf("%d", &id);

    for (i = 0; i < staffCount; i++) {
        if (staff[i].staffID == id) {
            found = 1;
            printf("\n--- STAFF DETAILS ---\n");
            printLine();
            printf("Staff ID   : %d\n", staff[i].staffID);
            printf("Name       : %s\n", staff[i].name);
            printf("Role       : %s\n", staff[i].role);
            printf("Shift      : %s\n", staff[i].shift);
            printf("Contact    : %s\n", staff[i].contactNo);
            if (staff[i].assignedTrainID == -1)
                printf("Train      : Unassigned\n");
            else
                printf("Train ID   : %d\n", staff[i].assignedTrainID);
            printLine();
            break;
        }
    }
    if (!found)
        printf("ERROR: Staff ID %d not found.\n", id);
}

/* 4.5 Update staff shift */
void updateStaffShift() {
    int id, i, found = 0;
    char newShift[10];

    printf("\nEnter Staff ID         : "); scanf("%d", &id);
    printf("New Shift (Morning/Evening/Night): "); scanf(" %9s", newShift);

    /* Validate shift */
    if (strcmp(newShift, "Morning") != 0 &&
        strcmp(newShift, "Evening") != 0 &&
        strcmp(newShift, "Night")   != 0) {
        printf("ERROR: Invalid shift. Use Morning, Evening, or Night.\n");
        return;
    }

    for (i = 0; i < staffCount; i++) {
        if (staff[i].staffID == id) {
            found = 1;
            strcpy(staff[i].shift, newShift);
            printf("SUCCESS: Staff %d shift updated to %s.\n", id, newShift);
            break;
        }
    }
    if (!found)
        printf("ERROR: Staff ID %d not found.\n", id);
}

/* 4.6 Generate a staff report grouped by role */
void generateStaffReport() {
    if (staffCount == 0) {
        printf("\nNo staff data available.\n");
        return;
    }

    char roles[4][12] = {"Driver", "Guard", "Cleaner", "Ticketer"};
    int r, i;

    printf("\n--- STAFF REPORT BY ROLE ---\n");
    printLine();

    for (r = 0; r < 4; r++) {
        int count = 0;
        printf("\n  [%s]\n", roles[r]);
        for (i = 0; i < staffCount; i++) {
            if (strcmp(staff[i].role, roles[r]) == 0) {
                printf("    - %s (ID:%d, Shift:%s)\n",
                       staff[i].name, staff[i].staffID, staff[i].shift);
                count++;
            }
        }
        if (count == 0) printf("    (none)\n");
    }
    printLine();
    printf("Total Staff: %d\n", staffCount);
}

/* Module 4 Sub-Menu */
void staffMenu() {
    int choice;
    do {
        clearScreen();
        printLine();
        printf("  MODULE 4 : STAFF & CREW MANAGEMENT\n");
        printLine();
        printf("  1. Add Staff Member\n");
        printf("  2. Assign Staff to Train\n");
        printf("  3. View All Staff on Duty\n");
        printf("  4. Search Staff by ID\n");
        printf("  5. Update Staff Shift\n");
        printf("  6. Generate Staff Report\n");
        printf("  0. Back to Main Menu\n");
        printLine();
        printf("  Enter choice: ");
        scanf("%d", &choice);

        clearScreen();
        switch (choice) {
            case 1: addStaff();           break;
            case 2: assignStaffToTrain(); break;
            case 3: viewStaffOnDuty();    break;
            case 4: searchStaffByID();    break;
            case 5: updateStaffShift();   break;
            case 6: generateStaffReport();break;
            case 0: break;
            default: printf("Invalid choice. Try again.\n");
        }
        if (choice != 0) pause();
    } while (choice != 0);
}

/* ============================================================
   MODULE 5 : REVENUE & REPORT GENERATION
   Member 5
   ============================================================ */

/* 5.1 Record daily revenue */
void recordRevenue() {
    if (revenueCount >= MAX_REVENUE) {
        printf("ERROR: Revenue log is full!\n");
        return;
    }

    struct Revenue r;
    printf("\n--- RECORD DAILY REVENUE ---\n");
    printf("Date (DD-MM-YYYY)  : "); scanf(" %11s", r.date);

    /* Check for duplicate date */
    int i;
    for (i = 0; i < revenueCount; i++) {
        if (strcmp(revenues[i].date, r.date) == 0) {
            printf("WARNING: Revenue record for %s already exists.\n", r.date);
            return;
        }
    }

    printf("Total Ticket Sales : LKR "); scanf("%f", &r.totalTicketSales);
    printf("Total Passengers   : ");     scanf("%d", &r.totalPassengers);
    printf("Operating Expenses : LKR "); scanf("%f", &r.expenses);

    r.netProfit = r.totalTicketSales - r.expenses;

    revenues[revenueCount++] = r;
    printf("SUCCESS: Revenue recorded for %s. Net Profit: LKR %.2f\n",
           r.date, r.netProfit);
}

/* 5.2 Display daily revenue */
void displayDailyRevenue() {
    if (revenueCount == 0) {
        printf("\nNo revenue records available.\n");
        return;
    }

    printf("\n--- DAILY REVENUE RECORDS ---\n");
    printLine();
    printf("%-14s %-16s %-12s %-14s %-12s\n",
           "Date", "Ticket Sales", "Passengers", "Expenses", "Net Profit");
    printLine();

    int i;
    for (i = 0; i < revenueCount; i++) {
        printf("%-14s %-16.2f %-12d %-14.2f %-12.2f\n",
               revenues[i].date,
               revenues[i].totalTicketSales,
               revenues[i].totalPassengers,
               revenues[i].expenses,
               revenues[i].netProfit);
    }
    printLine();
}

/* 5.3 Calculate total revenue across all days */
void calculateTotalRevenue() {
    if (revenueCount == 0) {
        printf("\nNo revenue records to calculate.\n");
        return;
    }

    float totalSales = 0, totalExp = 0, totalProfit = 0;
    int   totalPsgr  = 0;
    int   i;

    for (i = 0; i < revenueCount; i++) {
        totalSales  += revenues[i].totalTicketSales;
        totalExp    += revenues[i].expenses;
        totalProfit += revenues[i].netProfit;
        totalPsgr   += revenues[i].totalPassengers;
    }

    printf("\n--- CUMULATIVE REVENUE SUMMARY ---\n");
    printLine();
    printf("Total Days Recorded : %d\n",       revenueCount);
    printf("Total Ticket Sales  : LKR %.2f\n", totalSales);
    printf("Total Passengers    : %d\n",        totalPsgr);
    printf("Total Expenses      : LKR %.2f\n", totalExp);
    printf("Total Net Profit    : LKR %.2f\n", totalProfit);
    printLine();
}

/* 5.4 Find the highest revenue day */
void findHighestRevenueDay() {
    if (revenueCount == 0) {
        printf("\nNo revenue records available.\n");
        return;
    }

    int maxIndex = 0, i;
    for (i = 1; i < revenueCount; i++) {
        if (revenues[i].totalTicketSales > revenues[maxIndex].totalTicketSales)
            maxIndex = i;
    }

    printf("\n--- HIGHEST REVENUE DAY ---\n");
    printLine();
    printf("Date         : %s\n",        revenues[maxIndex].date);
    printf("Ticket Sales : LKR %.2f\n",  revenues[maxIndex].totalTicketSales);
    printf("Passengers   : %d\n",         revenues[maxIndex].totalPassengers);
    printf("Net Profit   : LKR %.2f\n",  revenues[maxIndex].netProfit);
    printLine();
}

/* 5.5 Generate full summary report */
void generateSummaryReport() {
    printf("\n");
    printLine();
    printf("          RAILWAY STATION - FULL SUMMARY REPORT\n");
    printLine();
    printf("  Trains Registered   : %d\n", trainCount);
    printf("  Passengers Registered: %d\n", passengerCount);
    printf("  Tickets Booked      : %d\n", ticketCount);

    /* Count active vs cancelled */
    int active = 0, cancelled = 0, i;
    for (i = 0; i < ticketCount; i++) {
        if (tickets[i].isActive) active++;
        else                      cancelled++;
    }
    printf("    - Active           : %d\n", active);
    printf("    - Cancelled        : %d\n", cancelled);

    printf("  Platforms Registered: %d\n", platformCount);

    int occupied = 0;
    for (i = 0; i < platformCount; i++)
        if (platforms[i].isOccupied) occupied++;
    printf("    - Occupied         : %d\n", occupied);
    printf("    - Available        : %d\n", platformCount - occupied);

    printf("  Staff Members       : %d\n", staffCount);
    printLine();

    /* Revenue summary */
    calculateTotalRevenue();
}

/* Module 5 Sub-Menu */
void revenueMenu() {
    int choice;
    do {
        clearScreen();
        printLine();
        printf("  MODULE 5 : REVENUE & REPORT GENERATION\n");
        printLine();
        printf("  1. Record Daily Revenue\n");
        printf("  2. Display Daily Revenue Records\n");
        printf("  3. Calculate Total Revenue\n");
        printf("  4. Find Highest Revenue Day\n");
        printf("  5. Generate Full Summary Report\n");
        printf("  0. Back to Main Menu\n");
        printLine();
        printf("  Enter choice: ");
        scanf("%d", &choice);

        clearScreen();
        switch (choice) {
            case 1: recordRevenue();         break;
            case 2: displayDailyRevenue();   break;
            case 3: calculateTotalRevenue(); break;
            case 4: findHighestRevenueDay(); break;
            case 5: generateSummaryReport(); break;
            case 0: break;
            default: printf("Invalid choice. Try again.\n");
        }
        if (choice != 0) pause();
    } while (choice != 0);
}

/* ============================================================
   MAIN FUNCTION - MENU-DRIVEN INTERFACE
   ============================================================ */
int main() {
    int choice;

    /* Pre-load sample platforms so the system has some data */
    struct Platform p1 = {1, 0, -1, "Available"};
    struct Platform p2 = {2, 0, -1, "Available"};
    struct Platform p3 = {3, 0, -1, "Available"};
    platforms[0] = p1;
    platforms[1] = p2;
    platforms[2] = p3;
    platformCount = 3;

    do {
        clearScreen();
        printLine();
        printf("       RAILWAY STATION MANAGEMENT SYSTEM\n");
        printf("       IN 1101 - Programming Fundamentals\n");
        printLine();
        printf("  1.  Train Schedule & Route Management\n");
        printf("  2.  Passenger Booking & Ticketing\n");
        printf("  3.  Platform & Track Assignment\n");
        printf("  4.  Staff & Crew Management\n");
        printf("  5.  Revenue & Report Generation\n");
        printf("  0.  Exit System\n");
        printLine();
        printf("  Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: trainMenu();    break;
            case 2: bookingMenu();  break;
            case 3: platformMenu(); break;
            case 4: staffMenu();    break;
            case 5: revenueMenu();  break;
            case 0:
                clearScreen();
                printLine();
                printf("  Thank you for using the Railway Station System.\n");
                printf("  Goodbye!\n");
                printLine();
                break;
            default:
                printf("  Invalid choice. Please enter 0-5.\n");
                pause();
        }

    } while (choice != 0);

    return 0;
}
