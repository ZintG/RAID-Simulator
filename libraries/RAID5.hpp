#ifndef RAID5_H
#define RAID5_H

#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>
#include"Disco.hpp"
#include"RAID.hpp"

using namespace std;

class RAID5: public RAID{
    public:
        /// @brief Constructor de arreglo RAID 5
        /// @param num Numero de Discos
        /// @param size Capacidad de los discos(entero)
        RAID5(int num, int size);

        /// @brief Escribir informacion en RAID 5
        /// @param info string de informacion a escribir
        void writeInfo(const string &info);

        /// @brief leer informacion de RAID 5
        /// @return string con toda la informacion almacenada en el arreglo
        string readInfo();

        /// @brief Verificar el estado de los discos
        /// @return Retorna un vector con el indice de los discos caidos.
        vector<int> verifyDisksStatus();

        /// @brief Recuperar informacion de disco caido
        /// @param diskNumber Numero del disco caido
        void recoverDisk(int diskNumber);

        ~RAID5(){}
};

//Implementacion metodos de RAID5

RAID5::RAID5(int num, int size): RAID(num, size){
    this->totalCapacity=num*size;
    this->usableCapacity=(num*size)-size;
}

void RAID5::writeInfo(const string &info){
    //Obtener numero de bytes a insertar en la fila
    int segmentsNumber=this->disks.size()-1;

    //Recorrer Info
    size_t stringIndex=0;
    while(stringIndex<info.size()){
        //Obtener fila actual
        int currentRow=this->disks[0].getData().size();

        //Calcular en que disco va la paridad en la fila actual
        size_t parityDisk=currentRow % this->disks.size();

        //Vector para almacenar los bytes a guardar en la fila
        vector<unsigned char> currentBytes;


        /*Guarda el segmento de bytes a guardar de la fila actual en el vector.
        Si faltan bytes para terminar la fila rellena con caracter nulo*/
        for(int i=0; i<segmentsNumber; i++){
            if(stringIndex<info.size()){
                currentBytes.push_back(info[stringIndex]);
                stringIndex++;
            }else{
                currentBytes.push_back('\0');
            }
        }

        //Calculo de paridad de la fila actual
        unsigned char parity='\0';
        for(size_t j=0; j<currentBytes.size(); j++){
            parity^=currentBytes[j];
        }

        /*Insercion en discos, guarda los bytes en orden y el bytes de paridad en el
        disco correspondiente en la fila*/
        int insertionIndex=0;
        for(size_t k=0; k<this->disks.size(); k++){
            vector<unsigned char> diskData=this->disks[k].getData();
            if(k==parityDisk){
                diskData.push_back(parity);
            }else{
                diskData.push_back(currentBytes[insertionIndex]);
                insertionIndex++;
            }

            this->disks[k].writeData(diskData);
        }
    }
}

string RAID5::readInfo(){
    //Obtener numero de filas y numero de discos
    int numberOfRows=this->disks[0].getData().size();
    int numberOfDisks=this->disks.size();
    //string de salida
    string exitData;

    //recorrer cada fila
    for(int i=0; i<numberOfRows; i++){
        //calcular disco de paridad
        int parityDisk= i % numberOfDisks;
        //recorrer cada disco
        for(int j=0; j<numberOfDisks; j++){
            //si no es el diso de paridad de la fila guarda el caracter
            if(j!=parityDisk){
                unsigned char c= this->disks[j].getData()[i];
                //verificar si el caracter no es relleno ('\0)
                if(c != '\0'){
                    exitData+= c;
                }
            }
        }
    }

    return exitData;
}

vector<int> RAID5::verifyDisksStatus(){
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

void RAID5::recoverDisk(int diskNumber){
    if(this->verifyDisksStatus().size()>1){
        cerr<<"[RAID5] Mas de un disco presenta fallas. Imposible recuperar. FATAL ERROR"<<endl;
        exit(1);
        return;
    }
    //Obtener numero de filas
    int numberOfRows;
    if(diskNumber==0){
        numberOfRows=this->disks[1].getData().size();
    }else{
        numberOfRows=this->disks[0].getData().size();
    }
    //Obtener numero de discos
    int numberOfDisks=this->disks.size();

    //Vector de informacion reconstruida
    vector<unsigned char> recoveredData;

    //recorrer fila por fila
    for(int i=0; i<numberOfRows; i++){

        unsigned char recoveredByte='\0';
        //Recorrer la fila en los discos funcionales y recuperar el byte perdido
        for(int j=0; j<numberOfDisks; j++){
            if(j!=diskNumber){
                recoveredByte^=this->disks[j].getData()[i];
            }
        }
        //Guardar byte restaurado en el vector
        recoveredData.push_back(recoveredByte);
    }
    //Escribir data recuperada en el disco
    this->disks[diskNumber].writeData(recoveredData);
    //Cambiar status a true
    this->disks[diskNumber].changeStatus();
}

#endif