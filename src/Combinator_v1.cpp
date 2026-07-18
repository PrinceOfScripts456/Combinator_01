#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include "../include/ConsoleUtils.h"
#include "../include/utils.h"

using std::cin;
using std::cout;
using std::cerr;
using std::string;
using std::stringstream;
using std::ios;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::runtime_error;
using std::invalid_argument;

class Wordlist
{
    private:
        int argc;                                                               // no. of original Arguments
        string argv[100];                                                       // stores actual Arguments in argv[]

        int R;                                                                  // no. of salts to use in a word
        int MinLen;                                                             // min length of a word
        int MaxLen;                                                             // max length of a word
        string Mode;                                                            // stores Mode

        int strLen;                                                             // no. of salts in Arguments or in str[]
        int strlen[100];                                                        // length of actual salts in str[]
        string str[100];                                                        // stores actual salts taken from Arguments - 1D approach

        int StrLen[100];                                                        // no. of salts at Str[]
        int Strlen[100][100];                                                   // length of actual salt at Str[][]
        string Str[100][100];                                                   // stores actual salts at Str[][] - 2D approach

        string inFname;                                                         // input filename
        string outFname;                                                        // output filename
        ifstream inFile;                                                        // input file (actual)
        ofstream outFile;                                                       // output file (actual)

        uint64_t wrtL;                                                          // no. of lines written
        uint64_t ftrL;                                                          // no. of lines filtered out - by rules
        uint64_t wrtL_R[10];                                                    // no. of lines written (for specific R, like R=1 as R[1])
        uint64_t ftrL_R[10];                                                    // no. of lines filtered out - by rules

    public:
        void Initializer();                                                     // intializes all variables to default values
        void Analyze_Arguments(int, char *[]);
        void Generate_Wordlist();

    private:
        void Arguments_Case();
        void parseIntOption(int &, string *, int &);
        void Generate_Wordlist_Arg();

        void File_Case();
        void parseStringOption(string &, string *, int &);
        void parseFile();
        void Generate_Wordlist_File();

        void File_FilterCase();
        void Generate_Wordlist_Filtered();

    public:
        void show_help();
        void Show_Data();
        void Wordlist_Info();
        void Validate_option_Values();
        void autoExecute(int, char *[]);

        bool showdata;                                                          // flag for showing data
        bool winfo;                                                             // flag for showing wordlist info
        bool wordlist;                                                          // flag for generating wordlist
        bool only_calc;
};


void Wordlist::Initializer()
{
    argc = 0;
    for(int i = 0; i < 100; i++) {
        argv[i] = "";
    }

    R = 1;
    MinLen = 8;
    MaxLen = 14;
    Mode = "#";

    strLen = 0;
    for(int i = 0; i < 100; i++) {
        strlen[i] = 0 ;
           str[i] = "";
    }

    for(int i = 0; i < 100; i++)
    {
        StrLen[i] = 0;
        for(int j = 0; j < 100; j++)
        {
            Strlen[i][j] = 0 ;
               Str[i][j] = "";
        }
    }

    inFname = "";
    outFname = "ATempo.txt";

    wrtL = 0;
    ftrL = 0;

    showdata = false;
    winfo    = false;
    wordlist = false;
    only_calc = false;

    for(int i = 0; i < 10; i++)
    {
        wrtL_R[i] = 0;
        ftrL_R[i] = 0;
    }
}

void Wordlist::Analyze_Arguments(int argcP, char *argvP[])
{
    argc = argcP;                                                               // copying outer argc,argv(P) into class's own argc,argv
    for(int i = 0; i < argc; i++) {
        argv[i] = argvP[i];
    }

    if(argv[1] == "-h" || argv[1] == "--help") {                                // Combinator.exe -h
        show_help();
        throw runtime_error("exit_code: 0");
    }
    else {
        Mode = argv[1];                                                         // Combinator.exe [Mode]
    }

    if(Mode == "A")
    {
        Arguments_Case();
    }
    else if(Mode == "F")
    {
        File_Case();
        parseFile();
    }
    else if(Mode == "f")
    {
        File_FilterCase();
    }
    else
    {
        cout<<"Undefined Mode selected: \'"<< Mode <<"\' \n";
        throw invalid_argument("exit_code: 1");
    }
}

void Wordlist::Generate_Wordlist()
{
    if(Mode == "A")
    {
        Generate_Wordlist_Arg();
    }
    else if(Mode == "F")
    {
        Generate_Wordlist_File();
    }
    else if(Mode == "f")
    {
        Generate_Wordlist_Filtered();
    }
    else
    {
        throw runtime_error("exit_code: 9");
    }
}


void Wordlist::Arguments_Case()
{
    for(int i = 2; i < argc; i++) // skips Combinator_v1.exe and Mode(A)
    {
        if(argv[i] == "-r" || argv[i] == "-R")
        {
            parseIntOption(R, argv, i);
        }
        else if(argv[i] == "-m" || argv[i] == "--min" || argv[i] == "--Min" || argv[i] == "--MIN")
        {
            parseIntOption(MinLen, argv, i);
        }
        else if(argv[i] == "-M" || argv[i] == "--max" || argv[i] == "--Max" || argv[i] == "--MAX")
        {
            parseIntOption(MaxLen, argv, i);
        }
        else if(argv[i] == "-o" || argv[i] == "--outfile")
        {
            parseStringOption(outFname, argv, i);
        }
        else if(argv[i] == "-h" || argv[i] == "--help")
        {
            show_help();
            throw runtime_error("exit_code: 0");
        }
        else if(argv[i] == "-s" || argv[i] == "--showdata")
        {
            showdata = true;
        }
        else if(argv[i] == "-w" || argv[i] == "--winfo")
        {
            winfo = true;
        }
        else if(argv[i] == "-W" || argv[i] == "--wordlist")
        {
            wordlist = true;
        }
        else if(argv[i] == "-oc" || argv[i] == "--onlycalc")
        {
            only_calc = true;
        }
        else
        {
            str[strLen] = argv[i];
            strlen[strLen] = static_cast<int>( str[strLen].length() );
            strLen++;
        }
    }
}

