#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

typedef struct types{
	int i;
	float f;
	char* c;
} type;


type symbols[1000];		// symbols store values to the identifier
char* char_symbols[1000];		// symbols store chartype values to the identifier
identifier id[1000];		// id will be the struct variable name and has 1000 indexes to store data



/* compIdxVar will compute the given variable index and return it */    
int compIdxVar(char* variable){
    int idx = *variable;
    return idx;
} 


/* getValue gets the given variable's int or float value and return it to the IDENTIFIER token */
type getValue(char* variable){	
	int i;
	
	int bucket = compIdxVar(variable);		// recognized variable index will be initialized to the bucket variable
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){		// <-- this means if the struct id.var is equal to the current variable name
			if(strcmp(id[i].typ,"int")==0){
				symbols[bucket].i = id[i].ival;	
				return symbols[bucket];		// returns the given variable's recognized int value according to its index
			}
			else if(strcmp(id[i].typ,"float")==0){
				symbols[bucket].f = id[i].fval;
				return symbols[bucket];		// returns the current variable's recognized float value according to its index
			}	
			else if(strcmp(id[i].typ,"char")==0){
				strcpy(symbols[bucket].c,id[i].cval);
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
				strcpy(char_symbols[bucket],id[i].cval);
				return char_symbols[bucket];		// returns the current variable's recognized char type value according to its index
			}
		}
	}
}


/* updateVal updates the given variable's int or float type value when given another new values */
void updateVal(char* variable, int iValue, float fValue, char* cValue){
	int i;
	//int toIntValue = (int)value;		// typecasting to int or to convert float value datatype to int
	int bucket = compIdxVar(variable);

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0){			
				symbols[bucket].i = iValue;
				id[i].ival = symbols[bucket].i;		// new int values will be saved to the struct identifiers (id.ival)		
				break;
			}
			else if(strcmp(id[i].typ,"float")==0){
				symbols[bucket].f = fValue;
				id[i].fval = symbols[bucket].f;		// new float values will be saved to the struct identifiers (id.fval)
				break;
			}
			else if(strcmp(id[i].typ,"char")==0){
				symbols[bucket].c = cValue;
				strcpy(id[i].cval,symbols[bucket].c);	// new float values will be saved to the struct identifiers (id.fval)
				break;
			}
		}
	}
}


/* updateCharVal updates the given variable's char type value when given another new values */
void updateCharVal(char* variable, char* value){
	int i;
	
	int bucket = compIdxVar(variable);
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"char")==0){
				char_symbols[bucket] = value;
				strcpy(id[i].cval,char_symbols[bucket]); // new char values will be saved to the struct identifiers (id.cval)
				break;
			}
		}
	}
}


/* saveThisVar saves the verified given variable and its given type to the struct identifiers */
void saveThisVar(char* variable, char* type){
	strcpy(id[indexVar].var,variable);
	strcpy(id[indexVar].typ,type);
	indexVar++;		// Increments to the next ID index after saving the variable and type
}


/* saveThisVal saves the value (int or float type) to the struct identifiers */
void saveThisVal(char* variable, int iValue, float fValue, char* cValue){
	int i;

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0){
				id[indexVar].ival = iValue;		
				break;		
			}
			else if(strcmp(id[i].typ,"float")==0){
				id[indexVar].fval = fValue;	
				break;
			}
			else if(strcmp(id[i].typ,"char")==0){
				strcpy(id[indexVar].cval,cValue);	
				break;
			}
		}
	}
}


/* saveThisCharVal saves the value (char type) to the struct identifiers */
void saveThisCharVal(char* variable, char* value){
	int i;
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"char")==0){
				strcpy(id[indexVar].cval,value);
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
		printf("\n---->>>> ERROR TYPE: VARIABLE REDECLARATION <<<<----");	
		exit(1);		// terminates the program
	}
	else{
		saveThisVar(variable,type);		// otherwise, it will invoke the saveThisVar function to save the variable and its type
		// printf("\nLINE %d: Correct Variable '%s' Declaration!",line,variable);	
	}
}


