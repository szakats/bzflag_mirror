;NSIS Modern User Interface version 1.69
;Original templates by Joost Verburg
;Redesigned for BZFlag by blast007

;--------------------------------
;BZLgen Version Variables

  !define VER_MAJOR 0
  !define VER_MINOR .0.1

;--------------------------------
;Compression options

  ;If you want to comment these
  ;out while testing, it speeds
  ;up the installer compile time
  ;Uncomment to reduce installer
  ;size by ~35%
  SetCompress auto
  SetCompressor /SOLID lzma

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;Configuration

  ;General
  Name "BZlGen ${VER_MAJOR}${VER_MINOR}"
  OutFile "..\..\bzlgen-${VER_MAJOR}${VER_MINOR}.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\BZlgen${VER_MAJOR}${VER_MINOR}"

  ; Make it look pretty in XP
  XPStyle on

;--------------------------------
;Variables

  Var MUI_TEMP
  Var STARTMENU_FOLDER

;--------------------------------
;Interface Settings

  ;Icons
  !define MUI_ICON ..\BZlgen.ico
  !define MUI_UNICON uninstall.ico

  ;Bitmaps
  !define MUI_WELCOMEFINISHPAGE_BITMAP "side.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "side.bmp"

  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP "header.bmp"
  !define MUI_COMPONENTSPAGE_CHECKBITMAP "${NSISDIR}\Contrib\Graphics\Checks\simple-round2.bmp"

  !define MUI_COMPONENTSPAGE_SMALLDESC

  ;Show a warning before aborting install
  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  ;Welcome page configuration
  !define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of BZFlag ${VER_MAJOR}${VER_MINOR}.\r\n\r\nBZFlag is a free multiplayer multiplatform 3D tank battle game. The name stands for Battle Zone capture Flag. It runs on Irix, Linux, *BSD, Windows, Mac OS X and other platforms. It's one of the most popular games ever on Silicon Graphics machines.\r\n\r\nClick Next to continue."

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "copying.rtf"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\BZlgen${VER_MAJOR}${VER_MINOR}" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

  !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER


  !insertmacro MUI_PAGE_INSTFILES

  ;Finished page configuration
  !define MUI_FINISHPAGE_NOAUTOCLOSE
  !define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\README"
  !define MUI_FINISHPAGE_SHOWREADME_TEXT "View Readme"
  !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED

  !define MUI_FINISHPAGE_LINK "BZFlag Home Page"
  !define MUI_FINISHPAGE_LINK_LOCATION "http://www.bzflag.org"

  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

  !define MUI_UNFINISHPAGE_NOAUTOCLOSE
  !insertmacro MUI_UNPAGE_FINISH
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "!BZlgen (Required)" BZlgen
  ;Make it required
  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File ..\..\BZlgen.exe
  File ..\..\AUTHORS
  File ..\..\COPYING
  File ..\..\README
  File bzlgen.exe.manifest
  
  ; make the data dir
  SetOutPath $INSTDIR\media
  File ..\..\media\*.*

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\BZlgen${VER_MAJOR}${VER_MINOR} "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BZlgen${VER_MAJOR}${VER_MINOR}" "DisplayName" "BZlgen ${VER_MAJOR}${VER_MINOR} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BZlgen${VER_MAJOR}${VER_MINOR}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"


  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Main start menu shortcuts
    SetOutPath $INSTDIR
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\BZlgen ${VER_MAJOR}${VER_MINOR}.lnk" "$INSTDIR\bzflag.exe" "" "$INSTDIR\bzflag.exe" 0
  
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_BZlgen ${LANG_ENGLISH} "Installs the main city generator files"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${BZlgen} $(DESC_BZlgen)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"
  ;remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BZlgen${VER_MAJOR}${VER_MINOR}"
  DeleteRegKey HKLM "SOFTWARE\BZlgen${VER_MAJOR}${VER_MINOR}"
  DeleteRegKey HKCU "Software\BZlgen"

  ; remove files
  Delete $INSTDIR\*.*
  Delete $INSTDIR\media\*.*

  ; MUST REMOVE UNINSTALLER, too
  Delete $INSTDIR\uninstall.exe

  ; remove directories used.
  RMDir "$INSTDIR\media"
  RMDir "$INSTDIR"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP

  ;remove shortcuts, if any.    
  Delete "$SMPROGRAMS\$MUI_TEMP\*.*"
  
  ;Delete empty start menu parent diretories
  StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"
 
  startMenuDeleteLoop:
    RMDir $MUI_TEMP
    GetFullPathName $MUI_TEMP "$MUI_TEMP\.."
    
    IfErrors startMenuDeleteLoopDone
  
    StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
  startMenuDeleteLoopDone:

SectionEnd
