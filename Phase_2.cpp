#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
char M[300][4], IR[4], GR[4];
bool C;
int PTR;
struct PCB
{
    char job[4], TTL[4], TLL[4];
} pcb;
int VA, RA;
int TTC, LLC, TTL, TLL;
int EM;
int SI, TI, PI;
int ttl, tll;
fstream fin, fout;
string line;
int random[30];
int l = -1;
int IC;
int pte;
int check1 = 0;
int InValid = 0;
void initialization();
void load();
void Pagetable();
void allocate();
void startExecution();
void executeProgram();
void AddMap();
void Examine();
void MOS();
int Errormsg();
void Terminate();
void read();
void write();

void initialization()
{
    SI = 0;
    TI = 0;
    PI = 0;
    TTC = 0;
    LLC = 0;
    TTL = 0;
    TLL = 0;
    EM = 0;
    VA = 0;
    RA = 0;
    IC = 0;
    PTR = 0;
    InValid = 0;
    for (int i = 0; i < 30; i++)
    {
        random[i] = 0;
    }
    for (int i = 0; i < 4; i++)
    {
        IR[i] = '&';
        GR[i] = '_';
    }
    for (int i = 0; i < 300; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = '_';
        }
    }
}
void printMem()
{
    for (int i = 0; i < 300; i++)
    {
        cout << "M[" << i << "]";
        for (int j = 0; j < 4; j++)
        {
            cout << M[i][j];
        }
        cout << endl;
    }
}

