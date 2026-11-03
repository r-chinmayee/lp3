#include <bits/stdc++.h>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node *l, *r;
};

struct Cmp {
    bool operator()(Node* a, Node* b) { return a->freq > b->freq; }
};

void build(Node* n, string code, map<char, string>& codes) {
    if (!n) return;
    if (!n->l && !n->r) codes[n->ch] = code.empty() ? "0" : code;
    build(n->l, code + "0", codes);
    build(n->r, code + "1", codes);
}

int main() {
    string s;
    cout << "Enter string: ";
    getline(cin, s);
    map<char, int> freq;
    for (char c : s) freq[c]++;

    cout << "\nFrequencies:\n";
    for (auto& [c, f] : freq) cout << "'" << c << "' : " << f << "\n";

    priority_queue<Node*, vector<Node*>, Cmp> pq;
    for (auto& [c, f] : freq) 
        pq.push(new Node{c, f, nullptr, nullptr});
    while (pq.size() > 1) {
        Node *a = pq.top(); pq.pop();
        Node *b = pq.top(); pq.pop();
        pq.push(new Node{'\0', a->freq + b->freq, a, b});
    }

    map<char, string> codes;
    build(pq.top(), "", codes);
    cout << "\nHuffman codes:\n";
    for (auto& [c, code] : codes) cout << "'" << c << "' : " << code << "\n";
}
