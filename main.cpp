#include "Node.hpp"

#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;
using queue_t = priority_queue<Node::pointer, vector <Node::pointer>, LowestPriority>;

void read_file(string filename, vector <int> & frequency) {
    ifstream ifs(filename, ifstream::binary);
    if(!ifs) {
        cerr << "Error in [" << __PRETTY_FUNCTION__ << "]: " << strerror(errno);
        return;
    }
    
    while(true) {
        char c;
        ifs.read(&c, 1);
        
        if (ifs.eof()) {
            break;
        }
        
        frequency[static_cast<unsigned char>(c)]++;
    }
}

void make_code(Node::pointer& node, string str, vector<string>& codes){
    if(node->left != nullptr) {
        make_code(node->left, str + "0", codes);
    }
    if(node->right != nullptr) {
        make_code(node->right, str + "1", codes);
    }
    
    if(node->left == nullptr && node->right == nullptr){
        if (node->parent == nullptr){
            str += "0";
        };
        node->code(str);
        codes[node->get_byte()] = str;
        //cout << "leaf: " << *node << ", code: " << node->code() << '\n';
    }
}

void fill_queue(vector<int>& frequency, queue_t& queue) {
    uchar counter = 0;
    for (auto value : frequency) {
        if (value) {
            Node::pointer node = make_shared<Node>(counter, value);
            queue.push(node);
        }
        counter++;
    };   
}

void build_tree(queue_t& queue){
    while (queue.size() > 1) {
        Node::pointer x = queue.top();
        queue.pop();
        Node::pointer y = queue.top();
        queue.pop();
        string name = x->get_name() + y->get_name();
        Node::pointer z = make_shared<Node>(name, x->get_frequency() + y->get_frequency());
        
        z->left = x;
        z->right = y;
        x->parent = z;
        y->parent = z;
        
        queue.push(z);
    }    
}

string message_to_code(const string& filename, const vector<string>& codes){
    string msg {""};
    ifstream ifs(filename, ifstream::binary);
    if(!ifs) {
        cerr << "Error in [" << __PRETTY_FUNCTION__ << "]: " << strerror(errno);
        return msg;
    }
    
    while(true) {
        char c;
        ifs.read(&c, 1);
        
        if (ifs.eof()) {
            break;
        }
        
        msg += codes[static_cast<uchar>(c)];
    }
    
    return msg;
}

void write_file (const string& filename, const vector<int>& frequency, vector<string>& codes, const queue_t& queue, const string& message) {
    ofstream ofs(filename, ofstream::binary);
    if(!ofs) {
        cerr << "Error in [" << __PRETTY_FUNCTION__ << "]: " << strerror(errno);
        return;
    }
    
    uchar count = count_if(frequency.begin(), frequency.end(), [](const int& value) {
                        return (value != 0);
                    });
    ofs << (int)count << "\n";
    for (int i = 0; i < frequency.size(); i++) {
        if (frequency[i])
            ofs << static_cast<char>(i) << ": " << codes[i] << "\n";
    }
    ofs << message << "\n";
}

int main() {
    vector <int> frequency(0x100, 0);
    read_file("text.txt", frequency);
    
    queue_t queue;
    fill_queue(frequency, queue);
    build_tree(queue);
    
    vector<string> codes(0x100, "");
    Node::pointer root = queue.top();
    make_code(root, "", codes);
    
    string message = message_to_code("text.txt", codes);
    
    write_file("code.txt", frequency, codes, queue, message);
    
    return 0;
}
