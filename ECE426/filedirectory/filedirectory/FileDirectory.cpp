#include "FileDirectory.h"
#include <iostream>
using namespace std;
FileDirectory::FileDirectory() //initialize all entries in the fileDirectory and FAT16 to be 0, i.e.safe values.
{
	for (int i = 0; i < 256; i++) FAT16[i] = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 32; j++)
			fileDirectory[i][j] = 0;
}
bool FileDirectory::create(char filename[], int numberBytes)
{
	unsigned short int i, j, firstClusterAddress;
	unsigned short int numClusters = numberBytes / 4;//4 bytes per cluster
	//(1)	to check if there is an unused entry in the File Directory;  (i.e.the first character of the file name in the File Directory is zero).Return false if not true.

	//TODO: FIRST CHECK FILE DIR. FOR FILENAME...

	for(int i = 0; i < 256; i++)
	{
		if(FAT16[i] == 0x0000 || FAT16[i] == 0x0001)
		{
			firstClusterAddress = FAT16[i];
			break;
		}
		if (i == 255) {
			//No unused entries
			return false;
		}
	}
	//(2)	to check if there are enough unused clusters to store the file with the numberBytes.Return false if not true.
	if (numClusters <= 256 - i)
	{
		//Fill the FAT table with the appropriate linked list addresses
		for (j = firstClusterAddress; j < numClusters - 1; j++)
		{
			FAT16[j] = j + 1;
		}
		FAT16[j] = 0xFFF8;
		return true;
	}
	return false;
}

bool FileDirectory::dlete(char filename[])
{
	/*(0)	to check if the file to be deleted; filename[]; is in the Directory.If not; return false.
	(1)	to change the first character of the file name in the File Directory to be zero;
	(2)	to change all entries of the clusters of this file in the FAT to 1; i.e.; unused.*/
	return false;
}

bool FileDirectory::read(char filename[])
{
	unsigned short int firstClusterAddress, clusterAddress, numClusters, p = 0;
	unsigned short int storeClusterLocation[256];
	unsigned char fileData[1024];
	/*purpose: to read  of data from data[] array from the file with the specified file name.
	(0)	to check if the file to be deleted; filename[]; is in the Directory.If not; return false.*/
	for (int i = 0; i < 4; i++)//Each cluster has 4 bytes
	{
		for (int j = 0; j < 8; j++)
		{
			if (filename[j] == fileDirectory[i][j])
			{
				if (j == 7)
				{
					//(1) Match found, set first cluster address
					firstClusterAddress = (fileDirectory[i][27] << 8) + fileDirectory[i][26];
				}
				continue;
			}
			else
			{
				//Filename doesn't match, check next file
				if (i == 3)
				{
					//No match found
					return false;
				}
				break;
			}
		}
	}
	//(2)	use the first cluster address to get all the cluster addresses of this file from the FAT - 16;
	clusterAddress = firstClusterAddress;
	for(numClusters = 0; numClusters < 256 && clusterAddress < 0xfff8; numClusters++)
	{
		storeClusterLocation[numClusters] = clusterAddress;
		clusterAddress = FAT16[clusterAddress];

	}

	/*
	(2)	use the first cluster address to get all the cluster addresses of this file from the FAT - 16;
	(3)	use all the cluster addresses to read the data from the disk / flash memory.*/
	for (int i = 0; i <= numClusters; i++)
	{
		for (int j = 0; j < 4; j++)//Each cluster has 4 bytes
		{
			fileData[p] = data[(storeClusterLocation[i] << 2) + j]; // data[cluster *4bytes + offset inside of cluster
			++p;//File data index index increment
		}
	}
	return true;
}


