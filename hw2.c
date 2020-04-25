/*
    1- It is users responsibility to enter UNIQUE ID's
    2- Each note has at least ONE LINE of text
    3- TAG allowed chars : letters, digits
    4- Find is in ASCENDING order by ID's
    5- AND( expression ) , watch the spaces
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGHT 501 //Normally 500, minding the termination char too.

#define ADD_COMMAND "ADD"
#define ADD_TERMINATE_WORD "END"

#define TAG_COMMAND "TAG"
#define MAX_TAG_NAME_LENGTH 100
#define TAG_TERMINATE_NUMBER -1

#define DISPLAY_COMMAND "DISPLAY"

#define FIND_COMMAND "FIND"
#define AND_COMMAND "AND"
#define AND_COMMAND_PARANTHESES "AND("
#define OR_COMMAND "OR"
#define OR_COMMAND_PARANTHESES "OR("
#define NOT_COMMAND "NOT"
#define NOT_COMMAND_PARANTHESES "NOT("

#define DEFAULT_ARRAY_SIZE 1000

#define EXIT_WORD "exit\n"

enum find_types
{
    and,
    not,
    or
};

struct tag_struct
{
    char name[MAX_TAG_NAME_LENGTH];
    int id_array[DEFAULT_ARRAY_SIZE];
    int id_length;
};

struct note_struct
{
    int id;
    char note_lines[MAX_LINE_LENGHT][MAX_LINE_LENGHT];
    int note_line_count;
};

struct morse_struct
{
    char *morse;
    char letter;
};

int add(char *linePtr);

int tag(char *tagName, int *indexesToAdd, int indexesCount, struct tag_struct *tags[]);

int display(int id);

int find(char *linePtr, int *indexes);
int and_helper(char *linePtr, int *indexes, int *indexesSize);
int or_helper(char *linePtr, int *indexes, int *indexesSize);
int not_helper(char *linePtr, int *indexes, int *indexesSize);
int and_insert_new_indexes_to_result(int *dst, int *src, int *dstLength, int srcLength, int iter);
int or_insert_new_indexes_to_result(int *dst, int *src, int *dstLength, int srcLength, int iter);
int not_insert_new_indexes_to_result(int *dst, int *src, int *dstLength, int srcLength, int iter);
int sort_ascending_order(int *r, int *rs);
int print_result_array(int *r, int *rs);

int reverse_array_for_not_ending(int *r, int *rs);

void fix_array(int *dst, int *dstLength);

//GLOBAL VARIABLES
struct note_struct *notes[DEFAULT_ARRAY_SIZE];

int notes_length = 0;

struct tag_struct *tags[DEFAULT_ARRAY_SIZE];
int tags_legth = 0;

//Strtok is destructive and it is heavily used in this program. There is no going back when a word is read.
FILE *f = NULL;
int main(int argc, char *argv[])
{
    f = fopen("input.txt", "r+");
    char *buffer = malloc(sizeof(char) * DEFAULT_ARRAY_SIZE);
    char bufferCopy[DEFAULT_ARRAY_SIZE];
    char *token = NULL;

    while (fgets(buffer, DEFAULT_ARRAY_SIZE, f))
    {
        strcpy(bufferCopy, buffer);
        if (strcmp(buffer, EXIT_WORD) == 0)
            break; //END entered, exit the loop

        token = strtok(bufferCopy, " ");

        if (strcmp(token, ADD_COMMAND) == 0)
        {
            token = strtok(NULL, " ");
            int res = add(token);
            if (res == -117)
                printf("Something is wrong, add returned -117\n");
        }
        else if (strcmp(token, TAG_COMMAND) == 0)
        {
            int indexes[DEFAULT_ARRAY_SIZE];
            int indexesCount = 0;
            char *tagName = strtok(NULL, " ");
            //Go until -1 is recieved and put each integer to the array
            token = strtok(NULL, " ");
            while (token != NULL && strcmp(token, "-1") != 0)
            {
                char *temp;
                //Not using atoi() since it is considered dangerous
                indexes[indexesCount++] = strtol(token, &temp, 10);
                token = strtok(NULL, " ");
                if (token != NULL && token[strlen(token) - 1] == '\n')
                    token[strlen(token) - 1] = '\0';
            }
            int res = 0;
            tag(tagName, indexes, indexesCount, tags);

            if (res == -117)
                printf("Something is wrong, add returned -117\n");
        }
        else if (strcmp(token, FIND_COMMAND) == 0)
        {
            int *results = malloc(sizeof(DEFAULT_ARRAY_SIZE) / sizeof(int));
            char *s = strstr(buffer, " ") + 1;
            int res = find(s, results);
            if (res == -117)
                printf("Something is wrong, add returned -117\n");
        }
        else if (strcmp(token, DISPLAY_COMMAND) == 0)
        {
            char *temp;
            token = strtok(NULL, " ");
            int id_to_send = strtol(token, &temp, 10);
            int res = display(id_to_send);
            if (res == -117)
                printf("Something is wrong, add returned -117\n");
        }
        else
        {
            printf("UNKNOWN METHOD NAME, first command should always be one of : ADD, TAG, FIND, DISPLAY\nThis program is case sensitive, maybe you mistyped something.\n");
        }
    }
    return 0;
}

//this method will recieve will the part after "ADD " -> ! WATCH OUT THE SPACE !
int add(char *linePtr)
{
    char buffer[MAX_LINE_LENGHT];
    struct note_struct *note = malloc(sizeof(struct note_struct));
    int at_line = 0;
    note->note_line_count = 0;
    char *temp; //Temp is useless, just for strtol

    note->id = strtol(linePtr, &temp, 10);
    while (fgets(buffer, MAX_LINE_LENGHT - 1, f))
    {
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';
        if (strcmp(buffer, ADD_TERMINATE_WORD) == 0)
            break; //This loop is done

        strcpy(note->note_lines[note->note_line_count++], buffer);
    }
    notes[notes_length++] = note;

    return 0;
}

int tag(char *tagName, int *indexesToAdd, int indexesCount, struct tag_struct *tags[])
{
    int tagIndexIfExists = -1; //If this value is -1, then no such tag is found.
    for (int i = 0; i < tags_legth; i++)
        if (strcmp(tags[i]->name, tagName) == 0) //TAG match ! There already is such a TAG
            tagIndexIfExists = i;

    if (tagIndexIfExists != -1 && tagIndexIfExists > -1)
    {
        for (int i = 0; i < indexesCount; i++)
        {
            tags[tagIndexIfExists]->id_array[tags[tagIndexIfExists]->id_length] = indexesToAdd[i];
            tags[tagIndexIfExists]->id_length++;
        }
    }
    else if (tagIndexIfExists == -1)
    {
        struct tag_struct *temp = malloc(sizeof(struct tag_struct));
        temp->id_length = 0;
        strcpy(temp->name, tagName);
        for (int i = 0; i < indexesCount; i++)
        {
            temp->id_array[i] = indexesToAdd[i];
            temp->id_length++;
        }
        tags[tags_legth++] = temp;
    }
}

int display(int id)
{
    for (int i = 0; i < notes_length; i++)
    {
        struct note_struct *n = notes[i];
        if (n->id == id)
        {
            printf("Id: %d\n", id);
            for (int j = 0; j < n->note_line_count; j++)
            {
                printf("%s\n", n->note_lines[j]);
            }
            return 117;
        }
        else
        {
        }
    }
    printf("Cannot find note with ID : %d\n", id);
}

/*
*Algorithm for find : 
* start from method Find();
* find has a resultArray[] : int, holding the indexes of matching id's
* parse word by word
* if AND(,OR(,NOT( found : call their respective functions and send the char pointer.
* a result array should be returned form these functions, append 'em to the main result array
* else, it must be a ), return the result array

*AND,OR,NOT methods : 
* each function creates its own int[], this will hold the results.
* each function needs to read word by word starting from the pointer it is given
* if a ) is found, the result should be returned 
*AND , OR, NOT recieves 
*
*/
int find(char *linePtr, int *finalArray)
{
    int *resultArray = malloc(sizeof(int) * DEFAULT_ARRAY_SIZE);
    int resultArraySize = 0;

    char bufferCopy[DEFAULT_ARRAY_SIZE];

    while (linePtr[0] == ' ')
        linePtr++;

    if (linePtr != NULL && linePtr[0] != ')')
    {

        int *indexes = malloc(sizeof(int) * DEFAULT_ARRAY_SIZE);
        int indexesSize = 0;
        strcpy(bufferCopy, linePtr);

        char *indexStart = &bufferCopy[0];
        char *indexEnd = &bufferCopy[0];

        indexEnd = strstr(indexStart, " ");
        char methodName[4];
        strncpy(methodName, indexStart, indexEnd - indexStart - 1);
        methodName[indexEnd - indexStart - 1] = '\0';
        indexStart = indexEnd;
        if (methodName == NULL)
            printf("TOKEN EMPTY ON FIND method\n");
        else if (strcmp(methodName, AND_COMMAND) == 0 || strcmp(methodName, AND_COMMAND_PARANTHESES) == 0)
        {
            and_helper(indexStart, indexes, &indexesSize);
            and_insert_new_indexes_to_result(resultArray, indexes, &resultArraySize, indexesSize, 0);
        }
        else if (strcmp(methodName, OR_COMMAND) == 0 || strcmp(methodName, OR_COMMAND_PARANTHESES) == 0)
        {
            or_helper(indexStart, indexes, &indexesSize);
            or_insert_new_indexes_to_result(resultArray, indexes, &resultArraySize, indexesSize, 0);
        }
        else if (strcmp(methodName, NOT_COMMAND) == 0 || strcmp(methodName, NOT_COMMAND_PARANTHESES) == 0)
        {
            not_helper(indexStart, indexes, &indexesSize);
            not_insert_new_indexes_to_result(resultArray, indexes, &resultArraySize, indexesSize, 0);
        }
        else
        {
            printf("DUNNO MAN THIS COMMAND LOOKS BS. FIND RECIEVED UNKNOWN COMMAND\n");
        }
    }
    print_result_array(resultArray, &resultArraySize);
    return 117;
}

