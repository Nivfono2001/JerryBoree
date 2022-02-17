#include "Jerry.h"

Jerry* CreateJerry(char *ID, int happiness, Origin * JerryOrigin) {
	if (ID == NULL)
		return NULL;
	Jerry *j = (Jerry*) malloc(sizeof(Jerry));
	if (j == NULL)
		return NULL;	//check if the memory allocation was successful
	int length = strlen(ID) + 1;
	j->ID = (char*) malloc(sizeof(char) * length);
	if (j->ID == NULL){		//check if the memory allocation was successful
		free(j);
		return NULL;
	}
	strcpy(j->ID, ID);
	j->Happinesslevel = happiness;
	j->JerryOrigin = JerryOrigin;
	j->num_pc = 0;
	j->Jerrypc = NULL;
	return j;
}

Planet* CreatePlanet(char *name, float x, float y, float z) {
	if (name == NULL)
		return NULL;
	Planet *p = (Planet*) malloc(sizeof(Planet));
	if (p == NULL)
		return NULL;
	int length = strlen(name) + 1;
	p->Planet_name = (char*) malloc(sizeof(char) * length);
	if (p->Planet_name == NULL){
		free(p);
		return NULL;
	}
	strcpy(p->Planet_name, name);
	p->x = x;
	p->y = y;
	p->z = z;
	return p;
}

PhysicalCharacteristic* CreatePC(char *name, float value) {
	if (name == NULL)
		return NULL;
	PhysicalCharacteristic *pc = (PhysicalCharacteristic*) malloc(sizeof(PhysicalCharacteristic));
	if (pc == NULL)
		return NULL;
	int length = strlen(name) + 1;
	pc->pc_name = (char*) malloc(sizeof(char) * length);
	if (pc->pc_name == NULL) {
		free(pc);
		return NULL;
	}
	strcpy(pc->pc_name, name);
	pc->pc_value = value;
	return pc;
}

Origin* CreateOrigin(char* name, Planet * oplanet) {
	if (name == NULL)
		return NULL;
	Origin *o = (Origin*) malloc(sizeof(Origin));
	if (o == NULL)
		return NULL;
	int length = strlen(name) + 1;
	o->dim_name = (char*) malloc(sizeof(char) * length);
	if (o->dim_name == NULL){
		free(o);
		return NULL;
	}
	strcpy(o->dim_name, name);
	o->origin_planet = oplanet;
	return o;
}

status IspcExist(Jerry j, char *pcname, bool *result) {
	if (pcname == NULL )
		return failure;
	int i;
	for (i = 0; i < j.num_pc; i++) {
		if (strcmp(j.Jerrypc[i]->pc_name, pcname) == 0) {
			*result = true;
			return success;
		}
	}
	*result = false;
	return failure;
}


status add_pc(Jerry *j, PhysicalCharacteristic * pc) {
	if (j == NULL)
		return failure;
	bool result;
	int totalSize;
	totalSize = sizeof(PhysicalCharacteristic*) * (j->num_pc + 1);
	if (j->num_pc == 0) {	//in case its the first pc we add
		j->Jerrypc = (PhysicalCharacteristic**) malloc(totalSize);
		if(j->Jerrypc == NULL)
			return failure;
	}
	else {	//in case its not the first pc we add
		if (IspcExist(*j, pc->pc_name, &result) == success) return failure;
		j->Jerrypc = (PhysicalCharacteristic**) realloc(j->Jerrypc, totalSize);
		if(j->Jerrypc == NULL)
			return failure;
	}
	j->Jerrypc[j->num_pc] = pc;	//add new pc in the end of array
	j->num_pc++;	//update num of pc
	return success;
}

status delete_pc(Jerry *j, char *name) {
	if (j == NULL || name == NULL)
		return failure;
	else {
		int totalSize, i, k;
		totalSize = sizeof(PhysicalCharacteristic*) * (j->num_pc - 1);
		PhysicalCharacteristic **newPcArray = (PhysicalCharacteristic**) malloc(totalSize);
		if (newPcArray == NULL)
			return failure;
		for (i = 0, k = 0; i < j->num_pc && k < j->num_pc - 1; i++, k++) {
			if (strcmp(j->Jerrypc[i]->pc_name, name) != 0)
				newPcArray[k] = j->Jerrypc[i];
			else{
				Destroy_PC(j->Jerrypc[i]); //destroy and free the deleted pc
				k = k - 1;
			}
		}
		if(k == i){		//in case the wanted pc is the last one
			Destroy_PC(j->Jerrypc[i]); //destroy and free the deleted pc
		}
		free(j->Jerrypc);	//free the old pc array pointer
		j->Jerrypc = newPcArray;	//new pc array pointer
		j->num_pc = j->num_pc - 1;	//update the num of pc
		return success;
	}
}

status print_planet(Planet p) {
	printf("Planet : %s (%.2f,%.2f,%.2f) \n", p.Planet_name, p.x, p.y, p.z);
	return success;
}

status print_jerry(Jerry j) {
	printf("Jerry , ID - %s : \n", j.ID);
	printf("Happiness level : %d \n", j.Happinesslevel);
	printf("Origin : %s \n", j.JerryOrigin->dim_name);
	print_planet(*j.JerryOrigin->origin_planet);
	if (j.num_pc > 0) {
		printf("Jerry's physical Characteristics available : \n");
		int i;
		printf("\t");
		for (i = 0; i < j.num_pc; i++) {
			if (i != j.num_pc - 1) {

				printf("%s : %.2f , ", j.Jerrypc[i]->pc_name, j.Jerrypc[i]->pc_value);
			} else {

				printf("%s : %.2f \n", j.Jerrypc[i]->pc_name, j.Jerrypc[i]->pc_value);
			}
		}
	}
	return success;
}

//destroys a planet (it's inner pointers and itself)
status Destroy_Planet(Planet *p) {
	if (p == NULL)
		return failure;
	free(p->Planet_name);
	free(p);
	return success;
}


//destroys an origin (it's inner pointers and itself)
status Destroy_Origin(Origin *o) {
	if (o == NULL)
		return failure;
	free(o->dim_name);
	free(o);
	return success;
}

//destroys a pc (it's inner pointers and itself)
status Destroy_PC(PhysicalCharacteristic *pc) {
	if (pc == NULL)
		return failure;
	free(pc->pc_name);
	free(pc);
	return success;
}

//destroys a Jerry (it's inner pointers and itself)
status Destroy_Jerry(Jerry *j) {
	if (j == NULL)
		return failure;
	Destroy_Origin(j->JerryOrigin);
	for (int i = 0; i<j->num_pc; i++) {
		Destroy_PC(j->Jerrypc[i]);
	}
	free(j->Jerrypc);
	free(j->ID);
	free(j);
	return success;
}

//--------------------------FUNCTIONS FOR ADT LINKED LIST OF JERRIES------------------------
//copies a jerry - returns a pointer to Jerry
Element copyJerry(Jerry *j){
	if(j == NULL)
		return NULL;
	Jerry *newjerry=CreateJerry(j->ID, j->Happinesslevel, j->JerryOrigin);
	int i;
	PhysicalCharacteristic *tmp_pc=NULL;
	for(i=0;i<j->num_pc;i++){
		tmp_pc=(j->Jerrypc[i]);
		add_pc(newjerry, tmp_pc);
	}
	return (Element)newjerry;
}

//compares between two jerries by their ID
bool equalJerries(Jerry* j1, Jerry* j2){
	if(j1 == NULL || j2 == NULL)
		return false;
	if(strcmp(j1->ID, j2->ID) == 0)
		return true;
	return false;
}
