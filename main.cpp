#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <set>
#include <windows.h>

using std::cout;
using std::cin;
using std::endl;

const std::string prompt = "\n>";

bool getYN(
    std::set<std::string> yes = {"y", "Y", "yes", "Yes"},
    std::set<std::string> no = {"n", "N", "no", "No"}
);

std::string enterCopyMenu();
std::string manualOrReload();
void fail();
void cd(std::string dir);
void cp(std::string file, std::string location);
void mv(std::string file, std::string destination);
void rm(std::string file);
void exec(std::string cmd);
std::string enclose(std::string str);
std::vector<std::string> getDrives();
std::vector<std::string> getRemovableDrives(std::vector<std::string> drives);
std::string find3dsDrive(std::vector<std::string> removableDrives);

int main(int argc, char* argv[]) {
    const std::string step1{"Step 1/"};
    const std::string step2{"Step 2/"};
    const std::string forSD{"For 3DS SD card/"};
    const std::string python{"python"};
    const std::string ctrKeyGen{"ctrKeyGen.py"};
    const std::string nccInfo{"ncchinfo.bin"};
    const std::string makecia{"decrypt and make.bat"};
    const std::string installcia{"install.cia"};
    const std::string rom{"rom.3ds"};

    cout << "CIA Auto Generator" << endl;
    if (argc != 2 || argv[1] == "-h" || argv[1] == "--help"){
        cout << "Usage: cagen.exe example.3ds";
        exit(0);
    }
    std::string romName{argv[1]};

    cp(romName, rom);
    cp(rom, step1 + rom);
    cd(step1);

    std::stringstream cmd;
    cmd << python << ' ' << enclose(ctrKeyGen) << ' ' << enclose(rom);
    exec(cmd.str());

    rm(rom);

    cd("..");
    cp(step1 + nccInfo, forSD + nccInfo);
    rm(step1 + nccInfo);

    //cout << "We now need to copy some data to your 3DS SD Card." << std::endl;
    //std::string sdDrive = enterCopyMenu();


    cout << '\n' << "Copy the data in " << enclose(forSD) << " to your " <<
            "3DS SD Card. Turn on your 3DS. Go to System Settings" <<
            " > Other Settings > Profile > Nintendo DS Profile." <<
            " Once you have retreived the xorpads, copy them to " <<
            enclose(step2) << '.' << endl;
    system("pause");

    cp(rom, step2 + rom);
    cd(step2);
    exec(enclose("" + makecia));

    std::string cianame = romName;
    auto size = cianame.size();
    cianame[size-3] = 'c';
    cianame[size-2] = 'i';
    cianame[size-1] = 'a';

    cp(installcia, "../" + cianame);
    rm(rom);
    rm(installcia);
    cd("..");
    rm(rom);
    rm(forSD + nccInfo);

    cout << endl;
    return 0;
}
bool getYN(std::set<std::string> yes, std::set<std::string> no){
    std::string response;
    while(true){
        getline(cin, response);
        if (yes.find(response) != yes.end()) {
            return false;
        }
        else if (no.find(response) != no.end()) {
            return true;
        }
    }
}
std::string enterCopyMenu(){
    cout << '\n' << "Available Drives: \n" ;
    auto drives = getDrives();
    for (auto &&drive : drives){
        cout << drive << '\n';
    }
    cout << "Auto-Locate 3DS SD Card?" << prompt << std::flush;
    if (getYN()){
        auto foundSD = find3dsDrive(getRemovableDrives(getDrives()));
        if (foundSD == ""){ // Not found
            cout << "Could not find 3DS SD Card." << endl;
            return manualOrReload();
        }
        else{
            cout << "Is this the correct drive letter?\n" <<
                    foundSD << prompt << std::flush;
            if (getYN()){
                return foundSD;
            }
            else{
                return manualOrReload();
            }
        }
    }
}
std::string manualOrReload(){
    cout << "Would you like to reload the drives, " <<
            "or manually specify which drive to copy to? (reload/manual)" <<
            prompt << std::flush;
    if(getYN(std::set<std::string>{"reload"}, std::set<std::string>{"manual"})){
        return enterCopyMenu();
    }else{
        cout << "Type the full drive name exactly as it appears in the list above." <<
                prompt << std::flush;
        std::string drive{};
        std::getline(cin, drive);
        return drive;
    }
};
void fail() {
    cout << "The previous operation failed. Would you like to continue? (y/n)" <<
            prompt << std::flush;
    if(getYN()){
        return;
    }
    else{
        exit(1);
    }
}
void cd(std::string dir) {
    cout << "Changing dir to " << dir << " ... " << endl;
    BOOL result = SetCurrentDirectory(dir.c_str());
    cout << (result ? "Success." : "Fail.") << endl;
    if (result == 0)
        fail();
}
void cp(std::string file, std::string location) {
    cout << "Copying " << file << " to " << location << " ... " << endl;
    BOOL result = CopyFile(file.c_str(), location.c_str(), FALSE);
    cout << (result ? "Success." : "Fail.") << endl;
    if (result == 0)
        fail();
}
void mv(std::string file, std::string destination){
    cout << "Renaming " << file << " to " << destination << " ... " << endl;
    BOOL result = MoveFile(file.c_str(), destination.c_str());
    cout << (result ? "Success." : "Fail.") << endl;
    if (result == 0)
        fail();
}
void rm(std::string file) {
    cout << "Removing " << file << " ... " << endl;
    BOOL result = DeleteFile(file.c_str());
    cout << (result ? "Success." : "Fail.") << endl;
    if (result == 0)
        fail();
}
void exec(std::string cmd) {
    cout << "Executing " << cmd << " ... " << endl;
    auto result = system(cmd.c_str());
    cout << ((result == 0) ? "Success." : "Fail.") << endl;
    if (result != 0)
        fail();
}
std::string enclose(std::string str) {
    std::stringstream result;
    result << '\"' << str << '\"';
    return result.str();
}
std::vector<std::string> getDrives() {
    std::vector<std::string> drives{};
    const int maxlen = 26*4+1;
    char namesBuffer[maxlen];
    GetLogicalDriveStrings(maxlen, namesBuffer);
    char* p = namesBuffer;
    while(*p != '\0'){
        auto nameSize = strlen(p);
        drives.push_back(std::string{p, p+nameSize});
        p += nameSize+1;
    }
    return drives;
}
std::vector<std::string> getRemovableDrives(std::vector<std::string> drives) {
    std::vector<std::string> removables{};
    for(auto &&drive : drives){
        auto driveType = GetDriveType(drive.c_str());
        if(driveType == DRIVE_REMOVABLE){
            removables.push_back(drive);
        }
    }
    return removables;
}
std::string find3dsDrive(std::vector<std::string> removableDrives) {
    std::vector<std::string> fileTests{"Nintendo 3DS"};
    for(auto &&drive : removableDrives){
        BOOL testPassed = TRUE;
        for (auto &&test : fileTests){
            if (GetFileAttributes((drive + test).c_str()) == INVALID_FILE_ATTRIBUTES &&
                    GetLastError() == ERROR_FILE_NOT_FOUND){
                testPassed = FALSE;
            }
        }
        if (testPassed){
            return drive;
        }
    }
    return "";
}