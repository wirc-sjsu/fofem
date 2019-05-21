

  This directory contains the FOFEM DLL code.

NOTE:...
  The code in this folder can be rounded up for user who want to compile it.
      ansi_mai.cpp - examples show how to use the code
      ansi-bat     - copies all files to ansi folder
      lower.bat    - renames all files to lower case, this helps if
                      user wants to compile on Unix which is case sensitive
      ansi.mak     - a make file, this still uses the borland compiler
                     but it's here for an example,
      g-ansi.bat   - runs the make file
   Take a look in the old FOFEM5 code folders there are some .txt document
   files that help explain the code, but read them over incase there are
   some difference with FOFEM6.
   **> in the FOFEM code the Mortality species internal table still uses
       the old 6 letters codes. User can use the fof_spp.dat for now, might
       have to fix the table at some point.
