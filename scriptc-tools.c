#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int line = 1;		// for getting incremented line number indexes
int indexVar = 0;		// for incrementing variable indexes

/* struct for storing ID or Variable data */
typedef struct indentifiers{
	char var[1000];		// var stores variable names
	char typ[1000];		// typ stores variable's type
	int ival;		// ival stores int type values
	float fval;		// fval stores float type values
	char cval[1000];		// cval stores char type values	
} identifier;

identifier id[1000];		// id will be the struct variable name and has 1000 indexes to store data
float symbols[1000];		// symbols store values to the identifier
char* char_symbols[1000];		// symbols store chartype values to the identifier
char stringsDisplay[100][100];	// symbols store strings values to be displayed



/* getType will get the value' type and return it */ 
int getType(char *variable){
	int i;
	int flag = 0;
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if (strcmp(id[i].typ,"int")==0){
				flag = 1;
				break;
			} else if (strcmp(id[i].typ,"float")==0){
				flag = 2;
				break;
			} else if (strcmp(id[i].typ,"char")==0){
				flag = 3;
				break;
			}
		}
	}
	return flag;
}

/* addStr will add the string to the string table */ 
void addStr(char* str, int length){
	strcpy(stringsDisplay[length], str);
}


/* compIdxVar will compute the given variable index and return it */    
int compIdxVar(char* variable){
    int idx = *variable;
    return idx;
} 


/* getValue gets the given variable's int or float value and return it to the IDENTIFIER token */
float getValue(char* variable){	
	int i;
	
	int bucket = compIdxVar(variable);		// recognized variable index will be initialized to the bucket variable
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){		// <-- this means if the struct id.var is equal to the current variable name
			if(strcmp(id[i].typ,"int")==0){
				symbols[bucket] = id[i].ival;	
				return symbols[bucket];		// returns the given variable's recognized int value according to its index
			}
			else if(strcmp(id[i].typ,"float")==0){
				symbols[bucket] = id[i].fval;
				return symbols[bucket];		// returns the current variable's recognized float value according to its index
			}	
		}
	}
}


/* getCharValue gets the given variable's char type value and return it to the IDENTIFIER token */
char* getCharValue(char* variable){	
	int i;
	
	int bucket = compIdxVar(variable);		// recognized variable index will be initialized to the bucket variable
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){	
			if(strcmp(id[i].typ,"char")==0){
				if(strlen(id[i].cval)!=0){
					strcpy(char_symbols[bucket],id[i].cval);
					return char_symbols[bucket];	// returns the current variable's recognized char type value according to its index
				}
				else{
					return NULL;
				}									
			}
		}
	}
}


/* updateVal updates the given variable's value when given another new values */
void updateVal(char* variable, int iValue, float fValue, char* cValue){
	int i;
	int bucket = compIdxVar(variable);

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0){			
				symbols[bucket] = iValue;
				id[i].ival = symbols[bucket];		// new int values will update the struct identifiers (id.ival)		
				break;
			}
			else if(strcmp(id[i].typ,"float")==0){
				symbols[bucket] = fValue;
				id[i].fval = symbols[bucket];		// new float values will update the struct identifiers (id.fval)
				break;
			}
			else if(strcmp(id[i].typ,"char")==0){
				char_symbols[bucket] = cValue;
				strcpy(id[i].cval,char_symbols[bucket]);	// new char values will update the struct identifiers (id.cval)
				break;
			}
		}
	}
}


/* registThisVar registers the verified given variable and its given type to the struct identifiers */
void registThisVar(char* variable, char* type){
	strcpy(id[indexVar].var,variable);
	strcpy(id[indexVar].typ,type);
	indexVar++;		// Increments to the next ID index after registering the variable and type
}



/* checkValue checks the values type matching */ 
void checkValue(char* variable, int iValue, float fValue, char* cValue, int type){
	int i;
	int flag = 0;
	int getType;
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			getType = i;
			if(strcmp(id[i].typ,"int")==0){
				flag = 1;	
				break;		
			}
			else if(strcmp(id[i].typ,"float")==0){
				flag = 2;	
				break;
			}
			else if(strcmp(id[i].typ,"char")==0){
				flag = 3;
				break;
			}
		}
	}
	if(flag==1){
		if(type==3){
			printf("\n---->>>> ERROR LINE %d: '%s' is not an 'int' type, type mismatched! <<<<----",line,cValue);
			exit(1);
		}
	}
	else if(flag==2){
		if(type==3){
			printf("\n---->>>> ERROR LINE %d: '%s' is not a 'float' type, type mismatched! <<<<----",line,cValue);
			exit(1);
		}
	}
	else if(flag==3){
		if(type==1){
			printf("\n---->>>> ERROR LINE %d: '%d' is not a 'char' type, type mismatched! <<<<----",line,iValue);
			exit(1);
		}
		else if(type==2){
			printf("\n---->>>> ERROR LINE %d: '%g' is not a 'char' type, type mismatched! <<<<----",line,fValue);
			exit(1);
		}
	}
}



