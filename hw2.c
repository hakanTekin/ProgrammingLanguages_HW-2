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

#define DEFAULT_ARRAY_SIZE 9999

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
};

struct note_struct
{
    int id;
    char note_lines[MAX_LINE_LENGHT][MAX_LINE_LENGHT];
    int note_line_count;
};

int add(char *linePtr);

int tag(char *tagName, int *indexesToAdd, struct tag_struct tags[]);

int display(int id);

int find(char *linePtr, int *indexes);
int and_helper(char *linePtr, int *indexes);
int or_helper(char *linePtr, int *indexes);
int not_helper(char *linePtr, int *indexes);

//GLOBAL VARIABLES
struct note_struct *notes[DEFAULT_ARRAY_SIZE];

int notes_length = 0;

struct tag_struct tags[DEFAULT_ARRAY_SIZE];
int tags_legth = 0;

//Strtok is destructive and it is heavily used in this program. There is no going back when a word is read.
int main(int argc, char *argv[])
{
    char buffer[DEFAULT_ARRAY_SIZE];
    char *token = NULL;

    while (fgets(buffer, DEFAULT_ARRAY_SIZE, stdin))
    {
        if(strcmp(buffer, EXIT_WORD) == 0)
            break; //END entered, exit the loop

        token = strtok(buffer, " ");

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

            //Go until -1 is recieved and put each integer to the array
            while(token != NULL && strcmp(token, "-1")){
                char *temp;
                //Not using atoi() since it is considered dangerous
                indexes[indexesCount++] = strtol(token, &temp, 10);
            }
            int res = tag(buffer, indexes, tags);

            if (res == -117)
                printf("Something is wrong, add returned -117\n");
        }
        else if (strcmp(token, FIND_COMMAND) == 0)
        {
            int res = add(buffer);
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
int add(char *linePtr) {
    char buffer[MAX_LINE_LENGHT];
    struct note_struct *note = malloc(sizeof(struct note_struct));
    int at_line = 0;
    note->note_line_count = 0;
    char *temp; //Temp is useless, just for strtol

    note->id = strtol(linePtr, &temp, 10);
    while(fgets(buffer, MAX_LINE_LENGHT-1, stdin)){
        if( buffer[strlen(buffer) -1]  == '\n') 
            buffer[strlen(buffer) -1]  = '\0';
        if(strcmp(buffer, ADD_TERMINATE_WORD) == 0)
            break; //This loop is done
        printf("->%s<-\n", buffer);
        strcpy(note->note_lines[note->note_line_count++], buffer);
    }
    notes[notes_length++] = note;

    return 0;
}

int tag(char *tagName, int *indexesToAdd, struct tag_struct tags[]) {

}

int display(int id) {
    printf("Displaying\n");
    for(int i = 0; i < notes_length; i++){
        struct note_struct *n = notes[i];
        if(n->id != NULL && n->id == id){
            printf("Displaying note with ID : %d\n", n->id);
            for(int j = 0; j<n->note_line_count; j++){
                printf("\t>%s\n", n->note_lines[j]);
            }
        }else{
            printf("GOD IS MY WITNESS IT GOT THE ELSE\n");
        }
    }
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
int find(char *linePtr, int *indexes) {}
int and_helper(char *linePtr, int *indexes) {}
int or_helper(char *linePtr, int *indexes) {}
int not_helper(char *linePtr, int *indexes) {}
