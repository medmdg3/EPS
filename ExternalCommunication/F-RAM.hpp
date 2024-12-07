#pragma once
#include "ExternalStorage.hpp"
#include "Spi.hpp"

class FRAM: public ExternalStorage {
public:
	FRAM(){
		bool temp=Init();
		temp&=true;
	}
    ~FRAM() {
        Deinit();
    }
	bool Init()override {
			hspi4.Instance = SPI4;
		  hspi4.Init.Mode = SPI_MODE_MASTER;
		  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
		  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
		  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
		  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
		  hspi4.Init.NSS = SPI_NSS_SOFT;
		  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
		  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
		  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
		  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
		  hspi4.Init.CRCPolynomial = 7;
		  hspi4.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
		  hspi4.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
			if (HAL_SPI_Init(&hspi4) != HAL_OK)
			{
				//Error_Handler();
				return false;

					}
		  FRAM_CS_HIGH();
		  FRAM_WP_HIGH();
		  FRAM_HOLD_HIGH();
			return true;
	}
	bool Deinit()override {
		if (HAL_SPI_DeInit(&hspi4) != HAL_OK) {
		    return false;
		}
		return true;
	}
	bool Write(std::uint32_t address, std::uint8_t* pData, std::uint16_t dataLen) override{
		std::uint8_t wrenCommand = 0x06; // Write Enable command
		std::uint8_t writeCommand = 0x02; // Write Memory command
		std::uint8_t txBuffer[4];

		txBuffer[0] = writeCommand;
		txBuffer[1] = (uint8_t)(address >> 16); // High byte (includes 3 bits)
		txBuffer[2] = (uint8_t)(address >> 8);  // Middle byte
		txBuffer[3] = (uint8_t)address;         // Low byte

		FRAM_CS_LOW();
		HAL_SPI_Transmit(&hspi4, &wrenCommand, 1, HAL_MAX_DELAY);
		FRAM_CS_HIGH();
		FRAM_CS_LOW();
		HAL_SPI_Transmit(&hspi4, txBuffer, 4, HAL_MAX_DELAY);
		HAL_SPI_Transmit(&hspi4, pData, dataLen, HAL_MAX_DELAY);
		FRAM_CS_HIGH();

		return true;
	}
	bool Read(std::uint32_t address, std::uint8_t* pData, std::uint16_t dataLen)override {
		if (address >= (1 << 19)) {
			return false;
		}

		uint8_t txBuffer[4];
		txBuffer[0] = 0x03; // Read command
		txBuffer[1] = (uint8_t)(address >> 16); // High byte (includes 3 bits)
		txBuffer[2] = (uint8_t)(address >> 8);  // Middle byte
		txBuffer[3] = (uint8_t)address;         // Low byte of address

		// Start the read operation
		FRAM_CS_LOW();
		if (HAL_SPI_Transmit(&hspi4, txBuffer, 4, HAL_MAX_DELAY) != HAL_OK) {
			FRAM_CS_HIGH();
			return false;
		}
		// Read the data
		if (HAL_SPI_Receive(&hspi4, pData, dataLen, HAL_MAX_DELAY) != HAL_OK) {
			FRAM_CS_HIGH();
			return false;
		}
		FRAM_CS_HIGH();

		return true;
	}
	uint8_t ReadStatusRegister()
	{
		uint8_t txBuffer[1];
		uint8_t rxBuffer[1];

		txBuffer[0] = 0x05; // Read Status Register command

		FRAM_CS_LOW();

		// Transmit the command
		if (HAL_SPI_Transmit(&hspi4, txBuffer, 1, HAL_MAX_DELAY) != HAL_OK)
		{
			FRAM_CS_HIGH();
			// Handle the error (e.g., return an error code or retry)
			return 0xFF; // Return an error code or status indicating failure
		}

		// Receive the status register value
		if (HAL_SPI_Receive(&hspi4, rxBuffer, 1, HAL_MAX_DELAY) != HAL_OK)
		{
			FRAM_CS_HIGH();
			// Handle the error (e.g., return an error code or retry)
			return 0xFF; // Return an error code or status indicating failure
		}

		FRAM_CS_HIGH();

		return rxBuffer[0]; // Return the status register value
	}

	bool WriteStatusRegister(std::uint8_t status)
	{
		std::uint8_t wrenCommand = 0x06; // Write Enable command
		std::uint8_t wrsrCommand = 0x01; // Write Status Register command
		std::uint8_t txBuffer[2];

		txBuffer[0] = wrsrCommand;
		txBuffer[1] = status; // Status register value to write

		// Send Write Enable command
		FRAM_CS_LOW();
		HAL_SPI_Transmit(&hspi4, &wrenCommand, 1, HAL_MAX_DELAY);
		HAL_SPI_Transmit(&hspi4, txBuffer, 2, HAL_MAX_DELAY);
		FRAM_CS_HIGH();
		return true;
	}
private:
	SPI_HandleTypeDef hspi4;
	const std::uint32_t lastAddress = 0x7FFFF;
};
