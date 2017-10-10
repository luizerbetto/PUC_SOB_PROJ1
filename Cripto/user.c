/*
#Luiz Felipe Zerbetto Masson RA: 15166804
#José Carlos Clausen Neto RA: 15055825
#Danilo Luis Lopes Raymundo Paixão RA: 15051659
#Yessica Melaine Castillo RA: 13054895
#Janaina Mendes Mano Sanches RA: 07270085
#Raissa Furlan Davinho RA: 15032006

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
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
	char op = 's';
	param[0] = '\0';
	
	// Abertura do device
	fp = open("/dev/crypto", O_RDWR);
	if(fp < 0){
		printf("Nao foi possivel abrir o device de criptografia.");
		return errno;
	}
	
	do{	
	
	menu();
	scanf("%c", choice);
	
	printf("\nDigite os dados que serão utilizados: ");
	scanf("%s", data);
	
	printf("\nEscolha de função: %s\nDados recebidos: %s\n", choice, data);
	
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
	printf("\nCodificacao em andamento :)\n");
	getchar();
	printf("Pressione ENTER");
	getchar();
	// Leitura do device
	rd = read(fp, answer, TAM_BUFFER);
	if(rd < 0){
		printf("Não foi possível ler o device de criptografia.\n");
		return errno;
	}
	printf("\nDados recebidos: %s\n", answer);
	
	printf("Decriptacao esta comecando!\n");

	
	strcpy(param, "");	


	strcat(param, "d");
	strcat(param, " ");
	strcat(param, answer);
	
	// Escrita no device
	wr = write(fp, param, strlen(param));
	if(wr < 0){
		printf("Nao foi possivel escrever no device de criptografia.\n");
		return errno;
	}
	printf("\nDecodificacao em andamento :)\n");
	printf("Pressione ENTER");
	getchar();
	// Leitura do device
	rd = read(fp, answer, TAM_BUFFER);
	if(rd < 0){
		printf("Não foi possível ler o device de criptografia.\n");
		return errno;
	}
	printf("\nDados recebidos: %s\n", answer);
	
	printf("Deseja fazer mais alguma operação (s/n)?  ");
	getchar();
	scanf("%c", &op);

	}while(op == 's');	
}
