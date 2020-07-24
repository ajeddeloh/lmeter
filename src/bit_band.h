#pragma once

#include "stm32l4xx.h"

#define BB(REG) ((volatile uint32_t*)(PERIPH_BB_BASE + ((uint32_t)&REG - PERIPH_BASE)*32U))