void Wordlist::parseIntOption(int &_int, string *argvP, int &i)
{
    if(argvP[i+1].length() == 0 )
    {
        cout<<"error in ("<< argvP[i] <<"):  no value was provided. \n";
        throw invalid_argument("exit_code: 2");
    }
    if(!(argvP[i+1][0] >= '0' && argvP[i+1][0] <= '9'))
    {
        cout<<"error in ("<< argvP[i] <<"):  invalid value = \'"<< argvP[i+1] <<"\' \n";
        throw invalid_argument("exit_code: 2");
    }
    if(argvP[i+1].length() >= 10 )
    {
        cout<<"error in ("<< argvP[i] <<"):  given value is causing overflow = \'"<< argvP[i+1] <<"\' \n";
        throw invalid_argument("exit_code: 2");
    }

    size_t idx;
    _int = stoi(argvP[i+1], &idx);
    if(idx != argvP[i+1].length())
    {
        cout<<"error in ("<< argvP[i] <<"):  partial invalid value = \'"<< argvP[i+1] <<"\' \n";
        throw invalid_argument("exit_code: 2");
    }
    i++;
}

void Wordlist::Generate_Wordlist_Arg()
{
    cout<<"Generating Wordlist(A): \n";

    if(only_calc == false)
    {
        outFile.open(outFname);
        if(!(outFile.is_open()))
        {
            cerr<<"Unable to Access file Named: "<< outFname <<"\n";
            throw invalid_argument("exit_code: 3");
        }
    }

    int WordLen; // will store length of a whole word (line)

    if(R == 1 || R > 1)
    {
        for(int n1 = 0; n1 < strLen; n1++)
        {
            WordLen = 0;
            WordLen += strlen[n1];

            if(WordLen >= MinLen && WordLen <= MaxLen)
            {
                if(only_calc == false)
                {
                    outFile << str[n1] << "\n";
                }
                wrtL++;
                wrtL_R[1]++;
            }
            else
            {
                ftrL++;
                ftrL_R[1]++;
            }
        }
    }
    if(R == 2 || R > 2)
    {
        for(int n1 = 0; n1 < strLen; n1++)
        for(int n2 = 0; n2 < strLen; n2++)
        {
            WordLen = 0;
            WordLen += strlen[n1];
            WordLen += strlen[n2];

            if(WordLen >= MinLen && WordLen <= MaxLen)
            {
                if(only_calc == false)
                {
                    outFile << str[n1] << str[n2] << "\n";
                }
                wrtL++;
                wrtL_R[2]++;
            }
            else
            {
                ftrL++;
                ftrL_R[2]++;
            }
        }
    }
    if(R == 3 || R > 3)
    {
        for(int n1 = 0; n1 < strLen; n1++)
        for(int n2 = 0; n2 < strLen; n2++)
        for(int n3 = 0; n3 < strLen; n3++)
        {
            WordLen = 0;
            WordLen += strlen[n1];
            WordLen += strlen[n2];
            WordLen += strlen[n3];

            if(WordLen >= MinLen && WordLen <= MaxLen)
            {
                if(only_calc == false)
                {
                    outFile << str[n1] << str[n2] << str[n3] << "\n";
                }
                wrtL++;
                wrtL_R[3]++;
            }
            else
            {
                ftrL++;
                ftrL_R[3]++;
            }
        }
    }
    if(R == 4 || R > 4)
    {
        for(int n1 = 0; n1 < strLen; n1++)
        for(int n2 = 0; n2 < strLen; n2++)
        for(int n3 = 0; n3 < strLen; n3++)
        for(int n4 = 0; n4 < strLen; n4++)
        {
            WordLen = 0;
            WordLen += strlen[n1];
            WordLen += strlen[n2];
            WordLen += strlen[n3];
            WordLen += strlen[n4];

            if(WordLen >= MinLen && WordLen <= MaxLen)
            {
                if(only_calc == false)
                {
                    outFile << str[n1] << str[n2] << str[n3] << str[n4] << "\n";
                }
                wrtL++;
                wrtL_R[4]++;
            }
            else
            {
                ftrL++;
                ftrL_R[4]++;
            }
        }
    }
    if(R == 5 || R > 5)
    {
        for(int n1 = 0; n1 < strLen; n1++)
        for(int n2 = 0; n2 < strLen; n2++)
        for(int n3 = 0; n3 < strLen; n3++)
        for(int n4 = 0; n4 < strLen; n4++)
        for(int n5 = 0; n5 < strLen; n5++)
        {
            WordLen = 0;
            WordLen += strlen[n1];
            WordLen += strlen[n2];
            WordLen += strlen[n3];
            WordLen += strlen[n4];
            WordLen += strlen[n5];

            if(WordLen >= MinLen && WordLen <= MaxLen)
            {
                if(only_calc == false)
                {
                    outFile << str[n1] << str[n2] << str[n3] << str[n4] << str[n5] << "\n";
                }
                wrtL++;
                wrtL_R[5]++;
            }
            else
            {
                ftrL++;
                ftrL_R[5]++;
            }
        }
    }
    if(R == 6 || R > 6)
    {
        for(int n1 = 0; n1 < strLen; n1++)
        for(int n2 = 0; n2 < strLen; n2++)
        for(int n3 = 0; n3 < strLen; n3++)
        for(int n4 = 0; n4 < strLen; n4++)
        for(int n5 = 0; n5 < strLen; n5++)
        for(int n6 = 0; n6 < strLen; n6++)
        {
            WordLen = 0;
            WordLen += strlen[n1];
            WordLen += strlen[n2];
            WordLen += strlen[n3];
            WordLen += strlen[n4];
            WordLen += strlen[n5];
            WordLen += strlen[n6];

            if(WordLen >= MinLen && WordLen <= MaxLen)
            {
                if(only_calc == false)
                {
                    outFile << str[n1] << str[n2] << str[n3] << str[n4] << str[n5] << str[n6] << "\n";
                }
                wrtL++;
                wrtL_R[6]++;
            }
            else
            {
                ftrL++;
                ftrL_R[6]++;
            }
        }
    }
    if(R == 7 || R > 7)
    {
        for(int n1 = 0; n1 < strLen; n1++)
        for(int n2 = 0; n2 < strLen; n2++)
        for(int n3 = 0; n3 < strLen; n3++)
        for(int n4 = 0; n4 < strLen; n4++)
        for(int n5 = 0; n5 < strLen; n5++)
        for(int n6 = 0; n6 < strLen; n6++)
        for(int n7 = 0; n7 < strLen; n7++)
        {
            WordLen = 0;
            WordLen += strlen[n1];
            WordLen += strlen[n2];
            WordLen += strlen[n3];
            WordLen += strlen[n4];
            WordLen += strlen[n5];
            WordLen += strlen[n6];
            WordLen += strlen[n7];

            if(WordLen >= MinLen && WordLen <= MaxLen)
            {
                if(only_calc == false)
                {
                    outFile << str[n1] << str[n2] << str[n3] << str[n4] << str[n5] << str[n6] << str[n7] << "\n";
                }
                wrtL++;
                wrtL_R[7]++;
            }
            else
            {
                ftrL++;
                ftrL_R[7]++;
            }
        }
    }
    if(R == 8 || R > 8)
    {
        for(int n1 = 0; n1 < strLen; n1++)
        for(int n2 = 0; n2 < strLen; n2++)
        for(int n3 = 0; n3 < strLen; n3++)
        for(int n4 = 0; n4 < strLen; n4++)
        for(int n5 = 0; n5 < strLen; n5++)
        for(int n6 = 0; n6 < strLen; n6++)
        for(int n7 = 0; n7 < strLen; n7++)
        for(int n8 = 0; n8 < strLen; n8++)
        {
            WordLen = 0;
            WordLen += strlen[n1];
            WordLen += strlen[n2];
            WordLen += strlen[n3];
            WordLen += strlen[n4];
            WordLen += strlen[n5];
            WordLen += strlen[n6];
            WordLen += strlen[n7];
            WordLen += strlen[n8];

            if(WordLen >= MinLen && WordLen <= MaxLen)
            {
                if(only_calc == false)
                {
                    outFile << str[n1] << str[n2] << str[n3] << str[n4] << str[n5] << str[n6] << str[n7] << str[n8] << "\n";
                }
                wrtL++;
                wrtL_R[8]++;
            }
            else
            {
                ftrL++;
                ftrL_R[8]++;
            }
        }
    }
    if(R == 9)
    {
        for(int n1 = 0; n1 < strLen; n1++)
        for(int n2 = 0; n2 < strLen; n2++)
        for(int n3 = 0; n3 < strLen; n3++)
        for(int n4 = 0; n4 < strLen; n4++)
        for(int n5 = 0; n5 < strLen; n5++)
        for(int n6 = 0; n6 < strLen; n6++)
        for(int n7 = 0; n7 < strLen; n7++)
        for(int n8 = 0; n8 < strLen; n8++)
        for(int n9 = 0; n9 < strLen; n9++)
        {
            WordLen = 0;
            WordLen += strlen[n1];
            WordLen += strlen[n2];
            WordLen += strlen[n3];
            WordLen += strlen[n4];
            WordLen += strlen[n5];
            WordLen += strlen[n6];
            WordLen += strlen[n7];
            WordLen += strlen[n8];
            WordLen += strlen[n9];

            if(WordLen >= MinLen && WordLen <= MaxLen)
            {
                if(only_calc == false)
                {
                    outFile << str[n1] << str[n2] << str[n3] << str[n4] << str[n5] << str[n6] << str[n7] << str[n8] << str[n9] << "\n";
                }
                wrtL++;
                wrtL_R[9]++;
            }
            else
            {
                ftrL++;
                ftrL_R[9]++;
            }
        }
    }

    cout<<"R(n): written | filtered -> total \n";
    for(int i = 1; i <= R; i++) {
        cout<<"R(" << i << "):  " << wrtL_R[i] <<" | "<< ftrL_R[i] <<"\t-> "<< wrtL_R[i] + ftrL_R[i] << "\n";
    }

    cout<<"No. of Lines Written = " << wrtL <<"\n"
        <<"No. of Lines Filterd = " << ftrL <<"\n"
        <<"\n";
    
}


