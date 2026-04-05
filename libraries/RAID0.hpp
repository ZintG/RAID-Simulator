#ifndef RAID0_H
#define RAID0_H

#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>
#include"Disco.hpp"
#include"RAID.hpp"

using namespace std;

class RAID0: public RAID{
    private:
        void recoverDisk(int /*diskNumber*/){
            cerr<<"[RAID 0]ERROR: La recuperacion de informacion no es posible. FATAL ERROR"<<endl;
            exit(1);
            return;
        }
    public:
        /// @brief Constructor de arreglo RAID 0
        /// @param num Numero de Discos
        /// @param size Capacidad de los discos (entero)
        RAID0(int num, int size);

        /// @brief Escribir informacion en RAID 0
        /// @param info string de informacion a escribir
        void writeInfo(const string &info);

        /// @brief leer informacion de RAID 0
        /// @return string con toda la informacion almacenada en el arreglo
        string readInfo();

        /// @brief Verificar el estado de los discos
        /// @return Retorna un vector con el indice de los discos caidos.
        vector<int> verifyDisksStatus();

        ~RAID0(){}
};

RAID0::RAID0(int num, int size): RAID(num, size){
    this->totalCapacity=num*size;
    this->usableCapacity=num*size;
}

void RAID0::writeInfo(const string &info){
    //Obtener el numero de discos
    int numberOfDisks=this->disks.size();

    //recorrer info
    size_t stringIndex=0;
    while(stringIndex<info.size()){

        //Extraer el segmento de bytes de la fila, si faltan bytes rellena con '\0'
        vector<unsigned char> currentBytes;
        for(int i=0; i<numberOfDisks; i++){
            if(stringIndex<info.size()){
                currentBytes.push_back(info[stringIndex]);
                stringIndex++;
            }else{
                currentBytes.push_back('\0');
            }
        }

        //almacenar en los discos
        for(int j=0; j<numberOfDisks; j++){
            vector<unsigned char> diskData=this->disks[j].getData();
            diskData.push_back(currentBytes[j]);
            this->disks[j].writeData(diskData);
        }
    }
}

string RAID0::readInfo(){
    //Obtener numero de filas y numero de discos
    int numberOfRows=this->disks[0].getData().size();
    int numberOfDisks=this->disks.size();
    //string de salida
    string exitData;

    //recorrer cada fila
    for(int i=0; i<numberOfRows; i++){
        //recorrer cada disco
        for(int j=0; j<numberOfDisks; j++){
            unsigned char c=this->disks[j].getData()[i];
            //verificar si el caracter no es relleno ('\0)
            if(c != '\0'){
                exitData+= c;
            }
        }
    }

    return exitData;
}

vector<int> RAID0::verifyDisksStatus(){
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

#endif