void load()
{
    while (fin)
    {
        int i, j;
        getline(fin, line);

        if (line[0] == '$' && line[1] == 'A' && line[2] == 'M' && line[3] == 'J')
        {
            if (check1 > 0)
            {
                printMem();
            }
            initialization();
            check1++;

            for (i = 4, j = 0; i < 8; i++, j++)
            {
                pcb.job[j] = line[i];
            }
            for (i = 8, j = 0; i < 12; i++, j++)
            {
                pcb.TTL[j] = line[i];
            }
            for (i = 12, j = 0; i < 16; i++, j++)
            {
                pcb.TLL[j] = line[i];
            }
            ttl = ((pcb.TTL[0] - 48) * 1000 + (pcb.TTL[1] - 48) * 100 + (pcb.TTL[2] - 48) * 10 + (pcb.TTL[3] - 48));
            tll = (pcb.TLL[0] - 48) * 1000 + (pcb.TLL[1] - 48) * 100 + (pcb.TLL[2] - 48) * 10 + (pcb.TLL[3] - 48);
            Pagetable();
            allocate();
        }
        if (line[0] == '$' && line[1] == 'D' && line[2] == 'T' && line[3] == 'A')
        {
            cout << "Job Execution Started" << endl;
            startExecution();
        }
    }
}
void Pagetable()
{
    int i, j;
    PTR = (rand() % 29) * 10;
    random[PTR / 10] = 1;

    for (i = PTR; i < PTR + 10; i++)
    {
        for (j = 0; j < 4; j++)
        {
            M[i][j] = '*';
        }
    }
}
void allocate()
{
    int level = 0;
    int check = 0;
    int i, j, pos = 0;
    int k = 0, len;
    char str[2];
    while (check != 1)
    {
        pos = (rand() % 29) * 10;
        while (random[pos / 10] != 0)
        {

            pos = (rand() % 29) * 10;
        }
        random[pos / 10] = 1;
        itoa(pos, str, 10);

        if (pos < 100)
        {
            M[PTR][2] = '0';
            M[PTR][3] = str[0];
        }
        else
        {
            M[PTR][2] = str[0];
            M[PTR][3] = str[1];
        }
        getline(fin, line);
        level++;
        k = 0;
        for (i = 0; i < line.size() / 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                M[pos + i][j] = line[k];
                k++;
                if (line[k] == 'H')
                {
                    check = 1;
                    M[pos + (i + 1)][0] = 'H';
                    M[pos + (i + 1)][1] = '0';
                    M[pos + (i + 1)][2] = '0';
                    M[pos + (i + 1)][3] = '0';
                }
                else
                    continue;
            }
        }
    }
}
void startExecution()
{
    IC = 0;
    executeProgram();
}
void executeProgram()
{
    int no;
    char v[3];
    int i, j, k;
    j = 0;

    v[0] = M[PTR][2];
    v[1] = M[PTR][3];
    v[2] = '\0';
    no = ((v[0] - 48) * 10) + (v[1] - 48);

    while (IR[0] != 'H')
    {
        for (int k = 0; k < 4; k++)
        {
            IR[k] = M[(no * 10) + IC][k];
        }
        if (!isdigit(IR[2]) || !isdigit(IR[3]) || isalpha(IR[2]) || isalpha(IR[3]))
        {
            PI = 2;
            if (TTC >= ttl)
                TI = 2;
            else
                TI = 0;
            MOS();
        }

        VA = ((IR[2] - 48) * 10) + (IR[3] - 48);
        AddMap();
        Examine();
    }
}
void AddMap()
{
    int pos;
    char str[2];
    pte = PTR + (VA / 10);
    if (M[pte][3] == '*')
    {
        PI = 3;
        pos = (rand() % 29) * 10;
        while (random[pos / 10] != 0)
            pos = (rand() % 29) * 10;
        random[pos / 10] = 1;
        itoa(pos, str, 10);
        if (pos / 100 == 0)
        {
            M[pte][2] = '0';
            M[pte][3] = str[0];
        }
        else
        {
            M[pte][2] = str[0];
            M[pte][3] = str[1];
        }
    }
    else
        PI = 0;
    int p = (M[pte][2] - 48) * 10 + (M[pte][3] - 48);
    RA = (p * 10) + (VA % 10);
    if (RA > 300)
    {
        PI = 2;
        TI = 0;
        MOS();
    }
}
void Examine()
{
    if (IR[0] == 'G')
    {
        IC = IC + 1;
        if (IR[1] == 'D')
        {
            SI = 1;
            TTC = TTC + 2;
            if (TTC < ttl)
                TI = 0;
            else
                TI = 2;
            MOS();
        }
        else
        {
            PI = 1;
            if (TTC > ttl)
                TI = 2;
            TI = 0;
            MOS();
        }
    }
    else if (IR[0] == 'P')
    {
        IC = IC + 1;
        if (IR[1] == 'D')
        {
            LLC = LLC + 1;
            TTC = TTC + 1;
            SI = 2;
            if (TTC < ttl)
            {
                TI = 0;
                if (PI == 3)
                {
                    InValid = 1;
                }
            }
            else
                TI = 2;
        }
        else
        {
            PI = 1;
            if (TTC >= ttl)
                TI = 2;
            else
                TI = 0;
        }
        MOS();
    }
    else if (IR[0] == 'L')
    {
        IC = IC + 1;
        if (IR[1] == 'R')
        {
            if (PI == 3)
            {
                InValid = 1;
                TI = 0;
                MOS();
            }
            else
            {
                for (int j = 0; j < 4; j++)
                    GR[j] = M[RA][j];
                TTC++;
            }
            if (TTC > ttl)
            {
                PI = 3;
                TI = 2;
                MOS();
            }
        }
        else
        {
            PI = 1;
            if (TTC >= ttl)
                TI = 2;
            TI = 0;

            MOS();
        }
    }
    else if (IR[0] == 'S')
    {
        IC = IC + 1;
        if (IR[1] == 'R')
        {
            for (int j = 0; j < 4; j++)
                M[RA][j] = GR[j];
            TTC = TTC + 2;

            if (TTC > ttl)
            {
                TI = 2;
                PI = 3;
                MOS();
            }
        }
        else
        {
            PI = 1;
            if (TTC > ttl)
                TI = 2;
            TI = 0;
            MOS();
        }
    }
    else if (IR[0] == 'C')
    {
        IC = IC + 1;
        if (IR[1] == 'R')
        {
            if (PI == 3)
            {
                InValid = 1;
                TI = 0;
                MOS();
            }
            else
            {
                if (M[RA][1] == GR[1] && M[RA][2] == GR[2] && M[RA][3] == GR[3] && M[RA][0] == GR[0])
                    C = true;
                else
                    C = false;
                TTC++;
            }
            if (TTC > ttl)
            {
                TI = 2;
                PI = 3;
                MOS();
            }
        }
        else
        {
            PI = 1;
            if (TTC > ttl)
                TI = 2;
            TI = 0;
            MOS();
        }
    }
    else if (IR[0] == 'B')
    {
        IC = IC + 1;
        if (IR[1] == 'T')
        {
            if (PI == 3)
            {
                InValid = 1;
                TI = 0;
                MOS();
            }
            else
            {
                if (C == true)
                    IC = VA;
                TTC++;
            }
            if (TTC > ttl)
            {
                TI = 2;
                PI = 3;
                MOS();
            }
        }
        else
        {
            PI = 1;
            if (TTC > ttl)
                TI = 2;
            TI = 0;
            MOS();
        }
    }
    else if (IR[0] == 'H')
    {
        IC = IC + 1;
        TTC++;
        if (TTC > ttl)
        {
            TI = 2;
            PI = 3;
            MOS();
        }
        else
        {
            SI = 3;
            MOS();
        }
    }
    else
    {
        PI = 1;
        if (TTC > ttl)
            TI = 2;
        TI = 0;
        MOS();
    }
}
void MOS()
{
    if (PI == 1)
    {
        if (TI == 0)
        {
            EM = 4;
            fout << "OpCode Error" << endl;
            Terminate();
        }
        else if (TI == 2)
        {
            EM = 3;
            fout << "Time Limit Exceeded" << endl;
            EM = 4;
            fout << "Operation Code Error" << endl;
            Terminate();
        }
    }
    else if (PI == 2)
    {
        if (TI == 0)
        {
            EM = 5;
            fout << "Operand Error" << endl;
            Terminate();
        }
        else if (TI == 2)
        {
            EM = 3;
            fout << "Time Limit Exceeded" << endl;
            EM = 5;
            fout << "Operand Error" << endl;
            Terminate();
        }
    }
    else if (PI == 3)
    {
        if (TI == 0)
        {
            if (InValid == 1)
            {
                EM = 6;
                fout << "Invalid Page Fault" << endl;
                Terminate();
            }
        }
        else if (TI == 2)
        {
            EM = 3;
            fout << "Time Limit Exceeded" << endl;
            Terminate();
        }
    }
    if (SI == 1)
    {
        if (TI == 0)
            read();
        else if (TI == 2)
        {
            EM = 3;
            fout << "Time Limit Exceeded" << endl;
            Terminate();
        }
    }
    if (SI == 2)
    {
        if (TI == 0)
            write();
        else if (TI == 2)
        {
            write();
            EM = 3;
            fout << "Time Limit Exceeded" << endl;
            Terminate();
        }
    }
    if (SI == 3)
    {
        EM = 0;
        fout << "No Error" << endl;
        Terminate();
    }
}
void Terminate()
{
    // MARK: Output job data when there's no error
    if (SI == 3 && EM == 0)
    { // When the job completes successfully
        fout << "Job completed successfully. Output:\n";
        write(); // Call write function to output job results
    }
    else if (EM != 0)
    { // When there is an error, output error details
        fout << "Job terminated with errors.\n";
    }

    // Write job termination details
    fout << "Job ID : " << pcb.job[0] << pcb.job[1] << pcb.job[2] << pcb.job[3]
         << "  TTL=" << ttl << "  TLL=" << tll << endl;
    fout << "PTR = " << PTR << " IC = " << IC << " EM = " << EM << " IR = ";
    for (int i = 0; i < 4; i++)
        fout << IR[i];
    fout << endl;
    fout << "SI = " << SI << "  TI =" << TI << "  PI=" << PI << endl;
    fout << "TTC = " << TTC << "  LLC=" << LLC << endl;
    fout << "-----------------------------------------------------------------------------------" << endl;
    fout << "-----------------------------------------------------------------------------------" << endl;

    load();  // Restart loading for the next job
    exit(0); // End current job
}

void read()
{

    getline(fin, line);
    if (line[0] == '$' && line[1] == 'E' && line[2] == 'N' && line[3] == 'D')
    {
        EM = 1;
        fout << "Out of Data" << endl;
        Terminate();
    }
    int i, j, k;
    k = 0;
    for (i = 0; k <= line.size(); i++)
    {
        for (j = 0; j < 4 && k <= line.size(); j++)
        {
            M[RA + i][j] = line[k];
            k++;
        }
    }
}
void write()
{

    char buff[40];
    int ra = 0, i, j, k;
    ra = RA;
    k = 0;
    if (LLC > tll)
    {
        EM = 2;
        fout << "Line Limit Exceeded" << endl;
        Terminate();
    }
    while (true)
    {
        for (i = 0; i < 4; i++)
        {
            if (M[ra][i] == '_')
                break;
            buff[k] = M[ra][i];
            k++;
        }
        if (M[ra][i] == '_')
            break;
        ra++;
    }
    buff[k] = '\0';
    fout << buff << endl;
}
int main()
{
    fin.open("input.txt", ios::in);
    fout.open("output.txt", ios::out);
    load();
    fin.close();
    fout.close();
    return 0;
}
