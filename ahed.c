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
//	std::cerr<<"Picking Root: "<<root<<std::endl;
//	std::cerr<<"Picking Root symbol: "<<root->symbol<<std::endl;
	
	if(newNodeLeft == NULL || newNodeRight == NULL)
	{
		free(newNodeLeft);
		free(newNodeRight);
		std::cerr<<"Nedostatek místa pro alokaci!"<<std::endl;
		exit(AHEDFail);
	}
	
	root->leftChild = newNodeLeft;
	root->rightChild = newNodeRight;
	root->symbol = NAS;
	
//	std::cerr<<"Set Root: "<<root<<std::endl;
//	std::cerr<<"Set Root symbol: "<<root->symbol<<std::endl;
//	std::cerr<<"Root code: "<<std::bitset<8>(root->code)<<std::endl;
	
	newNodeLeft->parent = newNodeRight->parent = root;
	newNodeLeft->leftChild = newNodeRight->leftChild = newNodeLeft->rightChild = newNodeRight->rightChild = NULL;
	
	newNodeLeft->symbol = ESC;
	newNodeLeft->count = 0;
//	std::cerr<<"CodeP: "<<std::bitset<8>(root->code)<<std::endl;
	newNodeLeft->code = (root->code << 1) + 0;
//	std::cerr<<"CodeR: "<<std::bitset<8>(newNodeLeft->code)<<std::endl;
	newNodeLeft->rank = root->rank + 2;
	
//	std::cerr<<"New Leftt: "<<newNodeLeft<<std::endl;
//	std::cerr<<"New Leftt symbol: "<<newNodeLeft->symbol<<std::endl;	
	
	newNodeRight->symbol = symbol;
	newNodeRight->count = 0;	
//	std::cerr<<"CodeP: "<<std::bitset<8>(root->code)<<std::endl;
	newNodeRight->code = (root->code << 1) + 1;	
	newNodeRight->rank = root->rank + 1;
//	std::cerr<<"CodeR: "<<std::bitset<8>(newNodeRight->code)<<std::endl;
	
	newNodeLeft->level = newNodeRight->level = newNodeLeft->parent->level + 1;
	
//	std::cerr<<"NewRoot: "<<newNodeLeft<<std::endl;
//	std::cerr<<"Symbol na: "<<symbol<<std::endl;
//	std::cerr<<"ESC na: "<<ESC<<std::endl;
//	std::cerr<<"ROOT na: "<<NAS+1<<std::endl;
//	std::cerr<<"ESC: "<<newNodeLeft<<std::endl;
//	std::cerr<<"ESC parent: "<<newNodeLeft->parent<<std::endl;
//	std::cerr<<"Roott: "<<root<<std::endl;
//	
//	std::cerr<<"chci vložit: "<<symbol<<" kód: "<<newNodeRight<<std::endl;
//	std::cerr<<"chci vložit: "<<ESC<<" kód: "<<newNodeLeft<<std::endl;
//	std::cerr<<"chci vložit: "<<NAS+root->rank<<" kód: "<<root<<std::endl;
//	

	lists.insert(std::pair<int,T_NODE_PTR>(symbol,newNodeRight));
//	lists.insert(std::pair<int,T_NODE_PTR>(ESC,newNodeLeft));
	lists[ESC] = newNodeLeft;
	lists.insert(std::pair<int,T_NODE_PTR>(NAS+root->rank,root));
	
//	std::cerr<<"Mapsize: "<<lists.size()<<std::endl;
//	
//	std::cerr<<"Vloženo/Upraveno: "<<symbol<<" kód: "<<lists[symbol]->code<<" Count: "<<lists[symbol]<<std::endl;
//	std::cerr<<"Vloženo/Upraveno: "<<ESC<<" kód: "<<lists[ESC]->code<<" Count: "<<lists[ESC]<<std::endl;
//	std::cerr<<"Vloženo/Upraveno: "<<NAS+1<<" kód: "<<lists[NAS+root->rank]->code<<" Count: "<<lists[NAS+root->rank]<<std::endl;
	
//	lists[symbol] = newNodeLeft;
//	lists[ESC] = newNodeRight;
//	lists[NAS+(root)->rank] = (root);
	
}


