#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define maxSize 60000000

int size;
int *buffer;

pthread_mutex_t mutex;
 // As variáveis que irão passar na função das threads//
typedef struct {
    int numero;
    int thread;
    int totalThreads;
}args;						

// Assinaturas das funções//

// A que vai multiplicar cada numero do fatorial pelo produto, algarismo por algarismo
void multiply(int fatNum);

void* threadCalc(void* args);

// A que vai calcular o fatorial de um numero passado pelo parâmetro
void factorial(int numero, pthread_t *thread, int numT);

// As Implementações

//Vai multiplicar os valores de 2 até o valor de numero, pelos algarismos do buffer
void* threadCalc(void* param){
    args *info = (args*) param; 
    int start = 1 + (info->thread*1000);		// Aqui define o valor inicial do loop a partir da thread
    int end = (info->thread + 1)*1000;			// Aqui define o valor final do loop  a partir da thread
    if(info->numero%1000 != 0 && (info->thread == info->totalThreads - 1))
        end = start - 1 + info->numero%1000;
    
    printf("inicializando thread %d | calculo de %d a %d\n", info->thread, start, end);
    
    pthread_mutex_lock(&mutex);					//É o que impede as outras threads de entrarem na regiao critica
    for(int i = start; i <= end; i++){
        multiply(i);							
    }
    pthread_mutex_unlock(&mutex);				// Libera o acesso à regiao crítica
}

void factorial(int numero, pthread_t *thread, int numT){
    buffer = (int*)malloc(maxSize*sizeof(int));             // Vai definir o tamanho do buffer
    size = 1;                                               // Vai inicializar o tamanho do buffer
    buffer[0] = 1;                                          // Vai inicializar o valor do fatorial com 1

    int i;      //define um contador

    pthread_mutex_init(&mutex, NULL);

    args parametros[numT];

    for(int i = 0; i < numT; i++){
    	printf("Thread %d\n",i);
        parametros[i].thread = i;
        parametros[i].numero = numero;
    	parametros[i].totalThreads = numT;
        pthread_create(&thread[i], NULL, threadCalc,(void*) &parametros[i]);
    }

    for (int i = 0; i < numT; i++){
        pthread_join(thread[i], NULL);
    }

    // Vai exibir o valor final do fatorial na tela
    printf("O fatorial de %d e igual a :\n", numero);
    for(i = size - 1; i >= 0; i--){
        printf("%d", buffer[i]);
    }
    printf("\n\n");   
    free(buffer);
}

void multiply(int fatNum){
    int carry = 0;      // Número carregado para outras casas do número final

    // Vai multiplicar o número passado, algarismo por algarismo
    for(int i = 0; i < size; i++){

        //  Vai multiplicar o nunmro do buffer[i] pelo valor passado, e somar com o carry
        int prod = buffer[i]*fatNum + carry;

        // Vai trocar o valor do buffer[i] pelo resto da divisao do produto anterior por 10
        buffer[i] = prod%10;

        // Vai definir o valor do carry como o quociente da divisao inteira de prod por 10
        carry = prod/10;
    }

    while(carry){					// Se ainda ainda existir algum valor em carry,ele será distribuído no produto final do vetor
        buffer[size] = carry%10;
        carry /= 10;
        size++;
    }
}

int main(){
    int fat;

    printf("Fatorial de : ");
    scanf("%d", &fat);              // Vai escrever na variavel "fat" o valor de fatorial que deve ser calculado

    if(fat < 0) return 0;           // Chama o fim do programa caso seja dado um valor invalido para calcular

    int numThreads = fat/1000;      // O que define o numero de threads que vão ser usadas
    if(fat%1000 != 0) numThreads++;
    printf("Threads a serem executadas : %d\n",numThreads);

    pthread_t thread[numThreads];

    factorial(fat, thread, numThreads);

    printf("O fatorial de %d possui %d algarismos\n", fat, size); // Vai dar o resutado final do programa
    return 0;
}
