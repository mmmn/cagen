#include <iostream>
#include <cstdlib>
#include <sstream>
#include <windows.h>

using std::cout;
using std::cin;
using std::endl;

const std::string prompt = "\n>";

void fail();
void cd(std::string dir);
void cp(std::string file, std::string location);
void rm(std::string file);
void exec(std::string cmd);
std::string enclose(std::string str);

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
void fail() {
    std::string response;
    cout << "The previous operation failed. Would you like to continue? (y/n)" <<
            prompt << std::flush;
    bool valid = false;
    while (!valid){
        getline(cin, response);
        if (response == "n" || response == "no" ||
            response == "N" || response == "No") {
            exit(1);
        }
        else if (response == "y" || response == "yes"  ||
                 response == "Y" || response == "Yes") {
            valid = true;
        }
    }
}
void cd(std::string dir) {
    cout << "Changing dir to " << dir << " ... " << endl;
    auto result = SetCurrentDirectory(dir.c_str());
    cout << (result ? "Success." : "Fail.") << endl;
    if (result == 0)
        fail();
}
void cp(std::string file, std::string location) {
    cout << "Copying " << file << " to " << location << " ... " << endl;
    auto result = CopyFile(file.c_str(), location.c_str(), FALSE);
    cout << (result ? "Success." : "Fail.") << endl;
    if (result == 0)
        fail();
}
void rm(std::string file) {
    cout << "Removing " << file << " ... " << endl;
    auto result = DeleteFile(file.c_str());
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