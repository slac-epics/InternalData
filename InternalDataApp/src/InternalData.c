/****************************************************
 * InternalData.c
 * 
 * Header file for the InternalData module
 * This module acts as the isolation layer between EPICS records and inernal code
 *
 * Created by: Zheqiao Geng, gengzq@slac.stanford.edu
 * Created on: 2011.05.17
 * Description: Initial creation
 *
 * Modified by: Zheqiao Geng
 * Modified on: 2/7/2013
 * Description: allow to specify the path for saving the files
 ****************************************************/
#include "InternalData.h"
#include "recordGenerate.h"

/*======================================
 * Global variables
 *======================================*/
static ELLLIST INTD_gvar_dataList;                                  /* later we need a mutex to protect this list! */
static int INTD_gvar_dataListInitialized = 0;

static char *INTD_gvar_scanStrs[] = {                               /* string for different scanning */
    "Passive",
    "I/O Intr",
    "10 second",
    "5 second",
    "2 second",
    "1 second",
    ".5 second",
    ".2 second",
    ".1 second"
};

int INTD_gvar_enaCallback = 1;                                      /* global switch for all data, 1 to enable the call back functions */

/*======================================
 * Private routines
 *======================================*/
static int INTD_func_getDataSize(INTD_enum_dataType dataType)
{
    int var_dataSize = 0;

    switch(dataType) {
        case INTD_CHAR:
        case INTD_UCHAR:
            var_dataSize = sizeof(char);
            break;

        case INTD_SHORT:
        case INTD_USHORT:
            var_dataSize = sizeof(short);
            break;

        case INTD_INT:
        case INTD_UINT:
            var_dataSize = sizeof(int);
            break;

        case INTD_LONG:
        case INTD_ULONG:
            var_dataSize = sizeof(long);
            break;

        case INTD_FLOAT:
            var_dataSize = sizeof(float);
            break;

        case INTD_DOUBLE:
            var_dataSize = sizeof(double);
            break;

        default:
            break;
    }

    return var_dataSize;
}

/**
 * Get system time
 */
static char *getSystemTime()
{
    time_t timer;
    timer = time(0);
    return asctime(localtime(&timer));
}

/**
 * Save the record list and the save/restore request file
 */ 