/* registThisVal registers the given variable's values to the struct identifiers */
void registThisVal(char* variable, int iValue, float fValue, char* cValue){
	int i;

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0){
				id[indexVar].ival = iValue;		// given int values will be registered to the struct identifiers (id.ival)
				break;		
			}
			else if(strcmp(id[i].typ,"float")==0){
				id[indexVar].fval = fValue;		// given float values will be registered to the struct identifiers (id.fval)	
				break;
			}
			else if(strcmp(id[i].typ,"char")==0){
				strcpy(id[indexVar].cval,cValue);		// given char values will be registered to the struct identifiers (id.cval)		
				break;
			}
		}
	}
}



/* checkVarDup checks if the given variable has duplicates or has been redeclared */
void checkVarDup(char* variable, char* type){
	int i;
	int flag = 0;

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){	
			flag = 1;
			break;
		}
	}
	if(flag==1){
		printf("\n---->>>> ERROR LINE %d: '%s' already declared! <<<<----",line,variable);	
		exit(1);		// terminates the program
	}
	else{
		registThisVar(variable,type);		// otherwise, it will invoke the registThisVar function to register the variable and its type
		// printf("\nLINE %d: Correct Variable '%s' Declaration!",line,variable);	
	}
}


/* checkVarExist checks if the given variable exists during initialization */
void checkVarExist(char* variable, int iValue, float fValue, char* cValue, int type){
	int i;
	int flag = 0;

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0){
				flag = 1;
				break;
			} 
			else if(strcmp(id[i].typ,"float")==0){
				flag = 1;
				break;
			}
			else if(strcmp(id[i].typ,"char")==0){
				flag = 1;
				break;
			}
		}
	}
	if(flag==1){
		checkValue(variable,iValue,fValue,cValue,type);
		registThisVal(variable,iValue,fValue,cValue);		// if exists, it will invoke the registThisVar function to save the variable's value
		updateVal(variable,iValue,fValue,cValue);		// then, it will invoke the updateVal function to update the variable's value
		// printf("\nLINE %d: Correct Variable '%s' Initialization!",line,variable);
	} else {
		printf("\n---->>>> ERROR LINE %d: '%s' undeclared! <<<<----",line,variable);
		exit(1);
	}
}


/* checkVar checks if the instance variable exists during printing */
void checkVar(char* variable){
	int i;
	int flag = 0;
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0 || strcmp(id[i].typ,"float")==0 || strcmp(id[i].typ,"char")==0){
				flag = 1;
				break;			
			}
		}
	}
	if(flag==0){
		printf("\n---->>>> ERROR LINE %d: '%s' undeclared! <<<<----",line,variable);
		exit(1);
	}
}


/* checkThisVar checks if the given variable (int or float type) initialized to another variable exists */
float checkThisNumVar(char* variable){
	int i,getIndex;
	int flag = 0;
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			getIndex = i;
			if(strcmp(id[i].typ,"int")==0){
				flag = 1;
				break;
			}
			else if (strcmp(id[i].typ,"float")==0){
				flag = 1;
				break;
			}
			else if (strcmp(id[i].typ,"char")==0){
				flag = 2;
				break;
			}
		}
	}
	if(flag==1){
		return getValue(variable);		// if exists, then it will invoke the getValue function
	} 
	else if(flag==2){		// causes error if the given variable is type mismatched for an int or float type
		if(strcmp(id[getIndex].typ,"char")==0){
			printf("\n---->>>> ERROR LINE %d: '%s' is a 'char' type, type mismatched! <<<<----",line,variable);
			exit(1);
		}
	} else {
		printf("\n---->>>> ERROR LINE %d: '%s' undeclared! <<<<----",line,variable);
		exit(1);
	}
}






/* checkThisCharVar checks if the given variable (char type) initialized to another variable exists */
char* checkThisCharVar(char* variable){
	int i,getIndex;
	int flag = 0;
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			getIndex = i;
			if (strcmp(id[i].typ,"char")==0){
				flag = 1;
				break;
			}
		}
	}
	if(flag==1)
		return getCharValue(variable); // if exists, then it will invoke the getCharValue function
	else {
		printf("\n---->>>> ERROR LINE %d: '%s' undeclared! <<<<----",line,variable);
		exit(1);
	}
}

char* getStringFromId(char *str){
	char* val = checkThisCharVar(str);
}


void printValues(char* string){
	printf("\nOUTPUT LINE %d: ",line);
	printf("%s", string);
}

// USED FOR COUNTING THE SPECIFIERS FOUND IN THE DISPLAY STRING
int count(const char *str, const char *sub) {
    int sublen = strlen(sub);
    if (sublen == 0) return 0;
    int res = 0;
    for (str = strstr(str, sub); str; str = strstr(str + sublen, sub))
        ++res;
    return res;
}

