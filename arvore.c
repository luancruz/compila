#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estrutura_arvore.h"
#include "myscanner.tab.h"

/* recursive drawing of the syntax tree */
void procNome (noTipo *ptr);
void comparaNome(char* nome);

void cgen(noTipo *ptr,int numLP);

//variaveis
int numNomes = 0;//contador de variaveis
char* vetNomes[10000];//vetor de variaveis
char* vetFuncao[10000];//vetor de variaveis
int head = 0;//controle para impressao do cabecalho uma vez
int foot = 0;
int arg = 0;


int init(noTipo *ptr){
    cgen(ptr,0);
    
    //termina geracao
    fprintf(yyout,"\nli $v0, 10\nsyscall\n");	

    return 0;
}

//armazena todas as variaveis em um vetor apenas uma vez
void comparaNome(char* nome){
    int i;
    if(!nome) return;

    if(numNomes == 0){
    	vetNomes[0] = malloc(1000*sizeof(char));
	strcpy(vetNomes[0],nome);
	numNomes++;
	return;
    }
	
    for(i = 0; i < numNomes; i++){
    	if(strcmp(vetNomes[i], nome) == 0 || strcmp(nome,"print") == 0)
       	    return;
    }

    vetNomes[numNomes] = malloc(1000*sizeof(char));
    strcpy(vetNomes[numNomes],nome);
    numNomes++;	
}


void procNome(noTipo *ptr){
    int i;
    if (!ptr) return;
    switch(ptr->tipo) {
        case tipoIdentAtrib:        
		comparaNome(ptr->noId.ident);
		break;
        case tipoIdent:        
		comparaNome(ptr->noId.ident);
		break;
        case tipoNaoTerm: 
		for(i =0; i< ptr->noNaoTerminal.numFilhos; i++){
		    procNome(ptr->noNaoTerminal.filho[i]);            
		} 	 
        	break;   
        case tipoOperad:           	
        	for (i = 0; i < ptr->noOperad.nops; i++) {
        		procNome(ptr->noOperad.op[i]); 				   	   		        
        	} 			 
		break;    
    }
}

/*
int comparaFuncao(char* funcao){
int i;
    if(!nome) return;

    if(numNomes == 0){
    	vetFuncao[0] = malloc(1000*sizeof(char));
	strcpy(vetFuncao[0],nome);
	numNomes++;
	return;
    }
	
    for(i = 0; i < numNomes; i++){
    	if(strcmp(vetNomes[i], nome) == 0 || strcmp(nome,"print") == 0)
       	    return;
    }

    vetFuncao[numNomes] = malloc(1000*sizeof(char));
    strcpy(vetNomes[numNomes],nome);
    numNomes++;		

if(*lista == NULL)
	{
		*lista = (t_lista*) xmalloc(sizeof(t_lista));
		(*lista)->chave = copiaString(chave);
		(*lista)->valor = copiaString(valor);
		(*lista)->i_valor = n_args;
		(*lista)->proximo = NULL;
	}
	else
	{
		t_lista * it = (*lista);

		while(1)
		{
			if( iguais(it->chave, chave) )
			{
				it->valor = valor;
				it->i_valor = n_args;
			}
			else
			{
				if(it->proximo != NULL)
				{
				    it = it->proximo;
				}
				else
				{
					break;
				}
			}
		}

		it->proximo = (t_lista*) xmalloc(sizeof(t_lista));
		it->proximo->chave = copiaString(chave);
		it->proximo->valor = copiaString(valor);
		it->proximo->i_valor = n_args;
		it->proximo->proximo = NULL;
	}

	return 1;
}

*/


