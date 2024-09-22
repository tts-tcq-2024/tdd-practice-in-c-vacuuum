#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Exception handler for negatives
void throwNegativeException(int negatives[], int count) {
    printf("Exception: negatives not allowed ");
    for (int i = 0; i < count; i++) {
        printf("%d ", negatives[i]);
    }
    printf("\n");
    exit(1);
}

// Helper function to check if a comma should be inserted between two digits
int shouldInsertComma(char currentChar, char nextChar) {
    return (currentChar >= '0' && currentChar <= '9') && (nextChar >= '0' && nextChar <= '9');
}

// Function to handle the empty delimiter case and insert commas between adjacent digits
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

// Helper function to check if the comma is followed by an invalid character or no number
int isInvalidCommaSequence(char currentChar, char nextChar) {
    return currentChar == ',' && (nextChar == '\0' || !(nextChar >= '0' && nextChar <= '9'));
}

// Helper function to check if the newline is followed by an invalid character or no number
int isInvalidNewlineSequence(char currentChar, char nextChar) {
    return currentChar == '\n' && (nextChar == '\0' || !(nextChar >= '0' && nextChar <= '9'));
}

// Function to validate input and reject invalid cases like "1,\n"
void validateInput(const char *numbers) {
    for (int i = 0; numbers[i] != '\0'; i++) {
        if (isInvalidCommaSequence(numbers[i], numbers[i + 1])) {
            printf("Error: Invalid input sequence at position %d (Comma).\n", i);
            exit(1);
        }
        if (isInvalidNewlineSequence(numbers[i], numbers[i + 1])) {
            printf("Error: Invalid input sequence at position %d (Newline).\n", i);
            exit(1);
        }
    }
}

// Function to check for non-numeric input or empty string and return 0
int isNonNumericOrEmptyInput(const char *input) {
    if (strlen(input) == 0) {
        return 1;  // Input is empty, return 1 to indicate it's invalid
    }

    int containsDigit = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] >= '0' && input[i] <= '9') {
            containsDigit = 1;
        }
    }
    return !containsDigit;  // Return 1 if no digit is found, indicating invalid input
}

// Function to extract the custom delimiter from the string
void extractCustomDelimiter(char *stringCopy, char *delimiter) {
    char *start = strchr(stringCopy, '[');
    char *end = strchr(stringCopy, ']');
    if (start != NULL && end != NULL && end > start) {
        strncpy(delimiter, start + 1, end - start - 1);  // Extract custom delimiter between [ and ]
        delimiter[end - start - 1] = '\0';  // Null-terminate custom delimiter
    }
}

// Function to handle the case when the delimiter is empty
void handleEmptyDelimiter(char *numbers, char *delimiter) {
    if (strcmp(delimiter, "") == 0) {
        insertCommasBetweenDigits(numbers);  // Insert commas between adjacent digits
        strcpy(delimiter, ",");  // Use commas as the effective delimiter
    }
}

// Helper function to check for negative numbers and add to the sum
int processNumber(int number, int *sum, int negatives[], int *negativeCount) {
    if (number < 0) {
        negatives[(*negativeCount)++] = number;  // Store negative number
        return 0;  // Return 0 to indicate it should not be added to the sum
    }
    if (number <= 1000) {
        *sum += number;  // Add number if it's 1000 or less
    }
    return 1;  // Return 1 to indicate processing was successful
}

// Function to validate if custom delimiter is set and assign numbers to process
void validateCustomDelimiter(char *stringCopy, char **numbers, char *delimiter) {
    if (strncmp(stringCopy, "//", 2) == 0) {
        extractCustomDelimiter(stringCopy, delimiter);  // Extract custom delimiter
        *numbers = strchr(stringCopy, '\n') + 1;  // Numbers start after the newline following the delimiter
        handleEmptyDelimiter(*numbers, delimiter);  // Handle special case for empty delimiter
    }
}

// Function to add numbers from the string
int add(const char *stringInput) {
    if (isNonNumericOrEmptyInput(stringInput)) {
        return 0;  // Return 0 for non-numeric or empty input
    }

    char stringCopy[512];
    strcpy(stringCopy, stringInput);  // Copy input string to a modifiable array

    char delimiter[256] = ",\n";  // Default delimiters are comma and newline
    char *numbers = stringCopy;  // Start processing the numbers part directly

    validateCustomDelimiter(stringCopy, &numbers, delimiter);  // Validate and set custom delimiter

    validateInput(numbers);  // Validate input (to handle cases like "1,\n")

    char *token = strtok(numbers, delimiter);
    int sum = 0;
    int negatives[256];  // Array to store negative numbers
    int negativeCount = 0;

    while (token != NULL) {
        int number = atoi(token);  // Convert token to integer

        // Process the number and handle sums and negatives
        processNumber(number, &sum, negatives, &negativeCount);
        
        token = strtok(NULL, delimiter);  // Move to the next token
    }

    // Handle negatives
    if (negativeCount > 0) {
        throwNegativeException(negatives, negativeCount);
    }

    return sum;  // Return the sum if no errors
}

int main() {
    // Custom delimiter test
    const char* test1 = "//;\n1;2";
    int result1 = add(test1);
    printf("Sum of numbers: %d (Expected: 3)\n", result1);

    // Test with any length of delimiters
    const char* test2 = "//[]\n12***3";
    int result2 = add(test2);
    printf("Sum of numbers: %d (Expected: 6)\n", result2);

    // Test with negative numbers
    const char* test3 = "//;\n1;-4;7;8";
    int result3 = add(test3);
    printf("Sum of numbers: %d\n", result3);  // Expected to throw an exception for negatives

    return 0;
}

