#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class node {
    public:
        int id;
        int data;
        node() {
            data = NULL;
        }
};

int main() {

    int dimension;
    cout << "Enter the power of two (used to determine nodes): ";
    cin >> dimension;

    int source_node;
    cout << "Enter the source node: ";
    cin >> source_node;

    vector<node> nodes;
    for (int i = 0; i < pow(2, dimension); i++) {
        node temp;
        temp.id = i;
        temp.data = NULL;
        nodes.push_back(temp);
    }

    nodes[source_node].data = 25;


    int global_data = nodes[source_node].data;
    for (int i = dimension - 1; i >= 0; i--) {
        int mask = (1 << i);
        for (int j = 0; j < nodes.size(); j++) {
            int current_node = nodes[j].id;
            int neighbour_node = current_node ^ mask;

            int current_virtual_id = current_node ^ source_node;
            int neighbour_virtual_id = neighbour_node ^ source_node;

            if (nodes[current_virtual_id].data == NULL && nodes[neighbour_virtual_id].data == NULL) {
                continue;
            }

            bool is_sender = (current_virtual_id & mask);
            if (is_sender) {
                cout << "Node: [" << current_virtual_id << "] is receiving data from [" << neighbour_virtual_id << "]" << endl;
            }
            else {
                cout << "Node: [" << current_virtual_id << "] is sending data to [" << neighbour_virtual_id << "]" << endl;
            }

            nodes[neighbour_virtual_id].data = nodes[current_virtual_id].data;
        }
    }

    // print final data
    cout << "Final Data: " << endl;
    for (int i = 0; i < nodes.size(); i++) {
        cout << "Node: [" << i << "] has data: " << nodes[i].data << endl;
    }
    cout << "\n";


    return 0;
}