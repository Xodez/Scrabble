#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE 7
#define ARR_SIZE_VALUES 26
#define FILEPATH "C:\\Users\\Kasparas\\CLionProjects\\School\\C-scrabble\\letter_values.txt"
#define ARR_SIZE_LETTERS 7

void generate_letters(int number, char *destination) {
    char vowels[] = {'a', 'e', 'i', 'o', 'u'};
    int i;
    unsigned int rand_seed;
    printf("Number < 1000:\n");
    scanf("%d", &rand_seed); // initialise the PRNG
    getchar();
    srand(rand_seed);
// Half of the letters should be vowels
    for (i = 0; i < number / 2; i++, destination++) *destination = vowels[rand() % 5];
    for (; i < number; i++, destination++) *destination = rand() % 26 + 'a';
}

int read_array_from_file(const char *filepath, int *array, int array_length) {
    FILE *file;
    int i;
    if ((file = fopen(filepath, "r")) == NULL) {
        perror("opening file");
        return -1;
    }
    for (i = 0; i < array_length; i++) {
        if (fscanf(file, "%d", &array[i]) == EOF) break;
    }
    return i;
}

int get_user_input(char *str) {
    char ch;
    int index = 0;
    while (ch != '\n') {
        ch = getchar();
        if (ch != '\n') {
            if (ch >= 97) {
                str[index] = ch;
            }
            else{
                ch += 32;
                str[index] = ch;
            }
        }
        index++;
    }
    str[index - 1] = '\0';
    return (index - 1);
}

int allocate_points(char *str, int *val) {
    int temp[7];
    int points = 0;
    for (int i = 0; i < ARR_SIZE_LETTERS; i++) {
        if (str[i] != '\0') {
            temp[i] = str[i] - 'a';
        } else {
            break;
        }
    }
    for (int i = 0; i < ARR_SIZE_LETTERS; i++) {
        if (str[i] != '\0') {
            points += val[temp[i]];
        } else {
            break;
        }
    }
    return points;
}

void organize_letters(const char *gen, char *org, int *amount) {
    int counter = 0;
    int bool1 = 0;
    int index = 0;
    for (int i = 0; i < ARR_SIZE; i++) {
        bool1 = 0;
        counter = 0;
        for (int x = 0; x < ARR_SIZE; x++) {
            if (gen[i] == gen[x]) {
                counter++;
            }
        }
        for (int y = 0; y < index; y++) {
            if (gen[i] == org[y]) {
                bool1 = 1;
            }
        }
        if (!bool1) {
            org[index] = gen[i];
            amount[index] = counter;
            index++;
        }
    }
    org[index] = '0';
}

int tally_points(const int *points, int totalPointsLen){
    int totalPoints = 0;
    for (int i = 0; i < totalPointsLen; i++){
        totalPoints += points[i];
    }
    return totalPoints;
}

void total_amount_of_each_letter(char *input, int inputLength, int *destination){
    for (int i = 0; i < inputLength; i++){
        destination[(input[i] - 'a')] += 1;
    }
}

int main() {
    char genLetters[ARR_SIZE];
    char dupeGenLetters[ARR_SIZE + 1];
    int amountDupeGenLetters[ARR_SIZE];
    int letterValues[ARR_SIZE_VALUES];
    char userInput[ARR_SIZE_LETTERS + 1];
    int letterGenCheck[ARR_SIZE_LETTERS];
    int letterInputCheck[ARR_SIZE_LETTERS];
    int totalAmountOfEachLetter[ARR_SIZE_VALUES];
    for (int i = 0; i < ARR_SIZE_VALUES; i++){
        totalAmountOfEachLetter[i] = 0;
    }
    int storedPoints[10];
    char enteredWords[10][8];
    int indexOfEnteredWords = 0;
    int totalAmountOfLetters = 0;
    read_array_from_file(FILEPATH, letterValues, ARR_SIZE_VALUES);
    generate_letters(ARR_SIZE, genLetters);
    organize_letters(genLetters, dupeGenLetters, amountDupeGenLetters);
    int whileTrue = 1;
    while (whileTrue) {
        for (int i = 0; i < ARR_SIZE; i++) {
            if (dupeGenLetters[i] != '0') {
                printf("%c --> %d\n", dupeGenLetters[i], amountDupeGenLetters[i]);
            } else {
                break;
            }
        }
        printf("Enter a word\n");
        int checkBool = 0;
        for (int i = 0; i < ARR_SIZE_LETTERS; i++) {
            letterGenCheck[i] = genLetters[i] - 'a';
        }
        char cnt;
        int userInputLength = get_user_input(userInput);
        for (int i = 0; i < userInputLength; i++) {
            letterInputCheck[i] = userInput[i] - 'a';
        }
        for (int x = 0; x < userInputLength; x++) {
            for (int z = 0; z < ARR_SIZE_LETTERS; z++) {
                checkBool = 0;
                if (letterGenCheck[z] == letterInputCheck[x]) {
                    letterGenCheck[z] = -1;
                    checkBool = 1;
                    break;
                }
            }
            if (!checkBool) {
                break;
            }
        }
        if (checkBool) {
            int points = allocate_points(userInput, letterValues);
            printf("%s --> %d\n", userInput, points);
            totalAmountOfLetters += userInputLength;
            total_amount_of_each_letter(userInput, userInputLength, totalAmountOfEachLetter);
            printf("Do you want to continue? y/n\n");
            scanf("%c", &cnt);
            getchar();
            for (int i = 0; i < userInputLength; i++) {
                enteredWords[indexOfEnteredWords][i] = userInput[i];
                if ((i + 1) == userInputLength){
                    enteredWords[indexOfEnteredWords][i + 1] = '\0';
                }
            }
            storedPoints[indexOfEnteredWords] = points;
            indexOfEnteredWords++;
            if (cnt == 'n') {
                for (int i = 0; i < indexOfEnteredWords; i++) {
                    printf("Word: %s --> %d\n", enteredWords[i], storedPoints[i]);
                }
                printf("Total points --> %d\n", tally_points(storedPoints, indexOfEnteredWords));
                printf("Total amount of letters --> %d\n", totalAmountOfLetters);
                for (int i = 0; i < (ARR_SIZE_VALUES); i++){
                    printf("%c --> %d\n", (i + 97), totalAmountOfEachLetter[i]);
                }
                whileTrue = 0;
            }
        } else {
            printf("Entered a letter that was not left in the pool!\n");
            printf("Do you want to continue? y/n\n");
            scanf("%c", &cnt);
            getchar();
            if (cnt == 'n') {
                for (int i = 0; i < indexOfEnteredWords; i++) {
                    printf("Word: %s --> %d\n", enteredWords[i], storedPoints[i]);
                }
                printf("Total points --> %d\n", tally_points(storedPoints, indexOfEnteredWords));
                printf("Total amount of letters --> %d\n", totalAmountOfLetters);
                for (int i = 0; i < (ARR_SIZE_VALUES); i++){
                    printf("%c --> %d\n", (i + 97), totalAmountOfEachLetter[i]);
                }
                whileTrue = 0;
            }
        }
    }
}