//imprime codigo mips no arquivo de saida
void cgen(noTipo *ptr,int numLP){
    int i;
    if (!ptr) return;

    if(head == 0){	
	numLP = 0;
	
        //impressao do codigo mips no arquivo - cabecalho
        procNome(ptr);//variaveis globais em vetNomes
	
        fprintf (yyout,".data\n");
        for(i = 0; i < numNomes; i++){
    	    fprintf (yyout,"_%s: .word 0\n",vetNomes[i]);
        }
        fprintf(yyout,"_newline: .asciiz \"\\n\"\n\n.text\n.globl main\n\nmain:\n\n");	
	head = 1;
    }

    switch(ptr->tipo) {
        case tipoConst: 
		fprintf (yyout,"li $a0, %d\n", ptr->noConst.num); break;
	case tipoIdentAtrib: 
		fprintf(yyout,"\nsw $a0, _%s\n\n", ptr->noId.ident); break;
		//if(strcmp(ptr->noId.ident,"print") == 0)
		    //fprintf (yyout,"\nli $v0, 1\nsyscall\n\nli $v0, 4\nla $a0, _newline\nsyscall\n\n"); break;
	case tipoIdentFunc: 
	    fprintf(yyout, "%s:\n", ptr->noId.ident);
	break;  
	case tipoIdent: 
	    arg++;
	break;        
	case tipoNaoTerm: 
	    //switch(ptr->noNaoTerminal.terminal){
		//case chamadadefuncao: 
		  //  if(strcmp(ptr->noId.ident,"print") == 0)
		    	//fprintf (yyout,"\nli $v0, 1\nsyscall\n\nli $v0, 4\nla $a0, _newline\nsyscall\n\n"); break;
	   // }
		for(i =0; i< ptr->noNaoTerminal.numFilhos; i++){
		    cgen(ptr->noNaoTerminal.filho[i],numLP);            
		} 	 
        	break;   
	case tipoOperad:             	
            switch(ptr->noOperad.oper){
		case T_FUNCTION:
		    fprintf(yyout,"j skip%d\n", numLP);
		    cgen(ptr->noOperad.op[0],numLP);
		    fprintf(yyout,"move $fp, $sp\nsw $ra, 0($sp)\naddiu $sp, $sp, -4\n");

		    arg = 0;
		    cgen(ptr->noOperad.op[1],numLP);
		    if(arg > 0){
			cgen(ptr->noOperad.op[2],numLP);
		    }	
		    fprintf(yyout,"lw $ra, 4($sp)\naddiu $sp, $sp %d\nlw $fp, 0($sp)\njr $ra\n", (8));	    
	            fprintf(yyout,"skip%d:\n", numLP);
		break;
		case T_PRINT: 
		    cgen(ptr->noOperad.op[0],0);
		    for(i = 0; i < numNomes; i++){
    			if(strcmp(vetNomes[i], ptr->noOperad.op[0]->noOperad.op[0]->noOperad.op[0]->noId.ident) == 0){		    
			    fprintf (yyout,"lw $a0, _%s\n", ptr->noOperad.op[0]->noOperad.op[0]->noOperad.op[0]->noId.ident);
			    break;
			}
		    }
		    fprintf (yyout,"\nli $v0, 1\nsyscall\n\nli $v0, 4\nla $a0, _newline\nsyscall\n\n"); break;   
		break;		
		case T_PLUS:  
		    cgen(ptr->noOperad.op[0],0);      
		    fprintf(yyout,"sw $a0, 0($sp)\naddiu $sp, $sp, -4\n"); //push
		    cgen(ptr->noOperad.op[1],0);
		    fprintf(yyout,"lw $t1, 4($sp)\nadd $a0, $a0, $t1\naddiu $sp, $sp, 4\n");   //add and pop  
		break;
		case T_MINUS:   
		   cgen(ptr->noOperad.op[0],0);     
		   fprintf(yyout,"sw $a0, 0($sp)\naddiu $sp, $sp, -4\n"); //push
		   cgen(ptr->noOperad.op[1],0);
		   fprintf(yyout,"lw $t1, 4($sp)\nsub $a0, $t1, $a0\naddiu $sp, $sp, 4\n");
		break;
                case T_TIMES:   
		    cgen(ptr->noOperad.op[0],0);    
		    fprintf(yyout,"sw $a0, 0($sp)\naddiu $sp, $sp, -4\n"); //push
		    cgen(ptr->noOperad.op[1],0);
		    fprintf(yyout,"lw $t1, 4($sp)\nmul $a0, $t1, $a0\naddiu $sp, $sp, 4\n");
		break;
                case T_DIV:     
		    cgen(ptr->noOperad.op[0],0); 
		    fprintf(yyout,"sw $a0, 0($sp)\naddiu $sp, $sp, -4\n"); //push
		    cgen(ptr->noOperad.op[1],0); 
		    fprintf(yyout,"lw $t1, 4($sp)\ndiv $a0, $a0, $t1\naddiu $sp, $sp, 4\n");     
		break;
                case T_LT:      
		    cgen(ptr->noOperad.op[0],0); 
		    fprintf(yyout,"sw $a0, 0($sp)\naddiu $sp, $sp, -4\n"); //push
		    cgen(ptr->noOperad.op[1],0);    
   		    fprintf(yyout,"lw $t1, 4($sp)\naddiu $sp, $sp 4\nblt $t1, $a0, true_branch%d\n",numLP);  
		    numLP = 1;
		break;
                case T_GT:      
		    cgen(ptr->noOperad.op[0],numLP); 
		    fprintf(yyout,"sw $a0, 0($sp)\naddiu $sp, $sp, -4\n"); //push
		    cgen(ptr->noOperad.op[1],numLP);    
   		    fprintf(yyout,"lw $t1, 4($sp)\naddiu $sp, $sp 4\nbgt $t1, $a0, true_branch%d\n",numLP);     
		break;
                case T_GTEQ:  //maior igual  
		    cgen(ptr->noOperad.op[0],numLP); 
		    fprintf(yyout,"sw $a0, 0($sp)\naddiu $sp, $sp, -4\n"); //push
		    cgen(ptr->noOperad.op[1],numLP);    
   		    fprintf(yyout,"lw $t1, 4($sp)\naddiu $sp, $sp 4\nbge $t1, $a0, true_branch%d\n",numLP);   
		break;
                case T_LTEQ:  //menor igual  
		    cgen(ptr->noOperad.op[0],numLP); 
		    fprintf(yyout,"sw $a0, 0($sp)\naddiu $sp, $sp, -4\n"); //push
		    cgen(ptr->noOperad.op[1],numLP);    
   		    fprintf(yyout,"lw $t1, 4($sp)\naddiu $sp, $sp 4\nble $t1, $a0, true_branch%d\n",numLP);       
		break;
                case T_NEQ:     
		    cgen(ptr->noOperad.op[0],numLP); 
		    fprintf(yyout,"sw $a0, 0($sp)\naddiu $sp, $sp, -4\n"); //push
		    cgen(ptr->noOperad.op[1],numLP);    
   		    fprintf(yyout,"lw $t1, 4($sp)\naddiu $sp, $sp 4\nbne $t1, $a0, true_branch%d\n",numLP);         
		break;
                case T_EQ:      
		    cgen(ptr->noOperad.op[0],numLP); 
		    fprintf(yyout,"sw $a0, 0($sp)\naddiu $sp, $sp, -4\n"); //push
		    cgen(ptr->noOperad.op[1],numLP);    
   		    fprintf(yyout,"lw $t1, 4($sp)\naddiu $sp, $sp 4\nbeq $t1, $a0, true_branch%d\n",numLP);         
		break;
                case T_AND:     
		    cgen(ptr->noOperad.op[0],numLP); 
		    fprintf(yyout,"sw $a0, 0($sp)\naddiu $sp, $sp, -4\n"); //push
		    cgen(ptr->noOperad.op[1],numLP);    
   		    fprintf(yyout,"lw $t1, 4($sp)\naddiu $sp, $sp 4\nand $t3, $t1, $a0\n");          
		break;
                case T_OR:      
		    cgen(ptr->noOperad.op[0],numLP); 
		    fprintf(yyout,"sw $a0, 0($sp)\naddiu $sp, $sp, -4\n"); //push
		    cgen(ptr->noOperad.op[1],numLP);    
   		    fprintf(yyout,"lw $t1, 4($sp)\naddiu $sp, $sp 4\nbeq $t3, $t1, $a0\n");          
		break;
		case T_NOT:     
		    cgen(ptr->noOperad.op[0],numLP);
    		    fprintf(yyout, "not $a0, $t1\n");   
		break; 
		case T_ASSIGN:	
   		    cgen(ptr->noOperad.op[1],numLP);					
		    cgen(ptr->noOperad.op[0],numLP);
		break;
		
                case T_WHILE:   
		    fprintf(yyout, "loop%d:\n",numLP);
		    cgen(ptr->noOperad.op[0],numLP);
		    fprintf(yyout, "j loop%d:\ntrue_branch%d:\nj $ra\n",numLP,numLP);
		break;
                case T_IF:      
		    cgen(ptr->noOperad.op[0],numLP);
		    fprintf(yyout,"false_brach%d:\n",numLP);
		    cgen(ptr->noOperad.op[2],numLP); //lista de elseif
		    cgen(ptr->noOperad.op[3],numLP);	//else								 
		    fprintf(yyout, "b end_if\ntrue_branch%d:\n", numLP);
		    cgen(ptr->noOperad.op[1],numLP); //then
		    fprintf(yyout, "end_if:\n");
		break;
                case T_ELSEIF:  
		    cgen(ptr->noOperad.op[0],numLP);
		    fprintf(yyout,"false_brach%d:\n",numLP);
		    cgen(ptr->noOperad.op[2],numLP); //lista de elseif								 
		    fprintf(yyout, "b end_if\ntrue_branch%d:\n", numLP);
		    cgen(ptr->noOperad.op[1],numLP); //then
		break;
		case T_ELSE:    
		    cgen(ptr->noOperad.op[0],numLP);    
		break;
		case T_FOR:     
		    cgen(ptr->noOperad.op[0],numLP);
		    fprintf(yyout, "loop%d:\n",numLP);						
		    cgen(ptr->noOperad.op[1],numLP);
		    if(ptr->noOperad.op[2]){ 
			cgen(ptr->noOperad.op[2],numLP);
		    }
		    fprintf(yyout, "j loop%d:\ntrue_branch%d:\nj $ra\n",numLP,numLP);		   
		break;  

	/*  
		case T_FUNCTION:fprintf(yyout, "[T_FUNCTION function]");    break;  
		case T_LOCAL:   fprintf(yyout, "[T_LOCAL local]");    break; 
		case T_THEN:    fprintf(yyout, "[T_THEN then]");    break;   
		case T_NIL:     fprintf(yyout, "[T_NIL nil]");    break; 
		case T_END:     fprintf(yyout, "[T_END end]");   break; 
		case T_RETURN:  fprintf(yyout,"[T_RETURN return]");    break; 
		case T_COMMA:   fprintf(yyout,"[T_COMMA ,]");    break; 
		case T_OPENPAR: fprintf(yyout,"[T_OPENPAR (]");    break; 
		case T_CLOSEPAR:fprintf(yyout, "[T_CLOSEPAR )]");   break;
	}
                
	

	*/	  
	    for (i = 0; i < ptr->noOperad.nops; i++) {
               cgen(ptr->noOperad.op[i],numLP); 				   	   		        
            } 			 
	    break;

	    }
    }//fecha switch


}