void Wordlist::File_Case()
{
    for(int i = 2; i < argc; i++) // skips Combinator_v1.exe and Mode(A)
    {
        if(argv[i] == "-m" || argv[i] == "--min" || argv[i] == "--Max" || argv[i] == "--MIN")
        {
            parseIntOption(MinLen, argv, i);
        }
        else if(argv[i] == "-M" || argv[i] == "--max" || argv[i] == "--Max" || argv[i] == "--MAX")
        {
            parseIntOption(MaxLen, argv, i);
        }
        else if(argv[i] == "-i" || argv[i] == "--infile")
        {
            parseStringOption(inFname, argv, i);
        }
        else if(argv[i] == "-o" || argv[i] == "--outfile")
        {
            parseStringOption(outFname, argv, i);
        }
        else if(argv[i] == "-h" || argv[i] == "--help")
        {
            show_help();
            throw runtime_error("exit_code: 0");
        }
        else if(argv[i] == "-s" || argv[i] == "--showdata")
        {
            showdata = true;
        }
        else if(argv[i] == "-w" || argv[i] == "--winfo")
        {
            winfo = true;
        }
        else if(argv[i] == "-W" || argv[i] == "--wordlist")
        {
            wordlist = true;
        }
        else
        {
            cout<<"undefined Argument: " << argv[i] <<"\n";
            throw invalid_argument("exit_code: 1");
        }
    }
}

