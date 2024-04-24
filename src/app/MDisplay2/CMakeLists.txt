message(STATUS "MDisplay2 ...")


file (GLOB MDISPLAY2_SOURCEFILES 
    src/*.cc
)

file (GLOB MDISPLAY2_HEADERFILES 
    src/*.hh
)

set (MDISPLAY2_DEP_LIBRARIES
    ND280Event
)

pbuilder_library( MDisplay2 MDISPLAY2_SOURCEFILES MDISPLAY2_DEP_LIBRARIES )
pbuilder_install_headers (${MDISPLAY2_HEADERFILES})

# ############ FOR MDISPLAY2 FRAMEWORK #############

set (MDISPLAY2SPROG_DEP_LIBRARIES
    ND280Event
    EffStudy
    Reconstruction
    MDisplay2
)

set (MDISPLAY2_PROGRAMS
    MDisplayApp2
)

pbuilder_executables( MDISPLAY2_PROGRAMS MDISPLAY2SPROG_DEP_LIBRARIES )
