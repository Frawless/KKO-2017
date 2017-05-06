/*
 * Autor: Jakub Stejskal (xstejs24)
 * Datum: 3.5. 2017
 * Soubor: ahead.h
 * Komentar:
 */ 

#ifndef __KKO_AHED_H__
#define __KKO_AHED_H__

#include <stdio.h>
#include <map>
#include <sys/types.h>
#include <inttypes.h>

#include <iostream>
#include <fstream>
#include <sstream>

#define AHEDOK 0
#define AHEDFail -1

#define ESC 256
#define NAS 257
#define SYMBOLS 257

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
	int64_t count;	 // cetnost	
	int16_t symbol;  // kod symbolu
	int16_t code;	// kod v binarnim strome
	int32_t level;	// uroven zanoreni
	int64_t rank;	// poradi vlozeni
} *T_NODE_PTR;



bool treeInit(T_NODE_PTR* root);
void dispose (T_NODE_PTR *root);

std::string getBinaryCode(int16_t n);


char *getBinaryCode (int16_t val, char *buff, int32_t sz);

int16_t parseSymbol(char symbol);
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