int sort_ascending_order(int *r, int *rs)
{
    int x[9999];
    int xs = *rs;

    for (size_t i = 0; i < xs; i++)
    {
        x[i] = r[i];
    }

    for (size_t i = 0; i < xs; i++)
    {
        for (size_t j = 0; j < xs; j++)
        {
            if (x[i] < x[j])
            {
                int temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }

    for (size_t i = 0; i < xs; i++)
    {
        r[i] = x[i];
    }
    *rs = xs;
    return -117;
}

int print_result_array(int *r, int *rs)
{
    sort_ascending_order(r, rs);
    printf("Results:\n");
    for (int i = 0; i < *rs; i++)
    {
        for (int j = 0; j < notes_length; j++)
        {
            if (r[i] == notes[j]->id)
            {
                printf("Id: %d %s\n", notes[j]->id, notes[j]->note_lines[0]);
            }
        }
    }
    return -117;
}


int and_helper(char *linePtr, int *resultArray, int *resultArraySize)
{
    int *indexes = malloc(sizeof(int) * DEFAULT_ARRAY_SIZE);
    int indexesSize = 0;

    while (linePtr[0] == ' ')
        linePtr++;
    char *s = linePtr;
    char *f = linePtr;

    int currentIteration = 0; //iteration handles first search problem. you cannot apply 'and' operation with only one side of an equation
    while (1)
    {
        indexesSize = 0;
        
        s = f;
        f = strstr(s + 1, " ");
        if (f == NULL)
            f = &s[strlen(s) - 1];
        while (s[0] == ' ')
            s++;
        char curWord[50];
        if (s == f)
        {
            curWord[0] = ')';
        }
        else
        {
            strncpy(curWord, s, f - s);
            curWord[f - s] = '\0';
        }

        if (curWord[0] == ')')
        { //If ) is found. this means the method is done, return to caller.
            linePtr = f;
            return 117;
        }

        else if (strcmp(curWord, AND_COMMAND) == 0 || strcmp(curWord, AND_COMMAND_PARANTHESES) == 0)
        {
            and_helper(f, indexes, &indexesSize);

            while (f[0] != ')')
                f++;
            f += 2;

            if (currentIteration == 0)
            {
                and_insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
                for (size_t i = 0; i < indexesSize; i++)
                    indexes[i] = -1;
                indexesSize = 0;
                /* code */
            }
            else
            {
                //This means there is a result array, the new results should be compared with the main result. If both arrays dont have something, result should remove that id
                for (size_t i = 0; i < *resultArraySize; i++)
                {
                    int isThere = 0;
                    for (size_t j = 0; j < indexesSize; j++)
                    {
                        if (resultArray[i] == indexes[j])
                            isThere = 1;
                    }
                    if (isThere == 0) //If not found, remove that item
                        resultArray[i] = -1;
                }

                fix_array(resultArray, resultArraySize);
            }
        }
        else if (strcmp(curWord, OR_COMMAND) == 0 || strcmp(curWord, OR_COMMAND_PARANTHESES) == 0)
        {
            or_helper(f, indexes, &indexesSize);
            //insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration)
            while (f[0] != ')')
                f++;
            f += 2;

            if (currentIteration == 0)
            {
                and_insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
                for (size_t i = 0; i < indexesSize; i++)
                    indexes[i] = -1;
                indexesSize = 0;
                /* code */
            }
            else
            {
                //This means there is a result array, the new results should be compared with the main result. If both arrays dont have something, result should remove that id
                for (size_t i = 0; i < *resultArraySize; i++)
                {
                    int isThere = 0;
                    for (size_t j = 0; j < indexesSize; j++)
                    {
                        if (resultArray[i] == indexes[j])
                            isThere = 1;
                    }
                    if (isThere == 0) //If not found, remove that item
                        resultArray[i] = -1;
                }

                fix_array(resultArray, resultArraySize);
            }
        }
        else if (strcmp(curWord, NOT_COMMAND) == 0 || strcmp(curWord, NOT_COMMAND_PARANTHESES) == 0)
        {
            not_helper(f, indexes, &indexesSize);
            //insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
            while (f[0] != ')')
                f++;
            f += 2;

            if (currentIteration == 0)
            {
                and_insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
                for (size_t i = 0; i < indexesSize; i++)
                    indexes[i] = -1;
                indexesSize = 0;
                /* code */
            }
            else
            {
                //This means there is a result array, the new results should be compared with the main result. If both arrays dont have something, result should remove that id
                for (size_t i = 0; i < *resultArraySize; i++)
                {
                    int isThere = 0;
                    for (size_t j = 0; j < indexesSize; j++)
                    {
                        if (resultArray[i] == indexes[j])
                            isThere = 1;
                    }
                    if (isThere == 0) //If not found, remove that item
                        resultArray[i] = -1;
                }

                fix_array(resultArray, resultArraySize);
            }
        }
        else
        { //else means there is a TAG name specified (e.g. 'todo')

            if (currentIteration == 0)
            {
                for (int i = 0; i < tags_legth; i++)
                {
                    if (strcmp(tags[i]->name, curWord) == 0)
                    { //There is a tag with that name, add the contents to result
                        for (int j = 0; j < tags[i]->id_length; j++)
                        {
                            indexes[indexesSize++] = tags[i]->id_array[j];
                        }
                    }
                }
            }
            else
            {
                for (size_t i = 0; i < tags_legth; i++)
                {
                    if (strcmp(tags[i]->name, curWord) == 0)
                    {
                        for (size_t j = 0; j < tags[i]->id_length; j++)
                        {
                            indexes[indexesSize++] = tags[i]->id_array[j];
                        }
                    }
                }
            }
            and_insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
        }
        currentIteration++; //A word is processed, increase iteration
    }

    for (int i = 0; i < indexesSize; i++)
    {
        printf(">%d\n is one index found babyyy\n", indexes[i]);
    }
}

int or_helper(char *linePtr, int *resultArray, int *resultArraySize)
{
    int *indexes = malloc(sizeof(int) * DEFAULT_ARRAY_SIZE);
    int indexesSize = 0;

    while (linePtr[0] == ' ')
        linePtr++;
    char *s = linePtr;
    char *f = linePtr;

    int currentIteration = 0; //iteration handles first search problem. you cannot apply 'and' operation with only one side of an equation
    while (1)
    {
        indexesSize = 0;
        s = f;
        f = strstr(s + 1, " ");
        if (f == NULL)
            f = &s[strlen(s) - 1];
        while (s[0] == ' ')
            s++;
        char curWord[50];
        strncpy(curWord, s, f - s);
        curWord[f - s] = '\0';

        if (curWord[0] == ')')
        { //If ) is found. this means the method is done, return to caller.
            linePtr = f;
            return 117;
        }

        else if (strcmp(curWord, AND_COMMAND) == 0 || strcmp(curWord, AND_COMMAND_PARANTHESES) == 0)
        {
            and_helper(f, indexes, &indexesSize);

            while (f[0] != ')')
                f++;
            f += 2;

            or_insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
            for (size_t i = 0; i < indexesSize; i++)
                indexes[i] = -1;
            indexesSize = 0;
            fix_array(resultArray, resultArraySize);
        }
        else if (strcmp(curWord, OR_COMMAND) == 0 || strcmp(curWord, OR_COMMAND_PARANTHESES) == 0)
        {
            or_helper(f, indexes, &indexesSize);
            //insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration)

            while (f[0] != ')')
                f++;
            f += 2;

            or_insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
            for (size_t i = 0; i < indexesSize; i++)
                indexes[i] = -1;
            indexesSize = 0;
            fix_array(resultArray, resultArraySize);
        }
        else if (strcmp(curWord, NOT_COMMAND) == 0 || strcmp(curWord, NOT_COMMAND_PARANTHESES) == 0)
        {
            not_helper(f, indexes, &indexesSize);

            while (f[0] != ')')
                f++;
            f += 2;

            or_insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
            for (size_t i = 0; i < indexesSize; i++)
                indexes[i] = -1;
            indexesSize = 0;
            fix_array(resultArray, resultArraySize);
        }
        else
        { //else means there is a TAG name specified (e.g. 'todo')

            if (currentIteration == 0)
            {
                for (int i = 0; i < tags_legth; i++)
                {
                    if (strcmp(tags[i]->name, curWord) == 0)
                    { //There is a tag with that name, add the contents to result
                        for (int j = 0; j < tags[i]->id_length; j++)
                        {
                            indexes[indexesSize++] = tags[i]->id_array[j];
                        }
                    }
                }
            }
            else
            {
                //Find the searched tag,
                //add each item to the indexes array if it is not in the damn array

                for (int i = 0; i < tags_legth; i++)
                {
                    if (strcmp(tags[i]->name, curWord) == 0)
                    {
                        for (int j = 0; j < tags[i]->id_length; j++)
                        {
                            int isThere = 0;
                            for (int k = 0; k < indexesSize; k++)
                            {
                                if (tags[i]->id_array[j] == indexes[k])
                                    isThere = 1;
                            }
                            if (isThere == 0)
                            {
                                indexes[indexesSize++] = tags[i]->id_array[j];
                            }
                        }
                    }
                }

                //At this point, indexes that are to be removed is marked by -1
                int newIndexes[indexesSize];
                int newIndexesCount = 0;
                for (int i = 0; i < indexesSize; i++)
                {
                    if (indexes[i] != -1)
                    {
                        newIndexes[newIndexesCount++] = indexes[i];
                    }
                }

                for (int i = 0; i < newIndexesCount; i++)
                {
                    indexes[i] = newIndexes[i];
                }
                indexesSize = newIndexesCount;
            }
            or_insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
        }
        currentIteration++; //A word is processed, increase iteration
    }

    for (int i = 0; i < indexesSize; i++)
    {
        printf(">%d\n is one index found babyyy\n", indexes[i]);
    }
}

int not_helper(char *linePtr, int *resultArray, int *resultArraySize)
{
    int *indexes = malloc(sizeof(int) * DEFAULT_ARRAY_SIZE);
    int indexesSize = 0;

    while (linePtr[0] == ' ')
        linePtr++;
    char *s = linePtr;
    char *f = linePtr;

    int currentIteration = 0; //iteration handles first search problem. you cannot apply 'and' operation with only one side of an equation
    while (1)
    {
        indexesSize = 0;
        s = f;
        f = strstr(s + 1, " ");
        if (f == NULL)
            f = &s[strlen(s) - 1];
        while (s[0] == ' ')
            s++;
        char curWord[50];
        strncpy(curWord, s, f - s);
        curWord[f - s] = '\0';

        if (curWord[0] == ')')
        { //If ) is found. this means the method is done, return to caller.
            reverse_array_for_not_ending(resultArray, resultArraySize);
            linePtr = f;
            return 117;
        }

        else if (strcmp(curWord, AND_COMMAND) == 0 || strcmp(curWord, AND_COMMAND_PARANTHESES) == 0)
        {
            and_helper(f, indexes, &indexesSize);

            while (f[0] != ')')
                f++;
            f += 2;

            not_insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
            for (size_t i = 0; i < indexesSize; i++)
                indexes[i] = -1;
            indexesSize = 0;
            fix_array(resultArray, resultArraySize);
        }
        else if (strcmp(curWord, OR_COMMAND) == 0 || strcmp(curWord, OR_COMMAND_PARANTHESES) == 0)
        {
            or_helper(f, indexes, &indexesSize);
            //insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration)

            while (f[0] != ')')
                f++;
            f += 2;

            not_insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
            for (size_t i = 0; i < indexesSize; i++)
                indexes[i] = -1;
            indexesSize = 0;
            fix_array(resultArray, resultArraySize);
        }
        else if (strcmp(curWord, NOT_COMMAND) == 0 || strcmp(curWord, NOT_COMMAND_PARANTHESES) == 0)
        {
            not_helper(f, indexes, &indexesSize);

            while (f[0] != ')')
                f++;
            f += 2;

            not_insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
            for (size_t i = 0; i < indexesSize; i++)
                indexes[i] = -1;
            indexesSize = 0;
            fix_array(resultArray, resultArraySize);
        }
        else
        { //else means there is a TAG name specified (e.g. 'todo')

            if (currentIteration == 0)
            {
                for (int i = 0; i < tags_legth; i++)
                {
                    if (strcmp(tags[i]->name, curWord) == 0)
                    { //There is a tag with that name, add the contents to result
                        for (int j = 0; j < tags[i]->id_length; j++)
                        {
                            indexes[indexesSize++] = tags[i]->id_array[j];
                        }
                    }
                }
            }
            else
            {
                //Find the searched tag,
                //add each item to the indexes array if it is not in the damn array

                for (int i = 0; i < tags_legth; i++)
                {
                    if (strcmp(tags[i]->name, curWord) == 0)
                    {
                        for (int j = 0; j < tags[i]->id_length; j++)
                        {
                            int isThere = 0;
                            for (int k = 0; k < indexesSize; k++)
                            {
                                if (tags[i]->id_array[j] == indexes[k])
                                    isThere = 1;
                            }
                            if (isThere == 0)
                            {
                                indexes[indexesSize++] = tags[i]->id_array[j];
                            }
                        }
                    }
                }

                //At this point, indexes that are to be removed is marked by -1
                int newIndexes[indexesSize];
                int newIndexesCount = 0;
                for (int i = 0; i < indexesSize; i++)
                {
                    if (indexes[i] != -1)
                    {
                        newIndexes[newIndexesCount++] = indexes[i];
                    }
                }

                for (int i = 0; i < newIndexesCount; i++)
                {
                    indexes[i] = newIndexes[i];
                }
                indexesSize = newIndexesCount;
            }
            not_insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize, currentIteration);
        }
        currentIteration++; //A word is processed, increase iteration
    }

    for (int i = 0; i < indexesSize; i++)
    {
        printf(">%d\n is one index found babyyy\n", indexes[i]);
    }
}

void fix_array(int *dst, int *dstLength)
{
    for (int i = 1; i < *dstLength; i++)
    {
        if (dst[i] != -1)
            while (dst[i - 1] == -1 && i >= 0 && i < *dstLength)
            {
                dst[i - 1] = dst[i];
                dst[i] = -1;
                i--;
            }
    }
    for (int i = 0; i < *dstLength; i++)
        if (dst[i] == -1)
        {
            *dstLength = i;
            break;
        }
}

//dst is the main result array, src is the temp results.
int and_insert_new_indexes_to_result(int *dst, int *src, int *dstLength, int srcLength, int iter)
{

    if (iter == 0)
    {
        int x = *dstLength;
        for (int i = 0; i < srcLength; i++)
        {
            dst[x++] = src[i];
        }
        *dstLength = x;
    }
    else
    {
        int x = *dstLength;
        for (int i = 0; i < *dstLength; i++)
        {
            int isThere = 0;
            for (int j = 0; j < srcLength; j++)
            {
                if (dst[i] == src[j])
                    isThere = 1;
            }
            if (isThere == 0)
                dst[i] = -1;
        }
        *dstLength = x;
    } //Result should be ready. Just remove -1's and shift the damn array
    fix_array(dst, dstLength);
}

int or_insert_new_indexes_to_result(int *dst, int *src, int *dstLength, int srcLength, int iter)
{

    if (iter == 0)
    {
        int x = *dstLength;
        for (int i = 0; i < srcLength; i++)
        {
            dst[x++] = src[i];
        }
        *dstLength = x;
    }
    else
    {
        int x = *dstLength;
        for (int j = 0; j < srcLength; j++)
        {
            int isThere = 0;
            for (int i = 0; i < *dstLength; i++)
            {
                if (dst[i] == src[j])
                    isThere = 1;
            }
            if (isThere == 0)
                dst[x++] = src[j];
        }
        *dstLength = x;
    } //Result should be ready. Just remove -1's and shift the damn array
    fix_array(dst, dstLength);
}

int not_insert_new_indexes_to_result(int *dst, int *src, int *dstLength, int srcLength, int iter)
{
    if (iter == 0)
    {
        int x = *dstLength;
        for (int i = 0; i < srcLength; i++)
        {
            dst[x++] = src[i];
        }
        *dstLength = x;
    }
    else
    {
        int x = *dstLength;
        for (int j = 0; j < srcLength; j++)
        {
            int isThere = 0;
            for (int i = 0; i < *dstLength; i++)
            {
                if (dst[i] == src[j])
                    isThere = 1;
            }
            if (isThere == 0)
                dst[x++] = src[j];
        }
        *dstLength = x;
    } //Result should be ready. Just remove -1's and shift the damn array
    fix_array(dst, dstLength);
}

//Before each 'NOT' operation is completed, the results needs to be reversed to achieve NOT effect.
//What NOT does is apply OR. And then reverse the entire array.
//This method is that reversal operation
int reverse_array_for_not_ending(int *r, int *rs)
{
    int n[DEFAULT_ARRAY_SIZE] = {-1};
    int ns = 0;
    for (int i = 0; i < tags_legth; i++)
    {
        for (int j = 0; j < tags[i]->id_length; j++)
        {
            int isThere = 0;
            for (int k = 0; k < *rs; k++)
            {
                if (tags[i]->id_array[j] == r[k])
                    isThere = 1;
            }
            if (isThere == 0)
            {
                n[ns++] = (int) tags[i]->id_array[j];
            }
        }
    }

    for (int i = 0; i < ns; i++)
        r[i] = n[i];
    *rs = ns;
}