"C:\Program Files\CMake\bin\cmake.exe" -G "Visual Studio 12 2013" .. -B .
"%VS120COMNTOOLS%\..\IDE\devenv" "libasio.sln" /Rebuild "Release|win32" /Project "libasio_static"