void printTree(T_NODE_PTR tree){
	if(tree == NULL)
		return;
	
	std::cerr<<"\t\t\ts: "<<tree->symbol<<" count: "<<tree->count<<" rank: "<<tree->rank<<" l: "<<tree->level<<" c: "<<std::bitset<8>(tree->code)<<std::endl;
	
//	if(tree->parent == NULL)
//		std::cerr<<tree<<" Vrchol: "<<tree->code<<" Count: "<<tree->count<<std::endl;
	
	if(tree->leftChild != NULL){
		std::cerr<<"s: "<<tree->leftChild->symbol<<" count: "<<tree->leftChild->count<<" rank: "<<tree->leftChild->rank<<" l: "<<tree->leftChild->level<<" c: "<<std::bitset<8>(tree->rightChild->code)<<std::endl;
//			std::cerr<<tree<<" Vrchol: "<<tree->symbol<<" Count: "<<tree->count<<" LeftS: "<<tree->leftChild->symbol<<" RightS: "<<tree->rightChild->symbol<<std::endl;
		
//			std::cerr<<tree<<" Vrchol: "<<tree->symbol<<" Count: "<<tree->count<<" LeftS: "<<tree->leftChild->symbol<<" RightS: "<<tree->rightChild->symbol<<" parent: "<<tree->parent<<" : "<<tree->parent->symbol<<std::endl;
			
	}
	if(tree->rightChild != NULL)
	{
		std::cerr<<"\t\t\t\t\t\ts: "<<tree->rightChild->symbol<<" count: "<<tree->rightChild->count<<" rank: "<<tree->rightChild->rank<<" l: "<<tree->rightChild->level<<" c: "<<std::bitset<8>(tree->rightChild->code)<<std::endl;
	}
	
//	std::cerr<<tree<<" Vrchol: "<<tree->code<<" Count: "<<tree->count<<std::endl;
//	std::cerr<<tree<<" Vrchol: "<<tree->code<<" Count: "<<tree->count<<" LeftS: "<<tree->leftChild->symbol<<" RightS: "<<tree->rightChild->symbol<<std::endl;

	if(tree->leftChild != NULL){
		printTree(tree->leftChild);
	}
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
	
//	std::cerr<<"Test: "<<last<<std::endl;
	int x = 0;
	while(last != NULL)
	{
//		if(x == 10)
//			return;
		T_NODE_PTR change = last;
//		std::cerr<<last->parent<<std::endl;
//		if(last->parent == NULL)
//			break;
		std::map<int,T_NODE_PTR>::iterator it;
		// Iteruju pres vsechny vlozene uzly
		for(it = lists.begin(); it != lists.end(); it++)
		{
			if(last->count == it->second->count && change->rank > it->second->rank)
			{
//				std::cerr<<"CNT: "<<last->count<<std::endl;
//				std::cerr<<"CNT it: "<<it->second->count<<std::endl;
//				std::cerr<<"rank: "<<last->rank<<std::endl;
//				std::cerr<<"rank it: "<<it->second->rank<<std::endl;
				change = it->second;
			}
		}
		
//		std::cerr<<"Test: "<<last<<std::endl;
//		std::cerr<<"Test2: "<<change<<std::endl;
		if(last->parent != change && last != change)
		{
			T_NODE_PTR tmp = last->parent;
			int64_t tmpRank = last->rank;

			if(last->parent->rightChild == last){
				last->parent->rightChild = change;
			}
			else{
				last->parent->leftChild = change;
			}
			
//			std::cerr<<"TestSWAP1"<<std::endl;
			
			if(change->parent->rightChild == change){
				change->parent->rightChild = last;
			}
			else{
				change->parent->leftChild = last;
			}
			

//			std::cerr<<"TestSWAP2"<<std::endl;

			last->parent = change->parent;
			last->rank = change->rank;
			
			change->parent = tmp;
			change->rank = tmpRank;
		
//			last->level = last->parent->level + 1;
//			change->level = change->parent->level + 1;
			
			updateCode(&last->parent);
			updateCode(&change->parent);
		}
		
//		std::cerr<<"Test3"<<std::endl;
		
		last->count++;
//		if(last->parent == NULL)
//			updateCode(&last);
		last = last->parent;
		x++;
	}
	
}


