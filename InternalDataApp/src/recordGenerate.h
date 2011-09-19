/****************************************************
 * recordGenerate.h
 * 
 * Header file for generating record from the internal data node
 *
 * Created by: Zheqiao Geng, gengzq@slac.stanford.edu
 * Created on: 2011.06.30
 * Description: Initial creation
 ****************************************************/
#ifndef RECORD_GENERATE_H
#define RECORD_GENERATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Routines for record generation
 */
void INTD_RECORD_AO(const char *moduleName, const char *recName, const char *scanMethod, char *outStr);
void INTD_RECORD_AI(const char *moduleName, const char *recName, const char *scanMethod, char *outStr);
void INTD_RECORD_BO(const char *moduleName, const char *recName, const char *scanMethod, char *outStr);
void INTD_RECORD_BI(const char *moduleName, const char *recName, const char *scanMethod, char *outStr);
void INTD_RECORD_LO(const char *moduleName, const char *recName, const char *scanMethod, char *outStr);
void INTD_RECORD_LI(const char *moduleName, const char *recName, const char *scanMethod, char *outStr);
void INTD_RECORD_MBBO(const char *moduleName, const char *recName, const char *scanMethod, char *outStr);
void INTD_RECORD_MBBI(const char *moduleName, const char *recName, const char *scanMethod, char *outStr);
void INTD_RECORD_WFO(const char *moduleName, const char *recName, const char *scanMethod, const char *pno, const char *dataType, char *outStr);
void INTD_RECORD_WFI(const char *moduleName, const char *recName, const char *scanMethod, const char *pno, const char *dataType, char *outStr);

#ifdef __cplusplus
}
#endif

#endif

