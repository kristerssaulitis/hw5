#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define MAXSIZE 1024
#define MAX_LENGTH 256

char* chunks;
char* sizes;
int* buffer;
char key[256];

char* shared_memory = NULL;
int* client_count = NULL;
int* shared_data = NULL;

int chechInput(int argc, char** argv){
    if (argc < 2){
        printf("not enough arguments \n");
        return -1;
    }

    int i = 0;
    if (strncmp("-c=", argv[1],3) < 0){
        printf("wrong parameter HOST");
        return -1;
    }
    if (strncmp("-s=", argv[2],3) < 0){
        printf("wrong parameter PORT %s \n", argv[2]);
        return -1;
    }

    char *randomchunks = argv[1];
    char *ranodmsizes = argv[2];

    for (i =0; i< 2; i++) chunks = strsep(&randomchunks,"=");
    for (i =0; i< 2; i++) sizes = strsep(&ranodmsizes,"=");

    printf("this is chunk :%s and this is size:%s      \n", chunks, sizes);
    return 0;
}

int readChunks(FILE *f, char *key){
    if (!f) return -1;

    while (fgets(key, MAX_LENGTH, f)){
        printf("%s", key);
    }
    /* Close file */
    fclose(f);
}

int createBuffer(int chunkcount){
    shared_memory = mmap(NULL, MAXSIZE-chunkcount, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    client_count = (int*) shared_memory;
    shared_data = (int*) (shared_memory + chunkcount);
}

int main(int argc, char** argv){
    buffer = malloc(MAXSIZE);

    chechInput(argc, argv);

    FILE *f = fopen(chunks, "r");
    readChunks(f, key);
    size_t n = sizeof(key) / sizeof(key[0]);
    createBuffer(n);

    return 0;
}