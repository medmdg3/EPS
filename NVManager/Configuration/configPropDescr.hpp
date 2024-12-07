#pragma once
#include <NVManager/Configuration/configAccessRole.hpp>
#include <array>
#include <cstring>
#include <cstdint>
#include <cstdio>

struct ConfigPropDescr {
    enum class Type : std::uint8_t {
        u8,
        i8,
        u16,
        i16,
        u32,
        i32,
        f,    /* Float.                                               */
        d,    /* Double.                                              */
        a8,   /* Byte array of 8 elements.                            */
        a16,  /* Byte array of 16 elements.                           */
        a32,  /* Byte array of 32 elements.                           */
        a64,  /* Byte array of 64 elements.                           */
        a128, /* Byte array of 128 elements.                          */
        s8,   /* String of 8 characters including null termination.   */
        s16,  /* String of 16 characters including null termination.  */
        s32,  /* String of 32 characters including null termination.  */
        s64,  /* String of 64 characters including null termination.  */
        s128, /* String of 128 characters including null termination. */
        unknown
    };

    /**
     * @brief Converts variable type to string variable.
     * 
     * @param type 
     * @return const char* 
     */
    static const char *typeToString(Type type)
    {
        const char *pResult = "";
        switch (type) {
        case Type::u8: {
            pResult = "uint8";
            break;
        }
        case Type::i8: {
            pResult = "int8";
            break;
        }
        case Type::u16: {
            pResult = "uint16";
            break;
        }
        case Type::i16: {
            pResult = "int16";
            break;
        }
        case Type::u32: {
            pResult = "uint32";
            break;
        }
        case Type::i32: {
            pResult = "int32";
            break;
        }
        case Type::f: {
            pResult = "float";
            break;
        }
        case Type::d: {
            pResult = "double";
            break;
        }
        case Type::a8: {
            pResult = "array8";
            break;
        }
        case Type::a16: {
            pResult = "array16";
            break;
        }
        case Type::a32: {
            pResult = "array32";
            break;
        }
        case Type::a64: {
            pResult = "array64";
            break;
        }
        case Type::a128: {
            pResult = "array128";
            break;
        }
        case Type::s8: {
            pResult = "string8";
            break;
        }
        case Type::s16: {
            pResult = "string16";
            break;
        }
        case Type::s32: {
            pResult = "string32";
            break;
        }
        case Type::s64: {
            pResult = "string64";
            break;
        }
        case Type::s128: {
            pResult = "string128";
            break;
        }
        case Type::unknown:
        default: {
            pResult = "unknown";
            break;
        }
        }
        return pResult;
    }

    /**
     * @brief Converts string variable to variable type.
     * 
     * @param pString 
     * @return Type 
     */
    static Type stringToType(const char *pString)
    {
        Type result = Type::unknown;
        if (0 == std::strcmp("uint8", pString)) {
            result = Type::u8;
        }
        else if (0 == std::strcmp("int8", pString)) {
            result = Type::i8;
        }
        else if (0 == std::strcmp("uint16", pString)) {
            result = Type::u16;
        }
        else if (0 == std::strcmp("int16", pString)) {
            result = Type::i16;
        }
        else if (0 == std::strcmp("uint32", pString)) {
            result = Type::u32;
        }
        else if (0 == std::strcmp("int32", pString)) {
            result = Type::i32;
        }
        else if (0 == std::strcmp("float", pString)) {
            result = Type::f;
        }
        else if (0 == std::strcmp("double", pString)) {
            result = Type::d;
        }
        else if (0 == std::strcmp("array8", pString)) {
            result = Type::a8;
        }
        else if (0 == std::strcmp("array16", pString)) {
            result = Type::a16;
        }
        else if (0 == std::strcmp("array32", pString)) {
            result = Type::a32;
        }
        else if (0 == std::strcmp("array64", pString)) {
            result = Type::a64;
        }
        else if (0 == std::strcmp("array128", pString)) {
            result = Type::a128;
        }
        else if (0 == std::strcmp("string8", pString)) {
            result = Type::s8;
        }
        else if (0 == std::strcmp("string16", pString)) {
            result = Type::s16;
        }
        else if (0 == std::strcmp("string32", pString)) {
            result = Type::s32;
        }
        else if (0 == std::strcmp("string64", pString)) {
            result = Type::s64;
        }
        else if (0 == std::strcmp("string128", pString)) {
            result = Type::s128;
        }
        return result;
    }
    /**
     * @brief Converts variable type to BYTE sizes, example: u8 -> 1 BYTE.
     * 
     * @param type 
     * @return constexpr std::size_t 
     */
    static constexpr std::size_t typeToSize(Type type)
    {
        std::size_t result = 0;
        switch (type) {
        case Type::u8: {
            result = sizeof(std::uint8_t);
            break;
        }
        case Type::i8: {
            result = sizeof(std::int8_t);
            break;
        }
        case Type::u16: {
            result = sizeof(std::uint16_t);
            break;
        }
        case Type::i16: {
            result = sizeof(std::int16_t);
            break;
        }
        case Type::u32: {
            result = sizeof(std::uint32_t);
            break;
        }
        case Type::i32: {
            result = sizeof(std::int32_t);
            break;
        }
        case Type::f: {
            result = sizeof(float);
            break;
        }
        case Type::d: {
            result = sizeof(double);
            break;
        }
        case Type::a8: {
            result = 8;
            break;
        }
        case Type::a16: {
            result = 16;
            break;
        }
        case Type::a32: {
            result = 32;
            break;
        }
        case Type::a64: {
            result = 64;
            break;
        }
        case Type::a128: {
            result = 128;
            break;
        }
        case Type::s8: {
            result = 8;
            break;
        }
        case Type::s16: {
            result = 16;
            break;
        }
        case Type::s32: {
            result = 32;
            break;
        }
        case Type::s64: {
            result = 64;
            break;
        }
        case Type::s128: {
            result = 128;
            break;
        }
        case Type::unknown:
        default: {
            result = 0;
            break;
        }
        }
        return result;
    }
    using ValidationF = bool(const std::uint8_t *pData, std::uint16_t dataLen);
    ConfigPropDescr(const std::uint16_t id,const Type type,const std::size_t offsetInStorage,ValidationF *pValidationF=nullptr,const ConfigAccessRole minAccessRole = ConfigAccessRole::user): id(id),type(type),offsetInStorage(offsetInStorage),pValidationF(pValidationF),minAccessRole(minAccessRole){}

