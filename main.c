#include "headers.h"


int main( int argc, char* argv[]){

	FILE *fp;
	char line[1024];
	//char* newsym;
	//char* nextoken;
	//char* thirdToken;
	char fullline[1024];
	int loCounter;
	int lineNum = 0;
	int startFound = 0;
	int loArr[1024];
	int loEle = 0;

	if ( argc != 2 ) {
	printf("ERROR: Usage: %s filename\n", argv[0]);
	return 0;
	}


	fp = fopen( argv[1], "r");

	if ( fp == NULL ) {
	printf("ERROR: %s could not be opened for reading,\n", argv[1] );
	return 0;
	}

	char* newsym = malloc(  1024 * sizeof(char)             ); //allocating space
	memset( newsym, '\0', 1024 * sizeof(char) );	//zeroing it out
	char* nextoken = malloc(  1024 * sizeof(char)             );
	memset( nextoken, '\0', 1024 * sizeof(char) );
	char* thirdToken = malloc(  1024 * sizeof(char)             );
	memset( thirdToken, '\0', 1024 * sizeof(char) );
	memset(SymbolTable, '\0', 1024 * sizeof(struct symbol*));


	while(  fgets( line , 1024 , fp ) != NULL   ) {

		strcpy( fullline, line );
		if ( line[0] == 35) {
			//printf("COMMENT:%s", line );
			lineNum++;

			continue;
		}

		if (  (line[0] >= 33 ) && ( line[0] <= 90 )   )  {

			//printf("b4 first strtok\n");
			newsym = strtok( line, " \r\t\n");
			//printf("FULL LINE:%s", fullline );
			//printf("NEW SYMBOL : %s\n",newsym);
			lineNum++;
			//error check test 4
			if(IsADirective(newsym) == 1){
				printf("\nERROR:\n\n%s\nLine %d, Symbol with a directive name.\n\n", fullline, lineNum);
				fclose(fp);
				return 0;
			}
			//symbol has instruction name check
			if(IsAnInstruction(newsym) == 1){
                                printf("\nERROR:\n\n%s\nLine %d, Symbol with an instruction name.\n\n", fullline, lineNum);
                                fclose(fp);
                                return 0;
                        }
			//edit error format
			if ( IsAValidSymbol(newsym) == 0 ) {
				printf("\nERROR:\n\n%s\nLine %d, Invalid symbol found.\n\n", fullline, lineNum);
                                fclose(fp);
                                return 0;
                        }
			//symbol has valid length, error check 9
			else{
				for(int i = 0; i < strlen(newsym); i++){
					if(newsym[i] == 32 ||//space
					newsym[i] == 36 ||//$
					newsym[i] == 33 ||//!
					newsym[i] == 61 ||//=
					newsym[i] == 43 ||//+
					newsym[i] == 45 ||//-
					newsym[i] == 40 ||//(
					newsym[i] == 41 ||//)
					newsym[i] == 64 ||//@
					newsym[i] == 37){//%
						printf("\nERROR:\n\n%s\nLine %d, Invalid symbol found.\n\n", fullline, lineNum);
                                                fclose(fp);
                                                return 0;
					}
					else{
						continue;
					}
				}
			}

			nextoken = strtok( NULL, " \t\n"  );
			//printf("NEXT TOKEN ON LINE IS %s\n", nextoken );
			//if new sym and directive
			if(IsADirective(nextoken) == 1){
				//printf("%s is a valid directive.\n\n", nextoken);
				//if else branch of directives for location counter movement
				//no need to check if start exists right now
				if(strcmp(nextoken, "START") == 0){
					startFound = 1;
					thirdToken = strtok(NULL, " \r\t\n");
					//error check 5
					int maxMem = strtol("8000", NULL, 16);
					if(strtol(thirdToken, NULL, 16) >= maxMem){
						printf("\nERROR:\n\n%s\nLine %d, SIC program starts outside of memory.\n\n", fullline, lineNum);
						fclose(fp);
						return 0;
					}
					loCounter = strtol(thirdToken, NULL, 16);
					loArr[loEle] = loCounter;
					loEle++;
					//finish lo counter array for each lo counter update and switch symtab in progLen while loop w/ the lo array
					addSymbol(SymbolTable, loCounter, lineNum, newsym);
				}
				//adding symbols after start
				else{
					//save loCounter and symbol into new struct
					thirdToken = strtok(NULL, "\t\n");
					//error check 2
					//printf("\n\n\nelse entered: %s\n\n\n", newsym);
					if(symbolExists(SymbolTable, newsym) != 0){ //newsym is just a string at this point
						printf("\nERROR:\n\n%s\nLine %d, Duplicate symbol found.\n\n", fullline, lineNum);
						fclose(fp);
						return 0;
					}
					addSymbol(SymbolTable, loCounter, lineNum, newsym);
					loCounter = updateLocation(nextoken, thirdToken, loCounter, fullline, lineNum);
					loArr[loEle] = loCounter;
                                        loEle++;
					//check test 10 after every usage of  update location
					int maxMem = strtol("8000", NULL, 16);
					if(loCounter >= maxMem){
						printf("\nERROR:\n\n%s\nLine %d, Program exceeded memory.\n\n", fullline, lineNum);
                                                fclose(fp);
                                                return 0;
					}

				}

			}
			//if new sym and instruction
			else if(IsAnInstruction(nextoken) == 1){
				//printf("%s is a valid instruction.\n\n", nextoken);
				//move location counter by 3bytes
				if(symbolExists(SymbolTable, newsym) != 0){ //newsym is just a string at this point
                                                printf("\nERROR:\n\n%s\nLine %d, Duplicate symbol found.\n\n", fullline, lineNum);
                                                fclose(fp);
                                                return 0;
                                }
				addSymbol(SymbolTable, loCounter, lineNum, newsym);
				//error check 10
				loCounter = updateLocation(NULL, NULL, loCounter, fullline, lineNum);
				loArr[loEle] = loCounter;
                                loEle++;
				int maxMem = strtol("8000", NULL, 16);
                                if(loCounter >= maxMem){
                                	printf("\nERROR:\n\n%s\nLine %d, Program exceeded memory.\n\n", fullline, lineNum);
                                        fclose(fp);
                                        return 0;
                                }
			}
			else{
				if(IsADirective(nextoken) == 0){
					printf("\nERROR. INVALID DIRECTIVE FOUND ON LINE: %d\n\n", lineNum);
                                	fclose(fp);
                                	return 0;
				}
				else{
					printf("\nERROR. INVALID INSTRUCTION FOUND ON LINE: %d\n\n", lineNum);
                                        fclose(fp);
                                        return 0;
				}
			}


			continue;
		}
		//try and condense this
		//printf("b4 second if\n");
		else if(((line[1] >= 65 ) && ( line[1] <= 90))){
			lineNum++;
			nextoken = strtok( line, " \t\n"  );
			//printf("\nFULL LINE:%s", fullline );
                        //printf("FIRST TOKEN ON LINE IS %s\n", nextoken );
			if(IsADirective(nextoken) == 1){
                                //printf("%s is a valid directive.\n\n", nextoken);
                                //if else branch of directives for location counter movement
				//error check 10
				loCounter = updateLocation(nextoken, NULL, loCounter, fullline, lineNum);
                        	loArr[loEle] = loCounter;
                                loEle++;
				int maxMem = strtol("8000", NULL, 16);
                                if(loCounter >= maxMem){
                                	printf("\nERROR:\n\n%s\nLine %d, Program exceeded memory.\n\n", fullline, lineNum);
                                        fclose(fp);
                                        return 0;
                                }
			}
                        else if(IsAnInstruction(nextoken) == 1){
                                //printf("%s is a valid instruction.\n\n", nextoken);
                                //move location counter by 3bytes
				//error check 10
				loCounter = updateLocation(nextoken, NULL, loCounter, fullline, lineNum);
				loArr[loEle] = loCounter;
                                loEle++;
				int maxMem = strtol("8000", NULL, 16);
                                if(loCounter >= maxMem){
                                	printf("\nERROR:\n\n%s\nLine %d, Program exceeded memory.\n\n", fullline, lineNum);
                                        fclose(fp);
                                        return 0;
                                }
                        }
                        else{
                                if(IsADirective(nextoken) == 0){
					printf("%s",nextoken);
                                        printf("\nERROR. INVALID DIRECTIVE FOUND ON LINE: %d\n\n", lineNum);
                                        fclose(fp);
                                        return 0;
                                }
                                else{
                                        printf("\nERROR. INVALID INSTRUCTION FOUND ON LINE: %d\n\n", lineNum);
                                        fclose(fp);
                                        return 0;
                                }
                        }
			continue;
		}
		//edit this to if else and add more specifics i.e line[0] == \n || \r
		else if(line[0] == '\n' || line[0] == '\r' || line[0] == '\t'){
			printf("\nERROR:\n\n%s\nLine %d, Blank line found.\n\n", fullline, lineNum+1);
			fclose(fp);
			return 0;
		}



		//printf("%s", line );


	}//end while
	//error check 25 missing start
	if(startFound == 0){
		printf("\nERROR:\n\nProgram missing START directive.\n\n");
		fclose(fp);
		return 0;
	}

	//print symbol table
//	printf("Symbol Table:\n");
//	int i = 0;
//	while(SymbolTable[i+1] != NULL){
//		printf("%s\t%X\n",SymbolTable[i]->Name,SymbolTable[i]->Address);
//		i++;
//	}//end while

		//--------set up location counter with valid increments
		//--------create symbol array (learn about type def struct)
                //--------output symbol and location


		//--------work for valid SIC file
		//--------check for duplicate symbols (store new symbols into array and check while updating)
		//--------check for invalid hex
		//--------check if symbols have a directive name
		//--------check if program starts outside of memory (>8000 in hex)
		//--------work for valid long byte constant
		//--------check for blank lines
		//--------check if word constant exceeds 24bit limit (2^23 = +/-8388608)
		//--------check for valid symbol name
		//--------check if program exceeds sic memory


/************************************************************************
 *									*
 *------------------------------Pass 2----------------------------------*
 *									*
 ************************************************************************/

	//getting length of program
/*	i = 0;
	while(loArr[i+1] != '\0'){
		i++;
	}
*/
	int proLen = (loArr[loEle-1] - loArr[0]);
	//printf("\n\n%X\n\n", proLen);

	//mem sets and allos
	char* hRec = malloc(1024 * sizeof(char));
	char tRec[1024][1024]; //= malloc(1024 * sizeof(char*));
	char mRec[1024][1024]; //= malloc(1024 * sizeof(char*));
	char* eRec = malloc(1024 * sizeof(char));
        memset( hRec, '\0', 1024 * sizeof(char) );
	//memset( tRec, '\0', 1024 * sizeof(char*) );
	//memset( mRec, '\0', 1024 * sizeof(char*) );
	memset( eRec, '\0', 1024 * sizeof(char) );

	rewind(fp);

	//variable to keep track of t records
	int i = 0;
	char* dirInst = malloc(1024 * sizeof(char));
	memset( dirInst, '\0', 1024 * sizeof(char) );
	char* tokThird = malloc(1024 * sizeof(char));
        memset( tokThird, '\0', 1024 * sizeof(char) );

	while(  fgets( line , 1024 , fp ) != NULL   ) {
		//if comment
		if ( line[0] == 35) {
			continue;
		}//end if
		//if label
		if (  (line[0] >= 33 ) && ( line[0] <= 90 )   )  {

			strtok( line, " \r\t\n");
			dirInst = strtok( NULL, " \r\t\n");

			//directive start (header record)
			if(strcmp(dirInst, "START") == 0){
				hRec = strcat(hRec, "H");
				hRec = strcat(hRec, SymbolTable[0]->Name);
				char* extraSpace = "      ";
				hRec = strncat(hRec, extraSpace, (6 - strlen(SymbolTable[0]->Name)));
				char buff[1024];
				snprintf(buff, sizeof(buff), "%06X", SymbolTable[0]->Address);
				hRec = strcat(hRec, buff);
				snprintf(buff, sizeof(buff), "%06X", proLen);
				hRec = strcat(hRec, buff);
				//printf("\n\n%s\n",hRec);
				//printf("Start location: %X\n", loArr[i]);
				continue;
			}//end if

			//directive word (t record)
			else if(strcmp(dirInst, "WORD") == 0){
				strcat(tRec[i], "T");
				char buff[1024];
				snprintf(buff, sizeof(buff), "%06X", loArr[i]);
				strcat(tRec[i], buff);
				//length of word
				strcat(tRec[i], "03");
				tokThird = strtok( NULL, "\r\t\n");
				int wordNum = atoi(tokThird);
				snprintf(buff, sizeof(buff), "%06X",wordNum);
				strcat(tRec[i], buff);
				//printf("Word record:\n");
				//printf("%s\n",tRec[i]);
				//printf("Word location: %X\n", loArr[i]);
				i++;
				continue;
			}//end else if
			//directive byte (t record)
			else if(strcmp(dirInst, "BYTE") == 0){

				strcat(tRec[i], "T");
				char buff[1024];
                                snprintf(buff, sizeof(buff), "%06X", loArr[i]);
				strcat(tRec[i], buff);
				char* XorC = strtok( NULL, "'\r\t\n");
				if(strcmp(XorC, "C") == 0){
					tokThird = strtok( NULL, "'\r\t\n");
					//printf("\n\n%s\n\n", tokThird);
					//int stringLen = strlen(tokThird);
					//snprintf(buff, sizeof(buff), "%02X", stringLen);
					//strcat(tRec[i], buff);
					int j = 0;
					/*while(strlen(tRec[i]) > 69){
						snprintf(buff, sizeof(buff), "%02X", tokThird[j]);
                                                strcat(tRec[i], buff);
                                                j++;
					}
					*/
					char* text = malloc(1024 * sizeof(char));
					memset( text, '\0', 1024 * sizeof(char) );
					while(strlen(text) < 60){
						if(tokThird[j] == '\0'){
							break;
						}
                                                snprintf(buff, sizeof(buff), "%02X", tokThird[j]);
                                                strcat(text, buff);
                                                j++;
                                        }
					int textLen = (strlen(text) / 2);
					snprintf(buff, sizeof(buff), "%02X", textLen);
					strcat(tRec[i], buff);
					snprintf(buff, sizeof(buff), "%s", text);
					strcat(tRec[i], buff);
					//printf("\n\nj is equal to: %d\n\n",j);
					//printf("Byte C record:\n");
					//printf("%s\n",tRec[i]);
					//if there was a cut off, do one more iteration picking up where we left off
					if(j == 30){

						strcat(tRec[i+1], "T");
                                		char buff[1024];
                                		snprintf(buff, sizeof(buff), "%06X", atoi("30"));
						strcat(tRec[i+1], buff);
						char* text = malloc(1024 * sizeof(char));
                                        	memset( text, '\0', 1024 * sizeof(char) );
						while(strlen(text) < 60){
                                                	if(tokThird[j] == '\0'){
                                                        break;
                                                	}
                                                	snprintf(buff, sizeof(buff), "%02X", tokThird[j]);
                                                	strcat(text, buff);
							j++;
						}
						textLen = (strlen(text) / 2);
                                        	snprintf(buff, sizeof(buff), "%02X", textLen);
                                        	strcat(tRec[i+1], buff);
                                        	snprintf(buff, sizeof(buff), "%s", text);
                                        	strcat(tRec[i+1], buff);
						//printf("Byte C record:\n");
                                        	//printf("%s\n",tRec[i+1]);
						i++;
						continue;
					}
					//printf("Byte C location: %X\n", loArr[i]);
					i++;
                                	continue;
				}
				//simply return the hex value given
				else if(strcmp(XorC, "X") == 0){
                                	tokThird = strtok( NULL, "'\r\t\n");
					snprintf(buff, sizeof(buff), "%02ld", (strlen(tokThird) / 2));
					strcat(tRec[i], buff);
					//printf("\n\ntok third is %s\n\n",tokThird);
					int hex = strtol(tokThird, NULL, 16);
					snprintf(buff, sizeof(buff), "%02X", hex);
					strcat(tRec[i], buff);
					//printf("Byte X record:\n");
                                        //printf("%s\n",tRec[i]);
					//printf("Byte X location: %X\n", loArr[i]);
					i++;
                                	continue;
				}

			}//end else if
			//directive end (e record)
			else if(strcmp(dirInst, "END") == 0){
				strcat(eRec, "E");
				tokThird = strtok( NULL, "'\r\t\n");
                                char buff[1024];
				int j = 0;
				while(SymbolTable[j+1] != NULL){
					if(strcmp(SymbolTable[j]->Name, tokThird) == 0){
						break;
					}
					j++;
				}
				if(SymbolTable[j+1] == NULL){
					printf("\nERROR:\n\nLabel %s was not found in Symbol Table.\n\nObject file creation stopped\n\n", tokThird);
					fclose(fp);
					exit(0);
				}
                                snprintf(buff, sizeof(buff), "%06X", SymbolTable[j]->Address);
                                strcat(eRec, buff);
				//printf("End record:\n");
                                //printf("%s\n",eRec);
				//printf("End location: %X\n", loArr[i]);
				i++;
                                continue;
			}
			//if instruction (t record)
			else if(IsAnInstruction(dirInst) == 1){
				strcat(tRec[i], "T");
                                char buff[1024];
                                snprintf(buff, sizeof(buff), "%06X", loArr[i]);
                                strcat(tRec[i], buff);
				strcat(tRec[i], "03");
				//not sure what this was about but for some reason the "E0" that instruction TD returned
				//kept coming out with 6 F's in front. I looked it up and the below code was the solution
				snprintf(buff, sizeof(buff), "%02X", getOpcode(dirInst) & 0xff);
				strcat(tRec[i], buff);
				//printf("\n\n%s\n\n", dirInst);
				tokThird = strtok( NULL, " '\r\t\n");
				//printf("\n\nTok tok third is %s\n\n",tokThird);
                                int j = 0;
                                while(SymbolTable[j+1] != NULL){
                                        if(strcmp(SymbolTable[j]->Name, tokThird) == 0){
                                                break;
                                        }
                                        j++;
                                }
                                if(SymbolTable[j+1] == NULL){
                                        printf("\nERROR:\n\nLabel %s was not found in Symbol Table.\n\nObject file creation stopped\n\n", tokThird);
                                        fclose(fp);
                                        exit(0);
                                }
                                snprintf(buff, sizeof(buff), "%04X", SymbolTable[j]->Address);
				strcat(tRec[i], buff);
				//printf("Label instruction record:\n");
                                //printf("%s\n",tRec[i]);
				//modification required (m record)
				strcat(mRec[i], "M");
                                snprintf(buff, sizeof(buff), "%06X", loArr[i] + 1);
                                strcat(mRec[i], buff);
				snprintf(buff, sizeof(buff), "04+%s", SymbolTable[0]->Name);
                                strcat(mRec[i], buff);
				//printf("Label instruction modification record:\n");
                                //printf("%s\n",mRec[i]);
				//printf("Label inst location: %X\n", loArr[i]);
				i++;
                                continue;
			}
			else if(strcmp(dirInst, "RESW") == 0 || strcmp(dirInst, "RESB") == 0){
				//printf("Reserve location: %X\n", loArr[i]);
                        	i++;
                        	continue;
                	}//end else if
		}//end if

		//if instruction with no label (t record)
		else if(((line[1] >= 65 ) && ( line[1] <= 90))){
			char* dirInst =  malloc(  1024 * sizeof(char)             );
        		memset( dirInst, '\0', 1024 * sizeof(char) );
			dirInst = strtok( line, " \r\t\n");
			char* oper =  malloc(  1024 * sizeof(char)             );
        		memset( oper, '\0', 1024 * sizeof(char) );
			oper = strtok( NULL, " ,\r\t\n");
			//prevents segfaulting at lines with only one token
			if(oper == NULL){
				strcat(tRec[i], "T");
                        	char buff[1024];
                        	snprintf(buff, sizeof(buff), "%06X", loArr[i]);
				strcat(tRec[i], buff);
				strcat(tRec[i], "03");
                        	snprintf(buff, sizeof(buff), "%02X", getOpcode(dirInst) & 0xff);
                        	strcat(tRec[i], buff);
				strcat(tRec[i], "0000");
				//printf("One token line location: %X\n", loArr[i]);
				i ++;
				continue;
			}
			strcat(tRec[i], "T");
                        char buff[1024];
                        snprintf(buff, sizeof(buff), "%06X", loArr[i]);
                        strcat(tRec[i], buff);
			strcat(tRec[i], "03");
			snprintf(buff, sizeof(buff), "%02X", getOpcode(dirInst) & 0xff);
                        strcat(tRec[i], buff);
			char* X = strtok( NULL, " ,\r\t\n");
			//regular addressing
			if(X == NULL){
				int j = 0;
                        	while(SymbolTable[j+1] != NULL){
                                	if(strcmp(SymbolTable[j]->Name, oper) == 0){
                                        	break;
                                	}
                                	j++;
                        	}
                        	if(SymbolTable[j+1] == NULL){
                               		printf("\nERROR:\n\nLabel %s was not found in Symbol Table.\n\nObject file creation stopped\n\n", oper);
                               		fclose(fp);
                               		exit(0);
                        	}
                        	snprintf(buff, sizeof(buff), "%04X", SymbolTable[j]->Address);
                        	strcat(tRec[i], buff);
				//printf("\n\noper is %s\n\n", oper);
                        	//printf("No label instruction record:\n");
                        	//printf("%s\n",tRec[i]);
                        	//modification is required (m record)
                        	strcat(mRec[i], "M");
                        	snprintf(buff, sizeof(buff), "%06X", loArr[i] + 1);
                        	strcat(mRec[i], buff);
                        	snprintf(buff, sizeof(buff), "04+%s", SymbolTable[0]->Name);
                        	strcat(mRec[i], buff);
                        	//printf("Label instruction modification record:\n");
                        	//printf("%s\n",mRec[i]);
				//printf("\n\nIterations: %d\n\n", i);
				//printf("No label instr location: %X\n", loArr[i]);
				i++;
				continue;
			}
			//indexed addressing
			else if(strcmp(X, "X") == 0){
				int j = 0;
                                while(SymbolTable[j+1] != NULL){
                                        if(strcmp(SymbolTable[j]->Name, oper) == 0){
                                                break;
                                        }
                                        j++;
                                }
                                if(SymbolTable[j+1] == NULL){
                                        printf("\nERROR:\n\nLabel %s was not found in Symbol Table.\n\nObject file creation stopped\n\n", oper);
                                        fclose(fp);
                                        exit(0);
                                }
                                snprintf(buff, sizeof(buff), "%04X", SymbolTable[j]->Address + 32768);
                                strcat(tRec[i], buff);
				//printf("No label instruction record (indexed):\n");
                                //printf("%s\n",tRec[i]);
                                //modification is required (m record)
                                strcat(mRec[i], "M");
                                snprintf(buff, sizeof(buff), "%06X", loArr[i] + 1);
                                strcat(mRec[i], buff);
                                snprintf(buff, sizeof(buff), "04+%s", SymbolTable[0]->Name);
                                strcat(mRec[i], buff);
                                //printf("%s\n",mRec[i]);
				//printf("No label indexed instr location: %X\n", loArr[i]);
				i++;
				continue;
			}//end else if

		}//end else if
/*
		else{
			i += 2;
                        continue;
		}
		//blank lines tested for in first pass through
*/
	}//end while


	fclose( fp );

	//after the storing done above, print all of the char pointers to the object file below
/*
	int l = 0;
	printf("\n\nlocation array:\n");
	while(l != i+5){
		printf("%X\n",loArr[l]);
		l++;
	}

	printf("\n\nloArr size: %d\n\n", l);
	printf("\n\niteration ammount: %d\n\n", i);
*/
	char buf[1024];
	snprintf(buf, sizeof(buf), "%s.obj", argv[1]);
	fp = fopen(buf, "w");

	//print header
	fprintf(fp, "%s\n",hRec);
	//for loop to print texts
	for(int j=0; j<i;j++){
		if(tRec[j][0] != 'T'){
			continue;
		}
		fprintf(fp, "%s\n",tRec[j]);
	}
	//for loop to print modifications
	for(int k=0; k<i;k++){
		if(mRec[k][0] != 'M'){
                        continue;
                }
                fprintf(fp, "%s\n",mRec[k]);
        }
	//print end
	fprintf(fp, "%s\n",eRec);

	fclose( fp );



}//end main

