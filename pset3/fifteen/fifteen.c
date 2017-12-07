/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();
    
    

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{

    
        int p = d*d-1;
        for(int x = 0;x<d;x++)
        {
            for(int y = 0;y<d;y++,p--)
            {
                
                board [x][y]= p;
            }
        }
        if((d*d-1)%2 != 0)
        {
            int z= 0;
            z=board[d-1][d-3];
            board[d-1][d-3]=board[d-1][d-2];
            board[d-1][d-2]=z;
        }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    {
    for(int a = 0;a<d;a++)
        {
            for(int b = 0;b<d;b++)
            {
                if(board[a][b]==0)
                {
                    printf(" _ ");
                }
                else
                {
                printf("%2d ",board[a][b]);
                }
            }
        printf("\n");
        }
        
}
}


/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int place=0;
    int place1=0;
    int place2=0;
    int place3=0;
    int place4=0;
    for(int c = 0;c<d;c++)
        {
            for(int e = 0;e<d;e++)
            {
                
                if(board [c][e]==tile)
                {
                    place1=c;
                    place2=e;
                    
                }
            }
        }
    for(int c = 0;c<d;c++)
        {
            for(int e = 0;e<d;e++)
            {
                
                if(board [c][e]==0)
                {
                    place3=c;
                    place4=e;
                    
                }
            }
        }
    if(place1-1>-1&&board[place1-1][place2]==board[place3][place4])
    {
        place=board[place1][place2];
        board[place1][place2] = board[place3][place4];
        board[place3][place4] = place;
        return true;
    }
    else if(place1+1<d&&board[place1+1][place2]==board[place3][place4])
    {
        place=board[place1][place2];
        board[place1][place2] = board[place3][place4];
        board[place3][place4] = place;
        return true;
    }
    else if(place2+1<d&&board[place1][place2+1]==board[place3][place4])
    {
        place=board[place1][place2];
        board[place1][place2] = board[place3][place4];
        board[place3][place4] = place;
        return true;
    }
    else if(place2-1>-1&&board[place1][place2-1]==board[place3][place4])
    {   
        place=board[place1][place2];
        board[place1][place2] = board[place3][place4];
        board[place3][place4] = place;
        return true;
    }
    else 
    {
    return false;
    }
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int p = 1;

    for(int c = 0;c<d;c++)
        {
            for(int e = 0;e<d;e++)
            {
                
                if(c==d-1&&e==d-1)
                {
                    return true;
                }
                if(board[c][e]!=p)
                {
                    return false;
                }
                p++;
            }
        }
    return true;  

    
    
}