#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

const int FRAME_SIZE = 4;

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
    queue<int> evict;
};

Proc swap_frame(Proc proc, int page_id_new, int page_id_old)
{
    Proc process = proc;
    process.page_table[page_id_old].occupied = false;
    process.page_table[page_id_new].occupied = true;
    process.page_table[page_id_new].frame_number = process.page_table[page_id_old].frame_number;
    return process;
}

void draw_frames(vector<int> physical_memory) {
    // Initialize the table

    int n = physical_memory.size();

    cout << "Frame no: ";
    for (int i = 0; i < n; ++i) {
        cout << "  " << setw(2) <<  i << "   ";
    }

    cout << "\n         +";

    for (int i = 0; i < n; ++i) {
        cout << "------+";
    }
    cout << "\n         |";
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

    cout << "Enter the length of reference string:\n";
    cin >> n;

    for (int i = 0; i < n; ++i)
    {

        // inputing processes request for a page.
        int page_id;
        cin >> page_id;
        if(page_id > process.page_table.size()) {
            cout <<  "Invalid reference.\n";
            --i;                                                   // get the reference again.
            draw_frames(physical_memory);
            continue;
        }
        // check if page is present inside of memory
        if (process.page_table[page_id].occupied)
        {
            ++page_hit;
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

                int evicted_id = process.evict.front();
                int current_frame = process.page_table[evicted_id].frame_number;
                
                process = swap_frame(process, page_id, evicted_id); // U[date page table
                
                physical_memory[current_frame] = page_id;

                process.evict.pop();
                process.evict.push(page_id);

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
                        process.evict.push(page_id);
                        --free_frame;
                        break;
                    }
                }               
                cout << "Page fault, allocated page to frame: " << process.page_table[page_id].frame_number<<'\n';
                draw_frames(physical_memory);
            }
        }
    }

    cout << "\nTotal Page Faults: " << page_fault_count << endl;

    return 0;
}
