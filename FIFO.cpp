#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

const int FRAME_SIZE = 4;
const int VIRTUAL_ADDRESS_SPACE = 32;

struct Frame {
    bool occupied;
    int page_number;
};

struct Process {
    int id;
    int size;
    vector<int> page_numbers;
};

int main() {
    int n, maximum_no_frame;

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
            int page;
            cin >> page;
            temp.page_numbers.push_back(page);
        }

        processes[i] = temp;
    }

    cout << "Enter the maximum number of frames in physical memory:\n";
    cin >> maximum_no_frame;

    vector<Frame> physical_memory(maximum_no_frame, { false, -1 });

    int page_faults = 0;

    for (const auto& process : processes) {
        cout << "\nProcess " << process.id << ":\n";

        for (int page : process.page_numbers) {
            bool page_fault = true;

            for (auto& frame : physical_memory) {
                if (frame.occupied && frame.page_number == page) {
                    page_fault = false;  // Page hit
                    break;
                }
            }

            if (page_fault) {
                ++page_faults;

                // Replace page using FIFO algorithm
                auto oldest_frame = min_element(physical_memory.begin(), physical_memory.end(),
                    [](const Frame& a, const Frame& b) {
                        return a.page_number < b.page_number;
                    });

                oldest_frame->page_number = page;
                oldest_frame->occupied = true;

                cout << "Page " << page << " loaded into physical memory.\n";
            }
        }
    }

    cout << "\nTotal Page Faults: " << page_faults << endl;

    return 0;
}
