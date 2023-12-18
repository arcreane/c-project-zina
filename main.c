#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.c"


// Define a TreeNode struct
typedef struct TreeNode {
    char letter;         // Letter
    int freq;            // Frequency
    struct TreeNode *left, *right;
} TreeNode;

// Function to create a new TreeNode
TreeNode* newTreeNode(char letter, int freq) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node) {
        node->letter = letter;
        node->freq = freq;
        node->left = node->right = NULL;
    }
    return node;
}

// Function to free the entire Huffman tree
void freeTree(TreeNode* node) {
    if (node) {
        freeTree(node->left);
        freeTree(node->right);
        free(node);
    }
}

// Function to enqueue a TreeNode into the queue
int EnQueueTreeNode(queue *q, TreeNode *node) {
    if (node== NULL) return -1;
    return EnQueue(q, node, node->freq);// return 3 element because check in queue.c the third one is for the priority queue 
    // so in here , the parameter of the priority will be the frequency 
}

// Function to dequeue a TreeNode from the queue
TreeNode* DeQueueTreeNode(queue *q) {
    TreeNode* node;  // Declare a pointer to TreeNode
    if (!isEmptyQueue(*q)) {  // Check if the queue is not empty before attempting to dequeue
        node = (TreeNode*)q->front->data; // Get the data (TreeNode) from the front of the queue
        if (DeQueue(q) != -1) { // Call DeQueue to remove the front element from the queue
            return node; // If DeQueue is successful, return the TreeNode
        }
    }
    return NULL;  // If the queue is empty or DeQueue fails, return NULL
}

// Function to build the Huffman tree
TreeNode* buildHuffmanTree(char data[], int freq[], int size) {
    queue pq = CreateQueue(); // Create a priority queue

    // Loop through each character and its frequency
    for (int i = 0; i < size; ++i) { 

        // Create a new tree node for each character and frequency
        TreeNode* node = newTreeNode(data[i], freq[i]);

        // Check if node creation failed or enqueueing fails
        if (!node || EnQueueTreeNode(&pq, node) == -1) {
        // If there's a failure, free any allocated node and return NULL
            freeTree(node);
            return NULL;
        }
    }

    // Loop until only one element remains in the priority queue
    while (pq.front && pq.front->next) {

        // Dequeue two nodes from the priority queue
        TreeNode* left = DeQueueTreeNode(&pq);
        TreeNode* right = DeQueueTreeNode(&pq);

        // Create a new internal node with the character ('$')
        // and frequency equal to the sum of the two dequeued nodes frequencies
        TreeNode* top = newTreeNode('$', left->freq + right->freq);

        // Assign the dequeued nodes as left and right children of the new node
        top->left = left;
        top->right = right;

        // Enqueue the new node back into the priority queue
        if (EnQueueTreeNode(&pq, top) == -1) {
            // Handle failure: free nodes and return NULL
            freeTree(top);
            return NULL;
        }
    }
    // Dequeue the remaining node from the queue, which is now the root of the Huffman tree
    return DeQueueTreeNode(&pq); 
}

/* Function to print the Huffman codes (for verification , at the beginning we wanted to verify if the conversion of the binary code was correct 
We compared the binary code of the character we wanted to convert*/
void printTree(TreeNode *root, int *code, int depth) {
    if (!root) return;
    if (root->left == NULL && root->right == NULL) {//it means that it's the end element (the character)
        printf("Character: %c, Code: ", root->letter);// Print the character at the leaf
       
         // Print the code by iterating through the code array up to the current depth
        for (int i = 0; i < depth; ++i) { 
            printf("%d", code[i]);
        }
        printf("\n");
    } else {
        if (root->left) {// checks if there is a left child of the current node.
            code[depth] = 0; //adds a 0 to the code array at the current depth
            printTree(root->left, code, depth + 1);//increase the depth by 1 so we are going one level deeper into the tree.
        }
        if (root->right) {// checks if there is a right child of the current node.
            code[depth] = 1;//adds a 1 to the code array at the current depth
            printTree(root->right, code, depth + 1);//increase the depth by 1 so we are going one level deeper into the tree.
        }
    }
}


//This function finds the Huffman code for a specified character in a Huffman tree and appends this code to the encodedcharacter array
void findAndPrintCode(TreeNode *root, char character, int *code, int depth, int *position, char *encodedcharacter) {
    if (!root) return;

    // Check if the current node is a leaf and contains the character we're looking for.
    if (root->left == NULL && root->right == NULL && root->letter == character) {
        //translate the Huffman code, which is stored as a series of binary digits (0s and 1s) in the code array, 
        //into a character string and append it to the encodedcharacter array.
        for (int i = 0; i < depth; ++i) {
            encodedcharacter[*position] =  '0' +code[i];
            (*position)++;
        }
        // Append a space to separate codes of different characters
        encodedcharacter[*position] = ' ';
        (*position)++;
        return;
    }

    // Search left and right subtrees
    // Search left and right subtrees
    if (root->left) {
        code[depth] = 0; // We're going left, so mark this step with a 0
        findAndPrintCode(root->left, character, code, depth + 1, position, encodedcharacter); // Now, go deeper into the left side
    }
    if (root->right) {
        code[depth] = 1; // We're going right, so mark this step with a 1
        findAndPrintCode(root->right, character, code, depth + 1, position, encodedcharacter); // Now, go deeper into the right side
}

}

