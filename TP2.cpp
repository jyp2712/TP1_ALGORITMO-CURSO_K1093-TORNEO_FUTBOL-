#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include "torneo.h"
 
#define MAX_EQUIPOS 100 // cantidad de equipos

using namespace std;

/* ******************************************************************
 *                     	 	 TEMPLATES
 * *****************************************************************/

template <typename T>
struct Nodo{
	T info;
	Nodo <T> *siguiente;
};


/* ******************************************************************
 *                     	   CONSTANTES
 * *****************************************************************/

enum  {
	Salir, // 0
	Agregar,// 1
	Eliminar,// 2
	Ver,// 3
};


/* ******************************************************************
 *                     	  TIPOS DE DATOS
 * *****************************************************************/

typedef struct {
	char ID[4];
	char nombre[32];
	int PotAtaque;
	int PotDefensa;
} t_equipo;

typedef struct {
	char localID[4];
	char localnombre[32];
	char visitanteID[4];
	char visitantenombre[32];
	int GolesL;
	int GolesV;
	int fecha;
} t_fixture;

typedef struct {
	char equipoID[4];
	char equipoNombre[32];
	int Puntos;
	int PJ;
	int PG;
	int PE;
	int PP;
	int GF;
	int GC;
	int DG;
} t_tabla;

/* ******************************************************************
 *                     	   PROTOTIPOS DE
 *                           FUNCIONES
 * *****************************************************************/

void Pasar_Archivo_Vector(t_equipo[], int&);

void MenuPrincipal (t_equipo[], int);

void MenuEditar (t_equipo[], int&);

void Agregar_equipo(t_equipo[], int&);

void Ingresar_equipo (t_equipo&);

void Buscar_equipo(t_equipo[], t_equipo, int, bool &);

void Modificar_equipo(t_equipo[], int, t_equipo);

void Eliminar_equipo(t_equipo[], int&);

void Ver_equipo(t_equipo[], int);

void Grabar_Archivo (t_equipo[], int);

void Simulacion (t_equipo [], int);

void Ordenamiento_Aleatorio (t_equipo [], int);

void CalcularFechasPartidos (int, int&, int&, int&, int&);

void SimularResultados (t_equipo [], int, int, int, int &, int, Nodo <t_fixture> *&);

void CargarDatosPartido (t_equipo [], int, int, int&, int&, int&, int&);

void CargarResultados (t_equipo [], int, int, int, int, int, Nodo <t_fixture> *&);

void InsertarEnLista (Nodo <t_fixture> *&, t_fixture);

void MenuSimular (Nodo <t_fixture> *, int);

void CalcularTabla (Nodo <t_fixture> *, int);

void actualizarEquipoEnTabla (char [], char [], int, int, t_tabla []);

void inicializarVector (t_tabla [], int);

void puntuarTabla(t_tabla [], int);

void buscaoInsertaEquipo (char [], char [], t_tabla [], int &);

void ordenarTabla(t_tabla [], int);

void mostrarTabla (t_tabla [], int);

void partidosPorEquipo (Nodo <t_fixture> *);

void MostrarPorFecha(Nodo <t_fixture> *);

void mostrarPartido(Nodo <t_fixture> *);

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

int main (){

int cantequipos = 0;
t_equipo equipo[MAX_EQUIPOS];


	Pasar_Archivo_Vector (equipo, cantequipos);
	MenuPrincipal(equipo, cantequipos);
	Grabar_Archivo (equipo, cantequipos);

return 0;
}

/* ******************************************************************
 *                     		FUNCIONES
 * *****************************************************************/

void Pasar_Archivo_Vector (t_equipo vector[], int &cant){

t_equipo team;
FILE *archivo;

	archivo = fopen ("Equipos.bin", "a+b");

	if (archivo != NULL){
		
	fread (&team, sizeof(t_equipo),1,archivo);

		while (!feof(archivo)){

			vector[cant] = team;
			cant++;
			fread (&team, sizeof(t_equipo),1,archivo);
		}
	}

	
	fclose(archivo);
}

