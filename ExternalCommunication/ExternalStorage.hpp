#pragma once
#include <array>
#include <cstring>
#include <cstdint>
#include <cstdio>
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
