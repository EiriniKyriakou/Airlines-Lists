Project for [CS-240] Data Structures 

Directory structure
-------------------

    src folder 
    ----------
    contains the source files of the project.

    main.c
    ------
    contains the main function of the project.

    airlines.h 
    ---------
    contains the function declaration's needed by main.c as well as
    the structure definitions.

    airlines.c 
    ---------
    contains the functions declared in airlines.h
    
    
    
Makefile
--------

    To use the provided Makefile to build your project all you need is
    3 commands.

    $ make

    this will build the project and produce a file 'airlines' which is
    the program executable with which you can test your
    project. i.e. './airlines Test_Files/testfile1.txt'.

    $ make clean

    this will erase any intermediate files produced during the compilation.

    $ make distclean

    this is the same with 'make clean' except that it also erases
    'airlines'.

    During compilation the Makefile will produce two directories, obj and
    dep. You don't have to worry about this directories. To remove them
    execute 'make clean'.
