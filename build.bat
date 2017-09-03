del cmakeBuild\* /Q
del bin\* /Q
rmdir /s /q cmakeBuild
rmdir /s /q bin

mkdir cmakeBuild
cd cmakeBuild
cmake ../

msbuild AC_SensorModels.sln /property:Configuration=Debug
msbuild AC_SensorModels.sln /property:Configuration=Release

cd ..

mkdir bin
mkdir bin\debug
mkdir bin\release
xcopy cmakeBuild\debug\AC_SensorModels.exe bin\debug\ /i
xcopy cmakeBuild\debug\AC_SensorModels.pdb bin\debug\ /i

xcopy cmakeBuild\release\AC_SensorModels.exe bin\release\ /i

echo AC_SensorModels.exe -d ../../data/sensor/ > bin\debug\run.bat
echo AC_SensorModels.exe -d ../../data/sensors/ > bin\release\run.bat