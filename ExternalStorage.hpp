#pragma once
#include <array>
#include <cstring>
#include <cstdint>
#include <cstdio>
using namespace std;
class ExternalStorage{
public:
	ExternalStorage(){}
	ExternalStorage(ExternalStorage&&) = delete;
	ExternalStorage(const ExternalStorage&) = delete;
	ExternalStorage& operator=(ExternalStorage&&) = delete;
	ExternalStorage& operator=(const ExternalStorage&) = delete;
	virtual ~ExternalStorage(){}
	virtual bool Init()=0;
	virtual bool Deinit()=0;
	virtual bool Write(std::uint32_t address, std::uint8_t* pData, std::uint16_t dataLen)=0;
	virtual bool Read(std::uint32_t address, std::uint8_t* pData, std::uint16_t dataLen)=0;

};
class MEM : public ExternalStorage {
public:
    MEM(const std::string& filename) {
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening file." << std::endl;
        }
    }
    bool Init(){
        return true;
    }
    bool Deinit(){
        return true;
    }
    ~MEM() {
        if (file.is_open()) {
            file.close();
        }
    }

    bool Write(std::uint32_t address, std::uint8_t* data, std::uint16_t size) {
        std::streampos index=address;
        if (file.is_open()) {
            for(int i=index;i<index+size;i++){
                file.seekp(i);
                file.put((char)data[i-index]);
            }
            file.flush();
            return true;
        }
        return false;
    }

    bool Read(std::uint32_t address, std::uint8_t* buffer, std::uint16_t size) {
         std::streampos index=address;
        if (file.is_open()) {
            file.seekg(index);
            file.read((char*)buffer, size);
            return true;
        }
        return false;
    }

private:
    std::fstream file;
};
