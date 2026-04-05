#ifndef DISCO_H
#define DISCO_H

#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;
class Disco{
    private:
        string diskName;
        int diskSize;
        static int numberOfExistingDisks;
        vector<unsigned char> data;
        bool status;
    public:
        ///@brief Constructor de la clase Disco.
        ///@param name Nombre del archivo. Si el archivo no existe lo crea
        ///@param size Capacidad del Disco en GB
        Disco(string name, int size);

        /// @brief Leer data de un disco
        /// @return vector<unsigned char>, cada espacio del vector es un byte
        vector<unsigned char> getData() const;

        /// @brief Guardar data modificada en un disco
        /// @param newData recibe vector<unsigned char> y lo escribe en el archivo(sobreescritura)
        void writeData(const vector<unsigned char> &newData);

        /// @brief Obtener capacidad del Disco en GB
        /// @return Capacidad del Disco en int
        int getDiskSize() const;

        /// @brief Obtener numeros de discos existentes
        /// @return Numero de discos existentes, int 
        static int getNumberOfDisks();

        /// @brief Retorna el estado del disco
        /// @return en caso de fallas retorna false y true en caso contrario
        bool getStatus() const;

        /// @brief Cambia el valor del atributo booleano status
        void changeStatus();

        /// @brief Eliminar la data del Disco(archivo)
        void eraseData();

        /// @brief Destructor (No borra el archivo)
        ~Disco(){}
};

Disco::Disco(string name, int size): diskName(name), diskSize(size), status(true){
    Disco::numberOfExistingDisks++;

    //Abrir archivo en modo lectura
    ifstream disk(this->diskName.c_str(), ios::binary);
    //verificar que existe y se abrio correctamente
    if(disk.is_open()){
        //cargar informacion del archivo en el vector data
        unsigned char nuevo;
        while(disk.read((char*)&nuevo, sizeof(nuevo))){
            this->data.push_back(nuevo);
        }
        disk.close();
    }else{
        //Si el archivo no se abrio o no existe se crea
        ofstream newDisk(this->diskName.c_str(), ios::binary);
        //verificacion de apertura/creacion correcta
        if(!newDisk.is_open()){
            cerr<<"Error al abrir/crear archivo"<<endl;
        }else{
            newDisk.close();
        }
    }
}

vector<unsigned char> Disco::getData() const{
    return this->data;
}

void Disco::writeData(const vector<unsigned char> &newData){
    //Cargar datos modificados en el objeto
    this->data=newData;

    ofstream disk(this->diskName.c_str(), ios::binary| ios::trunc);

    if(disk.is_open()){
        //Escribir en el archivo
        disk.write((char*)this->data.data(), this->data.size());

        disk.close();
    }else{
        cerr<<"Error al abrir el archivo"<<endl;
    }
}

int Disco::getDiskSize() const{
    return this->diskSize;
}

int Disco::getNumberOfDisks(){
    return Disco::numberOfExistingDisks;
}

void Disco::eraseData(){
    ofstream eraseDiskData(this->diskName.c_str(), ios::binary | ios::trunc);
    if(!eraseDiskData.is_open()){
        cerr<<"Error al truncar el archivo(abrir nuevo)"<<endl;
    }else{
        this->data.clear();
        eraseDiskData.close();
        this->status=false;
    }
}

bool Disco::getStatus() const{
    return this->status;
}

void Disco::changeStatus(){
    this->status=!this->status;
}

//Inicializacion del estatico
int Disco::numberOfExistingDisks=0;

#endif