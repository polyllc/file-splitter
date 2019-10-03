#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <cstring>
#include <system_error>


using namespace std;

fstream fileStream;
std::ifstream::pos_type filesize(const char* filename);
int GetFileSize(std::string filename);
string pieceTogether(string fileNameBase, int numberOfFiles);
int splitApart(string fileNameBase, int numberOfFiles, string outdirm, string miniName);//miniName is the name of the split files

int main(){
string dir; //directory of file to split or stitch
string odir; //output directory
string filename = "myFile"; //filename of the split files, myFile is default
long double fileSize = 1048576; //1MB
int option; //1 is to split, 2 is to stitch

cout << "(1) split a file into smaller pieces\n(2) stitch together many files made by this program";
cin >> option;
if(option == 1){
    cout << "Directory of file\n";
    cin >> dir;
    cout << "Enter file size for the split files (in MB)\n";
    cin >> fileSize;
    fileSize = fileSize * 1048576;
    cout << "Enter the name of the split files\n";
    cin >> filename;
    cout << "Enter output directory\n";
    cin >> odir;
    int success = splitApart(dir, fileSize, odir, filename);
    if(success == 1){
        cout << "Successfully split apart " << dir;
    }
    if(success == 2){
        cout << "Successfully split apart " << dir << "\nand didn't create as many files!";
    }
}
else if(option == 2){

}
else{
    exit(0);
}


return 0;
}

string pieceTogether(string fileNameBase, int numberOfFiles){ //takes multiple files, stitches them together
    string content;
    fstream finalFile;
    for(int i = 0; i < numberOfFiles; i++){
    finalFile.open(fileNameBase, ios::out);
        if(finalFile.is_open()) {
        fileStream.close();
        fileStream.open(fileNameBase + char(i), ios::in);
        cout << "Getting file " << fileNameBase << i << '\n';
        if(fileStream.is_open()) {
            cout << "Reading file " << fileNameBase << i << '\n';
            fileStream >> content;
            cout << "Pasting file " << fileNameBase << i << '\n';
            finalFile << content;
        }
        fileStream.close();
        }
    }
finalFile.close();
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

                    char* buffer = new char [fileSize]; //string doesn't seem to be working, so char is the replacement, might make a memory leak due to it loading the file into memory :/
                    while(currentChar < fileSize && totalnum < readsize){
                        readFile.getline(buffer, readsize*10);
                        currentChar += strlen(buffer);
                        totalnum += strlen(buffer);
                        miniFiles << buffer;
                        if(readFile.eof()){
                           cout << "eof came too early....";
                        }
                    }

                    delete[] buffer;

                cout << "Created " << outdir << miniName << i << '\n';
         }
         else{
            cout << "Failed";
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