void updateCode(T_NODE_PTR *tree)
{
	T_NODE_PTR root = (*tree);
	if(root->parent == NULL){
		root->code = 0;
		root->level = 0;
	}
	if(root->leftChild != NULL){
		root->leftChild->code = (root->leftChild->parent->code << 1) + 0;
		root->leftChild->level = root->leftChild->parent->level + 1;
		updateCode(&root->leftChild);
	}
	if(root->rightChild != NULL){
		root->rightChild->code = (root->rightChild->parent->code << 1) + 1;
		root->rightChild->level = root->rightChild->parent->level + 1;	
		updateCode(&root->rightChild);
	}
}

	

// http://stackoverflow.com/questions/699968/display-the-binary-representation-of-a-number-in-c
// Převzato
//int16_t getBinaryCode (int16_t val, int8_t *buff, int32_t sz) {
//    int8_t *pbuff = buff;
//
//    /* Must be able to store one character at least. */
//    if (sz < 1) return NULL;
//
//    /* Special case for zero to ensure some output. */
//    if (val == 0) {
//		for(int x = 1; x <= sz; x++)
//			*pbuff++ = '0';
//        *pbuff = '\0';
//        return buff;
//    }
//
//    /* Work from the end of the buffer back. */
//    pbuff += sz;
//    *pbuff-- = '\0';
//
//    /* For each bit (going backwards) store character. */
//    while (val != 0) {
//        if (sz-- == 0) return NULL;
//        *pbuff-- = ((val & 1) == 1) ? '1' : '0';
//
//        /* Get next bit. */
//        val >>= 1;
//    }
//    return pbuff+1;
//}

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

void printBuffer(FILE *file, T_BUFFER *buffer){
	printf("Zapisuju: %c\n",buffer->buffer);
	fprintf(file,"%c",buffer->buffer);
	buffer->buffer = 0;
	buffer->position = 0;
}

void encodeSymbol(int16_t symbol, int32_t level, T_BUFFER *buffer, FILE *outputFile)
{
	printf("Symbol pro zapis: \"%c\"\n",symbol);
	for(int x = 1; x <= level; x++){
		buffer->buffer = (buffer->buffer << 1) + ((symbol >> (level-x)) & 1);
		buffer->position++;
		printf("test\n");
		if(buffer->position == 8){
			printBuffer(outputFile,buffer);
		}
	}
	printf("Pozice: %d\n",buffer->position);
}


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
	
	T_BUFFER buffer;
	buffer.buffer = 0;
	buffer.position = 0;
	
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
				encodeSymbol(nodes[ESC]->code,nodes[ESC]->level,&buffer,outputFile);
				std::cerr<<"Code: "<<nodes[ESC]->code<<std::endl;
				std::cerr<<"Level: "<<nodes[ESC]->level<<std::endl;
//				for(int x = 1; x <= nodes[ESC]->level; x++){
//					buffer.buffer = nodes[ESC]->code << 1;
//					buffer.position++;
//					if(buffer.position == 8)
//						printBuffer(outputFile,&buffer);
//				}
			}
			std::cerr<<"Přidávám: "<<ch<<std::endl;
			// Vytvorim novy node
			createNewNode(nodes,ch);
			

			
			encodeSymbol(ch,CHAR_SIZE,&buffer,outputFile);
//			for(int x = 1; x <= 8; x++){
//				buffer.buffer = ch << 1;
//				buffer.position++;
//				if(buffer.position == 8)
//					printBuffer(outputFile,&buffer);
//			}			
			
			std::cerr<<"Buff znak: "<<std::bitset<8>(ch)<<std::endl;
			
		} else {
			std::cerr<<"Mělo by být zapsáno: "<<ch<<std::endl;
//			char *buffer = new char[nodes[ch]->level+1];
			std::cerr<<"Code: "<<nodes[ch]->code<<std::endl;
			std::cerr<<"Level: "<<nodes[ch]->level<<std::endl;
			
			encodeSymbol(nodes[ch]->code,nodes[ch]->level,&buffer,outputFile);
			
//			for(int x = 1; x <= nodes[ch]->level; x++){
//				buffer.buffer = nodes[ch]->code << 1;
//				buffer.position++;
//				if(buffer.position == 8)
//					printBuffer(outputFile,&buffer);
//			}
		}
		// Aktualizuju strom
		updateTree(ch,nodes);
		ahed->uncodedSize++;
	}
	
	std::cerr<<"Chci vložit poslední eof"<<std::endl;
	
	printf("Pred:Pozice: %d\n",buffer.position);
	std::cerr<<"Code: "<<nodes[ESC]->code<<std::endl;
	std::cerr<<"Level: "<<nodes[ESC]->level<<std::endl;
	// EOF
	encodeSymbol(nodes[ESC]->code,nodes[ESC]->level,&buffer,outputFile);
