/****************************************************
 * InternalData.h
 * 
 * Header file for the InternalData module
 * This module acts as the isolation layer between EPICS records and inernal code
 *
 * Created by: Zheqiao Geng, gengzq@slac.stanford.edu
 * Created on: 2011.05.17
 * Description: Initial creation
 *              To be done: Define an interrupt I/O scanning list and API so that the user code could be
 *                able to trigger the scanning of specified records
 *
 * Modified by: Zheqiao Geng
 * Modified on: 2011.06.07
 * Description: Introduce the I/O interrupt scanning
 ****************************************************/
#ifndef INTERNAL_DATA_H
#define INTERNAL_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dbAddr.h>
#include <dbScan.h>
#include <dbCommon.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsTypes.h>
#include <ellLib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Data type supported
 */
typedef enum {
    INTD_CHAR,
    INTD_UCHAR,
    INTD_SHORT,
    INTD_USHORT,
    INTD_INT,
    INTD_UINT,
    INTD_LONG,
    INTD_ULONG,
    INTD_FLOAT,
    INTD_DOUBLE
} INTD_enum_dataType;

/**
 * Record type supported
 */
typedef enum {
    INTD_AO,
    INTD_AI,
    INTD_BO,
    INTD_BI,
    INTD_LO,
    INTD_LI,
    INTD_MBBO,
    INTD_MBBI,
    INTD_WFO,
    INTD_WFI
} INTD_enum_recordType;

typedef enum {
    INTD_PASSIVE,
    INTD_IOINT,
    INTD_10S,
    INTD_5S,
    INTD_2S,
    INTD_1S,
    INTD_D5S,
    INTD_D2S,
    INTD_D1S
} INTD_enum_scanType;

/**
 * Function pointer definition
 */
typedef void (*INTD_CALLBACK)(void*);

/**
 * Data structure of the internal data
 */
typedef struct {
    ELLNODE              node;                   /* node for elllist */
    char                 moduleName[128];        /* module name that this data belongs to */
    char                 dataName[128];          /* internal data name */
    void                *dataPtr;                /* pointer to the data */
    void                *privateData;            /* private data that might be useful in the call back functions */
    unsigned int         pno;                    /* number of the points */
    IOSCANPVT           *ioIntScan;              /* pointer of the ioscanpvt */
    INTD_enum_dataType   dataType;               /* data type */
    INTD_CALLBACK        readCallback;           /* call back function when reading the data */
    INTD_CALLBACK        writeCallback;          /* call back function when writing the data */
    dbCommon            *epicsRecord;            /* EPICS record that is associate with this internal data node */    
    epicsMutexId         mutexId;                /* provide the possibility for locking, the mutex should be defined in user code */
    epicsEventId         eventId;                /* allow signal a event when this internal data is accessed */
    INTD_enum_recordType recordType;             /* record type for this data */
    INTD_enum_scanType   scanType;               /* scan type for this data */
} INTD_struc_node;

/**
 * Management routines
 */
int INTD_API_createDataNode(const char *moduleName, 
                            const char *dataName, 
                            void *dataPtr, 
                            void *privateData,
                            unsigned int pno, 
                            IOSCANPVT *ioIntScan,
                            INTD_enum_dataType dataType,
                            INTD_CALLBACK readCallback,
                            INTD_CALLBACK writeCallback,
                            epicsMutexId mutexId,
                            epicsEventId eventId,
                            INTD_enum_recordType recordType,
                            INTD_enum_scanType   scanType);

INTD_struc_node *INTD_API_findDataNode(const char *moduleName, const char *dataName, dbCommon *epicsRecord);

int INTD_API_getData(INTD_struc_node *dataNode, unsigned int pno, void *data);
int INTD_API_putData(INTD_struc_node *dataNode, unsigned int pno, void *data);

int INTD_API_genRecord(const char *dbFileName);

int INTD_API_syncWithRecords(int enaCallback);

#ifdef __cplusplus
}
#endif

#endif

