echo off

devenv EquinoxEngine.sln /Build Debug
devenv EquinoxEngine.sln /Build Release

set /p VERSIONNUMBER="Version Number: " || set "VERSIONNUMBER="
mkdir "Release Build"
cd "Release Build"
mkdir "EquinoxEngine %VERSIONNUMBER%"
cd "EquinoxEngine %VERSIONNUMBER%"
mkdir include
mkdir lib
cd include
mkdir Equinox

cd ../../../

xcopy "eqMath/src/Equinox" "Release Build/EquinoxEngine %VERSIONNUMBER%/include/Equinox" /e
xcopy "eqPhysics/src/Equinox" "Release Build/EquinoxEngine %VERSIONNUMBER%/include/Equinox" /e
xcopy "EquinoxEngine/src/Equinox" "Release Build/EquinoxEngine %VERSIONNUMBER%/include/Equinox" /e

copy "%cd%\x64\Debug\eqMath.lib" "%cd%\Release Build\EquinoxEngine %VERSIONNUMBER%\lib\eqMath-d.lib"
copy "%cd%\x64\Debug\eqPhysics.lib" "%cd%\Release Build\EquinoxEngine %VERSIONNUMBER%\lib\eqPhysics-d.lib"
copy "%cd%\x64\Debug\EquinoxEngine.lib" "%cd%\Release Build\EquinoxEngine %VERSIONNUMBER%\lib\EquinoxEngine-d.lib"

copy "%cd%\x64\Release\eqMath.lib" "%cd%\Release Build\EquinoxEngine %VERSIONNUMBER%\lib\eqMath.lib"
copy "%cd%\x64\Release\eqPhysics.lib" "%cd%\Release Build\EquinoxEngine %VERSIONNUMBER%\lib\eqPhysics.lib"
copy "%cd%\x64\Release\EquinoxEngine.lib" "%cd%\Release Build\EquinoxEngine %VERSIONNUMBER%\lib\EquinoxEngine.lib"