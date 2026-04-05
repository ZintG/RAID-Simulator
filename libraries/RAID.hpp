#ifndef RAID_H
#define RAID_H

#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<cstdlib>
#include"Disco.hpp"

using namespace std;

//Clase base RAID
class RAID{
    protected:
        vector<Disco> disks;
        int sizeOfDisks;
        int usableCapacity, totalCapacity;

    public:
        /// @brief Constructor de clase abstracta RAID
        /// @param num Numero de discos
        /// @param size Capacidad de los discos
        RAID(int num, int size);

        /// @brief Metodo virtual para escribir informacion
        /// @param info string que contiene la informacion a escribir
        virtual void writeInfo(const string &info)=0;

        /// @brief Metodo Virtual para leer la informacion del arreglo
        /// @return string con la informacion completa
        virtual string readInfo()=0;

        /// @brief Metodo virtual para recuperar un disco caido.
        /// @param diskNumber Numero del disco caido
        virtual void recoverDisk(int diskNumber)=0;

        /// @brief Metodo Virtual para verificar el estado de los discos
        /// @return Retorna un vector con el indice de los discos caidos.
        virtual vector<int> verifyDisksStatus()=0;

        /// @brief Simular fallo de disco. Borra toda su informacion
        /// @param diskNumber Numero del disco al que se le va a borrar la informacion
        void diskFailure(int diskNumber);

        /// @brief Obtener capacidad real utilizable del arreglo
        /// @return numero entero que representa la capacidad
        int getUsableCapacity() const;

        /// @brief Obtener capacidad total del arreglo
        /// @return numero entero que representa la capacidad
        int getTotalCapacity() const;

        virtual ~RAID(){}
};

//Implementacion metodos de RAID

RAID::RAID(int num, int size):sizeOfDisks(size){
    #ifdef _WIN32
        system("rmdir /s /q DiskData 2>nul & mkdir DiskData");
    #else
        system("rm -rf DiskData 2>/dev/null; mkdir -p DiskData");
    #endif

    for(int i=0; i<num; i++){
        char label[200];

        sprintf(label, "DiskData/disk%d.bin", i);

        string diskLabel=label;

        Disco newDisk(diskLabel, size);

        this->disks.push_back(newDisk);
    }
}

void RAID::diskFailure(int diskNumber){
    disks[diskNumber].eraseData();
}

int RAID::getTotalCapacity() const{
    return this->totalCapacity;
}

int RAID::getUsableCapacity() const{
    return this->usableCapacity;
}

#endif