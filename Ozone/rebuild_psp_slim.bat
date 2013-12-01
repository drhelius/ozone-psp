echo --------------------------------
echo Clean --------------------------
echo --------------------------------
del *.pbp
del *.o
del *.elf
del *.sfo
del *.prx
echo --------------------------------
echo Build --------------------------
echo --------------------------------
path=e:/pspdev/bin
make -f makefile_psp_slim



