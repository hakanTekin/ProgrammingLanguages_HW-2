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
#define OR_COMMAND "OR"
#define NOT_COMMAND "NOT"

#define DEFAULT_ARRAY_SIZE 1000

#define EXIT_WORD "exit"

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

int add(char *linePtr);

int tag(char *tagName, int *indexesToAdd, int indexesCount, struct tag_struct *tags[]);

int display(int id);

int find(char *linePtr, int *indexes);
int and_helper(char *linePtr, int *indexes, int *indexesSize);
int or_helper(char *linePtr, int *indexes, int *indexesSize);
int not_helper(char *linePtr, int *indexes, int *indexesSize);
int insert_new_indexes_to_result(int *dst, int *src, int *dstLength, int srcLength);

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
    int a[9999];
    printf("%d\n", sizeof(a) / sizeof(int));
    char buffer[DEFAULT_ARRAY_SIZE];
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
                printf("TAG name is : %s\n", tagName);
                //Not using atoi() since it is considered dangerous
                indexes[indexesCount++] = strtol(token, &temp, 10);
                printf("%d should be a id to be tagged with %s\n", indexes[indexesCount - 1], tagName);
                token = strtok(NULL, " ");
                if (token[strlen(token) - 1] == '\n')
                    token[strlen(token) - 1] = '\0';
            }
            int res = 0;
            tag(tagName, indexes, indexesCount, tags);

            if (res == -117)
                printf("Something is wrong, add returned -117\n");
        }
        else if (strcmp(token, FIND_COMMAND) == 0)
        {
            printf("HEHHE WEEEE\n");
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
        printf("->%s<-\n", buffer);
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
        for (int i = 0; i < sizeof(indexesToAdd) / sizeof(int); i++)
        {
            tags[tagIndexIfExists]->id_array[tags[tagIndexIfExists]->id_length] = indexesToAdd[i];
            tags[tagIndexIfExists]->id_length++;
        }
        printf("WELL. TAG SHOULD BE RECIEVED SOME NEW THINGYS\n");
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
        printf("WELL. A NEW TAG SHOULD BE ADDED\n");
    }
}

int display(int id)
{
    for (int i = 0; i < notes_length; i++)
    {
        struct note_struct *n = notes[i];
        if (n->id != NULL && n->id == id)
        {
            printf("Displaying note with ID : %d\n", n->id);
            for (int j = 0; j < n->note_line_count; j++)
            {
                printf("\t>%s\n", n->note_lines[j]);
            }
        }
        else
        {
        }
    }
    printf("Cannot find note with ID : %d", id);
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

    char buffer[DEFAULT_ARRAY_SIZE];
    char bufferCopy[DEFAULT_ARRAY_SIZE];

    while (linePtr[0] == ' ')
        linePtr++;

    char *indexStart = 0;
    char *indexEnd = 0;

    if (fgets(buffer, DEFAULT_ARRAY_SIZE, f))
    {

        int *indexes = malloc(sizeof(int) * DEFAULT_ARRAY_SIZE);
        int indexesSize = 0;
        strcpy(bufferCopy, buffer);

        indexEnd = strstr(indexStart, " ");
        char methodName[3];
        strncpy(methodName, indexStart, indexEnd - indexStart - 1);
        indexStart = indexEnd;
        if (methodName == NULL)
            printf("TOKEN EMPTY ON FIND method\n");
        else if (strcmp(methodName, AND_COMMAND) == 0)
        {
            and_helper(indexStart, indexes, &indexesSize);
            insert_new_indexes_to_result(resultArray, indexes, &resultArraySize, indexesSize);
        }
        else if (strcmp(methodName, OR_COMMAND) == 0)
        {
            or_helper(indexStart, indexes, &indexesSize);
            insert_new_indexes_to_result(resultArray, indexes, &resultArraySize, indexesSize);
        }
        else if (strcmp(methodName, NOT_COMMAND) == 0)
        {
            not_helper(indexStart, indexes, &indexesSize);
            insert_new_indexes_to_result(resultArray, indexes, &resultArraySize, indexesSize);
        }
        else
        {
            printf("DUNNO MAN THIS COMMAND LOOKS BS. FIND RECIEVED UNKNOWN COMMAND\n");
        }
    }
}
int and_helper(char *linePtr, int *resultArray, int *resultArraySize)
{

    int indexes[DEFAULT_ARRAY_SIZE];
    int indexesSize = 0;

    while (linePtr[0] == ' ')
        linePtr++;
    char *s = 0;
    char *f = 0;

    int currentIteration = 0; //iteration handles first search problem. you cannot apply 'and' operation with only one side of an equation
    while (1)
    {
        s = f + 1;
        f = strstr(s, " ");
        char curWord[50];
        strncpy(curWord, linePtr, f - s);

        if (strcmp(curWord, ")") == 0)
        { //If ) is found. this means the method is done, return to caller.

            return 117;
        }
        else if (strcmp(curWord, AND_COMMAND))
        {
            and_helper(s, indexes, &indexesSize);
            insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize);
        }
        else if (strcmp(curWord, OR_COMMAND))
        {
            or_helper(s, indexes, &indexesSize);
            insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize);
        }
        else if (strcmp(curWord, NOT_COMMAND))
        {
            not_helper(s, indexes, &indexesSize);
            insert_new_indexes_to_result(resultArray, indexes, resultArraySize, indexesSize);
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
                for(int i = 0; i<tags_legth; i++){
                    if(strcmp(tags[i]->name, curWord) == 0){
                        for(int j = 0; j<indexesSize; j++){
                            int isThere = 0;
                            for(int k = 0; k<tags[i]->id_length; k++){
                                if(tags[i]->id_array[k] == indexes[j]){
                                    isThere = 1;
                                }
                            }
                            if(isThere == 0){
                                indexes[j] = -1;
                            }
                        }
                    }
                }

                //At this point, indexes that are to be removed is marked by -1
                int newIndexes[indexesSize];
                int newIndexesCount = 0;
                for(int i = 0; i<indexesSize; i++){
                    if(indexes[i] != -1){
                        newIndexes[newIndexesCount++] = indexes[i];
                    }
                }

                for(int i = 0; i<newIndexesCount; i++){
                    indexes[i] = newIndexes[i];
                }
                indexesSize = newIndexesCount;

            }
            currentIteration++; //A word is processed, increase iteration
        }
    }

    for (int i = 0; i < indexesSize; i++)
    {
        printf(">%d\n is one index found babyyy", indexes[i]);
    }
}
int or_helper(char *linePtr, int *resultArray, int *resultArraySize)
{
    while (linePtr[0] == ' ')
        linePtr++;
}
int not_helper(char *linePtr, int *resultArray, int *resultArraySize)
{
    while (linePtr[0] == ' ')
        linePtr++;
}

//dst is the main result array, src is the temp results.
int insert_new_indexes_to_result(int *dst, int *src, int *dstLength, int srcLength)
{
    for (int i = 0; i < srcLength; i++)
    {
        dst[*dstLength++] = src[i];
    }
}
