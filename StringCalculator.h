#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int checkNextNumber(char nextChar)
{
    if(nextChar >= '0' && nextChar <= '9')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int checkCurrentNumber(char currentChar)
{
    if(currentChar >= '0' && currentChar <= '9')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Exception handler for negatives
void throwNegativeException(int negatives[], int count) {
    printf("Exception: negatives not allowed ");
    for (int i = 0; i < count; i++) {
        printf("%d ", negatives[i]);
    }
    printf("\n");
    exit(1);
}

// Check if a comma should be inserted between two digits
int shouldInsertComma(char currentChar, char nextChar) {
    return checkCurrentNumber(currentChar) && checkNextNumber(nextChar);
}

// Insert commas between adjacent digits
void insertCommasBetweenDigits(char *numbers) {
    char temp[512];
    int j = 0;

    for (int i = 0; numbers[i] != '\0'; i++) {
        temp[j++] = numbers[i];
        if (shouldInsertComma(numbers[i], numbers[i + 1])) {
            temp[j++] = ',';  // Insert a comma if needed
        }
    }
    temp[j] = '\0';
    strcpy(numbers, temp);  // Copy the modified string back
}

int checkIfNextCharIsNotNumber(char nextChar)
{
    if((nextChar == '\0' || !(nextChar >= '0' && nextChar <= '9')))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Handle invalid input sequences
int hasInvalidCommaSequence(char currentChar, char nextChar) {
    return currentChar == ',' && checkIfNextCharIsNotNumber(nextChar);
}

int hasInvalidNewlineSequence(char currentChar, char nextChar) {
    return currentChar == '\n' && checkIfNextCharIsNotNumber(nextChar);
}

int isInputInvalid(int i, const char* numbers)
{
    if(hasInvalidCommaSequence(numbers[i], numbers[i + 1]) || hasInvalidNewlineSequence(numbers[i], numbers[i + 1]))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void validateInput(const char *numbers) {
    for (int i = 0; numbers[i] != '\0'; i++) {
        if (isInputInvalid(i,numbers)) {
            printf("Error: Invalid input sequence at position %d.\n", i);
            exit(1);
        }
    }
}

int checkNoOfDigits(const char*input)
{
    int containsDigit = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] >= '0' && input[i] <= '9') {
            containsDigit = 1;
        }
    }
    return !containsDigit;  // Return 1 if no digit is found
}

// Check for non-numeric input or empty string
int isNonNumericOrEmptyInput(const char *input) {
    if (strlen(input) == 0) {
        return 1;  // Input is empty
    }
    return checkNoOfDigits(input);
}

// Extract custom delimiter from input
int isCustomDelimiter(char *stringCopy) {
    return strncmp(stringCopy, "//", 2) == 0;
}

int parseInputIfNotNull(char *start, char *end)
{
    if(start != NULL && end != NULL && end > start)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void extractCustomDelimiter(char *stringCopy, char *delimiter) {
    char *start = strchr(stringCopy, '[');
    char *end = strchr(stringCopy, ']');
    if (parseInputIfNotNull(start,end)) {
        strncpy(delimiter, start + 1, end - start - 1);  // Extract custom delimiter between [ and ]
        delimiter[end - start - 1] = '\0';  // Null-terminate custom delimiter
    }
}

// Handle empty custom delimiter
void handleEmptyDelimiter(char *numbers, char *delimiter) {
    if (strcmp(delimiter, "") == 0) {
        insertCommasBetweenDigits(numbers);  // Insert commas between adjacent digits
        strcpy(delimiter, ",");  // Use commas as the effective delimiter
    }
}

// Process numbers and handle negatives
int processNegativeNumber(int number, int negatives[], int *negativeCount) {
    if (number < 0) {
        negatives[(*negativeCount)++] = number;
        return 0;  // Return 0 to indicate that the number shouldn't be added to the sum
    }
    return 1;  // Valid positive number
}

void addToSumIfValid(int number, int *sum) {
    if (number <= 1000) {
        *sum += number;  // Add to sum if the number is <= 1000
    }
}

// Validate and assign custom delimiter if present
void validateCustomDelimiter(char *stringCopy, char **numbers, char *delimiter) {
    if (isCustomDelimiter(stringCopy)) {
        extractCustomDelimiter(stringCopy, delimiter);
        *numbers = strchr(stringCopy, '\n') + 1;  // Numbers start after the newline following the delimiter
        handleEmptyDelimiter(*numbers, delimiter);
    }
}

void HandleNegatives(int negativeCount, int negatives[])
{
    if (negativeCount > 0) {
        throwNegativeException(negatives, negativeCount);
    }
}

int processTokenToInteger(char *numbers,char delimiter[])
{
    char *token = strtok(numbers, delimiter);
    int sum = 0;
    int negatives[256];  // Array to store negative numbers
    int negativeCount = 0;
    while (token != NULL) {
        int number = atoi(token);  // Convert token to integer
        if (!processNegativeNumber(number, negatives, &negativeCount)) {
            token = strtok(NULL, delimiter);
            continue;
        }
        addToSumIfValid(number, &sum);
        token = strtok(NULL, delimiter);
    }
    // Handle negatives
    HandleNegatives(negativeCount,negatives);
    return sum;
}

int add(const char *stringInput) {
    if (isNonNumericOrEmptyInput(stringInput)) {
        return 0;  // Return 0 for non-numeric or empty input
    }
    char stringCopy[512];
    strcpy(stringCopy, stringInput);  // Copy input string to a modifiable array
    char delimiter[256] = ",\n";  // Default delimiters are comma and newline
    char *numbers = stringCopy;
    validateCustomDelimiter(stringCopy, &numbers, delimiter);
    validateInput(numbers);
    return processTokenToInteger(numbers,delimiter);
}