// Function to encode a sentence using the Huffman tree
void encodeSentence(TreeNode *root, const char *sentence, char *encodedcharacter) {
    int code[100];
    int position = 0; // position in encodedcharacter

    // we call findAndPrintCode to get the Huffman code for that character repeated for every character in the sentence,
    // building a complete encoded string of the entire sentence.
    for (int i = 0; sentence[i] != '\0'; ++i) {
        findAndPrintCode(root, sentence[i], code, 0, &position, encodedcharacter);// instead of char character , we have sentence[i], this is how we build the string
    }
    encodedcharacter[position] = '\0'; // Null-terminate the encoded string
}



// Function to decode an encoded sentence using the Huffman tree
void decodeSentence(TreeNode *root, const char *encodedSentence) {
    TreeNode *current = root; // Start from the root of the Huffman tree
    printf("Decoded Sentence: ");

    // Loop through each character in the encoded Huffman string
    for (int i = 0; encodedSentence[i] != '\0'; ++i) {
        // Decide the path based on the current character in the encoded string
        if (encodedSentence[i] == '0') {
            current = current->left; // Move left in the tree for '0'
        } else if (encodedSentence[i] == '1') {
            current = current->right; // Move right in the tree for '1'
        }

        // Check if the current node is a leaf node (no left or right children)
        if (current->left == NULL && current->right == NULL) {
            printf("%c", current->letter); // Print the character represented by this leaf node
            current = root; // Go back to the root to decode the next series of '0's and '1's
        }
    }
    printf("\n"); // End of the decoded sentence
}



int main() {
     // Define an array of characters for the Huffman tree
    char arr[] = {
    'e', 'a', 'i', 's', 'n', 'r', 't', 'o', 'l', 'u', 'd', 'c', 'm', 'p', 'g', 
    'b', 'v', 'h', 'f', 'q', 'y', 'x', 'j', 'k', 'w', 'z', 'A', 'B', 'C', 'D', 
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 
    'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ', '.', ',', '!', '?', ':', ';', '\'', 
    '\"', '(', ')', '-'
};

// Define frequency array for French
int freqF[] = {
    1210, 711, 659, 651, 639, 607, 592, 502, 496, 449, 367, 318, 262, 249, 123, 
    114, 111, 111, 111, 65, 46, 38, 34, 29, 17, 15, 50, 50, 50, 50, 50, 50, 50, 
    50, 50, 50, 50, 50, 50, 50, 50, 1000, 50, 50, 50, 50, 50, 50, 50, 50, 50, 100, 
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30
};
// Define frequency array for English
int freqE[] = {
    1260, 834, 671, 611, 680, 568, 937, 770, 424, 285, 414, 273, 253, 568, 192, 
    154, 106, 611, 203, 9, 204, 20, 23, 87, 234, 6, 50, 50, 50, 50, 50, 50, 50, 
    50, 50, 50, 50, 50, 50, 50, 50, 1000, 50, 50, 50, 50, 50, 50, 50, 50, 50, 100, 
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30
};


 // Calculate the size of the character array
int size = sizeof(arr) / sizeof(arr[0]);


    // Define a string to hold the language choice
    char language[10];
    printf("Enter the language (English/French): ");
    fgets(language, 10, stdin); // Read user input for language selection


    // Declare a pointer to hold the address of the frequency array
    int *freqArray;
    // Compare the input language with "English\n" for an exact match
    if (strcmp(language, "English\n") == 0) { 
        freqArray = freqE; // If the language is English, point to the English frequency array
    } else if (strcmp(language, "French\n") == 0) {  // Compare the input language with "French\n" for an exact match
        freqArray = freqF; // If the language is French, point to the French frequency array
    } else {

        // Handle cases where the input language does not match expected values
       
        printf("Invalid language. Exiting program.\n"); // Print an error message
        return 1; // exit the program
    }


    // Build the Huffman Tree with the chosen language's frequency array
    TreeNode *root = buildHuffmanTree(arr, freqArray, size);

    // Print Huffman codes for each character
    int code[100];
    printTree(root, code, 0);  // Assuming printTree function exists to display Huffman codes

    // Define a string to hold a sentence read from a file
    char sentence[2048];
    FILE *file = fopen("test.txt", "r"); // Open the file "test.txt" for reading
    if (file == NULL) { // Check for file open error
        perror("Error opening file");
        return 1; // Exit program due to file opening error
    }
    if (fgets(sentence, sizeof(sentence), file) == NULL) { // Read a sentence from the file
        perror("Error reading file");
        fclose(file); // Close the file before exiting
        return 1; // Exit program due to file reading error
    }
    fclose(file); // Close the file after reading
    

    // Loop to remove the newline character from the read sentence
   

    // Encode and Decode the sentence
    char encodedSentence[2048];
    encodeSentence(root, sentence, encodedSentence); // Assuming encodeSentence function exists
    printf("Encoded Sentence: %s\n", encodedSentence); // Print the encoded sentence
    decodeSentence(root, encodedSentence);  // Assuming decodeSentence function exists

    // Free memory used by Huffman tree
    freeTree(root);
    return 0; // End of main function
}


