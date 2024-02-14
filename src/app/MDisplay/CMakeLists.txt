message(STATUS "MDisplay ...")


file (GLOB MDISPLAY_SOURCEFILES 
    src/*.cc
)

file (GLOB MDISPLAY_HEADERFILES 
    src/*.hh
)

set (MDISPLAY_DEP_LIBRARIES
    ND280Event
)

pbuilder_library( MDisplay MDISPLAY_SOURCEFILES MDISPLAY_DEP_LIBRARIES )
pbuilder_install_headers (${MDISPLAY_HEADERFILES})

# ############ FOR MDISPLAY FRAMEWORK #############

set (MDISPLAYSPROG_DEP_LIBRARIES
    ND280Event
    EffStudy
    Reconstruction
    MDisplay
)

set (MDISPLAY_PROGRAMS
    MDisplayApp
)

pbuilder_executables( MDISPLAY_PROGRAMS MDISPLAYSPROG_DEP_LIBRARIES )
