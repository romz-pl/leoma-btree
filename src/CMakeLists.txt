
set( LIB_NAME leoma-btree )

add_library( ${LIB_NAME} STATIC
    bplustree.c
)

target_include_directories( ${LIB_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src )

target_compile_options( ${LIB_NAME} PRIVATE ${ROMZ_C_FLAGS} )


