//Final Game Code
#include <cstdlib> 
#include <iostream> 
#include <conio.h>
#include <string.h>
#include <time.h>

#define RESET   "\033[0;0m"
#define BLACK   "\033[0;30m"      /* Black */				 
#define RED     "\033[0;31m"      /* Red */
#define GREEN   "\033[0;32m"      /* Green */
#define YELLOW  "\033[0;33m"      /* Yellow */
#define BLUE    "\033[0;34m"      /* Blue */
#define MAGENTA "\033[0;35m"      /* Magenta */
#define CYAN    "\033[0;36m"      /* Cyan */
#define WHITE   "\033[0;37m"      /* White */
#define BACKBLACK   "\033[0;40m"      /* BACK Black */
#define BACKRED     "\033[0;41m"      /* BACK Red */
#define BACKGREEN   "\033[0;42m"      /* BACK Green */
#define BACKYELLOW  "\033[0;43m"      /* BACK Yellow */
#define BACKBLUE    "\033[0;44m"      /* BACK Blue */
#define BACKMAGENTA "\033[0;45m"      /* BACK Magenta */
#define BACKCYAN    "\033[0;46m"      /* BACK Cyan */
#define BACKWHITE   "\033[0;47m"      /* BACK White */

#define GAMESIZE 10
int AisComputer, BisComputer;
int board[GAMESIZE][GAMESIZE];
bool used[GAMESIZE][GAMESIZE];
int scoreA = 0, scoreB = 0;
int jokerA, jokerB;
bool turn = 0; // 0 is Player A, 1 is Player B
int selectible;
char ErrorMessage[100];
char AutoPlayer();

bool isSelectible(int i, int j);


void SetErrorMessage(char* str)
{
	strcpy(ErrorMessage, str);	
}

void Initiate()
{
	srand(time(NULL));
	for(int i=0; i<GAMESIZE; i++)
		for(int j=0; j<GAMESIZE; j++)
		{		
			board[i][j] = rand() % 100 - 50;
			if(board[i][j] >= 0) board[i][j] +=1;
			used[i][j] = false;
		}
	selectible = GAMESIZE / 2;
	SetErrorMessage("");
	jokerA = jokerB = GAMESIZE / 3;
	
}

void DrawLine()
{
	printf("       --------------------------------------------------------\n");
}

void PrintBoard()
{
	printf("       %s", !turn ? YELLOW : "" );	
	for(int i=0; i<GAMESIZE; i++)
		printf("     %c ", 65+i);
	printf("\n"RESET);
	DrawLine();
	
	for(int i=0; i<GAMESIZE; i++)
	{	
		printf("    %s%c: "RESET, turn ? YELLOW : "", 65+i);
		for(int j=0; j<GAMESIZE; j++)
			if(isSelectible(i, j))
			{				
				if(!used[i][j])
					printf(YELLOW"[%5d]"RESET, board[i][j]);
				else
					printf("       ");
			}
			else
			{				
				if(!used[i][j])
					printf("%6d ", board[i][j]);
				else
					printf("       ");
			}
		printf("\n");
	}
}

bool isEmpty(int rowcol)
{
	if (!turn)
	{
		for(int i=0; i<GAMESIZE; i++)
		   if(!used[rowcol][i]) return false;
	}
	else
	{
		for(int i=0; i<GAMESIZE; i++)
		   if(!used[i][rowcol]) return false;
	}
	 
	return true;
}


void PrintScores()
{
	printf("\n %c [%c] Player A: %5d   ", !turn ? '*' : ' ', AisComputer ? 'C' : 'H' ,scoreA);
	for(int i=0; i<jokerA; i++) printf("J");
		
	printf("\n %c [%c] Player B: %5d   ", turn ? '*' : ' ', BisComputer ? 'C' : 'H', scoreB);
	for(int i=0; i<jokerB; i++) printf("J");
	printf("\n\n");	
	DrawLine();
	printf(" *: Joker\n");
	printf(" X: Exit\n");
	
	printf(BACKRED"%s \n"RESET, ErrorMessage);
	printf("Select A row or column\n");	
	DrawLine();
}

bool isSelectible(int i, int j)
{
	if (!turn) 
		return (i == selectible); // && (!used[i][j])
	else
		return (j == selectible); // && (!used[i][j]);
}

void changeSelectible()
{
	int nonempty[GAMESIZE];
	int last=0;
	
	for(int i=0;i<GAMESIZE;i++)
		if(!isEmpty(i)) nonempty[last++]=i;
	
	selectible = nonempty[rand() % last];
}

void SelectPlayers()
{
	char c;
	system("cls");
	printf("Select Player A: (H)uman or (C)omputer: ");
	c=toupper(getch());
	AisComputer = c=='C';
	
	printf("\nSelect Player B: (H)uman or (C)omputer: ");
	c=toupper(getch());
	BisComputer = c=='C';
}

