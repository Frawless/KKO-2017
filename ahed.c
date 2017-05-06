/*
 * Autor: Jakub Stejskal (xstejs24)
 * Datum: 3.5. 2017
 * Soubor: ahead.c
 * Komentar: 
 */ 

#include <cstdlib>
#include "ahed.h"

#include <bitset>



/**
 * Funkce pro inicializaci stromu.
 * @param root ukazatel na strom
 */
bool treeInit(T_NODE_PTR* root)
{
	*root = (T_NODE_PTR)malloc(sizeof(T_NODE));
	
	// Osetreni nemoznosti alokace pameti
	if(*root == NULL)
		return false;
	
	(*root)->leftChild = NULL;
	(*root)->rightChild = NULL;
	(*root)->parent = NULL;
	(*root)->count = 0;
	(*root)->code = 0;
	(*root)->level = 0;
	(*root)->rank = 0;
	(*root)->symbol = ESC;
	
	return true;
}

/**
 * Funkce pro smazání stromu z paměti.
 * @param root - ukazatel na strom
 */
void dispose (T_NODE_PTR *root)
{
    if(*root == NULL)    
        return ;
    else
    {
        if((*root)->leftChild != NULL) // Pokud mohu jít doleva
            dispose(&(*root)->leftChild);
        if((*root)->rightChild != NULL) // Pokud mohu jít doleva
            dispose(&(*root)->rightChild);
        if(((*root)->leftChild == NULL) && ((*root)->leftChild == NULL)){
			(*root)->parent = NULL;
            free(*root);
		}
        *root = NULL;
		free(*root);
    }
	
    return ;
}

void createNewNode(std::map<int,T_NODE_PTR> &lists, int16_t symbol)
{
	T_NODE_PTR newNodeLeft = (T_NODE_PTR)malloc(sizeof(T_NODE));
	T_NODE_PTR newNodeRight = (T_NODE_PTR)malloc(sizeof(T_NODE));
	
	T_NODE_PTR root = lists[ESC];
	std::cerr<<"Picking Root: "<<root<<std::endl;
	std::cerr<<"Picking Root symbol: "<<root->symbol<<std::endl;
	
	if(newNodeLeft == NULL || newNodeRight == NULL)
	{
		free(newNodeLeft);
		free(newNodeRight);
		std::cerr<<"Nedostatek místa pro alokaci!"<<std::endl;
		exit(AHEDFail);
	}
	
	root->leftChild = newNodeLeft;
	root->rightChild = newNodeRight;
	root->symbol = 257;
	
	std::cerr<<"Set Root: "<<root<<std::endl;
	std::cerr<<"Set Root symbol: "<<root->symbol<<std::endl;
	
	newNodeLeft->parent = newNodeRight->parent = root;
	newNodeLeft->leftChild = newNodeRight->leftChild = newNodeLeft->rightChild = newNodeRight->rightChild = NULL;
	
	newNodeLeft->symbol = 256;
	newNodeLeft->count = 0;
//	newNodeLeft->code = (root->code << 1) + 0;
	newNodeLeft->rank = root->rank + 2;
	
	std::cerr<<"New Leftt: "<<newNodeLeft<<std::endl;
	std::cerr<<"New Leftt symbol: "<<newNodeLeft->symbol<<std::endl;	
	
	newNodeRight->symbol = symbol;
	newNodeRight->count = 0;	
//	newNodeRight->code = (root->code << 1) + 1;	
	newNodeRight->rank = root->rank + 1;
	
	newNodeLeft->level = newNodeRight->level = newNodeLeft->parent->level + 1;
	
	std::cerr<<"NewRoot: "<<newNodeLeft<<std::endl;
	std::cerr<<"Symbol na: "<<symbol<<std::endl;
	std::cerr<<"ESC na: "<<ESC<<std::endl;
	std::cerr<<"ROOT na: "<<NAS+1<<std::endl;
	std::cerr<<"ESC: "<<newNodeLeft<<std::endl;
	std::cerr<<"ESC parent: "<<newNodeLeft->parent<<std::endl;
	std::cerr<<"Roott: "<<root<<std::endl;
	
	std::cerr<<"chci vložit: "<<symbol<<" kód: "<<newNodeRight<<std::endl;
	std::cerr<<"chci vložit: "<<ESC<<" kód: "<<newNodeLeft<<std::endl;
	std::cerr<<"chci vložit: "<<NAS+1<<" kód: "<<root<<std::endl;
	

	lists.insert(std::pair<int,T_NODE_PTR>(symbol,newNodeRight));
	lists.insert(std::pair<int,T_NODE_PTR>(ESC,newNodeLeft));
	lists.insert(std::pair<int,T_NODE_PTR>(NAS+1,root));
	
	std::cerr<<"Mapsize: "<<lists.size()<<std::endl;
	
//	std::cerr<<"Vloženo/Upraveno: "<<symbol<<" kód: "<<lists[symbol]->code<<" Count: "<<lists[symbol]<<std::endl;
	std::cerr<<"Vloženo/Upraveno: "<<ESC<<" kód: "<<lists[ESC]->code<<" Count: "<<lists[ESC]<<std::endl;
	std::cerr<<"Vloženo/Upraveno: "<<NAS+1<<" kód: "<<lists[NAS+1]->code<<" Count: "<<lists[NAS+1]<<std::endl;
	
//	lists[symbol] = newNodeLeft;
//	lists[ESC] = newNodeRight;
//	lists[NAS+(root)->rank] = (root);
	
}


