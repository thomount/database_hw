@echo off
@for /f "tokens=1,2,3,4 delims=:." %%a in ('echo %time%') do @set h1=%%a&@set m1=%%b&@set s1=%%c&@set ms1=%%d
@%1
@for /f "tokens=1,2,3,4 delims=:." %%a in ('echo %time%') do @set h2=%%a&@set m2=%%b&@set s2=%%c&@set ms2=%%d
@set /a ds=(h2-h1)*3600+(m2-m1)*60+(s2-s1)
@set /a dms=ms2-ms1
@if %dms% lss 0 @set /a ds=ds-1&@set /a dms=100+dms
@echo %1 run for %ds%.%dms% seconds.