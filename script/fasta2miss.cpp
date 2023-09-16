const char* docstring=""
"fasta2miss input.list input.fasta miss.list output.fasta\n"
"\n"
"Input:\n"
"    input.list   - list of accession\n"
"    input.fasta  - input fasta\n"
"\n"
"Output:\n"
"    miss.list    - list of accession in input.list but not in input.fasta\n"
"    output.fasta - overlap set of input.list and input.fasta\n"
;

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;

void fasta2miss(const string &inputListFile, const string &inputFastaFile,
    const string &missListFile, const string &outputFastaFile)
{
    ifstream fp_in;
    ofstream fp_out;
    string line;
    
    vector<string>inputList;
    map<string,bool> inputMap;
    string txt;

    /* read input list */
    if (inputListFile!="-") fp_in.open(inputListFile.c_str());
    while ((inputListFile!="-")?fp_in.good():cin.good())
    {
        if (inputListFile!="-") getline(fp_in,line);
        else getline(cin,line);
        if (line.size()==0) continue;
        inputList.push_back(line);
        inputMap[line]=1;
        line.clear();
    }
    if (inputListFile!="-") fp_in.close();
    cerr<<inputList.size()<<" entries in "<<inputListFile<<endl;

    /* read fasta file */
    string sequence,header,header1;
    map<string,bool> headerMap;
    if (inputFastaFile!="-") fp_in.open(inputFastaFile.c_str());
    if (outputFastaFile!="-") fp_out.open(outputFastaFile.c_str());
    size_t found=0;
    size_t headerNum=0;
    size_t i;
    while ((inputFastaFile!="-")?fp_in.good():cin.good())
    {
        if (inputFastaFile!="-") getline(fp_in,line);
        else getline(cin,line);
        if (line.size()==0) continue;
        if (line[0]=='>')
        {
            for (i=0;i<header.size();i++) if (header[i]==' ') break;
            header1=header.substr(0,i);
            if (sequence.size()>0 && inputMap.count(header1))
            {
                if (outputFastaFile!="-") 
                    fp_out<<'>'<<header<<'\n'<<sequence<<endl;
                else
                    cout<<'>'<<header<<'\n'<<sequence<<endl;
                found++;
                if (found % 1000 == 0) cerr<<"Found "<<found<<" out of "
                    <<inputList.size()<<" input list among "
                    <<headerNum<<" input fasta"<<endl;
                headerMap[header1]=1;
            }
            sequence.clear();
            header=line.substr(1);
            headerNum++;
        }
        else sequence+=line;
    }
    if (inputListFile!="-") fp_in.close();
    for (i=0;i<header.size();i++) if (header[i]==' ') break;
    header1=header.substr(0,i);
    if (sequence.size()>0 && inputMap.count(header1))
    {
        found++;
        if (outputFastaFile!="-")
            fp_out<<'>'<<header<<'\n'<<sequence<<endl;
        else  cout<<'>'<<header<<'\n'<<sequence<<endl;
        headerMap[header1]=1;
    }
    if (outputFastaFile!="-") fp_out.close();
    header.clear();
    sequence.clear();
    cerr<<"Found "<<found<<" out of "<<inputList.size()
        <<" input list among "<<headerNum<<" input fasta"<<endl;

    /* write missing list */
    for (i=0;i<inputList.size();i++)
        if (headerMap.count(inputList[i])==0)
            txt+=inputList[i]+'\n';
    if (missListFile=="-") cout<<txt<<flush;
    else 
    {
        fp_out.open(missListFile.c_str());
        fp_out<<txt<<flush;
        fp_out.close();
    }

    /* clean up */
    txt.clear();
    vector<string>().swap(inputList);
    map<string,bool>().swap(inputMap);
    map<string,bool>().swap(headerMap);
    return;
}

int main(int argc, char **argv)
{
    /* parse commad line argument */
    if(argc!=5)
    {
        cerr<<docstring;
        return 0;
    }
    string inputListFile  =argv[1];
    string inputFastaFile =argv[2];
    string  missListFile  =argv[3];
    string outputFastaFile=argv[4];
    fasta2miss(inputListFile,inputFastaFile,missListFile,outputFastaFile);
    return 0;
}
