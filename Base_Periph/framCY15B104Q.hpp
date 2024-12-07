/*
 * framCY15B104Q.hpp
 *
 *  Created on: Jun 16, 2024
 *      Author: Youssef
 */

#ifndef INC_BASE_PERIPH_FRAMCY15B104Q_HPP_
#define INC_BASE_PERIPH_FRAMCY15B104Q_HPP_


struct Consts
    {
        static constexpr std::array<std::uint8_t, 9> deviceId512k = {{ 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0xC2, 0x26, 0x08 }};
        struct OpCodes
        {
            static constexpr std::uint8_t wren = 0x06;  /* set write enable latch.   */
            static constexpr std::uint8_t wrdi = 0x04;  /* reset write enable latch. */
            static constexpr std::uint8_t rdsr = 0x05;  /* read Status register.     */
            static constexpr std::uint8_t wrsr = 0x01;  /* write Status register.    */
            static constexpr std::uint8_t read = 0x03;  /* read memory data.         */
            static constexpr std::uint8_t fstrd = 0x0B; /* fast read memory data.    */
            static constexpr std::uint8_t write = 0x02; /* write memory data.        */
            static constexpr std::uint8_t sleep = 0xB9; /* enter sleep mode.         */
            static constexpr std::uint8_t rdid = 0x9F;  /* read device ID.           */
        };
    };



#endif /* INC_BASE_PERIPH_FRAMCY15B104Q_HPP_ */
