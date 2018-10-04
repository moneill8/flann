if(NOT EXISTS "/Users/Bryce/GitHub/flann/flann/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"/Users/Bryce/GitHub/flann/flann/install_manifest.txt\"")
endif(NOT EXISTS "/Users/Bryce/GitHub/flann/flann/install_manifest.txt")

file(READ "/Users/Bryce/GitHub/flann/flann/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
    message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
    if(EXISTS "$ENV{DESTDIR}${file}")
        exec_program("/Volumes/cmake-3.2.2-Darwin-x86_64/CMake.app/Contents/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
            OUTPUT_VARIABLE rm_out RETURN_VALUE rm_retval)
        if(NOT "${rm_retval}" STREQUAL 0)
            message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
        endif(NOT "${rm_retval}" STREQUAL 0)
    else(EXISTS "$ENV{DESTDIR}${file}")
        message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
    endif(EXISTS "$ENV{DESTDIR}${file}")
endforeach(file)