static int INTD_func_genRecordList(const char *path, const char *fileName, const char *srFileName)
{
    char var_fullListFileName[256] = "";
    char var_fullSRFileName[256]   = "";

    char var_modName[128]    = "";
    char var_recName[128]    = "";                                   /* record name */
    char var_scanMethod[128] = "";
    char var_pno[128]        = "";
    char var_dataType[128]   = "";

    FILE *var_outFile       = NULL;
    FILE *var_outFile_sr    = NULL;

    INTD_struc_node *ptr_dataNode  = NULL;
    
    /* check the input */
    if(!fileName || !fileName[0] || !srFileName || !srFileName[0]) return -1;

    /* generate the full file names */
    if(path && path[0]) {
        strcpy(var_fullListFileName, path);
        strcpy(var_fullSRFileName,   path);
        strcat(var_fullListFileName, "/");
        strcat(var_fullSRFileName,   "/");
        strcat(var_fullListFileName, fileName);
        strcat(var_fullSRFileName,   srFileName);
    } else {
        strcpy(var_fullListFileName, fileName);
        strcpy(var_fullSRFileName,   srFileName);
    }

    /* open the file */
    var_outFile     = fopen(var_fullListFileName,   "w");
    var_outFile_sr  = fopen(var_fullSRFileName,     "w");

    if(!var_outFile || !var_outFile_sr) {
        return -1;
    }

    /* write to file */
    fprintf(var_outFile, "#-----------------------------------------------\n");
    fprintf(var_outFile, "# %s\n", fileName);
    fprintf(var_outFile, "# record name, record type, scan type, data type, num of point\n"); 
    fprintf(var_outFile, "#-----------------------------------------------\n");
    fprintf(var_outFile, "\n");

    fprintf(var_outFile_sr, "#-----------------------------------------------\n");
    fprintf(var_outFile_sr, "# %s\n", srFileName);
    fprintf(var_outFile_sr, "# autosave request file\n"); 
    fprintf(var_outFile_sr, "#-----------------------------------------------\n");
    fprintf(var_outFile_sr, "\n");

    for(ptr_dataNode = (INTD_struc_node *)ellFirst(&INTD_gvar_dataList);
        ptr_dataNode;
        ptr_dataNode = (INTD_struc_node *)ellNext(&ptr_dataNode -> node)) {
        
        /* get the necessary information */
        strncpy(var_modName,    ptr_dataNode -> moduleName, 128);                           /* get module name */
        strncpy(var_recName,    ptr_dataNode -> dataName, 128);                             /* record name */
        strncpy(var_scanMethod, INTD_gvar_scanStrs[(int)ptr_dataNode -> scanType], 128);    /* scan method */
        sprintf(var_pno, "%d", ptr_dataNode -> pno);                                        /* point number (only used for waveform) */
        
        switch(ptr_dataNode -> dataType) {                                                  /* field data type (only used for waveform) */
            case INTD_CHAR:     strcpy(var_dataType, "CHAR");   break;
            case INTD_UCHAR:    strcpy(var_dataType, "UCHAR");  break;
            case INTD_SHORT:    strcpy(var_dataType, "SHORT");  break;
            case INTD_USHORT:   strcpy(var_dataType, "USHORT"); break;
            case INTD_INT:      strcpy(var_dataType, "LONG");   break;
            case INTD_UINT:     strcpy(var_dataType, "ULONG");  break;
            case INTD_LONG:     strcpy(var_dataType, "LONG");   break;
            case INTD_ULONG:    strcpy(var_dataType, "ULONG");  break;
            case INTD_FLOAT:    strcpy(var_dataType, "FLOAT");  break;
            case INTD_DOUBLE:   strcpy(var_dataType, "DOUBLE"); break;
            default:            strcpy(var_dataType, "");       break;
        }
        
        /* generate the string for record */
        switch(ptr_dataNode -> recordType) {
            case INTD_AO:       fprintf(var_outFile, "%s:%s\tao\t%s\tDOUBLE\t1\n",    var_modName, var_recName, var_scanMethod); 
                                fprintf(var_outFile_sr, "$(name_space):%s:%s\n",      var_modName, var_recName);  
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.HIHI\n", var_modName, var_recName);  
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.HIGH\n", var_modName, var_recName);  
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.LOW\n",  var_modName, var_recName);  
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.LOLO\n", var_modName, var_recName); break;

            case INTD_AI:       fprintf(var_outFile, "%s:%s\tai\t%s\tDOUBLE\t1\n",    var_modName, var_recName, var_scanMethod);   break;

            case INTD_BO:       fprintf(var_outFile, "%s:%s\tbo\t%s\tUSHORT\t1\n",    var_modName, var_recName, var_scanMethod);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s\n",      var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.ZNAM\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.ONAM\n", var_modName, var_recName); break;

            case INTD_BI:       fprintf(var_outFile, "%s:%s\tbi\t%s\tUSHORT\t1\n",    var_modName, var_recName, var_scanMethod);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.ZNAM\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.ONAM\n", var_modName, var_recName); break;

            case INTD_LO:       fprintf(var_outFile, "%s:%s\tlongout\t%s\tLONG\t1\n", var_modName, var_recName, var_scanMethod); 
                                fprintf(var_outFile_sr, "$(name_space):%s:%s\n",      var_modName, var_recName);  
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.HIHI\n", var_modName, var_recName);  
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.HIGH\n", var_modName, var_recName);  
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.LOW\n",  var_modName, var_recName);  
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.LOLO\n", var_modName, var_recName); break;

            case INTD_LI:       fprintf(var_outFile, "%s:%s\tlongin\t%s\tLONG\t1\n",  var_modName, var_recName, var_scanMethod);   break;

            case INTD_MBBO:     fprintf(var_outFile, "%s:%s\tmbbo\t%s\tULONG\t1\n",   var_modName, var_recName, var_scanMethod);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s\n",      var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.ZRST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.ONST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.TWST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.THST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.FRST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.FVST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.SXST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.SVST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.EIST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.NIST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.TEST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.ELST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.TVST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.TTST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.FTST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.FFST\n", var_modName, var_recName); break;

            case INTD_MBBI:     fprintf(var_outFile, "%s:%s\tmbbi\t%s\tULONG\t1\n",   var_modName, var_recName, var_scanMethod);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.ZRST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.ONST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.TWST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.THST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.FRST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.FVST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.SXST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.SVST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.EIST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.NIST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.TEST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.ELST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.TVST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.TTST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.FTST\n", var_modName, var_recName);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s.FFST\n", var_modName, var_recName); break;

            case INTD_WFO:      fprintf(var_outFile, "%s:%s\twaveform(out)\t%s\t%s\t%s\n", var_modName, var_recName, var_scanMethod, var_dataType, var_pno);
                                fprintf(var_outFile_sr, "$(name_space):%s:%s\n",           var_modName, var_recName); break;

            case INTD_WFI:      fprintf(var_outFile, "%s:%s\twaveform(in)\t%s\t%s\t%s\n",  var_modName, var_recName, var_scanMethod, var_dataType, var_pno); break;
            default: break;
        }            

    }    

    /* close to file */
    fflush(var_outFile);
    fclose(var_outFile);

    fflush(var_outFile_sr);
    fclose(var_outFile_sr);
}