//edit this to feed line and line number into parameters for error messages
int updateLocation(char *dirInst, char* tokThird, int currCount, char fullLine[], int lineNumber){
	//if else for directives that move locounter somewhere other than 3 bytes
        if(tokThird == NULL){
                //if instruction, move locounter 3 bytes
                currCount = currCount + 3;
                return currCount;
        }
	else if(strcmp(dirInst, "BYTE") == 0){
		//parse string for c string(move loCounter stringlength) for x(move loCounter 1 for each pair)
		//5,4,2,10 easiest checks according to fred

		//printf("\n\n%s\n\n", tokThird);
		char* XorC = strtok(tokThird,"'");
		//printf("\n\n%s\n\n", XorC);
		if(strcmp(XorC, "C") == 0){
			char* byteCheck = strtok(NULL, "'");
			//printf("\n\n%s\n\n", byteCheck);
                	currCount = currCount + (int)strlen(byteCheck);
			//printf("\n\n%d\n\n", (int)strlen(byteCheck));
                	return currCount;
		}
		else if(strcmp(XorC, "X") == 0){
			char* byteCheck = strtok(NULL, " '");
			//if gate checks for proper use of X constant, should be even number
			if(strlen(byteCheck) % 2 == 0){
				int validHex = 1;
				for(int i = 0; i < strlen(byteCheck); i++){
					if(byteCheck[i] == 48 ||
					byteCheck[i] == 49 ||
					byteCheck[i] == 50 ||
					byteCheck[i] == 51 ||
					byteCheck[i] == 52 ||
					byteCheck[i] == 53 ||
					byteCheck[i] == 54 ||
					byteCheck[i] == 55 ||
					byteCheck[i] == 56 ||
					byteCheck[i] == 57 ||
					byteCheck[i] == 65 ||
					byteCheck[i] == 66 ||
					byteCheck[i] == 67 ||
					byteCheck[i] == 68 ||
					byteCheck[i] == 69 ||
					byteCheck[i] == 70 ){

						continue;
					}
					else{
						validHex = 0;
						break;
					}
				}
				if(validHex == 1){
					currCount = currCount + (strlen(byteCheck) / 2);
					return currCount;
				}
				//test error check 3
				else{
					printf("\nERROR:\n\n%s\nLine %d, Invalid hex constant found.\n\n", fullLine, lineNumber);
					exit(0);
				}
			}
			else{
				printf("ERROR. INVALID BYTE CONSTANT FOUND.");
				exit(0);
			}
		}
		else{
			printf("ERROR. INVALID BYTE CONSTANT FOUND.");
			exit(0);
		}
		char* byteCheck = strtok(NULL, " '");
		printf("\n\nHERE\n%s\n\n%d\n\n", byteCheck, (int)strlen(byteCheck));
		//only works for small byte constant. Should fail test 6 as of right now.
		currCount = currCount + (int)strlen(byteCheck);
		return currCount;
	}
	else if(strcmp(dirInst, "END") == 0){
		return currCount;
	}
	else if(strcmp(dirInst, "RESB") == 0){
		currCount = currCount + atoi(tokThird);
		return currCount;
	}
	else if(strcmp(dirInst, "RESW") == 0){
		currCount = currCount + (atoi(tokThird) * 3);
		return currCount;
	}
	else if(strcmp(dirInst, "WORD") == 0){
		//still move loCounter by 3 but implement check for word constant
		if(atoi(tokThird) > 8388607){ //<-The bit limit
			printf("\nERROR:\n\n%s\nLine %d, Word constant exceeds 24 bit limit.\n\n", fullLine, lineNumber);
			exit(0);
		}
		currCount = currCount + 3;
		return currCount;
	}
	//else means EXPORT
	else{
		currCount = currCount + 3;
		return currCount;
	}

}

