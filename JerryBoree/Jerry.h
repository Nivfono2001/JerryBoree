#include "Defs.h"

//-----------------------STRUCTS DECLARATION------------------------------------------

/* definition of a planet of jerry's e.g C137*/
typedef struct Planet_t {
	char *Planet_name;
	float x, y, z;
} Planet;

/* definition of an origin of jerry's */
typedef struct Origin_t {
	char *dim_name;
	Planet * origin_planet;
} Origin;

/* definition of a physical characteristic of jerry's */
typedef struct PhysicalCharacteristic_t {
	char *pc_name;
	float pc_value;
} PhysicalCharacteristic;

/* definition of a Jerry */
typedef struct Jerry_t {
	char *ID;
	int Happinesslevel;
	Origin * JerryOrigin;
	int num_pc;
	PhysicalCharacteristic ** Jerrypc;
} Jerry;

//------------------------FUNCTION DECLARATION---------------------------------------

//function creates a Jerry and returns status if secceeded.
Jerry* CreateJerry(char *ID, int happines, Origin * JerryOrigin);

//function destroys a Jerry.
status Destroy_Jerry(Jerry *j);

//function creates a Planet and returns status if secceeded.
Planet* CreatePlanet(char *name, float x, float y, float z);

//function destroys a Planet.
status Destroy_Planet(Planet *p);

//function creates a Physical characteristic and returns status if secceeded.
PhysicalCharacteristic* CreatePC(char *name, float value);

//function destroys a Physical characteristic.
status Destroy_PC(PhysicalCharacteristic *pc);

//function creates an Origin and returns status if secceeded.
Origin* CreateOrigin(char *name, Planet * oplanet);

//function destroys an Origin.
status Destroy_Origin(Origin *o);

//function checks if a Pc exists in a Jerry - T\F and returns status if secceeded.
status IspcExist(Jerry j, char *pcname, bool *result);

//function adds a pc to a Jerry.
status add_pc(Jerry *j, PhysicalCharacteristic * pc);

//function deletes a pc from a Jerry
status delete_pc(Jerry *j, char *name);

//function prints a Jerry.
status print_jerry(Jerry j);

//function prints a Planet.
status print_planet(Planet p);

Element copyJerry(Jerry *j);

bool equalJerries(Jerry* j1, Jerry* j2);