/*======================================
 * Public Routines
 *======================================*/
/**
 * Create a data node. This will be used in the user code to create a internal data node and connect with a data in the user code
 * Input:
 *   Please see the definition of the data structure of INTD_struc_node
 * Return:
 *   0      : Successful
 *  -1      : Failed
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
                            INTD_enum_scanType   scanType)
{
    INTD_struc_node *ptr_dataNode = NULL;

    /* Check the input */
    if(!moduleName || !moduleName[0]) {
        printf("INTD_API_createDataNode: Illegal module name\n");
        return -1;
    }

    if(!dataName || !dataName[0]) {
        printf("INTD_API_createDataNode: Illegal data name\n");
        return -1;
    }

    if(!dataPtr) {
        printf("INTD_API_createDataNode: Illegal data assigned\n");
        return -1;
    }
    
    if(pno <= 0) {
        printf("INTD_API_createDataNode: Number of points must be larger than 0\n");
        return -1;
    }

    /* Init the list if necessary */
    if(!INTD_gvar_dataListInitialized) {
        ellInit(&INTD_gvar_dataList);
        INTD_gvar_dataListInitialized = 1;
    }

    /* Create a new data node */
    ptr_dataNode = (INTD_struc_node *)calloc(1, sizeof(INTD_struc_node));

    if(!ptr_dataNode) {
        printf("INTD_API_createDataNode: Failed to create the data node of %s for the module of %s\n", dataName, moduleName);
        return -1;
    }

    /* Initalize the data node */
    strcpy(ptr_dataNode -> moduleName, moduleName);
    strcpy(ptr_dataNode -> dataName,   dataName);
    
    ptr_dataNode -> dataPtr         = dataPtr;
    ptr_dataNode -> privateData     = privateData;
    ptr_dataNode -> pno             = pno;
    ptr_dataNode -> ioIntScan       = ioIntScan;
    ptr_dataNode -> dataType        = dataType;
    ptr_dataNode -> readCallback    = readCallback;
    ptr_dataNode -> writeCallback   = writeCallback;
    ptr_dataNode -> epicsRecord     = NULL;
    ptr_dataNode -> mutexId         = mutexId;
    ptr_dataNode -> eventId         = eventId;
    ptr_dataNode -> recordType      = recordType;
    ptr_dataNode -> scanType        = scanType;

    /* Add the data node to the list */
    ellAdd(&INTD_gvar_dataList, &ptr_dataNode -> node);

    return 0;
}

/**
 * Find a data node based on the module name and the data name. This will be called by the record_init function and the
 *   pointer of the record will be also remembered here
 * Input
 *   moduleName     : Module name
 *   dataName       : Data name
 *   epicsRecord    : The pointer to the EPICS recored that is attached to this data
 * Return:
 *   NULL           : Not found
 *   >0             : Address of the data node
 */
