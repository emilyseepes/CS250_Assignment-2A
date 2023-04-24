#include <iostream>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include <ostream>      // std::flush
#include <windows.h>
#include <fstream>
#include <math.h>
#include <iomanip>

using namespace std;

//generates random numbers 0 or 1
int coinToss()
{
    return rand() % 2;
}

//generates random coordinates for the grid
int randCoord(int ceiling, int floor)
{
    return floor + rand() % (( ceiling + 1 ) - floor);
}

//initializes the grid 2D array
void GridInit(char grid[25][25])
{
    for(int i=0; i<25; i++)
    {
        for(int j=0; j<25; j++)
            grid[i][j]=' ';
    }
}

//sets the borders for the grid
void SetGrid(char grid[25][25])
{
    for(int i=0; i<25; i++)
    {
        for(int j=0; j<25; j++)
        {
            if(i==0 || i==24)
                grid[i][j]='-';//-
            else
            {
                if(j==0 || j==24)
                    grid[i][j]='|';
                else
                    cout<<' ';
            }
        }
    }
}

//returns the direction towards the closest rabbit, given a foxes coordinates x and y
char ClosestRabbit(int x, int y, int rabbitsX[],int rabbitsY[], int numRabbits)
{

    double minDistance = 25;
    double temp_dist = -1;
    double v_distance = -1;
    double  h_distance = -1;

    //iterates through all rabbits and calculates their distance from fox's coordinates
    for(int i=0; i<numRabbits; i++)
    {
        if(rabbitsX[i]!= -1 && rabbitsY[i] != -1) // we dont consider rabbits that have been captured
        {
            temp_dist = (double)sqrt(pow(x - rabbitsX[i],2)+pow(y - rabbitsY[i],2));
            if(temp_dist<minDistance) //we keep track of minimum Distance
            {
                minDistance = temp_dist;
                v_distance = x - rabbitsX[i];
                h_distance = y - rabbitsY[i];
            }
        }
    }

    //if rabbit is closer in the vertical direction
    if(fabs(v_distance) > fabs(h_distance))
    {
        if(v_distance>0) //if distance is positive, move up
            return 'w';
        else
            return 's'; //otherwise move down
    }
    else
    {
        if(h_distance>0)
            return 'a';
        else
            return 'd';
    }
}

//checks if coordinates are invalid or not
bool InvalidPos(int x, int y)
{
    if(x ==-1 || y ==-1)
        return true;
    else
        return false;
}

//checks if capture occurs
//If rabbit is on an square next to the fox, or they are in the same square, capture happens
bool Capture(int x, int y, char grid[25][25])
{
    if(grid[x][y] == 'X' || grid[x+1][y] == 'X' || grid[x-1][y] == 'X' || grid[x][y+1] == 'X' || grid[x][y-1] == 'X')
        return true;
    else
        return false;
}

void UpdateGrid(int rabbitsX[],int rabbitsY[], int foxesX[], int foxesY[], char grid[25][25], int &score, int numRabbits, int numFoxes)
{
    //reset graphics for foxes
    for(int j=0; j< numFoxes; j++)
    {
        grid[foxesX[j]][foxesY[j]] = 'X';
    }

    //iterate through all rabbits and check if they are captured or not
    for(int i=0; i<numRabbits; i++)
    {
        if(InvalidPos(rabbitsX[i], rabbitsY[i]) == false)//only consider uncaptured rabbits
        {
            if(Capture(rabbitsX[i], rabbitsY[i], grid) == false)
            {
                grid[rabbitsX[i]][rabbitsY[i]]='o'; //not captured, set their graphic
            }

            else
            {
                //captured, set rabbits coordinate to invalid
                rabbitsX[i] = -1;
                rabbitsY[i] = -1;
                score++;    //increment score, how many rabbits have been captured
            }
        }
    }
}

void PrintMap(char grid[25][25], int score, int turns)
{
    system("CLS");

    //make sure grid edges remain intact
    for(int i=0; i<25; i++)
    {
        for(int j=0; j<25; j++)
        {
            if(i==0 || i==24)
                grid[i][j]='-';//-
            else
            {
                if(j==0 || j==24)
                    grid[i][j]='|';
            }
        }
    }

    for(int i=0; i<25; i++)
    {
        for(int j=0; j<25; j++)
            cout<<grid[i][j];
        cout<<endl;
    }
    cout<<"Score:"<<score<<endl;
    cout<<"Turns:"<<turns<<endl;
}

//since foxes move, we clear their last position
void ResetFoxes(char grid[25][25], int foxesX[], int foxesY[], int numFoxes)
{
    for(int i=0; i<numFoxes; i++)
    {
        if(foxesX[i]!= -1 && foxesY[i] != -1)
            grid[foxesX[i]][foxesY[i]] = ' ';
    }
}

void ResetRabbits(char grid[25][25], int rabbitsX[], int rabbitsY[], int numRabbits)
{
    for(int i=0; i<15; i++)
    {
        if(rabbitsX[i]!= -1 && rabbitsY[i] != -1)
            grid[rabbitsX[i]][rabbitsY[i]] = ' ';
    }
}

