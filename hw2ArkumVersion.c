

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct note{
    
    char lines[501][501];
    int lines_count;
    int note_id;

};

struct tag{
    
    int id_array[501];
    int id_array_length;
    char tag_name[500];

};

struct tag *tags[1234];
int all_tags_length = 0;

struct note *notes[1234];
int all_notes_length = 0;

int ADD(char *line){
    char *word = strtok(line, " "); //ADD
    word = strtok(NULL, " "); //id;
    int id = atoi(word);

    struct note *new_n = malloc(sizeof(struct note));
    new_n->note_id = id;
    bool is_continue = 1;
    while(is_continue){
        char buffer[1234];
        fgets(buffer, sizeof(buffer), stdin);
        char *pos = strstr(buffer, "\n");
        *pos = '\0';
        if(strcmp(buffer, "END") == 0){
            is_continue = 0;
            notes[all_notes_length++] = new_n;
            break;
        }else
        strcpy(new_n->lines[new_n->lines_count], buffer);
        new_n->lines_count++;
    }
}

int TAG(char *line){
    char *word = strtok(line, " "); //TAG
    word = strtok(NULL, " "); //tag_name;
    char tag_name[500];
    strcpy(tag_name, word);

    if( word[ strlen(word) -1 ] == "\n" )
        word[ strlen(word) -1 ] = '\0';
    
    char *one_index;
    int new_size = 0;
    int new_indexes[1234];
    do{
        one_index = strtok(NULL, " ");//an index

        int new_index = atoi(one_index);
        if(new_index == -1){
            break;
        }
        new_indexes[new_size++] = new_index;
    }while(strcmp(one_index, "-1") != 0);

    if(all_tags_length == 0){
        struct tag *new_t = malloc(sizeof(struct tag));
        strcpy(new_t->tag_name, tag_name);
        new_t->id_array_length = 0;
        tags[all_tags_length++] = new_t;
    }

    for (int i = 0; i < all_tags_length; i++)
    {
        if(strcmp(tags[i]->tag_name,tag_name) == 0){
            for (size_t j = 0; j < new_size; j++)
            {
                tags[i]->id_array[tags[i]->id_array_length++] = new_indexes[i];
            }
            
        }
    }
}

int FIND(char *ptr){
    int methodsStack[9999];
    int methodsStackLength = 0;

    int result[9999];
    char *miniptr;

    while(1){
        char line[9999];
        strcpy(line, ptr);
        char *line_ptr = strtok(line, " "); //It should be FIND
        if(*line_ptr == NULL || *line_ptr == '\n' || *line_ptr == '\0'){
            break;
        }else{
            line_ptr = strtok(NULL, " "); //Should be and,or,not
            if(strcmp("AND(", line_ptr)){

            }else if(strcmp("OR(", line_ptr)){
                
            }else if(strcmp("NOT(", line_ptr)){
                
            }
        }
        int level = 0;
    }
}

int main(){
    char buffer[9999];
    while(fgets(buffer, sizeof(buffer),stdin)){
        char temp_buffer[9999];
        char *temp_ptr = temp_buffer;
        strcpy(temp_buffer, buffer);
        char firstWord[999];
        strncpy(firstWord, temp_buffer, strstr(temp_buffer, " ") -  temp_buffer);
        if(strcmp(firstWord, "TAG") == 0){
            TAG(temp_buffer);
        }else if(strcmp(firstWord, "ADD") == 0){
            ADD(temp_buffer);
        }
    }
    FIND(buffer);
    return 0;
}