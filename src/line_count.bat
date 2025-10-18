@echo off
setlocal enabledelayedexpansion
set TOTAL=0

for %%F in (*.cpp) do (
    for /f "tokens=3" %%A in ('find /c /v "" "%%F"') do (
        set /a TOTAL+=%%A
        echo %%F: %%A çs
    )
)

echo ----------------------
echo çáåvçsêî: !TOTAL!
pause