//loads rabbit coordinates from file
void LoadRabbits(int rabbitsX[],int rabbitsY[], int numRabbits)
{
    ifstream myfile;
    myfile.open("rabbits.txt", ios::in);
    if(myfile)
    {
        int x;
        for(int i=0; i<15; i++)
        {


            myfile>>rabbitsX[i];
            myfile>>rabbitsY[i];


        }
        myfile.close();

    }
    else
        cout<<"Could not open file"<<endl;
}

//generates random coordinates for rabbits or load from file
void SetRabbits(int rabbitsX[],int rabbitsY[], bool RandPositions, int numRabbits)
{
    if(RandPositions == true)
    {
        for(int i=0; i<numRabbits; i++)
        {
            rabbitsX[i] = randCoord(23,1);
            rabbitsY[i] = randCoord(23,1);
        }
    }
    else
        LoadRabbits(rabbitsX, rabbitsY, numRabbits);
}

//reads coordinates from file and assigns them to the two arrays
void LoadFoxes(int foxesX[], int foxesY[], int numFoxes)
{
    //FILL THIS FUNCTION
    ifstream myfile;
    myfile.open("foxes.txt", ios::in); //Allows input from stream
    if(myfile)
    {
        int numFoxes;
        for(int numFoxes = 0; numFoxes<5; numFoxes++) //Number of elements in foxes file for x & y array
        {

            myfile>>foxesX[numFoxes];
            myfile>>foxesY[numFoxes];

        }
        myfile.close();


    }
    else
        cout << "Could not open file" << endl;
}

void SetFoxes(int foxesX[], int foxesY[], bool RandPositions, int numFoxes)
{
    if(RandPositions == true)
    {
        for(int i=0; i<numFoxes; i++)
        {
            foxesX[i] = randCoord(23,1);
            foxesY[i] = randCoord(23,1);
        }
    }
    else
        LoadFoxes(foxesX, foxesY, numFoxes);
}

//foxes and rabbits move randomly, if they try to move beyond the grid, stop them
//by adjusting their coordinates, if variable is 0 or greater than 23, it should be adjusted to 1 and 24 respectively
void EdgeDetect(int &x, int &y)
{
    //FILL THIS FUNCTION
    if(x <= 0) //If variable x or is 0 or less, automatically adjust to 1
        x = 1;
    if(y <= 0)
        y = 1;
    if(x > 23) //If variable x or y is greater than 23, adjust it to value of 24
        x = 24;
    if (y > 23)
        y = 24;

}

/*
randomly chooses where to move a rabbit or fox
uses random number generator to determine where to go
updates the x and y coordinates. You do not need to implement
your own random number generator. Instead, use the coinTossn function
*/
void AutonomousMove(int &x, int &y)
{
    //FILL THIS FUNCTION
    int axis = coinToss();
    int direction = coinToss();
    //Create nested if/else loops in order to allow 'o' and 'X' to move up, down, left, right
    if (axis == 0)
    {
        if (direction == 0)
        {
            x++; // x will shift left
        }
        else
        {
            x--; //x will shift right
        }
    }
    else
    {
        if (direction == 0)
        {
            y++; // y shifts up
        }
        else
        {
            y--; // y moves down
        }
    }

//Set reference x & y to random number generator to produce random move
}

void MoveRabbits(int rabbitsX[],int rabbitsY[], bool stationary, int numRabbits)
{
    //iterate through all rabbits
    for(int i=0; i<numRabbits; i++)
    {
        //if a rabbit is uncaptured, move, and check coordinates in case they move out of the grid
        if(InvalidPos(rabbitsX[i], rabbitsY[i])==false)
        {
            AutonomousMove(rabbitsX[i], rabbitsY[i]);
            EdgeDetect(rabbitsX[i], rabbitsY[i]);

        }
    }
}

void MoveFoxesAI(int foxesX[], int foxesY[], int rabbitsX[],int rabbitsY[], int numRabbits, int numFoxes)
{
    char direction = ' ';
    for(int i=0; i<numFoxes; i++)
    {
        //determine direction to closest rabbit, for fox "i"
        direction = ClosestRabbit(foxesX[i], foxesY[i], rabbitsX, rabbitsY, numRabbits);
        //update coordinates based on direction
        if(direction == 'a')
            foxesY[i]-=1;
        else if(direction == 'w')
            foxesX[i]-=1;
        else if(direction == 'd')
            foxesY[i]+=1;
        else if(direction == 's')
            foxesX[i]+=1;
        EdgeDetect(foxesX[i], foxesY[i]);
    }
}

//random fox movement, not considering rabbit's location
void RandMoveFoxes(int foxesX[], int foxesY[], bool stationary, int numFoxes)
{

    for(int i=0; i<numFoxes; i++)
    {
        AutonomousMove(foxesX[i], foxesY[i]);
        EdgeDetect(foxesX[i], foxesY[i]);
    }
}

