#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

/*constants*/
#define MAXSIZE 1024
#define MAX_LENGTH 256
#define MODE 4  /*can be changed from 0 to 4 | 0 FirstF 1 nextF 2 lastF(THIS our own for extra credit) 3 bestF 4 WorstF*/

/*for file reading*/
char* chunkFileName;
char* sizeFileName;
char key[256];
int indexNext = 0;

/*buffer memory pointers*/
struct MLLnode* shared_memory = NULL;
void* memory_chunk = NULL;

/*functions*/
int ch_num;

int createBuffer(int chunk_total);
int initialise_chunks(FILE *f, char *key);
int chechInput(int argc, char** argv);
int get_chunk_total(FILE *f, char *key);
void* find_free_chunks(FILE *f, int mode);

/*structure for bookeeping linked list*/
typedef struct MLLnode{
    int size;
    /*struct MLLnode* next;*/ /*we will use struct but it will not be linked list just array of structs*/
    void* memory;
} MLLnode;

void* first_fit(int siz){
    int i = 0;
    void* return_pointer = NULL;
    for (i = 0; i < ch_num; i++){
        int ret_size = 0;
        /*printf("sizes:     %i \n", siz);*/
        if (shared_memory[i].size >= siz){
            ret_size = shared_memory[i].size - siz;
            /*
            printf("return sizes :     %i \n", ret_size);
            printf("shared memory sizes :     %i \n", shared_memory[i].memory);
            printf("shared memory + 1 sizes :     %i \n", shared_memory[i+1].memory);
            */
            if (i!= ch_num-1){
                return_pointer = shared_memory[i].memory;
                shared_memory[i+1].size += ret_size;
                shared_memory[i+1].memory -= ret_size;
                int j = i;
                for (j; j < ch_num; j++){
                    shared_memory[j] = shared_memory[j+1];
                }
                /*printf("chunum %i \n", ch_num);*/
                ch_num--;
            } else {
                /*printf("chukity chunk %i + shared mamuti %i \n ", siz, shared_memory[i].size);*/
                /*last element stays the last element bet with a smaller size*/
                return_pointer = shared_memory[i].memory;
                shared_memory[i].size = ret_size;
                shared_memory[i].memory += siz;
            }
            /*printf("shared memory + 1 sizes :     %i \n", shared_memory[i+1].memory);*/
            break;
        }
    }
    return return_pointer;
}

void* next_fit(int siz){
    int i = 0;
    int count = 0;
    void* return_pointer = NULL;
    for (i = indexNext; i < ch_num; i++){
        int ret_size = 0;
        /*printf("sizes:     %i \n", siz);*/
        if (shared_memory[i].size >= siz){
            ret_size = shared_memory[i].size - siz;
            if (i!= ch_num-1){
                return_pointer = shared_memory[i].memory;
                shared_memory[i+1].size += ret_size;
                shared_memory[i+1].memory -= ret_size;
                int j = i;
                for (j; j < ch_num; j++){
                    shared_memory[j] = shared_memory[j+1];
                }
                /*printf("chunum %i \n", ch_num);*/
                ch_num--;
            } else {
                /*last element stays the last element bet with a smaller size*/
                return_pointer = shared_memory[i].memory;
                shared_memory[i].size = ret_size;
                shared_memory[i].memory += siz;
            }
            /*printf("shared memory + 1 sizes :     %i \n", shared_memory[i+1].memory);*/
            break;
        } else {
            if (i < ch_num){
                i = indexNext +1;
            } else{
                i = 0;
                indexNext = 0;
            }

        }
    }
    return return_pointer;
}

