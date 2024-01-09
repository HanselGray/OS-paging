#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;

int page_fault_count = 0, n, maximum_no_frame, free_frame, page_hit = 0;

struct Page
{
    bool referenced;
    bool occupied;
    int frame_number;
    Page(bool o, int frn) : occupied(o), frame_number(frn), referenced(0) {}
};

struct Proc
{
    vector<Page> page_table;   
    vector<int> evict; 
};

Proc swap_frame(Proc proc, int page_id_new, int page_id_old)
{
    Proc process = proc;
    process.page_table[page_id_old].occupied = false;
    process.page_table[page_id_new].occupied = true;
    process.page_table[page_id_new].frame_number = process.page_table[page_id_old].frame_number;
    return process;
}

Proc update_replace(Proc process, int hit_id){
    process.evict.erase(process.evict.begin());
    process.evict.push_back(hit_id);

    return process;
}

void draw_frames(vector<int> physical_memory) {
    // Initialize the table

    int n = physical_memory.size();
    cout<<"CURRENT PHYSICAL MEMORY STATE: \n\n";
    cout << "Frame no: ";
    for (int i = 0; i < n; ++i) {
        cout << "  " << setw(2) <<  i << "   ";
    }

    cout << "\n         +";

    for (int i = 0; i < n; ++i) {
        cout << "------+";
    }
    cout << "\nPage no: |";
    for (int i = 0; i < n; ++i) {
        cout << "  " << setw(2) << physical_memory[i] << "  |";
    }

    cout << "\n         +";
    for (int i = 0; i < n; ++i) {
        cout << "------+";
    }
    cout << "\n";

}



int main()
{   
    Proc process;
    int no_of_pages;
    cout << "Enter the number of pages for the process:\n";
    cin >> no_of_pages;

    for (int j = 0; j < no_of_pages; ++j)
    {
        process.page_table.emplace_back(Page(false, -1));
    }


    cout << "Enter the maximum number of frames in physical memory:\n";
    cin >> maximum_no_frame;
    free_frame = maximum_no_frame;

    vector<int> physical_memory(maximum_no_frame, -1);
    draw_frames(physical_memory);
    cout<<"BEGIN OF SIMULATION\n--------------------------------------------------------------------------------------------------------------\n\n";
   
    int page_id;
    cout<<"Enter a page number process want to access: ";
    cin>>page_id;
    while(page_id != -1)
    {
        //prevent accessing invalid section
        if(page_id >= process.page_table.size()) {
            cout <<  "Invalid reference.\n";
            draw_frames(physical_memory);
        }

        else
        {
        // check if page is present inside of memory
        if (process.page_table[page_id].occupied)
        {
            ++page_hit;
            
            process.page_table[page_id].referenced = 1;

            cout <<  "Page hit, page: " << page_id << " at frame " << process.page_table[page_id].frame_number<< ".\n";
            draw_frames(physical_memory);
        }

        // Frame allocate in fault case
        else
        {
            ++page_fault_count;
            // if memory have no free frame, must swap
            if (free_frame <= 0)
            {
                int evicted_id = process.evict[0];
                for (int j = 0; j < process.evict.size(); ++j) {

                    int curr_evicted_id = process.evict[j];
                    if(process.page_table[curr_evicted_id].referenced == 0) {
                        evicted_id = curr_evicted_id;
                        break;
                    } else {
                        process.page_table[curr_evicted_id].referenced = 0;
                    }
                }    
                int current_frame = process.page_table[evicted_id].frame_number;

                
                
                process = swap_frame(process, page_id, evicted_id); // Update page table
                physical_memory[current_frame] = page_id;

                // Update recency of pages
                process.evict.erase(process.evict.begin());


                cout<<"Memory full, swapping page: "<< evicted_id << " with page: "<< page_id << " at frame: " << current_frame << "\n";
                draw_frames(physical_memory);
            }

            // allocate to first empty frame inside physical memory
            else
            {
                for (int j = 0; j < physical_memory.size(); ++j)
                {
                    // check if frame is free
                    if (physical_memory[j] == -1)
                    {
                        process.page_table[page_id].occupied = true;
                        process.page_table[page_id].frame_number = j;
                        physical_memory[j] = page_id; // indicate this frame has page with id = page_id
                        process.evict.push_back(page_id);
                        --free_frame;
                        break;
                    }
                }               
                cout << "Page fault, allocated page to frame: " << process.page_table[page_id].frame_number<<'\n';
                draw_frames(physical_memory);
            }
        }
        }
        cout<<"\n--------------------------------------------------------------------------------------------------------------\n\n";
        // inputing processes request for a page.
        cout<<"Enter a page number process want to access: ";
        cin>>page_id;
    }

    // Print report
    cout<<"\n--------------------------------------------------------------------------------------------------------------\nEND OF SIMULATION";
    cout<<"\nTotal page access request: "<<page_hit+page_fault_count<<"\nHit count: "<<page_hit<<"\nTotal Page Faults: "<< page_fault_count<<'\n';

    return 0;
}
