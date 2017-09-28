#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define TAM_BUFFER 64
static char answer[TAM_BUFFER];

void menu(){
	/*  
	 * Função que exibe o menu;
	*/

	printf("Escolha seu comando:\n");
	printf("c - Codificar\nd - Decodificar\nh - Calculo de Hash\n");
	fflush(stdin);
}

char* dataToParam(char choice, char data[]){
	/*
	 * Converte os dados pegos para uma instrução
	 * que o módulo consegue reconhecer.
	 * Estrutura da instrução: comando dados
	*/

	char string[TAM_BUFFER] = { 0 };
	
	strcat(string, choice);
	strcat(string, " ");
	strcat(string, data);
	
	return string;
}

int main(){
	
	int fp;
	char data[TAM_BUFFER - 2], param[TAM_BUFFER];
	char choice;
	
	// Abertura do device
	fp = open("/dev/crypto", O_RDWR);
	if(fp < 0){
		printf("Nao foi possivel abrir o device de criptografia.");
		return errno;
	}
	
	menu();
	scanf("%c", &choice);
	
	printf("Digite os dados que serão utilizados: ");
	scanf("%s", data);
	
	printf("Escolha de função: %c\nDados recebidos: %s\n", choice, data);
	
	param = dataToParam(choice, &data);
	
	// Escrita no device
	wr = write(fp, param, strlen(param));
	if(wr < 0){
		printf("Nao foi possivel escrever no device de criptografia.\n");
		return errno;
	}
	printf("Escrita realizada com sucesso :)\n");
	
	// Leitura do device
	rd = read(fp, answer, TAM_BUFFER);
	if(rd < 0){
		printf("Não foi possível ler o device de criptografia.\n");
		return errno;
	}
	printf("Dados recebidos: %s\n", answer);	
}
