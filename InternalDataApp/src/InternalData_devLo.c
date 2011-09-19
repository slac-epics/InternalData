/****************************************************
 * InternalData_devLo.c
 *
 * Device support for Longout record
 *
 * Created by: Zheqiao Geng, gengzq@slac.stanford.edu
 * Created on: 2011.05.18
 * Description: Initial creation. Modified from the devLoSoft.c from the EPICS base
 ****************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "longoutRecord.h"
#include "epicsExport.h"

#include "InternalData.h"

/* Create the dset for devLoSoft */
static long init_record();
static long write_longout();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	write_longout;
}devLo_internalData={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	write_longout
};
epicsExportAddress(dset,devLo_internalData);

/* Init the record (later can be defined as a common function) */
static long init_record(longoutRecord *plongout)
{
    char fullStr[512]    = "";
    char moduleName[128] = "";
    char dataName[128]   = "";

    char *pat, *pdt, *ped;

    /* Get the OUP link strings, should be "@moduleName.dataName" */
    strncpy(fullStr, plongout -> out.value.constantStr, 512);

    pat = strchr(fullStr, '@');
    pdt = strchr(fullStr, '.');
    ped = strchr(fullStr, '\0');

    strncpy(moduleName, pat + 1, pdt - pat - 1);
    strncpy(dataName,   pdt + 1, ped - pdt - 1);

    /* Attach to a internal data */
    plongout -> dpvt = (void *)INTD_API_findDataNode(moduleName, dataName, (dbCommon *)plongout);

    if(plongout -> dpvt) {
        return 0;
    } else {
        printf("longout:init_record: Failed to find the internal data of %s for the module of %s\n", dataName, moduleName);
        return -1;
    }   
} 

/* Process the record */
static long write_longout(longoutRecord	*plongout)
{
    long status;

    if(!plongout -> dpvt) return -1;

    status = INTD_API_putData((INTD_struc_node *)plongout -> dpvt, 1, (void *)(&plongout -> val));

    return status;
}


