@REM 设置一些路径
set project_name=DreamCommon
set bin_base=.\bin
set export_base=..\..\3d_part
set version=v1.0.0

@REM 判断路径是否存在从而是否需要新建
set dst_base=%export_base%\%project_name%\%version%
if exist %dst_base% (
	rmdir /s /q %dst_base%
)

mkdir %dst_base%\include\DreamSky
mkdir %dst_base%\lib
mkdir %dst_base%\bin

@REM 拷贝文件
xcopy ..\include\DreamSky %dst_base%\include\DreamSky\ /Y /E /C
copy %bin_base%\*.dll %dst_base%\bin\
copy %bin_base%\*.lib %dst_base%\lib\