/* checkVarExist checks if the given variable (int or float type) exists during initialization */
void checkVarExist(char* variable, int iValue, float fValue, char* cValue){
	int i;
	int flag = 0;
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0 || strcmp(id[i].typ,"float")==0){
				flag = 1;
				break;			
			}
		}
	}
	if(flag==1){
		saveThisVal(variable,iValue,fValue,cValue);		// if exists, it will invoke the saveThisVar function to save the variable's value
		updateVal(variable,iValue,fValue,cValue);		// then, it will invoke the updateVal function to update the variable's value
		// printf("\nLINE %d: Correct Variable '%s' Initialization!",line,variable);
	} else {
		printf("\n---->>>> ERROR LINE %d: '%s' undeclared! <<<<----",line,variable);
		printf("\n---->>>> ERROR TYPE: UNDECLARED VARIABLE <<<<----");	
		exit(1);
	}
}


/* checkCharVarExist checks if the given variable (char type) exists during initialization */
void checkCharVarExist(char* variable, char* value){
	int i;
	int flag = 0;
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"char")==0){
				flag = 1;
				break;			
			}
		}
	}
	if(flag==1){
		saveThisCharVal(variable,value);		// if exists, it will invoke the saveThisVar function to save the variable's value
		updateCharVal(variable,value);		// then, it will invoke the updateCharVal function to update the variable's value
		// printf("\nLINE %d: Correct Variable '%s' Initialization!",line,variable);
	} else {
		printf("\n---->>>> ERROR LINE %d: '%s' undeclared! <<<<----",line,variable);
		printf("\n---->>>> ERROR TYPE: UNDECLARED VARIABLE <<<<----");	
		exit(1);
	}
}


/* checkThisVar checks if the given variable (int or float type) initialized to another variable exists */
/* checkThisVar also checks if the given variable (int or float type) exists during printing and type matching */
type checkThisVar(char* variable){
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
			printf("\n---->>>> ERROR LINE %d: '%s' is neither 'int' nor 'float' type! <<<<----",line,variable);
			printf("\n\t---->>>> ERROR TYPE: TYPE MISMATCH <<<<----");	
			exit(1);
		}
	} else {
		printf("\n---->>>> ERROR LINE %d: '%s' undeclared! <<<<----",line,variable);
		printf("\n---->>>> ERROR TYPE: UNDECLARED VARIABLE <<<<----");	
		exit(1);
	}
}


/* checkThisCharVar checks if the given variable (char type) initialized to another variable exists */
/* checkThisCharVar also checks if the given variable (char type) exists during printing and type matching */
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
			else{			
				flag = 2;
				break;
			}
		}
	}
	if(flag==1){
		return getCharValue(variable); // if exists, then it will invoke the getCharValue function
	} 
	else if(flag==2){		// causes error if the given variable is type mismatched for a char type
		if(strcmp(id[getIndex].typ,"int")==0 || strcmp(id[getIndex].typ,"float")==0){
			printf("\n---->>>> ERROR LINE %d: '%s' is not a 'char' type! <<<<----",line,variable);
			printf("\n\t---->>>> ERROR TYPE: TYPE MISMATCH <<<<----");		
			exit(1);
		}
	}
	else {
		printf("\n---->>>> ERROR LINE %d: '%s' undeclared! <<<<----",line,variable);
		printf("\n---->>>> ERROR TYPE: UNDECLARED VARIABLE <<<<----");	
		exit(1);
	}
}


/* oneValPrint prints one given variable's number value */
void oneValPrint(char* specifier, int iValue, float fValue){

	if(strcmp(specifier,"%d")==0){
		printf("\nLINE %d Output: %d",line,iValue);		// prints integer
	}
	else if(strcmp(specifier,"%f")==0){
		printf("\nLINE %d Output: %f",line,fValue);		// prints float
	}
}