/*
void expNode (noTipo *ptr){
	int i;
    if (!ptr) return;
    switch(ptr->tipo) {
        //case tipoConst: fprintf (yyout,"[T_NUMBER %d]", ptr->noConst.num); break;
        case tipoIdent:        fprintf (yyout,"[T_NAME %s]",ptr->noId.ident); break;
        case tipoNaoTerm: 
        	switch(ptr->noNaoTerminal.terminal){			
        	case bloco: fprintf(yyout,"[bloco"); break;
        	case programa: fprintf(yyout,"[programa"); break;
        	case comandoret: fprintf(yyout,"[comandoret"); break;
        	case comando: fprintf(yyout,"[comando"); break;
        	case exp: fprintf(yyout,"[exp"); break;
        	case chamadadefuncao: fprintf(yyout,"[chamadadefuncao"); break;
        	case listadenomes: fprintf(yyout,"[listadenomes"); break;
        	case listaexp: fprintf(yyout,"[listaexp"); break;
        	case opbin: fprintf(yyout,"[opbin"); break;
        	case opunaria: fprintf(yyout,"[opunaria"); break;        	
        }
        for(i =0; i< ptr->noNaoTerminal.numFilhos; i++){
            expNode(ptr->noNaoTerminal.filho[i]);            
        } 
		fprintf(yyout,"]")	;	 
        break;   
        case tipoOperad:             	
            switch(ptr->noOperad.oper){
                case T_WHILE:   fprintf(yyout,"[T_WHILE while]"); break;
                case T_IF:      fprintf(yyout,"[T_IF if]");   break;
                case T_SEMICOL: fprintf(yyout, "[T_SEMICOL ;]");     break;
                case T_ASSIGN:  fprintf(yyout,"[T_ASSIGN =]");     break;
                case T_MINUS:   fprintf(yyout,"[T_MINUS -]");     break;
                case T_PLUS:    fprintf(yyout,"[T_PLUS +]");     break;
                case T_TIMES:   fprintf(yyout, "[T_TIMES *]");     break;
                case T_DIV:     fprintf(yyout,"[T_DIV /]");     break;
                case T_LT:      fprintf(yyout, "[T_LT <]");     break;
                case T_GT:      fprintf(yyout, "[T_GT >]");     break;
                case T_GTEQ:    fprintf(yyout, "[T_GTEQ >=]");    break;
                case T_LTEQ:    fprintf(yyout, "[T_LTEQ <=]");    break;
                case T_NEQ:     fprintf(yyout, "[T_NEQ ~=]");    break;
                case T_EQ:      fprintf(yyout,"[T_EQ ==]");    break;
                case T_AND:     fprintf(yyout, "[T_AND and]");    break;
                case T_OR:      fprintf(yyout,"[T_OR or]");    break;
                case T_ELSEIF:  fprintf(yyout, "[T_ELSEIF elseif]");    break;
				case T_ELSE:    fprintf(yyout, "[T_ELSE else]");    break;
				case T_DO:      fprintf(yyout, "[T_DO do]");    break;  
				case T_FOR:     fprintf(yyout, "[T_FOR for]");    break;  
				case T_FUNCTION:fprintf(yyout, "[T_FUNCTION function]");    break;  
				case T_LOCAL:   fprintf(yyout, "[T_LOCAL local]");    break; 
				case T_THEN:    fprintf(yyout, "[T_THEN then]");    break;  
				case T_NOT:     fprintf(yyout, "[T_NOT not]");    break;  
				case T_NIL:     fprintf(yyout, "[T_NIL nil]");    break; 
				case T_END:     fprintf(yyout, "[T_END end]");   break; 
				case T_RETURN:  fprintf(yyout,"[T_RETURN return]");    break; 
				case T_COMMA:   fprintf(yyout,"[T_COMMA ,]");    break; 
				case T_OPENPAR: fprintf(yyout,"[T_OPENPAR (]");    break; 
				case T_CLOSEPAR:fprintf(yyout, "[T_CLOSEPAR )]");    break;												                	
            }
            for (i = 0; i < ptr->noOperad.nops; i++) {
               expNode (ptr->noOperad.op[i]); 				   	   		        
            } 			 
			break;    
		  
    }
}
*/



