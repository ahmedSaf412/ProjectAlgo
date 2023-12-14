#include <iostream>
#include <fstream>
#include <sstream>
#include<vector>
#include<cstdlib>
#include<list>
#include<iomanip>
#include<ctime>


using namespace std;

class graph{
    public:
    
    graph(int vertices){
        m_vertices = vertices;
        m_neighbours = new list<int>[vertices];
    }
    
    void follow_reg(int src, int dist);

    int m_vertices;
    list<int>* m_neighbours;

};

void graph::follow_reg(int src, int dist){
    m_neighbours->push_back(src);
}



void RandomID2(ofstream& outputfile) {
    int accs = 60000;
    
    if (outputfile.is_open()) {
        srand(static_cast<unsigned int>(time(0)));
        
        for (int i = 0; i < accs; ++i) {
            int ID = 1 + rand() % accs;
            outputfile << ID << endl;  // Assuming each ID should be on a new line
        }
        
        outputfile.close(); 
        cout << "Dummy IDs generated and saved to dummy_ids.txt." << endl;
    }
    else {
        cerr << "Unable to open file for writing." << endl;
    }
}


int main(){
    graph data(1000);

    ofstream outputfile("dummy_ids.txt");
    RandomID2(outputfile);

    return 0;
    return 0;
}