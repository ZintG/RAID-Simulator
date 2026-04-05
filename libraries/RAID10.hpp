#ifndef RAID10_H
#define RAID10_H

#include<iostream>
#include<vector>
#include<string>
#include"Disco.hpp"
#include"RAID.hpp"

using namespace std;

class RAID10: public RAID{
    public:
        /// @brief Constructor de arreglo RAID 10
        /// @param num Numero de Discos
        /// @param size Capacidad de los discos(entero)
        RAID10(int num, int size);

        /// @brief Escribir informacion en RAID 10
        /// @param info string de informacion a escribir
        void writeInfo(const string &info);

        /// @brief leer informacion de RAID 10
        /// @return string con toda la informacion almacenada en el arreglo
        string readInfo();

        /// @brief Verificar el estado de los discos
        /// @return Retorna un vector con el indice de los discos caidos.
        vector<int> verifyDisksStatus();

        /// @brief Recuperar informacion de disco caido
        /// @param diskNumber Numero del disco caido
        void recoverDisk(int diskNumber);

        ~RAID10(){}
};

//Implementacion metodos de RAID10

RAID10::RAID10(int num, int size): RAID(num, size){
    this->totalCapacity=num*size;
    this->usableCapacity=(num*size)/2;
}

void RAID10::writeInfo(const string &info){
    //Obtener en cuantos bytes se separa infor por fila
    int segmentsNumber=this->disks.size()/2;
    //Obtener numero de discos
    int numberOfDisks=this->disks.size();

    //Recorrer info
    size_t stringIndex=0;
    while(stringIndex<info.size()){

        //Extraer los bytes de la fila
        vector<unsigned char> currentBytes;
        for(int i=0; i<segmentsNumber; i++){
            if(stringIndex<info.size()){
                currentBytes.push_back(info[stringIndex]);
                stringIndex++;
            }else{
                //Si hacen falta bytes para terminar la fila rellena con '\0'
                currentBytes.push_back('\0');
            }
        }

        //Insercion en los discos
        int insertionIndex=0;
        for(int j=0; j<numberOfDisks; j+=2){
            //se incerta en los discos pares y se copia en los impares
            vector<unsigned char> diskData=this->disks[j].getData();

            diskData.push_back(currentBytes[insertionIndex]);

            this->disks[j].writeData(diskData);
            this->disks[j+1].writeData(diskData);
            insertionIndex++;
        }
    }
}

string RAID10::readInfo(){
    //string de salida
    string exitData;

    //Obtener numero de discos y filas 
    int numberOfDisks=this->disks.size();
    int numberOfRows=this->disks[0].getData().size();

    //recorrer filas
    for(int i=0; i<numberOfRows; i++){
        //recorrer solo los discos pares
        for(int j=0; j<numberOfDisks; j+=2){
            //extraer caracter de los discos y guardarlo en el string
            unsigned char c= this->disks[j].getData()[i];
            if(c != '\0'){
                //si el caracter es nulo lo ignora
                exitData+= c;
            }
        }
    }

    return exitData;
}

vector<int> RAID10::verifyDisksStatus(){
    //obtener numero de discos
    int numberOfDisks=this->disks.size();

    //vector de salida
    vector<int> errorDisks;

    //verificar el estado de cada disco
    for(int i=0; i<numberOfDisks; i++){
        if(!this->disks[i].getStatus()){
            errorDisks.push_back(i);
        }
    }

    return errorDisks;
}

void RAID10::recoverDisk(int diskNumber){
    /*Obtener indice del disco que se usara para recuperar
    Si el disco caido es par se recupera desde el siguiente impar
    Si el disco caido es impar se recupera desde el anterior par*/
    int recoveryIndex;
    if(diskNumber%2 == 0){
        recoveryIndex=diskNumber+1;
    }else{
        recoveryIndex=diskNumber-1;
    }
    //Se verifica que el disco del que se recuperar la informacion este funcionando
    if(this->disks[recoveryIndex].getStatus()){
        //Recuperar el disco caido y cambiar status a true
        this->disks[diskNumber].writeData(this->disks[recoveryIndex].getData());
        this->disks[diskNumber].changeStatus();
    }else{
        //Si la pareja del disco caido tambien presenta fallos no se puede recuperar.
        cerr<<"[RAID10] Error al recuperar discos, el disco "<<diskNumber<<" y su pareja estan caidos. FATAL ERROR"<<endl;
        exit(1);
    }
}

#endif