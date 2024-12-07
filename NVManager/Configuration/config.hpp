#pragma once
#include <NVManager/Configuration/configPropDescr.hpp>
#include <NVManager/Configuration/configProperties.hpp>
#include <type_traits>

class Config {
public:
    struct EventCallbacks {
        virtual ~EventCallbacks() {}
        /**
         * @brief Function that is called when a new value to the property is set.
         *
         * @param propId is an ID of the property whose value has changed.
         */
        virtual void onPropertyChange(std::uint16_t propId) = 0;

        /**
         * @brief Function that is called when configuration is saved into non-volatile memory.
         *
         * @param asFallback true if saved as fallback configuration.
         */
        virtual void onSave(bool asFallback) = 0;
    };

    struct StorageCallbacks {
        virtual ~StorageCallbacks() {}

        /**
         * @brief Load latest valid properties from the non volatile memory.
         * @param pPropertiesDest is destination buffer where the values will be saved.
         * @param Backup is the starting  of the backup in the non viotile memory
         * @param dataLen the size of the data to be copied in Bytes
         * @return true if loading successful. 
         * @return false if loading failed or loaded size differs from expected.
         */
        virtual bool loadProperties(std::uint8_t* pPropertiesDest,std::uint32_t dataLen, bool FromFallBack) = 0;

        /**
         * @brief Save properties into the non volatile memory.
         *
         * @param pPropertiesSrc is source buffer where the actual values are saved.
         * @param Backup is the starting  of the backup in the non viotile memory
         * @param dataLen the size of the data to be copied in Bytes
         * @return true if saving was successful.
         * @return false if saving was unsuccessful.
         */
        virtual bool saveProperties(std::uint8_t* pPropertiesSrc,std::uint32_t dataLen, bool FromFallBack) = 0;
    };
#define CONFIG_ERROR_CODES                                                                                                     \
    CONFIG_ERROR_CODE(ok)                                                                                                      \
    CONFIG_ERROR_CODE(parameterDoesNotExist)                                                                                   \
    CONFIG_ERROR_CODE(invalidParameterType)                                                                                    \
    CONFIG_ERROR_CODE(validationFail)                                                                                          \
    CONFIG_ERROR_CODE(storageFail)                                                                                             \
    CONFIG_ERROR_CODE(notInitialized)                                                                                          \
    CONFIG_ERROR_CODE(notLoadedOnInit)                                                                                         \
    CONFIG_ERROR_CODE(locked)                                                                                                  \
    CONFIG_ERROR_CODE(accessDenied)                                                                                            \
    CONFIG_ERROR_CODE(wrongPassword)                                                                                           \
    CONFIG_ERROR_CODE(unknown)

    enum class ErrorCode : std::uint8_t {
#define CONFIG_ERROR_CODE(errorCode) errorCode,
        CONFIG_ERROR_CODES
#undef CONFIG_ERROR_CODE
    };

    /**
     * @struct Status
     * @brief Status structure.
     */
    struct Status {
        bool isInitialized = false;
        bool isLocked = true;
        bool isFallbackConfigurationLoaded = false;
        bool isMainConfigurationLoaded = false;
        bool wasFallbackRequested = false;
        ConfigProperties::Source initSource = ConfigProperties::Source::fromMain;
        ConfigAccessRole currAccessRole = ConfigAccessRole::user;
        std::uint8_t totalNumOfErrors = 0;
        std::uint8_t numOfParameterDoesNotExistErrors = 0;
        std::uint8_t numOfInvalidParameterTypeErrors = 0;
        std::uint8_t numOfValidationFailErrors = 0;
        std::uint8_t numOfStorageFailErrors = 0;
        std::uint8_t numOfNotInitializedErrors = 0;
        std::uint8_t numOfNotLoadedOnInitErrors = 0;
        std::uint8_t numOfLockedErrors = 0;
        std::uint8_t numOfAccessDeniedErrors = 0;
        std::uint8_t numOfWrongPasswordErrors = 0;
        std::uint8_t numOfUnknownErrors = 0;