void MainLoop()
{
	char c = 0;
	while(c != 'X')
	{
		system("cls");
		PrintBoard();
		PrintScores();
		if(isEmpty(selectible))
		{
			printf(YELLOW"Game Over\n");
			if (scoreA > scoreB)
				printf("Player A WIN!!");
			else if (scoreB > scoreA)
				printf("Player B WIN!!");
			else // ==
				printf("Draw Game!");
			printf(RESET);
			break;			
		}
		
		if(!turn) 
			c=AisComputer ? AutoPlayer() : toupper( getch() );			
		else
			c=BisComputer ? AutoPlayer() : toupper( getch() );
					
		if(c=='X') break;
		if(c=='*')
		{
			SetErrorMessage("");
			if(!turn)
			{
				if(jokerA==0)
				{
					SetErrorMessage("You are out of Jokers");
					continue;
				}
				jokerA--;
			}
			else
			{
				if(jokerB==0)
				{
					SetErrorMessage("You are out of Jokers");
					continue;
				}
				jokerB--;
			}
			turn = !turn;			
			changeSelectible();
			continue;
		}
		if((c<65) || (c>64+GAMESIZE) )
		{
			SetErrorMessage("Select a row or coloumn in range");
			continue;
		}
		int idx = c-65;	
		//         for Player A								for Player B
		if(( !turn && used[selectible][idx] ) || ( turn && used[idx][selectible] ))
		{
				SetErrorMessage("This cell is already used. Please select another cell");
				continue;	
		}
		
				
		if(!turn)	// Player A
		{
			// Add the score
			scoreA += board[selectible][idx];
			// Delete Number
			used[selectible][idx] = true;
		}
		else		// Player B
		{
			// Add the score
			scoreB += board[idx][selectible];
			// Delete Number
			used[idx][selectible] = true;
		}
		// assign then new selection coloumn
		selectible = idx;
		SetErrorMessage("");
		turn = !turn;
	}
}


int main(int argc, char** argv) {
	SelectPlayers();
	Initiate();	
	MainLoop();
	return 0;
}


// ***********hazýr kod üzerinde buradan itibaren oluþturduðum oyuncu bot**************
char AutoPlayer() {
    int bestScore = -10000; // En düþük skorla baþla
    int bestMove = -1; // En iyi hamleyi sakla
    
    for (int i = 0; i < GAMESIZE; ++i) {
        if (!turn && used[selectible][i]) continue; // oyuncu a nýn sütunu doluysa atla
        if (turn && used[i][selectible]) continue; // oyuncu b nin satýrý doluysa atla
        
        int tempScore = 0;
        
        if (!turn) {
            tempScore = board[selectible][i]; // oyuncu anýn seçimi
        } else {
            tempScore = board[i][selectible]; // oyuncu b nin seçimi
        }

        // daha önce seçilmiþ bir sayýyý seçmemeye çalýþ
        if (tempScore < 0) continue; // seçilen sayý negatifse atla
        
        // en iyi seçeneði seç (negatif olmayan en büyük sayýyý bul)
        if (tempScore > bestScore && tempScore >= -10) {
            bestScore = tempScore;
            bestMove = i;
        }
    }
    
    // joker hakký varsa ve -10dan büyük bir sayý varsa joker kullan
    if (((turn == 0 && jokerA > 0) || (turn == 1 && jokerB > 0)) && bestScore < -10) {
        return '*';
    }
    
    // eðer joker hakký yoksa ve en iyi hamle negatifse veya joker hakký tükendiyse
    if ((((turn == 0 && jokerA == 0) || (turn == 1 && jokerB == 0)) && bestScore <= 0) || ((turn == 0 && jokerA == 0) || (turn == 1 && jokerB == 0)) && bestScore < -10) {
        int bestNegative = -10000;
        
        // negatifler arasýndan en iyi hamleyi seç
        for (int i = 0; i < GAMESIZE; ++i) {
            if (!turn && used[selectible][i]) continue;
            if (turn && used[i][selectible]) continue;
        
            int tempScore = 0;
            
            if (!turn) {
                tempScore = board[selectible][i];
            } else {
                tempScore = board[i][selectible];
            }
            
            if (tempScore < 0 && tempScore > bestNegative) {
                bestNegative = tempScore;
                bestMove = i;
            }
        }
        
        return (bestMove != -1) ? bestMove + 65 : '*'; // negatifler arasýndan en iyi hamleyi yap
    }

    // en iyi hamleyi yap
    return (bestMove != -1) ? bestMove + 65 : '*'; // seçilen hamleyi döndür (A, B, C, ...) veya joker kullan
}
