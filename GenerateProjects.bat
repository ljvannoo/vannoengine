rmdir /Q /S bin
rmdir /Q /S bin-temp

del vannoengine.sln
del VannoEngine\VannoEngine.vcxproj*
del Sandbox\Sandbox.vcxproj*

call vendor\premake\premake5.exe vs2019
PAUSE