        ErrorCode registerErrorAndReturn(ErrorCode errorCode){
        	switch (errorCode){
        	case ErrorCode::ok :{
        		break;
        	}
        	case ErrorCode::parameterDoesNotExist :{
        		numOfParameterDoesNotExistErrors++;
        		break;
        	}
        	case ErrorCode::invalidParameterType :{
        		numOfInvalidParameterTypeErrors++;
				break;
			}
        	case ErrorCode::validationFail :{
        		numOfValidationFailErrors++;
			break;
			}
			case ErrorCode::storageFail :{
				numOfStorageFailErrors++;
				break;
			}
			case ErrorCode::notInitialized :{
				numOfNotInitializedErrors++;
				break;
			}
			case ErrorCode::notLoadedOnInit :{
				numOfNotLoadedOnInitErrors++;
				break;
			}
			case ErrorCode::locked :{
				numOfLockedErrors++;
				break;
			}
			case ErrorCode::accessDenied :{
				numOfAccessDeniedErrors++;
				break;
			}
			case ErrorCode::wrongPassword :{
				numOfWrongPasswordErrors++;
				break;
			}
			case ErrorCode::unknown :{
				numOfUnknownErrors++;
				break;
        	}
        	}
        	if(ErrorCode::ok != errorCode)
        		totalNumOfErrors++;
            return errorCode;
        }
    };

    Config(const ConfigPropDescr *pDescriptors, std::uint16_t numOfDescriptors, ConfigProperties properties,
           EventCallbacks &eventCallbacks, StorageCallbacks &storageCallbacks, std::uint32_t superuserPassword = 123,
           std::uint32_t manufacturerPassword = 456):pDescriptors(pDescriptors),numOfDescriptors(numOfDescriptors),properties(properties),eventCallbacks(eventCallbacks),storageCallbacks(storageCallbacks),superuserPassword(superuserPassword),manufacturerPassword(manufacturerPassword){}
    Config(Config &&) = delete;
    Config(const Config &) = delete;
    Config &operator=(Config &&) = delete;
    Config &operator=(const Config &) = delete;
    ~Config(){

    	delete &properties;
    	delete &eventCallbacks;
    	delete &storageCallbacks;
    }

    /**
     * @brief Changes `CONFIG_ERROR_CODES` to string variable.
     * 
     * @param errorCode 
     * @return const char* 
     */
    static const char *errorCodeToString(ErrorCode errorCode){
    	const char *pResult = "";
    	switch (errorCode) {
			case ErrorCode::ok: {
				pResult = "OK";
				break;
			}
			case ErrorCode::parameterDoesNotExist: {
				 pResult = "parameterDoesNotExist";
				 break;
			}
			case ErrorCode::invalidParameterType: {
				pResult = "invalidParameterType";
				break;
			}
			case ErrorCode::validationFail: {
				pResult = "validationFail";
				break;
			}
			case ErrorCode::storageFail: {
				pResult = "storageFail";
				break;
			}
			case ErrorCode::notInitialized: {
				 pResult = "notInitialized";
				 break;
			}
			case ErrorCode::notLoadedOnInit: {
				pResult = "notLoadedOnInit";
				break;
			}
			case ErrorCode::locked: {
				pResult = "locked";
				break;
			}
			case ErrorCode::accessDenied: {
				pResult = "accessDenied";
				break;
			}
			case ErrorCode::wrongPassword: {
				pResult = "wrongPassword";
				break;
			}
			case ErrorCode::unknown: {
				pResult = "unknown";
				break;
			}
		}
		return pResult;
    }
    /**
     * @brief Changes `CONFIG_ACCESS_ROLES` to string variable.
     * 
     * @param accessRole 
     * @return const char* 
     */
    static const char *accessRoleToString(ConfigAccessRole accessRole){
    	const char* pResult ="";
    	switch(accessRole){
			case ConfigAccessRole::user:{
				pResult="user";
				break;
			}
			case ConfigAccessRole::superuser:{
				pResult="user";
				break;
			}
			case ConfigAccessRole::manufacturer:{
				pResult="user";
				break;
			}
			case ConfigAccessRole::unknown:{
				pResult="user";
				break;
			}
    	}
    	return pResult;
    }
    /**
     * @brief Changes `CONFIG_PROP_SOURCES` to string variable.
     * 
     * @param source 
     * @return const char* 
     */
    static const char *propSourceToString(ConfigProperties::Source source){
    	const char* pResult ="";
		switch(source){
			case ConfigProperties::Source::fromActive:{
				pResult="fromActive";
				break;
			}
			case ConfigProperties::Source::fromDefault:{
				pResult="fromActive";
				break;
			}
			case ConfigProperties::Source::fromMain:{
				pResult="fromActive";
				break;
			}
			case ConfigProperties::Source::fromFallback:{
				pResult="fromActive";
				break;
			}
		}
		return pResult;
    }
    /**
     * @brief 
     * 
     * @tparam T 
     * @param t 
     * @return constexpr std::underlying_type<T>::type 
     */
    template <typename T> static constexpr typename std::underlying_type<T>::type toUnderlying(T t)
    {
        return static_cast<typename std::underlying_type<T>::type>(t);
    }

