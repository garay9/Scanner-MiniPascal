#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"

struct define {
	char constant[31];
	char value[255];
} define;

///////////////////////////////////////////////////////////
/////////////////////EXTERNS OF FLEX///////////////////////
//////////////////////////////////////////////////////////
typedef struct yy_buffer_state * YY_BUFFER_STATE;
#define yyterminate() return YY_NULL
extern FILE *Preprocin, *Preprocout;
extern int Preproclex();
extern char* Preproctext;
extern int Preprocleng;
extern YY_BUFFER_STATE Preproc_create_buffer(FILE* file, int size);
extern void Preprocpush_buffer_state(YY_BUFFER_STATE new_buffer);
extern void Preprocpop_buffer_state();
extern void Preprocrestart (FILE* input_file);
extern int Preproclex_destroy(void);
extern void scanner(char *name);
char** includesBuffer;
//char defineBuffers 
int maxIncludeSize = 1024;
int includesBufferSize = 0;

struct define* definesBuffer;
int maxDefineSize = 1024;
int definesBufferSize = 0;


/////////////////////////////////////////////////////////
//////////////////AUXILIARY FUNCIONS////////////////////
////////////////////////////////////////////////////////

//copies the old buffer to a new buffer with double size
void listFull() {
	char** temp = includesBuffer;
	maxIncludeSize *= 2;
	includesBuffer = realloc(includesBuffer, maxIncludeSize);

	for(int i = 0; i < includesBufferSize; i++){
		includesBuffer[i] = malloc(255);
		strcpy(includesBuffer[i], temp[i]);
	}
	free(temp);
}

void repeatedDefine(char* constant, char* defineName){
	for(int i = 0; i < definesBufferSize ; i++){
		if(strcmp(definesBuffer[i].constant, constant) == 0){
			printf("REPEATED DEFINE %s\n", defineName);
			exit(1);
		}
	}
}


struct define* addDefine(struct define def){
	if(definesBufferSize == 0){
		definesBuffer =  malloc(maxDefineSize);
		strcpy(definesBuffer[definesBufferSize].constant, def.constant);
		strcpy(definesBuffer[definesBufferSize].value, def.value);

	}
	if(definesBufferSize== maxDefineSize){
		struct define* temp = definesBuffer;
		maxDefineSize*=2;
		definesBuffer =  realloc(definesBuffer, maxDefineSize);
		for(int i = 0;  i < definesBufferSize; i++){
			strcpy(definesBuffer[i].constant, temp[i].constant);
			strcpy(definesBuffer[i].value, temp[i].value);
		}
		strcpy(definesBuffer[definesBufferSize].constant, def.constant);
		strcpy(definesBuffer[definesBufferSize].value, def.value);
	}
	else{
		strcpy(definesBuffer[definesBufferSize].constant, def.constant);
		strcpy(definesBuffer[definesBufferSize].value, def.value);
	}
	return definesBuffer;
}

//inserts includes's file name
void addString(char* string){
	if(includesBuffer == 0){
		includesBuffer = (char**) malloc(maxIncludeSize * 255);
		for(int i = 0; i < maxIncludeSize; i++){
			includesBuffer[i] = (char*) malloc(255);
		}
		strcpy(includesBuffer[includesBufferSize], string);

	}
	else if(includesBufferSize == maxIncludeSize){
		listFull();
		strcpy(includesBuffer[includesBufferSize], string);
	}
	else{
		strcpy(includesBuffer[includesBufferSize], string);
	}
	includesBufferSize++;

}

//checks if an include is added
int isAdded(char* string) {
	for(int i = 0; i < includesBufferSize ; i++){
		if(strcmp(includesBuffer[i], string) == 0){
			return 1;
		}
	}
	return 0;
}


char* quitarExtension(const char *nombre) {
    char *resultado;
    char *punto;
    if (nombre == NULL){
         return NULL;
	}
    if ((resultado = malloc (strlen (nombre) + 1)) == NULL){
        return NULL;
	}
    strcpy (resultado, nombre);
    punto = strrchr (resultado, '.');
    if (punto != NULL)
        *punto = '\0';
    return resultado;
}

char* findDefine(char* id){
	for(int i = 0; i < definesBufferSize; i++) {
		if(strcmp(id, definesBuffer[i].constant) == 0){
			return definesBuffer[i].value;
		}
	}
	return "";
}

//This is the second reading of the file, flex omits the comments and replaces defines
char* applyDefines(char* name){
	char* newName = quitarExtension(name);
	strcat(newName, ".gfa");
	FILE* f = fopen(name, "r");
	FILE* output = fopen(newName, "w");
	Preprocrestart(f);
	
	int tok = Preproclex();

	while(tok != EXIT){
		if(tok == IDENTIFIER){
			char* substitute = findDefine(Preproctext);
			if(strcmp(substitute,"") == 0) 
				fputs(Preproctext, output);
			else
				fputs(substitute, output);
		}
			
		else{
			fputs(Preproctext, output);
		}
		tok = Preproclex();
	}
	
	fclose(f);
	fclose(output);
	return newName;
}