//	for(int x = 1; x <= nodes[ESC]->level; x++){
//		buffer.buffer = nodes[ESC]->code << 1;
//		buffer.position++;
//		if(buffer.position == 8)
//			printBuffer(outputFile,&buffer);
//	}
	
	printf("Out:Pozice: %d\n",buffer.position);
	
	// Zarovnani bajtu
	if(buffer.position != 0){
		while(buffer.position != CHAR_SIZE){
			buffer.buffer = (buffer.buffer << 1) + 0;
			buffer.position++;
		}
		printBuffer(outputFile,&buffer);		
	}

	
	printf("\n\n\n");
	printTree(huffmanTree);
	
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
	T_NODE_PTR huffmanTree;
	if(!treeInit(&huffmanTree))
		exit(AHEDFail);
	
	T_BUFFER buffer;
	buffer.buffer = 0;
	buffer.position = 0;
	
	bool ESCfound = true;
	int8_t currentBIT;
	
	std::map<int,T_NODE_PTR> nodes;
	nodes.insert(std::pair<int,T_NODE_PTR>(ESC,huffmanTree));
	
	T_NODE_PTR tmpNode = huffmanTree;
	
	int16_t ch;
	while((ch = fgetc(inputFile)) != EOF)
	{
		for(int x = 0; x < CHAR_SIZE; x++)
		{
			
			int8_t res = 0;          //Result
			int8_t temp = ch;     // ones complement?
			for(int y = 7; y > 0 ; y--)
			{
				res = res | (temp & 0x01);
				temp    = temp  >> 1;
				res       = res     << 1;
			}
			res = res | (temp & 0x01);			
			
//			std::cerr<<ch<<std::endl;
//			std::cerr<<"BIT: "<<((ch >> x) & 1)<<std::endl;
			std::cerr<<"BIT flip: "<<((res >> x) & 1)<<std::endl;
			
			currentBIT = (res >> x) & 1;
			
			if(ESCfound)
			{
				buffer.buffer = (buffer.buffer << 1) + ((res >> x) & 1);;
				buffer.position++;
				
				if(buffer.position == CHAR_SIZE)
				{
					std::cerr<<buffer.buffer<<std::endl;
					// Vytvorim novy node
					createNewNode(nodes,buffer.buffer);
					// Aktualizuj strom
					updateTree(buffer.buffer,nodes);
					printBuffer(outputFile,&buffer);
//					fprintf(outputFile,"%c",01100001);
					
					ESCfound = false;
					ahed->codedSize++;
				}
				continue;
			}
//			else{
			std::cerr<<"test1"<<std::endl;
				if(currentBIT == 1)
					tmpNode = tmpNode->rightChild;
				else
					tmpNode = tmpNode->leftChild;
				
			if(tmpNode == NULL){
				std::cerr<<"Prázdný storm, nazdar"<<std::endl;
				dispose(&huffmanTree);
				return AHEDFail;
			}
			
			std::cerr<<"test21"<<std::endl;
				if(tmpNode->symbol == ESC)
				{
					std::cerr<<"ESC - bude nový symbol"<<std::endl;
					ESCfound = true;
					tmpNode = huffmanTree;
				}
				else if(tmpNode->symbol > -1 && tmpNode->symbol < ESC)
				{
					std::cerr<<"Zapisuju stávající ze stromu"<<std::endl;
					updateTree(tmpNode->symbol,nodes);
//					std::cerr<<tmpNode->symbol<<std::endl;
					buffer.buffer = tmpNode->symbol;
					std::cerr<<buffer.buffer<<std::endl;
					printBuffer(outputFile,&buffer);
					
					tmpNode = huffmanTree;
				}
				
//			}			
		}
		ahed->codedSize++;
		
	}
		
	dispose(&huffmanTree);
	std::cerr<<"Dispose OK"<<std::endl;	
	return AHEDOK;
}



