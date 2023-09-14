/*
    Project 1 Final Code - DNA Profiling
    Author- Pedro Pardo
    UIN: 656286719
	IDE: Mac using VSCODE

    This program is used to read in mutiple files and utilize them to either display them or find something Within them. 
    With the use of ourvector we were able to store data and manipulate it to search within files to look for a specific person 
    based off a dna strand.

    Within this program there is a creative function that can do all the cases at once. To start this enter in "creative" for the command.
    It prompts the user with another input for a number, 1 through 20 which coreesponds to a text file.
    Off of this input it loads the corresponding filed needed to run, while processing it and searching for thr person its related to.
    All at once just off the number input. 
    To load the database after the searhc is conducted enter "show_database". Or alternatively you can enter the # key to exit this function 
    to use another command within the program.

*/

#include "ourvector.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

struct strands{ // used to store processed information into;
    ourvector<char> STRName; //stored names of the strands found within the database
    int count; //stores number of occurances when processed
};

//function that loads the data from the Data base into a 3d vector 
//contains 3d vector that has the database, and the text file to push into the database
void pushIntoStruct(string text, ourvector<ourvector<ourvector<char> > > &data2){ 
    ifstream infile(text);
    string value;
    ourvector <char> dataline; // takes each individual character into the first vector 
    ourvector<ourvector <char> > datarow; // takes thr row of the characters from dataline
    data2.clear(); //clears vector everytime its called so it can be used again
    
    while(!infile.eof()){
        if(getline(infile, value)){
            value += ','; //by putting a ',' after every itteration allows to the function to be able to tell when the row ends
            for (int i = 0; i < value.size(); i++) {
                if(value[i] == ','){ //once it hits a comma it pushing the chars into the row
                    datarow.push_back(dataline);
                    dataline.clear(); //then clears the char ourvector
                }
                else{ // pushes all the chars into the dataline ourvector
                    dataline.push_back(value[i]);
                }
            }
            data2.push_back(datarow); //pushes the row into the 3d vector that would contain everthing inside
            datarow.clear(); //clears the row just pushed into the 3d vector
        }
    }
}

//Function used to display everything when called
//contains 3d vector that has the database, dna strand, and the vector that contain the strand with their count to display whats inside them whenever theyre called
void display(ourvector<char> &dna, ourvector<ourvector<ourvector<char> > > &data2, ourvector<strands> &processDNA){
    //If/Else to display database, or display the error message that it isnt loaded in
    if (data2.size() != 0){ //only displays when the ourvector isnt empty
        cout << "Database loaded:" << endl;
        for( int i = 1; i < data2.size(); i++){
            for( int j = 0; j < data2[i].size(); j++){
                for(int k = 0; k < data2[i][j].size(); k++){
                    cout << data2[i][j][k];
                }
                cout << " ";
            }
            cout << endl;
        }
    }
    else{ //displays when the ourvector is empty
        cout <<  "No Database loaded." << endl;
    }

    //if/else to display the DNA strand that has been entered in
    if(dna.size() == 0){ //only displays when the ourvector size of DNA is empty
        cout << endl <<  "No DNA loaded." << endl;
    }
    else{//only displays when the ourvector size of DNA isnt empty
        cout << endl << "DNA Loaded:" << endl;
        for( int i = 0; i < dna.size(); i++){
            cout << dna[i];
        }
        cout << endl;
    }

    //if/else to displays the processed information
    if (processDNA.size() == 0){ //error message for the struct ourvector being empty
        cout << endl << "No DNA has been processed." << endl;
    }
    else{ //only displays when there is something eithin the ourvector of the struct info
        cout << endl << "DNA processed, STR counts: " << endl;
        for(int i = 0; i < processDNA.size(); i++){
            for( int j = 0; j < processDNA[i].STRName.size(); j++){
                cout << processDNA[i].STRName[j]; //displays the name of the strand
            }
            cout << ": " << processDNA[i].count << endl; //displays the number of count
        }
        cout << endl;
    }
}
//contains text file and dna vector to load the file into the vector char by char
void load_dna(string text, ourvector<char> &dna){ //loads in the dna into an ourvector 
    dna.clear();
    ifstream infile(text);
    char value;
    infile >> value;  
    while(!infile.eof()){
        dna.push_back(value); //pushes every characted into an ourvector
        infile >> value;  
    }
}

