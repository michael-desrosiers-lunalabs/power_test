@echo off

rem Move to initial directory
cd C:\Users\MichaelDesrosiers\zephyrproject

rem Activate Python virtual environment
call .venv\Scripts\activate

rem Change into the project directory
cd zephyr\serialsleep

rem Run the graphical build configuration tool
west build -t guiconfig