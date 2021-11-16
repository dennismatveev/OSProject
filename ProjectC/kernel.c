void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*,int);
void readFile(char*,char*,int*);
void handleInterrupt21(int,int,int,int);
void executeProgram(char*);
void terminate();


void main(){

	makeInterrupt21();
	interrupt(0x21,4,"shell",0,0);

/*
	makeInterrupt21();
	interrupt(0x21,5,"tstpr2",0,0);
*/
/*
	makeInterrupt21();
	interrupt(0x21,4,"tstpr1",0,0);
*/
/*
	char buffer[13312];
	int sectorsRead;
	makeInterrupt21();
	interrupt(0x21, 3, "messag", buffer, &sectorsRead);
	if (sectorsRead>0)
		interrupt(0x21, 0, buffer, 0, 0);

	else
		interrupt(0x21, 0, "messag not found\r\n", 0, 0);

	while(1);
*/
/*
	char line[80];
	makeInterrupt21();
//	interrupt(0x21,0,0,0,0);
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
*/
/*
	char buffer[512];
	readSector(buffer,30);
	printString(buffer);
*/
/*
	char line[80];
	printString("Enter a line: ");
	readString(line);
//	printString("Entered Line is: ");
	printString(line);
*/

}

void terminate(){
	char shellname[6];
	shellname[0]='s';
	shellname[1]='h';
	shellname[2]='e';
	shellname[3]='l';
	shellname[4]='l';
	shellname[5]='\0';

	executeProgram(shellname);

}

void executeProgram(char* name){
	char buffer[13312];
	int i, address;

	readFile(name, buffer,0);
	for(i = 0; i <13312; i++){
		putInMemory(0x2000,i,buffer[i]);
	}
	launchProgram(0x2000);

}

void readFile(char* fileName, char* buffer, int* sectorsReadptr){
	char dir[512];
	int fileEntry, fileExists, letter, s, sector;
	readSector(dir,2);

	for(fileEntry = 0;fileEntry<512;fileEntry +=32){
		fileExists = 1;
		for(letter=0;letter<6;letter++){
			if(dir[fileEntry+letter]!=fileName[letter]){
				fileExists = 0;
				break;
			}
		}
		if(fileExists==1){
			 break;
		}
	}
	if(fileExists==0){
		*sectorsReadptr=0;
		return;
	}

	for(s = 6;s<26;s++){
		sector = dir[fileEntry+s];
		if(sector==0)
			 break;
		readSector(buffer, sector);
		buffer= buffer+512;
		*sectorsReadptr = *sectorsReadptr+1;
	}

}

void printString(char* chars){
	int i = 0;
	while(chars[i] != '\0'){
		printChar(chars[i]);
		i++;
	}
}

void printChar(char c){
	interrupt(0x10,0xe*256+c,0,0,0);
}

void readString(char* chars){
	int i =0;
	int in = 0x0;
	while(in != 0xd){
		in = interrupt(0x16,0x0,0x0,0x0,0x0);
		printChar(in);
		if(in == 0x8 && i>0){
			printChar(0x0);
			printChar(in);
			i--;
		}
		else{
			chars[i] = in;
			i++;
		}
	}
	chars[i] = 0xa;
	chars[i+1] = 0x0;
	printChar(0xd);
	printChar(0xa);
}

void readSector(char* buffer,int sector){
	interrupt(0x13,2*256+1,buffer,0*256+sector+1,0*256+0x80);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
//	printString("Hello World");

	if(ax==0) printString(bx);
	else if(ax==1) readString(bx);
	else if(ax==2)readSector(bx,cx);
	else if(ax==3) readFile(bx,cx,dx);
	else if (ax==4) executeProgram(bx);
	else if (ax==5) terminate();
	else printString("Error not a valid number!");
}
