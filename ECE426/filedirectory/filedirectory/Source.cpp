#include <iostream>
#include "FileDirectory.h"
using namespace std;


int main()
{
	FileDirectory f1 = FileDirectory();
	//1.	create and write a file; file1;  of 40 bytes;
	f1.create("file1", 40);
	//2.	create and write a file; file2;  of 200 bytes;
	f1.create("file2", 200);
	//3.	create and write a file; file3;  of 300 bytes;
	f1.create("file3", 300);
	//4.	create and write a file; file4;  of 500 bytes.
	f1.create("file4", 500);
	//5.	delete file2.
	f1.dlete("file2");
	//6.	create and write a file; file4;  of 500 bytes.
	if (f1.create("file4", 500))
		f1.write("file4", 500, "data", 2017, 1, 17, 13, 0, 0);

	

	return 0;
}