void Wordlist::parseStringOption(string &_str, string *argvP, int &i)
{
    if(argvP[i+1].length() == 0 )
    {
        cout<<"error in ("<< argvP[i] <<"):  no value was provided. \n";
        throw invalid_argument("exit_code: 2");
    }
    if(argvP[i+1].length() > 46 )
    {
        cout<<"error in ("<< argvP[i] <<"):  soft limit of parsed string length is 46, Given value = \'"<< argvP[i+1] <<"\' \n";
        throw invalid_argument("exit_code: 2");
    }

    _str = argvP[i+1];
    i++;
}

void Wordlist::parseFile()
{
    inFile.open(inFname);

    if(!(inFile.is_open())) // is_open() returns true if opened successfully, false otherwise.
    {
        cerr<<"Unable to Access file Named: \'" << inFname << "\' \n";
        throw invalid_argument("exit_code: 3");
    }

    string Line;
    stringstream line;
    int  Lnum =  0; // General Line Number
    int  Snum =  0; // Salt number of a line (position)
    int SLnum = -1; // Salt line number      (R)(line)

    while(getline(inFile, Line))
    {
        Lnum += 1;
        if(Line[0] == '#' || Line.length() == 0) // Comments lines or Empty lines
        {
            continue;
        }
        else if(Line[0] == ')' && Line[1] == '>' && Line[2] == ' ' && Line[3] == '|') // )> |salt-1|salt-2|
        {
            SLnum += 1;
            Snum = -1;

            line.str(Line.substr(4)); // stores string starting from index 4
            line.clear(); // i don't know deeply - but it helps in loop running.

            while(getline(line, Str[SLnum][++Snum], '|'))
            {
                Strlen[SLnum][Snum] = static_cast<int>( Str[SLnum][Snum].length() );
            }
            StrLen[SLnum] = Snum;
        }
        else
        {
            cerr<<"syntax error on line: "<< Lnum <<"\n"
                <<"content: \'" << Line << "\' \n"
            ;
            throw runtime_error("exit_code: 4");
        }
    }
    R = SLnum + 1;
}

