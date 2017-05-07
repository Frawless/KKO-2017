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
#define CHAR_SIZE 8

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
	int64_t code;	// kod v binarnim strome
	int64_t level;	// uroven zanoreni
	int64_t rank;	// poradi vlozeni
} *T_NODE_PTR;

// Struktura po buffer udržující 8 bitů pro výpis
typedef struct{
	int8_t buffer;
	int8_t position;
} T_BUFFER;

/**
 * Funkce pro inicializaci stromu.
 * @param root ukazatel na strom
 */
bool treeInit(T_NODE_PTR* root);

/**
 * Funkce pro smazání stromu z paměti.
 * @param root - ukazatel na strom
 */
void dispose (T_NODE_PTR *root);

/**
 * Pomocna funkce pro vypsani stromu
 * @param tree - vytvoreny strom strom
 */
void printTree(T_NODE_PTR tree);

/**
 * Funkce pro vytvoreni noveho uzlu ve stromu pro vlozeni nove nacteneho symbolu
 * @param lists - ukazatel na mapu stromu (obsahuje cely strom)
 * @param symbol - nacteny symbol
 */
void createNewNode(std::map<int,T_NODE_PTR> &lists, int16_t symbol);

/**
 * Funkce pro aktualizaci Huffmanova stromu. Hlavni myslena prevzata z: http://www.stringology.org/DataCompression/fgk/index_cs.html
 * Funkce v pripade potreby aktualizuje HUffmanuv kod tak, aby odpovidal vlastnostem algoritmu.
 * @param symbol - ukazatel na mapu stromu (obsahuje cely strom)
 * @param lists - nacteny symbol
 */
void updateTree(int64_t symbol, std::map<int,T_NODE_PTR> &lists);

/**
 * Funkce pro aktualizaci presunutych casti stromu.
 * @param tree - cely strom pripadne jednotlive podstromy
 */
void updateCode(T_NODE_PTR *tree);

/**
 * Funkce pro vypsani bufferu bitu do souboru.
 * @param file - vystupni soubor
 * @param buffer - buffer obshaujici kodovy znak
 */
void printBuffer(FILE *file, T_BUFFER *buffer);

/**
 * Funkce pro naplneni bufferu codem vlozeneho znaku.
 * @param symbol - kod znaku
 * @param level - uroven znoareni ve stromu (slouzi k identifikaci spravneho poctu bitu)
 * @param buffer - buffer obsahujici vystupni posloupnost bitu
 * @param outputFile - vystupni soubor
 */
void encodeSymbol(tAHED *ahed,int64_t symbol, int64_t level, T_BUFFER *buffer, FILE *outputFile);

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

