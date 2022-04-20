//--------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------
#ifndef __KM_FLASH_H
#define __KM_FLASH_H
//--------------------------------------------------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include "board.h"
//--------------------------------------------------------------------------------------------------
/**
 *
 */
extern const uint8_t *flash_addr;
//--------------------------------------------------------------------------------------------------
/**
 * @brief Program data to internal flash
 *
 * @param sector sector number to program
 * @param offset offset to the sector (multiple of KALUMA_FLASH_PAGE_SIZE)
 * @param buffer buffer to write
 * @param size size of buffer to write (multiple of KALUMA_FLASH_PAGE_SIZE)
 * @return negative on error
 */
int flash_program(uint32_t sector, uint32_t offset, uint8_t *buffer, size_t size);
//--------------------------------------------------------------------------------------------------
/**
 * @brief Erase data in internal flash
 *
 * @param sector sector number to erase
 * @param count how many sectors to erase from the sector number
 * @return negative on error
 */
int flash_erase(uint32_t sector, size_t count);
//--------------------------------------------------------------------------------------------------
#endif /* __KM_FLASH_H */
//--------------------------------------------------------------------------------------------------
