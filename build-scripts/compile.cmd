@echo off
ninja -C ../build

IF EXIST "..\build\test\log.exe" (
  COPY "..\build\test\log.exe" "..\test\log.exe"
) ELSE (
  ECHO Executable does not exist!
)
