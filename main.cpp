#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>

// Define the node structure for the Huffman Tree
struct Node
{
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparison function for the priority queue
struct compare
{
    bool operator()(Node *l, Node *r)
    {
        return l->freq > r->freq; // Min-heap based on frequency
    }
};

// Class for Huffman Coding
class HuffmanCoder
{
    std::unordered_map<char, std::string> huffmanCode;
    std::unordered_map<char, int> freqMap;
    Node *root;

    // Function to build the frequency map
    void buildFrequencyMap(const std::string &str)
    {
        for (char ch : str)
        {
            freqMap[ch]++;
        }
    }

    // Recursive function to build the Huffman Tree
    void buildHuffmanTree()
    {
        // Create a priority queue to store live nodes of the Huffman Tree
        std::priority_queue<Node *, std::vector<Node *>, compare> pq;

        // Create a leaf node for each character and add it to the priority queue
        for (auto pair : freqMap)
        {
            pq.push(new Node(pair.first, pair.second));
        }

        // Iterate until we have only one node in the priority queue
        while (pq.size() != 1)
        {
            // Remove the two nodes of the highest priority (lowest frequency) from the queue
            Node *left = pq.top();
            pq.pop();
            Node *right = pq.top();
            pq.pop();

            // Create a new internal node with a frequency equal to the sum of the two nodes' frequencies
            int sum = left->freq + right->freq;
            Node *newNode = new Node('\0', sum); // '\0' is used as an internal node
            newNode->left = left;
            newNode->right = right;
            pq.push(newNode);
        }

        // The remaining node is the root node and the tree is complete
        root = pq.top();
    }

    // Recursive function to generate Huffman codes from the tree
    void generateHuffmanCodes(Node *node, const std::string &str)
    {
        if (!node)
            return;

        // If the node is a leaf node, store the code
        if (!node->left && !node->right)
        {
            huffmanCode[node->ch] = str;
        }

        generateHuffmanCodes(node->left, str + "0");
        generateHuffmanCodes(node->right, str + "1");
    }

    // Helper function to delete the tree (to avoid memory leak)
    void deleteTree(Node *node)
    {
        if (node)
        {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    HuffmanCoder(const std::string &str)
    {
        buildFrequencyMap(str);
        buildHuffmanTree();
        generateHuffmanCodes(root, "");
    }

    ~HuffmanCoder()
    {
        deleteTree(root);
    }

    // Function to encode the input string
    std::string encode(const std::string &str)
    {
        std::string encodedStr = "";
        for (char ch : str)
        {
            encodedStr += huffmanCode[ch];
        }
        return encodedStr;
    }

    // Function to decode the encoded string
    std::string decode(const std::string &encodedStr)
    {
        std::string decodedStr = "";
        Node *curr = root;
        for (char bit : encodedStr)
        {
            if (bit == '0')
            {
                curr = curr->left;
            }
            else
            {
                curr = curr->right;
            }

            // If we reach a leaf node, append the character and start over from the root
            if (!curr->left && !curr->right)
            {
                decodedStr += curr->ch;
                curr = root;
            }
        }
        return decodedStr;
    }
};

// Main function for testing HuffmanCoder in C++
int main()
{
    std::string str = "abbccda";

    HuffmanCoder hf(str);
    std::string cs = hf.encode(str);
    std::cout << "Encoded: " << cs << std::endl;

    std::string dc = hf.decode(cs);
    std::cout << "Decoded: " << dc << std::endl;

    return 0;
}