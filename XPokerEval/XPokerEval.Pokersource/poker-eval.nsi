; poker-eval.nsi
; based on
; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of makensisw.exe
; there. 

;--------------------------------

; The name of the installer
Name "poker-eval"

; The file to write
OutFile "poker-eval.exe"

; The default installation directory
InstallDir $PROGRAMFILES\poker-eval

;--------------------------------

; Pages

Page directory
Page instfiles

;--------------------------------

; The stuff to install
Section "" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File /r "poker-eval\*"


SectionEnd ; end the section