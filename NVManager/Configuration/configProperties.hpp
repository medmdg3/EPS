#pragma once
#include <cstdint>
#include <array>
#include <algorithm>

/**
 * @struct ConfigProperties
 * @brief Stores pointers to the actual structures of properties, where values are saved to/read from.
 *        It is assumed that pDefault structure already has all default (hardcoded) values set.
 */
struct ConfigProperties {
    ConfigProperties(std::uint16_t structSize, const void *pDefault, void *pActive, void *pMain, void *pFallback)
        : structSize(structSize), pDefault(static_cast<const std::uint8_t *>(pDefault)),
          pActive(static_cast<std::uint8_t *>(pActive)), pMain(static_cast<std::uint8_t *>(pMain)), pFallback(static_cast<std::uint8_t *>(pFallback))
    { /* Do nothing here */
    }
    ~ConfigProperties(){
    	free(pActive);
    	free(pFallback);
    	free(pMain);
    }

#define CONFIG_PROP_SOURCES                                                                                                    \
    CONFIG_PROP_SOURCE(fromDefault)                                                                                            \
    CONFIG_PROP_SOURCE(fromActive)                                                                                             \
    CONFIG_PROP_SOURCE(fromMain)                                                                                               \
    CONFIG_PROP_SOURCE(fromFallback)

    enum class Source : std::uint8_t {
#define CONFIG_PROP_SOURCE(propSource) propSource,
        CONFIG_PROP_SOURCES
#undef CONFIG_PROP_SOURCE
    };

    void copyFallbackToActive()
    {
        auto pStart = this->pFallback;
        auto pEnd = pStart + this->structSize;
        std::copy(pStart, pEnd, this->pActive);
    }

    void copyDefaultToActive()
    {
        auto pStart = this->pDefault;
        auto pEnd = pStart + this->structSize;
        std::copy(pStart, pEnd, this->pActive);
    }

    void copyMainToActive()
    {
        auto pStart = this->pMain;
        auto pEnd = pStart + this->structSize;
        std::copy(pStart, pEnd, this->pActive);
    }

    void copyActiveToMain()
    {
        auto pStart = this->pActive;
        auto pEnd = pStart + this->structSize;
        std::copy(pStart, pEnd, this->pMain);
    }

    void copyActiveToFallback()
    {
        auto pStart = this->pActive;
        auto pEnd = pStart + this->structSize;
        std::copy(pStart, pEnd, this->pFallback);
    }

    const std::uint16_t structSize = 0;
    const std::uint8_t *pDefault = nullptr;
    std::uint8_t *pActive = nullptr;
    std::uint8_t *pMain = nullptr;
    std::uint8_t *pFallback = nullptr;

};
