@echo off

echo WORKSPACE=[%WORKSPACE%]
echo BOARD=[%1]

rem Move to initial directory
cd C:\Users\MichaelDesrosiers\zephyrproject

rem Activate Python virtual environment
call .venv\Scripts\activate.bat

rem Change into the project directory
cd %WORKSPACE%
rem Run the Zephyr build
west build -p auto -b %1 .