bool FileDirectory::write(char filename[], int numberBytes, char data[], int year, int month, int day, int hour, int minute, int second)
{
	/*purpose: to write numberBytes bytes of data from data[] array into the file with the specified file name
	(0)	to look for the first unused entry(0 or 1) in the FAT - 16; and use it as the First Cluster Address.*/
	unsigned int firstClusterAddress;
	unsigned int allocatedBytes = 512;
	for(int i = 0; i < 256; i++)
	{
		if (FAT16[i] == 0 || FAT16[i] == 1)
		{
			firstClusterAddress = i;
			break;
		}
	}

	//(1)to look for the next unused entry(0 or 1) in the FAT - 16; and use it as the Next Cluster Address and write its value into the FAT - 16.
	while(allocatedBytes < numberBytes)
	{
		//(2)Repeat Step 2 until all clusters are found and the FAT - 16 is updated.
		for (int i = firstClusterAddress + 1; i < 255; i++)
		{
			if (FAT16[i] == 0 || FAT16[i] == 1)
			{
				firstClusterAddress = i;
				break;
			}
		}
		
	}
	//(3)	to write / update the file name; extension; date; time; file length and first cluster address into the first unused entry in the File Directory;*/
	unsigned short empty;
	for (int i = 0; i < 4; i++)
	{
		if (FAT16[i] == 0)
		{
			empty = i;
			break;
		}
	}
	//Write filename into the ith entry of the file directory
	for(int i = 0; i < 8; i++) fileDirectory[empty][i] = filename[i];

	//TODO: Write date into directory[24:25]

	//TODO: Write time in to directory[23:22]


	//Write first sector address into directory[27:26]
	fileDirectory[empty][27] = firstClusterAddress >> 8;
	fileDirectory[empty][26] = firstClusterAddress;

	//Write file length into directory
	fileDirectory[empty][31] = numberBytes >> 24;
	fileDirectory[empty][31] = numberBytes >> 16;
	fileDirectory[empty][31] = numberBytes >> 8;
	fileDirectory[empty][31] = numberBytes;

}

void FileDirectory::printClusters(char filename[])
{

}
/*purpose: to print all the clusters of a file.
(1)	to check if the file to be printed; filename[]; is in the Directory.If not; return false.
(2)	use the file name to get the file information from the File Directory; including the first cluster address;
(3)	use the first cluster address to get all cluster addresses from the FAT - 16;*/
void FileDirectory::printDirectory()
{
	unsigned short int date, time, firstClusterAddress;
	//prints all the  files of the directory.
	for (int i = 0; i < 4; i++)
	{
		//(1) If the file name is valid, print the filename, '.', the file extension
		//A valid filename doesn't begin with 0 (NULL)
		if (fileDirectory[i][0] != '0')
		{
			//Valid filename
			for (int j = 0; j < 8; j++)
			{
				cout << fileDirectory[i][j];
			}
			cout << '.';
			for (int j = 8; j < 3; j++)
			{
				cout << fileDirectory[i][j];
			}
			cout << "\t";

			//(2) Print date(DECODE)
			date = (fileDirectory[i][25] << 8) + fileDirectory[i][24];
			cout << (1980 + (date & 0xFE00) >> 9) << " ";
			//Print month
			cout << ((date & 0x01E0) >> 5) << " ";
			//Print day
			cout << (date & 0x001F);

			//Print time
			time = fileDirectory[i][23] << 8 + fileDirectory[i][22];

			//Hour
			cout << ((time & 0xF800) >> 11) <<":";

			//Minute
			cout << ((time & 0x07E0) >> 5) << ":";

			//Second
			cout << ((time & 0x001F) << 1);
		}
	}
	cout << endl;
}
/*prints all the  files of the directory.

(2)	use the first cluster address to get all cluster addresses from the FAT - 16;*/
void FileDirectory::printData(char filename[])
{
	unsigned short int firstSectorAddress;
	//Search for file in directory
	for (int i = 0; i < 4; i++)
	{
		//Check if name matches
		for (int j = 0; j < 8; j++)
		{
			if (filename[j] == fileDirectory[i][j])
			{
				if (j == 7)
				{
					//Match found
					//Get first sector address (fileDirectory[27:26])
					firstSectorAddress = (fileDirectory[i][27] << 8) + fileDirectory[i][26];
				}
				continue;
			}
			else
			{
				//Filename doesn't match
				break;
			}
		}
	}

	
}
/*prints the data of a file.
(1)	use the file name to get the file information from the File Directory; including the first cluster address;
(2)	use the first cluster address to get all cluster addresses from the FAT - 16;
(3)	use cluster address to read the data of the file.Use the file length to print these data in hexadecimal format.*/