INTD_struc_node *INTD_API_findDataNode(const char *moduleName, const char *dataName, dbCommon *epicsRecord)
{
    int              var_nodeFound = 0;
    INTD_struc_node *ptr_dataNode  = NULL;

    /* check the input */
    if(!moduleName || !moduleName[0]) {
        printf("INTD_API_findDataNode: Illegal module name\n");
        return NULL;
    }

    if(!dataName || !dataName[0]) {
        printf("INTD_API_findDataNode: Illegal data name\n");
        return NULL;
    }

    /* find the data node */
    for(ptr_dataNode = (INTD_struc_node *)ellFirst(&INTD_gvar_dataList);
        ptr_dataNode;
        ptr_dataNode = (INTD_struc_node *)ellNext(&ptr_dataNode -> node)) {
        
        if(strcmp(ptr_dataNode -> moduleName, moduleName) == 0 && strcmp(ptr_dataNode -> dataName, dataName) == 0) {
            var_nodeFound = 1;
            break;
        }

    }

    if(var_nodeFound) {
        ptr_dataNode -> epicsRecord = epicsRecord;

        return ptr_dataNode;
    } else {
        return NULL;
    }
}

/**
 * Get data from the data node. This will be called by the record processing routine for reading data
 * Input:
 *   dataNode       : Data node to get data from
 *   pno            : Number of points to get from the internal data node
 * Output:
 *   data           : The data read out from the node, the buffer should be already defined in the caller
 * Return:
 *   0              : Successful
 *  -1              : Failed
 */
int INTD_API_getData(INTD_struc_node *dataNode, unsigned int pno, void *data)
{
    int var_dataSize;

    /* Check the input */
    if(!dataNode || !data || !dataNode -> dataPtr || pno <= 0) return -1;

    /* Lock the mutex if it is specified */
    if(dataNode -> mutexId) epicsMutexMustLock(dataNode -> mutexId);

    /* Execute the call back if defined */
    if(dataNode -> readCallback && INTD_gvar_enaCallback) (*dataNode -> readCallback)((void *)dataNode);

    /* Get the data based on the size */
    if(pno > 1) {                                                   /* array */

        var_dataSize = INTD_func_getDataSize(dataNode -> dataType);
        memcpy(data, dataNode -> dataPtr, pno * var_dataSize);

    } else if(pno == 1) {                                           /* single value (these code to remove the overhead from memcpy) */

        switch(dataNode -> dataType) {
            case INTD_CHAR:     *((char *)data)             = *((char *)dataNode            -> dataPtr); break;
            case INTD_UCHAR:    *((unsigned char *)data)    = *((unsigned char *)dataNode   -> dataPtr); break;
            case INTD_SHORT:    *((short *)data)            = *((short *)dataNode           -> dataPtr); break;
            case INTD_USHORT:   *((unsigned short *)data)   = *((unsigned short *)dataNode  -> dataPtr); break;
            case INTD_INT:      *((int *)data)              = *((int *)dataNode             -> dataPtr); break;
            case INTD_UINT:     *((unsigned int *)data)     = *((unsigned int *)dataNode    -> dataPtr); break;
            case INTD_LONG:     *((long *)data)             = *((long *)dataNode            -> dataPtr); break;
            case INTD_ULONG:    *((unsigned long *)data)    = *((unsigned long *)dataNode   -> dataPtr); break;
            case INTD_FLOAT:    *((float *)data)            = *((float *)dataNode           -> dataPtr); break;
            case INTD_DOUBLE:   *((double *)data)           = *((double *)dataNode          -> dataPtr); break;
            default: break;
        }

    }

    /* Unlock the mutex if it is specified */
    if(dataNode -> mutexId) epicsMutexUnlock(dataNode -> mutexId);

    /* Signal the event if it is specified */
    if(dataNode -> eventId) epicsEventSignal(dataNode -> eventId);

    return 0;
}

/**
 * Put data from the data node. This will be called by the record processing routine for writing data
 * Input:
 *   dataNode       : Data node to get data from
 *   pno            : Number of points to get from the internal data node
 *   data           : The data write to the node, the buffer should be already defined in the caller
 * Return:
 *   0              : Successful
 *  -1              : Failed
 */
