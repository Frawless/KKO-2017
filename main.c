/*
 * Autor: Jakub Stejskal (xstejs24)
 * Datum: 3.5. 2017
 * Soubor: main.c
 * Komentar:
 */ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <bitset>
#include <limits>

#include <iostream>
#include <fstream>
#include <sstream>

#include "ahed.h"

using namespace std;    // Or using std::string

//struktura pro jméno interface
typedef struct{
	int ErrParam;
	int optindNumber;
	FILE *input;
	FILE *output;
	FILE *log;
	bool encode;
	bool decode;
} PARAMS;

/**
 * Funkce pro vypsání nápvoědy.
 */
void printHelp()
{
	cerr<<"Nápověda:"<<endl;
	cerr<<"Spuštění: ahead [-i inputFile] [-o outputFile] [-l logFile] [-h] -c|x"<<endl;
	cerr<<"-i název souboru pro vstup"<<endl;
	cerr<<"-o název souboru pro výstup"<<endl;
	cerr<<"-l název souboru pro výstupní analýzu"<<endl;
	cerr<<"-c - encoding"<<endl;
	cerr<<"-x - decoding"<<endl;
	cerr<<"-h nápověda"<<endl;
}


/**
 * Funkce ověří parametry z příkazové řádky
 * @param argc počet argumentů
 * @param argv pole argumentů
 * @param argInt struktura s názvem interface pro spuštění
 **/
PARAMS getParams (int argc, char *argv[], PARAMS params)
{
	int c;
	//ověření správnosti a počtu argumentů
	while((c = getopt(argc,argv, "i:o:l:cxh")) != -1)
	{
		//parametr i + interface_name
		switch(c)
		{
			case 'i':
//				strcpy(params.inputFile, optarg);
				if((params.input = fopen(optarg,"r")) == NULL){
					exit(AHEDFail);
				}
				cerr<<"Otevřen soubor"<<endl;
				break;
			case 'o':
//				strcpy(params.outputFile, optarg);
				break;
			case 'l':
//				strcpy(params.logFile, optarg);
				break;
			case 'c':
				params.encode = true;
				params.ErrParam++;
				break;
			case 'x':
				params.decode = true;
				params.ErrParam++;
				break;
			case 'h':
				//TODO - print help;
				printHelp();
				exit(EXIT_SUCCESS);
				break;				
			default:
				params.ErrParam = -1;
				//cerr<<"getParams() - Bad Argument Format!\nUsage: ahead [-i inputFile] [-o outputFile] [-l logFile] [-h] -c|x"<<endl;
				break;
		}
	}

	params.optindNumber = optind;
	
	if(params.ErrParam != 0){
		cerr<<"getParams() - Špatný formát argumentů!"<<endl;
		printHelp();
	}
		

	// vrací se struktura se zpracovanými parametry
	return params;
}

int main(int argc, char **argv)
{
	PARAMS params = {-1,-1,stdin,stdout,NULL,false,false};
	params = getParams(argc,argv,params);
	
	tAHED ahed;
	ahed.codedSize = 0;
	ahed.uncodedSize = 0;
	
	AHEDEncoding(&ahed,params.input,stdout);
	
//	cerr<<"Test: "<<bitset<4>(test)<<endl;
//	
//	int16_t test2 = (test << 1) + 1;
//	cerr<<"Test: "<<bitset<5>(test2)<<endl;
	
	
	fclose(params.input);
//	delete[] bufu_charf;
	
	exit(AHEDOK);
}


	
