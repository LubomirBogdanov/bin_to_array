Description  
===================================================================     
A simple program in C that reads a binary file and outputs a text  
file with extension .h that contains an array declaration in C.  

INSTALL  
===================================================================     
git clone https://github.com/LubomirBogdanov/bin_to_array  
cd bin_to_array  
make clean  
make  
  
Usage  
===================================================================     
cd bin_to_array/debug  
bin_to_array [path_and_file_name_in] [path_and_file_name_out]  
  
path_and_file_name_in - the binary file to be converted to a C array (bin, jpg, png, pdf, etc).  
path_and_file_name_out - the text file that will hold an array of bytes in C.  
  
Example:  
bin_to_array /home/user/Desktop/pic.jpg /home/user/Desktop/pic.h  