    /**
     * @brief Get the Type object
     * 
     * @tparam T 
     * @return constexpr ConfigPropDescr::Type 
     */
    template <typename T> constexpr static ConfigPropDescr::Type getType()
    {
        using DecayType = typename std::decay<T>::type; /* Remove reference, const or volatile from type. */
        if (true == std::is_same<DecayType, std::uint8_t>::value) {
            return ConfigPropDescr::Type::u8;
        }
        else if (true == std::is_same<DecayType, std::int8_t>::value) {
            return ConfigPropDescr::Type::i8;
        }
        else if (true == std::is_same<DecayType, std::uint16_t>::value) {
            return ConfigPropDescr::Type::u16;
        }
        else if (true == std::is_same<DecayType, std::int16_t>::value) {
            return ConfigPropDescr::Type::i16;
        }
        else if (true == std::is_same<DecayType, std::uint32_t>::value) {
            return ConfigPropDescr::Type::u32;
        }
        else if (true == std::is_same<DecayType, std::int32_t>::value) {
            return ConfigPropDescr::Type::i32;
        }
        else if (true == std::is_same<DecayType, float>::value) {
            return ConfigPropDescr::Type::f;
        }
        else if (true == std::is_same<DecayType, double>::value) {
            return ConfigPropDescr::Type::d;
        }
        else if (true == std::is_same<DecayType, std::array<std::uint8_t, 8>>::value) {
            return ConfigPropDescr::Type::a8;
        }
        else if (true == std::is_same<DecayType, std::array<std::uint8_t, 16>>::value) {
            return ConfigPropDescr::Type::a16;
        }
        else if (true == std::is_same<DecayType, std::array<std::uint8_t, 32>>::value) {
            return ConfigPropDescr::Type::a32;
        }
        else if (true == std::is_same<DecayType, std::array<std::uint8_t, 64>>::value) {
            return ConfigPropDescr::Type::a64;
        }
        else if (true == std::is_same<DecayType, std::array<std::uint8_t, 128>>::value) {
            return ConfigPropDescr::Type::a128;
        }
        else if (true == std::is_same<DecayType, std::array<char, 8>>::value) {
            return ConfigPropDescr::Type::s8;
        }
        else if (true == std::is_same<DecayType, std::array<char, 16>>::value) {
            return ConfigPropDescr::Type::s16;
        }
        else if (true == std::is_same<DecayType, std::array<char, 32>>::value) {
            return ConfigPropDescr::Type::s32;
        }
        else if (true == std::is_same<DecayType, std::array<char, 64>>::value) {
            return ConfigPropDescr::Type::s64;
        }
        else if (true == std::is_same<DecayType, std::array<char, 128>>::value) {
            return ConfigPropDescr::Type::s128;
        }
        return ConfigPropDescr::Type::unknown;
    }