void MenuPrincipal(t_equipo vec[], int cantvec)
{
int opc;
	
	do{
	printf("+--------------------------------+ \n");
	printf("|         MENU PRINCIPAL         | \n");
	printf("+--------------------------------+ \n");
	printf("|  1. Editar Equipos             | \n");
	printf("|  2. Simular                    | \n");
	printf("|                                | \n");
	printf("|  0. Salir                      | \n");
	printf("+--------------------------------+ \n");
	
	cout<<"Ingrese la opcion: ";
	cin>>opc;

	switch(opc)
	{
		case 1:
			MenuEditar(vec, cantvec);
		break;
		
		case 2:
			Simulacion(vec, cantvec);
		break;
	}
	}while (opc!=0);
}

void MenuEditar (t_equipo vector[], int &cant){

int numero;

	do{
		
		cout<<"1. Agregar Equipo"<<endl;
		cout<<"2. Eliminar Equipo"<<endl;
		cout<<"3. Ver Equipo"<<endl;
		cout<<"0. Salir"<<endl;
		cout<<"Elija una opciÃ³n: ";

		cin>>numero;

		switch (numero){

			case Agregar: 	cout<<endl;
					Agregar_equipo (vector, cant);
					break;
			case Eliminar: 	cout<<endl;
					Eliminar_equipo (vector, cant);
					break;
			case Ver: 	cout<<endl;
					Ver_equipo (vector, cant);
					break;
			case Salir: 	cout<<"Salir"<<endl;
					break;
			default:
					cout<<"OpciÃ³n incorrecta"<<endl;
					break;
		}
			
	}while(numero!=0);
}


void Agregar_equipo (t_equipo vector[], int &cant){

t_equipo team;
bool existe = false;

	Ingresar_equipo (team);
	Buscar_equipo(vector, team, cant, existe);
	
	if ((!existe) and (cant<MAX_EQUIPOS)){

		vector[cant] = team;
		cant++;
	}else{

		if ((!existe) and (cant>=MAX_EQUIPOS)){

			cout<<endl<<"Ya hay "<<MAX_EQUIPOS<<" equipos, no puede agregar mÃ¡s"<<endl;
		}

	}
}

void Ingresar_equipo (t_equipo &team){

	cout<<"Ingrese ID"<<endl;
	cin>>team.ID;
	cout<<"Ingrese nombre"<<endl;
	cin>>team.nombre;
	cout<<"Ingrese potencia de ataque"<<endl;
	cin>>team.PotAtaque;
	cout<<"Ingrese potencia de defensa"<<endl;
	cin>>team.PotDefensa;

}

void Buscar_equipo(t_equipo vector[], t_equipo team, int cant, bool &existe){

int i;

	for (i =0; i <cant; i++){

		if (strcmp(vector[i].ID,team.ID) ==0){

			existe = true;
			Modificar_equipo(vector, i, team);
		}
	}
}

void Modificar_equipo(t_equipo vector[], int posicion, t_equipo team){

char modificar;

	cout<<endl<<"Desea modificar el equipo S/N"<<endl;
	cin>>modificar;

	if (modificar == 'S'){

		vector[posicion] = team;
	}

}

void Eliminar_equipo (t_equipo vector[], int &cant){

char identificador[4];
int pos =0;
bool existe = false;

	cout<<"Ingrese ID"<<endl;
	cin>>identificador;

	while (!existe && pos < cant){
		if (strcmp(vector[pos].ID, identificador) == 0){
			existe = true;
		}
		else {
			pos++;
		}
	}
	if (existe) {
		cant--;
		for (int i = pos; i < cant; i++) {
			vector[i] = vector[i + 1];
		}	
	}
	else{
		cout<<endl<<"No existe el equipo que desea eliminar"<<endl;
	}
}


