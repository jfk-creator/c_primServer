#ifndef CALCTHREAD_H_
#define CALCTHREAD_H_
#include <stdint.h>
#include <inttypes.h>
struct routine_args_t {
    int socket; 
    uint64_t number;
};
uint64_t isPrime(uint64_t number){

    printf("starting with number: %" PRIu64 "\n", number);

    for(uint64_t i = 2; i < number / 2; i++){
        if(number % 2 == 0) {
            number++;
            i = 2;
        } 
        if(number % i == 0) {
            number++;
            i = 2;
        };
    }
    printf("result: %" PRIu64  "\n", number);
    return number;
}
void *routine(void *arg){
    struct routine_args_t routine_args = *(struct routine_args_t*) arg;    
    free(arg);
    printf("Client socket closed.\n");
    uint64_t prim = isPrime(routine_args.number); 
    char buffer[255];
    sprintf(buffer,"%" PRIu64 "\n", prim);
    send(routine_args.socket, buffer, strlen(buffer), 0);
    close(routine_args.socket);
    return NULL;
}

#endif
