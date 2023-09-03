@REM 设置一些路径
set project_name=DreamSocket
set bin_base=.\bin
set export_base=..\..\3d_part

@REM 判断路径是否存在从而是否需要新建
set dst_base=%export_base%\%project_name%
if exist %dst_base% (
	rmdir /s /q %dst_base%
)

mkdir %dst_base%\include\DreamSky
mkdir %dst_base%\lib

@REM 拷贝文件
copy ..\include\DreamSky\*.h %dst_base%\include\DreamSky\
copy %bin_base%\*.dll %dst_base%\lib\
copy %bin_base%\*.lib %dst_base%\lib\