void Wordlist::Generate_Wordlist_File()
{
    cout<<"Generating Wordlist(F): \n";

    outFile.open(outFname);
    if(!(outFile.is_open()))
    {
        cerr<<"Unable to Access file Named: "<< outFname <<"\n";
        throw invalid_argument("exit_code: 3");
    }

    int WordLen; // will store length of a whole word (line)
    switch(R)
    {
        case 1:
            for(int n1 = 0; n1 < StrLen[0]; n1++)
            {
                WordLen = 0;
                WordLen += Strlen[0][n1];

                if(WordLen >= MinLen && WordLen <= MaxLen)
                {
                    outFile << Str[0][n1] << "\n";
                    wrtL++;
                }
                else
                {
                    ftrL++;
                }
            }
            break;
        case 2:
            for(int n1 = 0; n1 < StrLen[0]; n1++)
            for(int n2 = 0; n2 < StrLen[1]; n2++)
            {
                WordLen = 0;
                WordLen += Strlen[0][n1];
                WordLen += Strlen[1][n2];

                if(WordLen >= MinLen && WordLen <= MaxLen)
                {
                    outFile << Str[0][n1] << Str[1][n2] << "\n";
                    wrtL++;
                }
                else
                {
                    ftrL++;
                }
            }
            break;
        case 3:
            for(int n1 = 0; n1 < StrLen[0]; n1++)
            for(int n2 = 0; n2 < StrLen[1]; n2++)
            for(int n3 = 0; n3 < StrLen[2]; n3++)
            {
                WordLen = 0;
                WordLen += Strlen[0][n1];
                WordLen += Strlen[1][n2];
                WordLen += Strlen[2][n3];

                if(WordLen >= MinLen && WordLen <= MaxLen)
                {
                    outFile << Str[0][n1] << Str[1][n2] << Str[2][n3] << "\n";
                    wrtL++;
                }
                else
                {
                    ftrL++;
                }
            }
            break;
        case 4:
            for(int n1 = 0; n1 < StrLen[0]; n1++)
            for(int n2 = 0; n2 < StrLen[1]; n2++)
            for(int n3 = 0; n3 < StrLen[2]; n3++)
            for(int n4 = 0; n4 < StrLen[3]; n4++)
            {
                WordLen = 0;
                WordLen += Strlen[0][n1];
                WordLen += Strlen[1][n2];
                WordLen += Strlen[2][n3];
                WordLen += Strlen[3][n4];

                if(WordLen >= MinLen && WordLen <= MaxLen)
                {
                    outFile << Str[0][n1] << Str[1][n2] << Str[2][n3] << Str[3][n4] << "\n";
                    wrtL++;
                }
                else
                {
                    ftrL++;
                }
            }
            break;
        case 5:
            for(int n1 = 0; n1 < StrLen[0]; n1++)
            for(int n2 = 0; n2 < StrLen[1]; n2++)
            for(int n3 = 0; n3 < StrLen[2]; n3++)
            for(int n4 = 0; n4 < StrLen[3]; n4++)
            for(int n5 = 0; n5 < StrLen[4]; n5++)
            {
                WordLen = 0;
                WordLen += Strlen[0][n1];
                WordLen += Strlen[1][n2];
                WordLen += Strlen[2][n3];
                WordLen += Strlen[3][n4];
                WordLen += Strlen[4][n5];

                if(WordLen >= MinLen && WordLen <= MaxLen)
                {
                    outFile << Str[0][n1] << Str[1][n2] << Str[2][n3] << Str[3][n4] << Str[4][n5] << "\n";
                    wrtL++;
                }
                else
                {
                    ftrL++;
                }
            }
            break;
        case 6:
            for(int n1 = 0; n1 < StrLen[0]; n1++)
            for(int n2 = 0; n2 < StrLen[1]; n2++)
            for(int n3 = 0; n3 < StrLen[2]; n3++)
            for(int n4 = 0; n4 < StrLen[3]; n4++)
            for(int n5 = 0; n5 < StrLen[4]; n5++)
            for(int n6 = 0; n6 < StrLen[5]; n6++)
            {
                WordLen = 0;
                WordLen += Strlen[0][n1];
                WordLen += Strlen[1][n2];
                WordLen += Strlen[2][n3];
                WordLen += Strlen[3][n4];
                WordLen += Strlen[4][n5];
                WordLen += Strlen[5][n6];

                if(WordLen >= MinLen && WordLen <= MaxLen)
                {
                    outFile << Str[0][n1] << Str[1][n2] << Str[2][n3] << Str[3][n4] << Str[4][n5] << Str[5][n6] << "\n";
                    wrtL++;
                }
                else
                {
                    ftrL++;
                }
            }
            break;
        case 7:
            for(int n1 = 0; n1 < StrLen[0]; n1++)
            for(int n2 = 0; n2 < StrLen[1]; n2++)
            for(int n3 = 0; n3 < StrLen[2]; n3++)
            for(int n4 = 0; n4 < StrLen[3]; n4++)
            for(int n5 = 0; n5 < StrLen[4]; n5++)
            for(int n6 = 0; n6 < StrLen[5]; n6++)
            for(int n7 = 0; n7 < StrLen[6]; n7++)
            {
                WordLen = 0;
                WordLen += Strlen[0][n1];
                WordLen += Strlen[1][n2];
                WordLen += Strlen[2][n3];
                WordLen += Strlen[3][n4];
                WordLen += Strlen[4][n5];
                WordLen += Strlen[5][n6];
                WordLen += Strlen[6][n7];

                if(WordLen >= MinLen && WordLen <= MaxLen)
                {
                    outFile << Str[0][n1] << Str[1][n2] << Str[2][n3] << Str[3][n4] << Str[4][n5] << Str[5][n6] << Str[6][n7] << "\n";
                    wrtL++;
                }
                else
                {
                    ftrL++;
                }
            }
            break;
        case 8:
            for(int n1 = 0; n1 < StrLen[0]; n1++)
            for(int n2 = 0; n2 < StrLen[1]; n2++)
            for(int n3 = 0; n3 < StrLen[2]; n3++)
            for(int n4 = 0; n4 < StrLen[3]; n4++)
            for(int n5 = 0; n5 < StrLen[4]; n5++)
            for(int n6 = 0; n6 < StrLen[5]; n6++)
            for(int n7 = 0; n7 < StrLen[6]; n7++)
            for(int n8 = 0; n8 < StrLen[7]; n8++)
            {
                WordLen = 0;
                WordLen += Strlen[0][n1];
                WordLen += Strlen[1][n2];
                WordLen += Strlen[2][n3];
                WordLen += Strlen[3][n4];
                WordLen += Strlen[4][n5];
                WordLen += Strlen[5][n6];
                WordLen += Strlen[6][n7];
                WordLen += Strlen[7][n8];

                if(WordLen >= MinLen && WordLen <= MaxLen)
                {
                    outFile << Str[0][n1] << Str[1][n2] << Str[2][n3] << Str[3][n4] << Str[4][n5] << Str[5][n6] << Str[6][n7] << Str[7][n8] << "\n";
                    wrtL++;
                }
                else
                {
                    ftrL++;
                }
            }
            break;
        case 9:
            for(int n1 = 0; n1 < StrLen[0]; n1++)
            for(int n2 = 0; n2 < StrLen[1]; n2++)
            for(int n3 = 0; n3 < StrLen[2]; n3++)
            for(int n4 = 0; n4 < StrLen[3]; n4++)
            for(int n5 = 0; n5 < StrLen[4]; n5++)
            for(int n6 = 0; n6 < StrLen[5]; n6++)
            for(int n7 = 0; n7 < StrLen[6]; n7++)
            for(int n8 = 0; n8 < StrLen[7]; n8++)
            for(int n9 = 0; n9 < StrLen[8]; n9++)
            {
                WordLen = 0;
                WordLen += Strlen[0][n1];
                WordLen += Strlen[1][n2];
                WordLen += Strlen[2][n3];
                WordLen += Strlen[3][n4];
                WordLen += Strlen[4][n5];
                WordLen += Strlen[5][n6];
                WordLen += Strlen[6][n7];
                WordLen += Strlen[7][n8];
                WordLen += Strlen[8][n9];

                if(WordLen >= MinLen && WordLen <= MaxLen)
                {
                    outFile << Str[0][n1] << Str[1][n2] << Str[2][n3] << Str[3][n4] << Str[4][n5] << Str[5][n6] << Str[6][n7] << Str[7][n8] << Str[8][n9] << "\n";
                    wrtL++;
                }
                else
                {
                    ftrL++;
                }
            }
            break;
        default:
            throw runtime_error("exit_code: none");
            break;
    }
    cout<<"No. of Lines Written = " << wrtL <<"\n"
        <<"No. of Lines Filterd = " << ftrL <<"\n"
        <<"\n";
}


