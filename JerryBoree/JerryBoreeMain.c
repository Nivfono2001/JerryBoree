#include "Defs.h"
#include "MultiValueHashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"
#include "Jerry.h"
#include "HashTable.h"
#include <math.h>
#define MIN(a,b) ((a)<(b)?(a):(b))


//declaration of functions
int transformIntoNumber(Element str);
int sizeOfTable(int num);
Planet* Find_Planet(char *p_name, Planet **planets_arr, int num_Of_Planets);
Jerry* MostFamiliar(LinkedList jerriesList,PhysicalCharacteristic *pc);
int SaddestJerry(LinkedList jerriesList);
bool equalHappiness(Element j1, Element happiness); //isPartEqual func for jerris in linked list
Element copyElement(Element j);
status printElement(Element j);
status DestroyElement(Element j);
bool equalElements(Element j1, Element j2);
Element copyStr(Element str);
status printStr(Element str);
status destroyStr(Element str);
bool equalStr(Element str1, Element str2);
bool equalID(Element j1, Element id );
status ShellowDestroyElement(Element j);





//main section
int main(int argc, char *argv[]) {

	int numberOfPlanets = atoi(argv[1]);
	char *configurationFile = argv[2];
	Planet **planet_arr = (Planet**) malloc(sizeof(Planet*) * numberOfPlanets);

	// Initilizing planet array to NULLs
	for(int i = 0; i<numberOfPlanets; i++){
		planet_arr[i] = NULL;
	}
	LinkedList jerriesList = createLinkedList(copyElement, DestroyElement, printElement, equalElements, equalHappiness);
	int i, j, i_Planet = 0, i_Jerry = 0, i_Pc = 0, w_size;
	char *token_var;
		//reading from the file
		FILE *file ;
		char str[300];
		file = fopen(configurationFile, "r");
		if (file == NULL) {
			return failure;
		} else {
			//initialization of variabels for Jerries and Planets.
			float x, y, z, j_PC_Val; //x y z are coordinates of a planet
			int j_level_Of_Happy, is_Jerry = 0;

			//Structs initialization
			PhysicalCharacteristic *tmp_pc = NULL;
			Planet *tmp_p = NULL;
			Origin *tmp_origin = NULL;

			//for each row in file:
			while (fgets(str, 300, file) != NULL) {

				//in case the row is 'Jerries' continue to next loop.
				if (strcmp(str, "Jerries\n") == 0){
					is_Jerry = 1;
				    continue;
				}

				//in case the row is "Planets" continue to next loop.
				//after Planets row go into this if:
				if (strcmp(str, "Planets\n") != 0 && is_Jerry == 0) {
					token_var = strtok(str, ",");
					w_size = strlen(token_var);
					char Planet_Name[w_size + 1];
					strcpy(Planet_Name, token_var);	//Planet's name
					token_var = strtok(NULL, ",");
					x = (float) atof(token_var);	//Planet's x coordinate
					token_var = strtok(NULL, ",");
					y = (float) atof(token_var);	//Planet's y coordinate
					token_var = strtok(NULL, ",");
					z = (float) atof(token_var);	//Planet's z coordinate
					planet_arr[i_Planet] = CreatePlanet(Planet_Name, x, y, z);
					if (planet_arr[i_Planet] == NULL){
						printf("Memory error!\n");
						return 1;
					}
					i_Planet++;
				}
				//Jerries details
				if (strcmp(str, "Jerries\n") != 0 && is_Jerry == 1 && str[0] != '\t') {
					token_var = strtok(str, ",");
					w_size = strlen(token_var);
					char j_ID[w_size + 1];
					strcpy(j_ID, token_var);	//Jerry's ID
					token_var = strtok(NULL, ",");
					w_size = strlen(token_var);
					char j_Origin_Name[w_size + 1];
					strcpy(j_Origin_Name, token_var);	//Jerry's origin name
					token_var = strtok(NULL, ",");
					w_size = strlen(token_var);
					char j_Planet_Name[w_size + 1];
					strcpy(j_Planet_Name, token_var);	//Jerry's planet name
					token_var = strtok(NULL, ",");
					j_level_Of_Happy = atoi(token_var);	//Jerry's level of happiness

					//functions for structs creation.
					tmp_p = Find_Planet(j_Planet_Name, planet_arr, numberOfPlanets);
					tmp_origin = CreateOrigin(j_Origin_Name, tmp_p);
					if (tmp_origin == NULL){
						printf("Memory Problem\n");
						return 1;
					}
					 if( appendNode(jerriesList, CreateJerry(j_ID, j_level_Of_Happy, tmp_origin)) == failure ){
						printf("Memory Problem\n");
						return 1;
					}
					tmp_p = NULL;
					tmp_origin = NULL;
					i_Jerry++;
				}

				//row of Jerries 'pc'.
				if (strcmp(str, "Jerries") != 0 && is_Jerry == 1 && '\t' == str[0]) {
					char *t_token = strtok(str, "\t");
					token_var = strtok(t_token, ":");
					w_size = strlen(token_var);
					char j_PC_Name[w_size + 1];
					strcpy(j_PC_Name, token_var);	//Jerry's pc name
					token_var = strtok(NULL, ":");
					j_PC_Val = atof(token_var);		//the value of the pc
					//create the pc & add to current Jerry
					tmp_pc = CreatePC(j_PC_Name, j_PC_Val);
					if (tmp_pc == NULL){
						printf("Memory error!\n");
						return 1;
					}
					add_pc(getDataByIndex(jerriesList, getLengthList(jerriesList)), tmp_pc); 	//adding the pc to current Jerry
					tmp_pc = NULL;
					i_Pc++;
				}
			}
		}
		fclose(file);
	//	displayList(jerriesList);
	int hashNumber = sizeOfTable(getLengthList(jerriesList));	//HASHNUM FOR HASHTABLE AND MULTIHASH IS THE SAME?
	hashTable jerriesHashTable = createHashTable(copyStr, destroyStr, printStr, copyElement, ShellowDestroyElement, printElement, equalStr, transformIntoNumber, hashNumber);
	for(i=1; i <= getLengthList(jerriesList); i++){
		Jerry* currJerry = getDataByIndex(jerriesList, i);
		addToHashTable(jerriesHashTable, currJerry->ID , currJerry);
		//print_jerry(*currJerry);
		//displayList(jerriesList);

	}
	//printf("The hash table is: \n");
	//displayHashElements(jerriesHashTable);
	multiValueHashTable pcMultiHashTable = createMultiValueHashTable(copyStr, destroyStr, printStr, copyElement, ShellowDestroyElement,
			printElement, equalStr, transformIntoNumber, hashNumber, equalElements, equalID);
	for(i=1; i <= getLengthList(jerriesList); i++){
		Jerry* currJerry = getDataByIndex(jerriesList, i);
		for(j=0; j<currJerry->num_pc; j++){
			addToMultiValueHashTable(pcMultiHashTable, currJerry->Jerrypc[j]->pc_name, currJerry);
		    //displayMultiValueHashElementsByKey(pcMultiHashTable, currJerry->Jerrypc[j]->pc_name);
		}
	}
	//printf("The multi hash table is: \n");
	//displayMultiValueHashElementsByKey(pcMultiHashTable, "LimbsNumber");
	int happy;
	float valPc;
	char id[300], p_name[300], o_name[300], pc_name[300];
	Jerry* newJerry=NULL,*tmpJerry = NULL;
	Planet* tmPlanet = NULL;
	Origin* newOrigin = NULL;
	PhysicalCharacteristic* newPc = NULL;
	LinkedList foundJerries = NULL;
	KeyValuePair kvp;
	//menu section
	char ch[300];	//the input string
	strcpy(ch,"1"); //initialization
	char ch1 = '0';	//the char for cases

	while (strcmp(ch, "0") != 0) {

		printf("Welcome Rick, what are your Jerry's needs today ? \n");
		printf("1 : Take this Jerry away from me \n");
		printf("2 : I think I remember something about my Jerry \n");
		printf("3 : Oh wait. That can't be right \n");
		printf("4 : I guess I will take back my Jerry now \n");
		printf("5 : I can't find my Jerry. Just give me a similar one \n");
		printf("6 : I lost a bet. Give me your saddest Jerry \n");
		printf("7 : Show me what you got \n");
		printf("8 : Let the Jerries play \n");
		printf("9 : I had enough. Close this place \n");
		scanf("%s", ch);

		//check the input correctness.
		if(strlen(ch) != 1){
			ch1 = 'v';	//random char not between 0-9
		}
		else
		{
			ch1 = ch[0];
		}
		switch (ch1) {
		case '1': /* add new Jerry to the JerryBoree */

			//first question:
			printf("What is your Jerry's ID ? \n");
			scanf("%s", id);
			if(lookupInHashTable(jerriesHashTable, id) != NULL){
				printf("Rick did you forgot ? you already left him here ! \n");
				break;
			}
			//second question:
			printf("What planet is your Jerry from ? \n");
			scanf("%s", p_name);
			tmPlanet = Find_Planet(p_name, planet_arr, numberOfPlanets);
			if( tmPlanet == NULL){
				printf("%s is not a known planet ! \n", p_name);
				break;
			}

			//third question:
			printf("What is your Jerry's dimension ? \n");
			scanf("%s", o_name);

			//fourth question
			printf("How happy is your Jerry now ? \n");
			scanf("%d", &happy);

			//creating the new Jerry
			newOrigin = CreateOrigin(o_name, tmPlanet);
			newJerry = CreateJerry(id, happy, newOrigin);
			appendNode(jerriesList, newJerry);
			int len = getLengthList(jerriesList);
			Jerry* currJerry = getDataByIndex(jerriesList, len);
			print_jerry(*currJerry);
			addToHashTable(jerriesHashTable, currJerry->ID , currJerry);
			newJerry->JerryOrigin = NULL;
			Destroy_Jerry(newJerry);	//we make deep copy in linked list

			break;

		case '2': /*add pc to a Jerry */

			//first question:
			printf("What is your Jerry's ID ? \n");
			scanf("%s", id);
			if(lookupInHashTable(jerriesHashTable, id) == NULL){
				printf("Rick this Jerry is not in the daycare ! \n");
				break;
			}

			//second question:
			printf("What physical characteristic can you add to Jerry - %s ? \n", id);
			scanf("%s", pc_name);
			kvp = lookupInHashTable(jerriesHashTable, id);
			newJerry=(Jerry*)getValue(kvp);
			//print_jerry(*newJerry);
			foundJerries = lookupInMultiValueHashTable(pcMultiHashTable, pc_name);
			if( foundJerries != NULL){
				if(searchByKeyInList(foundJerries, id) != NULL){
					printf("The information about his %s already available to the daycare ! \n", pc_name);
					break;
				}
				printf("What is the value of the %s ? \n", pc_name);
				scanf("%f", &valPc);
				newPc = CreatePC(pc_name, valPc);

				add_pc(newJerry, newPc);
				addToMultiValueHashTable(pcMultiHashTable, pc_name, newJerry);
				print_jerry(*newJerry);
				displayMultiValueHashElementsByKey(pcMultiHashTable, pc_name);
				break;

			}
			printf("What is the value of the %s ? \n", pc_name);
			scanf("%f", &valPc);
			newPc = CreatePC(pc_name, valPc);
			add_pc(newJerry, newPc);
			addToMultiValueHashTable(pcMultiHashTable, pc_name, newJerry);
			displayMultiValueHashElementsByKey(pcMultiHashTable, pc_name);
			break;

		case '3': /* remove pc from jerry*/

			//first question:
			printf("What is your Jerry's ID ? \n");
			scanf("%s", id);
			kvp = lookupInHashTable(jerriesHashTable, id);
			newJerry=(Jerry*)getValue(kvp);
			if(newJerry == NULL){
				printf("Rick this Jerry is not in %s the daycare ! \n",id);
				break;
			}

			//second question:
			printf("What physical characteristic do you want to remove from Jerry - %s ? \n", id);
			scanf("%s", pc_name);
			foundJerries = lookupInMultiValueHashTable(pcMultiHashTable, pc_name);
			if( foundJerries != NULL){
				tmpJerry=(Jerry*)searchByKeyInList(foundJerries, id);
				if(tmpJerry== NULL){
					printf("The information about his %s not available to the daycare ! \n", pc_name);
					break;
				}
				removeFromMultiValueHashTable(pcMultiHashTable, pc_name, id);
				delete_pc(newJerry, pc_name);
				print_jerry(*newJerry);
				break;
			}
			printf("The information about his %s not available to the daycare ! \n", pc_name);
			break;

		case '4': /* remove Jerry from daycare */

			//first question:
			printf("What is your Jerry's ID ? \n");
			scanf("%s", id);
			kvp = lookupInHashTable(jerriesHashTable, id);
			newJerry=(Jerry*)getValue(kvp);
			if (newJerry == NULL) {
				printf("Rick this Jerry is not in the daycare ! \n");
				break;
			}
			for (i = 0; i < newJerry->num_pc; i++) {
				foundJerries = lookupInMultiValueHashTable(pcMultiHashTable, newJerry->Jerrypc[i]->pc_name);
				deleteNode(foundJerries, newJerry);
			}
			deleteNode(jerriesList, newJerry);
			removeFromHashTable(jerriesHashTable, id);
			printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
			break;

		case '5': /* adopt similar Jerry */
			//first question:
			printf("What do you remember about your Jerry ? \n");
			scanf("%s", p_name);
			foundJerries = lookupInMultiValueHashTable(pcMultiHashTable, p_name);
			if (foundJerries == NULL) {
				printf("Rick we can not help you - we do not know any Jerry's %s ! \n", p_name);
				break;
			}
			printf("What do you remember about the value of his %s ? \n", p_name);
			scanf("%f", &valPc);

			//ADD HERE IMPLEMENTATION OF THE SEARCH FOR THE RIGHT PC VALS
			tmpJerry = getDataByIndex(foundJerries, 1);
			int tmp, min = tmpJerry->Jerrypc[0]->pc_value - valPc;
			for (i = 1; i <= getLengthList(foundJerries); i++) {
				newJerry = getDataByIndex(foundJerries, i);
				for (j = 0; j < newJerry->num_pc; j++) {
					if (strcmp(newJerry->Jerrypc[j]->pc_name, p_name) == 0) {
						if (newJerry->Jerrypc[j]->pc_value - valPc < 0)
							tmp = -(newJerry->Jerrypc[j]->pc_value - valPc);
						else
							tmp = newJerry->Jerrypc[j]->pc_value - valPc;
						if (tmp < min) {
							min = tmp;
							tmpJerry = newJerry;
						}
					}

				}


			}


			printf("Rick this is the most suitable Jerry we found : \n");
			print_jerry(*tmpJerry);
			for (i = 0; i < tmpJerry->num_pc; i++) {
				foundJerries = lookupInMultiValueHashTable(pcMultiHashTable,
						tmpJerry->Jerrypc[i]->pc_name);
				deleteNode(foundJerries, tmpJerry);
			}
			deleteNode(jerriesList, tmpJerry);
			removeFromHashTable(jerriesHashTable, tmpJerry->ID);

			break;

		case '6': /* removes from the daycare the saddest Jerry */
			if (getLengthList(jerriesList) == 0) {
				printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
				break;
			}
			happy = SaddestJerry(jerriesList);
			newJerry = searchByKeyInList(jerriesList, &happy);
			printElement(newJerry);
			for (i = 0; i < newJerry->num_pc; i++) {
				foundJerries = lookupInMultiValueHashTable(pcMultiHashTable,
						newJerry->Jerrypc[i]->pc_name);
				deleteNode(foundJerries, newJerry);
			}
			deleteNode(jerriesList, newJerry);
			removeFromHashTable(jerriesHashTable, newJerry->ID);
			break;

		case '7': /*information about all the data in the system(Jerries, by PC, planets) SHOULD BE DEBBUGED */
			printf("What information do you want to know ? \n");
			printf("1 : All Jerries \n");
			printf("2 : All Jerries by physical characteristics \n");
			printf("3 : All known planets \n");
			scanf("%s", ch);

			//check the input correctness.
			if (strlen(ch) != 1 || (strcmp("1", ch) != 0 && strcmp("2", ch) != 0 && strcmp("3", ch) != 0)) {
				printf("Rick this option is not known to the daycare \n");
				ch1 = 'v';	//random char not between 0-9
				break;
			}
			else {
				ch1 = ch[0];
			}

			switch (ch1) {
			case '1':/*prints all the Jerries*/
				if (getLengthList(jerriesList) != 0) {
					displayList(jerriesList);
					break;
				}
				else{
					printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
					break;
				}

			case '2':/*prints all the Jerries with pc */
				printf("What physical characteristics ? \n");
				scanf("%s", pc_name);
				foundJerries = lookupInMultiValueHashTable(pcMultiHashTable, pc_name);
				if (foundJerries == NULL) {
					printf("Rick we can not help you - we do not know any Jerry's %s ! \n", pc_name);
					break;
				}
				displayList(foundJerries);
				break;

			case '3': /*prints all the planets in the system */
				for (i = 0; i < numberOfPlanets; i++) {
					print_planet(*planet_arr[i]);
				}
				break;
			}	//WHY WE NEED THIS
			break;	//WHY WE NEED THIS

		case '8': /* playing with jerries SHOULD BE DEBBUGED*/
			printf("What activity do you want the Jerries to partake in ? \n");
			printf("1 : Interact with fake Beth \n");
			printf("2 : Play golf \n");
			printf("3 : Adjust the picture settings on the TV \n");
			scanf("%s", ch);

			//check the input correctness.
			if (strlen(ch) != 1 || (strcmp("1", ch) != 0 && strcmp("2", ch) != 0 && strcmp("3", ch) != 0)) {
				printf("Rick this option is not known to the daycare \n");
				ch1 = 'v';	//random char not between 0-9
				break;
			}
			else {
				ch1 = ch[0];
			}

			switch (ch1) {
			case '1':	/*Interact with fake Beth*/
				for (i = 1; i <= getLengthList(jerriesList); i++) {	//run on all jerries
					newJerry = getDataByIndex(jerriesList, i);//get jerry i in list
					if (newJerry->Happinesslevel >= 20) {	//if he is happy
						if (newJerry->Happinesslevel > 80)	//if he is too happy
							newJerry->Happinesslevel = 100;
						else
							newJerry->Happinesslevel += 15;
					}
					else {
						if (newJerry->Happinesslevel < 5)	//if he is too sad
							newJerry->Happinesslevel = 0;
						else
							newJerry->Happinesslevel -= 5;
					}
				}
				break;

			case '2':	/* Play golf */
				for (i = 1; i <= getLengthList(jerriesList); i++) {	//run on all jerries
					newJerry = getDataByIndex(jerriesList, i);//get jerry i in list
					if (newJerry->Happinesslevel >= 50) {	//if he is happy
						if (newJerry->Happinesslevel > 90)	//if he is too happy
							newJerry->Happinesslevel = 100;
						else
							newJerry->Happinesslevel += 10;
					} else {
						if (newJerry->Happinesslevel < 10)	//if he is too sad
							newJerry->Happinesslevel = 0;
						else
							newJerry->Happinesslevel -= 10;
					}
				}

				break;

			case '3':	/* Adjust the picture settings on TV */
				for (i = 1; i <= getLengthList(jerriesList); i++) {	//run on all jerries
					newJerry = getDataByIndex(jerriesList, i);//get jerry i in list
					if (newJerry->Happinesslevel > 80)	//if he is too happy
						newJerry->Happinesslevel = 100;
					else
						newJerry->Happinesslevel += 20;
				}
				break;
			}
			break;

		case '9': /* close the daycare and delete EVERYTHING  */

			destroyMultiValueHashTable(pcMultiHashTable);
			destroyHashTable(jerriesHashTable);
			destroyList(jerriesList);
			for (i = 0; i < numberOfPlanets; i++) {
				Destroy_Planet(planet_arr[i]);
			}
			free(planet_arr);
			 strcpy(ch,"0");
			 printf("The daycare is now clean and close !");
			break;
		default:
			printf("Rick this option is not known to the daycare ! \n");
			break;
		}
	}
	return 0;
}

