#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define IRQ0_SIGNAL SIGINT 
#define IRQ1_SIGNAL SIGUSR1 
#define IRQ2_SIGNAL SIGUSR2 

pid_t kernel_pid;

void kernelHandler(int signal) {
    if (signal == SIGHUP){
        printf("Kernel recebeu um sinal IRQ0 do controller...\n");
        irq0 = 1;
    } else if (signal == SIGUSR1){
        printf("Kernel recebeu um sinal IRQ1 do controller...\n");
        irq1 = 1;
    } else if (signal == SIGUSR2){
        printf("Kernel recebeu um sinal IRQ2 do controller...\n");
        irq2 = 1;
    }
}

void send_interrupt(int signal) {
    if (kill(kernel_pid, signal) == -1) {
        perror("Erro ao enviar sinal ao KernelSim");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <PID do KernelSim>\n", argv[0]);
        printf("KernelSim PID: %d\n", getpid());
        exit(EXIT_FAILURE);
    }

    kernel_pid = atoi(argv[1]);

    srand(time(NULL)); 

    while (1) {
        
        usleep(500000); 

        send_interrupt(IRQ0_SIGNAL);
        printf("InterController: Enviou IRQ0 (Time Slice)\n");

        double rand_prob = (double)rand() / RAND_MAX;

        if (rand_prob < 0.1) {
            send_interrupt(IRQ1_SIGNAL);
            printf("InterController: Enviou IRQ1 (Dispositivo D1)\n");
        }

        rand_prob = (double)rand() / RAND_MAX;

        if (rand_prob < 0.05) {
            send_interrupt(IRQ2_SIGNAL);
            printf("InterController: Enviou IRQ2 (Dispositivo D2)\n");
        }
    }

    
    return 0;
}

