#include <string.h>
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

struct file_object{
    string name;
    string clearance;
    list<file_object*> contents;
    file_object* parent;
    bool dir;
};

list<file_object*> objects = list<file_object*>();
vector<string> clearence_levels = vector<string>();
map<string, string> users = map<string, string>();
string current_user = "Alice";

file_object* parse(string input, file_object* current_dir, string clearance){

    char* cstr = new char [input.length()+1];
    strcpy(cstr, input.c_str());

    char* op_c = strtok(cstr, " ");
    char* arg_c = strtok(NULL, " ");
    char* arg2_c = strtok(NULL, " ");
    string op, arg, arg2;

    if(arg_c != NULL){
        arg = arg_c;
    }

    if(arg2_c != NULL){
        arg2 = arg2_c;
    }

    if(op_c != NULL){
        op = op_c;
    }
    
    delete[] cstr;

    if(op.compare("ls") == 0){

        cout << "=====================\n";
        cout << "Clearence levels\n";
        cout << "-----------------------\n";

        vector<string>::reverse_iterator it_clearence;
        int number = 1;
        for(it_clearence = clearence_levels.rbegin(); it_clearence != clearence_levels.rend(); it_clearence++){
            string user = *it_clearence;
            cout << number;
            cout << ". ";
            cout << user;
            cout << "\n";
            number++;
        }

        cout << "=====================\n";


        if(current_dir->contents.empty()){
            cout << "Empty directory\n";
        }
        else{
            cout << "Directory contents\n";
            cout << "-----------------------\n";
            cout << "Name | Type | Classification\n";
            cout << "-----------------------\n";

            list<file_object*>::iterator it;
            for(it = current_dir->contents.begin(); it != current_dir->contents.end(); it++){
                file_object* curr_dir_obj = *it;
                string output = curr_dir_obj->name;
                output.append(" ");
                if(curr_dir_obj->dir){
                    output.append("dir");
                }
                else{
                    output.append("file");
                }
                output.append(" ");
                output.append(curr_dir_obj->clearance);
                output.append("\n");
                cout << output;
            }
        }

        cout << "=====================\n";
        cout << "Users\n";
        cout << "-----------------------\n";
        cout << "Name | Clearence\n";
        cout << "-----------------------\n";

        map<string,string>::iterator it_users;
        for(it_users = users.begin(); it_users != users.end(); it_users++){
            pair<string, string> user = *it_users;
            string output = user.first;
            output.append(" ");
            output.append(user.second);
            output.append("\n");
            cout << output;
        }

        cout << "=====================\n";

    }
    else if(op.compare("cd") == 0){

        if(arg.empty()){
            cout << "No argument provided.\n";
            return current_dir;
        }

        file_object* curr_obj_dir;

        if(arg.compare("..") == 0){
            if(current_dir->parent == NULL){
                return current_dir;
            }

            curr_obj_dir = current_dir->parent;
        }
        else{

            auto it = find_if(begin(current_dir->contents), end(current_dir->contents), [arg] (file_object* f_obj) {return f_obj->name.compare(arg) == 0;});
            if(it == current_dir->contents.end()){
                cout << "Directory not found\n";
                return current_dir;
            }
            
            curr_obj_dir = *it;

            if(!curr_obj_dir->dir){
                cout << "Not a directory\n";
                return current_dir;
            }

            auto it_cleareance = find(clearence_levels.begin(), clearence_levels.end(), clearance);
            auto it_classification = find(clearence_levels.begin(), clearence_levels.end(), curr_obj_dir->clearance);

            if(distance(clearence_levels.begin(), it_cleareance) < distance(clearence_levels.begin(), it_classification)){
                cout << "CHYBA OPRAVNENI\n";
                return current_dir;
            }            

        }

        return curr_obj_dir;
    }
    else if(op.compare("touch") == 0){

        if(arg.empty()){
            cout << "No argument provided.\n";
            return current_dir;
        }

        auto it = find_if(begin(current_dir->contents), end(current_dir->contents), [arg] (file_object* f_obj) {return f_obj->name.compare(arg) == 0;});

        if(it != current_dir->contents.end()){
            cout << "File already exists\n";
            return current_dir;
        }
        else{
            file_object* file = new file_object;
            file->name = arg;
            file->clearance = clearance;
            file->parent = current_dir;
            file->contents = list<file_object*>();
            file->dir = false;
            current_dir->contents.push_back(file);
            objects.push_back(file);
        }

    }
    else if(op.compare("mkdir") == 0){

        if(arg.empty()){
            cout << "No argument provided.\n";
            return current_dir;
        }

        auto it = find_if(begin(current_dir->contents), end(current_dir->contents), [arg] (file_object* f_obj) {return f_obj->name.compare(arg) == 0;});

        if(it != current_dir->contents.end()){
            cout << "Directory already exists\n";
            return current_dir;
        }
        else{
            file_object* dir = new file_object;
            dir->name = arg;
            dir->clearance = clearance;
            dir->parent = current_dir;
            dir->contents = list<file_object*>();
            dir->dir = true;
            current_dir->contents.push_back(dir);
            objects.push_back(dir);
        }
    }
    else if(op.compare("rm") == 0){

        if(arg.empty()){
            cout << "No argument provided.\n";
            return current_dir;
        }

        auto it = find_if(begin(current_dir->contents), end(current_dir->contents), [arg] (file_object* f_obj) {return f_obj->name.compare(arg) == 0;});
        
        if(it == current_dir->contents.end()){
            cout << "File/Directory not found\n";
            return current_dir;
        }
        else{
            file_object* curr_dir_obj = *it;
            if(curr_dir_obj->dir && curr_dir_obj->contents.empty() == false){
                cout << "Directory still has contents\n";
                return current_dir;
            }
            objects.remove(curr_dir_obj);
            current_dir->contents.erase(it);
            delete curr_dir_obj;
        }
    }
    else if(op.compare("write") == 0){

        if(arg.empty()){
            cout << "No argument provided.\n";
            return current_dir;
        }

        auto it = find_if(begin(current_dir->contents), end(current_dir->contents), [arg] (file_object* f_obj) {return f_obj->name.compare(arg) == 0;});
        file_object* curr_dir_obj;

        if(it == current_dir->contents.end()){
            cout << "File not found\n";
            return current_dir;
        }
        else{
            curr_dir_obj = *it;
            if(curr_dir_obj->dir){
                cout << "Not a file\n";
                return current_dir;
            }
        }

        auto it_cleareance = find(clearence_levels.begin(), clearence_levels.end(), clearance);
        auto it_classification = find(clearence_levels.begin(), clearence_levels.end(), curr_dir_obj->clearance);

        if(distance(clearence_levels.begin(), it_cleareance) > distance(clearence_levels.begin(), it_classification)){
            cout << "CHYBA OPRAVNENI\n";
            return current_dir;
        }

        cout << "OK\n";
    }
    else if(op.compare("read") == 0){

        if(arg.empty()){
            cout << "No argument provided.\n";
            return current_dir;
        }

        auto it = find_if(begin(current_dir->contents), end(current_dir->contents), [arg] (file_object* f_obj) {return f_obj->name.compare(arg) == 0;});
        file_object* curr_dir_obj;

        if(it == current_dir->contents.end()){
            cout << "File not found\n";
            return current_dir;
        }
        else{
            curr_dir_obj = *it;
            if(curr_dir_obj->dir){
                cout << "Not a file\n";
                return current_dir;
            }
        }

        auto it_cleareance = find(clearence_levels.begin(), clearence_levels.end(), clearance);
        auto it_classification = find(clearence_levels.begin(), clearence_levels.end(), curr_dir_obj->clearance);

        if(distance(clearence_levels.begin(), it_cleareance) < distance(clearence_levels.begin(), it_classification)){
            cout << "CHYBA OPRAVNENI\n";
            return current_dir;
        }

        cout << "OK\n";
    }
    else if(op.compare("chmod") == 0){

        if(arg.empty()){
            cout << "File/Directory argument missing\n";
            return current_dir;
        }

        if(arg2.empty()){
            cout << "Clearence argument missing\n";
            return current_dir;
        }

        auto it = find_if(begin(current_dir->contents), end(current_dir->contents), [arg] (file_object* f_obj) {return f_obj->name.compare(arg) == 0;});
        file_object* curr_dir_obj;

        if(it == current_dir->contents.end()){
            cout << "File not found\n";
            return current_dir;
        }
        else{
            curr_dir_obj = *it;
        }

        auto it_assign_class = find(clearence_levels.begin(), clearence_levels.end(), arg2);

        if(it_assign_class == clearence_levels.end()){
            cout << "Such clearance level does not exist\n";
            return current_dir;
        }

        auto it_cleareance = find(clearence_levels.begin(), clearence_levels.end(), clearance);
        auto it_classification = find(clearence_levels.begin(), clearence_levels.end(), curr_dir_obj->clearance);

        if(distance(clearence_levels.begin(), it_cleareance) < distance(clearence_levels.begin(), it_classification)){
            cout << "CHYBA OPRAVNENI\n";
            return current_dir;
        }

        if(distance(clearence_levels.begin(), it_cleareance) < distance(clearence_levels.begin(), it_assign_class)){
            cout << "CHYBA OPRAVNENI\n";
            return current_dir;
        }

        curr_dir_obj->clearance = arg2;

    }
    else if(op.compare("usrmod") == 0){

        if(arg2.empty()){
            cout << "Missing user argument.\n";
            return current_dir;
        }

        if(arg.empty()){
            cout << "Missing clearence argument\n";
            return current_dir;
        }

        if(users.find(arg2) == users.end()){
            cout << "User not found\n";
            return current_dir;
        }

        auto it = find(clearence_levels.begin(), clearence_levels.end(), arg);
        
        if(it == clearence_levels.end()){
            cout << "Clearence level not found\n";
            return current_dir;
        }

        users[arg2] = arg;
    }
    else if (op.compare("usrswtch") == 0){
        if(arg.empty()){
            cout << "Missing user argument.\n";
            return current_dir;
        }

        if(users.find(arg) == users.end()){
            cout << "User not found\n";
            return current_dir;
        }

        current_user = arg;

    }
    else 
    {
        cout << "Unknown command\n";
    }

    return current_dir;
    
}