// REPLACES THE IDENTIFIER WITH THEIR VALUES
void substringInsert(int pos, char* str1, char* str2){
	int counter;
	int position = pos-1;
	int strLength = strlen(str1);
	char* mainStr, finalStr[200];
	int leftChars = position;
	int rightChars = strLength - position - 2;

	char left[100] = "";

	for (counter = 0; counter < leftChars; counter++){
		char currentChar = str1[counter];
		strncat(left, &currentChar, 1);
	}

	char right[100] = "";

	for (counter = 0; counter < rightChars; counter++){
		char currentChar = str1[position + 2 + counter];
		strncat(right, &currentChar, 1);
	}

	strcpy(str1, left);
	strcat(str1, str2);
	strcat(str1, right);
}

// GETS THE POSITION OF IDENTIFIER ON STRING
int getPosition(char* str, char* subStr){
	char* dest = strstr(str, subStr);
	int pos;
	pos = dest - str + 1;
	return pos;
}

// PRINTS THE FINAL STRING OUTPUT WITH NEW LINES
void printFinalString(char* strFinal){
	int counter2=0;
	printf("\nOUTPUT LINE %d: ",line);
	while(strFinal[counter2] != '\0')
		{
		 if(strFinal[counter2] == '\\' && strFinal[counter2+1] == 'n'){	
			printf("\n");
			counter2++;
		} else printf("%c", strFinal[counter2]);
			counter2++;
	}
}


// PRINTS THE STRING FOR THE FINAL PRODUCT IN YACC LINE 69
void printStruct(char* inputStr, const int iValues[], const float fValues[], int numbersLen, int stringsLen) {
	int numSpecifiers=0, strSpecifiers=0, floatSpecifiers=0, integerSpecifiers=0, charSpecifiers=0, stringSpecifiers=0;
	
	int posfloat=0, posint=0, poschar=0, posstr=0, counter;
	char strFinal[200], strInitial[200], strValue[100];

	floatSpecifiers = count(inputStr, "%f");
	integerSpecifiers = count(inputStr, "%d");
	charSpecifiers = count(inputStr, "%c");
	stringSpecifiers = count(inputStr, "%s");

	// final string
	strcpy(strFinal, inputStr);
	strcpy(strInitial, inputStr);
	
	numSpecifiers = floatSpecifiers + integerSpecifiers;
	strSpecifiers = charSpecifiers + stringSpecifiers;

	int position;

	// prints the number specifiers
	if(numSpecifiers > 0){
		for (counter = 0; counter <= numSpecifiers && counter <= numbersLen; counter++)
		{
			floatSpecifiers = count(strFinal, "%f");
			integerSpecifiers = count(strFinal, "%d");
	
			if(floatSpecifiers) posfloat = getPosition(strFinal, "%f"); 
			if(integerSpecifiers) posint = getPosition(strFinal, "%d"); 

			if(posfloat && posint && (posfloat < posint)){
				sprintf(strValue, "%f", fValues[counter]);
				substringInsert(posfloat, strFinal, strValue);
			} else if (posfloat && posint && (posfloat > posint)) {
				sprintf(strValue, "%d", iValues[counter]);
				substringInsert(posint, strFinal, strValue);
			} else if (!posint && posfloat){
				sprintf(strValue, "%f", fValues[counter]);
				substringInsert(posfloat, strFinal, strValue);
			} else if(!posfloat && posint){
				sprintf(strValue, "%d", iValues[counter]);
				substringInsert(posint, strFinal, strValue);
			}

			posfloat = posint = 0;
		}
	}

	// prints the string specifiers
	if(strSpecifiers > 0){

		for (counter = 0; counter <= strSpecifiers && counter <= stringsLen; counter++)
		{
			charSpecifiers = count(strFinal, "%c");
			stringSpecifiers = count(strFinal, "%s");
	
			if(charSpecifiers) poschar = getPosition(strFinal, "%c"); 
			if(stringSpecifiers) posstr = getPosition(strFinal, "%s"); 

			if(posstr && poschar && (posstr < poschar)){
				// printf("if 1 -> %s\n", stringsDisplay[counter]);
				sprintf(strValue, "%s", stringsDisplay[counter]);
				substringInsert(posstr, strFinal, strValue);

			} else if (poschar && posstr && (posstr > poschar)) {
				// printf("if 2 -> %s\n", stringsDisplay[counter]);
				sprintf(strValue, "%c", stringsDisplay[counter][strlen(stringsDisplay[counter])-1]);
				substringInsert(poschar, strFinal, strValue);

			} else if (!posstr && poschar){
				// printf("if 3 -> %s\n", stringsDisplay[counter]);
				sprintf(strValue, "%c", stringsDisplay[counter][strlen(stringsDisplay[counter])-1]);
				substringInsert(poschar, strFinal, strValue);

			} else if(!poschar && posstr){
				// printf("if 4 -> %s\n", stringsDisplay[counter]);
				sprintf(strValue, "%s", stringsDisplay[counter]);
				substringInsert(posstr, strFinal, strValue);
			}

			poschar = posstr = 0;

		} 
	}
	printFinalString(strFinal);
}