#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include<fcntl.h>
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

/*void dataToParam(char choice, char data[], char *param){
	/*
	 * Converte os dados pegos para uma instrução
	 * que o módulo consegue reconhecer.
	 * Estrutura da instrução: comando dados
	*
	
	strcat(param, choice);
	strcat(param, " ");
	strcat(param, data);
	
}*/

int main(){
	
	int fp, wr, rd;
	char data[TAM_BUFFER - 2], param[TAM_BUFFER+5];
	char choice[3];
	param[0] = '\0';
	
	// Abertura do device
	fp = open("/dev/crypto", O_RDWR);
	if(fp < 0){
		printf("Nao foi possivel abrir o device de criptografia.");
		return errno;
	}
	
	menu();
	scanf("%c", choice);
	
	printf("Digite os dados que serão utilizados: ");
	scanf("%s", data);
	
	printf("Escolha de função: %s\nDados recebidos: %s\n", choice, data);
	
	//dataToParam(choice, &data, param);
	strcat(param, choice);
	strcat(param, " ");
	strcat(param, data);
	
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
