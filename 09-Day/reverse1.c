/*
 *	Resolução de um crackMe. Todo esse código
 *	me serviu para entender uma rotina de comparação de strings,
 *	implementada no CrackMe feito pelo diABLO
 *
 *	Baixe aqui: https://github.com/ReversingID/Crackmes-Repository/tree/master
 *	Encontre ele no path: Crackmes.DE\1-very_easy_for_newbies\windows\diablo
 *
 *	Processo de funcionamento da função no final desse código
*/


#include <stdio.h>

int cmp_user_input(const char[], char[]);


int main()
{
	//key 
	const char* aVeryeasy = "***vErYeAsY***";
	char userIn[14];

	// get user input
	printf("\nDigit the Code: ");
	scanf("%s",userIn);
	
	//cmp_user_input(key, userIn);
	
	printf("[Start to compare strings]\n");
	if(strcmp(aVeryeasy, userIn) == 0)
	{
	printf("[Finish compare strings]\n");
		printf("\tWork Well!!!\n");
	}
	else{
		printf("[Finish compare strings]\n");
		printf("\tWrong Code! Try Again!\n");
	}
	printf("[Closing the program]\n");
	return 0;
}


int cmp_user_input(const char key[], char userIn[])
{
		return 0;
	
}



/*******************************************************/
/************ ENTENDENDO ROTINAS DO CRACKME ************/
/*******************************************************/
/* 1.chama funcao que conta o numero de chars das strings
   2.funcao conta_chars retorna
   3.dentro da funcao caller [crackme1.46B770]:

	Pega eax (o retorno) e:
	
	al = conta_input(User_Input); // exemple "666" = 3. al = 3
	setne al // Set byte if not equal (ZF=0).
	
	// Sets the destination operand to 0 or 1 depending on the settings of the status flags (CF, SF, OF, ZF, and PF) in the
    // . EFLAGS registerThe destination operand points to a byte register or a byte in memory. The condition code suffix
    // (cc) indicates the condition being tested for.

/*******************************************************/

/*******************************************************/
/*

[Testando alguns cálculos]
Enviando "AAA":
HEX: 0x41 + 0x41 + 0x41 = 0xC3
DEC: 65 + 65 + 65 = 195

HEX: 0x41^3 = 0x430C1
DEC: 65^3 = 274625

// 02136E90 é recorrente? [para "eae"?]
// 021352C4 é recorrente? [para "AAA"?]
[FIM Testando alguns cálculos]

[Funcao: crackme1.46B828]
As duas strings são compradas,
	DIFERENTES:
		- se diferentes o programa subtrai o tamanho das strings:
		tamanho da Senha certa "Code" (0xE) menos a do Input do usuário
		No exemplo: "AAA" = 3 e "***vErYeAsY***" = 14 ou 0xE
		3 - 0xE = FFFFFFF5
		Esse resultado se dá pelo MST ser setado - confirmar
   
		- soma o tamanho da string certa, o code com o resultado
		  da operação anterior:
		  0xE + FFFFFFF5 = 00000003
		  
		- Salva o resultado 00000003 na stack
		- dá um shift right 2 (SHR) no resultado anterior:
		  00000003 >> 2 = 00000000
		- Confirma a integridade da senha[?]
		- Guarda valor 3 em edx
		
		- faz operação AND entre edx e 3:
		  and 3 e 3: 
					0000 0011 
					0000 0011
					----------
					0000 0011
					
		  Ele só testa para ver se EDX é realmente 3.
		  
		  E em seguida ele vai testando os bytes, 
		  quatro por vez: por isso EDX é 3,
		  perceba que ele decrementa edx três vezes
		  se a string for a correta: 14 / 4 = 3,
		  e assim ele vai comparando o byte mais a esquerda (CH),
		  e depois o bytes mais a direita (CL), e assim ele 
		  vai indo até achar dois bytes diferentes ou confirmar
		  que todos os bytes são iguais.
		  
		  A função retorna 1 para strings incompátiveis
		  e retorna 0 para strings iguais.
		  
		  Ele nos direciona com base no retorno,
		  caso 0, nós obtemos a mensagem de sucesso,
		  caso 1, precisamos tentar denovo.
*/