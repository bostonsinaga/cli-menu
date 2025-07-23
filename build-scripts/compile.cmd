@echo off
ninja -C ../build

IF EXIST "..\build\test\organism.exe" (
  COPY "..\build\test\organism.exe" "."
) ELSE (
  ECHO Executable does not exist!
)
