main(){
	char* userCommand[20];
	while(1){
		syscall(0,"SHELL>",0);
		syscall(1,userCommand,0);
		if(userCommand != 0x0){
			syscall(0,"\n",0);
			syscall(0,"Bad Command!",0);
			syscall(0,"\n",0);
		}
	}
}
