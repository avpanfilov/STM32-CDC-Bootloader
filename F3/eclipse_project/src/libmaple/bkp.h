/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 LeafLabs, LLC.
 * Copyright (c) 2013 OpenMusicKontrollers.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file libmaple/stm32f3/include/series/bkp.h
 * @author F3-port by Hanspeter Portner <dev@open-music-kontrollers.ch>
 * @brief STM32F3 backup register support.
 */

#ifndef _LIBMAPLE_STM32F3_BKP_H_
#define _LIBMAPLE_STM32F3_BKP_H_


#ifdef __cplusplus
extern "C"{
#endif


#include "libmaple.h"
#include "pwr.h"


#define BKP_NR_DATA_REGS 16

/** Backup peripheral register map type. */
typedef struct bkp_reg_map {
    __IO uint32 DR0;            ///< Data register 0
    __IO uint32 DR1;            ///< Data register 1
    __IO uint32 DR2;            ///< Data register 2
    __IO uint32 DR3;            ///< Data register 3
    __IO uint32 DR4;            ///< Data register 4
    __IO uint32 DR5;            ///< Data register 5
    __IO uint32 DR6;            ///< Data register 6
    __IO uint32 DR7;            ///< Data register 7
    __IO uint32 DR8;            ///< Data register 8
    __IO uint32 DR9;            ///< Data register 9
    __IO uint32 DR10;           ///< Data register 10
    __IO uint32 DR11;           ///< Data register 11
    __IO uint32 DR12;           ///< Data register 12
    __IO uint32 DR13;           ///< Data register 13
    __IO uint32 DR14;           ///< Data register 14
    __IO uint32 DR15;           ///< Data register 15
} bkp_reg_map;

/** Backup peripheral register map base pointer. */
#define BKP_BASE  ((struct bkp_reg_map*)0x40002850) //is part of RTC memory map

/** Backup peripheral device type. */
typedef struct bkp_dev {
    bkp_reg_map *regs; /**< Register map */
} bkp_dev;

extern const bkp_dev bkp_device;
#define BKP (&bkp_device)


/**
 * @brief Initialize backup interface.
 *
 * Enables the power and backup interface clocks, and resets the
 * backup device.
 */
static inline void bkp_init(void) {
	// Don't call pwr_init(), or you'll reset the device.  We just need the clock.
	rcc_clk_enable(RCC_PWR);
}
//-----------------------------------------------------------------------------
static inline void bkp_deinit(void) {
	rcc_clk_disable(RCC_PWR);
}

/**
 * Enable write access to the backup registers.  Backup interface must
 * be initialized for subsequent register writes to work.
 * @see bkp_init()
 */
static inline void bkp_enable_writes(void) {
	PWR->CR |= PWR_CR_DBP;
}

/**
 * Disable write access to the backup registers.
 */
static inline void bkp_disable_writes(void) {
	PWR->CR &= PWR_CR_DBP;
}

/*
 * this function needs to be implemented for each series separately
 */
static inline __IO uint32* bkp_data_register(uint8 reg) {
	return (uint32*)BKP_BASE + (reg-1); // regs are accessed from 1-16
}

/**
 * Read a value from given backup data register.
 * @param reg Data register to read, from 1 to BKP_NR_DATA_REGS (10 on
 *            medium-density devices, 42 on high-density devices).
 */
static inline uint16 bkp_read(uint8 reg) {
	if (reg < 1 || reg > BKP_NR_DATA_REGS)
		return 0;
	__IO uint32* dr = bkp_data_register(reg);
	 return (uint16)*dr;
}

/**
 * @brief Write a value to given data register.
 *
 * Write access to backup registers must be enabled.
 *
 * @param reg Data register to write, from 1 to BKP_NR_DATA_REGS (10
 *            on medium-density devices, 42 on high-density devices).
 * @param val Value to write into the register.
 * @see bkp_enable_writes()
 */
static inline void bkp_write(uint8 reg, uint16 val) {
	if (reg < 1 || reg > BKP_NR_DATA_REGS)
		return;
    __IO uint32* dr = bkp_data_register(reg);
    *dr = (uint32)val;
}



#ifdef __cplusplus
}
#endif


#endif
