#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

#define width 13
#define height 22
#define speed 800

int pi[17];
unsigned char pos[width * height];
char tets[17];
int score=0;
int bag[7];
int level=0;

void Gotoxy(int x,int y) {
	COORD coord; coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

char tetromino[7][4][4] ={
	{
		"..x.",
		"..x.",
		"..x.",
		"..x.",
	},
	{
		"....",
		".xx.",
		".xx.",
		"....",
	},
	{
		"..x.",
		".xx.",
		".x..",
		"....",
	},
	{
		".x..",
		".xx.",
		"..x.",
		"....",
	},
	{
		".x..",
		".xx.",
		".x..",
		"...."
	},
	{
		".x..",
		".x..",
		".xx.",
		"...."
	},
	{
		"..x.",
		"..x.",
		".xx.",
		"...."
	}
};

int rotate(int x,int y,int r){
	int i=0;
	switch(r)
	{
		case 1:
			i=y*4 +x;
			break;
		case 2:
			i=12+y-(x*4);
			break;
		case 3:
			i=15-(y*4)-x;
			break;
		case 4:
			i = 3 -y +(4*x);
			break;
	}
	return i;
}

void create_piece(int currentx,int currenty,int ti,int r){
	int s=0;
	for (int y=0; y<4; y++){
		for(int x=0; x<4; x++){
			int i=(currenty + y)*width +(currentx+x);
			int j=rotate(x,y,r);
			pi[s]=i;
			s++;
			tets[j]=tetromino[ti][y][x];
		}
	}
}

void copy_piece(){
	for(int j=0; j<16; j++){
		if(tets[j]=='x'){
			pos[pi[j]]=tets[j];
		}
	}
}

void is_border() {
	int y = 0;
	for (y = 0; y < height; y++) {
		int x = 0;
		for (x=0; x<width; x++)	{
			if ( x== 0 || x == width - 2 || y == height - 1) {
				pos[y * width + x] =176;
			}
			else if(pos[y * width + x] != 'x') {
				pos[y * width + x]='.';
			}
		}
		pos[y*width + width-1]='\n';
	}
}

void release(){
	for(int i=0; i<16; i++){
		if(pos[pi[i]]=='x'){
			pos[pi[i]]='.';
		}
	}
}

void store(){
	for (int i=0; i<16; i++){
		if(pos[pi[i]]=='x'){
			pos[pi[i]]=254;
		}
	}
}

int check(int currentx,int currenty,int ti,int rotation){
	create_piece(currentx,currenty,ti,rotation);
	for(int i=0; i<16; i++){
		if(tets[i]!='.' && pos[pi[i]]!='.'){
			return 0;}}
	return 1;
}

void check_full_line(int currenty){
	int count=0;
	for(int y=0; y<4; y++){
		if((currenty+y) < (height-1)){
			int flag_line=1;
			for(int x=1; x<width-2; x++){
				flag_line &=(pos[(currenty+y)*width + x]!='.');
			}
			if(flag_line){
				for(int x=1; x<width-2; x++){
					pos[(currenty+y)*width + x]='.';
				}
			}
			if(flag_line){
				count++;
				for(int yy=currenty+y; yy>1; yy--){
					for(int x=1; x<width-2; x++){
						pos[(yy)*width + x]=pos[(yy-1)*width +x];
					}
				}
			}
		}
	}				
	level+=count;
	if(count>0){
		score+=(count*75+(count-1)*25);
	}
}

void init_7_bag(){
	for (int i=0; i<7; i++){
		bag[i]=rand()%7;
	}
}

int pick_from_bag(){
	int ti=bag[0];
	for(int i=0; i<6; i++){
		bag[i]=bag[i+1];
	}
	bag[6]=rand()%7;
	return ti;
}

void print_next_piece(){
	int x=width+1;
	int y =4;
	for(int i=0; i<4; i++){
		Gotoxy(x,y);
		for(int j=0; j<4; j++){
			if(tetromino[bag[0]][i][j]=='x'){
				unsigned char c=254;
				printf("%c",c);}
			else{printf(" ");}
		}
		y++;
	}
}

int play_again(){
	char c;
	do{
		Gotoxy(0,height+3);
		printf("Play_again?[y/n]:");
		scanf("%c",&c);
	}while(c!='y' && c!='n');
	if(c=='y'){return 1;}
	else {return 0;}
}

void welcome(){
	is_border();
	int currentx=width/2 -2;
	int currenty=2;
	Gotoxy(0,0);
	printf("%s",pos);
	Sleep(500);
	create_piece(currentx,currenty,4,3);
	copy_piece();
	Gotoxy(0,0);
	printf("%s",pos);
	release();
	Sleep(500);
	Gotoxy(0,0);
	printf("%s",pos);
	Sleep(500);
	create_piece(currentx,currenty,2,2);
	copy_piece();
	Gotoxy(0,0);
	printf("%s",pos);
	release();
	Sleep(500);
	Gotoxy(0,0);
	printf("%s",pos);
	Sleep(500);
	create_piece(currentx,currenty,0,1);
	copy_piece();
	Gotoxy(0,0);
	printf("%s",pos);
	release();
	Sleep(500);
	Gotoxy(0,0);
	printf("%s",pos);
	Sleep(500);
}

int main() {
	system("cls");
	int play_flag=1;
	do{
		system("cls");
		welcome();
		srand(time(0));
		init_7_bag();
		score=0;
		char key;
		is_border();
		Gotoxy(0,0);
		printf("%s",pos);
		int gameover=1;
		int cur_speed=speed;
		int lvl=0;
		while(gameover==1){
			int currentx=width/2-2;
			int currenty=0;
			int rotation=1;
			int ti=pick_from_bag();
			if(level!=0 && level/10==1 && lvl<18){ 
				if(level>=10){
					level=level%10;}
				lvl++;
				if(lvl<5){cur_speed -=80;}
				else if (lvl<10){cur_speed -=40;}
				else{cur_speed -=4;}
			}
			Gotoxy(width+1,2);
			printf("NEXT");
			print_next_piece();
			Gotoxy(0,height);
			printf("\nLEVEL: %d",lvl);
			printf("\nSCORE: %d",score);
			while(1){
				clock_t start_time = clock();     
				do{
					if (_kbhit() ){
						int esc_flag=1;
						//_getch();
						//arrow keys need 2 getch(one for esc char & one for the value)
						key=_getch();
						if(key==0 || key==224){key=256+_getch();}
						if (key==72){
							rotation++;
							if(rotation==5){
								rotation=1;
							}
							if(!check(currentx,currenty,ti,rotation)){
								rotation--;}
						}
						else if(key==75){
							currentx--;
							if(!check(currentx,currenty,ti,rotation)){currentx++;}
						}
						else if(key==77){
							currentx++;
							if(!check(currentx,currenty,ti,rotation)){currentx--;}
						}
						else if(key==27){
							if(esc_flag==1){
								esc_flag=0;
								_getch();}
							else{break;}
						}
						else if(key==80){break;}
						else{continue;}
						create_piece(currentx,currenty,ti,rotation);
						copy_piece();
						Gotoxy(0,0);
						printf("%s",pos);
						release();
					}
				}while(clock()<start_time + cur_speed);
				if(check(currentx,currenty+1,ti,rotation)){currenty++;}
				else{
					create_piece(currentx,currenty,ti,rotation);
					gameover=check(currentx,currenty,ti,rotation);
					copy_piece();
					store();
					Gotoxy(0,0);
					printf("%s",pos);
					check_full_line(currenty);
					break;
				}
				create_piece(currentx,currenty,ti,rotation);
				copy_piece();
				Gotoxy(0,0);
				printf("%s",pos);
				release();
			}
			score +=25;
		};
		Gotoxy(1,10);
		printf(".GameOver");
		Sleep(1000);
		play_flag=play_again();
	}while(play_flag);
	return 0;
}