//function that deals with the process command
//contains 3d vector that has the database, dna strand, and the vector that contain the strand with their count to process all the information together and push back into processDNA itself
void process(ourvector<ourvector<ourvector<char> > > &data2, ourvector<char> &dna, ourvector<strands> &processDNA){
    processDNA.clear(); //clears the vector everytime called
    int count = 0; int realCount = 0; int maxCount = 0; //int varaiables
    for (int i = 1; i < data2[0].size(); i++){  //loops through the 3d vector, but only the people names and their strand numbers
        for (int j = 0; j < dna.size(); j++){  //loops through the DNA vector 
            if(dna[j] == data2[0][i][0]){ //checks if the char of dna is equal to the first char of the first row which is the STRANDS themselves
                for (int k = 0; k < data2[0][i].size(); k++){ //loops thorugh the 2d vector of rows 
                    if(j+k < dna.size()){ //this makes sure the loop doesnt go out of bounds with the dna size
                        if(dna[j+k] == data2[0][i][k]){ //checks if the next elements of both the strand and the dna are also equal to eachother
                            count++; //if they are it adds a count number
                        }
                    }
                }
            }
            if(count == data2[0][i].size()){ //once count of chars compared reaches the size of the strand from the database 
                realCount++; //adds one to the count of occurances
                while(realCount > maxCount){ //in order to check for the max times its repeated it
                    maxCount = realCount; //finds max number of occurances within the dna file
                }
                j += data2[0][i].size()-1; //sets dna strand postion to end of the name postion it found
                count = 0; //resets original char count back to 0 for next word
            }
            else{ //resets numbers back to zero
                realCount = 0;
                count = 0;
            }
        }
        processDNA.push_back({data2[0][i],maxCount}); //pushes back the strand name and the maxcount of occurances
        //resets all numbers back to 0
        count = 0;
        realCount = 0;
        maxCount = 0;
    }
}

//function for searching within the database to find who matches the dna file
//contains 3d vector that has the database and the vector that contain the strand with their count to search within both vectors to check if theyre within eachother
void search(ourvector<ourvector<ourvector<char> > > &data2, ourvector<strands> &processDNA){
    bool searched = false;
    int found = 0;
    string value = "";
    for (int i = 1; i < data2.size(); i++){ //seraches through the database names of people and their data 
        for (int k = 1; k < data2[i].size(); k++){ //loops though the row of people with thier data
            for (int l = 0; l < data2[i][k].size(); l++){ //loop thorugh the individual characters
                value += data2[i][k][l]; //coverts the number into a string in order to compare 
                int num = 0;
                stringstream string;
                string << value;
                string >> num; //concverts the string into a number
                if(num == processDNA[k-1].count){ //if the number is equal to the count within the struct then run
                    searched = true; //makes the bool that the name is found 
                }
                else{
                    searched = false; //makes the bool false cause it want found, it continues on
                }
            }value.clear(); //clears the string to move onto the next number
        }
        if(searched == true){ //when the bool is returbed true 
            found = i; //found gets set to the element where it was found
            break; //needs to go outside to display messages
        }
    }
    if(found == 0){ //when name isnt found displays error message
        cout << "Not found in database." << endl;

    }
    else{ //if found doesnt equal zero means something is found 
        cout << "Found in database! DNA matches: ";
        for(int i = 0; i < data2[found][0].size(); i++){
            cout << data2[found][0][i]; //finds the name within the 3d vector
        }
        cout << endl;
    }
    found = 0; //resets found back to 0
}