/* tw0ValPrint prints two given variable's number values */
void twoNumValPrint(char* specifier, char* specifier2, int iValue, float fValue, int iValue2, float fValue2){

	if(strcmp(specifier,"%d")==0){
		if(strcmp(specifier2,"%d")==0){
			printf("\nLINE %d Output: %d%d",line,iValue,iValue2);		// prints two integers
		}
		else{
			printf("\nLINE %d Output: %d%f",line,iValue,fValue2);		// prints integer then float
		}
	}
	else if(strcmp(specifier,"%f")==0){
		if(strcmp(specifier2,"%f")==0){
			printf("\nLINE %d Output: %f%f",line,fValue,fValue2);		// prints two floats
		}
		else{
			printf("\nLINE %d Output: %f%d",line,fValue,iValue2);		// prints float, then integer
		}
	}
}


/* oneCharValPrint prints one given variable's character value */
void oneCharValPrint(char* specifier, char* cValue){
	char* c;
	
	if(strcmp(specifier,"%c")==0){
		printf("\nLINE %d Output: %c",line,cValue[0]);	// prints single character
	}
	else if(strcmp(specifier,"%s")==0){
		printf("\nLINE %d Output: %s",line,cValue);	// prints strings
	}
}


/* twoCharValPrint prints two given variable's character values */
void twoCharValPrint(char* specifier, char* specifier2, char* cValue, char* cValue2){

	if(strcmp(specifier,"%c")==0){
		if(strcmp(specifier2,"%c")==0){
			printf("\nLINE %d Output: %c%c",line,cValue[0],cValue2[0]);		// prints two single character
		}
		else if(strcmp(specifier2,"%s")==0){
			printf("\nLINE %d Output: %c%s",line,cValue[0],cValue2);		// prints single character, then strings
		}
	}
	else if(strcmp(specifier,"%s")==0){
		if(strcmp(specifier2,"%c")==0){
			printf("\nLINE %d Output: %s%c",line,cValue,cValue2[0]);		// prints strings, then single character
		}
		else if(strcmp(specifier2,"%s")==0){
			printf("\nLINE %d Output: %s%s",line,cValue,cValue2);		// prints two strings
		}
	}
}


/* NumCharValPrint prints the number values first, then the character values */
void NumCharValPrint(char* specifier, char* specifier2, int iValue, float fValue, char* cValue){

	if(strcmp(specifier,"%d")==0){
		if(strcmp(specifier2,"%c")==0)
			printf("\nLINE %d Output: %d%c",line,iValue,cValue[0]);	// prints integer, then single character
		else if(strcmp(specifier2,"%s")==0)
			printf("\nLINE %d Output: %d%s",line,iValue,cValue);	// prints integer, then strings
	}
	else if(strcmp(specifier,"%f")==0){
		if(strcmp(specifier2,"%c")==0)
			printf("\nLINE %d Output: %f%c",line,fValue,cValue[0]);	// prints float, then single character
		else if(strcmp(specifier2,"%s")==0)
			printf("\nLINE %d Output: %f%s",line,fValue,cValue);	// prints float, then strings
	}
}


/* CharNumValPrint prints the character values first, then the number values */
void CharNumValPrint(char* specifier, char* specifier2, char* cValue, int iValue, float fValue){

	if(strcmp(specifier,"%c")==0){
		if(strcmp(specifier2,"%d")==0)
			printf("\nLINE %d Output: %c%d",line,cValue[0],iValue);	// prints single character, then integer
		else if(strcmp(specifier2,"%f")==0)
			printf("\nLINE %d Output: %c%f",line,cValue[0],fValue);	// prints single character, then float
	}
	else if(strcmp(specifier,"%s")==0){
		if(strcmp(specifier2,"%d")==0)
			printf("\nLINE %d Output: %s%d",line,cValue,iValue);	// prints strings, then integer
		else if(strcmp(specifier2,"%f")==0)
			printf("\nLINE %d Output: %s%f",line,cValue,fValue);	// prints strings, then float
	}
}

void printAnything(char* strings){
	printf("\nLINE %d Output: %s",line,strings);	// prints strings, then integer
}
