/*
 * 									BITWISE_OPERATIONS
 */

#ifndef BITWISE_OPERATIONS_H_
#define BITWISE_OPERATIONS_H_

#define SET_BIT(REG, BIT_NUM) (REG = REG | (1 << BIT_NUM))
#define CLEAR_BIT(REG, BIT_NUM) (REG = REG & (~(1 << BIT_NUM)))
#define TOGGLE_BIT(REG, BIT_NUM) (REG = REG ^ (1 << BIT_NUM))
#define ROR(REG, BIT_NUM) (REG = (REG >> BIT_NUM) | (REG << (8-BIT_NUM)))
#define ROL(REG, BIT_NUM) (REG = (REG << BIT_NUM) | (REG >> (8-BIT_NUM)))
#define BIT_IS_SET(REG, BIT_NUM) (REG & (1 << BIT_NUM))
#define BIT_IS_CLEAR(REG, BIT_NUM) (!(REG & (1 << BIT_NUM)))

#endif /* BITWISE_OPERATIONS_H_ */
