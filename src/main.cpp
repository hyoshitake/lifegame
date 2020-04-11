#include "DxLib.h"
#include <stdio.h>

#define HIGH 60+2
#define WIDE 74+2
#define TILE_WH 16
#define C_TIME 10

enum {SYOKI, INP, FINP, ACTION}game;

typedef struct data{
	int re, gr, bl;
	char life;
}DATA;

void SyokiTile(DATA [2][WIDE][HIGH]);
void DrawTile(DATA [WIDE][HIGH]);
void Input(DATA [WIDE][HIGH], DATA *);
void Write(DATA [2][WIDE][HIGH], int);
int Judge(DATA *, DATA, int);
void Grad(void);
void Pallet(void);
bool InputFile(int, char []);
DATA CheckColor(DATA [WIDE][HIGH], int, int);

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	ChangeWindowMode(TRUE);

	if( DxLib_Init() == -1 )		// ＤＸライブラリ初期化処理
		 return -1;					// エラーが起きたら直ちに終了

	SetDrawScreen(DX_SCREEN_BACK);
	SetTransColor(120, 195, 128);	//透明色設定
	SetMouseDispFlag( TRUE );
	
	DATA Tile[2][WIDE][HIGH];
	DATA pen;
	DATA Tempt[WIDE][HIGH];
	unsigned int animation;
	int flip, left_c = 0;
	int inph;
	char grad_flag = 0, fname[30];
	bool sorl = false;				//false = load, true = save

	game = SYOKI;
	pen.life = 1;
	
	while(ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){	//エラー無しorEscキーが押されてない場合

		ClsDrawScreen();					//画面クリア

		animation++;		

		switch(game){
			case SYOKI:
				SyokiTile(Tile);
				game = INP;
				break;
				
			case INP:
				flip = 0;
				if(CheckHitKey( KEY_INPUT_F ) == 1){
					inph = MakeKeyInput( 30, TRUE, TRUE, FALSE) ;
					SetActiveKeyInput(inph) ;
					sorl = false;
					game = FINP;
				}else if(CheckHitKey( KEY_INPUT_D) == 1){
					for(int i=1; i<HIGH-1; i++){
						for(int j=1; j<WIDE-1; j++){
							Tempt[j][i] = Tile[flip][j][i];
						}
					}
					inph = MakeKeyInput( 30, TRUE, TRUE, FALSE) ;
					SetActiveKeyInput(inph) ;
					sorl = true;
					game = FINP;
				}else{
					Input(Tile[0], &pen);
					if(CheckHitKey( KEY_INPUT_RETURN ) == 1){
						game = ACTION;
						animation = 1;
					}
				}

				break;
			
			case FINP:
				if(InputFile(inph, fname) == true){
					FILE *fp;
					char fch;
					if(sorl == false){
						if((fp = fopen(fname,"r")) != NULL){
							int i = 1,j = 1;
							while((fch = fgetc(fp)) != EOF){
								if(fch == '1'){
									Tile[0][j][i] = pen;
									i++;
								}else if(fch == '0'){
									Tile[0][j][i].life = 0;
									i++;
								}else if(fch == '\n'){
									j++;									
									i=1;
								}
							}
						}
					}else{
						fp = fopen(fname, "w");
						for(int i=1; i<HIGH-1; i++){
							for(int j=1; j<WIDE-1; j++){
								 fputc(Tempt[j][i].life+'0', fp);
							}
							fputc('\n',fp);
						}
					}
					fclose(fp);
				}
				break;
			
			case ACTION:
				flip = (animation/C_TIME)%2;
				if(animation%C_TIME == 0){
					Write(Tile, flip);
				}
				break;
		
		}

		if(game != FINP){
			if(CheckHitKey(KEY_INPUT_S) == 1)
				game = SYOKI;
	        else if(CheckHitKey(KEY_INPUT_A) == 1)
				game = INP;
	
			
			if(CheckHitKey( KEY_INPUT_G ) == 1 && (animation - left_c) > 5){
				grad_flag = 1 - grad_flag;
				left_c = animation;
			}
			
			//表示
			DrawTile(Tile[flip]);
			if(grad_flag >= 1)
				Grad();
			Pallet();
		}
     	ScreenFlip();                       //裏と表を回転
	}

	DxLib_End() ;							// ＤＸライブラリ使用の終了処理

	return 0;	
}

bool InputFile(int inph, char fname[])
{
	int white = GetColor(255,255,255);
	bool returnf = false;
	DrawString(10,10,"ファイル名を入力してください",white);
	if( CheckKeyInput( inph ) != 0 ){
		GetKeyInputString( fname, inph);
		ClearInputCharBuf();
		DeleteKeyInput( inph ) ;
		game = INP;
		returnf = true;
	}
	DrawKeyInputString( 10 , 30, inph) ;
	return returnf;
}