    /**
     * @brief `Config` initialization.
     * 
     * @param fallbackRequest 
     * @return true if initialization was successful.
     * @return false if initialization was unsuccessful.
     */
    bool init(bool fallbackRequest = false){
    	loadMain();
    	loadFallback();
    	switch (this->currStatus.initSource){
    		case ConfigProperties::Source::fromActive:{
				return false;
			}
			case ConfigProperties::Source::fromDefault:{
				properties.copyDefaultToActive();
				break;
			}
			case ConfigProperties::Source::fromMain:{
				if(ErrorCode::ok != ActivateMain()){
					return false;
				}
				break;
			}
			case ConfigProperties::Source::fromFallback:{
				if(ErrorCode::ok != ActivateMain()){
					return false;
				}
				break;
			}
    	}
    	this->currStatus.isInitialized=true;
    	return true;
    }
    /**
     * @brief Get the `Config` object status.
     * 
     * @return Status 
     */
    Status getCurrStatus() { return this->currStatus; }
    /**
     * @brief Changes access role. 
     * 
     * @note There are 3 assess roles: `USER`, `SUPERUSER` and `MANUFACTURER`.
     * @param role 
     * @param password using for `SUPERUSER` or `MANUFACTURER. For `USER` role no need any password.
     * @return ErrorCode 
     */
    ErrorCode elevateAccessRole(ConfigAccessRole role, std::uint32_t password = 0){
    	if (false == this->currStatus.isInitialized) {
    		return this->currStatus.registerErrorAndReturn(ErrorCode::notInitialized);
    	}
    	if(role==ConfigAccessRole::user){
    		this->currStatus.currAccessRole=role;
    		return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
    	}
    	if(role==ConfigAccessRole::superuser){
			if(password==superuserPassword){
				this->currStatus.currAccessRole=role;
				return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
			}else{
				return this->currStatus.registerErrorAndReturn(ErrorCode::wrongPassword);
			}
		}
    	if(role==ConfigAccessRole::manufacturer){
    		if(password==manufacturerPassword){
    			this->currStatus.currAccessRole=role;
				return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
    		}else{
    			return this->currStatus.registerErrorAndReturn(ErrorCode::wrongPassword);
    		}
    	}
    	return this->currStatus.registerErrorAndReturn(ErrorCode::unknown);
    }
    /**
     * @brief Get the `Config` object current access role.
     * 
     * @return ConfigAccessRole 
     */
    ConfigAccessRole getCurrAccessRole() { return this->currStatus.currAccessRole; }
    /**
     * @brief Loads Main properties from the non volatile memory to the storage.
     * 
     * @return ErrorCode 
     */
    ErrorCode loadMain(){
    	if(storageCallbacks.loadProperties(properties.pMain, properties.structSize, false)){
    		this->currStatus.isMainConfigurationLoaded=true;
    		return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
    	}
    	return this->currStatus.registerErrorAndReturn(ErrorCode::storageFail);
    }
    /**
     * @brief Loads Fallback properties from the non volatile memory to the storage.
     * 
     * @return ErrorCode 
     */
    ErrorCode loadFallback(){
    	if(storageCallbacks.loadProperties(properties.pFallback, properties.structSize, false)){
			this->currStatus.isFallbackConfigurationLoaded=true;
			return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
		}
		return this->currStatus.registerErrorAndReturn(ErrorCode::storageFail);
    }
    /**
	 * @brief Loads Main properties from the storage memory to the actif memory.
	 *
	 * @return ErrorCode
	 */
	ErrorCode ActivateMain(){
		if(false == this->currStatus.isInitialized){
			return this->currStatus.registerErrorAndReturn(ErrorCode::notInitialized);
		}
		if(false == this->currStatus.isMainConfigurationLoaded){
			return this->currStatus.registerErrorAndReturn(ErrorCode::notLoadedOnInit);
		}
		properties.copyMainToActive();
		return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
	}
	/**
	 * @brief Loads Fallback properties from the storage memory to the actif memory.
	 *
	 * @return ErrorCode
	 */
	ErrorCode ActivateFallback(){
		if(false == this->currStatus.isInitialized){
			return this->currStatus.registerErrorAndReturn(ErrorCode::notInitialized);
		}
		if(false == this->currStatus.isFallbackConfigurationLoaded){
			return this->currStatus.registerErrorAndReturn(ErrorCode::notLoadedOnInit);
		}
		properties.copyFallbackToActive();
		this->currStatus.wasFallbackRequested=true;
		return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
	}
    /**
     * @brief Unlocks storage.
     * 
     * @return ErrorCode 
     */
    ErrorCode unlock(){
    	if (false == this->currStatus.isInitialized) {
			return this->currStatus.registerErrorAndReturn(ErrorCode::notInitialized);
		}
    	this->currStatus.isLocked=false;
		return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
    }
    /**
     * @brief Locks storage.
     * 
     * @return ErrorCode 
     */
    ErrorCode lock(){
    	if (false == this->currStatus.isInitialized) {
			return this->currStatus.registerErrorAndReturn(ErrorCode::notInitialized);
		}
    	this->currStatus.isLocked=true;
		return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
    }
    /**
     * @brief Saves active properties to storage.	minimum Access Role is superuser
     * 
     * @param asFallback 
     * @return ErrorCode 
     */
    ErrorCode save(bool asFallback = false){
    	if (false == this->currStatus.isInitialized) {
    		return this->currStatus.registerErrorAndReturn(ErrorCode::notInitialized);
    	}
    	if (this->currStatus.currAccessRole<ConfigAccessRole::superuser) {
			return this->currStatus.registerErrorAndReturn(ErrorCode::accessDenied);
		}
    	if(this->currStatus.isLocked){
    		return this->currStatus.registerErrorAndReturn(ErrorCode::locked);
    	}
    	if(false == asFallback){
    		properties.copyActiveToMain();
    		return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
    	}
		properties.copyActiveToFallback();
		this->eventCallbacks.onSave(asFallback);
		return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
    }

