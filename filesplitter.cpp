#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <cstring>
#include <system_error>
#include <algorithm>

using namespace std;

fstream fileStream;
std::ifstream::pos_type filesize(const char* filename);
int GetFileSize(std::string filename);
int pieceTogether(string fileNameBase, int numberOfFiles, string finaldir, string filesdir);
int splitApart(string fileNameBase, int numberOfFiles, string outdirm, string miniName);//miniName is the name of the split files

int main(){
string dir; //directory of file to split or stitch
string odir; //output directory
string filename = "myFile"; //filename of the split files, myFile is default
long double fileSize = 1048576; //1MB
int option; //1 is to split, 2 is to stitch

cout << "(1) split a file into smaller pieces\n(2) stitch together many files made by this program\n";
cin >> option;
if(option == 1){
    bool incorrect = false; //to see if everything is good
    char confirm = 'y'; //to get the option
    bool direxists = true; //see if dir exists
    bool sizevalid = true; //to see if the size is not a negative number, signed wouldn't work, it would crash :/
    bool validdir = true; //to see if the output directory is valid
    do{
    while(direxists){
    cout << "Directory of file\n";
    cin >> dir;
    if(GetFileSize(dir) != -1){
        direxists = false; //onwards to other options!
    }
    else{
        cout << "It seems to be that this file doesn't exist... try again\n";
    }
    }
    while(sizevalid){
    cout << "Enter file size for the split files (in MB)\n";
    cin >> fileSize;
    fileSize = fileSize * 1048576;
    if(fileSize < 0){
        cout << "Cannot be a negative number, would probably overwrite your data ;)\n";
    }
    else{
        sizevalid = false; //yay it's valid!
    }
    }
    cout << "Enter the name of the split files\n";
    cin >> filename;
    filename.erase(std::remove_if(filename.begin(), filename.end(),
[]( auto const& c ) -> bool { return !std::isalnum(c); } ), filename.end()); //removes all non alphanumeric characters, thanks stack overflow!

    cout << "Enter output directory\n";
    cin >> odir;

    cout << "Is everything correct?\nDirectory of File: " << dir << "\nMax file size of each split file: " << fileSize << "MB\nFile name of the split files: " << filename << "\nOutput directory (where the split files go): " << odir << "\n(y/n)";
    cin >> confirm;
    if(confirm == 'y'){
        incorrect = false;
        continue;
    }
    else if(confirm == 'n'){
        incorrect = true;
        bool direxists = true;
        bool sizevalid = true;
        bool validdir = true;
    }
    } while(incorrect);
    int success = splitApart(dir, fileSize, odir, filename);
    if(success == 1){
        cout << "Successfully split apart " << dir;
    }
}
else if(option == 2){
    string dir;
    string fdir; //final file directory
    string filenamebase;
    int num; //number of cutup files
    bool incorrect = false; //to see if everything is good
    char confirm = 'y'; //to get the option
    bool direxists = true; //see if dir exists
    bool sizevalid = true; //to see if the size is not a negative number, signed wouldn't work, it would crash :/
    bool validdir = true; //to see if the output directory is valid
    do{
    cout << "Directory of cutup files\n";
    cin >> dir;

    cout << "Directory of final file (with extension if you want)\n";
    cin >> fdir;

    while(sizevalid){
    cout << "Number of cutup files (Please be right!!! It will fail if you aren't!)";
    cin >> num;
    if(num < 0){
        cout << "Cannot be a negative number, would probably overwrite your data ;)\n";
    }
    else{
        sizevalid = false; //yay it's valid!
    }
    }
    cout << "Enter the filename base, the name of the split files without the number at the end\n(i.e. Cutup files: myFile0, myFile1, myFile2 | filename base: myFile\n";
    cin >> filenamebase;
    filenamebase.erase(std::remove_if(filenamebase.begin(), filenamebase.end(),
    []( auto const& c ) -> bool { return !std::isalnum(c); } ), filenamebase.end()); //removes all non alphanumeric characters, thanks stack overflow!

    cout << "\n\nIs everything correct?\nDirectory of Cutup Files: " << dir << "\nNumber of Cutup files: " << num << " Files\nFinal file directory and filename: " << fdir << "Filename base: " << filenamebase << "\n(y/n)";
    cin >> confirm;
    if(confirm == 'y'){
        incorrect = false;
        continue;
    }
    else if(confirm == 'n'){
        incorrect = true;
        bool sizevalid = true;
    }
    } while(incorrect);

    int valid = pieceTogether(filenamebase, num, fdir, dir);
    if(valid){
        cout << "Your file is ready at " << fdir << " (!)";
    }
}
else{
    exit(0);
}


return 0;
}

