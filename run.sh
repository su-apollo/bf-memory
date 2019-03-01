echo off
echo >/dev/null # >nul & GOTO WINDOWS & rem ^
echo 'Processing for Linux'
# ***********************************************************
# * NOTE: If you modify this content, be sure to remove carriage returns (\r)
# *       from the Linux part and leave them in together with the line feeds
# *       (\n) for the Windows part. In summary:
# *           New lines in Linux: \n
# *           New lines in Windows: \r\n
# ***********************************************************
# Do Linux Bash commands here... for example:

docker-compose -f docker-compose.yml up $@

# Then, when all Linux commands are complete, end the script with 'exit'...
exit 0
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
:WINDOWS
@echo off
echo "Processing for Windows"

docker-compose -f docker-compose.yml up $*

REM Then, when all Windows commands are complete... the script is done.
