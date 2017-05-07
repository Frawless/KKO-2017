/*
 * Autor: Jakub Stejskal (xstejs24)
 * Datum: 3.5. 2017
 * Soubor: ahead.c
 * Komentar: Knihovna obsahující potřebné funkce pro kódování a dekódování pomocí Adaptivní Huffmanovy metody.
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
	// Inicializacni hodnoty korene
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

/**
 * Pomocna funkce pro vypsani stromu
 * @param tree - vytvoreny strom strom
 */
void printTree(T_NODE_PTR tree)
{
	if(tree == NULL)
		return;
	// Vypis rootu
	std::cerr<<"\t\t\ts: "<<tree->symbol<<" count: "<<tree->count<<" rank: "<<tree->rank<<" l: "<<tree->level<<" c: "<<std::bitset<8>(tree->code)<<std::endl;
	// Levy potomek
	if(tree->leftChild != NULL)
		std::cerr<<"s: "<<tree->leftChild->symbol<<" count: "<<tree->leftChild->count<<" rank: "<<tree->leftChild->rank<<" l: "<<tree->leftChild->level<<" c: "<<std::bitset<8>(tree->rightChild->code)<<std::endl;
	// pravy potomek
	if(tree->rightChild != NULL)
		std::cerr<<"\t\t\t\t\t\ts: "<<tree->rightChild->symbol<<" count: "<<tree->rightChild->count<<" rank: "<<tree->rightChild->rank<<" l: "<<tree->rightChild->level<<" c: "<<std::bitset<8>(tree->rightChild->code)<<std::endl;
	
	if(tree->leftChild != NULL)
		printTree(tree->leftChild);
	
	if(tree->rightChild != NULL)
		printTree(tree->rightChild);
}

/**
 * Funkce pro vytvoreni noveho uzlu ve stromu pro vlozeni nove nacteneho symbolu
 * @param lists - ukazatel na mapu stromu (obsahuje cely strom)
 * @param symbol - nacteny symbol
 */
void createNewNode(std::map<int,T_NODE_PTR> &lists, int16_t symbol)
{
	// Alokace novych synu
	T_NODE_PTR newNodeLeft = (T_NODE_PTR)malloc(sizeof(T_NODE));
	T_NODE_PTR newNodeRight = (T_NODE_PTR)malloc(sizeof(T_NODE));
	// Ziskani listu, ktery budeme rozvetvovat (VZDY ESC list))
	T_NODE_PTR root = lists[ESC];
	// Osetrenialokace
	if(newNodeLeft == NULL || newNodeRight == NULL)
	{
		free(newNodeLeft);
		free(newNodeRight);
		std::cerr<<"Nedostatek místa pro alokaci!"<<std::endl;
		exit(AHEDFail);
	}
	
	root->symbol = NAS;
	root->leftChild = newNodeLeft;
	root->rightChild = newNodeRight;
	// Nastaveni leveho syna (V leve (nulove) vetvy je vzdy ESC list)	
	newNodeLeft->parent = newNodeRight->parent = root;
	newNodeLeft->leftChild = newNodeRight->leftChild = newNodeLeft->rightChild = newNodeRight->rightChild = NULL;
	newNodeLeft->symbol = ESC;						// znak ESC
	newNodeLeft->count = 0;							// cetnost znaku
	newNodeLeft->code = (root->code << 1) + 0;		// Nastaveni kodu znaku, po aktualizaci stromu je nutne kody aktualizovat (viz. fce updateCode v aktualizaci stromu)
	newNodeLeft->rank = root->rank + 2;
	
	newNodeRight->symbol = symbol;					// nacteny znak
	newNodeRight->count = 0;						// cetnost znaku
	newNodeRight->code = (root->code << 1) + 1;		// Nastaveni kodu znaku, po aktualizaci stromu je nutne kody aktualizovat (viz. fce updateCode v aktualizaci stromu)	
	newNodeRight->rank = root->rank + 1;

	// Zvyseni urovne zanoreni ve stromu, aktualizovano stejne jako polozka code
	newNodeLeft->level = newNodeRight->level = newNodeLeft->parent->level + 1;
	
	// Vlozeni ukazatelu na vznikle listy do mapy, neni nutne pak prohledavat cely strom
	lists.insert(std::pair<int,T_NODE_PTR>(symbol,newNodeRight));
	lists[ESC] = newNodeLeft;
	lists.insert(std::pair<int,T_NODE_PTR>(NAS+root->rank,root));	
}

/**
 * Funkce pro aktualizaci Huffmanova stromu. Hlavni myslena prevzata z: http://www.stringology.org/DataCompression/fgk/index_cs.html
 * Funkce v pripade potreby aktualizuje HUffmanuv kod tak, aby odpovidal vlastnostem algoritmu.
 * @param symbol - ukazatel na mapu stromu (obsahuje cely strom)
 * @param lists - nacteny symbol
 */
