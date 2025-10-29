if(NOT WIN32)
    # uses the command line tool 'client-register', does not make sense on Windows
    add_subdirectory(clientregister)
endif(NOT WIN32)
