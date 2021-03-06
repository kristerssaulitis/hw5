#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define MAXSIZE 1024
#define MAX_LENGTH 256
#define SIZEOF(x) (sizeof(x) / sizeof((x)[0]))

char* chunkFileName;
char* sizeFileName;
int* buffer;
char key[256];

char* shared_memory = NULL;
int* chunkTotal = NULL;
int* chunkBuffer = NULL;
int* chunkPointer = NULL;
int* shared_data = NULL;

size_t chunkCount = 0;

int createBuffer();
int readChunks(FILE *f, char *key);
int chechInput(int argc, char** argv);

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

    for (i =0; i< 2; i++) chunkFileName = strsep(&randomchunks,"=");
    for (i =0; i< 2; i++) sizeFileName = strsep(&ranodmsizes,"=");

    /*printf("this is chunk :%s and this is size:%s      \n", chunkFileName, sizeFileName);*/
    return 0;
}

int readChunks(FILE *f, char *key){
    if (!f) return -1;
    int i = 0;
    while (fgets(key, MAX_LENGTH, f)){
        buffer[i] = atoi(key);
        printf("buffer:      %i \n", buffer[i]);
        i++;
    }
    chunkCount = i;

    /* Close file */
    fclose(f);
    return 0;
}

int createBuffer(){
    printf("chunkCount %i \n", chunkCount);
    /* the buffer is 1024, however, we have to take into account the bookkeping info
                        chunk count +      chunks                        chunk pointer
                    example:  3 chunks     |10|222|3|              |pointer to 10 in the shared data| pointer to 222 in the shared data .....
    | 0000 | 0000 0000 0000 | 00000000 00000000 0000000 | shared data | = 1024 bytes
    */
    int memoryBeforeShared = sizeof(int) + (chunkCount * sizeof(int)) + (chunkCount * sizeof(int*));
    shared_memory = mmap(NULL, MAXSIZE-memoryBeforeShared, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    chunkTotal = (int*) shared_memory;
    chunkBuffer = (int*) (shared_memory + sizeof(chunkTotal)); /*sizeof(chunkTotal) = 4*/
    chunkPointer = (int*) (shared_memory + sizeof(chunkTotal) + sizeof(chunkBuffer));
    shared_data = (int*) (shared_memory + sizeof(chunkTotal) + sizeof(chunkBuffer) + sizeof(chunkPointer));
    return 0;
}

int main(int argc, char** argv){
    buffer = malloc(MAXSIZE);
    printf("size of int pointer %lu \n", sizeof(char*)); /* all pointers are 8 bytes long */
    chechInput(argc, argv);

    FILE *f = fopen(chunkFileName, "r");
    readChunks(f, key);

    createBuffer();

    return 0;
}