/****************************************************
 * InternalData_iocShell.c
 * 
 * Define ioc shell commands
 *
 * Created by: Zheqiao Geng, gengzq@slac.stanford.edu
 * Created on: 2011.06.30
 * Description: Initial creation
 ****************************************************/
#include <epicsTypes.h>
#include <epicsExport.h>
#include <iocsh.h>

#include "InternalData.h"

#ifdef __cplusplus
extern "C" {
#endif

/*======================================
 * IOC shell Routines
 *======================================*/
/* INTD_API_genRecord */
static const iocshArg        INTD_generateRecords_Arg0    = {"dbFileName", iocshArgString};
static const iocshArg *const INTD_generateRecords_Args[1] = {&INTD_generateRecords_Arg0};
static const iocshFuncDef    INTD_generateRecords_FuncDef = {"INTD_generateRecords", 1, INTD_generateRecords_Args};
static void  INTD_generateRecords_CallFunc(const iocshArgBuf *args) {INTD_API_genRecord(args[0].sval);}

/* INTD_API_syncWithRecords */
static const iocshArg        INTD_syncWithRecords_Arg0    = {"enaCallback", iocshArgInt};
static const iocshArg *const INTD_syncWithRecords_Args[1] = {&INTD_syncWithRecords_Arg0};
static const iocshFuncDef    INTD_syncWithRecords_FuncDef = {"INTD_syncWithRecords", 1, INTD_syncWithRecords_Args};
static void  INTD_syncWithRecords_CallFunc(const iocshArgBuf *args) {INTD_API_syncWithRecords(args[0].ival);}

void INTD_IOCShellRegister(void)
{
    iocshRegister(&INTD_generateRecords_FuncDef,  INTD_generateRecords_CallFunc);
    iocshRegister(&INTD_syncWithRecords_FuncDef,  INTD_syncWithRecords_CallFunc);
}

epicsExportRegistrar(INTD_IOCShellRegister);

#ifdef __cplusplus
}
#endif






