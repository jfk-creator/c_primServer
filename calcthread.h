#ifndef CALCTHREAD_H_
#define CALCTHREAD_H_
struct routine_args_t {
    int socket; 
    int number;
};
int isPrime(int number){

    printf("starting with number: %d\n", number);

    for(int i = 2; i < number / 2; i++){
        if(number % 2 == 0) {
            number++;
            i = 2;
        } 
        if(number % i == 0) {
            number++;
            i = 2;
        };
    }
    printf("result: %d\n", number);
    return number;
}
void *routine(void *arg){
    struct routine_args_t routine_args = *(struct routine_args_t*) arg;    
    free(arg);
    printf("Thread started with %d\n", routine_args.socket);
    printf("Client socket closed.\n");
    int prim = isPrime(routine_args.number); 
    char buffer[255];
    sprintf(buffer, "%d\n", prim);
    send(routine_args.socket, buffer, strlen(buffer), 0);
    close(routine_args.socket);
    return NULL;
}

#endif