int INTD_API_putData(INTD_struc_node *dataNode, unsigned int pno, void *data)
{
    int var_dataSize;

    /* Check the input */
    if(!dataNode || !data || !dataNode -> dataPtr || pno <= 0) return -1;

    /* Lock the mutex if it is specified */
    if(dataNode -> mutexId) epicsMutexMustLock(dataNode -> mutexId);

    /* Put the data based on the size */
    if(pno > 1) {                                                   /* array */

        var_dataSize = INTD_func_getDataSize(dataNode -> dataType);
        memcpy(dataNode -> dataPtr, data, pno * var_dataSize);

    } else if(pno == 1) {                                           /* single value (these code to remove the overhead from memcpy) */

        switch(dataNode -> dataType) {
            case INTD_CHAR:     *((char *)dataNode            -> dataPtr) = *((char *)data);              break;
            case INTD_UCHAR:    *((unsigned char *)dataNode   -> dataPtr) = *((unsigned char *)data);     break;
            case INTD_SHORT:    *((short *)dataNode           -> dataPtr) = *((short *)data);             break;
            case INTD_USHORT:   *((unsigned short *)dataNode  -> dataPtr) = *((unsigned short *)data);    break;
            case INTD_INT:      *((int *)dataNode             -> dataPtr) = *((int *)data);               break;
            case INTD_UINT:     *((unsigned int *)dataNode    -> dataPtr) = *((unsigned int *)data);      break;
            case INTD_LONG:     *((long *)dataNode            -> dataPtr) = *((long *)data);              break;
            case INTD_ULONG:    *((unsigned long *)dataNode   -> dataPtr) = *((unsigned long *)data);     break;
            case INTD_FLOAT:    *((float *)dataNode           -> dataPtr) = *((float *)data);             break;
            case INTD_DOUBLE:   *((double *)dataNode          -> dataPtr) = *((double *)data);            break;
            default: break;
        }

    }

    /* Execute the call back if defined */    
    if(dataNode -> writeCallback && INTD_gvar_enaCallback) (*dataNode -> writeCallback)((void *)dataNode);

    /* Unlock the mutex if it is specified */
    if(dataNode -> mutexId) epicsMutexUnlock(dataNode -> mutexId);

    /* Signal the event if it is specified */
    if(dataNode -> eventId) epicsEventSignal(dataNode -> eventId);

    return 0;
}

/**
 * Generate records for the internal data
 * Input:
 *   path:          the path for the file
 *   dbFileName:    full file name (including path) of the output db file
 * Return:
 *   0              : Successful
 *  -1              : Failed
 */
