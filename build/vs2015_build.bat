"C:\Program Files\CMake\bin\cmake.exe" -G "Visual Studio 14 2015" .. -B .
"%VS140COMNTOOLS%\..\IDE\devenv" "libasio.sln" /Rebuild "Release|win32" /Project "libasio_static"
