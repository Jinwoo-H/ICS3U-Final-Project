//NAME: Jinwoo Hong
//ICS3U
//FINAL PROJECT
//DUE AT END OF LAST CLASS

// SEE EXAMPLE OF FINISHED PROGRAM HERE
//       http://dev.emmell.org:8888/
// SEE EXAMPLE OF FINISHED PROGRAM HERE

// FINAL PROJECT MARKING GUIDE
/*
See comments above each function for a description of how they work.
Also use the example finished program as example to see expected behaviour.

There are 8 total functions that need to be completed. I recommend the following order from simplest to most complex.

- printNiceTime
- findFastestLap
- findSlowestLap
- findAverageTime
- editDriver
- readIn
- deleteDriver
- addDriver

Obtain a level 1 mark by fully completing any 4 functions
Obtain a level 2 mark by fully completing any 5 functions
Obtain a level 3 mark by fully completing any 6 functions
Obtain a level 4 mark by fully completing all 8 functions

All levels will be adjusted +/- based on commenting, formatting, variable naming and efficiency.
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

// STANDARD DEFINITIONS TO PROVIDE CROSS PLATFORM clear screen and pause commands
#if defined(_WIN32) || defined(_WIN64)
    #define clrscr() system("cls")
    #define pause() printf("\n"); system("pause")
#else
    #define clrscr() printf("\x1B[1;1H\x1B[2J")
    #define pause() printf("\n"); system("read -r -p \"Press any key to continue...\" key")
#endif

//Structure for defining what a single Racer looks like
typedef struct {
    char driver[50];
    char team[50];
    int carNumber;
    double bestLap;
} Racer;

//Function that reads in from the "final-data.txt" file
//Populates an array of Racer, and updates the total number of racers
// THE NAME OF YOUR DATA FILE IS: "data.txt" (see tab at top of coding window)
// File format is:
//  Name
//  Team
//  Car_Number
//  Best_lap_time
//
// NOTE! Up until now your data has been populated via the "populateArray" function. You will need to swap the comments in main in order to have 'readIn' be called.
//       That is:
//          - In main() --> Comment        populateArray(data,&numRacers);
//          - In main() --> Un comment     readIn(data,&numRacers);

int testZero(char input[50]){ //Function to test if the users inputted time is 0
    if (atof(input) == 0){
        printf("Your lap time cannot be 0 seconds!\n");
        return 0;
    }
    else{ //Note: Even if the inputted time is propably impossible to acheive in a real life scenario, such as 0.001 seconds, it will still be considered a valid input
        return 1;
    }
}

int testEmpty(char input[50]){ //Function to test if the user inputted nothing and just pressed enter
    if (input[0] == 10){ //If the first spot in the array is the ASCII code for enter, meaning the user inputted nothing and pressed enter
        printf("You cannot leave this space blank\n");
        return 0;
    }
    else{
        return 1;
    }
}


int testNum(char input[50]){

    //Variable Declaration
    int i;

    for (i=0 ; input[i] != '\0' ; i++){ //Goes through each spot in array until NULL

        if (!(input[i] >= '0' && input[i] <= '9')){ //Checks if character is not a number from 0 to 9
            printf("Please enter a whole number with no spaces\n");
            return 0;
        }
    }
    return 1; //Return 1 for is number if input is all numbers
}


int testFloat(char input[50]){

    //Variable Declaration
    int i;
    int decimal = 0;

    for (i=0 ; input[i] != '\0' ; i++){ //Goes through each spot in array until NULL

        if (!(input[i] >= '0' && input[i] <= '9')){ //Checks if character is not a number from 0 to 9

            if(input[i] == '.'){ //checks if the character is a decimal point
                decimal++; //Increase decimal count by one
            }
            else{
                printf("Please enter a valid lap time\n");
                return 0;
            }
            if(decimal > 1){ //If there is more than one decimal point
                printf("Please enter a valid lap time\n");
                return 0; //Returns 0 for not float
            }
        }
    }
    return 1; //Return 1 for is number if input is all numbers and not more than one decimal point
}

void removeTrailingEndline(char *str) { //Function to remove the \n and \r characters of a string
    int x=0;
    while (str[x] != 13 && str[x] != 10) {
        x++;
    }
    str[x] = 0;
}

void readIn(Racer data[],int *numRacers) {

    char temp[50]; //Temporary array for carNumber and bestLap

    FILE *fp; //Declare file pointer
    fp = fopen("data.txt", "r"); //Open file for reading

    if (fp == NULL){ //If file cannot be opened
        printf("Could not open file\n");
        exit(1); //Closes program
    }

    int i = 0; //Index for struct

    while(!feof(fp)){ //Untill the cursor reaches the end of the file

        fgets(data[i].driver, 50, fp); //Reads the whole line, including the \n and \r characters
        removeTrailingEndline(data[i].driver); //Function to remove the \n and \r characters

        fgets(data[i].team, 50, fp);
        removeTrailingEndline(data[i].team);

        fgets(temp, 50, fp);
        removeTrailingEndline(temp);
        data[i].carNumber = atoi(temp); //Converts temporary string into an integer value in the struct
        memset(temp, 0, sizeof(temp)); //Clears temp array

        fgets(temp, 50, fp);
        removeTrailingEndline(temp);
        data[i].bestLap = atof(temp); //Converts the temporary string into a double float value in the struct
        memset(temp, 0, sizeof(temp)); //Clears temp array

        i++; //Index of data struct goes up by one for the next driver
    }

    *numRacers = i; //The number of drivers is the value of i

    fclose(fp); //Close file

}

//Function that delets a Racer from the array and updates total number of Racers to relfect the deletion.
// Parameter "toDelete" is the number that the user wants deleted (as seen in the menu)
// All other items in the array should shift position as a result of the deletion.
// Example, if the array were to contain:   A  B  C  D  E
// After deleting the 2nd value, the array would now contain: A  C  D  E
void deleteDriver(int toDelete, Racer data[], int *numRacers) {

    clrscr();

    char option[50];
    int validInput = 0; //Do while loop condition
    int i;

    do{
        printf("=== DELETING DRIVER ===\n\nConfirm, delete %s? (Y/N): ", data[toDelete-1].driver); //Index is -1 of toDelete because index starts a 0
        memset(option, 0, sizeof(option)); //Resets option array (not required)
        fgets(option, 50, stdin);
        removeTrailingEndline(option);

        if (!strcmp(option, "Y") || !strcmp(option, "y")){ //If user typed Y or y
            validInput = 1; //Breaks do while loop

            for (i=toDelete-1 ; i<*numRacers ; i++){ //Does loop for each driver after the chosen driver

                strcpy(data[i].driver, data[i+1].driver); //Overwrites the next drivers data into the one that is getting deleted or shifted to the left
                memset(data[i+1].driver, 0, sizeof(data[i+1].driver)); //Resets array as it is going to be overwritten next loop, or if it is the last driver, the data will not be needed and erased

                strcpy(data[i].team, data[i+1].team);
                memset(data[i+1].team, 0, sizeof(data[i+1].team));

                data[i].carNumber = data[i+1].carNumber; //Since it is an integer and double float
                                                        //you can set the value using an equals sign
                data[i].bestLap = data[i+1].bestLap;
            }

            *numRacers = *numRacers - 1; //There is one less driver now
        }
        else if (!strcmp(option, "N") || !strcmp(option, "n")){ //If user types N or n
            validInput = 1;
        }
        else{ //Option is not y Y, n, or N
            printf("Enter a valid response\n");
            pause();
            clrscr();
        }
    } while (validInput == 0);

}


//Function to edit the values stored for a given Racer
// Parameter "toEdit" refers to the number selected as the entry to edit (as seen in the menu)
// User should be shown the current data and asked to type in what the new entry should be
void editDriver(int toEdit, Racer data[]) {

    clrscr();

    //Varaible Declaration
    char input[50];
    int validNum = 0;
    int validInput = 0;
    int validTime = 0;

    printf("=== EDITING DRIVER ===\nPlease enter information about the new driver to add:\n\n");

    do{
        memset(input, 0, sizeof(input)); //Clears input so it can be used again instead of using another array for team
        printf("New name [Current name: %s]: ", data[toEdit-1].driver); //Index is -1 of edit driver, because index starts at 0
        fgets(input, 50, stdin); //Using fgets for the strings to read the whole line in the standart input instead of just one word
        validInput = testEmpty(input); //Function to test if the user pressed enter without inputting anything

    } while (validInput == 0); //The returned value of validInput will either be 0 or 1

    removeTrailingEndline(input); //Removes the \n and \r characters from the input
    memset(data[toEdit-1].driver, 0, sizeof(data[toEdit-1].driver)); //Clears the previous driver data (not required)
    strcpy(data[toEdit-1].driver, input); //Copy the newName onto the chosen driver

    validInput = 0; //Resets loop condition

    do{                                                     //Similar to previous code
        memset(input, 0, sizeof(input));
        printf("New team [Current team: %s]: ", data[toEdit-1].team);
        fgets(input, 50, stdin);
        validInput = testEmpty(input);

    } while (validInput == 0);

    removeTrailingEndline(input);
    memset(data[toEdit-1].team, 0, sizeof(data[toEdit-1].team));
    strcpy(data[toEdit-1].team, input);

    validInput = 0;

    do{
        memset(input, 0, sizeof(input));
        printf("New car number [Current number: %i]: ", data[toEdit-1].carNumber);
        fgets(input, 50, stdin);
        validInput = testEmpty(input);
        removeTrailingEndline(input);
        validNum = testNum(input); //Function to check if input is only numbers

    } while (validNum == 0 || validInput == 0); //Invalid input if the user inputted something besides a number or inputted nothing


    data[toEdit-1].carNumber = atoi(input); //Once the function verifys that the input is only integers, it converts input into integers and sets it into carNumber

    validNum = 0; //Resets both loop conditions
    validInput = 0; //back to 0

    do{
        memset(input, 0, sizeof(input));
        printf("New best lap time in seconds [Current time: %.3f]: ", data[toEdit-1].bestLap);
        fgets(input, 50, stdin);
        validInput = testEmpty(input);
        removeTrailingEndline(input);
        validNum = testFloat(input); //Function I made to test if the string is a float, returns 1 if it is, and 0 if it isn't

       if (validNum == 1 && validInput == 1){ //If the input is a valid number and not an empty input
            validTime = testZero(input); //Function to check if the number is 0;
        }

    } while(validNum == 0 || validInput == 0 || validTime == 0); //Loops if the inputted string is not a float or is zero

    data[toEdit-1].bestLap = atof(input); //Sets bestLap to the converted float from the string
}

//Function to add a new Racer to the list
// Will ask the user for all relevant information and update the total number of Racers
void addDriver(Racer data[],int *numRacers) {

    clrscr();

    //Varaible Declaration
    char input[50];
    int validNum = 0;
    int validInput = 0;
    int validTime = 0;

    printf("=== ADDING NEW DRIVER ===\nPlease enter information about the new driver to add:\n\n");

    //Asks the user for each member of the struct for the data, then stores it in an unused index of the array

    do{
        memset(input, 0, sizeof(input)); //Clears the array for input
        printf("Driver's full name: ");
        fgets(data[*numRacers].driver, 50, stdin); //Using fgets for the strings to read the whole line in the standard input instead of just one word
        validInput = testEmpty(data[*numRacers].driver); //Function to test if the user pressed enter without inputting anything
        removeTrailingEndline(data[*numRacers].driver); //Removes the \n and \r characters from the input

    } while (validInput == 0); //The returned value of validInput will either be 0 or 1

    validInput = 0; //Resets loop condition

    do{                                                     //Similar to previous code
        memset(input, 0, sizeof(input));
        printf("\nTeam name: ");
        fgets(data[*numRacers].team, 50, stdin);
        validInput = testEmpty(data[*numRacers].team);
        removeTrailingEndline(data[*numRacers].team);

    } while (validInput == 0);

    do {
        memset(input, 0, sizeof(input));
        printf("\nCar number: ");
        fgets(input, 50, stdin);
        validInput = testEmpty(input); //Tests if input is empty
        removeTrailingEndline(input); //Removes \n and \r character
        validNum = testNum(input); //Check if input is only numbers

    } while (validNum == 0 || validInput == 0); //If either the input has characters other than letters or is empty the code will loop

    data[*numRacers].carNumber = atoi(input); //Takes the converted integer form of the input to the carNumber once it is verified that the input is all integers

   validNum = 0;  //Resets both
   validInput = 0; //loop conditions

    do {                                                 //Similar to previous code
        memset(input, 0, sizeof(input));
        printf("\nTime in seconds (ex: 76.432 seconds): ");
        fgets(input, 50, stdin);
        validInput = testEmpty(input);
        removeTrailingEndline(input);
        validNum = testFloat(input); //Uses testFloat function to test if input is a proper integer or float

        if (validNum == 1 && validInput == 1){ //If the input is a valid number and not an empty input
            validTime = testZero(input); //Function to check if the number is 0;
        }
    }while (validNum == 0 || validInput == 0 || validTime == 0); //Loops if any of the conditions are not met

    data[*numRacers].bestLap = atof(input); //Takes the converted float form of the input to the carNumber once it is verified that the input is a proper float or integer

    *numRacers = *numRacers + 1; //Increases driver count by one
}

//Function to convert a total time in seconds to a nicely formatted time
// NOTE: This function does not return, simply prints out with no line break at the end
// NOTE: The final print out should have three decimal places, as shown in the example below
// Ex: 76.314 seconds gets printed as: 1:16.314
void printNiceTime(double time) {

    //Varaible Declaration
    int minutes = 0;

while(time-60 >= 0){ //Does loop until the time is not enough to make a minute

    if (time/60 >= 0){ //If the time in seconds /60 is 0 or more, the time is at least one minute
        minutes++; //The counter for minute is increased by one
        time-=60; //60 (seconds) is subracted from time
    }
}
    if (time/10 >= 1){ //If the time divided by 10 is over 0
        printf("%i:%.3f", minutes, time); //Prints minutes and then the time to 3 decimal places
    }
    else if (time/10 < 1){ //If the time divided by 10 is less than 1
        printf("%i:0%.3f", minutes, time); //It prints a 0 before, for example if the time is 5 seconds, it prints 05 seconds
    }

}

//Function that returns the index of the Racer with the fastest lap (smallest time)
int findFastestLap(Racer data[],int numDrivers) {

    //Varaible Declaration
    int i;
    double bestTime;
    int fastestLap = 0; //Initializes and sets the fastest lap to 0

    bestTime = data[0].bestLap; //Temporarily set the best time as the first element in array

    for(i=0 ; i<numDrivers ; i++){ //Does loop until every driver is checked

        if(data[i].bestLap < bestTime){ //If the driver has a better time than the current best time
            bestTime = data[i].bestLap; //Sets new best time as the current driver
            fastestLap = i; //Saves the index number of the current fastest time
        }
    }

    return(fastestLap); //Returns the index of the final best time once all drivers are checked
}

//Function that returns the index of the Racer with the slowest lap (largest time)
int findSlowestLap(Racer data[],int numDrivers) {

    //Varaible Declaration
    int i;
    double worstTime;
    int slowestLap = 0; //Initializes and sets the slowest lap to 0

    worstTime = data[0].bestLap; //Temporarily set the worst time as the first element in array

    for(i=0 ; i<numDrivers ; i++){ //Does loop until every driver is checked

        if(data[i].bestLap > worstTime){ //If the driver has a worse time than the current worst time
            worstTime = data[i].bestLap; //Sets the new worst time as the current driver
            slowestLap = i; //Saves the index number of the current worst time
        }
    }

    return(slowestLap); //Returns the index of the final worst time once all drivers are checked
}

//Function that returns the average of all Racer times
double findAverageTime(Racer data[], int numDrivers) {

    //Varaible Declaration
    int i;
    double averageTime = 0; //Initializes and sets averageTime to 0

    for(i=0 ; i<numDrivers ; i++){ //Does loop for each driver

        averageTime+=data[i].bestLap; //Add the lap times together for each drivers best time
    }

    averageTime/=numDrivers; //Divide the total time by the number of drivers to get the average

    return(averageTime); //Returns the average time

}

//Function that calls the other statistics functions and displays them nicely.
// NOTE: This function should not be edited. You may edit it for the sake of debugging,
//       however it needs to work as show below. It will be restored to this version when being marked.
void showStats(Racer data[],int numDrivers) {
    int x,slowestLapIdx,fastestLapIdx;

    slowestLapIdx = findSlowestLap(data,numDrivers);
    fastestLapIdx = findFastestLap(data,numDrivers);

    clrscr();
    printf("=== DRIVER STATISTICS ===\n\n");
    printf("There are %i drivers\n",numDrivers);

    printf("The fastest driver was %s with a time of ",data[fastestLapIdx].driver);
    printNiceTime(data[fastestLapIdx].bestLap);
    printf("\n");

    printf("The slowest driver was %s with a time of ",data[slowestLapIdx].driver);
    printNiceTime(data[slowestLapIdx].bestLap);
    printf("\n");

    printf("The average lap time was ");
    printNiceTime(findAverageTime(data,numDrivers));
    printf("\n");
    pause();
}

//A function to populate the array of Racers
// This function will only be used if you can't get FILE I/O to work (uncomment it within the 'main' function)
void populateArray(Racer data[],int *numRacers) {strcpy(data[0].driver,"Lewis Hamilton");strcpy(data[0].team,"Mercedes");data[0].carNumber = 44;data[0].bestLap = 75.484;strcpy(data[1].driver,"Valtteri Bottas");strcpy(data[1].team,"Mercedes");data[1].carNumber = 77;data[1].bestLap = 75.902;strcpy(data[2].driver,"Max Verstappen");strcpy(data[2].team,"Red Bull Racing");data[2].carNumber = 33;data[2].bestLap = 77.637;strcpy(data[3].driver,"Daniel Ricciardo");strcpy(data[3].team,"Renault");data[3].carNumber = 3;data[3].bestLap = 77.552;strcpy(data[4].driver,"Charles Leclerc");strcpy(data[4].team,"Ferrari");data[4].carNumber = 16;data[4].bestLap = 78.173;strcpy(data[5].driver,"Sergio Perez");strcpy(data[5].team,"Racing Point");data[5].carNumber = 11;data[5].bestLap = 78.084;strcpy(data[6].driver,"Lando Norris");strcpy(data[6].team,"McLaren");data[6].carNumber = 4;data[6].bestLap = 78.069;strcpy(data[7].driver,"Carlos Sainz");strcpy(data[7].team,"McLaren");data[7].carNumber = 55;data[7].bestLap = 78.118;strcpy(data[8].driver,"Alexander Albon");strcpy(data[8].team,"Red Bull Racing");data[8].carNumber = 23;data[8].bestLap = 76.177;strcpy(data[9].driver,"Pierre Gasly");strcpy(data[9].team,"AlphaTauri");data[9].carNumber = 10;data[9].bestLap = 80.403;strcpy(data[10].driver,"Lance Stroll");strcpy(data[10].team,"Racing Point");data[10].carNumber = 18;data[10].bestLap = 78.36;strcpy(data[11].driver,"Esteban Ocon");strcpy(data[11].team,"Renault");data[11].carNumber = 12;data[11].bestLap = 79.606;strcpy(data[12].driver,"Daniil Kvyat");strcpy(data[12].team,"AlphaTauri");data[12].carNumber = 26;data[12].bestLap = 77.666;strcpy(data[13].driver,"Sebastian Vettel");strcpy(data[13].team,"Ferrari");data[13].carNumber = 5;data[13].bestLap = 77.767;strcpy(data[14].driver,"Nico Hulkenberg");strcpy(data[14].team,"Racing Point");data[14].carNumber = 27;data[14].bestLap = 78.942;*numRacers = 15;}


//This function runs the actual program
// NOTE: This function should not be edited. You may edit it for the sake of debugging,
//       however it needs to work as show below. It will be restored to this version when being marked.
int main() {
    Racer data[100];
    int x,numRacers,menuChoice,input;

    //*********************
    //WORKING ON FILE I/O?
    //COMMENT THE FOLLOWING and UNCOMMENT THE 'readIn' FUNCTION
    //populateArray(data,&numRacers);
    readIn(data,&numRacers);
    //*********************

    do {
        clrscr();
        printf("== 2020 Formula 1 Racing: %i racers ==\n",numRacers);
        printf("     %-20s%-20s%5s%23s\n","DRIVER","TEAM","NUM","BEST LAP TIME (M:SSS)");
        for (x=0;x<numRacers;x++) {
            printf("[%2i] ",x+1);
            printf("%-20s",data[x].driver);
            printf("%-20s",data[x].team);
            printf("%5i",data[x].carNumber);
            printf("\t");
            printNiceTime(data[x].bestLap);
            printf("\n");
        }

        printf("\n");
        printf("1. Add new driver\n");
        printf("2. Edit driver info\n");
        printf("3. Delete driver\n");
        printf("4. Statistics\n");
        printf("0. Quit\n");
        printf("\n ==> ");
        scanf("%i",&menuChoice);
        getchar();

        if (menuChoice == 1) {
            //Add driver
            addDriver(data,&numRacers);
        }

        else if (menuChoice == 2) {
            //Edit driver
            do {
                printf("\nWhich driver to edit? ");
                scanf("%i",&input);
                getchar();
                if (input < 1 || input > numRacers) {
                    printf("Sorry, not a valid choice. There are %i drivers, use the numbers on the left side.\n",numRacers);
                    pause();
                }
            } while (input < 1 || input > numRacers);

            editDriver(input,data);
        }

        else if (menuChoice == 3) {
            //Delete driver
            do {
                printf("\nWhich driver to delete? ");
                scanf("%i",&input);
                getchar();
                if (input < 1 || input > numRacers) {
                    printf("Sorry, not a valid choice. There are %i drivers, use the numbers on the left side.\n",numRacers);
                    pause();
                }
            } while (input < 1 || input > numRacers);

            deleteDriver(input,data,&numRacers);
        }

        else if (menuChoice == 4) {
            //Statistics
            clrscr();
            showStats(data,numRacers);
        }

        else if (menuChoice != 0) {
            printf("Sorry, please choose a valid option from the menu.\n");
            pause();
        }

    } while (menuChoice != 0);

    clrscr();
    printf("\nThanks for visiting! Please come again.\n\n\n");
}
