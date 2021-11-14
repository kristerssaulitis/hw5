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
int indexNext = 0;

/*buffer memory pointers*/
struct MLLnode* shared_memory = NULL;
void* memory_chunk = NULL;

/*functions*/
int createBuffer(int chunk_total);
int initialise_chunks(FILE *f, char *key);
int chechInput(int argc, char** argv);
int get_chunk_total(FILE *f, char *key);
void* find_free_chunks(FILE *f, int ch_num, int mode);

/*structure for bookeeping linked list*/
typedef struct MLLnode{
    int size;
    /*struct MLLnode* next;*/ /*we will use struct but it will not be linked list just array of structs*/
    void* memory;
} MLLnode;

void* first_fit(int siz, int ch_num){
    int num = ch_num;
    int i = 0;
    for (i = 0; i < num; i++){
        int ret_size = 0;
        printf("sizes:     %i \n", siz);
        if (shared_memory[i].size >= siz){
            ret_size = shared_memory[i].size - siz;
            /*
            printf("return sizes :     %i \n", ret_size);
            printf("shared memory sizes :     %i \n", shared_memory[i].memory);
            printf("shared memory + 1 sizes :     %i \n", shared_memory[i+1].memory);
            */
            if (i!= num-1){
                shared_memory[i+1].size += ret_size;
                shared_memory[i+1].memory -= ret_size;
                int j = i;
                for (j; j < num; j++){
                    shared_memory[j] = shared_memory[j+1];
                }
                printf("chunum %i \n", num);
                num--;
            } else {
                printf("chukity chunk %i + shared mamuti %i \n ", siz, shared_memory[i].size);
                /*last element stays the last element bet with a smaller size*/
                shared_memory[i].size = ret_size;
                shared_memory[i].memory += siz;
            }
            /*printf("shared memory + 1 sizes :     %i \n", shared_memory[i+1].memory);*/
            break;
        }
    }
    return shared_memory[i].memory;
}

void* next_fit(int siz, int ch_num){
    int num = ch_num;
    int i = 0;
    int count = 0;
    for (i = indexNext; i < num; i++){
        int ret_size = 0;
        printf("sizes:     %i \n", siz);
        if (shared_memory[i].size >= siz){
            ret_size = shared_memory[i].size - siz;
            if (i!= num-1){
                shared_memory[i+1].size += ret_size;
                shared_memory[i+1].memory -= ret_size;
                int j = i;
                for (j; j < num; j++){
                    shared_memory[j] = shared_memory[j+1];
                }
                printf("chunum %i \n", num);
                num--;
            } else {
                /*last element stays the last element bet with a smaller size*/
                shared_memory[i].size = ret_size;
                shared_memory[i].memory += siz;
            }
            /*printf("shared memory + 1 sizes :     %i \n", shared_memory[i+1].memory);*/
            break;
        } else {
            if (i < num){
                i = indexNext +1;
            } else{
                i = 0;
                indexNext = 0;
            }

        }
    }
    return shared_memory[i].memory;
}

void* last_fit(int siz, int ch_num){
    int num = ch_num;
    int i = 0;
    for (i = 0; i < num; i++){
        int ret_size = 0;
        printf("sizes:     %i \n", siz);
        if (shared_memory[-1-i].size >= siz){
            ret_size = shared_memory[-1-i].size - siz;
            if (i!= num-1){
                shared_memory[i+1].size += ret_size;
                shared_memory[i+1].memory -= ret_size;
                int j = i;
                for (j; j < num; j++){
                    shared_memory[j] = shared_memory[j+1];
                }
                printf("chunum %i \n", num);
                num--;
            } else {
                printf("chukity chunk %i + shared mamuti %i \n ", siz, shared_memory[i].size);
                /*last element stays the last element bet with a smaller size*/
                shared_memory[i].size = ret_size;
                shared_memory[i].memory += siz;
            }
            break;
        }
    }
    return shared_memory[i].memory;
}

void* find_free_chunks(FILE *f, int ch_num, int mode){
    if (!f){printf("feiled to open file\n") ;return -1;}
    int i = 0;

    while (fgets(key, MAX_LENGTH, f)){
        int siz = atoi(key);
        printf("!!!! SIZ is %d\n", siz);
        if (mode == 0){
            first_fit(siz, ch_num);
        } else if (mode == 1){
            next_fit(siz, ch_num);
<<<<<<< HEAD
        } else if (mode == 2){
            best_fit(siz, ch_num);
=======
        } if (mode == 2){
            last_fit(siz, ch_num);
>>>>>>> 0508a676e4407e7a28018a472f63920829a5f09a
        }
    }
    fclose(f);


}

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
        FILE *fd = fopen(sizeFileName, "r");
        find_free_chunks(fd, ch_num, 2);


    }else{
        printf("mmap feiled");
        return -1;
    }


    /* can use this to initialise LL struct with pointers*/
    /*readChunks(f, key);*/



    return 0;
}