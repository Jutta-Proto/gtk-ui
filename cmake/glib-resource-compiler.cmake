function(glib_add_resource_file)
    find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources)
    mark_as_advanced(GLIB_COMPILE_RESOURCES)
    
    if (${GLIB_COMPILE_RESOURCES})
        message(FATAL_ERROR "Glib resource compiler not found")
        return()
    endif ()
    cmake_parse_arguments(GLIB_COMP "" "TARGET;RESOURCEDEF;OUTDIR;OUTFILE;WORKING_DIR" "DEPENDS" ${ARGN})
    add_custom_command(OUTPUT ${GLIB_COMP_OUTFILE}
                       COMMAND ${GLIB_COMPILE_RESOURCES}
                       ARGS
                            ${GLIB_COMP_RESOURCEDEF}
                            --target=${CMAKE_CURRENT_BINARY_DIR}/${GLIB_COMP_OUTFILE}
                            --generate-source
                       COMMENT "Generate resource file ${GLIB_COMP_OUTFILE}"
                       MAIN_DEPENDENCY ${GLIB_COMP_RESOURCEDEF}
                       DEPENDS ${GLIB_COMP_DEPENDS}
                       IMPLICIT_DEPENDS ${GLIB_COMP_DEPENDS}
                       WORKING_DIRECTORY ${GLIB_COMP_WORKING_DIR})
endfunction()