int pieceTogether(string fileNameBase, int numberOfFiles, string finaldir, string filesdir){ //takes multiple files, stitches them together
    string content;
    fstream finalFile;
    finalFile.open(finaldir, ios::out | ios::binary);
    for(int i = 0; i < numberOfFiles; i++){
        if(finalFile.is_open()) {
        fileStream.open(filesdir + fileNameBase + to_string(i), ios::in | ios::binary); //binary is best, maybe option for text and binary in the future for power users?!?
        cout << "Getting file " << fileNameBase << i << '\n';
        if(fileStream.is_open()) {
            cout << "Reading file " << fileNameBase << i << '\n';
            char a;
            while (fileStream.read(&a, 1) && finalFile.write(&a, 1)); //copy and paste away!
        }
        else{
            cout << "Looks like the directory of the files to piece together doesn't exist.....";
            exit(0);
        }
        fileStream.close();
        }
        else{
            cout << "Seems like the directory doesn't exist from where you want the file to be......";
            exit(0);
        }
    }
finalFile.close(); //piecing together is so much easier than cutting up!
return 1;
}



int GetFileSize(std::string filename) //GetFileSize from Stack Overflow, would be nice to use something better, but tellg() isn't the best
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}


int splitApart(string fileName, int fileSize, string outdir, string miniName){  //splitting a file into many different files
    cout << fileName << '\n';
    fstream miniFiles; //the smaller files
    int readsize = GetFileSize(fileName);
    cout << readsize << '\n';
    int numFiles = readsize/fileSize + 1;
    cout << int(filesize(fileName.c_str()));
    cout << '\n' << fileSize;
    int currentChar = 0; //the current number of characters processed, only for getLine() to see when to stop getting a line
    int totalnum = 0; //the total number of characters processed
    ifstream readFile (fileName, ios_base::in | ios_base::binary); //the file to split

    if(readFile.is_open()){
    for(int i = 0; i < numFiles; i++){
        cout << "Creating file " << i << " of " << numFiles << "(" << outdir + miniName << i << ")\n";
        string minidir = outdir + miniName + to_string(i);
        try{
        miniFiles.open(minidir, ios::out | ios::binary);
        }
        catch (const std::system_error& e) {
            std::cout << "Exception caught (system_error):\n";
            std::cout << "Error: " << e.what() << '\n';
            std::cout << "Code: " << e.code().value() << '\n';
            std::cout << "Category: " << e.code().category().name() << '\n';
            std::cout << "Message: " << e.code().message() << '\n';
          }

             if(miniFiles.is_open()) {

                    char a;
                    while (readFile.read(&a, 1) && miniFiles.write(&a, 1) && currentChar < fileSize && totalnum < readsize){
                        currentChar += 1;
                        totalnum += 1;
                        if(readFile.eof()){
                           cout << "eof came too early....";
                           return 0;
                        }
                    }

                cout << "Created " << outdir << miniName << i << '\n';
         }
         else{
            cout << "Failed, your output directory might be wrong.";
            readFile.close();
            exit(0);
         }
         miniFiles.close();
         currentChar = 0;
    }
    }
    readFile.close();
    return 1;
}




