find_package (Threads)

include_directories(
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/crnlib
)

# Defines the source code for the library
set(TO_CRN_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tocrn.h
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_library(tocrn  SHARED ${TO_CRN_SRCS})
target_link_libraries(tocrn ${CRUNCH_LIBRARY_NAME} ${CMAKE_THREAD_LIBS_INIT})
