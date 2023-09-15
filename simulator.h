#include "chordHeader.h"
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <bits/stdc++.h>
using namespace std;

class simulator
{
     HANDLE console_color;
    console_color = GetStdHandle(
        STD_OUTPUT_HANDLE);
    int i;
    node *n = nullptr;
    void displayLine()
    {
        SetConsoleTextAttribute(console_color, 50);
        std::cout << "=============================================================" << std::endl;
    }
 
    int instructionDecode(std::string s)
    {
        if (s == "create-network")
            return 0;
        else if (s == "node-join")
            return 1;
        else if (s == "key-find")
            return 2;
        else if (s == "delete-node")
            return 3;
        else if (s == "exit")
            return 5;
        else if (s == "display")
            return 6;
        else
            return -1;
    }

    void instructionExecute(int instruction, int key)
    {
        node *ntemp = nullptr;
        switch (instruction)
        {
        case 0:
            std::cout << "Creating network . . ." << std::endl;
            std::cout << "Inserting node " << (key == -1 ? 0 : key) << " in the network." << std::endl;
            n = new node((key == -1 ? 0 : key));
            n->createNetwork();
            break;
        case 1:
            std::cout << "Executing Node Join . . ." << std::endl;
            std::cout << "Inserting node " << key << " in the network." << std::endl;
            ntemp = new node(key);
            ntemp->join(n);
            break;
        case 2:
            std::cout << "Executing Key Find . . ." << std::endl;
            ntemp = n->findKey(key % N);
            std::cout << "Key: " << key << " found at Node: " << ntemp->identifier << std::endl;
            break;
        case 3:
            std::cout << "Executing Node delete . . ." << std::endl;
            if (key == n->identifier)
                n = n->successor;
            ntemp = n->findKey(key);
            if (ntemp->identifier == key)
            {
                std::cout << "Deleting node " << key << " from the network." << std::endl;
                ntemp->nodeLeave();
            }
            else
            {
                std::cout << "Error no such node found . . ." << std::endl;
            }
            break;
        case 4:
            break;
        case 5:
            exit(0);
            break;
        case 6:
            if (key != -1)
            {
                ntemp = n->findKey(key);
                if (ntemp->identifier == key)
                    ntemp->showFingerTable();
                else
                {
                    std::cout << "Error no such node found . . ." << std::endl;
                }
            }
            else
            {
                ntemp = n->findKey(0);
                display(ntemp);
            }
            break;
        default:
            break;
        }
    }

    void display(node *t)
    {
        node *temp = t;
        do
        {
            t->showFingerTable();
            t = t->successor;
        } while (t != temp);
    }

public:
    void runSimulator()
    {
        std::string s;
        std::cout << ">> ";
        while (getline(std::cin, s))
        {
            int phase = 0;
            std::string arg = "";
            std::string errorMessage = "";
            int instruction = -1;
            int key = -1;
            for (auto c : s)
            {
                if (c == ' ')
                {
                    if (phase == 0)
                    {
                        instruction = instructionDecode(arg);
                        arg = "";
                        phase = 1;
                    }
                }
                if (c != ' ')
                {
                    arg = arg + c;
                }
            }
            if (phase == 0)
            {
                if (arg == "")
                {
                    std::cout << ">> ";
                    continue;
                }
                else
                {
                    instruction = instructionDecode(arg);
                }
            }
            else
            {
                key = stoi(arg);
            }

            if (instruction == -1)
            {
                std::cout << "error: \"" << s << "\" : no such command found" << std::endl;
            }
            else if (instruction != 0 && !n && instruction != 5 && instruction != 4)
            {
                std::cout << "error: create a network first" << std::endl;
            }
            else if ((instruction == 1 || instruction == 2 || instruction == 3) && key == -1)
            {
                std::cout << "error: need to give arguments try again." << std::endl;
            }
            else
                instructionExecute(instruction, key);
            if (n)
            {
                node *t = n;
                do
                {
                    n->stablizeNode();
                    n = n->successor;
                } while (t != n);
            }
            std::cout << ">> ";
        }
    };
    void headerDisplay()
    {
        displayLine();
        cout <<endl;
        cout << "\t\tCODE BY BHUSHAN SHRIRAME 23CS06002" << endl;
        cout << "\t\tWelcome to Chord Simulator" << endl;
        cout <<endl;

        displayLine();
        cout <<endl;
        cout << "Here are multiple commands which you can run " << endl;
        cout << ">> create-network [node ID](integer)" << endl;
        cout <<endl;
        cout << ">> node-join nodeID(integer)" <<endl;
        cout << std::endl;
        cout << ">> key-find keyID(integer)" <<endl;
        cout << std::endl; 
        cout << ">> delete-node nodeID(integer)" << endl;
        cout << std::endl;
        cout << ">> display [nodeID](integer)" << endl;
        cout <<endl;
        cout << ">> exit " <<endl;
        cout <<endl;
        displayLine();
        cout <<endl;
    }
};