void Wordlist::File_FilterCase()
{
    for(int i = 2; i < argc; i++) // skips Combinator_v1.exe f
    {
        if(argv[i] == "-m" || argv[i] == "--min" || argv[i] == "--Max" || argv[i] == "--MIN")
        {
            parseIntOption(MinLen, argv, i);
        }
        else if(argv[i] == "-M" || argv[i] == "--max" || argv[i] == "--Max" || argv[i] == "--MAX")
        {
            parseIntOption(MaxLen, argv, i);
        }
        else if(argv[i] == "-i" || argv[i] == "--infile")
        {
            parseStringOption(inFname, argv, i);
        }
        else if(argv[i] == "-o" || argv[i] == "--outfile")
        {
            parseStringOption(outFname, argv, i);
        }
        else if(argv[i] == "-h" || argv[i] == "--help")
        {
            show_help();
            throw runtime_error("exit_code: 0");
        }
        else if(argv[i] == "-s" || argv[i] == "--showdata")
        {
            showdata = true;
        }
        else if(argv[i] == "-W" || argv[i] == "--wordlist")
        {
            wordlist = true;
        }
        else if(argv[i] == "-oc" || argv[i] == "--onlycalc")
        {
            only_calc = true;
        }
        else
        {
            cout<<"undefined Argument: " << argv[i] <<"\n";
            throw invalid_argument("exit_code: 1");
        }
    }
}

void Wordlist::Generate_Wordlist_Filtered()
{
    cout<<"Generating Wordlist(f): \n";

    inFile.open(inFname);
    if(!(inFile.is_open()))
    {
        cerr<<"Unable to Access file Named: "<< inFname <<"\n";
        throw invalid_argument("exit_code: 3");
    }

    if(only_calc == false)
    {
        outFile.open(outFname);
        if(!(outFile.is_open()))
        {
            cerr<<"Unable to Access file Named: "<< outFname <<"\n";
            throw invalid_argument("exit_code: 3");
        }
    }

    string line;

    while(getline(inFile, line))
    {
        if(line.length() >= static_cast<size_t>(MinLen) && line.length() <= static_cast<size_t>(MaxLen) )
        {
            if(only_calc == false)
            {
                outFile << line << "\n";
            }
            wrtL++;
        }
        else
        {
            ftrL++;
        }
    }
    cout<<"No. of Lines Written = " << wrtL <<"\n"
        <<"No. of Lines Filterd = " << ftrL <<"\n"
        <<"\n";
}


void Wordlist::show_help()
{
    cout<<"Combinator (v1.0.0)...                                                        \n"
        <<"                                                                              \n"
        <<"Syntax(A):  Combinator [MODE] \'salt-1\' \'salt-2\' ... \'salt-x\' [options]  \n"
        <<"Syntax(F):  Combinator [MODE] -i \'filename\' [options]                       \n"
        <<"                                                                              \n"
        <<" [Supported Modes]:  ( A | F | f )                                            \n"
        <<"        A  |  Arguments Mode - everything expected from Args                  \n"
        <<"        F  |  File Mode      - salt-x taken from file, options from Args      \n"
        <<"        f  |  Filter Mode    - filer out OutOfRange lines from wordlist       \n"
        <<"                                                                              \n"
        <<" [options]                                                                    \n"
        <<"    -r,            |   Salts used in one word                                 \n"
        <<"    -m, --Min      |   Min length of one word                                 \n"
        <<"    -M, --Max      |   Max length of one word                                 \n"
        <<"    -i, --infile   |   Input filename                                         \n"
        <<"    -o, --outfile  |   Output filename                                        \n"
        <<"    -?             |                                                          \n"
        <<"                                                                              \n"
        <<" [Flags]                                                                      \n"
        <<"  -h, --help      |   Display help menu                                       \n"
        <<"  -s, --showdata  |   Display values of all variables at backend              \n"
        <<"  -w, --winfo     |   Shows guessed metadata of wordlist                      \n"
        <<"  -W, --wordlist  |   Create wordlist                                         \n"
        <<"  -oc,--onlycalc  |   only Calcuate wordlist                                  \n"
        <<"                                                                              \n"
        <<"                                                                              \n"
        <<" [Mode specific features]                                                     \n"
        <<"    Mode(A):  { R | m | M |   | o | s | w | W | oc }                          \n"
        <<"    Mode(F):  {   | m | M | i | o | s | w | W |    }                          \n"
        <<"    Mode(f):  {   | m | M | i | o |   |   |   | oc }                          \n"
        <<"                                                                              \n"
        <<"                                                                              \n"
        <<" [input file format]                                                          \n"
        <<"   )> |1|2|3|                                                                 \n"
        <<"   )> |5|6|7|                                                                 \n"
        <<"   )> |7|8|9|                                                                 \n"
        <<"   # Comment Lines                                                            \n"
        <<"                                                                              \n"
        <<"Usage Examples(A):                                                            \n"
        <<"              Combinator A \'123\' \'abc\' \'XYZ\'                            \n"
        <<"              Combinator A \'123\' \'abc\' \'XYZ\' -r 3 -min 1 -max 4         \n"
        <<"Usage Examples(F):                                                            \n"
        <<"              Combinator F -i fname.txt                                       \n"
        <<"              Combinator F -i fname.txt -r 3 -min 4 -max 6                    \n"
        <<"                                                                              \n"
    ;
}

