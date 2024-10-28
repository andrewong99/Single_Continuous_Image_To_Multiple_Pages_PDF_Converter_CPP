Single_Continuous_Image_To_Multiple_Pages_PDF_Converter_CPP is a single C++ file written using Notepad++.
You need to build this "main.cpp" file inside your system in order to use it.
It is better that you build it yourself from source code. I have a .exe file that is built but some systems will promt this as virus/trojan/security risk due to lack of signature and the way this .cpp file operates.
Its operation logic is to retrieve the single continuous image file then cut it into fixed page size then convert it into single page pdf, then save it in cache, eventually combine it into multiple page pdf file.
The way it operates includes the permissions to create file and delete temporary file, a trait that is easily misidentified as security risk, but you can check the source code itself that it is 100% safe and it only does the job it intends.

Notes when build from source:
1) Place main.cpp under C:\ directory.
2) Use g++ to build in MINGW64 Shell.
3) Use wxWidgets, openCV, libharu libraries.
4) Must use pacman -S mingw-w64-x86_64 inside MINGW64 Shell to install all the libraries, if not, cannot quote the libraries when build the c++ .exe file.
5) Cannot install separately under C:\ directory. Must use pacman command to install inside MINGW64 Shell in order to quote these libraries correctly.

Notes when using executable file (.exe) directly.
1) Open and select the single continuous image you intend to convert become multipage pdf file.
2) If you do not have that single continuous image file, you can use "save webpage as image" to save all webpage in a way as it displays, then convert to pdf later.
