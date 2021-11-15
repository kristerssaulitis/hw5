void* best_fit(int siz, int ch_num){
    int num = ch_num;
    int i;
    int best_size = 1000000;
    int best = NULL;
    int ret_size = 0;
    void* return_pointer;

    for (i = 0; i < num; i++){
        printf("sizes:     %i \n", siz);
        printf("shmem is %d and siz is %i\n", shared_memory[i], siz);
        if (shared_memory[i].size >= (int)siz){
            
            if(best_size < (shared_memory[i].size - siz)){
                best_size = shared_memory[i].size -siz;
                best = i;
                printf("shmem size is %d\n", shared_memory[i]);
            }
        }

    }

        ret_size = shared_memory[best].size - siz;
        /*
        printf("return sizes :     %i \n", ret_size);
        printf("shared memory sizes :     %i \n", shared_memory[i].memory);
        printf("shared memory + 1 sizes :     %i \n", shared_memory[i+1].memory);
        */
            printf("num is %d but best is %d\n", num, best);
            if (best != num-1){
                return_pointer = shared_memory[best].memory;
                shared_memory[best+1].size += ret_size;
                shared_memory[best+1].memory -= ret_size;
                int j = best;
                for (j; j < num; j++){
                    shared_memory[j] = shared_memory[j+1];
                }
                printf("chunum %i and nex block now is %d big\n", num, shared_memory[best].size);
                num--;
            } else {
                printf("chukity chunk %i + shared mamuti %i \n ", siz, shared_memory[i].size);
                /*last element stays the last element bet with a smaller size*/
                return_pointer = shared_memory[best].memory;
                shared_memory[best].size = ret_size;
                shared_memory[best].memory += siz;
            }
            /*printf("shared memory + 1 sizes :     %i \n", shared_memory[i+1].memory);*/
        
    return return_pointer;
}