string get_dir_structure(file_object* current_dir){
    string structure = " ";
    file_object* dir = current_dir;
    structure.insert(0, dir->name);

    while(dir->parent != NULL){
        dir = dir->parent;
        structure.insert(0, "/");
        structure.insert(0, dir->name);
    }

    structure.insert(0, "# ");

    return structure;
}

int main(){

    ifstream config("config.txt");
    string config_contents;

    while(getline(config, config_contents)){
        config_contents.erase(remove(config_contents.begin(), config_contents.end(), '\r'), config_contents.end());
        clearence_levels.push_back(config_contents);
    }

    users.insert(make_pair("Alice", clearence_levels[0]));
    users.insert(make_pair("Bob", clearence_levels[1]));
    users.insert(make_pair("David", clearence_levels[2]));
    users.insert(make_pair("Kate", clearence_levels[3]));

    file_object* current_dir = new file_object;
    current_dir->name = "root";
    current_dir->clearance = config_contents[0];
    current_dir->parent = NULL;
    current_dir->contents = list<file_object*>();
    current_dir->dir = true;

    objects.push_front(current_dir);

    string input;

    while(true){
        cout << current_user;
        cout << get_dir_structure(current_dir);
        getline(cin, input);
        if(input.compare("exit") == 0){
            break;
        }
        current_dir = parse(input, current_dir, users.at(current_user));
    }

    list<file_object*>::iterator it;
    for(it = objects.begin(); it != objects.end(); it++){
        file_object* object = *it;
        delete object;
    }

    return EXIT_SUCCESS;
}