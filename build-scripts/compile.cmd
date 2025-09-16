@echo off
ninja -C ../build

IF EXIST "..\build\test\users.exe" (
  COPY "..\build\test\users.exe" "."
) ELSE (
  ECHO Executable does not exist!
)
