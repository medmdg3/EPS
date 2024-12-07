#include "bits/stdc++.h"
#include "ExternalStorage.hpp"
#include "Files.hpp"
MEM memi("memory.txt");
char buffer[10000];
vector<string> Split(string a,string banned=" \t"){
    a+=banned[0];
    vector<string> ans;
    string temp;
    for(int i=0;i<a.size();i++){
        temp+=a[i];
        for(auto &j:banned)
            if(a[i]==j){
                temp.pop_back();
                if(temp.size())ans.push_back(temp);
                temp="";
                break;
            }

    }
    return ans;
}
bool Manager(NVFile &file){
    string command;
    getline(cin,command);
    vector<string> commands=Split(command);
    for(int i=0;i<5;i++)commands.push_back("");
    if(commands[0]=="Exit") return false;
    if(commands[0]=="Add"){
        if(commands[1]=="Entry"){
            string Text= Split(command,"\"")[1];
            strcpy(buffer,Text.c_str());
            file.Add_Entry((std::uint8_t*)buffer,Text.size());
            return true;
        }
        cout<<"Unknown command!"<<endl;
    }
    if(commands[0]=="Delete"){
        if(commands[1]=="Entry"){
            string Text= commands[2];
            strcpy(buffer,Text.c_str());
            unsigned id;
            sscanf(buffer,"%u",&id);
            if(!file.Delete_Entry(id))
                cout<<"Cannot delete the cell!"<<endl;
            return true;
        }
        cout<<"Unknown command!"<<endl;
    }
    if(commands[0]=="Read"){
        if(commands[1]=="Entry"){
            string Text= commands[2];
            strcpy(buffer,Text.c_str());
            unsigned id;
            sscanf(buffer,"%u",&id);
            if(file.Read_Cell(id,(std::uint8_t*)buffer)){
                printf("%s\n",buffer+file.Cell_Header_Size);
            }else cout<<"Cannot read the cell!"<<endl;
            return true;
        }
        if(commands[1]=="All"){
            for(int id=1;id<=file.used_cells;id++){
                if(file.Read_Cell(id,(std::uint8_t*)buffer)){
                    printf("%s\n",buffer+file.Cell_Header_Size);
                }else cout<<"Cannot read the cell!"<<endl;
            }
            return true;
        }
        cout<<"Unknown command!"<<endl;
    }
    if(commands[0]=="Get"){
        if(commands[1]=="Infos"){
            printf("File name: %s\n",file.file_name);
            printf("Id: %u\n",file.file_id);
            printf("Status: %d\n",file.status);
            printf("File status: %d\n",file.file_status);
            printf("last entry cell id: %u\n",file.last_entry_cell_id);
            printf("total entries: %u\n",file.total_entries);
            printf("cell Size: %u\n",file.cell_Size);
            printf("used cells: %u\n",file.used_cells);
            printf("max cells: %u\n",file.max_cells);
            return true;
        }
        cout<<"Unknown command!"<<endl;
    }
    if(commands[0]=="Clear"){
        file.Clear();
        return true;
    }

    if(commands[0]=="Deep"){
            if(commands[1]=="Clean"){
                file.Deep_Clean();
                return true;
            }
            cout<<"Unknown command!"<<endl;
    }
    if(commands[0]=="Errors"){
        cout<<"Writing errors: "<<file.errors.Writing_Errors<<"\nReading errors:"<<file.errors.Reading_Errors<<"\nCRC errors: "<<file.errors.CRC_Errors<<endl;
        return true;
    }
    cout<<"Unknown command!"<<endl;
    return true;
}
int main() {
    NVFile file(memi,0x00000044);
    cout<<file.used_cells<<endl;
    while(Manager(file));
}
