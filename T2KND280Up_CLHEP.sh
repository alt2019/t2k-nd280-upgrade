

# setup GEANT4
#source /atlas/users/dsgalabe/GEANT4/geant4.10.01.p03-build/geant4make.sh # env var of G4 (e.g. data variables...)
source /atlas/users/dsgalabe/GEANT4/geant4.10.01.p03-install/bin/geant4.sh

export GEANT4_INCLUDE=/atlas/users/dsgalabe/GEANT4/geant4.10.01.p03-install/include/Geant4/
export GEANT4_INSTALL_BIN=/atlas/users/dsgalabe/GEANT4/geant4.10.01.p03-install/bin/
export GEANT4_LIBRARY=/atlas/users/dsgalabe/GEANT4/geant4.10.01.p03-install/lib64

# setup ROOT
source /atlas/users/dsgalabe/ROOT/root/bin/thisroot.sh

# # setup CMT
# source /atlas/users/dsgalabe/CMT/setup.sh
# export CMTPATH=$1

# # setup highlandUp
# source /atlas/users/dsgalabe/t2k-nd280-upgrade/highlandUP/ndupAnalysis/v0r0/cmt/setup.sh
# source /atlas/users/dsgalabe/t2k-nd280-upgrade/highlandUP/highlandTools/v0r0/cmt/setup.sh 

# setup gcc-xml
#export PATH=/afs/cern.ch/user/d/dsgalabe/GCCXML/gccxml-install/bin:$PATH # gcc-xml

# setup Xerces-C++
#export XERCESC_INSTALL_DIR=/atlas/users/dsgalabe/XERCES-C++/xerces-c-3.1.3-install
export XERCESC_INSTALL_DIR=/atlas/users/dsgalabe/xerces-c-3.1.3/xerces-c-3.1.3-install
#export XERCESC_INCLUDE_DIR=/atlas/users/dsgalabe/xerces-c-3.1.3/xerces-c-3.1.3-install/include
#export XERCESC_LIBRARY_DIR=/atlas/users/dsgalabe/xerces-c-3.1.3/xerces-c-3.1.3-install/lib
#export LD_LIBRARY_PATH=$XERCESC_LIBRARY_DIR:$XERCESC_INCLUDE_DIR:$XERCESC_INSTALL_DIR/lib:$LD_LIBRARY_PATH # Xerces-C++
export LD_LIBRARY_PATH=$XERCESC_INSTALL_DIR/lib:$LD_LIBRARY_PATH # Xerces-C++

# setup CLHEP
export CLHEP_INCLUDE_DIR=/atlas/users/dsgalabe/CLHEP/clhep_install/include
export CLHEP_LIBRARY_DIR=/atlas/users/dsgalabe/CLHEP/clhep_install/lib
export LD_LIBRARY_PATH=$CLHEP_INCLUDE_DIR:$CLHEP_LIBRARY_DIR:$LD_LIBRARY_PATH        

# # setup VGM
# export VGM_INSTALL_DIR=/atlas/users/dsgalabe/VGM/vgm.4.3-install

# setup t2k-nd280-upgrade
export T2KND280UP=$1
export T2KND280UP_SRC=$T2KND280UP/src/app/EffStudy 
export T2KND280UP_EVT=$T2KND280UP/src/app/nd280UpEvent