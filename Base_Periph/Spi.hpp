/*
 * spi.hpp
 *
 *  Created on: Jun 16, 2024
 *      Author: Youssef
 */
#include "main.hpp"
#include <array>
#include <cstdint>
#ifndef INC_BASE_PERIPH_SPI_HPP_
#define INC_BASE_PERIPH_SPI_HPP_

#define FRAM_CS_LOW()       HAL_GPIO_WritePin(_CS0_GPIO_Port, _CS0_Pin, GPIO_PIN_RESET)
#define FRAM_CS_HIGH()      HAL_GPIO_WritePin(_CS0_GPIO_Port, _CS0_Pin, GPIO_PIN_SET)
#define FRAM_WP_LOW()       HAL_GPIO_WritePin(_WP_GPIO_Port, _WP_Pin, GPIO_PIN_RESET)
#define FRAM_WP_HIGH()      HAL_GPIO_WritePin(_WP_GPIO_Port, _WP_Pin, GPIO_PIN_SET)
#define FRAM_HOLD_LOW()       HAL_GPIO_WritePin(_Hold_GPIO_Port, _Hold_Pin, GPIO_PIN_RESET)
#define FRAM_HOLD_HIGH()      HAL_GPIO_WritePin(_Hold_GPIO_Port, _Hold_Pin, GPIO_PIN_SET)

extern "C" {
#include "stm32f3xx_hal.h"
}


class Spi{
public :
	Spi();
	Spi(Spi &&) = delete;
	Spi(const Spi &) = delete;
	Spi &operator=(Spi &&) = delete;
	Spi &operator=(const Spi &) = delete;
	virtual ~Spi();

	bool init();
	virtual bool deinit();
	bool FRAM_Write(std::uint32_t address, std::uint8_t *pData, std::uint16_t dataLen);
	bool FRAM_Read(std::uint32_t address, std::uint8_t *pData, std::uint16_t dataLen);
	uint8_t FRAM_ReadStatusRegister();
	bool FRAM_WriteStatusRegister(uint8_t status);
	bool readId(std::array<std::uint8_t, 9> & id);


private :
	SPI_HandleTypeDef hspi4;
	std::uint32_t lastAddress = 0x7FFFF;
};



#endif /* INC_BASE_PERIPH_SPI_HPP_ */