void addSymbol(struct symbol* Tab[], int addr, int src, char* symName){

	int index;
	index = 0;
	struct symbol* newSym;
	newSym = malloc(sizeof(struct symbol));
	newSym->Address = addr, newSym->DefinedOnSourceLine = src, strcpy(newSym->Name, symName);
	while(Tab[index] != NULL){
        	index++;
	}
	Tab[index] = newSym;
	//printf("%s\t%X\n",Tab[index]->Name,Tab[index]->Address);

}
//returns zero or non zero//
int symbolExists(struct symbol* Tab[], char* sName){
	//printf("Symbol exists entered \n\n");
	int result = 0;
	int index = 0;
	while(Tab[index] != NULL){
        	if(strcmp(sName, Tab[index]->Name) == 0){
                	result = -1;
			//printf("Dup found in symbol exists\n");
                	break;
        	}//end if
		else{
			//printf("\nSymbol exists else entered %d times\n",index);
        		index++;
			continue;
		}
	}//end while

	return result;
}

char getOpcode(char* nem){

        if(strcmp( nem, "ADD") == 0){
                return 0x18;
        }
        else if(strcmp( nem, "AND") == 0){
                return 0x40;
        }else if(strcmp( nem, "COMP") == 0){
                return 0x28;
        }else if(strcmp( nem, "DIV") == 0){
                return 0x24;
        }else if(strcmp( nem, "J") == 0){
                return 0x3C;
        }else if(strcmp( nem, "JEQ") == 0){
                return 0x30;
        }else if(strcmp( nem, "JGT") == 0){
                return 0x34;
        }else if(strcmp( nem, "JLT") == 0){
                return 0x38;
        }else if(strcmp( nem, "JSUB") == 0){
                return 0x48;
        }else if(strcmp( nem, "LDA") == 0){
                return 0x00;
        }else if(strcmp( nem, "LDCH") == 0){
                return 0x50;
        }else if(strcmp( nem, "LDL") == 0){
                return 0x08;
        }else if(strcmp( nem, "LDX") == 0){
                return 0x04;
        }else if(strcmp( nem, "MUL") == 0){
                return 0x20;
        }else if(strcmp( nem, "OR") == 0){
                return 0x44;
        }else if(strcmp( nem, "RD") == 0){
                return 0xD8;
        }else if(strcmp( nem, "RSUB") == 0){
                return 0x4C;
        }else if(strcmp( nem, "STA") == 0){
                return 0x0C;
        }else if(strcmp( nem, "STCH") == 0){
                return 0x54;
        }else if(strcmp( nem, "STL") == 0){
                return 0x14;
        }else if(strcmp( nem, "STSW") == 0){
                return 0xE8;
        }else if(strcmp( nem, "STX") == 0){
                return 0x10;
        }else if(strcmp( nem, "SUB") == 0){
                return 0x1C;
        }else if(strcmp( nem, "TD") == 0){
                return 0xE0;
        }else if(strcmp( nem, "TIX") == 0){
                return 0x2C;
        }else if(strcmp( nem, "WD") == 0){
                return 0xDC;
        }else{
                printf("\nunrecognized instruction found\n\n%s\n\n",nem);
		return 0;
       }

}


