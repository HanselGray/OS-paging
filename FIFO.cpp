#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

const int FRAME_SIZE = 4;
const int VIRTUAL_ADDRESS_SPACE = 32;
int page_fault_count = 0,n, maximum_no_frame, allocated =0;
struct Frame {
    bool occupied;
    int page_number;
    int phyiscal_address;

    // Frame(bool o, int pn, int pa) : occupied(o), page_number(pn), physical_address(pa) {}
};

struct Process {
    int id;
    int size;
    vector<Frame>page_numbers;
    queue<int>evict;
};

void swap_frame(Process *process,page_id,page_id_swap)
{
    *process.page_numbers[page_id].occupied = true;

}


int main(){


    cout << "How many processes?\n";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; ++i) {
        int no_of_pages;
        cout << "Enter the number of pages for process " << i << ":\n";
        cin >> no_of_pages;

        Process temp;
        temp.id = i;

        cout << "Enter the page numbers for process " << i << ":\n";
        for (int j = 0; j < no_of_pages; ++j) {
            temp.page_numbers.emplace_back({false,j,-1});
        }

        processes[i] = temp;
    }

    cout << "Enter the maximum number of frames in physical memory:\n";
    cin >> maximum_no_frame;
    vector<int>physical_memory(maximum_no_frame);


    cout << "Number of frame access request:\n";
    cin >> n;
    vector<pair<int,int>>request(n);
    for(int i = 0; i<n;++i){
        int t1,t2;
        cin>>t1>>t2;
        request.emplace_back(make_pair(t1,t2));
    }

    for(int i = 0 ; i<n;++i){
        bool page_fault = true;

        // inputing processes request for a page.
        int process_id, page_id;
        cin >>process_id>>page_id;
        bool page_fault = true;

        //check if page is present inside of memory
        if(processes[process_id][page_id].occupied = true ){
            page_fault = false;
        }

        // Frame allocate in fault case
            if (page_fault) {
                ++page_fault_count;
                for(int j=0;j<physical_memory.size();++i){

                    //if memory have no free frame, must swap
                    if(allocated == physical_memory.size()){
                        int evicted_id = proccess[process_id].evict.pop();
                        swap_frame(*processes[process_id],page_id,evicted_id);
                        break;
                    }
                    // frame is free
                    else if(physical_memory[i]== '-1'){
                        processes[process_id].page_numbers[page_id].occupied = true;
                        processes[process_id].page_numbers[page_id].phyiscal_address = j;
                        physical_memory[j] = new; // indicate this frame inside memory have data
                        processes[process_id].evict.push(page_id);
                        break;
                    }
                    cout << "Process "<<process_id<<"Page fault, allocated page to frame: "<<processes[process_id].page_numbers[page_id].phyiscal_address;
                }
            }
            else{
                ++page_hit;
                cout << "Process "<<process_id<<"Page hit in: "<<processes[process_id].page_numbers[page_id].phyiscal_address;
            }
        }

    cout << "\nTotal Page Faults: " << page_faults << endl;

    return 0;
}