    /**
	 * @brief Saves stored properties to the non volatile memory. minimum Access Role is manufacturer
	 *
	 * @param asFallback
	 * @return ErrorCode
	 */
	ErrorCode DeepSave(bool asFallback = false){
		if (false == this->currStatus.isInitialized) {
			return this->currStatus.registerErrorAndReturn(ErrorCode::notInitialized);
		}
		if (this->currStatus.currAccessRole<ConfigAccessRole::manufacturer) {
			return this->currStatus.registerErrorAndReturn(ErrorCode::accessDenied);
		}
		if(this->currStatus.isLocked){
			return this->currStatus.registerErrorAndReturn(ErrorCode::locked);
		}
		if(false == asFallback){
			if(storageCallbacks.saveProperties(properties.pMain, properties.structSize, false)){
				return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
			}
			return this->currStatus.registerErrorAndReturn(ErrorCode::storageFail);
		}
		if(storageCallbacks.saveProperties(properties.pFallback, properties.structSize, true)){
			return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
		}
		return this->currStatus.registerErrorAndReturn(ErrorCode::storageFail);
	}
    template <typename I, typename T> ErrorCode get(I id, T &value, ConfigProperties::Source source)
    {
        return this->get(this->toUnderlying(id), value, source);
    }

    template <typename T> ErrorCode get(std::uint16_t id, T &value, ConfigProperties::Source source)
    {
        switch (source) {
        case ConfigProperties::Source::fromDefault:
            return this->getDefault(id, value);
            break;
        case ConfigProperties::Source::fromActive:
            return this->get(id, value);
            break;
        case ConfigProperties::Source::fromMain:
            return this->getMain(id, value);
            break;
        case ConfigProperties::Source::fromFallback:
            return this->getFallback(id, value);
            break;
        default:
            break;
        }
        return this->currStatus.registerErrorAndReturn(ErrorCode::validationFail);
    }

