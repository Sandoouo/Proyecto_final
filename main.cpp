#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <locale.h>

using namespace std;

struct RegSolicitante {
    char nombre[200];
    char email[100];
    char profesion[100];
    char descripcion[500];
};

struct RegPostulacion {
    char email[100];
    char codigoOferta[20];
    double salarioEsperado;
    char fechaPostulacion[11];
};

struct RegOferta {
    char codigo[20];
    char empresa[200];
    char cargo[100];
    char profesiones[200];
    char descripcion[500];
    double salario;
};

// NOMBRE DE LA FUNCION: ObtenerFechaActual
// OBJETIVO: Obtener la fecha actual y almacenarla en el buffer proporcionado
// ENTRADAS: buffer (char*), size (size_t) - buffer para almacenar la fecha y tamaño del buffer
// SALIDAS: Ninguna
void obtenerFechaActual(char* buffer, size_t size) {
    time_t t = time(0);
    struct tm* now = localtime(&t);
    snprintf(buffer, size, "%02d/%02d/%04d", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}

// NOMBRE DE LA FUNCION: InsertarOferta
// OBJETIVO: Permitir al usuario ingresar una oferta y guardarla en el archivo
// ENTRADAS: Ninguna
// SALIDAS: Ninguna
void insertarOferta() {
    system("cls");
    RegOferta regO;
    ofstream archi("ofertas.dat", ios::binary | ios::app);
    if (archi) {
        cout << "====== Insertar oferta ======" << endl << endl;
        cout << "Digite el codigo: ";
        cin >> regO.codigo;
        cout << "Digite la empresa: ";
        cin.ignore();
        cin.getline(regO.empresa, sizeof(regO.empresa));
        cout << "Digite el cargo: ";
        cin.getline(regO.cargo, sizeof(regO.cargo));
        cout << "Digite las profesiones (separadas por coma): ";
        cin.getline(regO.profesiones, sizeof(regO.profesiones));
        cout << "Digite la descripcion de la oferta: ";
        cin.getline(regO.descripcion, sizeof(regO.descripcion));
        cout << "Digite el salario ofrecido: ";
        cin >> regO.salario;
        archi.write((char*)&regO, sizeof(regO));
        archi.close();
        cout << "Registro grabado correctamente!" << endl;
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}

// NOMBRE DE LA FUNCION: ListarOfertas
// OBJETIVO: Mostrar todas las ofertas almacenadas en el archivo
// ENTRADAS: Ninguna
// SALIDAS: Ninguna
void listarOfertas() {
    system("cls");
    RegOferta regO;
    ifstream archi("ofertas.dat", ios::binary);
    if (archi) {
        cout << "====== Listado de ofertas ======" << endl << endl;
        cout << "Empresa\tCargo\tProfesiones\tSalario\tDescripcion" << endl;
        while (archi.read((char*)&regO, sizeof(regO))) {
            cout << regO.empresa << "\t" << regO.cargo << "\t" << regO.profesiones << "\t" << regO.salario << "\t" << regO.descripcion << endl;
        }
        archi.close();
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}

// NOMBRE DE LA FUNCION: BuscarOferta
// OBJETIVO: Buscar una oferta por su código y mostrarla si se encuentra
// ENTRADAS: Ninguna
// SALIDAS: Ninguna
void buscarOferta() {
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
            if (strcmp(regO.codigo, codigo) == 0) {
                encontrado = true;
                cout << regO.empresa << "\t" << regO.cargo << "\t" << regO.profesiones << "\t" << regO.salario << endl;
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

// NOMBRE DE LA FUNCION: ModificarOferta
// OBJETIVO: Modificar la descripción de una oferta existente por su código
// ENTRADAS: Ninguna
// SALIDAS: Ninguna
void modificarOferta() {
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
            if (strcmp(regO.codigo, codigo) == 0) {
                encontrado = true;
                cout << "Oferta encontrada: " << endl;
                cout << "Empresa: " << regO.empresa << endl;
                cout << "Cargo: " << regO.cargo << endl;
                cout << "Profesiones: " << regO.profesiones << endl;
                cout << "Descripcion: " << regO.descripcion << endl;
                cout << "Salario: " << regO.salario << endl;

                cout << "Digite la nueva descripcion: ";
                cin.ignore();
                cin.getline(regO.descripcion, sizeof(regO.descripcion));

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

// NOMBRE DE LA FUNCION: BorrarOferta
// OBJETIVO: Borrar una oferta por su código del archivo de ofertas
// ENTRADAS: Ninguna
// SALIDAS: Ninguna
void borrarOferta() {
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
            if (strcmp(regO.codigo, codigo) != 0) {
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

// NOMBRE DE LA FUNCION: InsertarHojaDeVida
// OBJETIVO: Permitir al usuario ingresar una hoja de vida y guardarla en el archivo
// ENTRADAS: Ninguna
// SALIDAS: Ninguna
void insertarHojaDeVida() {
    system("cls");
    RegSolicitante regS;
    ofstream archi("hojasdevida.dat", ios::binary | ios::app);
    if (archi) {
        cout << "====== Insertar hoja de vida ======" << endl << endl;
        cout << "Digite nombres y apellidos: ";
        cin.ignore();
        cin.getline(regS.nombre, sizeof(regS.nombre));
        cout << "Digite el email: ";
        cin.getline(regS.email, sizeof(regS.email));
        cout << "Digite la profesion: ";
        cin.getline(regS.profesion, sizeof(regS.profesion));
        cout << "Digite la descripcion del perfil laboral: ";
        cin.getline(regS.descripcion, sizeof(regS.descripcion));
        archi.write((char*)&regS, sizeof(regS));
        archi.close();
        cout << "Hoja de vida registrada correctamente!" << endl;
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    system("pause");
}

// NOMBRE DE LA FUNCION: BuscarHojaDeVida
// OBJETIVO: Buscar una hoja de vida por su email y mostrarla si se encuentra
// ENTRADAS: Ninguna
// SALIDAS: Ninguna
void buscarHojaDeVida() {
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
            if (strcmp(regS.email, email) == 0) {
                encontrado = true;
                cout << "Nombre: " << regS.nombre << endl;
                cout << "Email: " << regS.email << endl;
                cout << "Profesion: " << regS.profesion << endl;
                cout << "Descripcion: " << regS.descripcion << endl;
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

// NOMBRE DE LA FUNCION: ReportePostulacionesSolicitante
// OBJETIVO: Mostrar un reporte de todas las postulaciones realizadas por un solicitante, filtradas por su email
// ENTRADAS: Ninguna
// SALIDAS: Ninguna
void reportePostulacionesSolicitante() {
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
            if (strcmp(regP.email, email) == 0) {
                RegOferta regO;
                ifstream archiO("ofertas.dat", ios::binary);
                while (archiO.read((char*)&regO, sizeof(regO))) {
                    if (strcmp(regO.codigo, regP.codigoOferta) == 0) {
                        encontrado = true;
                        cout << regO.empresa << "\t" << regO.cargo << "\t" << regP.fechaPostulacion << "\t" << regP.salarioEsperado << endl;
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

// NOMBRE DE LA FUNCION: ReportePostulantes
// OBJETIVO: Mostrar un reporte de todos los postulantes a todas las ofertas disponibles
// ENTRADAS: Ninguna
// SALIDAS: Ninguna
void reportePostulantes() {
    system("cls");
    RegPostulacion regP;
    ifstream archi("postulaciones.dat", ios::binary);
    if (archi) {
        cout << "====== Reporte de postulantes ======" << endl << endl;
        while (archi.read((char*)&regP, sizeof(regP))) {
            RegOferta regO;
            ifstream archiO("ofertas.dat", ios::binary);
            while (archiO.read((char*)&regO, sizeof(regO))) {
                if (strcmp(regO.codigo, regP.codigoOferta) == 0) {
                    cout << "Cargo: " << regO.cargo << endl;
                    cout << "Email: " << regP.email << endl;
                    cout << "Salario Esperado: " << regP.salarioEsperado << endl;
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

// NOMBRE DE LA FUNCION: DescartaPostulante
// OBJETIVO: Eliminar todas las postulaciones correspondientes a un código de oferta específico
// ENTRADAS: Ninguna
// SALIDAS: Ninguna
void descartaPostulante() {
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
            if (strcmp(regP.codigoOferta, codigo) != 0) {
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

// NOMBRE DE LA FUNCION: Postularse
// OBJETIVO: Permitir a un usuario postularse a una oferta específica y guardar la postulación en el archivo
// ENTRADAS: Ninguna
// SALIDAS: Ninguna
void postularse() {
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
        cin >> regP.salarioEsperado;
        obtenerFechaActual(regP.fechaPostulacion, sizeof(regP.fechaPostulacion));
        strcpy(regP.codigoOferta, codigo);
        strcpy(regP.email, email);
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
            case 1: insertarOferta(); break;
            case 2: listarOfertas(); break;
            case 3: buscarOferta(); break;
            case 4: modificarOferta(); break;
            case 5: borrarOferta(); break;
            case 6: insertarHojaDeVida(); break;
            case 7: buscarHojaDeVida(); break;
            case 8: postularse(); break;
            case 9: reportePostulacionesSolicitante(); break;
            case 10: reportePostulantes(); break;
            case 11: descartaPostulante(); break;
            case 0: cout << "Saliendo..." << endl; break;
            default: cout << "Opcion no valida!" << endl; break;
        }
    } while (opcion != 0);
    return 0;
}

    } while (opcion != 0);
    return 0;
}
