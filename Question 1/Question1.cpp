#include <iostream>
#include <pthread.h>
#include <time.h>
#include<fstream>
//id = 21K-4754
//id_A = 47, id_B = 54, id_C = 4754
int id_A = 47, id_B = 54, id_C = 4754;
using namespace std;
int count = 0;
void writestuff(){
    fstream files[id_A];
    for(int i = 0; i < id_A; i++){
        std::string n = std::to_string(i + 1);
        string filename = "File" + n + ".txt";
        files[i].open(filename, ios::out);
        files[i] << "Wrting something in file number: "<< i<<endl;
        files[i].close();
    }
}

void *readstuff(void *arg){
    count += 1;
    fstream files;
    char buffer[4754];
    std::string n = std::to_string(count);
    string filename = "File" + n + ".txt";
    files.open(filename, ios::in);
    while(!files.eof()){
        files.read(buffer,4754);
        int n = files.gcount();
        if(n > 0){
            string s(buffer, n);
            cout << s;
        }
    }
        //cout<<endl;
    files.close();
    return 0;
}
int main(){
    pthread_t threads[id_A];
    writestuff();
    for (int i = 0; i < id_A; i++)
		pthread_create(&threads[i], NULL, readstuff,(void *)NULL);
   
   for (int i = 0; i < id_A; i++)
		pthread_join(threads[i], NULL);
		
    cout<<"Hello world";
    return 0;
}
