#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<math.h>
#include<vector>
#include<unordered_map>
using namespace std;


class color_table
{
	
    public:
    unsigned int key, R, G, B;
	color_table(unsigned int a, unsigned int b, unsigned int c, unsigned int d): key(a), R(b), G(c), B(d){}
	
};

class pixel
{public:
	unsigned int key;
	pixel(unsigned int k) :key(k){}
};

class pixel_row
{
public:
	unsigned int nuofpixels;
	vector<pixel> px;
	pixel_row(unsigned int a, vector <pixel>b)
	{
		nuofpixels = a;
		px = b;
	}

};

int main(int argc, char *argv[]) {
	unsigned int size;
	 char * memblock;
	 unsigned int * memblock2;
	 

	ifstream file;
	char* input_file = argv[1];
	char* output_file = argv[2];
	cout <<"Input file is "<< input_file << endl;
	cout <<"output file is"<< output_file << endl;
		file.open(input_file, ios::in | ios::binary );
		fstream opfile;
		opfile.open(output_file, ios::out | ios::binary);

		//reading the file and storing everything in a buffer
		if (file.is_open())
		{
			file.seekg(0, ios::beg);
			int beginning = file.tellg();
			file.seekg(0,ios::end);
			int end = file.tellg();
			size = end - beginning;
			memblock = new char[size];
			memblock2 = new unsigned int[size];
			
			file.seekg(0, ios::beg);
			file.read(memblock, size);
			file.close();
			
			for (int i = 0; i < size; i++)
			{
				file >> memblock[i];
				memblock2[i]= (unsigned int)(unsigned char)memblock[i];
				
				
			}
			
			
			
		}
		else { cout << "Unable to open file"; return -1; }
		//global counter
		int i=4;
		//getting the file name
		int filenamesize=0;
		
			filenamesize += ((memblock2[i++]*(pow((int)16, 2))));
			
			filenamesize += ((memblock[i++] * (pow((int)16, 0))));
			
		
		//store file name
		char *filename = new char[filenamesize];
		for (int j = 0; j < filenamesize; j++)
		{
			filename[j] = (char)memblock2[i];
			i++;
			
			
		}
		filename[filenamesize] = '\0';
		cout << "filename is "<<filename<<endl;
		i++;
		int colortablesize = 0;
		
		colortablesize += (memblock2[i++] * (pow((int)16, 2)));
		
		colortablesize += (memblock2[i++] );
		
		
	
		unordered_map<unsigned int, color_table> ctable;
		for (int j = 0; j < colortablesize; )
		{
			i = i + 3;
			j = j + 3;

			ctable.emplace(memblock2[i], color_table(memblock2[i], memblock2[i + 1], memblock2[i + 2], memblock2[i + 3]));
			i = i + 4;
			j = j + 4;
			
		}
		
		i++;
		
		
		
		
		//getting pixell data

		int pixeldatasize = 0;
		int pixelgroupsize = 0;
		
		
		pixeldatasize += memblock2[i++] * (pow((int)16, 2));
		
		pixeldatasize += memblock2[i++];
		
		vector<pixel>pxdata;
		vector<pixel_row>pxrowdata;
		int rowpixeldatasize = 0;
		int j=0;
		while (j < pixeldatasize)
		{
			
			if (memblock2[i] == 6)
			{	
				
				pxdata.clear();
				i++; j++;
				rowpixeldatasize = 0;
				rowpixeldatasize += (memblock2[i++] * (pow((int)16, 2))); j++;
				rowpixeldatasize += (memblock2[i++] * (pow((int)16, 0))); j++;
				
				int l = 0;
				for (int k = 0; k < rowpixeldatasize; )
				{
					
					if (memblock2[i] == 8)//singlepixel
					{
						
						i = i + 3;
						k = k + 3;
						j = j + 3;
						
						pxdata.push_back(pixel((unsigned int)memblock2[i]));
						i++;
						k++;
						j++;
						l++;
					}
					if (memblock2[i] == 7)//pixelgroup
					{
						
						i = i + 3;
						k = k + 3;
						j = j + 3;
						pixelgroupsize = memblock2[i];
						i++;
						j++;
						k++;
						for (int k = 0; k < pixelgroupsize; k++)
						{
							pxdata.push_back(pixel((unsigned int)memblock2[i]));
							l++;
						}
						i++;
						j++;
						k++;



					}


				}pxrowdata.push_back(pixel_row(l, pxdata));
			}
		}
	
		cout << endl;
		//writing to output file
		cout<<"Writing to output file .........."<<endl;
		opfile << filename<<endl<<endl;
		opfile << pxrowdata.size() << " " << pxdata.size()<<endl<<endl;
		
		
		for (auto it1 = pxrowdata.begin(); it1 != pxrowdata.end(); it1++)
		{
			for (auto it3 = it1->px.begin(); it3 != it1->px.end(); it3++)
			{
				
				
				
				auto it4 = ctable.find(it3->key);
			
				opfile.width(2);
				opfile.fill('0');
				opfile<<(hex)<< it4->second.R << " ";
				opfile.width(2);
				opfile.fill('0');
				opfile  << (hex)<<it4->second.G<<" ";
				opfile.width(2);
				opfile.fill('0');
				opfile <<(hex)<< it4->second.B << " ";
			}opfile << endl;
		}
		opfile.close();
		
	
	char ch;
	cin >> ch;
	return 0;
}
