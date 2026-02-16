@echo off

rem Move to initial directory
cd C:\Users\MichaelDesrosiers\zephyrproject

rem Activate Python virtual environment
call .venv\Scripts\activate

rem Change into the project directory
cd zephyr\power_test

rem Flash the build
west -v flash --runner %1