    template <typename T> ErrorCode get(const ConfigPropDescr &descr, T &value, ConfigProperties::Source source)
    {
        switch (source) {
        case ConfigProperties::Source::fromDefault:
            return this->getDefault(descr, value);
            break;
        case ConfigProperties::Source::fromActive:
            return this->get(descr, value);
            break;
        case ConfigProperties::Source::fromMain:
            return this->getMain(descr, value);
            break;
        case ConfigProperties::Source::fromFallback:
            return this->getFallback(descr, value);
            break;
        default:
            break;
        }
        return this->currStatus.registerErrorAndReturn(ErrorCode::validationFail);
    }

    /* Get active. */
    template <typename I, typename T> ErrorCode get(I id, T &value)
    {
        return this->get(this->toUnderlying(id), value, this->properties.pActive);
    }

    template <typename T> ErrorCode get(std::uint16_t id, T &value) { return this->get(id, value, this->properties.pActive); }

    template <typename T> ErrorCode get(const ConfigPropDescr &descr, T &value)
    {
        return this->get(descr, value, this->properties.pActive);
    }

    /* Get default. */
    template <typename I, typename T> ErrorCode getDefault(I id, T &value)
    {
        return this->get(this->toUnderlying(id), value, this->properties.pDefault);
    }

    template <typename T> ErrorCode getDefault(std::uint16_t id, T &value)
    {
        return this->get(id, value, this->properties.pDefault);
    }

    template <typename T> ErrorCode getDefault(const ConfigPropDescr &descr, T &value)
    {
        return this->get(descr, value, this->properties.pDefault);
    }

    /* Get main. */
    template <typename I, typename T> ErrorCode getMain(I id, T &value)
    {
        return this->get(this->toUnderlying(id), value, this->properties.pMain);
    }

