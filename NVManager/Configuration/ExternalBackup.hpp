#include <NVManager/Configuration/config.hpp>
#include <ExternalCommunication/F-RAM.hpp>
struct ExternalBackupStorageCallbacks: public Config::StorageCallbacks {
    ExternalBackupStorageCallbacks(ExternalStorage &ExternalStorageSource,std::uint32_t adressMain, std::uint32_t adressFallBack): ExternalStorageSource(ExternalStorageSource), adressMain(adressMain),adressFallBack(adressFallBack){}
    ~ExternalBackupStorageCallbacks(){
    	delete &ExternalStorageSource;
    }
    ExternalStorage &ExternalStorageSource;
    const std::uint32_t adressMain;
    const std::uint32_t adressFallBack;
     bool loadProperties(std::uint8_t* pPropertiesDest,std::uint32_t dataLen, bool FromFallBack) override{
         if(FromFallBack)
             return ExternalStorageSource.Read(adressFallBack, pPropertiesDest, dataLen);
         else
             return ExternalStorageSource.Read(adressMain, pPropertiesDest, dataLen);
     }

    bool saveProperties(std::uint8_t* pPropertiesSrc,std::uint32_t dataLen,bool FromFallBack) override {
         if(FromFallBack)
             return ExternalStorageSource.Write(adressFallBack, pPropertiesSrc, dataLen);
         else
             return ExternalStorageSource.Write(adressMain, pPropertiesSrc, dataLen);
    }
};

struct ExternalBackupEventCallbacks:public Config::EventCallbacks {
    ExternalBackupEventCallbacks(){}
    virtual void onPropertyChange(std::uint16_t propId) {

    }

    /**
     * @brief Function that is called when configuration is saved into non-volatile memory.
     *
     * @param asFallback true if saved as fallback configuration.
     */
    virtual void onSave(bool asFallback){

    }
};