void printTree(T_NODE_PTR tree){
	if(tree == NULL)
		return;
	
	std::cerr<<"Vrchol: "<<tree->code<<" Count: "<<tree->count<<std::endl;
	if(tree->leftChild != NULL)
		printTree(tree->leftChild);
	if(tree->rightChild != NULL)
		printTree(tree->rightChild);
}

// Aktualizace
//21:          
//22:  procedure aktualizuj_strom(U)
//23:  begin
//24:     while (U!=kořen) do 
//25:      begin
//26:        if (existuje uzel U1 se stejným ohodnocením a vyšším pořadím) then 
//27:           vyměň U1 a U
//28:        zvyš ohodnocení U o 1
//29:        U := předek(U)
//30:      end
//31:     zvyš ohodnocení U o 1, aktualizuj kódy listů
//32:  end
void updateTree(int64_t symbol, std::map<int,T_NODE_PTR> &lists)
{
	T_NODE_PTR last = lists[symbol];
	T_NODE_PTR change;
	
	std::cerr<<"Test: "<<last->count<<std::endl;
	
	while(last != NULL)
	{
		change = last;
		if(last->parent == NULL)
			break;
		std::map<int,T_NODE_PTR>::iterator it;
		// Iteruju pres vsechny vlozene uzly
		for(it = lists.begin(); it != lists.end(); it++)
		{
			if(last->count == it->second->count && last->rank < it->second->rank)
			{
				std::cerr<<"CNT: "<<last->count<<std::endl;
				std::cerr<<"CNT it: "<<it->second->count<<std::endl;
				std::cerr<<"rank: "<<last->rank<<std::endl;
				std::cerr<<"rank it: "<<it->second->rank<<std::endl;
				change = it->second;
			}
		}
		std::cerr<<" : "<<last<<" : "<<change<<std::endl;
		std::cerr<<" : "<<last->symbol<<" : "<<change->symbol<<std::endl;
		
		
		if(last->parent != change && last != change)
		{
			T_NODE_PTR tmp = last;
					
			std::cerr<<"TestIN"<<std::endl;
			
			if(last->parent->rightChild == last){
				last->parent->rightChild = change;
				change->code = (last->parent->code << 1) + 0;
			}
			else{
				last->parent->leftChild = change;
				change->code = (last->parent->code << 1) + 1;				
			}
			
			std::cerr<<"TestSWAP1"<<std::endl;
			
			if(change->parent->rightChild == change){
				change->parent->rightChild = last;
				last->code = (change->parent->code << 1) + 0;
			}
			else{
				change->parent->leftChild = last;
				last->code = (change->parent->code << 1) + 1;				
			}
			

			std::cerr<<"TestSWAP2"<<std::endl;

			last->parent = change->parent;
			change->parent = tmp->parent;
			
			last->rank = change->rank;
			change->rank = tmp->rank;

			last->level = last->parent->level + 1;
			change->level = change->parent->level + 1;
		}
		
		std::cerr<<"Test3"<<std::endl;
		
		last->count++;
		last = last->parent;
		if(last == NULL)
			std::cerr<<"Nasrat"<<std::endl;
	}
}

// http://stackoverflow.com/questions/699968/display-the-binary-representation-of-a-number-in-c
// Převzato
char *getBinaryCode (int16_t val, char *buff, int32_t sz) {
    char *pbuff = buff;

    /* Must be able to store one character at least. */
    if (sz < 1) return NULL;

    /* Special case for zero to ensure some output. */
    if (val == 0) {
        *pbuff++ = '0';
        *pbuff = '\0';
        return buff;
    }

    /* Work from the end of the buffer back. */
    pbuff += sz;
    *pbuff-- = '\0';

    /* For each bit (going backwards) store character. */
    while (val != 0) {
        if (sz-- == 0) return NULL;
        *pbuff-- = ((val & 1) == 1) ? '1' : '0';

        /* Get next bit. */
        val >>= 1;
    }
    return pbuff+1;
}

