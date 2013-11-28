/**
 * Projeto      : aquario
 * Arquivo      : aquaproj.h
 *
 * Alunos       : Thyago Lainequer | Flavio
 * Data         : 12-nov-2013
 *
 */

#ifndef AQUAPROJ_H
#define	AQUAPROJ_H

#ifdef	__cplusplus
extern "C" {
#endif

// --------- definir I/O's p/ projeto aquario aqui --------- //

#define	LED1	PIN_D0
#define	LED2	PIN_D1
#define	LED3	PIN_D2
#define	LED4	PIN_D3
#define	LED5	PIN_D4
#define	LED6	PIN_D5
#define	LED7	PIN_D6
#define LED8	PIN_D7

/* pinos para teclado matricial 3x4
    col2______________
    col1_________    |
    col0______   |   |
             |   |   |
    lin0----[1] [2] [3]
    lin1----[4] [5] [6]
    lin2----[7] [8] [9]
    lin3----[*] [0] [#]
*/
#define col0    PIN_E2
#define col1    PIN_E1
#define col2    PIN_E0
#define lin0    PIN_A2
#define lin1    PIN_A3
#define lin2    PIN_A4
#define lin3    PIN_A5

#define SENSOR  PIN_A0


#ifdef	__cplusplus
}
#endif

#endif	/* AQUAPROJ_H */

