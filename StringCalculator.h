#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// this function is a common helper function to check if input is a digit i.e (0-9)
int isDigit(char character) {
    if(character >= '0' && character <= '9') {
        return 1;
    }
    else {
        return 0;
    }
}

// this function prints an exception onto the console if the input has a negative number along with the negative inputs
void printExceptionForNegativeNumbers(int negativeDigits[], int count) {
    printf("Exception: negativeDigits not allowed ");
    for (int index = 0; index < count; index++) {
        printf("%d ", negativeDigits[index]);
    }
    printf("\n");
}

// this function checks for two consecutive digits
int shouldInsertComma(char currentChar, char nextChar) {
    return isDigit(currentChar) && isDigit(nextChar);
}

// this function inserts a comma between two consecutive digits
void insertCommasBetweenDigits(char *numbers) {
    char temp[512];
    int commaIndex = 0;
    for (int index = 0; numbers[index] != '\0'; index++) {
        temp[commaIndex++] = numbers[index];
        if (shouldInsertComma(numbers[index], numbers[index + 1])) {
            temp[commaIndex++] = ',';  
        }
    }
    temp[commaIndex] = '\0';
    strcpy(numbers, temp);  
}

// this function checks whether the next character of the input string is NOT a digit and if the string has reached its end i.e null-terminated
int checkIfNextCharIsNotNumber(char nextChar) {
    if((nextChar == '\0' || !isDigit(nextChar))) {
        return 1;
    }
    else {
        return 0;
    }
}

// this function checks for invalid characters in the input string
int hasInvalidSequence(char currentChar, char nextChar, char character) {
    return currentChar == character && checkIfNextCharIsNotNumber(nextChar);
}

// this function uses a helper function to check if the characters of the input string other than the delimiters are digits and if it can be passed to the add function
int isInputInvalid(int index, const char* numbers) {
    if(hasInvalidSequence(numbers[index], numbers[index + 1],',') || hasInvalidSequence(numbers[index], numbers[index + 1],'\n')) {
        return 1;
    }
    else {
        return 0;
    }
}

// this function is used to act as a check for a single input as the sum in such a case cannot be calculated
int validateInput(const char *numbers) {
    int returnValue = 1;
    for (int index = 0; numbers[index] != '\0'; index++) {
        if (isInputInvalid(index,numbers)) {
            printf("Exception: Invalid input sequence at position %d.\n", index);
            returnValue = 0;
        }
    }
    return returnValue;
}

// this function is used as a check for valid digits for further processing
int parseDigits(const char* input) {
    int containsDigit = 0;
    for (int index = 0; input[index] != '\0'; index++) {
        if (isDigit(input[index])) {
            containsDigit = 1;
        }
    }
    return !containsDigit;
}

// this function checks for non-numeric or empty input string
int isNonNumericOrEmptyInput(const char *input) {
    if (strlen(input) == 0) {
        return 1;
    }
    return parseDigits(input);
}

// this function checks for custom delimiter with the specified format
int isCustomDelimiter(char *stringCopy) {
    return strncmp(stringCopy, "//", 2) == 0;
}

// this helper function ensures the end index is greater than the start index of the input string i.e valid length
int parseInputIfValid(char *start, char *end) {
    if(end > start) {
        return 1;
    }
    return 0;
}

// this helper function parses the input string characters only if its length is non-zero
int parseInputIfNotNull(char *start, char *end) {
    if(start != NULL && end != NULL) {
        return 1;
    }
    return 0;
}

// this function does the initial valid input string checks for custom delimiter functionality
int parseInput(char *start, char *end) {
    if(parseInputIfNotNull(start,end) && parseInputIfValid(start,end)) {
        return 1;
    }
    else {
        return 0;
    }
}

// this function extracts the delimiter based on the given format
void extractCustomDelimiter(char *stringCopy, char *delimiter) {
    char *start = strchr(stringCopy, '[');
    char *end = strchr(stringCopy, ']');
    if (parseInput(start,end)) {
        strncpy(delimiter, start + 1, end - start - 1);
        delimiter[end - start - 1] = '\0';
    }
}

// this function handles a case where the custom delimiter is empty
void handleEmptyDelimiter(char *numbers, char *delimiter) {
    if (strcmp(delimiter, "") == 0) {
        insertCommasBetweenDigits(numbers);
        strcpy(delimiter, ",");
    }
}

// this function deals with processing of negative digits and storing it into an array
int processNegativeNumber(int number, int negativeDigits[], int *negativeCount) {
    if (number < 0) {
        negativeDigits[(*negativeCount)++] = number;
        return 0;
    }
    return 1;
}

// this function only considers numbers of the input string that are less than or equal to 1000, and ignores the numbers that are greater than 1000.
void addToSumIfValid(int number, int *sum) {
    if (number <= 1000) {
        *sum += number;
    }
}

// this function combines all the helper functions to check for a valid input string with custom delimiter
void validateCustomDelimiter(char *stringCopy, char **numbers, char *delimiter) {
    if (isCustomDelimiter(stringCopy)) {
        extractCustomDelimiter(stringCopy, delimiter);
        *numbers = strchr(stringCopy, '\n') + 1;
        handleEmptyDelimiter(*numbers, delimiter);
    }
}

// this function takes care of negative digits in input string
int HandleNegatives(int negativeCount, int negativeDigits[],int sum) {
    if (negativeCount > 0) {
        printExceptionForNegativeNumbers(negativeDigits, negativeCount);
        sum = 0;
    }
    return sum;
}

// this function does the actual addition calculation to get the final sum as result
int calculateSum(char *numbers, char delimiter[]) {
    char *token = strtok(numbers, delimiter);
    int sum = 0;
    int negativeDigits[256];
    int negativeCount = 0;
    while (token != NULL) {
        int number = atoi(token);
        if (!processNegativeNumber(number, negativeDigits, &negativeCount)) {
            token = strtok(NULL, delimiter);
            continue;
        }
        addToSumIfValid(number, &sum);
        token = strtok(NULL, delimiter);
    }
    sum = HandleNegatives(negativeCount,negativeDigits,sum);
    return sum;
}

// this function makes use of other functions to get the final result
int add(const char *stringInput) {
    int result = 0;
    if(isNonNumericOrEmptyInput(stringInput)) {
        return 0;
    }
    char stringCopy[512];
    strcpy(stringCopy, stringInput);
    char delimiter[256] = ",\n;*";
    char *numbers = stringCopy;
    validateCustomDelimiter(stringCopy, &numbers, delimiter);
    if(validateInput(numbers))
    {
        result = calculateSum(numbers,delimiter);
    }
    return result;
}