void Ver_equipo (t_equipo vector[], int cant){

int i;
	if (cant >0){

		cout<<endl<<"Lista de equipos:"<<endl;

		for (i =0; i <cant; i++){
					
			cout<<endl;
			cout<<"ID equipo: "<<vector[i].ID<<endl;
			cout<<"Nombre equipo: "<<vector[i].nombre<<endl;
			cout<<"Potencia ataque equipo: "<<vector[i].PotAtaque<<endl;
			cout<<"Potencia defensa equipo: "<<vector[i].PotDefensa<<endl;
		}

	}else{

		cout<<endl<<"No hay equipos"<<endl;

	}
	
}

void Grabar_Archivo (t_equipo vector[], int cant){

int i;
t_equipo team;
FILE *archivo;

	archivo = fopen ("Equipos.bin", "wb");
	
	for (i = 0; i <cant; i++){

		team = vector[i];
		fwrite (&team, sizeof(t_equipo),1,archivo);

	}
	
	fclose(archivo);
}

void Simulacion (t_equipo vector[], int cant){

int cantfechas, cantpartidos, fechaReal, fechaAleatoria, fechaTemp;
Nodo <t_fixture> *fixture = NULL;

	Ordenamiento_Aleatorio (vector, cant);
	CalcularFechasPartidos (cant, cantfechas, cantpartidos, fechaReal, fechaAleatoria);
	fechaTemp=fechaAleatoria+1;
	SimularResultados (vector, cant, fechaTemp, cantfechas, fechaReal, cantpartidos, fixture);
	fechaTemp=1;
	SimularResultados (vector, cant, fechaTemp, fechaAleatoria, fechaReal, cantpartidos, fixture);
	MenuSimular (fixture, cant);
}	

void Ordenamiento_Aleatorio (t_equipo vector[], int cant){

int i, j, k;
t_equipo aux;

	srand (time(NULL));
	for (i=1; i<cant; i++){
		j = rand () % cant;
		k = rand () % cant;
		aux = vector [j];
		vector [j] = vector [k];
		vector [k] = aux;
	}				
}

void CalcularFechasPartidos (int cant, int &cantfechas, int &cantpartidos, int &fechaReal, int &fechaAleatoria){

	if (cant%2 ==0){
		cantfechas=cant-1;
	}else{
		cantfechas=cant;
	}
	cantpartidos=cant/2;
	fechaReal=1;
	srand (time(NULL));
	fechaAleatoria=rand() % cantfechas;
}

void SimularResultados (t_equipo vector[], int cant, int fechaInicial, int fechaFinal, int &fechaReal, int partidos, Nodo <t_fixture> *&fixture){

int i, partido, local, visitante, ataqueL, ataqueV, DefL, DefV, golesL, golesV;
	
	for (i=fechaInicial; i<=fechaFinal; i++){
		for (partido=1; partido<=partidos; partido++){
			generarPartido (cant, i, partido, local, visitante);
			CargarDatosPartido (vector, local, visitante, ataqueL, ataqueV, DefL, DefV);
			simularPartido (ataqueL, ataqueV, DefL, DefV, golesL, golesV);
			CargarResultados (vector, local, visitante, golesL, golesV, fechaReal, fixture);
		}
	fechaReal++;
	}
}

void CargarDatosPartido (t_equipo vec[], int local, int visitante, int &ataqueL, int &ataqueV, int &DefL, int &DefV){

	ataqueL=vec[local].PotAtaque;
	DefL=vec[local].PotDefensa;
	ataqueV=vec[visitante].PotAtaque;
	DefV=vec[visitante].PotDefensa;
}

void CargarResultados (t_equipo vec[], int local, int visitante, int GL, int GV, int fecha, Nodo <t_fixture> *&fixture){

t_fixture partido;
	
	strcpy(partido.localID, vec[local].ID);
	strcpy(partido.localnombre, vec[local].nombre);
	strcpy(partido.visitanteID, vec[visitante].ID);
	strcpy(partido.visitantenombre, vec[visitante].nombre);
	partido.GolesL=GL;
	partido.GolesV=GV;
	partido.fecha=fecha;
	InsertarEnLista (fixture, partido);
}

