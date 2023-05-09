#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"
#include "dir_file.h"

directory_t* create_dir(char* name) {
    directory_t *dir;

    dir = (directory_t*)malloc(sizeof(directory_t));

    memcpy(dir->name, name, MAX_NAME_SIZE);

    dir->num_files = 0;
    dir->num_dirs = 0;

    return dir;

}

directory_t* find_dir(directory_t* dir, char* name) {
    assert(dir != NULL);
    assert(name != NULL);
    assert(strlen(name) > 0);

    for (int i = 0; i < dir->num_dirs; i++) {
        if (strcmp(dir->dir_list[i]->name, name) == 0)
            return dir->dir_list[i];
    }

    return NULL;
}

// This find_create_dir() find directory_t matched with name variable, in parent_dir.
// Otherwise, if this directory not exists, creates directory.
directory_t* find_create_dir(directory_t* parent_dir, char* name, bool* is_create) {
    directory_t *dir;

    assert(parent_dir != NULL);

    dir = find_dir(parent_dir, name);

    if (dir == NULL) {
        dir = create_dir(name);
        *is_create = true;
    }

    return dir;
}

file_t* create_file(char* name) {
    file_t *file;

    file = (file_t*)malloc(sizeof(file_t));

    memcpy(file->name, name, MAX_NAME_SIZE);

    return file;
}

file_t* find_file(directory_t* dir, char* name) {
    assert(dir != NULL);
    assert(name != NULL);
    assert(strlen(name) > 0);

    for (int i = 0; i < dir->num_files; i++) {
        if (strcmp(dir->file_list[i]->name, name) == 0)
            return dir->file_list[i];
    }

    return NULL;
}

// This find_create_file() find file matched with name variable, in dir.
// Otherwise, if this file not exists, create the file.
file_t* find_create_file(directory_t* dir, char* name, bool* is_create) {
    file_t *file;

    assert(dir != NULL);

    file = find_file(dir, name);

    if (file == NULL) {
        file = create_file(name);
        *is_create = true;
    }

    return file;
}

// This function creates the hierarchy of files and directories
// which are indicated by token_list. Everything starts in root_dir.
// You can implement this function using the above find_create_dir() 
// and find_create_file() functions.
void make_dir_and_file(directory_t* root_dir, char** token_list, int num_tokens) {
    /* Fill this function */
    directory_t *current_dir=root_dir;
    directory_t *child_dir;
    file_t *file;
    int index=0;
    static int i=0;
    bool is_create;
    //printf("--------\n");
    while(index<num_tokens-1){  //file 나오기 전까지 반복문 동작
        //printf("\nworking(while)\n");
        is_create=false;
        child_dir = find_create_dir(current_dir, token_list[index], &is_create);
        //printf("\nworking(find_create_dir)\n");
        //printf("child_dir=%s\n", child_dir->name); 
        //printf("file created?: %d\n", is_create); 
        //printf("current_dir name: %s\n", current_dir->name);
        
        if(is_create){ //root_dir안에 dir_list안에 token_list[index]이름을 가진 폴더구조체생성
            //처음에는 root_dir안에 dir_list안에 ex)home이라는 directory_t 생성
            //printf("\nc new dir: %s\n", child_dir->name);
            //printf("-------------------------------------------");
            //printf("현재dir의 num_dirs: %d\n", current_dir->num_dirs);
            current_dir->dir_list[current_dir->num_dirs]=child_dir;
            (current_dir->num_dirs)++;
            //printf("is_create working(dir)\n");
        }

        current_dir=child_dir;
        //printf("next current_dir name: %s\n", current_dir->name);
        index++;
        //printf("index: %d\n\n", index);
    }

    file=find_create_file(current_dir, token_list[index], &is_create);
    if(is_create){
        current_dir->file_list[current_dir->num_files]=file;
        //printf("\nfile name: %s", current_dir->file_list[current_dir->num_files]);
        (current_dir->num_files)++;
    }
    
    //printf("\nnum_files?: %d\n", current_dir->num_files);
    //printf("-------------------------------------------");
    i++;
}

void free_dir_and_file(directory_t* dir) {
    for (int i = 0; i < dir->num_files; i++)
        free(dir->file_list[i]);

    for (int i = 0; i < dir->num_dirs; i++)
        free_dir_and_file(dir->dir_list[i]);

    free(dir);
}

// This find_target_dir() find the directoy which is indicated as full path by token_list
// and return directory_t* entry of the found directory or NULL when such directory not exists.
directory_t* find_target_dir(directory_t* root_dir, char** token_list, int num_tokens) {
    directory_t *current_dir, *child_dir;
    char *token;
    char path[MAX_BUFFER_SIZE];

    memset(path, 0, MAX_BUFFER_SIZE);

    current_dir = root_dir;

    for (int i = 0; i < num_tokens; i++) {
        strcat(path, "/");
        token = token_list[i];
        child_dir = find_dir(current_dir, token);

        if (child_dir == NULL) {
            fprintf(stderr, "Not found '%s' directory in %s\n", token, path);
            return NULL;
        }

        current_dir = child_dir;
        strcat(path, token);
        child_dir = NULL;
    }

    return current_dir;
}

void print_files_on_dir(directory_t* dir) {
    for (int i = 0; i < dir->num_files; i++)
        printf("%s\n", dir->file_list[i]->name);
}
