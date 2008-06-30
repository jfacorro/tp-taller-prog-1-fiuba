# Microsoft Developer Studio Project File - Name="BattleCityServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=BattleCityServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BattleCityServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BattleCityServer.mak" CFG="BattleCityServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BattleCityServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "BattleCityServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BattleCityServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x400a /d "NDEBUG"
# ADD RSC /l 0x400a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "BattleCityServer - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "..\SDL\include" /I "Tanque\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x400a /d "_DEBUG"
# ADD RSC /l 0x400a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\SDL\SDL.lib ..\SDL\SDLmain.lib ..\SDL\SDL_image.lib ws2_32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "BattleCityServer - Win32 Release"
# Name "BattleCityServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Tanque\ArrayList.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\BattleCityCommunicationProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\BattleCityEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\BattleCityServer.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\BattleCityTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\BattleCityWall.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\Screen.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\SDL_rotozoom.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\SDLHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\Socket.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\StringHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\Tag.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\TagProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\Tanque.cpp
# End Source File
# Begin Source File

SOURCE=.\Tanque\XMLParser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Tanque\ArrayList.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\BattleCityCommunicationProtocol.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\BattleCityEngine.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\BattleCityServer.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\BattleCityTypes.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\BattleCityWall.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\Common.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\Exception.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\Screen.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\SDL_keysym.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\SDL_rotozoom.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\SDLHelper.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\Socket.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\StringHelper.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\Tag.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\TagProperty.h
# End Source File
# Begin Source File

SOURCE=.\Tanque\XMLParser.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
