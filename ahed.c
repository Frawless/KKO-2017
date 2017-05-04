/*
 * Autor: Jakub Stejskal (xstejs24)
 * Datum: 3.5. 2017
 * Soubor: ahead.c
 * Komentar: 
 */ 

#include <cstdlib>
#include "ahed.h"


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
    }
	
    return ;
}

bool updateTree(T_NODE_PTR *root, int64_t symbol)
{
	if((*root)->symbol == symbol)
	{
		(*root)->count++;
		if((*root)->parent != NULL)
		{
			(*root)->parent->count++;
		}
	}
	// TODO - aby to bylo OK vzhledem k četnostem
	else if((*root)->leftChild != NULL)
		updateTree(&((*root)->leftChild), symbol);
	else if((*root)->rightChild != NULL)
		updateTree(&((*root)->rightChild), symbol);
	else
	{
		(*root)->leftChild = (T_NODE_PTR)malloc(sizeof(T_NODE));
		(*root)->rightChild = (T_NODE_PTR)malloc(sizeof(T_NODE));

		// Osetreni nemoznosti alokace pameti
		if((*root)->rightChildt == NULL || (*root)->leftChild == NULL)
			return false;	
		
		(*root)->leftChild->symbol = ESC;
		(*root)->leftChild->parent = (*root);
		(*root)->rightChild->count = 0;
		(*root)->leftChild->leftChild = NULL;
		(*root)->leftChild->rightChild = NULL;
		// parent code + 0
		(*root)->leftChild->code = (*root)->code + 1; 
		
		
		(*root)->rightChild->symbol = symbol;
		(*root)->rightChild->parent = (*root);
		(*root)->rightChild->count = 0;
		(*root)->rightChild->rightChild = NULL;
		(*root)->rightChild->leftChild = NULL;
		// parrent code + 1
		(*root)->rightChild->code = (*root)->code + 1;
	}
	
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



