#include <iostream>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

class node {
    public:
        int id;
        vector<int> data;
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
    nodes[0].data.push_back(25);
    for (int i = dimension - 1; i >= 0; i--) {
        int mask = (1 << i);

        cout << "============================## STEP ##============================" << endl;
        for (int j = 0; j < nodes.size(); j++) {

            int current_node = nodes[j].id;
            int neighbour_node = current_node ^ mask;

            if (nodes[current_node].data.empty() && nodes[neighbour_node].data.empty()) {
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

            // print the data being transferred
            cout << "\n\t\tData: ";
            for (int k = 0; k < nodes[current_node].data.size(); k++) {
                cout << nodes[current_node].data[k] << " ";
            }
            cout << "\n\n";

            // merge data from both nodes
            vector <int> merged_data = nodes[current_node].data;
            merged_data.insert(merged_data.end(), nodes[neighbour_node].data.begin(), nodes[neighbour_node].data.end());

            nodes[current_node].data = merged_data;
            nodes[neighbour_node].data = merged_data;
        }
        cout << "============================## STEP ##============================\n\n\n";

    }



    cout << "Final state of the network: " << endl;
    // print all nodes with their data
    for (int i = 0; i < nodes.size(); i++) {
        cout << "Node: " << nodes[i].id << " has data: [ ";
        for (int j = 0; j < nodes[i].data.size(); j++) {
            cout << nodes[i].data[j] << " ";
        }
        cout << " ]\n";
    }


    return 0;
}