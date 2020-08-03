#ifndef NODE_H
#define NODE_H

#include <memory>
#include <iostream>

using namespace std;
using uchar = unsigned char;

class Node {
public:
    using pointer = shared_ptr<Node>;
    
    pointer left {nullptr};
    pointer right {nullptr};
    pointer parent {nullptr};
    
    Node() = default;
    Node(uchar uch, int f) : ch(uch), frequency(f) {};
    Node(const string& n, int f) : name(n), frequency(f) {};
    
    bool operator<(const Node& oth) const{
        return frequency < oth.frequency;
    }
    
    friend ostream& operator<<(ostream& os, const Node& node);
    
    int get_frequency() const{
        return frequency;
    };
    
    int set_frequency(int f) {
        frequency = f;    
    };
    
    string code() const {
        return code_string;  
    };
    
    void code(const string& c) {
        code_string = c;
    };
    
    uchar get_byte() const {
        return ch;
    }
    
    string get_name() const{
        if (ch == 0) {
            return name;
        }
        else {
            if (ch == '\n') {
                return "\\n";
            }
            return string(1, static_cast<char>(ch));
        }
    }
    
private:
    string name {""};
    uchar ch {0};
    int frequency {0};
    string code_string {""};
};

ostream& operator<<(ostream& os, const Node& node){
    return os << "[" << node.get_name() << "] = " << node.frequency;
}

class LowestPriority{
public:
    bool operator()(const Node::pointer& left, const Node::pointer& right) const {
        return left->get_frequency() > right->get_frequency();
    }
};
#endif //NODE_H
