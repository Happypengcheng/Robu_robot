#ifndef PLXINIT_H
#define PLXINIT_H

/******************************************************************************
 *
 * File Name:
 *
 *      PlxInit.h
 *
 * Description:
 *
 *      Header file for the PlxInit.c module
 *
 * Revision History:
 *
 *      01-01-07 : PLX SDK v5.00
 *
 ******************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif


#include "PlxTypes.h"




/******************************
*        Definitions
******************************/
#define MAX_DEVICES_TO_LIST        50

typedef struct _API_ERRORS
{
    RETURN_CODE  code;
    char        *text;
} API_ERRORS;




/********************************************************************************
*        Functions
*********************************************************************************/
S8
SelectDevice(
    PLX_DEVICE_KEY *pKey
    );

char*
PlxSdkErrorText(
    RETURN_CODE code
    );

void
PlxSdkErrorDisplay(
    RETURN_CODE code
    );




#ifdef __cplusplus
}
#endif

#endif
