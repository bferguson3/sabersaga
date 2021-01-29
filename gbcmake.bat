@ECHO OFF 
IF EXIST sabersaga.gbc ( del /s *.gbc )
IF EXIST sabersaga.gb ( del /s *.gb )
IF NOT EXIST out ( mkdir out )
rem cd out 
if !%1==!clean (
    ECHO Cleaning build folder...
    del /s *.o *.lst *.map *.gb *.gbc *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm *.noi *.txt *.bin >NUL
)
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -o out\sabersaga.gb .\src\main.c 1>out\output.txt 2>&1
echo  [33m
findstr "warning" out\output.txt
echo  [31m
findstr "error" out\output.txt 
echo  [37m
copy out\sabersaga.gb . >NUL
rem cd ..
python3 .\tools\makegbc.py sabersaga.gb 
ECHO Build done. Launching emulator...
c:\Users\Bent\Downloads\visualboyadvance-m.exe sabersaga.gbc
@ECHO ON
