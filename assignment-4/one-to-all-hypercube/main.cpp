#include <iostream>
#include <vector>
#include <string>
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

    // taking dimension of the hypecube
    int dimension;
    cout << "Enter number of dimensions: ";
    cin >> dimension;


    // generating nodes
    vector <node> nodes;
    for (int i = 0; i < pow(2, dimension); i++) {
        node temp;
        temp.id = i;
        nodes.push_back(temp);
    }

    // putting data at 0th node
    nodes[0].data = 25;
    for (int i = dimension - 1; i >= 0; i--) {
        int mask = (1 << i);

        cout << "============================## STEP ##============================" << endl;
        for (int j = 0; j < nodes.size(); j++) {

            int current_node = nodes[j].id;
            int neighbour_node = current_node ^ mask;

            if (nodes[current_node].data == NULL && nodes[neighbour_node].data == NULL) {
                continue;
            }

            // print a message indicating that the data is being transferred
            bool is_sender = (current_node & mask);
            if (is_sender) {
                cout << "Node: [" << current_node << "] is receiving data from [" << neighbour_node << "]" << endl;
            }
            else {
                cout << "Node: [" << current_node << "] is sending data to [" << neighbour_node << "]" << endl;
            }
            
            nodes[neighbour_node].data = nodes[current_node].data;

        }
        cout << "============================## STEP ##============================\n\n\n";

    }



    cout << "Final state of the network: " << endl;
    // print all nodes with their data
    for (int i = 0; i < nodes.size(); i++) {
        cout << "Node: " << nodes[i].id << " has data: [ " << nodes[i].data << " ]" << endl;
    }


    return 0;
}