/*
This function implements the Menu functionality
It keeps track of the game modes RandPositions, stationary, and Random Fox movement which are then passed to other methods to alter functionality. Besides printing the menu, it sets the game modes(boolean variables passed as parameters) so make sure that they are set before exiting the function.
*/

void Menu(bool &RandPositions, bool &stationary, bool &randMove)
{

    //FILL THIS FUNCTION

    int action; // int for user to input some type of option within Menu

    do //do while loop to allow menu to stay on screen while adjusting modes. Will only exit if action == 0 || action == 1 (Launch and executes)
    {
        cout << "Launch" << setw(29)<< "(1)" << endl;
        cout << "Import Positions from File" << setw(9) << "(2)" << endl;
        cout << "Random Positions" << setw(19) << "(3)" << endl;
        cout << "Stationary Rabbits" << setw(17) << "(4)" << endl;
        cout << "Moving Rabbits" << setw(21) << "(5)" << endl;
        cout << "Random Fox Movement" << setw(16) << "(6)" << endl;
        cout << "Chase Fox Movement" << setw(17) << "(7)" << endl;
        cout << "-----------------------------------------" << endl;
        cout << "Stationary Rabbits: "<< setw(14) << stationary << endl;
        cout << "Random Fox Movement: " << setw(13) << randMove << endl;
        cout << "Random Positions: " << setw(16) << RandPositions << endl;
        cout << endl <<"Enter Action:";
        cin >> action;


        switch(action)
        {
        case 0:
            exit(0); //Will break out of entire program
            break;
        //Launch
        case 1:
            break; //Terminates loop to launch game

        case 2:
            //if (2), load coordinates from rabbit and foxes files
            //Will also set 'Random Position' to 0 as file has specific coordinates
            RandPositions = false;

            break;


        case 3:
            // if (3), coordinates will be randomly generated;
            RandPositions = true;




        case 4:
            // if (4), rabbits will stay in the same coordinates

            stationary = true;
            break;


        case 5:

            stationary = false;
            // if (5), rabbits will roam grid randomly
            break;

        case 6:
            randMove = true;
            // if (6), foxes will roam grid randomly
            break;
        case 7:
            randMove = false;

            // if (7), foxes will move to closest rabbits
            break;
        default:
            break;

        }//switch bracket

    }
    while (action != 1);  //while action does not equal 1 (which will launch program to run)

}//menu
//This function is activated when a best score has been beaten
// it prompts the user for an alias
//and prints the alias along with score on the 'leaderboard.txt' file
void SaveScore(int score, bool RandMove, bool stationary)
{
    string alias;
    ofstream outfile;
    outfile.open("leaderboard.txt");
    if(stationary == true)
        outfile<<" Stationary Rabbits,";
    else
        outfile<<" Moving     Rabbits,";
    if(RandMove == true)
        outfile<<" Random Fox Movement,";
    else
        outfile<<"Chase  Fox Movement,";
    outfile<<" Turns:"<<score<<endl;
    outfile.close();
}

int main()
{
    int turns = 0;
    int score = 0;
    int numRabbits = 15;
    int numFoxes = 5;

    //Pulling this information to implement cases in Menu function
    bool stationary = true;
    bool RandPositions = true;
    bool randMove      = true;


    srand(time(NULL));

    char grid[25][25];
    int rabbitsX[numRabbits];
    int rabbitsY[numRabbits];
    int foxesX[numFoxes];
    int foxesY[numFoxes];

    //Menu set to true for all arguments
    Menu(RandPositions, stationary, randMove);


    SetRabbits(rabbitsX, rabbitsY, RandPositions, numRabbits);
    SetFoxes(foxesX, foxesY, RandPositions, numFoxes);
    GridInit(grid);
    SetGrid(grid);
    UpdateGrid(rabbitsX, rabbitsY, foxesX, foxesY, grid, score, numRabbits, numFoxes);
    PrintMap(grid, score, turns);

    while(turns<100 && score != numRabbits)
    {
        ResetRabbits(grid, rabbitsX, rabbitsY, numRabbits);
        ResetFoxes(grid, foxesX, foxesY, numFoxes);
        if(stationary == false)
            MoveRabbits(rabbitsX, rabbitsY, stationary, numRabbits);
        if(randMove == true)
            RandMoveFoxes(foxesX, foxesY, stationary, numFoxes);
        else
            MoveFoxesAI(foxesX, foxesY, rabbitsX, rabbitsY, numRabbits, numFoxes);
        UpdateGrid(rabbitsX, rabbitsY, foxesX, foxesY, grid, score, numRabbits, numFoxes);
        Sleep(500);
        PrintMap(grid, score, turns);
        turns++;
    }

    SaveScore(score, randMove, stationary);

    return 0;
}

//MATERIALS USED:
// Malik, D. S. (2018).
//In C++ programming: From problem analysis to program design. Hard Copy Book, Cengage Learning.
//(pp. 160–173) FOR FILES
//(pp. 619-622) FOR ARRAYS IN STRUCT
// Lecture 9/8/2022 and Lecture 9/13/2022 notes


