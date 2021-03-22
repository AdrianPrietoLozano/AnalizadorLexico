#include <iostream>
#include <string>
#include <iomanip>
#include <ctype.h>
using namespace std;
#define ERROR 300
#define ACEPTAR 301
#define I 0

int pos = 0;

char obtenerChar( string );
void error( void );
bool esAgrupacion( char );
bool esOpeAritmetico( char );
string verificarToken( string );
void imprimirToken( string, int );
void retrocederUno( void );
int obtenerRecorrido( int[], string );

int tabla[29][19] = {{1, 1, 3, 10, 11, 2, 13, 18, 16, 23, 21, 25, I, I, I, 20, 15, I, ERROR}, //0
					{ 1, 1, 1,  I,  I, I,  I,  I,  I,  I,  I,  1, I, I, 1,  I,  I, I, ACEPTAR}, //1, 
					{ I, I, 3,  I,  I, 9,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //2,
					{ I, I, 3,  I,  I, I,  I,  I,  I,  I,  I,  3, 4, 6, I,  I,  I, I, ACEPTAR}, //3, 
					{ I, I, 5,  I,  I, I,  I,  I,  I,  I,  I,  5, I, I, I,  I,  I, I, ERROR}, //4, 
					{ I, I, 5,  I,  I, I,  I,  I,  I,  I,  I,  5, I, 6, I,  I,  I, I, ACEPTAR}, //5
					{ I, I, 8,  I,  7, 7,  I,  I,  I,  I,  I,  8, I, I, I,  I,  I, I, ERROR}, //6, 
					{ I, I, 8,  I,  I, I,  I,  I,  I,  I,  I,  8, I, I, I,  I,  I, I, ERROR}, //7
					{ I, I, 8,  I,  I, I,  I,  I,  I,  I,  I,  8, I, I, I,  I,  I, I, ACEPTAR}, //8
					{ I, I, I,  I,  I, I,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //9, 
					{ I, I, I,  I,  I, I,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //10,
					{ I, I, I,  I, 12, I,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //11, 
					{ I, I, I,  I,  I, I,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //12, 
					{ I, I, I,  I,  I, I,  I,  I, 14,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //13, 
					{ I, I, I,  I,  I, I,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //14, 
					{ I, I, I,  I,  I, I,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //15, 
					{ I, I, I,  I,  I, I,  I,  I, 17,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //16, 
					{ I, I, I,  I,  I, I,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //17, 
					{ I, I, I,  I,  I, I,  I,  I, 19,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //18, 
					{ I, I, I,  I,  I, I,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //19
					{ I, I, I,  I,  I, I,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //20 
					{ I, I, I,  I,  I, I,  I,  I,  I,  I, 22,  I, I, I, I,  I,  I, I, ERROR}, //21
					{ I, I, I,  I,  I, I,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, //22, 
					{ I, I, I,  I,  I, I,  I,  I,  I, 24,  I,  I, I, I, I,  I,  I, I, ERROR}, // 23, 
					{ I, I, I,  I,  I, I,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}, // 24,
					{ I, I,26,  I,  I, I,  I,  I,  I,  I,  I, 26, I, I, 26, I,  I, I, ERROR}, // 25,
					{ I, I,27,  I,  I, I,  I,  I,  I,  I,  I, 27, I, I, 27, I,  I, I, ERROR}, //26, 
					{ I, I,28,  I,  I, I,  I,  I,  I,  I,  I, 28, I, I, 28, I,  I, I, ERROR}, // 27, 
					{ I, I, I,  I,  I, I,  I,  I,  I,  I,  I,  I, I, I, I,  I,  I, I, ACEPTAR}}; // 28, 


int main( void )
{
	string cadena;
	string lexema;
	bool continuar = true;
	int estado = 0, posRecorrido = 1;
	int recorrido[ 1000 ] = {0};
	int recorridoAlternativo[ 1000 ];

	enum Entradas{ LETRA, GUIONBAJO, DIGITO, ARITMETICO, MAS, MENOS, RELACIONAL, NOT, IGUAL, OR, AND, CERO, PUNTO, EXPONENTE, HEXA, PUN_COMA, AGRUPACION, OTRO, FDC };

	Entradas entrada;
	int estadoAux;
	char c;

	system( "cls" );
	cout << "Ingresa la cadena a analizar (ctrl + z + enter para salir)" << endl;
	getline( cin, cadena );

	while( true )
	{
		pos = 0;
		estado = 0;
		posRecorrido = 1;
		continuar = true;
		cout << "---------------------------------------------" << endl;
		cout << setw( 25 ) << left << "TOKEN" << setw( 10 ) << right << "LEXEMA" << endl;
		cout << "---------------------------------------------" << endl;
		do
		{
			c = obtenerChar( cadena );
			if( isalpha( c ) && estado != 3 && estado != 5 && estado != 25 && estado != 26 && estado != 27 )
				entrada = LETRA;
			else if( c == '_' )
				entrada = GUIONBAJO;
			else if( isdigit( ( int ) c ) && c != '0' )
				entrada = DIGITO;
			else if( esOpeAritmetico( c ) )
				entrada = ARITMETICO;
			else if( c == '+' )
				entrada = MAS;
			else if( c == '-' )
				entrada = MENOS;
			else if( c == '<' || c == '>' )
				entrada = RELACIONAL;
			else if( c == '!' )
				entrada = NOT;
			else if( c == '=' )
				entrada = IGUAL;
			else if( c == '|' )
				entrada = OR;
			else if( c == '&' )
				entrada = AND;
			else if( c == '.' )
				entrada = PUNTO;
			else if( c == 'e' || c == 'E' )
				entrada = EXPONENTE;
			else if( c == ';' )
				entrada = PUN_COMA;
			else if( esAgrupacion( c ) )
				entrada = AGRUPACION;
			else if( c == '0' )
				entrada = CERO;
			else if( c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F' )
				entrada = HEXA;
			else if( c == EOF )
				entrada = FDC;
			else
				error();

			estadoAux = estado;
			estado = tabla[ estado ][ entrada ];
			recorrido[ posRecorrido++ ] = estado;
			if( estado != ACEPTAR && estado != 0 )
				lexema += c;
			if( estado == 0 || estado == ACEPTAR )
			{
				imprimirToken( lexema, estadoAux );
				lexema.clear();
				if( c != EOF )
					retrocederUno();
				if( estado == 0 )
					continuar = false;
			}
			if( estado == ERROR )
				error();

		}while( estado != ACEPTAR );

		cout << "---------------------------------------------" << endl;
		pos = 0;
		int tamRecorrido = obtenerRecorrido( recorridoAlternativo, cadena );

		cout << "\nRecorrido en el automata" << endl;
		if( tamRecorrido )
		{
			for( int j = 0; j < tamRecorrido; j++ )
				if( recorridoAlternativo[ j ] == ACEPTAR )
					cout << "ACEPTAR";
				else
					cout << recorridoAlternativo[ j ] << ", ";
			cout << "\n\nCadena valida\n" << endl;
		}
		else if( continuar )
		{
			for( int j = 0; j < posRecorrido; j++ )
				if( recorrido[ j ] == ACEPTAR )
					cout << "ACEPTAR";
				else
					cout << recorrido[ j ] << ", ";
			cout << "\n\nCadena valida\n" << endl;
		}
		else
			cout << "\n\nCadena NO valida\n" << endl;

		system( "pause" );
		system( "cls" );

		cout << "Ingresa la cadena a analizar (ctrl + z + enter para salir)" << endl;
		getline( cin, cadena );
	}

	return 0;
}


void retrocederUno( void )
{
	pos--;
}

char obtenerChar( string s )
{
	while( pos < s.length() )
	{
		while( s[ pos ] == '\n' || s[ pos ] == '\t' || s[ pos ] == ' ' ){
			pos++;
		}
		return s[ pos++ ];
	}
	return EOF;
}

bool esAgrupacion( char c )
{
	if( c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' )
		return true;
	return false;
}

bool esOpeAritmetico( char c )
{
	if( c == '*' || c == '/' || c == '^' || c == '%' )
		return true;
	return false;
}

void error( void )
{
	cout << "\nCadena no valida" << endl;
	exit( -1 );
}

string verificarToken( string lexema )
{
	static string reservadas[] = { "int", "float", "char", "double", "long", "for", "while", "if", "else", "switch", "case", "void", "return", "do", "default" };
	for( int i = 0; i < 15; i++ )
		if( reservadas[ i ] == lexema )
			return "PALABRA RESERVADA";
	return "IDENTIFICADOR";
}

void imprimirToken( string lexema, int estado )
{
	string nombreToken;
	switch( estado )
	{
		case 1:
			nombreToken = verificarToken( lexema );
			break;
		case 2:
			nombreToken = "OPERADOR ARITMETICO";
			break;
		case 3:
			if( lexema[ 0 ] == '+' || lexema[ 0 ] == '-' )
				nombreToken = "NUMERO ENTERO CON SIGNO";
			else
				nombreToken = "NUMERO ENTERO";
			break;
		case 5:
			if( lexema[ 0 ] == '+' || lexema[ 0 ] == '-' )
				nombreToken = "NUMERO REAL CON SIGNO";
			else
				nombreToken = "NUMERO REAL";
			break;
		case 8:
			nombreToken = "NUMERO EXPONENCIAL";
			break;
		case 9:
			nombreToken = "OPERADOR DECREMENTO";
			break;
		case 10:
		case 11:
			nombreToken = "OPERADOR ARITMETICO";
			break;
		case 12:
			nombreToken = "OPERADOR INCREMENTO";
			break;
		case 13:
		case 14:
		case 17:
		case 19:
			nombreToken = "OPERADOR RELACIONAL";
			break;
		case 15:
			nombreToken = "OPERADOR DE AGRUPACION";
			break;
		case 16:
			nombreToken = "ASIGNACION";
			break;
		case 18:
			nombreToken = "OPERADOR LOGICO NOT";
			break;
		case 20:
			nombreToken = "PUNTO Y COMA";
			break;
		case 22:
			nombreToken = "OPERADOR LOGICO AND";
			break;
		case 24:
			nombreToken = "OPERADOR LOGICO OR";
			break;
		case 28:
			nombreToken = "HEXADECIMAL";
			break;
		default:
			nombreToken = "CADENA NO VALIDA";
			error();
			break;
	}
	cout << setw(25) << left << nombreToken << setw(10) << right << lexema << endl;
}

int obtenerRecorrido( int recorrido[], string cadena )
{
	static int tabla[ 37 ][ 8 ] = {{1, 1, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR},
								 {1, 1, 1, 29, ERROR, ERROR, ERROR, ACEPTAR},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I}, // 10
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I}, // 20
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I},
								 {I, I, I, I, I, I, I, I}, // 28
								 {30, 30, 32, ERROR, ERROR, ERROR, ERROR, ERROR},
								 {30, 30, 30, ERROR, 31, 35, ERROR, ERROR},
								 {30, 30, 32, ERROR, ERROR, ERROR, ERROR, ERROR},
								 {ERROR, ERROR, 32, 33, 31, 36, ERROR, ERROR},
								 {ERROR, ERROR, 34, ERROR, ERROR, ERROR, ERROR, ERROR},
								 {ERROR, ERROR, ERROR, ERROR, 31, 36, ERROR, ERROR},
								 {ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ACEPTAR},
								 {ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ACEPTAR}};

	enum Entradas{ LETRA, GUIONBAJO, DIGITO, IGUAL, ARITMETICO, PUN_COMA, OTRO, FDC };

	int estado = 0;
	Entradas entrada;
	int indice = 0;
	recorrido[ indice++ ] = estado;
	char c;
	do
	{
		c = obtenerChar( cadena );
		if( isalpha( c ) )
			entrada = LETRA;
		else if( c == '_' )
			entrada = GUIONBAJO;
		else if( isdigit( ( int ) c ) )
			entrada = DIGITO;
		else if( c == '=' )
			entrada = IGUAL;
		else if( esOpeAritmetico( c ) || c == '+' || c == '-' )
			entrada = ARITMETICO;
		else if( c == ';' )
			entrada = PUN_COMA;
		else if( c == EOF )
			entrada = FDC;
		else
			return 0;
		estado = tabla[ estado ][ entrada ];
		if( estado == ERROR )
			return 0;
		recorrido[ indice++ ] = estado;
	}while( estado != ACEPTAR );

	return indice;
}
