##########################################################
# Makefile
# 
# Makefile for the InternalData module
# 
# Created by: Zheqiao Geng, gengzq@slac.stanford.edu
# Created on: 2011.05.17
# Description: Initial creation 
##########################################################
TOP=../..

include $(TOP)/configure/CONFIG

#------------------------------------------------
# build the library for InternalData
#------------------------------------------------
# ---- library name ----
LIBRARY_IOC = InternalData

# ---- ported include files ----
INC += InternalData.h
INC += recordGenerate.h

# ---- library database definition files (including record type definitions and all registerations) ----
DBD += InternalData.dbd
InternalData_DBD += base.dbd
InternalData_DBD += InternalData_dev.dbd
InternalData_DBD += InternalData_iocShell.dbd

# ---- private source files ----
InternalData_SRCS += InternalData.c
InternalData_SRCS += recordGenerate.c
InternalData_SRCS += InternalData_iocShell.c
InternalData_SRCS += InternalData_devAo.c
InternalData_SRCS += InternalData_devBo.c
InternalData_SRCS += InternalData_devLo.c
InternalData_SRCS += InternalData_devWf.c
InternalData_SRCS += InternalData_devAi.c
InternalData_SRCS += InternalData_devBi.c
InternalData_SRCS += InternalData_devLi.c
InternalData_SRCS += InternalData_devMbbi.c
InternalData_SRCS += InternalData_devMbbo.c

# ---- finally link to the EPICS Base libraries ----
InternalData_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES

