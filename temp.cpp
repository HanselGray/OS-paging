#include<iostream>
#include <cstdlib>
using namespace std;
int n, maximum_no_frame;

const int FRAME_SIZE = 4, VIRTUAL_ADDRESS_SPACE = 32;

struct frame{
    bool mark;
    int data[FRAME_SIZE];
};

struct process{
    int id;
    int size;
    vector<int>page_numbers;
};

struct page_table{
    
}


int main()
{
    cout<<"How many process?\n";
    cin>>n;
    
    vector<process>a(n);

    for(int i=0;i<n;++i){

        int no_of_page;
        cin>>no_of_page;
        process temp;
        
        temp.id = i;

        for(int j = 0 ; j< no_of_page;++j){
            int tmp; 
            cin>>tmp;
            temp.page_numbers.push_back(tmp);
        }

        a.push_back(temp);
    }

    cout<<"Maximum number of frame in physical memory?:\n";
    cin>>maximum_no_frame;

    vector<int>physical_memory(maximum_no_frame);

    for(int i=)
}