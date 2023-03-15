powershell.exe -Command "(gci -include *.cpp,*.hpp,*.h -recurse | select-string .).Count"
pause