/****************************************************
 * recordGenerate.c
 * 
 * Source file for generating record from the internal data node
 *
 * Created by: Zheqiao Geng, gengzq@slac.stanford.edu
 * Created on: 2011.06.30
 * Description: Initial creation
 ****************************************************/
#include "recordGenerate.h"

void INTD_RECORD_AO(const char *moduleName, const char *recName, const char *scanMethod, char *outStr)
{
    if(!moduleName || !recName || !scanMethod || !outStr) return;

    strcpy(outStr, "record(ao, $(name_space):");
    strcat(outStr, moduleName);
    strcat(outStr, ":");
    strcat(outStr, recName);
    strcat(outStr, ") {\n        field(SCAN, \"");
    strcat(outStr, scanMethod);
    strcat(outStr, "\")\n        field(DTYP, \"InternalData\")\n        field(OUT, \"@");
    strcat(outStr, moduleName);
    strcat(outStr, ".");
    strcat(outStr, recName);
    strcat(outStr, "\")\n}");     
}

void INTD_RECORD_AI(const char *moduleName, const char *recName, const char *scanMethod, char *outStr)
{
    if(!moduleName || !recName || !scanMethod || !outStr) return;

    strcpy(outStr, "record(ai, $(name_space):");
    strcat(outStr, moduleName);
    strcat(outStr, ":");
    strcat(outStr, recName);
    strcat(outStr, ") {\n        field(SCAN, \"");
    strcat(outStr, scanMethod);
    strcat(outStr, "\")\n        field(DTYP, \"InternalData\")\n        field(INP, \"@");
    strcat(outStr, moduleName);
    strcat(outStr, ".");    
    strcat(outStr, recName);
    strcat(outStr, "\")\n}");     
}

void INTD_RECORD_BO(const char *moduleName, const char *recName, const char *scanMethod, char *outStr)
{
    if(!moduleName || !recName || !scanMethod || !outStr) return;

    strcpy(outStr, "record(bo, $(name_space):");
    strcat(outStr, moduleName);
    strcat(outStr, ":");
    strcat(outStr, recName);
    strcat(outStr, ") {\n        field(SCAN, \"");
    strcat(outStr, scanMethod);
    strcat(outStr, "\")\n        field(DTYP, \"InternalData\")\n        field(OUT, \"@");
    strcat(outStr, moduleName);
    strcat(outStr, ".");
    strcat(outStr, recName);
    strcat(outStr, "\")\n}");     
}

void INTD_RECORD_BI(const char *moduleName, const char *recName, const char *scanMethod, char *outStr)
{
    if(!moduleName || !recName || !scanMethod || !outStr) return;

    strcpy(outStr, "record(bi, $(name_space):");
    strcat(outStr, moduleName);
    strcat(outStr, ":");
    strcat(outStr, recName);
    strcat(outStr, ") {\n        field(SCAN, \"");
    strcat(outStr, scanMethod);
    strcat(outStr, "\")\n        field(DTYP, \"InternalData\")\n        field(INP, \"@");
    strcat(outStr, moduleName);
    strcat(outStr, ".");
    strcat(outStr, recName);
    strcat(outStr, "\")\n}");     
}

void INTD_RECORD_LO(const char *moduleName, const char *recName, const char *scanMethod, char *outStr)
{
    if(!moduleName || !recName || !scanMethod || !outStr) return;

    strcpy(outStr, "record(longout, $(name_space):");
    strcat(outStr, moduleName);
    strcat(outStr, ":");
    strcat(outStr, recName);
    strcat(outStr, ") {\n        field(SCAN, \"");
    strcat(outStr, scanMethod);
    strcat(outStr, "\")\n        field(DTYP, \"InternalData\")\n        field(OUT, \"@");
    strcat(outStr, moduleName);
    strcat(outStr, ".");
    strcat(outStr, recName);
    strcat(outStr, "\")\n}");     
}

