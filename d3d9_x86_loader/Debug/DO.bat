taskkill /f /im TeknoMW3.exe
TIMEOUT /T 1
C:
cd \
cd Games
cd "Call of Duty- Modern Warfare 3"
copy ".\d3d9_x86_loader\Debug\d3d9.dll" d3d9.dll /Y
start TeknoMW3.exe