void* last_fit(int siz){
    int i = 1;
    for (i; i < ch_num + 1; i++){
        int ret_size = 0;
        /*printf("sizes:     %i \n", siz);*/
        if (shared_memory[ch_num-i].size >= siz){
            /*printf("WE HERE BITCH ch num %d \n", ch_num);*/
            ret_size = shared_memory[ch_num-i].size - siz;
            if (i!= ch_num-1){
                shared_memory[i+1].size += ret_size;
                shared_memory[i+1].memory -= ret_size;
                int j = i;
                for (j; j < ch_num; j++){
                    shared_memory[j] = shared_memory[j+1];
                }
                /*printf("chunum %i \n", ch_num);*/
                ch_num--;
                /*printf("this is the num after we find the right place %d", ch_num);*/
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

void* best_fit(int siz){
    int i = 0;
    void* return_pointer = NULL;
    int best_s = 10000;
    int best = -1;
    for (i = 0; i < ch_num; i++){
        if(shared_memory[i].size >= siz){
            if(best_s > shared_memory[i].size - siz){
                best_s = shared_memory[i].size - siz;
                best = i;
                printf("best_s with atlikums %d at chunk %d\n", best_s, best);
            }
        }

    }
        if(best < 0){return NULL;}

        int ret_size = 0;
        /*printf("sizes:     %i \n", siz);*/

            ret_size = shared_memory[best].size - siz;
            /*
            printf("return sizes :     %i \n", ret_size);
            printf("shared memory sizes :     %i \n", shared_memory[i].memory);
            printf("shared memory + 1 sizes :     %i \n", shared_memory[i+1].memory);
            */
            if (best != ch_num-1){
                return_pointer = shared_memory[best].memory;
                shared_memory[best+1].size += ret_size;
                shared_memory[best+1].memory -= ret_size;
                int j = best;
                for (j; j < ch_num; j++){
                    shared_memory[j] = shared_memory[j+1];
                }
                /*printf("chunum %i \n", ch_num);*/
                ch_num--;
            } else {
                /*printf("chukity chunk %i + shared mamuti %i \n ", siz, shared_memory[best].size);*/
                /*last element stays the last element bet with a smaller size*/
                return_pointer = shared_memory[best].memory;
                shared_memory[best].size = ret_size;
                shared_memory[best].memory += siz;
            }
            /*printf("shared memory + 1 sizes :     %i \n", shared_memory[i+1].memory);*/
    return return_pointer;

}

void* worst_fit(int siz){
    int i = 0;
    void* return_pointer = NULL;
    int best_s = -1;
    int best = -1;
    for (i = 0; i < ch_num; i++){
        if(shared_memory[i].size >= siz){
            if(best_s < shared_memory[i].size - siz){
                best_s = shared_memory[i].size - siz;
                best = i;
                printf("best_s with atlikums %d at chunk %d\n", best_s, best);
            }
        }

    }

    if(best < 0){return NULL;}

        int ret_size = 0;
        /*printf("sizes:     %i \n", siz);*/

            ret_size = shared_memory[best].size - siz;
            /*
            printf("return sizes :     %i \n", ret_size);
            printf("shared memory sizes :     %i \n", shared_memory[i].memory);
            printf("shared memory + 1 sizes :     %i \n", shared_memory[i+1].memory);
            */
            if (best != ch_num-1){
                return_pointer = shared_memory[best].memory;
                shared_memory[best+1].size += ret_size;
                shared_memory[best+1].memory -= ret_size;
                int j = best;
                for (j; j < ch_num; j++){
                    shared_memory[j] = shared_memory[j+1];
                }
                /*printf("chunum %i \n", ch_num);*/
                ch_num--;
            } else {
                /*printf("chukity chunk %i + shared mamuti %i \n ", siz, shared_memory[best].size);*/
                /*last element stays the last element bet with a smaller size*/
                return_pointer = shared_memory[best].memory;
                shared_memory[best].size = ret_size;
                shared_memory[best].memory += siz;
            }
            /*printf("shared memory + 1 sizes :     %i \n", shared_memory[i+1].memory);*/
    return return_pointer;

}

void* find_free_chunks(FILE *f, int mode){
    if (!f){printf("feiled to open file\n") ;return -1;}
    int i = 0;

    while (fgets(key, MAX_LENGTH, f)){
        int siz = atoi(key);
        printf("allocating size block of size %d\n", siz);
        /*printf("!!!! SIZ is %d\n", siz);*/
        void* ret_pointer;
        if (mode == 0){
            ret_pointer = first_fit(siz);
            printf("memory block of size %d is available at %x\n", siz, ret_pointer);
        } else if (mode == 1){
            ret_pointer = next_fit(siz);
            printf("memory block of size %d is available at %x\n", siz, ret_pointer);
        } else if (mode == 2){
            ret_pointer = last_fit(siz);
            printf("memory block of size %d is available at %x\n", siz, ret_pointer);
        } else if (mode == 3){
            ret_pointer = best_fit(siz);
            printf("memory block of size %d is available at %x\n", siz, ret_pointer);
        } else if (mode == 4){
            ret_pointer = worst_fit(siz);
            printf("memory block of size %d is available at %x\n", siz, ret_pointer);
        } else{
            printf("wrong mode index select 0 - 4\n");
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
        printf("chunk total is too big\n");
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
        /*printf("here are the pointers: %d end of free data is here %d\n", shared_memory[i].memory, (void*)shared_memory + MAXSIZE);*/

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
        printf("invalid chunk size total; over 1024\n");
    }

    printf("this is total chunk size: %d \n", total);
    /*creating buffer*/
    if(!createBuffer(total)){

        FILE *f = fopen(chunkFileName, "r");
        ch_num = initialise_chunks(f, key);
        printf("%d\n", ch_num);
        FILE *fd = fopen(sizeFileName, "r");
        find_free_chunks(fd, MODE);


    }else{
        printf("mmap feiled\n");
        return -1;
    }


    /* can use this to initialise LL struct with pointers*/
    /*readChunks(f, key);*/



    return 0;
}