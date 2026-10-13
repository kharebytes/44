@echo off
g++ -std=c++11 -Wall -Wextra main.cpp -o library
if %errorlevel%==0 (echo Build OK. Run: library) else (echo Build FAILED)