// function for creative component that loops though the database to find the person for the file
//contains 3d vector that has the database, dna strand, and the vector that contain the strand with their count to process the information and display if needed
void database_search(string text, ourvector<ourvector<ourvector<char> > > &data2, ourvector<char> &dna, ourvector<strands> &processDNA){
    string choice;
    pushIntoStruct(text, data2); //pushes the text into the struct and loads the database
    process(data2, dna, processDNA); //processes the database and dna file
    cout << "Searching the files for a match..."; 
    search(data2, processDNA); //searches for the match
    cout << "If you would like to view the rest of the database please enter 'show_database' or press # to end this command" << endl;
    cin >> choice; 
    if(choice == "show_database"){ //outputs the data base 
        for (int i = 1; i < data2.size();i++){
            for (int j = 0; j < data2[i].size();j++){
                for (int k = 0; k < data2[i][j].size();k++){
                    cout << data2[i][j][k];
                }
                cout << " ";
            }
            cout << endl;         
        }
    }
    else if (choice == "#"){ //if choice is # then it stops the functioin
    }

}


//function that displays the dna strand matching the input of number
//contains dna to load from the txt file
void creative_display(ourvector <char> &dna){
    for( int i = 0; i < dna.size(); i++){
        cout << dna[i];
    }
    cout << endl;
}


//function that takes in input and checks in the number chosen is between 1 and 20
//contains num to take in input to check if its valid and returns it back
int creative_num_selection(int &num){
    cin >> num;
    if(num < 1 || num > 20){
        cout << endl <<  "Invalid Number." << endl << "Once again, please choose a number between 1 and 20." << endl;
        creative_num_selection(num); //uses recursion if number is invalid
    }
    return num; //returns the choice
}

//main loop for creative component
//contains 3d vector that has the database, dna strand, and the vector that contain the strand with their count to send them to different functions and load them in
void creative_loop(int &num, char &choice, ourvector <char> &dna, ourvector<ourvector<ourvector<char> > > &data2, ourvector<strands> &processDNA){
    cin >> choice;
    string text, dnaText;
    if(choice == 'y' || choice == 'Y' ){ //checks for both lowercase and capital
        cout << endl << "Loading information... " << endl << "Based of choice of Number, the Dna makeup that corresponds to this number is as followed: " << endl;
        dnaText = to_string(num) + ".txt"; //created the input to a string and valid txt file
        load_dna (dnaText, dna); //loads the dna 
        creative_display(dna); //displays the dna
        if(num < 5){ //function to make the input small if its within 1-4
            text = "small.txt";
            database_search(text, data2, dna, processDNA); //loads the database
        }
        else{ //if its between 5-20 it loads large.txt
            text = "large.txt";
            database_search(text, data2, dna, processDNA); //loads the database
        }
    }
    else if (choice == 'n' || choice == 'N'){ //loop for if user inputs no
        cout << endl << "Please choose a different number." << endl;
        creative_num_selection(num); //reruns the input number selection
        cout << endl <<  "You have chosen " << num << " correct? Please type y for yes or n for no." << endl;
        creative_loop(num, choice, dna, data2, processDNA); //runs creative loop again to check if its the
    }
    else{ // if input is something other then whats asked it runs an error message
        cout << endl << "Im not sure I understand..." << endl << "Could you please try again, Please type y for yes or n for no." << endl; 
        creative_loop(num, choice, dna, data2, processDNA); //recalls the funtion to try again
    }
}


//function for creative component
//contains 3d vector that has the database, dna strand, and the vector that contain the strand with their count to use within the creative and load them in
void creative(ourvector<char> &dna, ourvector<ourvector<ourvector<char> > > &data2, ourvector<strands> &processDNA){
    int num;
    char choice;
    cout << "Welcome..." << endl << "Please choose a number between 1 and 20." << endl; //welcome statmemnt
    creative_num_selection(num); //calls input function
    cout << endl << "You have chosen " << num << " correct? Please type y for yes or n for no." << endl;
    creative_loop(num, choice, dna, data2, processDNA); //calls loop that runs recursivly
}