void updateTree(int64_t symbol, std::map<int,T_NODE_PTR> &lists)
{
	// Ziskani naposled vlozeneho listu
	T_NODE_PTR last = lists[symbol];
	// Pomocne promenne
	T_NODE_PTR change;
	T_NODE_PTR tmp;
	int64_t tmpRank;
	
	// Prochazim od listu ke koreni
	while(last != NULL)
	{
		change = last;
		std::map<int,T_NODE_PTR>::iterator it;
		// Iteruju pres vsechny vlozene uzly
		for(it = lists.begin(); it != lists.end(); it++)
		{
			//  if (existuje uzel U1 se stejným ohodnocenim a vyssim poradim) then
			if(last->count == it->second->count && change->rank > it->second->rank)
			{
				change = it->second;
			}
		}
		// Update stromu, prohozeni ukazatelu a zvyseni urovne
		if(last->parent != change && last != change)
		{
			// Ulozeni ukazatele na rodice posledne zpracovavaneho
			// Pokud bychom ulozili ukazatel na posledne zpracovaneho tak se k puvodnimu rodici uz nedostaneme
			tmp = last->parent;
			// Ulozeni poradi vlozeni
			tmpRank = last->rank;
			// Vymena ukazatelu u rodiceu
			if(last->parent->rightChild == last){	// Pokud jsem pravy syn sveho rodice
				last->parent->rightChild = change;
			}
			else{
				last->parent->leftChild = change;	// Pokud jsem levy syn sveho rodice
			}
			// Vymena ukazatelu u rodiceu
			if(change->parent->rightChild == change){	// Pokud jsem pravy syn sveho rodice
				change->parent->rightChild = last;
			}
			else{
				change->parent->leftChild = last;	// Pokud jsem levy syn sveho rodice
			}
			// Vymena ukazatelu na rodice 
			last->parent = change->parent;
			last->rank = change->rank;
			
			change->parent = tmp;
			change->rank = tmpRank;

			// Aktualizace noveho kodu znaku dle pozie ve stromu a jeho urovne
			updateCode(&last->parent);
			updateCode(&change->parent);
		}
		
		last->count++;
		last = last->parent;
	}
}

/**
 * Funkce pro aktualizaci presunutych casti stromu.
 * @param tree - cely strom pripadne jednotlive podstromy
 */
void updateCode(T_NODE_PTR *tree)
{
	T_NODE_PTR root = (*tree);
	if(root->parent == NULL){
		root->code = 0;
		root->level = 0;
	}
	// levy syn (nulova vetev - ESC))
	if(root->leftChild != NULL){
		root->leftChild->code = (root->leftChild->parent->code << 1) + 0;
		root->leftChild->level = root->leftChild->parent->level + 1;
		updateCode(&root->leftChild);
	}
	// pravy syn
	if(root->rightChild != NULL){
		root->rightChild->code = (root->rightChild->parent->code << 1) + 1;
		root->rightChild->level = root->rightChild->parent->level + 1;	
		updateCode(&root->rightChild);
	}
}


/**
 * Funkce pro vypsani bufferu bitu do souboru.
 * @param file - vystupni soubor
 * @param buffer - buffer obshaujici kodovy znak
 */
void printBuffer(FILE *file, T_BUFFER *buffer)
{
	fprintf(file,"%c",buffer->buffer);
	buffer->buffer = 0;
	buffer->position = 0;
}

/**
 * Funkce pro naplneni bufferu codem vlozeneho znaku.
 * @param symbol - kod znaku
 * @param level - uroven znoareni ve stromu (slouzi k identifikaci spravneho poctu bitu)
 * @param buffer - buffer obsahujici vystupni posloupnost bitu
 * @param outputFile - vystupni soubor
 */
