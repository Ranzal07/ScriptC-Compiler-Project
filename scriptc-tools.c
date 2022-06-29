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

float symbols[1000];		// symbols store values to the identifier
char* char_symbols[1000];		// symbols store chartype values to the identifier
identifier id[1000];		// id will be the struct variable name and has 1000 indexes to store data



/* compIdxVar will compute the given variable index and return it */    
int compIdxVar(char* variable){
    int idx = *variable;
    return idx;
} 


/* getNumValue gets the given variable's int or float value and return it to the IDENTIFIER token */
float getNumValue(char* variable){	
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
				strcpy(char_symbols[bucket],id[i].cval);
				return char_symbols[bucket];		// returns the current variable's recognized char type value according to its index
			}
		}
	}
}


/* updateNumVal updates the given variable's int or float type value when given another new values */
void updateNumVal(char* variable, float value){
	int i;
	int toIntValue = (int)value;		// typecasting to int or to convert float value datatype to int
	int bucket = compIdxVar(variable);

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0){			
				symbols[bucket] = toIntValue;
				id[i].ival = symbols[bucket];		// new int values will be saved to the struct identifiers (id.ival)		
				break;
			}
			else if(strcmp(id[i].typ,"float")==0){
				symbols[bucket] = value;
				id[i].fval = symbols[bucket];		// new float values will be saved to the struct identifiers (id.fval)
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


/* saveThisNumVal saves the value (int or float type) to the struct identifiers */
void saveThisNumVal(char* variable, float value){
	int i;
	int toIntValue = (int)value;

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0){
				id[indexVar].ival = toIntValue;		
				break;		
			}
			else if(strcmp(id[i].typ,"float")==0){
				id[indexVar].fval = value;	
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
		printf("\n>>>> ERROR LINE %d: '%s' already declared! <<<<",line,variable);	
		exit(1);		// terminates the program
	}
	else{
		saveThisVar(variable,type);		// otherwise, it will invoke the saveThisVar function to save the variable and its type
		// printf("\nLINE %d: Correct Variable '%s' Declaration!",line,variable);	
	}
}


/* checkNumVarExist checks if the given variable (int or float type) exists during initialization */
void checkNumVarExist(char* variable, float value){
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
		saveThisNumVal(variable,value);		// if exists, it will invoke the saveThisVar function to save the variable's value
		updateNumVal(variable,value);		// then, it will invoke the updateNumVal function to update the variable's value
		// printf("\nLINE %d: Correct Variable '%s' Initialization!",line,variable);
	} else {
		printf("\n>>>> ERROR LINE %d: '%s' undeclared! <<<<",line,variable);
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
		printf("\n>>>> ERROR LINE %d: '%s' undeclared! <<<<",line,variable);
		exit(1);
	}
}


/* checkThisNumVar checks if the given variable (int or float type) initialized to another variable exists */
/* checkThisNumVar also checks if the given variable (int or float type) exists during printing and type matching */
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
		return getNumValue(variable); // if exists, then it will invoke the getNumValue function
	} 
	else if(flag==2){		// causes error if the given variable is type mismatched for an int or float type
		if(strcmp(id[getIndex].typ,"char")==0){
			printf("\n>>>> ERROR LINE %d: '%s' is neither 'int' nor 'float' type! <<<<",line,variable);
			exit(1);
		}
	} else {
		printf("\n>>>> ERROR LINE %d: '%s' undeclared! <<<<",line,variable);
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
			printf("\n>>>> ERROR LINE %d: '%s' is not a 'char' type! <<<<",line,variable);
			exit(1);
		}
	}
	else {
		printf("\n>>>> ERROR LINE %d: '%s' undeclared! <<<<",line,variable);
		exit(1);
	}
}


/* oneNumValPrint prints one given variable's number value */
void oneNumValPrint(char* specifier, float value){
	int toIntValue = (int)value;

	if(strcmp(specifier,"%d")==0){
		printf("\nLINE %d Output: %d",line,toIntValue);		// prints integer
	}
	else if(strcmp(specifier,"%f")==0){
		printf("\nLINE %d Output: %f",line,value);		// prints float
	}
}