void Wordlist::Show_Data()
{
    cout<<"\n";
    cout<<"Showing Data: \n";
    cout<<"argc = " << argc << "\n"
        <<"argv[] = { ";
        for(int i = 0; i < argc; i++)
        {
            if( i < argc-1 )
                cout<< argv[i] << " , ";
            else
                cout<< argv[i];
        }
                cout<< " } \n \n";

    cout<<"Selected Mode = \'" << Mode << "\' \n"
        <<"R = " << R << "\n"
        <<"MinLen = " << MinLen << "\n"
        <<"MaxLen = " << MaxLen << "\n"
        <<"input file = " <<  inFname << " \n"
        <<"output file = " << outFname << "\n \n"
    ;

    cout<<"strLen = "<< strLen << "\n"
        <<"strlen[] = { ";
        for(int i = 0; i < strLen; i++)
        {
            if( i < strLen-1 )
                cout<< strlen[i] << " , ";
            else
                cout<< strlen[i];
        }
                cout<< " } \n";
    cout<<"str[] = { ";
        for(int i = 0; i < strLen; i++)
        {
            if( i < strLen-1 )
                cout<< str[i] << " , ";
            else
                cout<< str[i];
        }
                cout<< " } \n \n"
    ;

    cout<<"StrLen[] = { ";
        for(int i = 0; i < R; i++)
        {
            if( i < R-1 )
                cout<< StrLen[i] << " , ";
            else
                cout<< StrLen[i];
        }
                cout<< " } \n";
    cout<<"Strlen[][] = { ";
        for(int i = 0; i < R; i++)
        {
            for(int j = 0; j < StrLen[i]; j++)
            {
                if( j < StrLen[i]-1 )
                    cout<< Strlen[i][j] << " , ";
                else
                    cout<< Strlen[i][j];
            }
            if( i < R-1 )
                cout<<"\n               ";
            else
                cout<< " } \n";
        }
    cout<<"Str[][] = { ";
    for(int i = 0; i < R; i++)
        {
            for(int j = 0; j < StrLen[i]; j++)
            {
                if( j < StrLen[i]-1 )
                    cout<< Str[i][j] << " , ";
                else
                    cout<< Str[i][j];
            }
            if( i < R-1 )
                cout<<"\n            ";
            else
                cout<< " } \n";
        }
                cout<<"\n \n"
    ;

    cout<<"Flags:                                       \n"
        <<"show data = "          << showdata  <<"\n"
        <<"wordlist info = "      << winfo     <<"\n"
        <<"generate wordlist = "  << wordlist  <<"\n"
        <<"only Calc wordlist = " << only_calc <<"\n \n"
    ;

}

void Wordlist::Wordlist_Info()
{
    cout<<"Wordlist META: \n";
    uint64_t iGPU = 4500;
    uint64_t dGPU = 41000;
    uint64_t MaxLenofLine = 0;
    uint64_t TNoL = 0;
    if(Mode == "A")
    {
        cout<<"R(n): number of lines \n";
        for(int i = 1; i <= R; i++)
        {
            Math M;
            cout<<"R(" << i << "):  " << M.intPow_Ologn(strLen, i)  << "\n";
            TNoL += M.intPow_Ologn(strLen, i);
        }

        for(int i = 0; i < R; i++) { 
            MaxLenofLine += strlen[i]; 
        }

        SizeFormatter FB;
        Time T;

        cout<<"Total Number of Lines = "<< TNoL << "\n"
            <<"Max Length of a Line = " << MaxLenofLine << "\n"
            <<"Guessed File Size = " << FB.format_Bytes((TNoL*(MaxLenofLine+2))) << "\n"
            <<"Time Required: \n"
            // <<"   (iGPU) -> "<< (TNoL /iGPU)/60/60 << "h " << (TNoL /iGPU)/60%60 << "m " << (TNoL /iGPU)%60%60 <<"s \n"
            // <<"   (dGPU) -> "<< (TNoL/ dGPU)/60/60 << "h " << (TNoL/ dGPU)/60%60 << "m " << (TNoL/ dGPU)%60%60 <<"s \n"
            <<"   (iGPU) -> "<< T.seconds_converter(TNoL/iGPU) <<"\n"
            <<"   (dGPU) -> "<< T.seconds_converter(TNoL/dGPU) <<"\n"
            <<"\n"
        ;
    }
    else if(Mode == "F")
    {
        TNoL = 1;
        for(int i = 0; i < R; i++) { TNoL = TNoL * StrLen[i]; }
        
        cout<<"Total Number of Lines = "<< TNoL << "\n"
            <<"Time Required: \n"
            <<"   (iGPU) -> "<< (TNoL /iGPU)/60/60 << "h " << (TNoL /iGPU)/60%60 << "m " << (TNoL /iGPU)%60%60 <<"s \n"
            <<"   (dGPU) -> "<< (TNoL/ dGPU)/60/60 << "h " << (TNoL/ dGPU)/60%60 << "m " << (TNoL/ dGPU)%60%60 <<"s \n"
            <<"\n"
            ;
    }
    else
    {
        throw runtime_error("exit_code: 9");
    }

    char ch;
    cout<<"Continue (Y/N): ";
    cin>>ch;
    if(!( ch == 'Y' || ch == 'y' || ch == '1' ))
    {
        cout<<"As you Wish! \n";
        throw runtime_error("exit_code: 0");
    }
}