void encodeSymbol(tAHED *ahed,int64_t symbol, int64_t level, T_BUFFER *buffer, FILE *outputFile)
{
	// Prochazime kod symbolu a ulozime jen pouzivanou cast
	// Napr: funkce dostane 0000....0000 0110  a uroven zanoreni je 3, prebytecne znaky se oriznou a buffer dostane pouze 110
	for(int x = 1; x <= level; x++){
		buffer->buffer = (buffer->buffer << 1) + ((symbol >> (level-x)) & 1);
		buffer->position++;
		// Vypis bufferu pokud je plny
		if(buffer->position == 8){
			ahed->codedSize++;
			printBuffer(outputFile,buffer);
		}
	}
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
	nodes.insert(std::pair<int,T_NODE_PTR>(ESC,huffmanTree));
	
	int16_t ch;
	while((ch = fgetc(inputFile)) != EOF)
	{
		if (nodes.find(ch) == nodes.end()){
			// Znak jeste neni ve stromu
			if(ahed->uncodedSize != 0){
				// Pokud nezpracovavam prvni znak tak vlozim ESC 
				encodeSymbol(ahed,nodes[ESC]->code,nodes[ESC]->level,&buffer,outputFile);
			}
			// Vytvorim novy node se zpracovavanym znakem
			createNewNode(nodes,ch);
			// Vypisu znak
			encodeSymbol(ahed,ch,CHAR_SIZE,&buffer,outputFile);
		}else{	// Pokud znak jiz je ve strome obsazen
			// Vypisu znak
			encodeSymbol(ahed,nodes[ch]->code,nodes[ch]->level,&buffer,outputFile);
		}
		// Aktualizuju strom a zvysim pocet pro statistiky
		updateTree(ch,nodes);
		ahed->uncodedSize++;
	}
	// Vlozeni EOF ve forme ESC
	encodeSymbol(ahed,nodes[ESC]->code,nodes[ESC]->level,&buffer,outputFile);
	
	// Zarovnani bajtu (V pripade, ze obsah posledniho bufferu neni plny nebo prazdny)
	if(buffer.position != 0){
		while(buffer.position != CHAR_SIZE){
			buffer.buffer = (buffer.buffer << 1) + 0;
			buffer.position++;
		}
		printBuffer(outputFile,&buffer);		
		ahed->codedSize++;
	}
	// Smazani stromu a ukonceni
	dispose(&huffmanTree);
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
	// Vytvoreni stromu (zatim jen koren se znakem ESC)
	T_NODE_PTR huffmanTree;
	if(!treeInit(&huffmanTree))
		exit(AHEDFail);
	// Buffer pro plneni znaky
	T_BUFFER buffer;
	buffer.buffer = 0;
	buffer.position = 0;
	
	// Pomocne promenne
	bool ESCchar = true;
	int8_t currentBIT;
	// Mapa se stromem pro snazsi pristup
	std::map<int,T_NODE_PTR> nodes;
	nodes.insert(std::pair<int,T_NODE_PTR>(ESC,huffmanTree));
	// Pomocny ukazatel na nodestromu
	T_NODE_PTR tmpNode = huffmanTree;
	
	int16_t ch;
	// Nacitani vstupu dokud nejsou na EOF
	while((ch = fgetc(inputFile)) != EOF)
	{
		for(int x = 0; x < CHAR_SIZE; x++)
		{
			// ##################################
			// Obraceni poradi bitu v nactenem znaku (potreba pro spravne zpracovani)
			int8_t res = 0;
			int8_t temp = ch;
			for(int y = 7; y > 0 ; y--)
			{
				res = res | (temp & 0x01);
				temp = temp >> 1;
				res = res << 1;
			}
			res = res | (temp & 0x01);			
			// ##################################
			
			// Nastaveni aktualniho BITu
			currentBIT = (res >> x) & 1;
			// Zpracovani noveho znaku (uplne prvni znak nebo znak po ESC)
			if(ESCchar)
			{
				// Postupne plneni bufferu
				buffer.buffer = (buffer.buffer << 1) + ((res >> x) & 1);;
				buffer.position++;
				// Buffer je plny a vypisujeme
				if(buffer.position == CHAR_SIZE)
				{
					// Vytvorim novy node
					createNewNode(nodes,buffer.buffer);
					// Aktualizuj strom
					updateTree(buffer.buffer,nodes);
					// Vypis
					printBuffer(outputFile,&buffer);
					
					ESCchar = false;
//					ahed->codedSize++;
					ahed->uncodedSize++;
				}
			}
			else{
				// Posunuti ve strome na zaklade zpracovavaneho bitu
				if(currentBIT == 1)
					tmpNode = tmpNode->rightChild;
				else
					tmpNode = tmpNode->leftChild;

				// Osetreni nekonzistence
				if(tmpNode == NULL){
					dispose(&huffmanTree);
					return AHEDFail;
				}

				// Nalezen ESC znak, nastavime priznak avratime se do korene
				if(tmpNode->symbol == ESC)
				{
					ESCchar = true;
					tmpNode = huffmanTree;
				}
				// Nalezeny znak neni ESC, update stromu a jeho vypis
				else if(tmpNode->symbol > -1 && tmpNode->symbol < ESC)
				{
					updateTree(tmpNode->symbol,nodes);
					buffer.buffer = tmpNode->symbol;
					printBuffer(outputFile,&buffer);
					ahed->uncodedSize++;
					tmpNode = huffmanTree;
				}				
			}
		}
		ahed->codedSize++;
	}
	// Uklid a ukonceni
	dispose(&huffmanTree);
	return AHEDOK;
}