/* tw0NumValPrint prints two given variable's number values */
void twoNumValPrint(char* specifier, char* specifier2, float value, float value2){
	int toIntValue = (int)value;
	int toIntValue2 = (int)value2;

	if(strcmp(specifier,"%d")==0){
		if(strcmp(specifier2,"%d")==0){
			printf("\nLINE %d Output: %d%d",line,toIntValue,toIntValue2);		// prints two integers
		}
		else{
			printf("\nLINE %d Output: %d%f",line,toIntValue,value2);		// prints integer then float
		}
	}
	else if(strcmp(specifier,"%f")==0){
		if(strcmp(specifier2,"%f")==0){
			printf("\nLINE %d Output: %f%f",line,value,value2);		// prints two floats
		}
		else{
			printf("\nLINE %d Output: %f%d",line,value,toIntValue2);		// prints float, then integer
		}
	}
}


/* oneCharValPrint prints one given variable's character value */
void oneCharValPrint(char* specifier, char* value){
	if(strcmp(specifier,"%c")==0){
		printf("\nLINE %d Output: %c",line,value[0]);	// prints single character
	}
	else if(strcmp(specifier,"%s")==0){
		printf("\nLINE %d Output: %s",line,value);	// prints strings
	}
}


/* twoCharValPrint prints two given variable's character values */
void twoCharValPrint(char* specifier, char* specifier2, char* value, char* value2){

	if(strcmp(specifier,"%c")==0){
		if(strcmp(specifier2,"%c")==0){
			printf("\nLINE %d Output: %c%c",line,value[0],value2[0]);		// prints two single character
		}
		else if(strcmp(specifier2,"%s")==0){
			printf("\nLINE %d Output: %c%s",line,value[0],value2);		// prints single character, then strings
		}
	}
	else if(strcmp(specifier,"%s")==0){
		if(strcmp(specifier2,"%c")==0){
			printf("\nLINE %d Output: %s%c",line,value,value2[0]);		// prints strings, then single character
		}
		else if(strcmp(specifier2,"%s")==0){
			printf("\nLINE %d Output: %s%s",line,value,value2);		// prints two strings
		}
	}
}


/* NumCharValPrint prints the number values first, then the character values */
void NumCharValPrint(char* specifier, char* specifier2, float value, char* value2){
	int toIntValue = (int)value;

	if(strcmp(specifier,"%d")==0){
		if(strcmp(specifier2,"%c")==0)
			printf("\nLINE %d Output: %d%c",line,toIntValue,value2[0]);	// prints integer, then single character
		else if(strcmp(specifier2,"%s")==0)
			printf("\nLINE %d Output: %d%s",line,toIntValue,value2);	// prints integer, then strings
	}
	else if(strcmp(specifier,"%f")==0){
		if(strcmp(specifier2,"%c")==0)
			printf("\nLINE %d Output: %f%c",line,value,value2[0]);	// prints float, then single character
		else if(strcmp(specifier2,"%s")==0)
			printf("\nLINE %d Output: %f%s",line,value,value2);	// prints float, then strings
	}
}


/* CharNumValPrint prints the character values first, then the number values */
void CharNumValPrint(char* specifier, char* specifier2, char* value, float value2){
	int toIntValue2 = (int)value2;

	if(strcmp(specifier,"%c")==0){
		if(strcmp(specifier2,"%d")==0)
			printf("\nLINE %d Output: %c%d",line,value[0],toIntValue2);	// prints single character, then integer
		else if(strcmp(specifier2,"%f")==0)
			printf("\nLINE %d Output: %c%f",line,value[0],value2);	// prints single character, then float
	}
	else if(strcmp(specifier,"%s")==0){
		if(strcmp(specifier2,"%d")==0)
			printf("\nLINE %d Output: %s%d",line,value,toIntValue2);	// prints strings, then integer
		else if(strcmp(specifier2,"%f")==0)
			printf("\nLINE %d Output: %s%f",line,value,value2);	// prints strings, then float
	}
}

