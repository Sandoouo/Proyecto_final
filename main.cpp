#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <locale.h>

using namespace std;

struct RegSolicitante {
    char Nombre[200];
    char Email[100];
    char Profesion[100];
    char Descripcion[500];
};

struct RegPostulacion {
    char Email[100];
    char CodigoOferta[20];
    double SalarioEsperado;
    char FechaPostulacion[11];
};

struct RegOferta {
    char Codigo[20];
    char Empresa[200];
    char Cargo[100];
    char Profesiones[200];
    char Descripcion[500];
    double Salario;
};


// Nombre de la funcion: ObtenerFechaActual
// Objetivo: Obtener la fecha actual y almacenarla en el buffer proporcionado
// Entradas: buffer (char*), size (size_t) - buffer para almacenar la fecha y tamaño del buffer
// Salidas: Ninguna
void ObtenerFechaActual(char* buffer, size_t size) {
    time_t t = time(0);
    struct tm* now = localtime(&t);
    snprintf(buffer, size, "%02d/%02d/%04d", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}

// Nombre de la funcion: InsertarOferta
// Objetivo: Permitir al usuario ingresar una oferta y guardarla en el archivo
// Entradas: Ninguna
// Salidas: Ninguna
void InsertarOferta() {
    system("cls");
    RegOferta regO;
    ofstream archi("ofertas.dat", ios::binary | ios::app);
    if (archi) {
        cout << "====== Insertar oferta ======" << endl << endl;
        cout << "Digite el codigo: ";
        cin >> regO.Codigo;
        cout << "Digite la empresa: ";
        cin.ignore();
        cin.getline(regO.Empresa, sizeof(regO.Empresa));
        cout << "Digite el cargo: ";
        cin.getline(regO.Cargo, sizeof(regO.Cargo));
        cout << "Digite las profesiones (separadas por coma): ";
        cin.getline(regO.Profesiones, sizeof(regO.Profesiones));
        cout << "Digite la descripcion de la oferta: ";
        cin.getline(regO.Descripcion, sizeof(regO.Descripcion));
        cout << "Digite el salario ofrecido: ";
        cin >> regO.Salario;
        archi.write((char*)&regO, sizeof(regO));
        archi.close();
        cout << "Registro grabado correctamente!" << endl;
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}


// Nombre de la funcion: ListarOfertas
// Objetivo: Mostrar todas las ofertas almacenadas en el archivo
// Entradas: Ninguna
// Salidas: Ninguna
void ListarOfertas() {
    system("cls");
    RegOferta regO;
    ifstream archi("ofertas.dat", ios::binary);
    if (archi) {
        cout << "====== Listado de ofertas ======" << endl << endl;
        cout << "Empresa\tCargo\tProfesiones\tSalario\tDescripcion" << endl;
        while (archi.read((char*)&regO, sizeof(regO))) {
            cout << regO.Empresa << "\t" << regO.Cargo << "\t" << regO.Profesiones << "\t" << regO.Salario << "\t" << regO.Descripcion << endl;
        }
        archi.close();
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}


// Nombre de la funcion: BuscarOferta
// Objetivo: Buscar una oferta por su código y mostrarla si se encuentra
// Entradas: Ninguna
// Salidas: Ninguna
void BuscarOferta() {
    system("cls");
    RegOferta regO;
    ifstream archi("ofertas.dat", ios::binary);
    if (archi) {
        char codigo[20];
        bool encontrado = false;
        cout << "====== Busqueda de oferta ======" << endl << endl;
        cout << "Digite codigo de oferta a buscar: ";
        cin >> codigo;
        cout << "Empresa\tCargo\tProfesiones\tSalario" << endl;
        while (archi.read((char*)&regO, sizeof(regO))) {
            if (strcmp(regO.Codigo, codigo) == 0) {
                encontrado = true;
                cout << regO.Empresa << "\t" << regO.Cargo << "\t" << regO.Profesiones << "\t" << regO.Salario << endl;
                break;
            }
        }
        if (!encontrado) {
            cout << "Codigo de oferta no encontrado!" << endl;
        }
        archi.close();
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}

// Nombre de la funcion: ModificarOferta
// Objetivo: Modificar la descripción de una oferta existente por su código
// Entradas: Ninguna
// Salidas: Ninguna
void ModificarOferta() {
    system("cls");
    fstream archi("ofertas.dat", ios::binary | ios::in | ios::out);
    if (archi) {
        char codigo[20];
        bool encontrado = false;
        RegOferta regO;
        cout << "====== Modificar una oferta ======" << endl << endl;
        cout << "Digite codigo de oferta a modificar: ";
        cin >> codigo;
        archi.seekg(0, ios::beg);
        while (archi.read((char*)&regO, sizeof(regO))) {
            if (strcmp(regO.Codigo, codigo) == 0) {
                encontrado = true;
                cout << "Oferta encontrada: " << endl;
                cout << "Empresa: " << regO.Empresa << endl;
                cout << "Cargo: " << regO.Cargo << endl;
                cout << "Profesiones: " << regO.Profesiones << endl;
                cout << "Descripcion: " << regO.Descripcion << endl;
                cout << "Salario: " << regO.Salario << endl;

                cout << "Digite la nueva descripcion: ";
                cin.ignore();
                cin.getline(regO.Descripcion, sizeof(regO.Descripcion));

                archi.seekp(-static_cast<int>(sizeof(regO)), ios::cur);
                archi.write((char*)&regO, sizeof(regO));
                cout << "Registro modificado correctamente" << endl;
                break;
            }
        }
        if (!encontrado) {
            cout << "Codigo de oferta no encontrado!" << endl;
        }
        archi.close();
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}


// Nombre de la funcion: BorrarOferta
// Objetivo: Borrar una oferta por su código del archivo de ofertas
// Entradas: Ninguna
// Salidas: Ninguna
void BorrarOferta() {
    system("cls");
    char codigo[20];
    RegOferta regO;
    ifstream archi("ofertas.dat", ios::binary);
    ofstream archiTemp("OfertasTemp.dat", ios::binary);
    if (archi && archiTemp) {
        cout << "====== Borrar una oferta ======" << endl << endl;
        cout << "Digite codigo de oferta a borrar: ";
        cin >> codigo;
        while (archi.read((char*)&regO, sizeof(regO))) {
            if (strcmp(regO.Codigo, codigo) != 0) {
                archiTemp.write((char*)&regO, sizeof(regO));
            }
        }
        archi.close();
        archiTemp.close();
        remove("ofertas.dat");
        rename("OfertasTemp.dat", "ofertas.dat");
        cout << "Registro borrado correctamente" << endl;
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}


// Nombre de la funcion: InsertarHojaDeVida
// Objetivo: Permitir al usuario ingresar una hoja de vida y guardarla en el archivo
// Entradas: Ninguna
// Salidas: Ninguna
void InsertarHojaDeVida() {
    system("cls");
    RegSolicitante regS;
    ofstream archi("hojasdevida.dat", ios::binary | ios::app);
    if (archi) {
        cout << "====== Insertar hoja de vida ======" << endl << endl;
        cout << "Digite nombres y apellidos: ";
        cin.ignore();
        cin.getline(regS.Nombre, sizeof(regS.Nombre));
        cout << "Digite el email: ";
        cin.getline(regS.Email, sizeof(regS.Email));
        cout << "Digite la profesion: ";
        cin.getline(regS.Profesion, sizeof(regS.Profesion));
        cout << "Digite la descripcion del perfil laboral: ";
        cin.getline(regS.Descripcion, sizeof(regS.Descripcion));
        archi.write((char*)&regS, sizeof(regS));
        archi.close();
        cout << "Hoja de vida registrada correctamente!" << endl;
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}


// Nombre de la funcion: BuscarHojaDeVida
// Objetivo: Buscar una hoja de vida por su email y mostrarla si se encuentra
// Entradas: Ninguna
// Salidas: Ninguna
void BuscarHojaDeVida() {
    system("cls");
    RegSolicitante regS;
    ifstream archi("hojasdevida.dat", ios::binary);
    if (archi) {
        char email[100];
        bool encontrado = false;
        cout << "====== Buscar hoja de vida ======" << endl << endl;
        cout << "Digite el email del solicitante: ";
        cin.ignore();
        cin.getline(email, sizeof(email));
        while (archi.read((char*)&regS, sizeof(regS))) {
            if (strcmp(regS.Email, email) == 0) {
                encontrado = true;
                cout << "Nombre: " << regS.Nombre << endl;
                cout << "Email: " << regS.Email << endl;
                cout << "Profesion: " << regS.Profesion << endl;
                cout << "Descripcion: " << regS.Descripcion << endl;
                break;
            }
        }
        if (!encontrado) {
            cout << "Email no encontrado!" << endl;
        }
        archi.close();
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}


// Nombre de la funcion: ReportePostulacionesSolicitante
// Objetivo: Mostrar un reporte de todas las postulaciones realizadas por un solicitante, filtradas por su email
// Entradas: Ninguna
// Salidas: Ninguna
void ReportePostulacionesSolicitante() {
    system("cls");
    RegPostulacion regP;
    ifstream archi("postulaciones.dat", ios::binary);
    if (archi) {
        char email[100];
        bool encontrado = false;
        cout << "====== Reporte de postulaciones ======" << endl << endl;
        cout << "Digite el email del solicitante: ";
        cin.ignore();
        cin.getline(email, sizeof(email));
        cout << "Empresa\tCargo\tFecha\tSalario Esperado" << endl;
        while (archi.read((char*)&regP, sizeof(regP))) {
            if (strcmp(regP.Email, email) == 0) {
                RegOferta regO;
                ifstream archiO("ofertas.dat", ios::binary);
                while (archiO.read((char*)&regO, sizeof(regO))) {
                    if (strcmp(regO.Codigo, regP.CodigoOferta) == 0) {
                        encontrado = true;
                        cout << regO.Empresa << "\t" << regO.Cargo << "\t" << regP.FechaPostulacion << "\t" << regP.SalarioEsperado << endl;
                        break;
                    }
                }
                archiO.close();
            }
        }
        if (!encontrado) {
            cout << "No se encontraron postulaciones!" << endl;
        }
        archi.close();
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}


// Nombre de la funcin: ReportePostulantes
// Objetivo: Mostrar un reporte de todos los postulantes a todas las ofertas disponibles
// Entradas: Ninguna
// Salidas: Ninguna
void ReportePostulantes() {
    system("cls");
    RegPostulacion regP;
    ifstream archi("postulaciones.dat", ios::binary);
    if (archi) {
        cout << "====== Reporte de postulantes ======" << endl << endl;
        while (archi.read((char*)&regP, sizeof(regP))) {
            RegOferta regO;
            ifstream archiO("ofertas.dat", ios::binary);
            while (archiO.read((char*)&regO, sizeof(regO))) {
                if (strcmp(regO.Codigo, regP.CodigoOferta) == 0) {
                    cout << "Cargo: " << regO.Cargo << endl;
                    cout << "Email: " << regP.Email << endl;
                    cout << "Salario Esperado: " << regP.SalarioEsperado << endl;
                    cout << "----------------------------" << endl;
                    break;
                }
            }
            archiO.close();
        }
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}


// Nombre de la funcion: DescartaPostulante
// Objetivo: Eliminar todas las postulaciones correspondientes a un código de oferta específico
// Entradas: Ninguna
// Salidas: Ninguna
void DescartaPostulante() {
    system("cls");
    char codigo[20];
    RegPostulacion regP;
    ifstream archi("postulaciones.dat", ios::binary);
    ofstream archiTemp("PostulantesTemp.dat", ios::binary);
    if (archi && archiTemp) {
        cout << "====== Descartar postulante ======" << endl << endl;
        cout << "Digite codigo de oferta a descartar postulantes: ";
        cin >> codigo;
        while (archi.read((char*)&regP, sizeof(regP))) {
            if (strcmp(regP.CodigoOferta, codigo) != 0) {
                archiTemp.write((char*)&regP, sizeof(regP));
            }
        }
        archi.close();
        archiTemp.close();
        remove("postulaciones.dat");
        rename("PostulantesTemp.dat", "postulaciones.dat");
        cout << "Postulantes descartados correctamente" << endl;
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}


// Nombre de la funcion: Postularse
// Objetivo: Permitir a un usuario postularse a una oferta específica y guardar la postulación en el archivo
// Entradas: Ninguna
// Salidas: Ninguna
void Postularse() {
    system("cls");
    RegPostulacion regP;
    ofstream archi("postulaciones.dat", ios::binary | ios::app);
    if (archi) {
        char codigo[20];
        char email[100];
        cout << "====== Postularse a oferta ======" << endl << endl;
        cout << "Digite el codigo de la oferta: ";
        cin >> codigo;
        cout << "Digite su email: ";
        cin.ignore();
        cin.getline(email, sizeof(email));
        cout << "Digite el salario esperado: ";
        cin >> regP.SalarioEsperado;
        ObtenerFechaActual(regP.FechaPostulacion, sizeof(regP.FechaPostulacion));
        strcpy(regP.CodigoOferta, codigo);
        strcpy(regP.Email, email);
        archi.write((char*)&regP, sizeof(regP));
        archi.close();
        cout << "Postulacion realizada correctamente!" << endl;
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}

int main() {
    setlocale(LC_CTYPE,"spanish");
    int opcion;
    do {
        system("cls");
        cout << "===== Agencia de Empleo =====" << endl;
        cout << "1. Insertar oferta" << endl;
        cout << "2. Listar ofertas" << endl;
        cout << "3. Buscar oferta" << endl;
        cout << "4. Modificar oferta" << endl;
        cout << "5. Borrar oferta" << endl;
        cout << "6. Insertar hoja de vida" << endl;
        cout << "7. Buscar hoja de vida" << endl;
        cout << "8. Postularse a una oferta" << endl;
        cout << "9. Reporte de postulaciones de solicitante" << endl;
        cout << "10. Reporte de postulantes" << endl;
        cout << "11. Descartar postulante" << endl;
        cout << "0. Salir" << endl;
        cout << "Digite su opcion: ";
        cin >> opcion;
        switch (opcion) {
            case 1: InsertarOferta(); break;
            case 2: ListarOfertas(); break;
            case 3: BuscarOferta(); break;
            case 4: ModificarOferta(); break;
            case 5: BorrarOferta(); break;
            case 6: InsertarHojaDeVida(); break;
            case 7: BuscarHojaDeVida(); break;
            case 8: Postularse(); break;
            case 9: ReportePostulacionesSolicitante(); break;
            case 10: ReportePostulantes(); break;
            case 11: DescartaPostulante(); break;
            case 0: cout << "Saliendo..." << endl; break;
            default: cout << "Opcion no valida!" << endl; break;
        }
    } while (opcion != 0);
    return 0;
}
