#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include<climits>
using namespace std;

int page_fault_count = 0, n, maximum_no_frame, free_frame, page_hit = 0;

struct Page
{
    bool occupied;
    int frame_number;
    Page(bool o, int frn) : occupied(o), frame_number(frn) {}
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

Proc update_hit(Proc process, int hit_id){
    ++process.evict[hit_id];            
    return process;
}

Proc update_replace(Proc process, int page_id){
    int min = INT_MAX,evict_id =0;
    for(int i=0;i< process.evict.size();++i){
        if(process.evict[i] < min && process.page_table[i].occupied){
            evict_id = i;
            min = process.evict[i];
        } 
    }
    
    process = swap_frame(process, page_id, evict_id);

    int current_frame = process.page_table[page_id].frame_number;
    cout<<"Memory full, swapping page: "<< evict_id << " with page: "<< page_id << " at frame: " << current_frame << "\n";

    process.evict[evict_id] = 0;
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
    cout<<"Enter the number of pages for the process:\n";
    cin >> no_of_pages;
    //init evict to track frequency
    process.evict.assign(no_of_pages,0);
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
    cout<<"REQUEST NUMBER: "<<page_fault_count+page_hit+1<<"\nEnter a page number process want to access: ";
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
            
            // Update recency of pages
            process = update_hit(process, page_id);

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
                     
                // Update recency of pages
                process = update_replace(process, page_id);

                int current_frame = process.page_table[page_id].frame_number;

                physical_memory[current_frame] = page_id;
                
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
                        process.evict[page_id] = 1;
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
        cout<<"REQUEST NUMBER: "<<page_fault_count+page_hit+1<<"\nEnter a page number process want to access: ";
        cin>>page_id;
    }

    // Print report
    cout<<"\n--------------------------------------------------------------------------------------------------------------\nEND OF SIMULATION";
    cout<<"\nTotal page access request: "<<page_hit+page_fault_count<<"\nHit count: "<<page_hit<<"\nTotal Page Faults: "<< page_fault_count<<'\n';

    return 0;
}