/*---------------------------------------------------------------------------------------------*/
//functions that call the Jerry functions with the elements they got
//instead of changing the whole Jerry module

//copyFunction
Element copyElement(Element j){
	Jerry* newJerry = (Jerry*)j;
	Jerry* copyOfJerry = copyJerry(newJerry);
	return copyOfJerry;
}

//pringFunction
status printElement(Element j){
	Jerry* newJerry = (Jerry*)j;
	return print_jerry(*newJerry);
}

//freeFunction
status DestroyElement(Element j){
	Jerry* newJerry = (Jerry*)j;
	return Destroy_Jerry(newJerry);
}
status ShellowDestroyElement(Element j){
	j=NULL;
	return success;

}
//equalFunction
bool equalElements(Element j1, Element j2){
	Jerry* jerry1 = (Jerry*)j1;
	Jerry* jerry2 = (Jerry*)j2;
	return equalJerries(jerry1, jerry2);
}
//~~~~~~~~~~~~~~~~~~~~~~~~ FOR ADT hash and multi tables functions ~~~~~~~~~~~~
//copy function for string keys - DO WE NEED HERE MALLOC???
Element copyStr(Element s){
	if(s == NULL)
		return NULL;
	char* str = (char*)s;
	char* newStr = (char*)malloc(sizeof(str));
	return strcpy(newStr, str);
}