// Princip algoritmu
// 1:  begin
// 2:     vytvoř uzel ZERO
// 3:     readSymbol(X)
// 4:     while (X!=EOF) do 
// 5:      begin
// 6:        if (prvníNačteni(X)) then
// 7:         begin
// 8:           output(kód uzlu ZERO)
// 9:           output(X)
//10:           vytvoř nový uzel U s nasledníky ZERO a list s X
//11:           aktualizuj_strom(U);
//12:         end
//13:        else
//14:         begin
//15:           output(kód uzlu s X)
//16:           aktualizuj_strom(uzel s X)
//17:         end
//18:        readSymbol(X)
//19:      end
//20:  end

/* Nazev:
 *   AHEDEncoding
 * Cinnost:
 *   Funkce koduje vstupni soubor do vystupniho souboru a porizuje zaznam o kodovani.
 * Parametry:
 *   ahed - zaznam o kodovani
 *   inputFile - vstupni soubor (nekodovany)
 *   outputFile - vystupni soubor (kodovany)
 * Navratova hodnota: 
 *    0 - kodovani probehlo v poradku
 *    -1 - pri kodovani nastala chyba
 */
int AHEDEncoding(tAHED *ahed, FILE *inputFile, FILE *outputFile)
{
	T_NODE_PTR huffmanTree;
	if(!treeInit(&huffmanTree))
		exit(AHEDFail);
	
//	u_char *output;
	
	std::map<int,T_NODE_PTR> nodes;
	std::cerr<<"Test: "<<huffmanTree<<std::endl;
	nodes.insert(std::pair<int,T_NODE_PTR>(ESC,huffmanTree));
	
	int16_t ch;
	while((ch = fgetc(inputFile)) != EOF)
	{
		std::cerr<<"Znak: "<<ch<<std::endl;
		
//		if(nodes[ch] == NULL)
//			std::cerr<<"Není vestromě"<<std::endl;
		
		
//		std::cerr<<"Nodes in: "<<nodes[ch]<<std::endl;
		if (nodes.find(ch) == nodes.end()){
			// NOT found
			if(ahed->uncodedSize != 0){
//				char *buffer = new char[nodes[ESC]->level+1];
//				std::cerr<<"Code: "<<nodes[ESC]->code<<std::endl;
//				std::cerr<<"Level: "<<nodes[ESC]->level<<std::endl;
//				getBinaryCode(nodes[ESC]->code,buffer,nodes[ESC]->level);
			}
			std::cerr<<"Přidávám: "<<ch<<std::endl;
			// Vytvorim novy node
			createNewNode(nodes,ch);
			
			printTree(huffmanTree);
			
			// Aktualizuju strom
//			updateTree(ch,nodes);
			
		} else {
		// found
		}
		ahed->uncodedSize++;
		
		
		std::map<int,T_NODE_PTR>::iterator it;
		// Iteruju pres vsechny vlozene uzly
		for(it = nodes.begin(); it != nodes.end(); it++)
		{
			if(it->second->parent != NULL)
				std::cerr<<"Key: "<<it->first<<" code: "<<it->second->code<<" parent symbol: "<<it->second->parent->symbol<<" borther: "<<it->second->parent->rightChild->code<<" : "<<it->second->parent->leftChild->code<<std::endl;
			else
				std::cerr<<"Key: "<<it->first<<" code: "<<it->second->code<<std::endl;
		}			
		
//		std::cerr<<"UncodedSize: "<<ahed->uncodedSize<<std::endl;
	}
	
	dispose(&huffmanTree);
	
	
//	free(huffmanTree);
	
//	std::cerr<<nodes[ESC]->code<<std::endl;
//	nodes.clear();
	
	std::cerr<<"Dispose OK"<<std::endl;	
	return AHEDOK;
}

/* Nazev:
 *   AHEDDecoding
 * Cinnost:
 *   Funkce dekoduje vstupni soubor do vystupniho souboru a porizuje zaznam o dekodovani.
 * Parametry:
 *   ahed - zaznam o dekodovani
 *   inputFile - vstupni soubor (kodovany)
 *   outputFile - vystupn� soubor (nekodovany)
 * Navratova hodnota: 
 *    0 - dekodovani probehlo v poradku
 *    -1 - pri dekodovani nastala chyba
 */
int AHEDDecoding(tAHED *ahed, FILE *inputFile, FILE *outputFile)
{
	return AHEDOK;
}



