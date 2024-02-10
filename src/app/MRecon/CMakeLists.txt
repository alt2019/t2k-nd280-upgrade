message(STATUS "MRecon ...")


file (GLOB MRECON_SOURCEFILES 
    src/*.cc
)

file (GLOB MRECON_HEADERFILES 
    src/*.hh
)

set (MRECON_DEP_LIBRARIES
    ND280Event
)

pbuilder_library( MRecon MRECON_SOURCEFILES MRECON_DEP_LIBRARIES )
pbuilder_install_headers (${MRECON_HEADERFILES})

# ############ FOR MRECON FRAMEWORK #############

set (MRECONSPROG_DEP_LIBRARIES
    ND280Event
    EffStudy
    Reconstruction
    MRecon
)

set (MRECON_PROGRAMS
    MReconApp
)

pbuilder_executables( MRECON_PROGRAMS MRECONSPROG_DEP_LIBRARIES )
