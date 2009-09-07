/* -------------------------------- Arctic Core ------------------------------
 * Arctic Core - the open source AUTOSAR platform http://arccore.com
 *
 * Copyright (C) 2009  ArcCore AB <contact@arccore.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * -------------------------------- Arctic Core ------------------------------*/








/*
 * context.h
 *
 *  Created on: 7 jul 2009
 *      Author: mahi
 *
 * DESCRIPTION
 *   ARM Cortex-M3 (architecture ARMv7-M).
 *
 * REFERENCES
 * - Procedure Call Standard for the ARM Architecture, release 2.07
 *
 * REGISTER USE
 *   Args
 *     r0-r3
 *
 *   Non-volatile regs (saved by function call)
 *     r4-r8,r10,r11 and SP
 *
 *   Misc
 *     r9 - Platform specific ???
 *     r12 - IP
 *     r13 - SP
 *     r14 - LR
 *     r15 - PC
 *
 * EXCEPTION FRAME
 *
 *   The following registers are auto-magically pushed by the CPU
 *   Pushes:
 *     <previous>
 *     xPSR
 *     PC (r15)
 *     LR (r14)
 *     r12,r3,r2,r1,r0
 *
 * EXCEPTION/IRQ TABLE
 *   The table is a combined exception and irq table. The first 16 (0 to 15) entries
 *   are exceptions and the rest are irq's.
 *   The table just lists the addresses of the handlers (offset of 4 bytes)
 *
 *   EXCEPTIONS
 *     There are fixed negative priority values for Reset, Hard fault and NMI.
 *     For the rest of the exceptions prio's can be set to 0-15. Priority 0 have higher
 *     priority than 1. Access through SHPRx.
 *
 *   IRQ
 *     The IRQ's also have 4-bits of priority. All IRQ prio's are squeezed into
 *     17 registers (4 in each) -> 68 IRQ's. Of the 8 bits accesssible to the
 *     priority only 4 bits are used, the least significant nibble is 0.
 *     Access through NVIC_IPR0 to IVPR_IPR16.
 *
 * EXCEPTION/IRQ FLOW
 *   - The exception hits
 *   - The handler is called
 *
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_


#define SC_PATTERN		0xde
#define LC_PATTERN		0xad

/* Minimum alignment req */
#define ARCH_ALIGN		4

/* Small context (task swap==function call) */
#define SAVE_NVGPR(_x,_y)
#define RESTORE_NVGPR(_x,_y)

/* Save volatile regs, NOT preserved by function calls */
#define SAVE_VGPR(_x,_y)
#define RESTORE_VGPR(_x,_y)

/* Large context (interrupt) */
#define SAVE_ALL_GPR(_x,_y)
#define RESTORE_ALL_GPR(_x,_y)

#define C_SIZE			16
#define C_SP_OFF		0
#define C_CONTEXT_OFF	4
#define C_LR_OFF		8
#define C_CR_OFF		12

#define C_CONTEXT_OFFS 10

#define SC_SIZE			32

#if !defined(_ASSEMBLER_)

/* These are auto-magically pushed by the hardware */
typedef struct StackException {
	uint32_t backChain;
	uint32_t psr;
	uint32_t pc;
	uint32_t lr;
	uint32_t r12;
	uint32_t r3;
	uint32_t r2;
	uint32_t r1;
} StackExceptionType;

typedef struct StackNvgpr {
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r10;
	uint32_t r11;
	uint32_t va;
} StackNvgprType;

struct StackVGpr {
	uint32_t i_have_no_idea;
};


typedef struct StackCallAndContext {
	uint32_t context;
	// possibly some backchains and other stuff here..
} StackCallAndContextType;

#endif /* _ASSEMBLER_ */


#endif /* CONTEXT_H_ */
