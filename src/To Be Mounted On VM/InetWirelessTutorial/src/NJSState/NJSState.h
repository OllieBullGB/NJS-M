/*
 * NJSState.h
 *
 *  Created on: Jan 3, 2025
 *      Author: veins
 */

#ifndef NJSSTATE_NJSSTATE_H_
#define NJSSTATE_NJSSTATE_H_

enum NJSState
{
    IDLE = 0,
    RECEIVING = 1,
    TRANSMITTING = 2,
    CORRUPTED = 3
};


#endif /* NJSSTATE_NJSSTATE_H_ */
