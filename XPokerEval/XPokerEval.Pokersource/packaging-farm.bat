rem http://gna.org/projects/packaging-farm/

echo !ACTION!
if !ACTION!==get (
	cd !FARMROOT!

        rem ######################################################################
        rem # EDIT
	set MODULE=poker-eval
        rem ######################################################################

	set CVSDIR=_cvs\!MODULE!
	if not exist !CVSDIR! mkdir !CVSDIR!
	cd !CVSDIR!
	set CVS_RSH=TortoisePlink.exe
	PATH=!PATH!;C:\Program Files\TortoiseCVS

        rem ######################################################################
        rem # EDIT
	cvs -d :pserver:anonymous@cvs.gna.org:/cvs/pokersource co poker-eval || echo FARMFAILED
        rem ######################################################################

	cd !FARMROOT!
	if not exist _build\!MODULE! mkdir _build\!MODULE!
	set CVSDIR=_cvs/!MODULE!
	set BUILDDIR=_build/!MODULE!

        rem ######################################################################
        rem # EDIT
	rem rsync -av --exclude=CVS !CVSDIR! !BUILDDIR! || echo FARMFAILED
	rsync -av --exclude=CVS --delete !CVSDIR!/ !BUILDDIR!/ || echo FARMFAILED
        rem ######################################################################

) else if !ACTION!==build (
	cd !FARMROOT!
	set MODULE=poker-eval

        rem ######################################################################
        rem # EDIT
	rem set CONFIGURATION=Master
	set CONFIGURATION=Release
        rem ######################################################################

        rem ######################################################################
        rem # EDIT
	set SOLUTION=_build\!MODULE!\poker-eval\poker-eval.sln
        rem ######################################################################

        rem ######################################################################
        rem # EDIT
	rem set VISUALSTUDIOROOT="C:\\Program Files\\Microsoft Visual Studio .NET 2003"
	set VISUALSTUDIOROOT="C:\\Apps\\MsVs2003"
        rem ######################################################################
	PATH=!PATH!;!VISUALSTUDIOROOT!\\Common7\Tools
	vsvars32.bat || echo FARMFAILED

        rem ######################################################################
        rem # EDIT
	rem set MYVAR=NEEDEDFORBUILD
        rem ######################################################################

	devenv.com !SOLUTION! /build !CONFIGURATION! || echo FARMFAILED
) else if !ACTION!==master (
	cd !FARMROOT!
	set MODULE=poker-eval

        rem ######################################################################
        rem # EDIT
        rem # Name of the .exe file and the name of the directory
	set TOMODULE=poker-eval
        rem ######################################################################

	if exist _master\!MODULE! rmdir /s /q _master\!MODULE!
	mkdir _master\!MODULE!\!TOMODULE!
	set BUILDDIR=_build/!MODULE!
	set MASTERDIR=_master/!MODULE!
	set SRCDIR=!BUILDDIR!
	set DISTDIR=!MASTERDIR!/!TOMODULE!

	set ROOT=!SRCDIR!

	set TARGET=!DISTDIR!
	set TARGETWIN=_master\!MODULE!\!TOMODULE!
	set EXCLUDE=--exclude=CVS --exclude='*~' --exclude='*keepme*' --delete
	set RSYNC=rsync -v !EXCLUDE!

	!RSYNC! -a --include='*.exe' --exclude='*' !BUILDDIR!/poker-eval/examples/release/ !TARGET!/examples/ || echo FARMFAILED
	!RSYNC! -a --include='*.h' --exclude='*' !BUILDDIR!/poker-eval/include/ !TARGET!/include/ || echo FARMFAILED
	!RSYNC! -a --include='*.h' --exclude='*' !BUILDDIR!/poker-eval/include/inlines/ !TARGET!/include/inlines/ || echo FARMFAILED
	mkdir !TARGET!/bin
	!RSYNC! !BUILDDIR!/poker-eval/Release/poker-eval.lib !TARGET!/bin || echo FARMFAILED
	!RSYNC! !BUILDDIR!/poker-eval/AUTHORS !BUILDDIR!/poker-eval/ChangeLog !BUILDDIR!/poker-eval/LICENSE !BUILDDIR!/poker-eval/README !TARGET! || echo FARMFAILED

	PATH=%PATH%;c:\Program Files\NSIS
	set NSI=!FARMROOT!\_build\!MODULE!\!MODULE!\poker-eval.nsi
	cd _master\!MODULE!
	
	makensis /NOCD !NSI! || echo FARMFAILED

) else if !ACTION!==collect (
	cd !FARMROOT!
	set MODULE=poker-eval
        rem ######################################################################
        rem # EDIT
        rem # Name of the .exe file and the name of the directory
	set TOMODULE=poker-eval
        rem ######################################################################
	set MASTERDIR=_master/!MODULE!
	echo RSYNC_DESTINATION !RSYNC_DESTINATION!
 	rsync -av !MASTERDIR!/ !RSYNC_DESTINATION!/ || echo FARMFAILED
)
ping -n 5 127.0.0.1
exit
