@echo off

::���ʱ��������λ����
set _interval=5

::��Ҫ�ػ��Ľ�������
set _processName=DataTransfer

::��Ҫ�ػ��Ľ�����������
set _processCmd=DataTransfer.exe

::��Ҫ�ػ��Ľ���Ԥ�������������ʱ�䣬��λ����
set _processTimeout=10

::��Ҫ�ػ��Ľ����������Ķ˿�
set _port=8080

::�����û�����һ����Administrator
set _username=Administrator

:LOOP
set /a isAlive=false

::ͨ���������Ƽ��
tasklist /FI "username eq %_username%" | find /C "%_processName%" > temp.txt
set /p num= < temp.txt
del /F temp.txt


if "%num%" == "0" (
start %_processCmd% | echo ���� %_processName% �� %time%
choice /D y /t %_processTimeout% > nul
)

if "%num%" NEQ "0" echo ������

::ping -n %_interval% 127.1>nul
choice /D y /t %_interval% >nul

goto LOOP