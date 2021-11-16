void print(char*);
void input(char*);
void parse(char*);

main(){
	char buffer[100];

	while(1){
		print("SHELL>");
		input(buffer);
		parse(buffer);
	}
}

void print(char* buffer){
	syscall(0,buffer,0);
}

void input(char*buffer){
	syscall(1,buffer,0);
}

void parse(char* line){
	char file[7];
	char buffer[13312];
	int i;
	int sectorsRead;

	for(i=0;i<6;i++)
		file[i] = line[i+5];

	file[i+1] = 0x0;

	if(line[0] =='t' && line[1] == 'y' && line[2] == 'p'
			&& line[3] == 'e' && line[4] == ' '){
		syscall(3,file,buffer,&sectorsRead);
		if(sectorsRead>0)
			print(buffer);
		else
			print("Error File Name Not Found!\r\n");
	}

	else if(line[0] =='e' && line[1] == 'x' && line[2] == 'e'
			&& line[3] == 'c' && line[4] == ' '){
		syscall(3,file,buffer,&sectorsRead);
		if(sectorsRead>0)
			syscall(4,file,0);
		else
			print("Error File Name Not Found!\r\n");
	}

	else
		print("Bad Command!\r\n");
}

