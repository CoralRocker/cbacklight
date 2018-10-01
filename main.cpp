/*
 * Author: Gaultier Delbarre
 * Written: 2018
 * BLIGHT --------
 * A program to change the screen brightness of linux and linux-based systems. Written in C++.
 * Uses fstreams to write to file. Must be run as root or sudo.
 *
 * USAGE ---------
 * To set brightness
 * 	->blight -s [brightness percent]
 * To find the current brightness
 * 	->blight -q
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
	//cmd.append(" 2&>1");
	char var[40];
	string data;
	fp = popen(cmd.c_str(), "r");
	while (fgets(var, 40, fp) != NULL)
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
/*	
	if(argc <= 1)
	{
		cout << "ERROR: NOT ENOUGH ARGUMENTS" << endl
			 << "MUST HAVE AT LEAST ONE ARGUMENT" << endl;
		return 0;
	}

	if(geteuid() != 0)
	{
		cout << "MUST BE ROOT" << endl;
		return -1;
	}
*/
	checkUse(argc);
	istringstream(checkOutput("cat /sys/class/backlight/intel_backlight/max_brightness")) >> max_b;
	istringstream(checkOutput("cat /sys/class/backlight/intel_backlight/brightness")) >> cur_b;
	pct_b = cur_b*100/max_b;
	
	while ((option = getopt(argc, argv, "s:q")) != -1)
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
				break;
		}
	}

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
