/*
 Para compilar deve incluir a flag “-lcrypto” .
Há a entrada de uma string (argv [1]) e gera um string hash através da função SHA1 (). O valor de "SHA_DIGEST_LENGTH" é de 20 bytes, essa é a dimensão da saída SHA1.
Esta função armazena o valor calculado na variável "calc".
Então, no loop for, eu copio o valor de "calc" para "transf", de acordo com o formato padrão das cordas SHA1.
*/
#include <stdio.h>
#include <string.h>
#include <openssl/crypto.h>
#define SHA_DIGEST_LENGTH 20
 
int main(int argn, char *argv[]) {
 
    int i = 0;
    unsigned char calc[SHA_DIGEST_LENGTH];
// SHA_DIGEST_LENGTH*2 p/ evitar estouro de pilha ("smahing stack detected")
    char transf[SHA_DIGEST_LENGTH*2];

    //avisando que faltou string
    if ( argn != 2 ) {
        printf("Usando: %s faltou string\n", argv[0]);
        return -1;
    }
 //preenchendo, inicializabdo transf e calc
    memset(transf, 0x0, SHA_DIGEST_LENGTH*2);
    memset(calc, 0x0, SHA_DIGEST_LENGTH);
 
    SHA1((unsigned char *)argv[1], strlen(argv[1]), calc);
 
    for (i=0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf((char*)&(transf[i*2]), "%02x", calc[i]);
    }
 
    printf("SHA1 of %s is %s\n", argv[1], transf);
 
    return 0;
 
}
