
include(CMakeParseArguments)
find_program(ROOTCINT_EXECUTABLE rootcint PATHS $ENV{ROOTSYS}/bin)

#----------------------------------------------------------------------------
# function ROOT_GENERATE_DICTIONARY( dictionary   
#                                    header1 header2 ... 
#                                    LINKDEF linkdef1 ... 
#                                    OPTIONS opt1...)
function(ROOT_GENERATE_DICTIONARY dictionary)
  CMAKE_PARSE_ARGUMENTS(ARG "" "LIBRARY" "LINKDEF;OPTIONS;DEPENDS" "" ${ARGN})
  #---Get the library name --------------------------------
  set(library ${ARG_LIBRARY}) # this is the final generated library name
  message("============= ARG_LIBRARY: ${ARG_LIBRARY}")

  #---Get the list of header files-------------------------
  set(headerfiles)
  foreach(fp ${ARG_UNPARSED_ARGUMENTS})
    file(GLOB files ${fp})
    if(files)
      foreach(f ${files})
        if(NOT f MATCHES LinkDef)
          set(headerfiles ${headerfiles} ${f})
        endif()
      endforeach()
    else()
      set(headerfiles ${headerfiles} ${fp})
    endif()
  endforeach()
  #---Get the list of include directories------------------
  get_directory_property(incdirs INCLUDE_DIRECTORIES)
  set(includedirs) 
  foreach( d ${incdirs})    
    if (d MATCHES "^/usr/include")
      # message("omit ${d}")
    else()
      set(includedirs ${includedirs} -I${d})
    endif()
  endforeach()
  #---Get LinkDef.h file------------------------------------
  set(linkdefs)
  foreach( f ${ARG_LINKDEF})
    if( IS_ABSOLUTE ${f})
      set(linkdefs ${linkdefs} ${f})
    else() 
      if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/inc/${f})
        set(linkdefs ${linkdefs} ${CMAKE_CURRENT_SOURCE_DIR}/inc/${f})
      else()
        set(linkdefs ${linkdefs} ${CMAKE_CURRENT_SOURCE_DIR}/${f})
      endif()
    endif()
  endforeach()
  #---call rootcint------------------------------------------
  add_custom_command(OUTPUT ${dictionary}.cxx 
                            # ${dictionary}.h # the dict header is not generated any more.
                            ${dictionary}.rootmap
                            ${dictionary}_rdict.pcm
                     # COMMAND ${ROOTCINT_EXECUTABLE} -cint -f  ${dictionary}.cxx 
                     #                      -c ${ARG_OPTIONS} ${includedirs} ${headerfiles} ${linkdefs} 
                     COMMAND echo "hello"
                     COMMAND ${ROOTCINT_EXECUTABLE} -f ${dictionary}.cxx 
                                          -rml lib${library}.so # get the library name from package name
                                          -rmf ${dictionary}.rootmap
                                          -c ${ARG_OPTIONS} ${includedirs} ${headerfiles} ${linkdefs} 
                     DEPENDS ${headerfiles} ${linkdefs}
                             ${ARG_DEPENDS}
                     )
endfunction()
