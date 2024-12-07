#pragma once
#include <cstdint>

#define CONFIG_ACCESS_ROLES                                                                                                    \
    CONFIG_ACCESS_ROLE(user)                                                                                                   \
    CONFIG_ACCESS_ROLE(superuser)                                                                                              \
    CONFIG_ACCESS_ROLE(manufacturer)                                                                                           \
    CONFIG_ACCESS_ROLE(unknown)                                                                                                

enum class ConfigAccessRole : std::uint8_t {
#define CONFIG_ACCESS_ROLE(accessRole) accessRole,
    CONFIG_ACCESS_ROLES
#undef CONFIG_ACCESS_ROLE
};
