#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<fstream>
#include"RAID.hpp"
#include"RAID0.hpp"
#include"RAID1.hpp"
#include"RAID5.hpp"
#include"RAID10.hpp"

using namespace std;

void clearScreen();

int main(){
    clearScreen();

    ifstream logoIn("logo.in");
    string logo;
    while(getline(logoIn, logo)){
        cout<<logo<<endl;
    }
    logoIn.close();

    cout<<"Simulador de arreglos RAID."<<endl<<endl;
    cout<<"Elija el tipo de RAID a simular:"<<endl;
    cout<<"1. RAID 0"<<endl;
    cout<<"2. RAID 1"<<endl;
    cout<<"3. RAID 5"<<endl;
    cout<<"4. RAID 10"<<endl;
    cout<<"5. Salir"<<endl;
    cout<<"Ingrese una opcion:"<<endl;

    int version;
    do{
        cin>>version;
        if(version > 5 || version<1){
            cout<<"Opcion invalida. Intente otra vez:"<<endl;
        }
    }while(version > 5 || version<1);


    if(version==5){
        return 0;
    }

    clearScreen();

    cout<<"Ingrese un numero de discos para el arreglo."<<endl;
    switch (version)
    {
    case 1:
        cout<<"Minimo para RAID 0: 2 discos"<<endl;
        break;
    case 2:
        cout<<"Minimo para RAID 1: 2 discos"<<endl;
        break;
    case 3:
        cout<<"Minimo para RAID 5: 3 discos"<<endl;
        break;
    case 4:
        cout<<"Minimo para RAID 10: 4 discos. La cantidad de Discos debe ser par."<<endl;
        break;
    default:
        break;
    }

    int numberOfDisks;
    bool readed=false;
    do{
        cin>>numberOfDisks;
        switch (version)
        {
        case 1:
        case 2:
            if(numberOfDisks<2){
                cout<<"Numero de discos invalido intente otra vez:"<<endl;
            }else{
                readed= true;
            }
            break;
        case 3:
            if(numberOfDisks<3){
                cout<<"Numero de discos invalido intente otra vez:"<<endl;
            }else{
                readed= true;
            }
            break;
        case 4:
            if(numberOfDisks<4 || numberOfDisks%2 != 0){
                cout<<"Numero de discos invalido intente otra vez:"<<endl;
            }else{
                readed= true;
            }
            break;
        default:
            break;
        }
    }while(readed==false);

    clearScreen();

    cout<<"Ingrese la capacidad de los discos en GB. Solo numeros enteros positivos:"<<endl;

    int diskSize;
    do{
        cin>>diskSize;
        if(diskSize<1){
            cout<<"Capacidad invalida. Intente otra vez:"<<endl;
        }
    }while(diskSize<1);

    clearScreen();

    RAID *sistema;

    switch(version){
        case 1:
            sistema= new RAID0(numberOfDisks, diskSize);
            break;
        case 2:
            sistema= new RAID1(numberOfDisks, diskSize);
            break;
        case 3:
            sistema= new RAID5(numberOfDisks, diskSize);
            break;
        case 4:
            sistema= new RAID10(numberOfDisks, diskSize);
        default:
            break;
    }

    int menuOption;
    do{
        cout<<"Elija una opcion:"<<endl<<endl;
        cout<<"1. Escribir informacion en el arreglo."<<endl;
        cout<<"2. Leer informacion desde el arreglo."<<endl;
        cout<<"3. Ver capacidad total del arreglo."<<endl;
        cout<<"4. Ver capacidad utilizable real en el arreglo"<<endl;
        cout<<"5. Simular fallo de disco."<<endl;
        cout<<"6. Recuperar informacion de disco caido."<<endl;
        cout<<"7. Mostrar discos con fallos."<<endl;
        cout<<"8. Salir (Se perdera la informacion y la configuracion)."<<endl<<endl;
        cout<<"Ingrese una opcion: "<<endl;

        do{
            cin>>menuOption;
            if(menuOption<1 || menuOption>8){
                cout<<"Opcion Invalida. Intente otra vez"<<endl;
            }
        }while(menuOption<1 || menuOption>8);

        clearScreen();

        switch(menuOption){
            case 1:{
                vector<int> fallenDisks=sistema->verifyDisksStatus();
                if(fallenDisks.size()>0){
                    cout<<"Se detectaron discos con fallas. Iniciando restauracion."<<endl;
                    for(size_t i=0; i<fallenDisks.size(); i++){
                        sistema->recoverDisk(fallenDisks[i]);
                    }
                    cout<<"Restauracion completada. Continuando con la operacion."<<endl<<endl;
                }

                cout<<"Elija de donde se va a recibir la informacion:"<<endl;
                cout<<"1. Archivo"<<endl;
                cout<<"2. Teclado"<<endl;
                int submenu;
                do{
                    cin>>submenu;
                    if(submenu>2 || submenu<1){
                        cout<<"Opcion invalida. Intenta otra vez:"<<endl;
                    }
                }while(submenu>2 || submenu<1);

                switch (submenu)
                {
                case 1:{
                    cout<<"Ingrese el nombre del archivo (Este debe estar en la carpeta raiz del programa)"<<endl;
                    cin.ignore();
                    string fileName;
                    getline(cin, fileName);
                    
                    ifstream input(fileName.c_str());
                    string inputLine;

                    while(getline(input, inputLine)){
                        inputLine+='\n';
                        sistema->writeInfo(inputLine);
                    }
                    
                    input.close();
                    
                    cout<<"Informacion del archivo cargada correctamente"<<endl;

                    break;
                }
                case 2:{
                    cout<<"Escriba la informacion a guardar:"<<endl;
                    string info;
                    cin.ignore();
                    getline(cin, info);
                    info+='\n';
                    sistema->writeInfo(info);
                    cout<<endl<<"Informacion cargada correctamente"<<endl;
                    break;
                }
                default:
                    break;
                }

                break;
            }
            case 2:{
                vector<int> fallenDisks=sistema->verifyDisksStatus();
                if(fallenDisks.size()>0){
                        cout<<"Se detectaron discos con fallas. Iniciando restauracion."<<endl;
                    for(size_t i=0; i<fallenDisks.size(); i++){
                        sistema->recoverDisk(fallenDisks[i]);
                    }
                    cout<<"Restauracion completada. Continuando con la operacion."<<endl<<endl;
                }
                string info;
                info=sistema->readInfo();
                cout<<info<<endl;
                break;
            }
            case 3:
                cout<<"Capacidad total :"<<sistema->getTotalCapacity()<<" GB"<<endl;
                break;
            case 4: 
                cout<<"Capacidad utilizable :"<<sistema->getUsableCapacity()<<" GB"<<endl;
                break;
            case 5:{
                cout<<"Ingrese el numero del disco cuya informacion sera borrada:"<<endl;
                int diskNumber;
                do{
                    cin>>diskNumber;
                    if(diskNumber>numberOfDisks-1 || diskNumber<0){
                        cout<<"Opcion invalida. Intenta otra vez:"<<endl;
                    }
                }while(diskNumber>numberOfDisks-1 || diskNumber<0);
                sistema->diskFailure(diskNumber);
                cout<<"Informacion del disco "<<diskNumber<<" borrada con exito"<<endl;
                break;
            }
            case 6:
                cout<<"Ingrese el numero de disco que se va a restaurar:"<<endl;
                int diskNumber;
                do{
                    cin>>diskNumber;
                    if(diskNumber>numberOfDisks-1 || diskNumber<0){
                        cout<<"Opcion invalida. Intenta otra vez:"<<endl;
                    }
                }while(diskNumber>numberOfDisks-1 || diskNumber<0);
                sistema->recoverDisk(diskNumber);
                cout<<"Disco "<<diskNumber<<" restaurado con exito"<<endl;
                break;
            case 7:{
                vector<int> fallenDisks=sistema->verifyDisksStatus();
                if(fallenDisks.size()==0){
                    cout<<"No hay discos con fallos."<<endl;
                }else{
                    for(size_t i=0; i<fallenDisks.size(); i++){
                        cout<<"Disco "<<fallenDisks[i]<<endl;
                    }
                }
                break;
            }
            case 8:
                break;
            default:
             break;
        }
        cin.ignore();
        cout<<"Pulsa ENTER para continuar."<<endl;
        cin.ignore();
        clearScreen();
    }while(menuOption!=8);


    delete sistema;
    return 0;
}

void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}