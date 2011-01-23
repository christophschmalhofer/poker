@echo off


echo Compiling PSim wrapper
javac psim\PSim.java
javac psim\SimResults.java


echo Generating C header file
javah -d PSim PSim.PSim


echo -------------------- 


echo Attempting to compile the PSim Java Test.
javac PSimTest.java


echo Attempting to run the PSim Java Test.
echo -------------------- 
java -Djava.library.path=PSim PSimTest


pause
