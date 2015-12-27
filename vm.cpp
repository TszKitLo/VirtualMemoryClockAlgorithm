#include <iostream>
#include <string>
#include <bitset>
#include <fstream>

int main(int argc, char *argv[]){
	int i,j;
	/*Initialize virtual memory*/
	unsigned int ram[4];//physical page 
	unsigned int use[4];//use bit

	for(i = 0; i < 4; i++){
		ram[i] = 0;
		use[i] = 0;
	}
		
	/*page write and read*/

	std::ifstream fs;
	fs.open(argv[1]);
	std::ofstream of;
	of.open("vm-out.txt");
	unsigned int info, vm_page;
	//unsigned int offset;
	int counter = 1;
	int write_pos = -1;
	int stop = 0;
	int match = 0;
	int pointing = 1;
	int fulluseChecker;

	while(fs >> std::hex >> info){
		stop = 0;
		write_pos = -1;
		match = 0;

		//offset = info & 0xFFF;
		vm_page	= (info & 0xFFFFF000) >> 12;

		if(counter == 17)	//initial case
		{
			ram[0] = vm_page;
			use[0] = 1;
			of << std::hex << ram[0] << "000" << std::endl;
		}

		if(counter > 17) //counter > 16
		{

			for(i = 0; i<4;i++)//looking for the same address
			{
				if(ram[i] == vm_page)
				{
					match = 1;
					use[i] = 1;
					//pointing++;
					if(pointing == 4)
						pointing = 0;

					// if(use[i] == 0)
					// {
					// 	use[i] = 1;
					// }

					// else	//use[i] == 1
					// {
					// 	use[i] = 1;
					// 	if(i == pointing)
					// 	pointing++;

					// 	if(pointing == 4)
					// 	pointing = 0;
					// }					

				}
			}//for - check if its in ram

			if(match == 0)//address not match
			{
				
				while(stop != 1)//write by clock replacement
				{	

					if(use[pointing] == 0)
					{
			
						write_pos = pointing;
						pointing++;
						ram[write_pos] = vm_page;
						use[write_pos] = 1;
						stop = 1;
					}

					else
					{
						use[pointing] = 0;
						pointing++;
						fulluseChecker = 1;
						for(int i = 0; i < 4; i++){
							if(use[i] == 0)
							{
								fulluseChecker = 0;
								break;
							}
						}
			
						if(fulluseChecker == 1)
						{
							write_pos = -1;//all lines are used
							stop = 1;
						}
						// if(pointing == 4)
						// 	stop = 1;
					}

					if(pointing == 4)
						pointing = 0;

				}//clock goes through to find where to write. If it writes somewhere, the clock stops.
			

				if(write_pos == -1)//all lines are using
				{	
					//std::cout << "Banana ";
					for(i = 0; i<4; i++)
						use[i] = 0;
					ram[0] = vm_page;
					use[0] = 1;
					pointing = 1;
				}


			}
			/*------------print out result--------------*/
			for(j=0 ; j<4; j++)	//print out the result
			{
				if(ram[j] != 0)
				{
					if(counter == 18)
					{
						if(j != 1)
							of << std::hex << ram[j] << "000 ";
						else
							of << std::hex << ram[j] << "000";
					}
					else if(counter == 19)
					{
						if(j != 2)
							of << std::hex << ram[j] << "000 ";
						else
							of << std::hex << ram[j] << "000";
					}
					else
					{
						if(j != 3)
							of << std::hex << ram[j] << "000 ";
						else
							of << std::hex << ram[j] << "000";
					}
				}


			}
			
			// for(j=0;j<4;j++)
			// 	std::cout << " " << use[j] << " ";
			
			
			
			// /*^^^^^^^^^^^^print out result^^^^^^^^^^^*/
			// //std::cout << " Was trying to add: " << vm_page << " current pointing: " << pointing;
			of << std::endl;
		}

		counter++;
	}

	fs.close();
	of.close();

	return 0;
}