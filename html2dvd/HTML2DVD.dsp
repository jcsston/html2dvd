# Microsoft Developer Studio Project File - Name="HTML2DVD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HTML2DVD - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HTML2DVD.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HTML2DVD.mak" CFG="HTML2DVD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HTML2DVD - Win32 Release DLL" (based on "Win32 (x86) Application")
!MESSAGE "HTML2DVD - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HTML2DVD - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HTML2DVD - Win32 Release DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseDll"
# PROP BASE Intermediate_Dir "ReleaseDll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseDll"
# PROP Intermediate_Dir "ReleaseDll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /YX /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "..\matroska\wxWindows-2.4.2\include" /I "..\matroska\wxWindows-2.4.2\lib\mswdllu" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_UNICODE" /D "UNICODE_MT" /D wxUSE_GUI=1 /D "WXUSINGDLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /i "../../include" /d "NDEBUG"
# ADD RSC /l 0x409 /i "../../include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 comctl32.lib rpcrt4.lib wsock32.lib wxmsw24u.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Unicows.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\matroska\wxWindows-2.4.2\lib"

!ELSEIF  "$(CFG)" == "HTML2DVD - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /YX /FD /c
# ADD CPP /nologo /MD /W4 /O2 /Ob2 /I "..\matroska\wxWindows-2.4.2\include" /I "..\matroska\wxWindows-2.4.2\lib\mswu" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_UNICODE" /D "UNICODE" /D "_MT" /D wxUSE_GUI=1 /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /i "../../include" /d "NDEBUG"
# ADD RSC /l 0x409 /i "../../include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib zlib.lib regex.lib png.lib jpeg.lib tiff.lib wxmswu.lib Unicows.lib /nologo /subsystem:windows /map /machine:I386 /libpath:"..\matroska\wxWindows-2.4.2\lib"

!ELSEIF  "$(CFG)" == "HTML2DVD - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /YX /FD /c
# ADD CPP /nologo /MDd /W4 /Zi /Od /I "..\matroska\wxWindows-2.5.1\include" /I "..\matroska\wxWindows-2.5.1\lib\mswud" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_UNICODE" /D "UNICODE" /D "_MT" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /i "../../include" /d "_DEBUG"
# ADD RSC /l 0x409 /i "../../include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib zlibd.lib regexd.lib pngd.lib jpegd.lib tiffd.lib wxmswud.lib Unicows.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\matroska\wxWindows-2.5.1\lib"

!ENDIF 

# Begin Target

# Name "HTML2DVD - Win32 Release DLL"
# Name "HTML2DVD - Win32 Release"
# Name "HTML2DVD - Win32 Debug"
# Begin Source File

SOURCE=.\dvdauthorController.cpp
# End Source File
# Begin Source File

SOURCE=.\dvdauthorController.h
# End Source File
# Begin Source File

SOURCE=.\DVDMenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DVDMenuButton.h
# End Source File
# Begin Source File

SOURCE=.\DVDMenuWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\DVDMenuWriter.h
# End Source File
# Begin Source File

SOURCE=.\HTML2DVD.cpp
# End Source File
# Begin Source File

SOURCE=.\HTML2DVD.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\HTML2DVD.h
# End Source File
# Begin Source File

SOURCE=.\HTML2DVD.ico
# End Source File
# Begin Source File

SOURCE=.\HTML2DVD.rc
# End Source File
# Begin Source File

SOURCE=.\HTML2DVDImages.cpp
# End Source File
# Begin Source File

SOURCE=.\HTML2DVDImages.h
# End Source File
# Begin Source File

SOURCE=.\ResourceLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceLoader.h
# End Source File
# Begin Source File

SOURCE=.\resources.zip
# End Source File
# Begin Source File

SOURCE=.\wxFontConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\wxFontConfig.h
# End Source File
# Begin Source File

SOURCE=.\wxHTMLBitmapRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\wxHTMLBitmapRenderer.h
# End Source File
# Begin Source File

SOURCE=.\wxImageResampler.cpp
# End Source File
# Begin Source File

SOURCE=.\wxImageResampler.h
# End Source File
# End Target
# End Project
