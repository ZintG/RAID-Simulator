#ifndef RAID1_H
#define RAID1_H

#include<iostream>
#include<vector>
#include<string>
#include"Disco.hpp"
#include"RAID.hpp"

using namespace std;

class RAID1: public RAID{
    public:
        /// @brief Constructor de arreglo RAID 1
        /// @param num Numero de Discos
        /// @param size Capacidad de los discos(entero)
        RAID1(int num, int size);

        /// @brief Escribir informacion en RAID 1
        /// @param info string de informacion a escribir
        void writeInfo(const string &info);

        /// @brief leer informacion de RAID 1
        /// @return string con toda la informacion almacenada en el arreglo
        string readInfo();

        /// @brief Verificar el estado de los discos
        /// @return Retorna un vector con el indice de los discos caidos.
        vector<int> verifyDisksStatus();

        /// @brief Recuperar informacion de disco caido
        /// @param diskNumber Numero del disco caido
        void recoverDisk(int diskNumber);

        ~RAID1(){}
};

//Implementacion metodos de RAID1

RAID1::RAID1(int num, int size): RAID(num, size){
    this->totalCapacity=num*size;
    this->usableCapacity=size;
}

void RAID1::writeInfo(const string &info){
    //Obtener data de los discos para escribir
    vector<unsigned char> diskData = this->disks[0].getData();
    //Insertar info en data
    for(size_t i=0; i<info.size(); i++){
        diskData.push_back(info[i]);
    }
    //Escribir data en todos los discos
    for(size_t j=0; j<this->disks.size(); j++){
        this->disks[j].writeData(diskData);
    }
}

string RAID1::readInfo(){
    //string de salida
    string exitData;
    //Obtener data de los discos
    vector<unsigned char> diskData = this->disks[0].getData();
    //recorrer data
    for(size_t i=0; i<diskData.size(); i++){
        exitData+=diskData[i];
    }
    return exitData;
}

vector<int> RAID1::verifyDisksStatus(){
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

void RAID1::recoverDisk(int diskNumber){
    vector<unsigned char> recoveredData;
    bool aux=true;
    size_t i=0;
    //obtener data desde un disco sano
    while(aux && i<this->disks.size()){
        if(this->disks[i].getStatus()){
            recoveredData= this->disks[i].getData();
            aux=false;
        }
        i++;
    }
    //recuperar data en el disco caido
    this->disks[diskNumber].writeData(recoveredData);
    this->disks[diskNumber].changeStatus();
}

#endif