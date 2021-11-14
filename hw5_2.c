#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

/*constants*/
#define MAXSIZE 1024
#define MAX_LENGTH 256

/*for file reading*/
char* chunkFileName;
char* sizeFileName;
char key[256];

/*buffer memory pointers*/
struct MLLnode* shared_memory = NULL;
void* memory_chunk = NULL;

/*functions*/
int createBuffer(chunk_total);
int initialise_chunks(FILE *f, char *key);
int chechInput(int argc, char** argv);
int get_chunk_total(FILE *f, char *key);

/*structure for bookeeping linked list*/
typedef struct MLLnode{
    int size;
    /*struct MLLnode* next;*/ /*we will use struct but it will not be linked list just array of structs*/
    void* memory;
} MLLnode;


/*left this function unchanged*/
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



int get_chunk_total(FILE *f, char *key){
    if (!f) return -1;

    int retval = 0;
    int count = 0;
    while (fgets(key, MAX_LENGTH, f)){
        /*
        //buffer[i] = atoi(key);
        //printf("buffer:      %i \n", buffer[i]);
        */
        /*summs up the total memory of chunks*/
        count++;
        retval += atoi(key); 
    }
    /*we could put this into loop so it checks after every chunk then we can stop when chuunk size excedes 1024 with bookeeping*/
    if (count*16 + retval > 1024){
        printf("chunk total too big");
        return -1;
    }

    /* Close file */
    fclose(f);
    return retval;
}


int initialise_chunks(FILE *f, char *key){
    if (!f){printf("feiled to open file\n") ;return -1;}

    int i = 0;
    int sum = 0;
/*
    struct MLLnode* test;
    struct MLLnode test2;
*/
    while (fgets(key, MAX_LENGTH, f)){
        /*atoi(key); */
        
        struct MLLnode newnode = shared_memory[i];
        shared_memory[i].size = atoi(key);
        shared_memory[i].memory = memory_chunk + sum;

        /*print to check if shared_memory[i] holds real struct - A - it does to check uncoment tests and print*/
        /*printf("this is nodes size %d and size of struct: %d in comperison to pointer %d and struct in stack %d\n", shared_memory[i].size, (int)sizeof(shared_memory[i]), (int)sizeof(test), (int)sizeof(test2));*/

        /*cpmment to test pointers to memory - all good*/
        printf("here are the pointers: %d end of free data is here %d\n", shared_memory[i].memory, (void*)shared_memory + MAXSIZE);

        sum += shared_memory[i].size;
        i++;
    }
    /* Close file */
    fclose(f);
    return i;
}


int createBuffer(chunk_total){
    /*printf("chunkCount %i \n", chunkCount);*/
    if(shared_memory = mmap(NULL, MAXSIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0)){
        memory_chunk = (void*)shared_memory;
        memory_chunk = (memory_chunk + MAXSIZE - 1 - chunk_total);
        printf("succesfully created buffer\n");
        return 0;
    }else{
        printf("could not mmap allocate MAXSIZE\n");
        return -1;
    }


    /*error code if we want to check for mmap succuess*/
}

int main(int argc, char** argv){

    chechInput(argc, argv);
    FILE *f = fopen(chunkFileName, "r");
    int total = get_chunk_total(f, key);

    if (total < 0){
        printf("invalid chunk size total; over 1024");
    }

    printf("this is total chunk size: %d \n", total);
    /*creating buffer*/
    if(!createBuffer(total)){
        
        FILE *f = fopen(chunkFileName, "r");
        int ch_num = initialise_chunks(f, key);
        printf("%d\n", ch_num);
        

        
    }else{
        printf("mmap feiled");
        return -1;
    }


    /* can use this to initialise LL struct with pointers*/
    /*readChunks(f, key);*/

    

    return 0;
}