void INTD_RECORD_LI(const char *moduleName, const char *recName, const char *scanMethod, char *outStr)
{
    if(!moduleName || !recName || !scanMethod || !outStr) return;

    strcpy(outStr, "record(longin, $(name_space):");
    strcat(outStr, moduleName);
    strcat(outStr, ":");
    strcat(outStr, recName);
    strcat(outStr, ") {\n        field(SCAN, \"");
    strcat(outStr, scanMethod);
    strcat(outStr, "\")\n        field(DTYP, \"InternalData\")\n        field(INP, \"@");
    strcat(outStr, moduleName);
    strcat(outStr, ".");
    strcat(outStr, recName);
    strcat(outStr, "\")\n}");     
}

void INTD_RECORD_MBBO(const char *moduleName, const char *recName, const char *scanMethod, char *outStr)
{
    if(!moduleName || !recName || !scanMethod || !outStr) return;

    strcpy(outStr, "record(mbbo, $(name_space):");
    strcat(outStr, moduleName);
    strcat(outStr, ":");
    strcat(outStr, recName);
    strcat(outStr, ") {\n        field(SCAN, \"");
    strcat(outStr, scanMethod);
    strcat(outStr, "\")\n        field(DTYP, \"InternalData\")\n        field(OUT, \"@");
    strcat(outStr, moduleName);
    strcat(outStr, ".");
    strcat(outStr, recName);
    strcat(outStr, "\")\n}");     
}

void INTD_RECORD_MBBI(const char *moduleName, const char *recName, const char *scanMethod, char *outStr)
{
    if(!moduleName || !recName || !scanMethod || !outStr) return;

    strcpy(outStr, "record(mbbi, $(name_space):");
    strcat(outStr, moduleName);
    strcat(outStr, ":");
    strcat(outStr, recName);
    strcat(outStr, ") {\n        field(SCAN, \"");
    strcat(outStr, scanMethod);
    strcat(outStr, "\")\n        field(DTYP, \"InternalData\")\n        field(INP, \"@");
    strcat(outStr, moduleName);
    strcat(outStr, ".");
    strcat(outStr, recName);
    strcat(outStr, "\")\n}");     
}

void INTD_RECORD_WFO(const char *moduleName, const char *recName, const char *scanMethod, const char *pno, const char *dataType, char *outStr)
{
    if(!moduleName || !recName || !scanMethod || !pno || !dataType || !outStr) return;

    strcpy((outStr), "record(waveform, $(name_space):");
    strcat(outStr, moduleName);
    strcat(outStr, ":");
    strcat((outStr), recName);
    strcat((outStr), ") {\n        field(DESC, \"[W]\")\n        field(SCAN, \"");
    strcat((outStr), scanMethod);
    strcat((outStr), "\")\n        field(NELM, \"");
    strcat((outStr), pno);
    strcat((outStr), "\")\n        field(DTYP, \"InternalData\")\n        field(FTVL, \"");
    strcat((outStr), dataType);
    strcat((outStr), "\")\n        field(INP, \"@");
    strcat(outStr, moduleName);
    strcat(outStr, ".");
    strcat((outStr), recName);
    strcat((outStr), "\")\n}");    
}

void INTD_RECORD_WFI(const char *moduleName, const char *recName, const char *scanMethod, const char *pno, const char *dataType, char *outStr)
{
    if(!moduleName || !recName || !scanMethod || !pno || !dataType || !outStr) return;

    strcpy((outStr), "record(waveform, $(name_space):");
    strcat(outStr, moduleName);
    strcat(outStr, ":");
    strcat((outStr), recName);
    strcat((outStr), ") {\n        field(SCAN, \"");
    strcat((outStr), scanMethod);
    strcat((outStr), "\")\n        field(NELM, \"");
    strcat((outStr), pno);
    strcat((outStr), "\")\n        field(DTYP, \"InternalData\")\n        field(FTVL, \"");
    strcat((outStr), dataType);
    strcat((outStr), "\")\n        field(INP, \"@");
    strcat(outStr, moduleName);
    strcat(outStr, ".");
    strcat((outStr), recName);
    strcat((outStr), "\")\n}");    
}