    template <typename T> ErrorCode getMain(std::uint16_t id, T &value)
    {
        if (false == this->currStatus.isMainConfigurationLoaded) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::notLoadedOnInit);
        }
    	return this->get(id, value, this->properties.pMain);
    }

    template <typename T> ErrorCode getMain(const ConfigPropDescr &descr, T &value)
    {
        if (false == this->currStatus.isMainConfigurationLoaded) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::notLoadedOnInit);
        }
        return this->get(descr, value, this->properties.pMain);
    }

    /* Get fallback. */
    template <typename I, typename T> ErrorCode getFallback(I id, T &value)
    {
        if (false == this->currStatus.isFallbackConfigurationLoaded) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::notLoadedOnInit);
        }
        return this->get(this->toUnderlying(id), value, this->properties.pFallback);
    }

    template <typename T> ErrorCode getFallback(std::uint16_t id, T &value)
    {

        if (false == this->currStatus.isFallbackConfigurationLoaded) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::notLoadedOnInit);
        }
        return this->get(id, value, this->properties.pFallback);
    }

    template <typename T> ErrorCode getFallback(const ConfigPropDescr &descr, T &value)
    {

        if (false == this->currStatus.isFallbackConfigurationLoaded) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::notLoadedOnInit);
        }
        return this->get(descr, value, this->properties.pFallback);
    }

    template <typename I> ErrorCode getType(I id, ConfigPropDescr::Type &type)
    {
        return this->getType(this->toUnderlying(id), type);
    }

    ErrorCode getType(std::uint16_t id, ConfigPropDescr::Type &type){
    	if(false == this->currStatus.isInitialized) return this->currStatus.registerErrorAndReturn(ErrorCode::notInitialized);
    	const ConfigPropDescr *Descriptor=getDescriptor(id);
    	if(Descriptor==nullptr)	return this->currStatus.registerErrorAndReturn(ErrorCode::parameterDoesNotExist);
    	type=Descriptor->type;
    	return this->currStatus.registerErrorAndReturn(ErrorCode::ok);
    }

    template <typename I> const ConfigPropDescr *getDescriptor(I id) { return this->getDescriptor(this->toUnderlying(id)); }

    const ConfigPropDescr *getDescriptor(std::uint16_t id){
    	if(id < 0 || id >= numOfDescriptors) return nullptr;
    	return pDescriptors+id;
    }

    template <typename I, typename T> ErrorCode set(I id, const T value) { return this->set(this->toUnderlying(id), value); }

    template <typename T> ErrorCode set(std::uint16_t id, const T newValue)
    {
        if (false == this->currStatus.isInitialized) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::notInitialized);
        }
        const ConfigPropDescr *pDescr = this->findDescriptor(id);
        if (nullptr == pDescr) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::parameterDoesNotExist);
        }
        // Get what property type is expected from this function call
        constexpr ConfigPropDescr::Type currType = Config::getType<T>();
        static_assert(currType != ConfigPropDescr::Type::unknown, "Unknown type passed to the configuration!");

        if (currType != pDescr->type) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::invalidParameterType);
        }
        // Check access role
        if (this->currStatus.currAccessRole < pDescr->minAccessRole) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::accessDenied);
        }

        /* Validate */
        bool isValid = false;
        if (pDescr->pValidationF != nullptr) {
            isValid = pDescr->pValidationF(reinterpret_cast<const std::uint8_t *>(&newValue), sizeof(T));
        }
        else { /* Validation method does not exist, skip it */
            isValid = true;
        }
        if (false == isValid) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::validationFail);
        }
        /* Write value into active properties */
        T *p = reinterpret_cast<T *>(this->properties.pActive + pDescr->offsetInStorage);
        *p = newValue;
        this->eventCallbacks.onPropertyChange(id);
        return ErrorCode::ok;
    }

    /* Public members. */
    const ConfigPropDescr *pDescriptors = nullptr;
    const std::uint16_t numOfDescriptors = 0;

private:
    const ConfigPropDescr *findDescriptor(std::uint16_t id);
    const ConfigPropDescr *findDescriptor(const char *pName);

    template <typename T> ErrorCode get(std::uint16_t id, T &value, const std::uint8_t *pProperties)
    {
        if (false == this->currStatus.isInitialized) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::notInitialized);
        }
        const ConfigPropDescr *pDescr = this->findDescriptor(id);
        if (nullptr == pDescr) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::parameterDoesNotExist);
        }
        return this->get(*pDescr, value, pProperties);
    }

    template <typename T> ErrorCode get(const ConfigPropDescr &descr, T &value, const std::uint8_t *pProperties)
    {
        if (false == this->currStatus.isInitialized) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::notInitialized);
        }
        // Get what property type is expected from this function call
        constexpr ConfigPropDescr::Type currType = Config::getType<T>();
        static_assert(currType != ConfigPropDescr::Type::unknown, "Unknown type passed to the configuration!");

        if (currType != descr.type) {
            return this->currStatus.registerErrorAndReturn(ErrorCode::invalidParameterType);
        }
        /* Read value from specified properties */
        value = *reinterpret_cast<const T *>(pProperties + descr.offsetInStorage);
        return ErrorCode::ok;
    }
    Status currStatus;
    ConfigProperties &properties;
    EventCallbacks &eventCallbacks;
    StorageCallbacks &storageCallbacks;
    const std::uint32_t superuserPassword = 0;
    const std::uint32_t manufacturerPassword = 0;
};
