@REM ����һЩ·��
set project_name=DreamHeaders
set export_base=..\..\3d_part

@REM �ж�·���Ƿ���ڴӶ��Ƿ���Ҫ�½�
set dst_base=%export_base%\%project_name%
if exist %dst_base% (
	rmdir /s /q %dst_base%
)

mkdir %dst_base%\include\DreamSky

@REM �����ļ�
copy ..\include\DreamSky\*.h %dst_base%\include\DreamSky\
rem copy ..\include\DreamSky\*.hpp %dst_base%\include\DreamSky\