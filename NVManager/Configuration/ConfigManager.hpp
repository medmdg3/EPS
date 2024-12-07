#pragma once
#include <NVManager/Configuration/config.hpp>
#include <NVManager/Configuration/configuration.hpp>
#include <NVManager/Configuration/ConfigVariables.hpp>
#include <NVManager/Configuration/ExternalBackup.hpp>
#include <ExternalCommunication/F-RAM.hpp>
#define  CONFIG_NV_STORAGE_SOURCES 	\
CONFIG_NV_STORAGE_SOURCE(FRAM)

enum class NonVolatileStorageSource{
#define CONFIG_NV_STORAGE_SOURCE(type) type
CONFIG_NV_STORAGE_SOURCES
#undef CONFIG_NV_STORAGE_SOURCE
};
class ConfigurationManager{

public:
     static Config* Make_Configuration(NonVolatileStorageSource type,std::uint32_t adressMain=Adress_Main,std::uint32_t adressFallback=Adress_Fallback){
    	 std::uint8_t *pActive=(std::uint8_t *)malloc(sizeof(std::uint8_t)*structSize);
    	 std::uint8_t *pMain=(std::uint8_t *)malloc(sizeof(std::uint8_t)*structSize);
    	 std::uint8_t *pFallback=(std::uint8_t *)malloc(sizeof(std::uint8_t)*structSize);
    	 ConfigProperties *properties;
    	 Config::EventCallbacks *eventCallbacks;
    	 Config::StorageCallbacks *storageCallbacks;
    	 if(type==NonVolatileStorageSource::FRAM){
			properties= new ConfigProperties(structSize,pDefault,pActive,pMain,pFallback);
			FRAM *fram=new FRAM();
			eventCallbacks=new ExternalBackupEventCallbacks();
			storageCallbacks=new ExternalBackupStorageCallbacks(*fram,adressMain,adressFallback);
		}
		pActiveProperties=pActive;
		return new Config(Descriptors,numOfDescriptors,*properties,*eventCallbacks,*storageCallbacks,123,456);
    }
};
