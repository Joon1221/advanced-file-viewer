================================================================================
                              Advanced File Viewer                                                      
================================================================================


INTRODUCTION
------------

This program is an advanced file viewer that takes features from a regular file
viewer but adds more advanced features such as viewing files in hexadecimal and
compressing files. The program was inspired by the lack of a convenient multiple 
purpose tool on the MacOS platform. 


FEATURES
--------

- Compressing and decompressing files
- Viewing files in ascii, hexadecimal, and binary
- Opening and viewing multiple files at once


STAFF
-----

Programmer : Joon Kang
Mentor : Samil Chai


WORK PERIOD
-----------

Start Date: 2017-06-02
End Date: 2017-09-07


INSTALLATION
------------

- Install wxWidget library
	- https://github.com/wxWidgets/wxWidgets/archive/v3.1.0.zip
- Compile wxWidget
	- Open terminal and paste the following
	- cd wxWidgets-3.1.0
	- mkdir build-cocoa-debug
	- cd build-cocoa-debug
	- ../configure --enable-debug
	- make
- Add header search paths 
	- in Xcode under Project Settings -> Build Settings, set header search 
	  paths to...
		- wxWidgets-3.1.0/build-cocoa-debug/lib
		- wxWidgets-3.1.0/include
- Add library search paths 
	- in Xcode under Project Settings -> Build Settings, set library search 
	  paths to...
		- wxWidgets-3.1.0/build-cocoa-debug/lib
- Link libraries 
	- in Xcode under Project Settings -> General, link all libraries
	  ending with .dylib in wxWidgets-3.1.0/build-cocoa-debug/lib


HOW TO USE
----------

- Build and Run the program using Xcode
- Navigate the left window as you would a normal file explorer
- Compress and Decompress files by selecting a file and pressing their 
  respective buttons (*Note that only .lz77 files can be decompressed)
- Selecting files will allow user to view content in ascii, hexadecimal, and 
  binary


LIBRARIES
---------

- wxWidget 3.1.0


COMPRESSING FILES
-----------------

The advanced file viewer uses the LZ77 algorithm to compress and decompress
files. Compressed files end with ".lz77" file extension and decompressed files
end with ".lz77.org". To prevent memory overflow when dealing with large files,
a sliding-window is used. 


PRIVACY
-------

Portions of source code have been removed for privacy purposes.



