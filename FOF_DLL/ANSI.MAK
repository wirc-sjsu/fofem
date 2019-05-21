#
#   ANSI standard
#  compiles a DOS ANSI version
#
#
#  -A Ansi
#


# Says the Soucre Code is in the above directory
SC =

# -I switch it where to look for include files
BCCA = bcc -A -Fm -Ff=5000 -v -c -ml -a- -DANSI -nobj -f -I$(SC)

ansi.exe:   obj\ansi_mai.obj \
            obj\fof_mrt.obj \
            obj\fof_cm.obj \
            obj\fof_ci.obj \
            obj\fof_co.obj \
            obj\fof_lem.obj \
            obj\fof_hsf.obj \
            obj\fof_bcm.obj \
            obj\fof_duf.obj \
            obj\bur_bov.obj \
            obj\bur_brn.obj \
            obj\fof_sgv.obj \
            obj\fof_util.obj \
            obj\fof_unix.obj \
            obj\fof_disp.obj \
            obj\fof_sh.obj \
            obj\fof_sd.obj \
            obj\fof_se.obj \
            obj\fof_sha.obj \
            obj\fof_soi.obj \

 tlink /v   @ansi.lnk, ansi.exe, ansi.map, @ansi_lib.lnk

obj\ansi_mai.obj:  ansi_mai.cpp
   $(BCCA)         ansi_mai.cpp

obj\fof_unix.obj:  fof_unix.cpp
   $(BCCA)         fof_unix.cpp


obj\fof_mrt.obj:   $(SC)fof_mrt.cpp
   $(BCCA)         $(SC)fof_mrt.cpp

obj\fof_cm.obj:    $(SC)fof_cm.cpp
   $(BCCA)         $(SC)fof_cm.cpp

obj\fof_ci.obj:    $(SC)fof_ci.cpp
   $(BCCA)         $(SC)fof_ci.cpp

obj\fof_co.obj:    $(SC)fof_co.cpp
   $(BCCA)         $(SC)fof_co.cpp

obj\fof_lem.obj:   $(SC)fof_lem.cpp
   $(BCCA)         $(SC)fof_lem.cpp

obj\fof_hsf.obj:   $(SC)fof_hsf.cpp
   $(BCCA)         $(SC)fof_hsf.cpp

obj\fof_bcm.obj:   $(SC)fof_bcm.cpp
   $(BCCA)         $(SC)fof_bcm.cpp

obj\fof_duf.obj:   $(SC)fof_duf.cpp
   $(BCCA)         $(SC)fof_duf.cpp

obj\bur_bov.obj:   $(SC)bur_bov.cpp
   $(BCCA)         $(SC)bur_bov.cpp

obj\bur_brn.obj:   $(SC)bur_brn.cpp
   $(BCCA)         $(SC)bur_brn.cpp

obj\fof_sgv.obj:   $(SC)fof_sgv.cpp
   $(BCCA)         $(SC)fof_sgv.cpp

obj\fof_util.obj:  $(SC)fof_util.cpp
   $(BCCA)         $(SC)fof_util.cpp


obj\fof_disp.obj:  $(SC)fof_disp.cpp
   $(BCCA)         $(SC)fof_disp.cpp

obj\fof_sh.obj:    $(SC)fof_sh.cpp
   $(BCCA)         $(SC)fof_sh.cpp

obj\fof_sd.obj:    $(SC)fof_sd.cpp
   $(BCCA)         $(SC)fof_sd.cpp

obj\fof_se.obj:    $(SC)fof_se.cpp
   $(BCCA)         $(SC)fof_se.cpp

obj\fof_sha.obj:   $(SC)fof_sha.cpp
   $(BCCA)         $(SC)fof_sha.cpp

obj\fof_soi.obj:   $(SC)fof_soi.cpp
   $(BCCA)         $(SC)fof_soi.cpp
