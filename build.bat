@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

SET deps=D:\Dev\Win\OpenGL
SET includes=/I %deps%/includes /I src/includes
SET links=/link glfw3_mt.lib opengl32.lib kernel32.lib user32.lib gdi32.lib shell32.lib /LIBPATH:%deps%/libs 
SET defines=/D DEBUG

echo "Building main..."
cl /nologo /EHsc %includes% %defines% /Fobuild\ /Femain.exe src/*.c* %links%