//error function for process error messages
//contains 3d vector that has the database, dna strand, and the vector that contain the strand with their count to check if their empty or not
void errorFunctionProcess(ourvector<char> &dna, ourvector<ourvector<ourvector<char> > > &data2, ourvector<strands> &processDNA){
    if(data2.size() == 0 ){ //when size == 0 it outputs a error
        cout << "No database loaded." << endl;
    }
    else if(dna.size() == 0 ){ //when dna size is 0 outputs an error
        cout << "No DNA loaded." << endl;
    }
    else if (data2.size() != 0 && dna.size() != 0 ){ //when bpth ourvectors are loaded it in processes
        cout << "Processing DNA..." << endl;
        process(data2, dna, processDNA);
    }
}

//error function for Search error messages
//contains 3d vector that has the database, dna strand, and the vector that contain the strand with their count to check if theyre empty or not
void errorFunctionSearch(ourvector<char> &dna, ourvector<ourvector<ourvector<char> > > &data2, ourvector<strands> &processDNA){
    if(data2.size() == 0 ){//when size == 0 it outputs a error
        cout << "No database loaded." << endl;
    }
    else if(dna.size() == 0 ){//when dna size is 0 outputs an error
        cout << "No DNA loaded." << endl;
    }
    else if (processDNA.size() == 0 ){//when process vector size is 0 outputs an error
        cout << "No DNA processed." << endl;
    }
    else if (data2.size() != 0 && dna.size() != 0 && processDNA.size() != 0 ){//when all ourvectors are loaded it in processes
        cout << "Searching database..." << endl;
        search(data2, processDNA);
    }
}


//main recurssive fucntion for all the commands
//contains 3d vector that has the database, dna strand, and the vector that contain the strand with their count to send them to their respective functions
void DNAProfiling(ourvector<ourvector<ourvector<char> > > &data2, ourvector<char> &dna, ourvector<strands> &processDNA){
    cout << "Enter command or # to exit:" << endl;
    string choice, text;
    cin >> choice; //takes input for a choice of command
    if (choice == "load_db"){ //if statement for load_db
        cin >> text;
        if (text == "small.txt" || text == "large.txt"){ //runs when the txt files are valid
            cout << "Loading database..." << endl;
            pushIntoStruct(text, data2); //loads database into the struct
            DNAProfiling(data2, dna, processDNA); //recalls function to run again
        }
        else{ //runs when txt files arent valid, displays error messages 
            cout << "Loading database..." << endl;
            cout << "Error: unable to open '" << text << "'" << endl;
            DNAProfiling(data2, dna, processDNA); //recalls function to run again
        }
    }
    if (choice == "display"){ //runs when the choice is display
        display(dna, data2, processDNA); //displays what has been loaded so far
        DNAProfiling(data2, dna, processDNA); //recalls function to run again
    }
    if (choice == "load_dna"){ //runs when the choice is display
        ifstream lol;
        cin >> text;
        lol.open(text);
        if(lol.is_open()){
            load_dna(text,dna);
            cout << "Loading DNA..." << endl;
            DNAProfiling(data2, dna, processDNA); //recalls function to run again
        }
        else{
            cout << "Loading DNA..." << endl;
            cout << "Error: unable to open '"<< text << "'";
            DNAProfiling(data2, dna, processDNA); //recalls function to run again
        }
    }
    if (choice == "process"){ //runs when the choice is process
        errorFunctionProcess(dna, data2, processDNA); //checks for errors when loading in
        DNAProfiling(data2, dna, processDNA); //recalls function to run again
    }
    if (choice == "search"){ //runs when the choice is search
        errorFunctionSearch(dna, data2, processDNA);  //checks for errors when loading in
        DNAProfiling(data2, dna, processDNA); //recalls function to run again
    }
    if (choice == "creative"){ //runs when the choice is creative
        creative(dna, data2, processDNA); //runs function for creative component
        DNAProfiling(data2, dna, processDNA); //recalls function to run again
    }
    if(choice == "#"){ //runs when the choice is exit
    }


}

//initalizes the vectors and welcome message
int main() {
    ourvector<char> dna;
    ourvector<ourvector<ourvector<char> > > data2;
    ourvector<strands> processDNA;
    cout << "Welcome to the DNA Profiling Application." << endl;
    DNAProfiling(data2, dna, processDNA); //runs the main loop function
    //TODO:  Write your code here.  You should have LOTS of function decomposition.
    return 0;
}