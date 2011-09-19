/****************************************************
 * InternalData_devLi.c
 *
 * Device support for Longin record
 *
 * Created by: Zheqiao Geng, gengzq@slac.stanford.edu
 * Created on: 2011.06.22
 * Description: Initial creation. Modified from the devLiSoft.c from the EPICS base
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
#include "longinRecord.h"
#include "epicsExport.h"

#include "InternalData.h"

/* Create the dset for devLiSoft */
static long init_record();
static long read_longin();
static long get_ioint_info(int delFrom, dbCommon *prec, IOSCANPVT *ppvt);

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_longin;
}devLi_internalData={
	5,
	NULL,
	NULL,
	init_record,
	get_ioint_info,
	read_longin
};
epicsExportAddress(dset,devLi_internalData);

/* Init the record (later can be defined as a common function) */
static long init_record(longinRecord *plongin)
{
    char fullStr[512]    = "";
    char moduleName[128] = "";
    char dataName[128]   = "";

    char *pat, *pdt, *ped;

    /* Get the OUP link strings, should be "@moduleName.dataName" */
    strncpy(fullStr, plongin -> inp.value.constantStr, 512);

    pat = strchr(fullStr, '@');
    pdt = strchr(fullStr, '.');
    ped = strchr(fullStr, '\0');

    strncpy(moduleName, pat + 1, pdt - pat - 1);
    strncpy(dataName,   pdt + 1, ped - pdt - 1);

    /* Attach to a internal data */
    plongin -> dpvt = (void *)INTD_API_findDataNode(moduleName, dataName, (dbCommon *)plongin);

    if(plongin -> dpvt) {
        return 0;
    } else {
        printf("longin:init_record: Failed to find the internal data of %s for the module of %s\n", dataName, moduleName);
        return -1;
    }   
} 

/* For I/O interrupt scanning */
static long get_ioint_info(int delFrom, dbCommon *prec, IOSCANPVT *ppvt)
{
    longinRecord    *plongin      = (longinRecord *)prec;
    INTD_struc_node *dataNode = (INTD_struc_node *)plongin -> dpvt;

    if(!dataNode || !dataNode -> ioIntScan) return -1;

    *ppvt = *dataNode -> ioIntScan;
    return 0;
}

/* Process the record */
static long read_longin(longinRecord *plongin)
{
    long status;    
    INTD_struc_node *dataNode = (INTD_struc_node *)plongin -> dpvt;

    if(!dataNode) return -1;

    status = INTD_API_getData(dataNode, 1, (void *)(&plongin -> val));

    plongin -> udf = FALSE;

    if(status == 0) return 2;
    else return status;

}


