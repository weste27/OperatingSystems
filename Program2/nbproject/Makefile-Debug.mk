#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/MemoryAllocator.o \
	${OBJECTDIR}/PageTableManager.o \
	${OBJECTDIR}/Process.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../OperatingSystems/MemorySubsystemS2018/dist/Debug/GNU-Linux/libmemorysubsystems2018.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/program2

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/program2: ../OperatingSystems/MemorySubsystemS2018/dist/Debug/GNU-Linux/libmemorysubsystems2018.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/program2: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/program2 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/MemoryAllocator.o: MemoryAllocator.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../OperatingSystems/MemorySubsystemS2018 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MemoryAllocator.o MemoryAllocator.cpp

${OBJECTDIR}/PageTableManager.o: PageTableManager.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../OperatingSystems/MemorySubsystemS2018 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PageTableManager.o PageTableManager.cpp

${OBJECTDIR}/Process.o: Process.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../OperatingSystems/MemorySubsystemS2018 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Process.o Process.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../OperatingSystems/MemorySubsystemS2018 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../OperatingSystems/MemorySubsystemS2018 && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
	cd ../OperatingSystems/MemorySubsystemS2018 && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