//prints a string key
status printStr(Element str){
	if(str == NULL)
		return failure;
	char* newStr = (char*)str;
	printf("%s\n", newStr);
	return success;

}

//frees a string of key
status destroyStr(Element str) {
	if (str == NULL)
		return failure;
	str = NULL;
	return success;
}

//compares between two string keys
bool equalStr(Element str1, Element str2) {
	if (str1 == NULL || str2 == NULL)
		return false;
	char *newStr1 = (char*) str1;
	char *newStr2 = (char*) str2;
	return strcmp(newStr1, newStr2) == 0 ? true : false;
}

//isPartEqual function for a list of Jerries and a given ID
bool equalID(Element j1, Element id ){
	if(j1 == NULL || id == NULL)
		return false;
	Jerry* currJerry = (Jerry*)j1;
	char* ID = (char*)id;
	return (strcmp(currJerry->ID, ID) == 0);
}


//----------------------------------------- HELPER FUNCS: --------------------------------------------------
//HELPER: CHECK WHETHER NUBER IS PRIME OR NOT - PROBLEM HERE NOT SURE WHY

//PROBLEM HERE!
bool isPrime(int num) {
	if (num == 1 || num == 2)
		return true;
	int i, k;
	//k = (int)sqrt(num);	//doesnt work well
	k=num;
	for (i = 2; i < k; i++)
		if (num % i == 0)
			return false;
	return true;
}

//HELPER: GETS A NUMBER, RETURNS THE CLOSEST PRIME NUMBER FROM ABOVE
int sizeOfTable(int num) {
	while (1) {
		if (isPrime(num) == true) //warning here! not sure why
			return num;
		num++;
	}
}

//transforms a string into number
int transformIntoNumber(Element s){
	if(s == NULL)
		return 0;
	char* str = (char*)s;
	int i, sum = 0;
	for (i=0; i<strlen(str) ; i++){
		sum = sum + str[i];
	}
	return sum;
}

// finds a planet by it's name in an array of Planets and returns its pointer.
Planet* Find_Planet(char *p_name, Planet **planets_arr, int num_Of_Planets) {
	if (p_name == NULL || planets_arr == NULL)
		return NULL;
	int i;
	for (i = 0; i < num_Of_Planets; i++) {
		if (strcmp(planets_arr[i]->Planet_name, p_name) == 0) {

			return planets_arr[i];
		}
	}
	return NULL;
}

//returns the most identical jerry of all this jerries by a pc value
Jerry* MostFamiliar(LinkedList jerriesList,  PhysicalCharacteristic* pc) {
	if (jerriesList == NULL || pc == NULL)
		return NULL;
	int i, k;
	Jerry *tmpj = NULL;
	//float mostIdenticalVal;
	for (i = 1; i <= getLengthList(jerriesList); i++) {
		Jerry *tmpj = getDataByIndex(jerriesList, i);
		for( k=0;  k<tmpj->num_pc;k++){
			if(pc->pc_value == tmpj->Jerrypc[k]->pc_value){
				//func for menu number 5!!!! not sure how to implement
			}
		}
	}
	return tmpj;
}

//returns the lowest level of happiness in list of Jerries
int SaddestJerry(LinkedList jerriesList) {
	if (jerriesList == NULL)
		return 0;
	int i, happiness;
	Jerry *tmpj = getDataByIndex(jerriesList, 1);
	happiness = tmpj->Happinesslevel;	//initial level of happiness
	for (i = 2; i <= getLengthList(jerriesList); i++) {
		tmpj = getDataByIndex(jerriesList, i);
		happiness = MIN(happiness, tmpj->Happinesslevel);
	}
	return happiness;
}

//checks if jerry's happiness is the same as given value - use it in func saddestJerry
//also - it's the isPartWqual we sent to ADT linkedList to create list of Jerries
//expect to get Jerry* and int as parameters
bool equalHappiness(Element j1, Element happiness) {
	if (j1 == NULL || happiness == NULL)
		return false;
	Jerry* newJerry = (Jerry*)j1;
	int* happy = (int*)happiness;
	return newJerry->Happinesslevel == *happy ? true : false;
}
