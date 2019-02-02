Systems Programming: Assignment One

A C Program which counts and ranks pairs of words 
by the number of times they appear in one or more text files.

By: Andrew Oakes
Email: andrew.oakes@wsu.edu

BUILDING INSTRUCTIONS ----------------------------------------------------

To build the program, type "make" in the terminal while in the program directory.

To build with debug information type "make debug" into the terminal while in the program directory.

To remove all object files type "make clean".

NOTE: to change the files which are used for the getWord code, change the variable called GET_WORD in the make file, 
and change the #define in the strHash.c file to refer to the correct header file. The way the makefile is setup, it will
attempt to build using ALL .c and .h files in whatever directory is given. 

USAGE INSTRUCTIONS -------------------------------------------------------

To run the program, first build the code by typing "make", then to run the new executable type the following: 

./wordpairs <-count> filepath1 <filepath2>...

<-count> - is a '-' followed by the number of results which should be displayed after running 
filepath1 - the path to the file that is to be read by the program.
<filepath2>... - One or more file paths which are optional 

SOURCES ------------------------------------------------------------------

Daniel J. Bernstein and his DJ2B hashing algorithm (found here http://www.cse.yorku.ca/~oz/hash.html)

Some useful videos on hashing and its various applications 

(MIT) https://www.youtube.com/watch?v=0M_kIqhwbFo
(Cryptography) https://www.youtube.com/watch?v=tLkHk__-M6Q&t=823s
