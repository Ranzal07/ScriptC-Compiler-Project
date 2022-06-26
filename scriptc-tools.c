#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int line=1;			// for getting incremented line number indexes
int indexVar=0;		// for incrementing variable indexes

/*struct for storing ID or Variable data*/
typedef struct indentifiers{
	char var[1000];		// var stores variable names
	char typ[1000];		// type stores variable's type
	int ival;			// ival stores int type values
	float fval;			// fval stores float type values
} identifier;

float symbols[1000];		// symbols store values to the identifier
identifier id[1000];		// id will be the struct variable name and has 1000 indexes to store data


/* compIdxVar will compute the given variable index and return it*/    
int compIdxVar(char* variable){
    int idx = *variable;
    return idx;
} 

/* getValue gets the given variable's int or float value and return it to the IDENTIFIER token */
float getValue(char* variable){	
	int i;
	int flag = 0;
	
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


/* updateVal updates the given variable's value when given another new values */
void updateVal(char* variable, float value){
	int i;
	int toIntValue = value;
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

/* saveThisVar saves the verified given variable and its given type to the struct identifiers */
void saveThisVar(char* variable, char* type){
	strcpy(id[indexVar].var,variable);
	strcpy(id[indexVar].typ,type);
	indexVar++;		// Increments to the next ID index after saving the variable and type
}

/* saveThisVal saves any value to the struct identifiers */
void saveThisVal(char* variable, float value){
	int toIntValue = value;
	int i;

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0){
				id[indexVar].ival = toIntValue;				
			}
			else if(strcmp(id[i].typ,"float")==0){
				id[indexVar].fval = value;	
			}
		}
	}
}

/* checkVarDup checks any duplicate or redeclared given variable  */
void checkVarDup(char* variable, char* type){
	int i;
	int flag=0;
	char* temp;

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){	
			flag=1;
			break;
		}
	}
	if(flag==1){
		printf("\n****ERROR LINE %d: '%s' already declared!****",line,variable);	
	}
	else{
		saveThisVar(variable,type);		// otherwise, it will invoke the saveThisVar function to save the variable and its type
		// printf("\nLINE %d: Correct Variable '%s' Declaration!",line,variable);	
	}
}

/* checkVarExist checks if the given variable exists during initialization */
void checkVarExist(char* variable, float value){
	int i;
	int flag = 0;
	char* temp;
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0 || strcmp(id[i].typ,"float")==0){
				flag = 1;
				break;			
			}
		}
	}
	if(flag==1){
		saveThisVal(variable,value);		// if exists, it will invoke the saveThisVar function to save the variable's value
		updateVal(variable,value);		// then, it will invoke the updateVal function to update the variable's value
		// printf("\nLINE %d: Correct Variable '%s' Initialization!",line,variable);
	} else {
		printf("\n****ERROR LINE %d: '%s' undeclared!****",line,variable);
	}
}

/* checkThisVar checks if the given variable initialized to another variable exists*/
float checkThisVar(char* variable){
	int i, toIntValue;
	int flag = 0;
	char* temp;

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0 || strcmp(id[i].typ,"float")==0){
				flag = 1;
				break;			
			}
		}
	}
	if(flag==1){
		return getValue(variable); // if exists, then it will invoke the getValue function
	} else {
		printf("\n****ERROR LINE %d: '%s' undeclared!****",line,variable);
	}
}

/* oneValPrint prints one given variable's value*/
void oneValPrint(char* specifier, float value){
	int toIntValue = value;

	if(strcmp(specifier,"%d")==0){
		printf("\nLINE %d Output: %d",line,toIntValue);
	}
	else if(strcmp(specifier,"%f")==0){
		printf("\nLINE %d Output: %g",line,value);
	}
	
}

/* oneValPrint prints two given variables' values*/
void twoValPrint(char* specifier, char* specifier2, float value, float value2){
	int toIntValue = value;
	int toIntValue2 = value2;

	if(strcmp(specifier,"%d")==0){
		if(strcmp(specifier2,"%d")==0){
			printf("\nLINE %d Output: %d%d",line,toIntValue,toIntValue2);
		}
		else{
			printf("\nLINE %d Output: %d%g",line,toIntValue,value2);
		}
	}
	else if(strcmp(specifier,"%f")==0){
		if(strcmp(specifier2,"%f")==0){
			printf("\nLINE %d Output: %g%g",line,value,value2);
		}
		else{
			printf("\nLINE %d Output: %g%d",line,value,toIntValue2);
		}
	}
}
