/*
 * Autor: Jakub Stejskal (xstejs24)
 * Datum: 3.5. 2017
 * Soubor: main.c
 * Komentar: Soubor obsahujici main a zpracovani argumentu progrmau.
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
				break;
			case 'o':
				if((params.output = fopen(optarg,"w+")) == NULL){
					exit(AHEDFail);
				}
//				strcpy(params.outputFile, optarg);
				break;
			case 'l':
				if((params.log = fopen(optarg,"w+")) == NULL){
					exit(AHEDFail);
				}				
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
				printHelp();
				exit(AHEDOK);
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
		exit(AHEDFail);
	}
	
	if(params.encode && params.decode){
		cerr<<"getParams() - Špatný formát argumentů!"<<endl;
		printHelp();
		exit(AHEDFail);
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
	
	if(params.encode)
		AHEDEncoding(&ahed,params.input,params.output);
	else
		AHEDDecoding(&ahed,params.input,params.output);
	
	if(params.log != NULL){
		fprintf(params.log,"login = xstejs24\n");
		fprintf(params.log,"uncodedSize = %ld\n",ahed.uncodedSize);
		fprintf(params.log,"codedSize = %ld\n",ahed.codedSize);
		fclose(params.log);
	}
	
	// cisteni
	fclose(params.input);
	fclose(params.output);
	
	exit(AHEDOK);
}


	