void Pallet(void)
{
	int i,j,k;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			for(k=0; k<3; k++){
				DrawBox(640-(k+1)*16, j*16+i*48, 640-k*16, (j+1)*16+i*48, GetColor(127*i,127*j,127*k), TRUE);
			}
		}
	}
}

void Grad(void)
{
	int i, black;
	black = GetColor(0,0,0);
	for(i=1; i<HIGH; i++){
		DrawLine( 0, i*TILE_WH, (WIDE-2)*TILE_WH, i*TILE_WH, black);
	}
	for(i=1; i<WIDE; i++){
		DrawLine( i*TILE_WH, 0, i*TILE_WH, (HIGH-2)*TILE_WH, black);
	}
}

void SyokiTile(DATA a[2][WIDE][HIGH])
{
	int i, j, k;
	for(k=0; k<2; k++){
		for(i=0; i<WIDE; i++){
			for(j=0; j<HIGH; j++){
				a[k][i][j].life = 0;
				a[k][i][j].re = 255;
				a[k][i][j].gr = 255;
				a[k][i][j].bl = 255;
			}
		}
	}
}

void Input(DATA a[WIDE][HIGH], DATA *pen)
{
	int mou_x, mou_y;
	GetMousePoint( &mou_x, &mou_y );
	if(mou_x > 0 && mou_x < 640 && mou_y > 0 && mou_y < 480){
		if(mou_x > 592 && mou_y < 144 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0){
			pen->re = mou_y/48 * 127;
			pen->gr = (mou_y/16)%3 * 127;
			pen->bl = (640-mou_x)/16 * 127;
		}
		if( (GetMouseInput() & MOUSE_INPUT_LEFT) != 0){
			int x = (mou_x/TILE_WH) + 1;
			int y = (mou_y/TILE_WH) + 1; 
			a[x][y] = *pen;
			if(pen->re == 254 && pen->gr == 254 && pen->bl == 254)
				a[x][y].life = 0;
		}else if((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0){
			int x = (mou_x/TILE_WH) + 1;
			int y = (mou_y/TILE_WH) + 1; 
			a[x][y].life = 0;
			a[x][y].re = 255;
			a[x][y].gr = 255;
			a[x][y].bl = 255;
		}
	}
}

DATA CheckColor(DATA old[WIDE][HIGH], int i, int j)
{
	int cl[3] = {0,0,0};
	int x,y;
	DATA ret;
	for(x=i-1; x<=i+1; x++){
		for(y=j-1; y<=j+1; y++){
			if(x != i || y != j){
				if(old[x][y].life == 1){
					cl[0] += old[x][y].re;
					cl[1] += old[x][y].gr;
					cl[2] += old[x][y].bl;
				}
			}
		}
	}
	ret.re = cl[0]/3;
	ret.gr = cl[1]/3;
	ret.bl = cl[2]/3;
	ret.life = 1;
	
	return ret;
}
				
void Write(DATA a[2][WIDE][HIGH], int count)
{
	int i, j, eight_life;
	int old = 1-count;
	int next = count;
	int tmp;

	for(j=1; j<HIGH-1; j++){
		tmp = a[old][0][j-1].life + a[old][1][j-1].life + a[old][2][j-1].life
			   + a[old][0][j+1].life + a[old][1][j+1].life + a[old][2][j+1].life;
		eight_life = tmp + a[old][0][j].life + a[old][2][j].life;
		if(Judge(&a[next][1][j], a[old][1][j], eight_life))
			a[next][1][j] = CheckColor(a[old], 1, j);
		
		for(i=2; i<WIDE-1; i++){
			tmp += ( (a[old][i+1][j-1].life + a[old][i+1][j+1].life)
						 - (a[old][i-2][j-1].life + a[old][i-2][j+1].life) );
			eight_life = tmp + a[old][i-1][j].life + a[old][i+1][j].life;
			if(Judge(&a[next][i][j], a[old][i][j], eight_life))
				a[next][i][j] = CheckColor(a[old], i, j);
		}
	}
}

int Judge(DATA *a, DATA b, int life)
{
	int ret = 0;
	switch(life){
		case 3:
			a->life = 1;
			ret = 1;
			break;

		case 2:
			*a = b;
			break;

		default:
			a->life = 0;
			a->re = 255;
			a->gr = 255;
			a->bl = 255;			
	}
	return ret;
}


void DrawTile(DATA a[WIDE][HIGH])
{
	int i,j;
	for(i=0; i<WIDE-2; i++){
		for(j=0; j<HIGH-2; j++){
			DrawBox( i*TILE_WH, j*TILE_WH, (i+1)*TILE_WH, (j+1)*TILE_WH,
				GetColor(a[i+1][j+1].re, a[i+1][j+1].gr, a[i+1][j+1].bl), TRUE);
        }
	}
}

