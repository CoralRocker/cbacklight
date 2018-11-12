/*
 * Author: Gaultier Delbarre
 * Written: 2018
 * BLIGHT --------
 * A program to change the backlight strength of systems which store backlight info in the 
 * /sys/class/intel_backlight/ directory. Written in C++. Uses fstreams to write to file. 
 * Must be run as root or sudo, as it modifies files in /sys/class. Am working on an 
 * implementation in C, and am cleaning up the mess of code which is this project. 
 *
 * USAGE ---------
 * To set brightness
 * 	->blight -s [brightness percent]
 * To find the current brightness
 * 	->blight -q
 * To increment brightness
 * 	->blight -i [percent]
 * To decrement brightness
 * 	->blight -d [percent]
 *
 * Must be run with an argument.
 */
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <string>

using namespace std;

int checkUse(int args)
{
	if(args <= 1)
	{
		cout << "Must Have At Least One Argument" << endl;
		exit(-1);
	}
	if(geteuid() != 0)
	{
		cout << "Must Be Root Or Sudo To Run Program" << endl;
		exit(-1);
	}
	return 0;
}	

string checkOutput(string cmd)
{
	FILE *fp;
	char var[16];
	string data;
	fp = popen(cmd.c_str(), "r");
	while (fgets(var, 16, fp) != NULL)
	{
		data.append(var);
	}
	pclose(fp);
	return data;
}
int main(int argc, char **argv)
{
	int option = -1, i_pct;
	float pct_b, cur_b, max_b;
	char *cvalue = NULL;
	bool inc = false;
	bool dec = false;
	bool set = false;
	checkUse(argc);
	istringstream(checkOutput("cat /sys/class/backlight/intel_backlight/max_brightness")) >> max_b;
	istringstream(checkOutput("cat /sys/class/backlight/intel_backlight/brightness")) >> cur_b;
	pct_b = cur_b*100/max_b;
	
	while ((option = getopt(argc, argv, "i:d:s:q")) != -1)
	{
		switch (option)
		{
			case 'q':
				cout << "Current Brightness: " << cur_b << endl 
					<< "Maximum Brightness: " << max_b << endl
					 << "Percent Brightness: " << pct_b << "%" << endl;
				return 0;
				break;

			case 's':
				cvalue = optarg;
				set = true;
				break;
			case 'i':
				cvalue = optarg;
				inc = true;
				break;
			case 'd':
				cvalue = optarg;
				dec = true;
				break;
		}
	}

	if(inc)
	{
		pct_b = atof(cvalue)/100 * max_b;
		//cout << "INC BY: " << pct_b << endl;
		cur_b += pct_b;
		if((int)cur_b > max_b)
			cur_b = max_b;
		//cout << "INC BY: " << (int)cur_b << endl;
		fstream file;
		file.open("/sys/class/backlight/intel_backlight/brightness", ios::out);
                file << to_string((int)cur_b) << endl;

	}

	if(dec)
	{
		pct_b = atof(cvalue)/100 * max_b;
		//cout << "DEC BY: " << pct_b << endl;
		cur_b -= pct_b;
		fstream file;
		file.open("/sys/class/backlight/intel_backlight/brightness", ios::out);
		file << to_string((int)cur_b) << endl;
	}

	if(set)
	{
		if (!(atof(cvalue) > 100) && !(atof(cvalue) <= 0))
		{
			i_pct = (atof(cvalue)/100) * max_b;
			fstream file;
			file.open("/sys/class/backlight/intel_backlight/brightness", ios::out);
			file << to_string(i_pct) << endl;
		}else{
			cout << cvalue << " is not between 0 and 100" << endl;
			return 0;
		}

	}
}