    const std::uint16_t id;
    const char *pName;
    const Type type;
    const std::size_t offsetInStorage;
    ValidationF *pValidationF;
    const ConfigAccessRole minAccessRole = ConfigAccessRole::user;

    /**
     * @brief Calculate minimal expected storage size from the table of descriptors.
     *
     * @tparam N
     * @param propDescriptors
     * @return constexpr std::size_t
     */
    template <std::size_t N>
    static constexpr std::size_t calcMinStorageSizeFromPropDescrTable(const ConfigPropDescr (&propDescriptors)[N])
    {
        std::size_t totalSize = 0;
        for (auto &&i : propDescriptors) {
            totalSize += typeToSize(i.type);
        }
        return totalSize;
    }

    /* Validation helpers */

    template <typename T, T valueMin, T valueMax> static bool validateMinMax(const std::uint8_t *pData, std::uint16_t dataLen)
    {
        bool result = false;
        if (pData != nullptr && sizeof(T) == dataLen) {
            const T value = *reinterpret_cast<const T *>(pData);
            if (value >= valueMin && value <= valueMax) {
                result = true;
            }
        }
        return result;
    }

    template <typename T, T valueMin1, T valueMax1, T valueMin2, T valueMax2> static bool doubleMinMax(const std::uint8_t *pData, std::uint16_t dataLen)
    {
        bool result = false;
        if (pData != nullptr && sizeof(T) == dataLen) {
            const T value = *reinterpret_cast<const T *>(pData);
            if ((value >= valueMin1 && value <= valueMax1) ||
                (value >= valueMin2 && value <= valueMax2)) {
                result = true;
            }
        }
        return result;
    }

    template <typename T, T valueMin, T valueMax, T valueException>
    static bool validateMinMaxWithException(const std::uint8_t *pData, std::uint16_t dataLen)
    {
        bool result = false;
        if (pData != nullptr && sizeof(T) == dataLen) {
            const T value = *reinterpret_cast<const T *>(pData);
            if (value >= valueMin && value <= valueMax) {
                result = true;
            }
            if (valueException == value) {
                result = true;
            }
        }
        return result;
    }

    template <typename T, T value1, T value2> static bool validateEqual(const std::uint8_t *pData, std::uint16_t dataLen)
    {
        bool result = false;
        if (pData != nullptr && sizeof(T) == dataLen) {
            const T value = *reinterpret_cast<const T *>(pData);
            if (value1 == value || value2 == value) {
                result = true;
            }
        }
        return result;
    }

    template <typename T, T value1, T value2, T value3>
    static bool validateEqual(const std::uint8_t *pData, std::uint16_t dataLen)
    {
        bool result = false;
        if (pData != nullptr && sizeof(T) == dataLen) {
            const T value = *reinterpret_cast<const T *>(pData);
            if (value1 == value || value2 == value || value3 == value) {
                result = true;
            }
        }
        return result;
    }

    template <typename T>
    static bool validateCidr(const std::uint8_t *pData, std::uint16_t dataLen)
    {
        T tempBuffer = {};
        char *pTemp = &tempBuffer[0];
        std::copy(pData, pData+dataLen, pTemp);

        /* Get first token */
        pTemp = std::strtok(pTemp, ",");

        while (pTemp) {
            if(std::strlen(pTemp) < 5){
                return false;
            }
            int address = 0, netmask = 0, mac = 255;
            char name[32] = {};
            if (std::sscanf(pTemp, "%u/%u %s %u", &address, &netmask, name, &mac) != 4) {
                if (std::sscanf(pTemp, "%u/%u %s", &address, &netmask, name) != 3) {
                    return false;
                }
            }
            pTemp = std::strtok(NULL, ",");
        }

        return true;
    }
};
