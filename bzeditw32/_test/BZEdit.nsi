; BZEdit.nsi
;
; This script is based on example1.nsi, but adds uninstall support
; and (optionally) start menu shortcuts.
;
; It will install notepad.exe into a directory that the user selects,
;

!define VER_MAJOR 1.6
!define VER_MINOR .3

; Main Installer Options
Name "BZEdit"
WindowIcon On
EnabledBitmap "EnableCheck.bmp"
DisabledBitmap "DisableCheck.bmp"
Caption "BZEdit ${VER_MAJOR}${VER_MINOR} for Windows - Setup"

; The file to write
OutFile "BZE_${VER_MAJOR}${VER_MINOR}_Installer.exe"

; The default installation directory
InstallDir $PROGRAMFILES\BZEdit${VER_MAJOR}${VER_MINOR}

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM SOFTWARE\BZEdit${VER_MAJOR}${VER_MINOR} "Install_Dir"

; The text to prompt the user to enter a directory
ComponentText "This will install the BZEdit ${VER_MAJOR}${VER_MINOR} for Windows on your computer."

; The text to prompt the user to enter a directory
DirText "Please choose a directory to install in to:"

CompletedText " Thank you for installing BZEdit${VER_MAJOR}${VER_MINOR}."
; The stuff to install

Section "BZEdit (required)"
	; Set output path to the installation directory.
	SetOutPath $INSTDIR
	; Put file there
	File BZEdit32.exe
	File BZEdit32.exe.manifest
	File PocketEngine.dll
	File TextureMngr.dll

	; make the data dir
	SetOutPath $INSTDIR\data
	File data\*.*	

	; Write the installation path into the registry
	WriteRegStr HKLM SOFTWARE\BZEdit${VER_MAJOR}${VER_MINOR} "Install_Dir" "$INSTDIR"

	; Write the uninstall keys for Windows
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BZEdit${VER_MAJOR}${VER_MINOR}" "DisplayName" "BZEdit(remove only)"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BZFEdit${VER_MAJOR}${VER_MINOR}" "UninstallString" '"$INSTDIR\uninstall.exe"'
	WriteUninstaller "uninstall.exe"
SectionEnd

; optional sections
Section "Start Menu Shortcuts"
	; Main start menu shortcuts
	SetOutPath $INSTDIR
	CreateDirectory "$SMPROGRAMS\BZEdit${VER_MAJOR}${VER_MINOR}"
	CreateShortCut "$SMPROGRAMS\BZEdit${VER_MAJOR}${VER_MINOR}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
	CreateShortCut "$SMPROGRAMS\BZEdit${VER_MAJOR}${VER_MINOR}\BZEdit ${VER_MAJOR}${VER_MINOR}.lnk" "$INSTDIR\BZEdit32.exe" "" "$INSTDIR\bBZEdit32.exe" 0 
SectionEnd

Section "Quick Launch Shortcuts"
	; shortcut in the "quick launch bar"
	SetOutPath $INSTDIR
	CreateShortCut "$QUICKLAUNCH\BZEdit${VER_MAJOR}${VER_MINOR}.lnk" "$INSTDIR\BZEdit32.exe" "" "$INSTDIR\BZEdit32.exe" 0 
SectionEnd

; uninstall stuff
UninstallText "This will uninstall BZEdit. Please hit next to continue with the removal."

; special uninstall section.
Section "Uninstall"
	; remove registry keys

	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BZEdit"
	DeleteRegKey HKLM SOFTWARE\BZEdit
	; remove files

	Delete $INSTDIR\BZEdit32.exe
	Delete $INSTDIR\BZEdit32.exe.manifest
	Delete $INSTDIR\PocketEngine.dll
	Delete $INSTDIR\TextureMngr.dll

	Delete $INSTDIR\data\*.*

	; MUST REMOVE UNINSTALLER, too
	Delete $INSTDIR\uninstall.exe

	; remove shortcuts, if any.
	Delete "$SMPROGRAMS\BZEdit${VER_MAJOR}${VER_MINOR}\*.*"
	Delete "$$QUICKLAUNCH\BZEdit{VER_MAJOR}${VER_MINOR}.lnk"

	; remove directories used.
	RMDir "$SMPROGRAMS\BZEdit${VER_MAJOR}${VER_MINOR}"
	RMDir "$INSTDIR\data"
	RMDir "$INSTDIR"
SectionEnd

; eof