void Wordlist::Validate_option_Values()
{
    if(!( R >= 1 && R <= 9 ))
    {
        cout<<"Invalid value(s): \n" 
            <<"                 R = \'" << R << "\' \n"
            <<" Allowed Range is: 1 to 9. \n"
        ;
        throw runtime_error("exit_code: 2");
    }

    if(!( MinLen >= 1 && MaxLen <= 40 && MinLen <= MaxLen ))
    {
        cout<<"Invalid value(s): \n"
            <<"                 MinLen = \'" << MinLen << "\' \n"
            <<"                 MaxLen = \'" << MaxLen << "\' \n"
            <<" Rules: \n"
            <<"       MinLen must be greater than zero. \n"
            <<"       MaxLen must be greater than zero & less than MinLen. \n"
            <<"       max value of MaxLen is set to 40. \n"
        ;
        throw runtime_error("exit_code: 2");
    }
    
    if( (Mode == "A" && str[0].length() == 0) || (Mode == "F" && Str[0][0].length() == 0) )
    {
        cout<<"No salt-x are provided. \n";
        throw runtime_error("exit_code: 2");
    }

    if(Mode == "f")
    {
        if(inFname.length() == 0)
        {
            cout<<"This MODE requires an input file. \n";
            throw runtime_error("exit_code: 1");
        }
    }
}


void Wordlist::autoExecute(int argcP, char *argvP[])
{
    RunTime RT;
    ConsoleUtils console;
    console.enableANSI();

    if(argcP == 1)
    {
        cout<<"\033[32mNothing to do. \033[0m\n";
        return;
    }
    else if(argcP >= 2 && argcP <= 100)
    {
        try {
            Initializer();
            Analyze_Arguments(argcP, argvP);
            Validate_option_Values();

            if(showdata == true) { Show_Data(); }
            if(winfo == true) { Wordlist_Info(); }
            if(wordlist == true) { Generate_Wordlist(); }
        }
        catch (const std::invalid_argument &e)
        {
            cout<< e.what() << "\n";
            return;
        }
        catch (const std::exception &e)
        {
            cout<< e.what() <<"\n";
            return;
        }
        catch (...) {
            cout<<"unkown fatal error \n";
            return;
        }
        return;
    }
    else
    {
        cout<<"\033[31mArguments limit exceeded. (>100) \033[0m\n";
        return;
    }
}


int main(int argc, char *argv[])
{
    RunTime RT;
    ConsoleUtils console;
    console.enableANSI(); // enable colorful texts in window's cmd

    if(argc == 1)
    {
        cout<<"\033[32mNothing to do. \033[0m\n";
        return 0;
    }
    else if(argc >= 2 && argc <= 100)
    {
        try {
            Wordlist W;

            W.Initializer();
            W.Analyze_Arguments(argc, argv);
            W.Validate_option_Values();

            if(W.showdata == true) { W.Show_Data(); }
            if(W.winfo == true) { W.Wordlist_Info(); }
            if(W.wordlist == true) { W.Generate_Wordlist(); }
        }
        catch (const std::invalid_argument &e)
        {
            cout<< e.what() << "\n";
            return 0;
        }
        catch (const std::exception &e)
        {
            cout<< e.what() <<"\n";
            return 0;
        }
        catch (...) {
            cout<<"unkown fatal error \n";
            return 9;
        }
        return 0;
    }
    else
    {
        cout<<"\033[31mArguments limit exceeded. (>100) \033[0m\n";
        return 1;
    }
}

/*
-> Next Steps:
    - IDK for now.. maybe fix bugs? which you run into naturally while using it.

Functions:
    Initializer():
        initializes class's variables to default clean state

Error Codes:
    0:   none.
    1:   syntax error
    2:   option value error
    3:   file access error
    4:   infile error
    5:   post processing error
    9:   unexpected logical error

Wordflow:
    CMD -> No Arguments
                -> exit
    CMD -> has Arguments
            -> Initializer() - initializes class's variables to default clean state
            -> Analyze_Arguments() - identifies Mode
                -> Mode A:
                    -> Arguments_Case() - stores option values and salts
                -> Mode F:
                    -> File_Case() - stores option values
                    -> parseFile() - stores salts in 2D fashion
            -> Validate_Option_Values() - checks option values are in expected range
            -> Show_Data() - prints all data of class variables if asked
            -> Generate Wordlist() - indentifies Mode selection
                -> Mode A:
                    -> Generate_Wordlist_Arg() - create wordlist using 1D approach
                -> Mode F:
                    -> Generate_Wordlist_File() - create wordlist using 2D approach

Notes:
    stoi():
        'idx' stores index of first unconverted character.
            for 'aaa' , idx = 0
            for '01a' , idx = 2
            for '012' , idx = 3 - important!

Practices Learned:
    1) Make Functions - if you feel you are writing same lines more than three times, STOP! and make a function.
*/