void InsertarEnLista (Nodo <t_fixture> *&lista, t_fixture info){

Nodo <t_fixture> *aux, *ant;
	aux = lista;
	while ((aux != NULL) and (aux->info.fecha<=info.fecha)){
		ant = aux;
		aux = aux->siguiente;
	}
	
	Nodo <t_fixture> *nuevo = new Nodo <t_fixture>;
	nuevo->info = info;
	nuevo->siguiente = aux;

	if (aux!= lista){
		ant->siguiente = nuevo;
	}else{
		lista = nuevo;
	}
}

void MenuSimular(Nodo <t_fixture> *lista, int cant)
{
int opc;

	do{
	printf("+--------------------------------+ \n");
	printf("|           MENU SIMULAR         | \n");
	printf("+--------------------------------+ \n");
	printf("|  1. Ver tabla de posiciones    | \n");
	printf("|  2. Ver partidos por equipo    | \n");
	printf("|  3. Ver partidos por fecha     | \n");
	printf("|  0. Volver                     | \n");
	printf("+--------------------------------+ \n");
	
	printf("Ingrese la opcion: ");
	scanf("%d",&opc);
	
	switch(opc)
	{
		case 1:
			CalcularTabla(lista, cant);
		break;
		
		case 2:
			partidosPorEquipo(lista);
		break;
		
		case 3:
			MostrarPorFecha(lista);
		break;
	}
	}while (opc!=0);
}

void CalcularTabla (Nodo <t_fixture> *fixture, int cantequipos){
	
	int fecha;
	
	cout<<"Ingrese una fecha: ";
	
	cin>>fecha;
	
	t_tabla vector_tabla[cantequipos];
	inicializarVector (vector_tabla, cantequipos);
	
	while ((fixture!=NULL) && (fixture->info.fecha <= fecha)){
		
	actualizarEquipoEnTabla (fixture->info.localID, fixture->info.localnombre, fixture->info.GolesL, fixture->info.GolesV, vector_tabla);
	actualizarEquipoEnTabla (fixture->info.visitanteID, fixture->info.visitantenombre, fixture->info.GolesV, fixture->info.GolesL, vector_tabla);
	
	fixture = fixture->siguiente;
	}
	
	puntuarTabla(vector_tabla, cantequipos);
	ordenarTabla(vector_tabla, cantequipos);
	mostrarTabla(vector_tabla, cantequipos);
}

void actualizarEquipoEnTabla (char equipoID[], char equiponombre[], int golesConv, int golesRec, t_tabla vector_tabla[]){
	
	int pos;
	
	buscaoInsertaEquipo (equipoID, equiponombre, vector_tabla, pos);
	if(golesConv>golesRec){
		vector_tabla[pos].PG++;
	}else{if(golesConv==golesRec){
		vector_tabla[pos].PE++;
	}else{vector_tabla[pos].PP++;
	}}
	
	vector_tabla[pos].GF = vector_tabla[pos].GF + golesConv;
	vector_tabla[pos].GC = vector_tabla[pos].GC + golesRec;
}

void inicializarVector (t_tabla vector_tabla[], int cant){

int i;
for (i =0; i <cant; i++){
	strcpy(vector_tabla[i].equipoID, "VAC");
	vector_tabla[i].Puntos = 0;
	vector_tabla[i].PJ = 0;
	vector_tabla[i].PG = 0;
	vector_tabla[i].PE = 0;
	vector_tabla[i].PP = 0;
	vector_tabla[i].GF = 0;
	vector_tabla[i].GC = 0;
	vector_tabla[i].DG = 0;
}
}

