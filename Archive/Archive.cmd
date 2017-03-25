@set TARGET=%1
@set ORIGIN=%~dp0
@set DEPLOY=%TARGET%.deploy
@set TRANSLATE=%ORIGIN%\..\translate"
@if exist ..\Archive\%DEPLOY% del /s /q ..\Archive\%DEPLOY%
@mkdir ..\Archive >nul 2>&1
@mkdir ..\Archive\%DEPLOY% >nul 2>&1
@mkdir ..\Archive\Symbols >nul 2>&1
@copy /y %TARGET%.exe ..\Archive\%DEPLOY% >nul
@copy /y %TARGET%.pdb ..\Archive\Symbols\ >nul
@cd ..\Archive\%DEPLOY%
@windeployqt %TARGET%.exe -verbose=0
@if exist %TRANSLATE% copy /y %TRANSLATE%\*.qm translations\ >nul
