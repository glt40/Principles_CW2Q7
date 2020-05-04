#include <stdio.h>
#include <stdlib.h>

/*
 * finds the length of a string, given a pointer to the first char, returning an int
 */
int find_length(const char *string) {
    int i = 0;
    while (string[i] != '\0') {
        i++;
    }
    return i;
}

/*
 * finds the size, in characters, of a file, returns an int, parameter is the filename
 */
int find_size(char *filename) {
    FILE *fp = fopen(filename, "r");
    int i = 0;
    while (((char) fgetc(fp)) != EOF) {
        i++;
    }
    fclose(fp);
    return i;
}

/*
 * strips a string of spaces and transmutes all lower case to upper case, returns the string, parameter is a string
 */
char *strip_upper(char *string) {
    int length = find_length(string);
    char *newstr = malloc(sizeof(char) * (find_length(string) + 1));
    int newstrlen = 0;
    char c;
    // loops through each char and adds them to the new string only if they are letters and all in uppercase
    for (int i = 0; i < length; i++) {
        c = string[i];
        if (c >= 97 && c <= 122) {
            // lowercase, change c to uppercase and put in newstr
            c = c - 32;
            newstr[newstrlen] = c;
            newstrlen++;
        } else if (c >= 65 && c <= 90) {
            // uppercase, put c into newstr
            newstr[newstrlen] = c;
            newstrlen++;
        }
    }
    newstr[newstrlen] = '\0';
    return newstr;
}

/*
 * returns a string from a file, parameter is filename to read into the array
 */
char *file_to_arr(char *filename) {
    char *array = malloc(sizeof(char) * find_size("text.txt"));
    FILE *fp = fopen(filename, "r");
    int i = 0;
    char c;
    while ((c = (char) fgetc(fp)) != EOF) {
        array[i] = c;
        i++;
    }
    fclose(fp);
    return array;
}

/*
 * sorts an array alphabetically according to its header chars
 */
char **rearrange(char **input, int colnum) {
    char **output = malloc(sizeof(char*)*colnum);
    int j = 0;
    //key must be in capital letters
    for (char letter = 65; letter < 91; letter++) {
        for (int i = 0; i < colnum; i++) {
            if (input[i][0] == letter) {
                output[j] = input[i];
                // printf("%c", output [j][0]);
                j++;
            }
        }
    }
    return output;
}

/*
 * turns a string into columns in a 2-D array under headings of the encryption key one char at a time
 * returns 2-D array, parameters are message string and key string
 */
char **to_columns (char *message, char *key) {
    int keylen = find_length(key);
    int messagelen = find_length(message);
    // array will be 2-dimensional and have the number of letters in the key as number of columns
    char **array = malloc(sizeof(char *) * keylen);
    // loop assigns the "header" for each column
    for (int i = 0; i < keylen; ++i) {
        // allocate each column memory for messagelen / keylen chars, rounded up
        array[i] = malloc(sizeof(char) * (int)((messagelen + (keylen - 1)) / keylen));
        array[i][0] = key[i];
    }
    int rownum = 1;
    for (int j = 0; j < messagelen; j++) {
        if (j % keylen == 0 && j != 0) {
            // start a new row
            rownum++;
        }
        array[j % keylen][rownum] = message[j];
    }
    // add null car to each column, since they're printed as strings
    for (int k = 0; k < keylen; ++k) {
        if (messagelen % keylen == 0) {
            // each column is the same length
            array[k][rownum] = '\0';
        } else if (k < messagelen % keylen) {
            // these columns have one extra row than the following ones
            array[k][rownum + 1] = '\0';
        } else {
            // these columns have one fewer row that the previous ones
            array[k][rownum] = '\0';
        }
    }
    return array;
}

void encode(char *message, char *key) {
    int keylen = find_length(key);
    // strip out punctuation and spaces and put the message in uppercase
    message = strip_upper(message);
    key = strip_upper(key);
    char **array = to_columns(message, key);
    // now re-arrange columns according to the header chars' alphabetical order
    array = rearrange(array, keylen);
    // finally read off along columns
    for (int i = 1; i < keylen; ++i) {
        // print columns as if they were strings
        printf("%s",array[i]);
    }
    printf("\n");
}

/*
 * main method, calls encode on a file
 */
int main() {
    char *messagetext = file_to_arr("text.txt");
    encode(messagetext, "LOVELACE");
    return 0;
}