void puntuarTabla(t_tabla vector_tabla[], int CANT_EQUIPOS){

int i;
for (i =0; i <CANT_EQUIPOS; i++){
	vector_tabla[i].PJ = vector_tabla[i].PG + vector_tabla[i].PE + vector_tabla[i].PP;
	vector_tabla[i].DG = vector_tabla[i].GF - vector_tabla[i].GC;
	vector_tabla[i].Puntos = vector_tabla[i].PG*3+vector_tabla[i].PE;
}
}

void ordenarTabla(t_tabla vec[], int cant){
int i,j;
t_tabla aux;
		for (i=0;i<cant-1;i++){
			for (j=i+1;j<cant;j++){
				if (vec[i].Puntos<vec[j].Puntos){
					aux=vec[i];
					vec[i]=vec[j];
					vec[j]=aux;
				}else{
					if ((vec[i].Puntos==vec[j].Puntos) && (vec[i].DG<vec[j].DG)){
						aux=vec[i];
						vec[i]=vec[j];
						vec[j]=aux;
					}else{
						if ((vec[i].Puntos==vec[j].Puntos) && (vec[i].DG==vec[j].DG) && (vec[i].GF<vec[j].GF)){
							aux=vec[i];
							vec[i]=vec[j];
							vec[j]=aux;
						}
					}
				}
			}
		}

}

void buscaoInsertaEquipo (char equipoID[], char equipoNombre[], t_tabla vector_tabla[], int &pos){
	
	int i = 0;
	
	while((strcmp(vector_tabla[i].equipoID,"VAC")!=0) && (strcmp(vector_tabla[i].equipoID,equipoID)!=0)){
		i++;
	}
	
	if(strcmp(vector_tabla[i].equipoID, equipoID)==0){
		pos = i;
	}else{
		strcpy(vector_tabla[i].equipoID, equipoID);
		strcpy(vector_tabla[i].equipoNombre, equipoNombre);
		pos = i;
	}
}

void mostrarTabla (t_tabla vector_tabla[], int CANT_EQUIPOS){

int i;	
	cout<<"Tabla de posiciones"<<endl;
	cout<<"Equipo		Puntos	PJ	PG	PE	PP	GF	GC	DG"<<endl;

	for (i=0; i <CANT_EQUIPOS; i++){
	cout<<vector_tabla[i].equipoID;	cout<<" "<<vector_tabla[i].equipoNombre<<"	"<<vector_tabla[i].Puntos<<"	"<<vector_tabla[i].PJ<<"	"<<vector_tabla[i].PG<<"	"<<vector_tabla[i].PE<<"	"<<vector_tabla[i].PP<<"	"<<vector_tabla[i].GF<<"	"<<vector_tabla[i].GC<<"	"<<vector_tabla[i].DG<<endl;
	}
}

void partidosPorEquipo (Nodo <t_fixture> *fixture){

	char equipoID[4];

	cout<<"Ingrese un equipo: ";

	cin>>equipoID;
	
cout<<"Partidos de ";	cout<<equipoID<<endl;

while (fixture != NULL){

	if((strcmp(fixture->info.localID, equipoID)==0) || (strcmp(fixture->info.visitanteID, equipoID)==0)){
		mostrar(fixture);
	}
	fixture = fixture->siguiente;
	}
}

void MostrarPorFecha(Nodo <t_fixture> *fixture)
{
int fecha;

	cout<<"Ingrese la fecha: ";
	cin>>fecha;
	cout<<"Partidos de la fecha "<<fecha<<endl;
	while(fixture!=NULL)
	{
		if(fixture->info.fecha==fecha)
		{
			mostrarPartido(fixture);
		}
		fixture=fixture->siguiente;
	}
}

void mostrarPartido(Nodo <t_fixture> *resultado)
{
	cout<<resultado->info.localID<<" ";
	cout<<resultado->info.localnombre<<"	";
	cout<<resultado->info.GolesL<<"-";
	cout<<resultado->info.GolesV<<" ";
	cout<<resultado->info.visitanteID<<" ";
	cout<<resultado->info.visitantenombre<<""<<endl;
}