int INTD_API_genRecord(const char *path, const char *dbFileName)
{
    char var_fullFileName[256] = "";
    char var_recStr[512]       = "";                                   /* final string of the record */

    char var_modName[128]      = "";
    char var_recName[128]      = "";                                   /* record name */
    char var_scanMethod[128]   = "";
    char var_pno[128]          = "";
    char var_dataType[128]     = "";

    char *timeStr;

    FILE *var_outFile = NULL;

    INTD_struc_node *ptr_dataNode  = NULL;
    
    /* check the input */
    if(!dbFileName || !dbFileName[0]) return -1;

    /* generate the full file name and open the file */
    if(path && path[0]) {
        strcpy(var_fullFileName, path);
        strcat(var_fullFileName, "/");
        strcat(var_fullFileName, dbFileName);
    } else {
        strcpy(var_fullFileName, dbFileName);
    }

    var_outFile = fopen(var_fullFileName, "w");

    if(!var_outFile) {
        printf("INTD_API_genRecord: Failed to create file of %s\n", var_fullFileName);
        return -1;
    }

    /* write to file */
    timeStr = getSystemTime();

    fprintf(var_outFile, "#-----------------------------------------------\n");
    fprintf(var_outFile, "# %s\n", dbFileName);
    fprintf(var_outFile, "# EPICS database file\n");
    fprintf(var_outFile, "# Auto generated by InternalData module! Do not modify...\n");
    fprintf(var_outFile, "# Generated on %s", timeStr);
    fprintf(var_outFile, "#-----------------------------------------------\n");
    fprintf(var_outFile, "\n");

    for(ptr_dataNode = (INTD_struc_node *)ellFirst(&INTD_gvar_dataList);
        ptr_dataNode;
        ptr_dataNode = (INTD_struc_node *)ellNext(&ptr_dataNode -> node)) {
        
        /* get the necessary information */
        strncpy(var_modName,    ptr_dataNode -> moduleName, 128);                           /* get module name */
        strncpy(var_recName,    ptr_dataNode -> dataName, 128);                             /* record name */
        strncpy(var_scanMethod, INTD_gvar_scanStrs[(int)ptr_dataNode -> scanType], 128);    /* scan method */
        sprintf(var_pno, "%d", ptr_dataNode -> pno);                                        /* point number (only used for waveform) */
        
        switch(ptr_dataNode -> dataType) {                                                  /* field data type (only used for waveform) */
            case INTD_CHAR:     strcpy(var_dataType, "CHAR");   break;
            case INTD_UCHAR:    strcpy(var_dataType, "UCHAR");  break;
            case INTD_SHORT:    strcpy(var_dataType, "SHORT");  break;
            case INTD_USHORT:   strcpy(var_dataType, "USHORT"); break;
            case INTD_INT:      strcpy(var_dataType, "LONG");   break;
            case INTD_UINT:     strcpy(var_dataType, "ULONG");  break;
            case INTD_LONG:     strcpy(var_dataType, "LONG");   break;
            case INTD_ULONG:    strcpy(var_dataType, "ULONG");  break;
            case INTD_FLOAT:    strcpy(var_dataType, "FLOAT");  break;
            case INTD_DOUBLE:   strcpy(var_dataType, "DOUBLE"); break;
            default:            strcpy(var_dataType, "");       break;
        }
        
        /* generate the string for record */
        switch(ptr_dataNode -> recordType) {
            case INTD_AO:       INTD_RECORD_AO(var_modName, var_recName, var_scanMethod, var_recStr);    break;
            case INTD_AI:       INTD_RECORD_AI(var_modName, var_recName, var_scanMethod, var_recStr);    break;
            case INTD_BO:       INTD_RECORD_BO(var_modName, var_recName, var_scanMethod, var_recStr);    break;
            case INTD_BI:       INTD_RECORD_BI(var_modName, var_recName, var_scanMethod, var_recStr);    break;
            case INTD_LO:       INTD_RECORD_LO(var_modName, var_recName, var_scanMethod, var_recStr);    break;
            case INTD_LI:       INTD_RECORD_LI(var_modName, var_recName, var_scanMethod, var_recStr);    break;
            case INTD_MBBO:     INTD_RECORD_MBBO(var_modName, var_recName, var_scanMethod, var_recStr);  break;
            case INTD_MBBI:     INTD_RECORD_MBBI(var_modName, var_recName, var_scanMethod, var_recStr);  break;
            case INTD_WFO:      INTD_RECORD_WFO(var_modName, var_recName, var_scanMethod, var_pno, var_dataType, var_recStr); break;
            case INTD_WFI:      INTD_RECORD_WFI(var_modName, var_recName, var_scanMethod, var_pno, var_dataType, var_recStr); break;
            default: strcpy(var_recStr, "");
        }
            
        /* save to file */
        fprintf(var_outFile, "%s\n\n", var_recStr);

    }    

    /* close to file */
    fflush(var_outFile);
    fclose(var_outFile);

    /* record list and autosave list */
    INTD_func_genRecordList(path, "recordList.txt", "info_settings.req");

    return 0;
}

/**
 * Synchronize the internal data with the records
 * senario 1: put the record data into the internal data and read the internal data to the record, without executing the call back functions
 * senario 2: put the record data into the internal data and read the internal data to the record, wit executing the call back functions
 */
int INTD_API_syncWithRecords(int enaCallback)
{
    INTD_struc_node *ptr_dataNode  = NULL;

    /* set the flag for call back function */
    INTD_gvar_enaCallback = enaCallback;

    /* process the records */
    for(ptr_dataNode = (INTD_struc_node *)ellFirst(&INTD_gvar_dataList);
        ptr_dataNode;
        ptr_dataNode = (INTD_struc_node *)ellNext(&ptr_dataNode -> node)) {
        
        if(ptr_dataNode -> epicsRecord)
            dbProcess(ptr_dataNode -> epicsRecord);

    }    

    /* recover the call back flag */
    INTD_gvar_enaCallback = 1;
}







