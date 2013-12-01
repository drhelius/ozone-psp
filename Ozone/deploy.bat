echo --------------------------------
echo Clean --------------------------
echo --------------------------------
del *.pbp
del *.o
del *.elf
del *.sfo
echo --------------------------------
echo Build --------------------------
echo --------------------------------
path=e:/pspdev/bin
makegenerator "Ozone"
make
echo --------------------------------
echo PSPBrew ------------------------
echo --------------------------------
path="c:/Archivos de programa/PSP Brew"
pspbrew /p eboot.pbp /h Ozone /d OZONE /n 1 /u E: /f 1 /s 0


