#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int FRAME_SIZE = 4;
const int VIRTUAL_ADDRESS_SPACE = 32;
int page_fault_count = 0, n, maximum_no_frame, free_frame, page_hit = 0;
struct Frame
{
    bool occupied;
    bool referenced = 0;  // shield
    int physical_address;
    Frame(bool o, int pa) : occupied(o), physical_address(pa) {}
};

struct Proc
{
    int id;
    int size;
    vector<Frame> page_numbers;
    vector<int> evict;
};

Proc swap_frame(Proc process, int page_id, int page_id_swap)
{
    process.page_numbers[page_id].occupied = true;
    process.page_numbers[page_id_swap].occupied = false;
    process.page_numbers[page_id].physical_address = process.page_numbers[page_id_swap].physical_address;

    return process;
}

int main()
{

    cout << "How many processes?\n";
    cin >> n;

    vector<Proc> processes(n);

    for (int i = 0; i < n; ++i)
    {
        int no_of_pages;
        cout << "Enter the number of pages for process " << i << ":\n";
        cin >> no_of_pages;

        Proc temp;
        temp.id = i;

        for (int j = 0; j < no_of_pages; ++j)
        {
            temp.page_numbers.emplace_back(Frame(false, -1));
        }

        processes[i] = temp;
    }

    cout << "Enter the maximum number of frames in physical memory:\n";
    cin >> maximum_no_frame;
    free_frame = maximum_no_frame;
    vector<int> physical_memory(maximum_no_frame, -1);

    cout << "Number of frame access request:\n";
    cin >> n;

    for (int i = 0; i < n; ++i)
    {

        // inputing processes request for a page.
        int process_id, page_id;
        cin >> process_id >> page_id;

        // check if page is present inside of memory
        if (processes[process_id].page_numbers[page_id].occupied)
        {
            ++page_hit;
            processes[process_id].page_numbers[page_id].referenced = 1;
            cout << "Process " << process_id << " Page hit, frame number: " << processes[process_id].page_numbers[page_id].physical_address<<'\n';
        }

        // Frame allocate in fault case
        else
        {
            ++page_fault_count;
            // if memory have no free frame, must swap
            if (free_frame <= 0)
            {   
                vector<int>tmp_evict = processes[process_id].evict;
                // TODO: int evicted_id = get_evicted_id(string algorithm, processes)
                int evict_idx = 0;

                for (int j = 0; j < tmp_evict.size(); ++j) {

                    if(processes[process_id].page_numbers[tmp_evict[j]].referenced) {
                        processes[process_id].page_numbers[tmp_evict[j]].referenced = 0;
                    }
                    else{
                        int evict_idx = j;
                    }
                }
                int evicted_id = tmp_evict[evict_idx];
                processes[process_id] = swap_frame(processes[process_id], page_id, evicted_id);
                
                processes[process_id].evict.erase(processes[process_id].evict.begin()+evict_idx);
                processes[process_id].evict.push_back(page_id);

                cout<<"Memory full, swapping page in memory: "<< evicted_id << " with page: "<< page_id <<'\n';
                break;
            }

            // allocate to first empty frame inside physical memory
            else
            {
                for (int j = 0; j < physical_memory.size(); ++j)
                {
                    // check if frame is free
                    if (physical_memory[j] == -1)
                    {
                        processes[process_id].page_numbers[page_id].occupied = true;
                        processes[process_id].page_numbers[page_id].physical_address = j;
                        physical_memory[j] = 1; // indicate this frame inside memory have data
                        processes[process_id].evict.push_back(page_id);
                        --free_frame;
                        break;
                    }
                }               
            }
            cout << "Process " << process_id << " Page fault, allocated page to frame: " << processes[process_id].page_numbers[page_id].physical_address<<'\n';
        }
    }

    cout << "\nTotal Page Faults: " << page_fault_count << endl;

    return 0;
}