//Writes a intermediate file after preprocessing
char* writeFile(char const *fileName, char* string) {
	FILE* f;
	char* tempName = malloc(strlen(fileName));
	strcpy(tempName, fileName);
	char* newName = quitarExtension(tempName);
	strcat(newName, ".Tgfa");
	f = fopen(newName, "w");
	fprintf(f,"%s\n", string);
	free(tempName);
	fclose(f);
	char * result = applyDefines(newName);
	remove(newName);
	return result;
}

int isOperator(char c){
	if(c == '+'|| c== '-'|| c == '/') return 1;
	return 0;
}


///////////////////////////////////////////////////////////////////////////
////////////////////////////////PREPROCESSOR///////////////////////////////
//////////////////////////////////////////////////////////////////////////

char* preprocessor(char const *name){
	FILE* currentFile;
	char* result;
	char c;
	char* fileName;
	char* copyFile;
	int sizeCounter;
	
	currentFile = fopen(name, "r");

	if(currentFile == NULL){
		printf("FILE %s NOT FOUND\n", name);
		exit(1);
	}

	//gets current file size
	fseek(currentFile, 0, SEEK_END);
	int size = ftell(currentFile);
	fseek(currentFile, 0, SEEK_SET);
	size = size*2 +1;
	result = (char*) calloc(size, sizeof(char));

	//moves the pointer of currentFile to the beginning
	rewind(currentFile);

	//new buffer of flex.
	YY_BUFFER_STATE newBuffer = Preproc_create_buffer(currentFile, size);
	Preprocpush_buffer_state(newBuffer);
	
	//get last token matched	
	int tok = Preproclex();

	int sizeCopy;

	while(tok != EXIT){
		if(tok == INCLUDE){

			//yyleng yytext
			char includeName[Preprocleng];
			strcpy(includeName, Preproctext);

			//processes #include<file.ext> and returns file.ext 
			fileName = strrchr(Preproctext, '<');
			fileName = strtok(fileName, "<");
			fileName = strtok(fileName, ">");


			//if there are the same include, omits and puts \0 
			if(isAdded(fileName)){
				strcat(result, "");
		
			}else{
				addString(fileName);

				copyFile = preprocessor(fileName);
				sizeCopy =  strlen(copyFile);


				char* resultCopy = (char*) malloc(strlen(result));
				strcpy(resultCopy, result);

				//Replaces include to file content
				size *= 2;
				result = (char*) realloc(result, size + strlen(includeName));
				strcat(result, resultCopy);
				strcat(result, copyFile);
				free(resultCopy);
			}
 
		}
		else if (tok == DEFINE){
			int modified = 0;
			struct define def;

			char defineName[Preprocleng];
			strcpy(defineName, Preproctext);
			printf("DEFINE %s\n", defineName);

			//gets the ID of define
			char* variable = malloc(strlen(defineName));
			strcpy(variable, defineName);
			variable = strchr(variable, ' ');
			variable = strtok(variable, " ");

			//gets the constat associated with the ID
			char* value = malloc(strlen(defineName));
			value = strrchr(defineName, ' ');
			value = strtok(value, " ");

			//data storages the name of identifiers
			char* data =  calloc(256, 1);
			//new value has all IDs repleaced
			char* newValue = calloc(256,1);
			int valueSize = strlen(value);
		
			for(int i = 0; i <= valueSize; i++){
				char c =  value[i];
				char* valueSubstituted = findDefine(data);
				if(isOperator(value[i])){
					//if there are no defines associated with data, concatenates data.	
					if(strcmp(valueSubstituted,"") == 0 && strlen(data)!=0){
							strcat(newValue, data);
						}else{
							//else, adds the value associated with data
							strcat(newValue, valueSubstituted);
						}

						//cleans data and concatenates the operator
						strcpy(data, "");
						strncat(newValue, &c, 1);
						modified = 1;
				}else{
					//adds a character on data
					strncat(data, &c, 1);
					char* lastID = findDefine(data);

					//if is there are no more operators, checks the associated value
					if(strcmp(lastID,"") == 0 && i == valueSize){
						strcat(newValue, data);
					}else if(i==valueSize){
						strcat(newValue, lastID);
					}
				}
				
			
			}
			if(modified){
				strcpy(def.constant, variable);
				strcpy(def.value, newValue);	

			}
			else{
				char* substitute = findDefine(value);
				
				strcpy(def.constant, variable);
				if(strcmp(substitute, "") == 0){
					strcpy(def.value, value);
				}
				else{
					strcpy(def.value, substitute);
				}
			
			}
		
			//checks if constant is on buffer
			repeatedDefine(def.constant, defineName);
			definesBuffer = addDefine(def);
			definesBufferSize++;
			

		}
		else{
			//puts other characters into string
			strcat(result, Preproctext);
		}

		//go to the next token
		tok = Preproclex();
	}
	//Pop the last buffer
	Preprocpop_buffer_state();
	fclose(currentFile);

	return result;
}



int main(int argc, char const *argv[]){
	char* data;
	char* fileName;
	data = preprocessor(argv[1]);
	fileName = writeFile(argv[1], data);
	printf("****************SCANNER RESULT********************\n");
	printf("NOMBRE; %s\n", fileName);
	//scanner(fileName);
	return 0;
}