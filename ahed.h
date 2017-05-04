/*
 * Autor: Jakub Stejskal (xstejs24)
 * Datum: 3.5. 2017
 * Soubor: ahead.h
 * Komentar:
 */ 

#ifndef __KKO_AHED_H__
#define __KKO_AHED_H__

#include <stdio.h>
#include <sys/types.h>

#define AHEDOK 0
#define AHEDFail -1

#define ESC 0

/* Datovy typ zaznamu o (de)kodovani */
typedef struct{
	/* velikost nekodovaneho retezce */
	int64_t uncodedSize;
	/* velikost kodovaneho retezce */
	int64_t codedSize;
} tAHED;

// Struktura pro strom
typedef struct T_NODE {
	struct T_NODE *parent;
	struct T_NODE *leftChild;
	struct T_NODE *rightChild;
	int64_t count;
	int64_t symbol;
	int64_t code;
} *T_NODE_PTR;



bool treeInit(T_NODE_PTR* root);
void dispose (T_NODE_PTR *root);


/* Nazev:
 *   AHEDEncoding
 * Cinnost:
 *   Funkce koduje vstupni soubor do vystupniho souboru a porizuje zaznam o kodovani.
 * Parametry:
 *   ahed - zaznam o kodovani
 *   inputFile - vstupni soubor (nekodovany)
 *   outputFile - vystupn� soubor (kodovany)
 * Navratova hodnota: 
 *    0 - kodovani probehlo v poradku
 *    -1 - p�i kodovani nastala chyba
 */
int AHEDEncoding(tAHED *ahed, FILE *inputFile, FILE *outputFile);


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
 *    -1 - p�i dekodovani nastala chyba
 */
int AHEDDecoding(tAHED *ahed, FILE *inputFile, FILE *outputFile);

#endif

