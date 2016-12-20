CMTPATH=/home/evrazhka/ND280/nd280rep/v11r31
CMT_tag=$(tag)
CMTROOT=/home/evrazhka/ND280/CMT/v1r18p20061003
CMT_root=/home/evrazhka/ND280/CMT/v1r18p20061003
CMTVERSION=v1r18p20061003
cmt_hardware_query_command=uname -m
cmt_hardware=`$(cmt_hardware_query_command)`
cmt_system_version_query_command=${CMTROOT}/mgr/cmt_linux_version.sh | ${CMTROOT}/mgr/cmt_filter_version.sh
cmt_system_version=`$(cmt_system_version_query_command)`
cmt_compiler_version_query_command=${CMTROOT}/mgr/cmt_gcc_version.sh | ${CMTROOT}/mgr/cmt_filter3_version.sh
cmt_compiler_version=`$(cmt_compiler_version_query_command)`
PATH=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/numuCCMultiPiAnalysis/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/ndupAnalysis/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/baseAnalysis/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandTools/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheSelections/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandCorrections/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandUtils/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandCore/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandIO/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/ndupEvent/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandEventModel/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheEventModel/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheNDUPUtils/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheUtils/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheCore/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/ROOT/v5r34p09n04/amd64_linux26/bin:/home/evrazhka/ND280/nd280rep/v11r31/GSL/v1r15p0n00/amd64_linux26/bin:/home/evrazhka/ND280/nd280rep/v11r31/MYSQL/v5r051an01/amd64_linux26/bin:/home/evrazhka/ND280/nd280rep/v11r31/EXTERN/v3r7/scripts:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psychePolicy/v0r0/scripts:/home/evrazhka/ND280/CMT/v1r18p20061003/${CMTBIN}:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/wgmc/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/wgMagnetCalib/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/wgRuntimeParameters/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/wgEvent/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/testBase/v1r15/scripts:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/testBase/v1r15/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/wgPolicy/v0r0/scripts:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/GEANT/v9r4p04n00/:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/CLHEP/v2r1p1n00/amd64_linux26/bin:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/EXTERN/v3r7/scripts:/usr/lib64/qt-3.3/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/wgEvent/v0r0/scripts:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/ndupEvent/v0r0/scripts
CLASSPATH=/home/evrazhka/ND280/CMT/v1r18p20061003/java
debug_option=-g
cc=gcc
ccomp=$(cc) -c $(includes) $(cdebugflags) $(cflags) $(pp_cflags)
clink=$(cc) $(clinkflags) $(cdebugflags)
ppcmd=-I
preproc=c++ -MD -c 
cpp=g++
cppflags=-g -pipe -O -Wall -ansi -Wctor-dtor-privacy -Wnon-virtual-dtor -fpic -Woverloaded-virtual 
pp_cppflags=-D_GNU_SOURCE
cppcomp=$(cpp) -c $(includes) $(cppdebugflags) $(cppflags) $(pp_cppflags)
cpplinkflags=-Wl,--no-as-needed
cpplink=$(cpp) $(cpplinkflags) $(cppdebugflags)
for=g77
fflags=$(debug_option)
fcomp=$(for) -c $(fincludes) $(fflags) $(pp_fflags)
flink=$(for) $(flinkflags)
javacomp=javac -classpath $(src):$(CLASSPATH) 
javacopy=cp
jar=jar
X11_cflags=-I/usr/include
Xm_cflags=-I/usr/include
X_linkopts=-L/usr/X11R6/lib -lXm -lXt -lXext -lX11 -lm
lex=lex $(lexflags)
yaccflags= -l -d 
yacc=yacc $(yaccflags)
ar=ar r
ranlib=ranlib
make_shlib=${CMTROOT}/mgr/cmt_make_shlib_common.sh extract
shlibsuffix=so
shlibbuilder=g++ $(cmt_installarea_linkopts) 
shlibflags=-shared
symlink=/bin/ln -fs 
symunlink=/bin/rm -f 
library_install_command=${symlink}
build_library_links=$(cmtexe) build library_links -quiet -tag=$(tags)
remove_library_links=$(cmtexe) remove library_links -quiet -tag=$(tags)
cmtexe=${CMTROOT}/${CMTBIN}/cmt.exe
build_prototype=$(cmtexe) build prototype
build_dependencies=$(cmtexe) -quiet -tag=$(tags) build dependencies
build_triggers=$(cmtexe) build triggers
implied_library_prefix=-l
SHELL=/bin/sh
q="
src=../src/
doc=../doc/
inc=../src/
mgr=../cmt/
application_suffix=.exe
library_prefix=lib
lock_command=chmod -R a-w ../*
unlock_command=chmod -R g+w ../*
MAKEFLAGS= --no-print-directory 
gmake_hosts=lx1 rsplus lxtest as7 dxplus ax7 hp2 aleph hp1 hpplus papou1-fe atlas
make_hosts=virgo-control1 rio0a vmpc38a
everywhere=hosts
install_command=cp 
uninstall_command=/bin/rm -f 
cmt_installarea_command=ln -fs 
cmt_uninstallarea_command=/bin/rm -f 
cmt_install_area_command=$(cmt_installarea_command)
cmt_uninstall_area_command=$(cmt_uninstallarea_command)
cmt_install_action=$(CMTROOT)/mgr/cmt_install_action.sh
cmt_installdir_action=$(CMTROOT)/mgr/cmt_installdir_action.sh
cmt_uninstall_action=$(CMTROOT)/mgr/cmt_uninstall_action.sh
cmt_uninstalldir_action=$(CMTROOT)/mgr/cmt_uninstalldir_action.sh
mkdir=mkdir
cmt_cvs_protocol_level=v1r1
cmt_installarea_prefix=InstallArea
CMT_PATH_remove_regexp=/[^/]*/
CMT_PATH_remove_share_regexp=/share/
NEWCMTCONFIG=x86_64-slc65-gcc447
numuCCMultiPiAnalysis_tag=$(tag)
NUMUCCMULTIPIANALYSISROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/numuCCMultiPiAnalysis/v0r0
numuCCMultiPiAnalysis_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/numuCCMultiPiAnalysis/v0r0
NUMUCCMULTIPIANALYSISVERSION=v0r0
numuCCMultiPiAnalysis_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
numuCCMultiPiAnalysis_offset=highlandUP
numuCCMultiPiAnalysis_project=nd280rep
numuCCMultiPiAnalysis_project_release=v11r31
ndupAnalysis_tag=$(tag)
NDUPANALYSISROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/ndupAnalysis/v0r0
ndupAnalysis_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/ndupAnalysis/v0r0
NDUPANALYSISVERSION=v0r0
ndupAnalysis_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
ndupAnalysis_offset=highlandUP
ndupAnalysis_project=nd280rep
ndupAnalysis_project_release=v11r31
baseAnalysis_tag=$(tag)
BASEANALYSISROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/baseAnalysis/v0r0
baseAnalysis_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/baseAnalysis/v0r0
BASEANALYSISVERSION=v0r0
baseAnalysis_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
baseAnalysis_offset=highlandUP
baseAnalysis_project=nd280rep
baseAnalysis_project_release=v11r31
highlandIO_tag=$(tag)
HIGHLANDIOROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandIO/v0r0
highlandIO_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandIO/v0r0
HIGHLANDIOVERSION=v0r0
highlandIO_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
highlandIO_offset=highlandUP
highlandIO_project=nd280rep
highlandIO_project_release=v11r31
psycheCore_tag=$(tag)
PSYCHECOREROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheCore/v0r0
psycheCore_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheCore/v0r0
PSYCHECOREVERSION=v0r0
psycheCore_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
psycheCore_offset=highlandUP
psycheCore_project=nd280rep
psycheCore_project_release=v11r31
psychePolicy_tag=$(tag)
PSYCHEPOLICYROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psychePolicy/v0r0
psychePolicy_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psychePolicy/v0r0
PSYCHEPOLICYVERSION=v0r0
psychePolicy_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
psychePolicy_offset=highlandUP
psychePolicy_project=nd280rep
psychePolicy_project_release=v11r31
ROOT_tag=$(tag)
ROOTROOT=/home/evrazhka/ND280/nd280rep/v11r31/ROOT/v5r34p09n04
ROOT_root=/home/evrazhka/ND280/nd280rep/v11r31/ROOT/v5r34p09n04
ROOTVERSION=v5r34p09n04
ROOT_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
ROOT_project=nd280rep
ROOT_project_release=v11r31
EXTERN_tag=$(tag)
EXTERNROOT=/home/evrazhka/ND280/nd280rep/v11r31/EXTERN/v3r7
EXTERN_root=/home/evrazhka/ND280/nd280rep/v11r31/EXTERN/v3r7
EXTERNVERSION=v3r7
EXTERN_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
EXTERN_project=nd280rep
EXTERN_project_release=v11r31
MYSQL_tag=$(tag)
MYSQLROOT=/home/evrazhka/ND280/nd280rep/v11r31/MYSQL/v5r051an01
MYSQL_root=/home/evrazhka/ND280/nd280rep/v11r31/MYSQL/v5r051an01
MYSQLVERSION=v5r051an01
MYSQL_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
MYSQL_project=nd280rep
MYSQL_project_release=v11r31
LD_LIBRARY_PATH=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/numuCCMultiPiAnalysis/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/ndupAnalysis/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/baseAnalysis/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandTools/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheSelections/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandCorrections/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandUtils/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandCore/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandIO/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/ndupEvent/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandEventModel/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheEventModel/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheNDUPUtils/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheUtils/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheCore/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/ROOT/v5r34p09n04/amd64_linux26/lib/root:/home/evrazhka/ND280/nd280rep/v11r31/GSL/v1r15p0n00/amd64_linux26/lib:/home/evrazhka/ND280/nd280rep/v11r31/MYSQL/v5r051an01/amd64_linux26/lib/mysql:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/wgmc/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/wgMagnetCalib/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/wgRuntimeParameters/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/wgEvent/v0r0/amd64_linux26:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/GEANT/v9r4p04n00/amd64_linux26/Linux-g++:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/GEANT/v9r4p04n00/amd64_linux26/plists/Linux-g++:/home/evrazhka/ND280/nd280rep/v11r31/newSoft/CLHEP/v2r1p1n00/amd64_linux26/lib
MYSQL_linkopts=`$(MYSQLROOT)/$(MYSQL_tag)/bin/mysql_config  --libs`
MYSQL_BASE_DIR=/home/evrazhka/ND280/nd280rep/v11r31/MYSQL/v5r051an01/amd64_linux26
MYSQL_LIB_DIR=${MYSQL_BASE_DIR}/lib/mysql
MYSQL_INCLUDE_DIR=${MYSQL_BASE_DIR}/include/mysql
MAKFLAGS=--no-print-directory
GSL_tag=$(tag)
GSLROOT=/home/evrazhka/ND280/nd280rep/v11r31/GSL/v1r15p0n00
GSL_root=/home/evrazhka/ND280/nd280rep/v11r31/GSL/v1r15p0n00
GSLVERSION=v1r15p0n00
GSL_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
GSL_project=nd280rep
GSL_project_release=v11r31
GSL_cppflags=`$(GSLROOT)/$(GSL_tag)/bin/gsl-config --cflags`
GSL_linkopts=`$(GSLROOT)/$(GSL_tag)/bin/gsl-config --libs`
GSL_BASE_DIR=/home/evrazhka/ND280/nd280rep/v11r31/GSL/v1r15p0n00/amd64_linux26
GSL_LIB_DIR=${GSL_BASE_DIR}/lib
GSL_INCLUDE_DIR=${GSL_BASE_DIR}/include
ROOT_PATH=$(ROOTROOT)/$(tag)
ROOT_BIN=$(ROOT_PATH)/bin
ROOT_LIB=$(ROOT_PATH)/lib/root
ROOTSYS=/home/evrazhka/ND280/nd280rep/v11r31/ROOT/v5r34p09n04/amd64_linux26
PYTHONPATH=${ROOTSYS}/lib/root
ROOT_linkopts=`$(ROOT_BIN)/root-config --glibs` -lEG -lGeom -lMinuit -lFumili -lXMLIO
ROOT_cppflags=-DROOT_USED `$(ROOT_BIN)/root-config --cflags` -Wno-long-long
ROOT_stamps=$(ROOTROOT)/$(ROOTCONFIG)/ROOT.stamp
dict=../dict/
rootcint=$(ROOT_BIN)/rootcint
psycheCore_cppflags=-DPSYCHECORE_USED -fPIC
psycheCore_linkopts=-L$(PSYCHECOREROOT)/$(psycheCore_tag) -lpsycheCore
psycheCore_stamps= $(PSYCHECOREROOT)/$(psycheCore_tag)/psycheCore.stamp
highlandEventModel_tag=$(tag)
HIGHLANDEVENTMODELROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandEventModel/v0r0
highlandEventModel_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandEventModel/v0r0
HIGHLANDEVENTMODELVERSION=v0r0
highlandEventModel_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
highlandEventModel_offset=highlandUP
highlandEventModel_project=nd280rep
highlandEventModel_project_release=v11r31
psycheEventModel_tag=$(tag)
PSYCHEEVENTMODELROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheEventModel/v0r0
psycheEventModel_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheEventModel/v0r0
PSYCHEEVENTMODELVERSION=v0r0
psycheEventModel_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
psycheEventModel_offset=highlandUP
psycheEventModel_project=nd280rep
psycheEventModel_project_release=v11r31
psycheNDUPUtils_tag=$(tag)
PSYCHENDUPUTILSROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheNDUPUtils/v0r0
psycheNDUPUtils_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheNDUPUtils/v0r0
PSYCHENDUPUTILSVERSION=v0r0
psycheNDUPUtils_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
psycheNDUPUtils_offset=highlandUP
psycheNDUPUtils_project=nd280rep
psycheNDUPUtils_project_release=v11r31
psycheUtils_tag=$(tag)
PSYCHEUTILSROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheUtils/v0r0
psycheUtils_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheUtils/v0r0
PSYCHEUTILSVERSION=v0r0
psycheUtils_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
psycheUtils_offset=highlandUP
psycheUtils_project=nd280rep
psycheUtils_project_release=v11r31
psycheUtils_cppflags= -DPSYCHEUTILS_USED
psycheUtils_linkopts= -L$(PSYCHEUTILSROOT)/$(psycheUtils_tag) -lpsycheUtils 
psycheUtils_stamps= $(PSYCHEUTILSROOT)/$(psycheUtils_tag)/psycheUtils.stamp
psycheNDUPUtils_cppflags= -DPSYCHENDUPUTILS_USED
psycheNDUPUtils_linkopts= -L$(PSYCHENDUPUTILSROOT)/$(psycheNDUPUtils_tag) -lpsycheNDUPUtils 
psycheNDUPUtils_stamps= $(PSYCHENDUPUTILSROOT)/$(psycheNDUPUtils_tag)/psycheNDUPUtils.stamp
psycheEventModel_cppflags=-DPSYCHEEVENTMODEL_USED -fPIC
psycheEventModel_linkopts=-L$(PSYCHEEVENTMODELROOT)/$(psycheEventModel_tag) -lpsycheEventModel
psycheEventModel_stamps= $(PSYCHEEVENTMODELROOT)/$(psycheEventModel_tag)/psycheEventModel.stamp
highlandEventModel_cppflags=-DHIGHLANDEVENTMODEL_USED -fPIC
highlandEventModel_linkopts=-L$(HIGHLANDEVENTMODELROOT)/$(highlandEventModel_tag) -lhighlandEventModel 
highlandEventModel_stamps= $(HIGHLANDEVENTMODELROOT)/$(highlandEventModel_tag)/highlandEventModel.stamp
ndupEvent_tag=$(tag)
NDUPEVENTROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/ndupEvent/v0r0
ndupEvent_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/ndupEvent/v0r0
NDUPEVENTVERSION=v0r0
ndupEvent_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
ndupEvent_offset=highlandUP
ndupEvent_project=nd280rep
ndupEvent_project_release=v11r31
install_dir=$(NDUPEVENTROOT)/$(ndupEvent_tag)
ndupEvent_cppflags=-fPIC -DNDUPEVENT_USED 
ndupEvent_linkopts= -L$(NDUPEVENTROOT)/$(ndupEvent_tag) -lndupEvent 
ndupEvent_stamps= $(ndupEventstamp)
highlandIO_cppflags= -DHIGHLANDIO_USED -fPIC
highlandIO_linkopts=-L$(HIGHLANDIOROOT)/$(highlandIO_tag) -lhighlandIO -lCore
highlandIO_stamps= $(HIGHLANDIOROOT)/$(highlandIO_tag)/highlandIO.stamp
highlandCorrections_tag=$(tag)
HIGHLANDCORRECTIONSROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandCorrections/v0r0
highlandCorrections_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandCorrections/v0r0
HIGHLANDCORRECTIONSVERSION=v0r0
highlandCorrections_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
highlandCorrections_offset=highlandUP
highlandCorrections_project=nd280rep
highlandCorrections_project_release=v11r31
highlandUtils_tag=$(tag)
HIGHLANDUTILSROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandUtils/v0r0
highlandUtils_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandUtils/v0r0
HIGHLANDUTILSVERSION=v0r0
highlandUtils_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
highlandUtils_offset=highlandUP
highlandUtils_project=nd280rep
highlandUtils_project_release=v11r31
highlandCore_tag=$(tag)
HIGHLANDCOREROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandCore/v0r0
highlandCore_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandCore/v0r0
HIGHLANDCOREVERSION=v0r0
highlandCore_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
highlandCore_offset=highlandUP
highlandCore_project=nd280rep
highlandCore_project_release=v11r31
highlandCore_cppflags=-DHIGHLANDCORE_USED -fPIC
highlandCore_linkopts=-L$(HIGHLANDCOREROOT)/$(highlandCore_tag) -lhighlandCore 
highlandCore_stamps= $(HIGHLANDCOREROOT)/$(highlandCore_tag)/highlandCore.stamp
highlandUtils_cppflags= -DHIGHLANDUTILS_USED
highlandUtils_linkopts=-L$(HIGHLANDUTILSROOT)/$(highlandUtils_tag) -lhighlandUtils 
highlandUtils_stamps= $(HIGHLANDUTILSROOT)/$(highlandUtils_tag)/highlandUtils.stamp
highlandCorrections_cppflags= -DHIGHLANDCORRECTIONS_USED
highlandCorrections_linkopts= -L$(HIGHLANDCORRECTIONSROOT)/$(highlandCorrections_tag) -lhighlandCorrections 
highlandCorrections_stamps= $(HIGHLANDCORRECTIONSROOT)/$(highlandCorrections_tag)/highlandCorrections.stamp
psycheSelections_tag=$(tag)
PSYCHESELECTIONSROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheSelections/v0r0
psycheSelections_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/psycheSelections/v0r0
PSYCHESELECTIONSVERSION=v0r0
psycheSelections_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
psycheSelections_offset=highlandUP
psycheSelections_project=nd280rep
psycheSelections_project_release=v11r31
psycheSelections_cppflags= -DPSYCHESELECTIONS_USED
psycheSelections_linkopts= -L$(PSYCHESELECTIONSROOT)/$(psycheSelections_tag) -lpsycheSelections 
psycheSelections_stamps= $(PSYCHESELECTIONSROOT)/$(psycheSelections_tag)/psycheSelections.stamp
highlandTools_tag=$(tag)
HIGHLANDTOOLSROOT=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandTools/v0r0
highlandTools_root=/home/evrazhka/ND280/nd280rep/v11r31/highlandUP/highlandTools/v0r0
HIGHLANDTOOLSVERSION=v0r0
highlandTools_cmtpath=/home/evrazhka/ND280/nd280rep/v11r31
highlandTools_offset=highlandUP
highlandTools_project=nd280rep
highlandTools_project_release=v11r31
highlandTools_cppflags=-DHIGHLANDTOOLS_USED -fPIC
highlandTools_linkopts=-L$(HIGHLANDTOOLSROOT)/$(highlandTools_tag) -lhighlandTools 
highlandTools_stamps= $(HIGHLANDTOOLSROOT)/$(highlandTools_tag)/highlandTools.stamp
baseAnalysis_cppflags= -DBASEANALYSIS_USED
baseAnalysis_linkopts= -L$(BASEANALYSISROOT)/$(baseAnalysis_tag) -lbaseAnalysis 
baseAnalysis_stamps= $(BASEANALYSISROOT)/$(baseAnalysis_tag)/baseAnalysis.stamp
ndupAnalysis_cppflags= -DNDUPANALYSIS_USED
ndupAnalysis_linkopts= -L$(NDUPANALYSISROOT)/$(ndupAnalysis_tag) -lndupAnalysis 
ndupAnalysis_stamps= $(ndupAnalysisROOT)/$(ndupAnalysis_tag)/ndupAnalysis.stamp
numuCCMultiPiAnalysis_cppflags= -DNUMUCCMULTIPIANALYSIS_USED
numuCCMultiPiAnalysis_linkopts= -L$(NUMUCCMULTIPIANALYSISROOT)/$(numuCCMultiPiAnalysis_tag) -lnumuCCMultiPiAnalysis 
numuCCMultiPiAnalysis_stamps= $(NUMUCCMULTIPIANALYSISROOT)/$(numuCCMultiPiAnalysis_tag)/numuCCMultiPiAnalysis.stamp
register_dependencies= version  version  version  version  version  version  version  version  version  version  version  version  version 
tag=amd64_linux26
package=numuCCMultiPiAnalysis
version=v0r0
PACKAGE_ROOT=$(NUMUCCMULTIPIANALYSISROOT)
srcdir=../src
bin=../$(numuCCMultiPiAnalysis_tag)/
javabin=../classes/
mgrdir=cmt
project=nd280rep
cmt_installarea_paths= $(cmt_installarea_prefix)/$(CMTCONFIG)/bin
use_linkopts= $(cmt_installarea_linkopts)   $(numuCCMultiPiAnalysis_linkopts)  $(ndupAnalysis_linkopts)  $(baseAnalysis_linkopts)  $(highlandIO_linkopts)  $(highlandCorrections_linkopts)  $(highlandUtils_linkopts)  $(highlandEventModel_linkopts)  $(psycheSelections_linkopts)  $(psycheEventModel_linkopts)  $(highlandTools_linkopts)  $(psycheNDUPUtils_linkopts)  $(psycheUtils_linkopts)  $(highlandCore_linkopts)  $(psycheCore_linkopts)  $(psychePolicy_linkopts)  $(ndupEvent_linkopts)  $(ROOT_linkopts)  $(MYSQL_linkopts)  $(GSL_linkopts)  $(EXTERN_linkopts) 
nd280rep_installarea_prefix=$(cmt_installarea_prefix)
nd280rep_installarea_prefix_remove=$(nd280rep_installarea_prefix)
use_requirements=requirements $(CMTROOT)/mgr/requirements $(NDUPANALYSISROOT)/cmt/requirements $(BASEANALYSISROOT)/cmt/requirements $(HIGHLANDIOROOT)/cmt/requirements $(HIGHLANDCORRECTIONSROOT)/cmt/requirements $(HIGHLANDUTILSROOT)/cmt/requirements $(HIGHLANDEVENTMODELROOT)/cmt/requirements $(PSYCHESELECTIONSROOT)/cmt/requirements $(PSYCHEEVENTMODELROOT)/cmt/requirements $(HIGHLANDTOOLSROOT)/cmt/requirements $(PSYCHENDUPUTILSROOT)/cmt/requirements $(PSYCHEUTILSROOT)/cmt/requirements $(HIGHLANDCOREROOT)/cmt/requirements $(PSYCHECOREROOT)/cmt/requirements $(PSYCHEPOLICYROOT)/cmt/requirements $(NDUPEVENTROOT)/cmt/requirements $(ROOTROOT)/cmt/requirements $(MYSQLROOT)/cmt/requirements $(GSLROOT)/cmt/requirements $(EXTERNROOT)/cmt/requirements 
use_includes= $(ppcmd)"$(ndupAnalysis_root)/src" $(ppcmd)"$(baseAnalysis_root)/src" $(ppcmd)"$(highlandIO_root)/src" $(ppcmd)"$(highlandCorrections_root)/src" $(ppcmd)"$(highlandUtils_root)/src" $(ppcmd)"$(highlandEventModel_root)/src" $(ppcmd)"$(psycheSelections_root)/src" $(ppcmd)"$(psycheEventModel_root)/src" $(ppcmd)"$(highlandTools_root)/src" $(ppcmd)"$(psycheNDUPUtils_root)/src" $(ppcmd)"$(psycheUtils_root)/src" $(ppcmd)"$(highlandCore_root)/src" $(ppcmd)"$(psycheCore_root)/src" $(ppcmd)"$(psychePolicy_root)/src" $(ppcmd)"$(ndupEvent_root)/src" $(ppcmd)"/home/evrazhka/ND280/nd280rep/v11r31/ROOT/v5r34p09n04/amd64_linux26/include/root" $(ppcmd)"none" $(ppcmd)"/home/evrazhka/ND280/nd280rep/v11r31/MYSQL/v5r051an01/amd64_linux26/include/mysql" $(ppcmd)"/home/evrazhka/ND280/nd280rep/v11r31/GSL/v1r15p0n00/amd64_linux26/include" $(ppcmd)"$(EXTERN_root)/src" 
use_fincludes= $(use_includes)
use_stamps= $(numuCCMultiPiAnalysis_stamps)  $(ndupAnalysis_stamps)  $(baseAnalysis_stamps)  $(highlandIO_stamps)  $(highlandCorrections_stamps)  $(highlandUtils_stamps)  $(highlandEventModel_stamps)  $(psycheSelections_stamps)  $(psycheEventModel_stamps)  $(highlandTools_stamps)  $(psycheNDUPUtils_stamps)  $(psycheUtils_stamps)  $(highlandCore_stamps)  $(psycheCore_stamps)  $(psychePolicy_stamps)  $(ndupEvent_stamps)  $(ROOT_stamps)  $(MYSQL_stamps)  $(GSL_stamps)  $(EXTERN_stamps) 
use_cflags=  $(numuCCMultiPiAnalysis_cflags)  $(ndupAnalysis_cflags)  $(baseAnalysis_cflags)  $(highlandIO_cflags)  $(highlandCorrections_cflags)  $(highlandUtils_cflags)  $(highlandEventModel_cflags)  $(psycheSelections_cflags)  $(psycheEventModel_cflags)  $(highlandTools_cflags)  $(psycheNDUPUtils_cflags)  $(psycheUtils_cflags)  $(highlandCore_cflags)  $(psycheCore_cflags)  $(psychePolicy_cflags)  $(ndupEvent_cflags)  $(ROOT_cflags)  $(MYSQL_cflags)  $(GSL_cflags)  $(EXTERN_cflags) 
use_pp_cflags=  $(numuCCMultiPiAnalysis_pp_cflags)  $(ndupAnalysis_pp_cflags)  $(baseAnalysis_pp_cflags)  $(highlandIO_pp_cflags)  $(highlandCorrections_pp_cflags)  $(highlandUtils_pp_cflags)  $(highlandEventModel_pp_cflags)  $(psycheSelections_pp_cflags)  $(psycheEventModel_pp_cflags)  $(highlandTools_pp_cflags)  $(psycheNDUPUtils_pp_cflags)  $(psycheUtils_pp_cflags)  $(highlandCore_pp_cflags)  $(psycheCore_pp_cflags)  $(psychePolicy_pp_cflags)  $(ndupEvent_pp_cflags)  $(ROOT_pp_cflags)  $(MYSQL_pp_cflags)  $(GSL_pp_cflags)  $(EXTERN_pp_cflags) 
use_cppflags=  $(numuCCMultiPiAnalysis_cppflags)  $(ndupAnalysis_cppflags)  $(baseAnalysis_cppflags)  $(highlandIO_cppflags)  $(highlandCorrections_cppflags)  $(highlandUtils_cppflags)  $(highlandEventModel_cppflags)  $(psycheSelections_cppflags)  $(psycheEventModel_cppflags)  $(highlandTools_cppflags)  $(psycheNDUPUtils_cppflags)  $(psycheUtils_cppflags)  $(highlandCore_cppflags)  $(psycheCore_cppflags)  $(psychePolicy_cppflags)  $(ndupEvent_cppflags)  $(ROOT_cppflags)  $(MYSQL_cppflags)  $(GSL_cppflags)  $(EXTERN_cppflags) 
use_pp_cppflags=  $(numuCCMultiPiAnalysis_pp_cppflags)  $(ndupAnalysis_pp_cppflags)  $(baseAnalysis_pp_cppflags)  $(highlandIO_pp_cppflags)  $(highlandCorrections_pp_cppflags)  $(highlandUtils_pp_cppflags)  $(highlandEventModel_pp_cppflags)  $(psycheSelections_pp_cppflags)  $(psycheEventModel_pp_cppflags)  $(highlandTools_pp_cppflags)  $(psycheNDUPUtils_pp_cppflags)  $(psycheUtils_pp_cppflags)  $(highlandCore_pp_cppflags)  $(psycheCore_pp_cppflags)  $(psychePolicy_pp_cppflags)  $(ndupEvent_pp_cppflags)  $(ROOT_pp_cppflags)  $(MYSQL_pp_cppflags)  $(GSL_pp_cppflags)  $(EXTERN_pp_cppflags) 
use_fflags=  $(numuCCMultiPiAnalysis_fflags)  $(ndupAnalysis_fflags)  $(baseAnalysis_fflags)  $(highlandIO_fflags)  $(highlandCorrections_fflags)  $(highlandUtils_fflags)  $(highlandEventModel_fflags)  $(psycheSelections_fflags)  $(psycheEventModel_fflags)  $(highlandTools_fflags)  $(psycheNDUPUtils_fflags)  $(psycheUtils_fflags)  $(highlandCore_fflags)  $(psycheCore_fflags)  $(psychePolicy_fflags)  $(ndupEvent_fflags)  $(ROOT_fflags)  $(MYSQL_fflags)  $(GSL_fflags)  $(EXTERN_fflags) 
use_pp_fflags=  $(numuCCMultiPiAnalysis_pp_fflags)  $(ndupAnalysis_pp_fflags)  $(baseAnalysis_pp_fflags)  $(highlandIO_pp_fflags)  $(highlandCorrections_pp_fflags)  $(highlandUtils_pp_fflags)  $(highlandEventModel_pp_fflags)  $(psycheSelections_pp_fflags)  $(psycheEventModel_pp_fflags)  $(highlandTools_pp_fflags)  $(psycheNDUPUtils_pp_fflags)  $(psycheUtils_pp_fflags)  $(highlandCore_pp_fflags)  $(psycheCore_pp_fflags)  $(psychePolicy_pp_fflags)  $(ndupEvent_pp_fflags)  $(ROOT_pp_fflags)  $(MYSQL_pp_fflags)  $(GSL_pp_fflags)  $(EXTERN_pp_fflags) 
use_libraries= $(ndupAnalysis_libraries)  $(baseAnalysis_libraries)  $(highlandIO_libraries)  $(highlandCorrections_libraries)  $(highlandUtils_libraries)  $(highlandEventModel_libraries)  $(psycheSelections_libraries)  $(psycheEventModel_libraries)  $(highlandTools_libraries)  $(psycheNDUPUtils_libraries)  $(psycheUtils_libraries)  $(highlandCore_libraries)  $(psycheCore_libraries)  $(psychePolicy_libraries)  $(ndupEvent_libraries)  $(ROOT_libraries)  $(MYSQL_libraries)  $(GSL_libraries)  $(EXTERN_libraries) 
includes= $(ppcmd)"$(srcdir)" $(use_includes)
fincludes= $(includes)
numuCCMultiPiAnalysis_use_linkopts=  $(numuCCMultiPiAnalysis_linkopts)  $(ndupAnalysis_linkopts)  $(baseAnalysis_linkopts)  $(highlandIO_linkopts)  $(highlandCorrections_linkopts)  $(highlandUtils_linkopts)  $(highlandEventModel_linkopts)  $(psycheSelections_linkopts)  $(psycheEventModel_linkopts)  $(highlandTools_linkopts)  $(psycheNDUPUtils_linkopts)  $(psycheUtils_linkopts)  $(highlandCore_linkopts)  $(psycheCore_linkopts)  $(psychePolicy_linkopts)  $(ndupEvent_linkopts)  $(ROOT_linkopts)  $(MYSQL_linkopts)  $(GSL_linkopts)  $(EXTERN_linkopts) 
RunNumuCCMultiPiAnalysis_use_linkopts=  $(numuCCMultiPiAnalysis_linkopts)  $(ndupAnalysis_linkopts)  $(baseAnalysis_linkopts)  $(highlandIO_linkopts)  $(highlandCorrections_linkopts)  $(highlandUtils_linkopts)  $(highlandEventModel_linkopts)  $(psycheSelections_linkopts)  $(psycheEventModel_linkopts)  $(highlandTools_linkopts)  $(psycheNDUPUtils_linkopts)  $(psycheUtils_linkopts)  $(highlandCore_linkopts)  $(psycheCore_linkopts)  $(psychePolicy_linkopts)  $(ndupEvent_linkopts)  $(ROOT_linkopts)  $(MYSQL_linkopts)  $(GSL_linkopts)  $(EXTERN_linkopts) 
constituents= numuCCMultiPiAnalysis RunNumuCCMultiPiAnalysis 
all_constituents= $(constituents)
constituentsclean= RunNumuCCMultiPiAnalysisclean numuCCMultiPiAnalysisclean 
all_constituentsclean= $(constituentsclean)
cmt_actions_constituents= make 
cmt_actions_constituentsclean= makeclean 
documentation_constituents= doxygen 
documentation_constituentsclean= doxygenclean 
