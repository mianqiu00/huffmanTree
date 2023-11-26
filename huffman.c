#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node
{
    char data;
    double weight;
    char code[15];
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

typedef struct
{
    char character;
    char code[15];
} HuffmanEntry;

Node* create_tree(char* input_file);
Node* merge(Node* left, Node* right);
void read_weight(char* input_file, Node forest[100], int* n);
void nodecopy(Node* dest, Node* source);
void tree2file(Node* tree, char* filename);
void traverseTreeToFile(char* filename, Node* tree, char* currentCode, int depth, int* maxLength, double* wplSum);
HuffmanEntry* loadHuffmanTable(const char* filename, int* tableSize);
void encodeToFile(const char* source, HuffmanEntry* huffmanTable, int tableSize, const char* outputFilename);
char* readFirstLine(char* filename);
char* decode(char* code, HuffmanEntry* huffmanTable);
void decode2file(char* decode_txt, char* output_file);

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        printf("The executable file was generated successfully\n");
        return 0;
    }
    if (argc < 7)
    {
        printf("please input enough arguments\n");
        return 0;
    }

    if (strcmp(argv[1], "-m") != 0)
    {
        printf("please input correct order\n");
        return 0;
    }
    if (strcmp(argv[2], "c") == 0)
    {
        // 建树模式
        if (argc != 7)
        {
            printf("please input correct order\n");
            return 0;
        }
        if (strcmp(argv[3], "-i") != 0)
        {
            printf("please input correct order\n");
            return 0;
        }
        if (strcmp(argv[5], "-o") != 0)
        {
            printf("please input correct order\n");
            return 0;
        }
        char* input_file = strdup(argv[4]);
        char* output_file = strdup(argv[6]);
        Node* tree = create_tree(input_file);
        tree2file(tree, output_file);
    }
    else if (strcmp(argv[2], "e") == 0)
    {
        // 编码模式
        if (argc != 9)
        {
            printf("please input correct order\n");
            return 0;
        }
        if (strcmp(argv[3], "-i") != 0)
        {
            printf("please input correct order\n");
            return 0;
        }
        if (strcmp(argv[5], "-h") != 0)
        {
            printf("please input correct order\n");
            return 0;
        }
        if (strcmp(argv[7], "-o") != 0)
        {
            printf("please input correct order\n");
            return 0;
        }
        char* input_file = strdup(argv[4]);
        char* model_file = strdup(argv[6]);
        char* output_file = strdup(argv[8]);
        // 待编码的文本
        char* sourceText = readFirstLine(input_file);

        // 加载哈夫曼编码表
        int tableSize;
        HuffmanEntry* huffmanTable = loadHuffmanTable(model_file, &tableSize);

        // 编码文本
        encodeToFile(sourceText, huffmanTable, tableSize, output_file);
        free(sourceText);
        free(huffmanTable);
    }
    else if (strcmp(argv[2], "d") == 0)
    {
        // 解码模式
        if (argc != 9)
        {
            printf("please input correct order\n");
            return 0;
        }
        if (strcmp(argv[3], "-i") != 0)
        {
            printf("please input correct order\n");
            return 0;
        }
        if (strcmp(argv[5], "-h") != 0)
        {
            printf("please input correct order\n");
            return 0;
        }
        if (strcmp(argv[7], "-o") != 0)
        {
            printf("please input correct order\n");
            return 0;
        }
        char* input_file = strdup(argv[4]);
        char* code_file = strdup(argv[6]);
        char* output_file = strdup(argv[8]);

        char* code = readFirstLine(input_file);
        int tableSize;
        HuffmanEntry* huffmamTable = loadHuffmanTable(code_file, &tableSize);
        char* decode_txt = decode(code, huffmamTable);
        decode2file(decode_txt, output_file);
    }
    else
    {
        printf("please input correct order\n");
    }

    return 0;
}

// 连接两个节点
Node* merge(Node* left, Node* right)
{
    Node* root = (Node*)malloc(sizeof(Node));
    left->parent = root;
    right->parent = root;
    root->left = left;
    root->right = right;
    root->weight = left->weight + right->weight;
    root->parent = 0;
    root->data = '\0';
    return root;
}

// 复制节点信息
void nodecopy(Node* dest, Node* source)
{
    dest->data = source->data;
    dest->left = source->left;
    dest->parent = source->parent;
    dest->right = source->right;
    dest->weight = source->weight;
}

// 连接权重最小两个节点，删除两个节点，将根节点存入原数组
void merge2least(Node forest[100], int* n)
{
    Node empty = {0};
    empty.data = '\0';
    Node* left = (Node*)malloc(sizeof(Node));
    Node* right = (Node*)malloc(sizeof(Node));
    nodecopy(left, &forest[*n-2]);
    nodecopy(right, &forest[*n-1]);
    Node* newNode = merge(left, right);
    forest[(*n)-1] = empty;
    forest[(*n)-2] = *newNode;
    for (int i = *n - 2; i > 0; i--)
    {
        if (forest[i].weight > forest[i-1].weight)
        {
            empty = forest[i];
            forest[i] = forest[i-1];
            forest[i-1] = empty;
        }
        else
        {
            break;
        }
    }
    *n = *n - 1;
}

// 从txt到huffman树
Node* create_tree(char* input_file)
{   
    Node forest[100] = {0};
    int n;
    read_weight(input_file, forest, &n);
    while(n > 1)
    {
        merge2least(forest, &n);
    }
    Node* tree = &forest[0];
    return tree;
}

// 读取权重表
void read_weight(char* input_file, Node forest[100], int* n)
{
    // 打开文件
    FILE *file = fopen(input_file, "r");

    // 检查文件是否成功打开
    if (file == NULL)
    {
        fprintf(stderr, "cannot open the file: %s\n", input_file);
        return;
    }

    // 逐行读取文件
    int i = 0;
    char line[10];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // 使用sscanf解析每行的数据
        char c;
        double w;
        if (sscanf(line, "%c %lf", &c, &w) == 2)
        {
            // 如果成功解析了字符和权重，则存储到结构体数组中
            forest[i].data = c;
            forest[i].weight = w;
        }
        else
        {
            // 字符是空格
            forest[i].data = ' ';
            forest[i].weight = atoi(line);
        }
        forest[i].left = 0;
        forest[i].right = 0;
        forest[i].parent = 0;
        i++;
    }

    // 关闭文件
    fclose(file);

    // 将权重从大到小排序
    int j, k;
    Node temp;
    for (j = 0; j < i; j++)
    {
        for (k = j + 1; k < i; k++)
        {
            if (forest[j].weight < forest[k].weight)
            {
                temp = forest[j];
                forest[j] = forest[k];
                forest[k] = temp;
            }
        }
    }
    for (j = i; j < 100; j++)
    {
        forest[j].data = '\0';
    }
    *n = i;
}

// 利用huffman树生成编码，并打印和整理文件
void tree2file(Node* tree, char* filename)
{
    remove(filename);
    char currentCode[20];  // 用于存储当前节点的编码
    int maxLength = 0;     // 记录最长编码长度
    double wplSum = 0.0;   // 记录带权路径长度总和

    traverseTreeToFile(filename, tree, currentCode, 0, &maxLength, &wplSum);

    printf("Max_length: %d\n", maxLength);
    printf("WPL: %lf\n", wplSum);

    return;
}

// 先根遍历树
void traverseTreeToFile(char* filename, Node* tree, char* currentCode, int depth, int* maxLength, double* wplSum)
{
    // 判断是否为叶子节点
    if (tree->left == NULL && tree->right == NULL)
    {
        currentCode[depth] = '\0';  // 添加字符串结束符
        FILE* file = fopen(filename, "a");
        fprintf(file, "%c %s\n", tree->data, currentCode);
        fclose(file);

        // 计算并更新最长长度
        int currentLength = strlen(currentCode);
        if (currentLength > *maxLength) {
            *maxLength = currentLength;
        }

        // 计算带权路径长度并更新总和
        *wplSum += tree->weight * currentLength;

        return;
    }

    // 向左遍历，表示添加'0'
    if (tree->left != NULL)
    {
        currentCode[depth] = '0';
        traverseTreeToFile(filename, tree->left, currentCode, depth + 1, maxLength, wplSum);
    }

    // 向右遍历，表示添加'1'
    if (tree->right != NULL)
    {
        currentCode[depth] = '1';
        traverseTreeToFile(filename, tree->right, currentCode, depth + 1, maxLength, wplSum);
    }
}

// 从文件中加载哈夫曼编码表
HuffmanEntry* loadHuffmanTable(const char* filename, int* tableSize)
{
    FILE* file = fopen(filename, "r");

    // 假设文件中最多有100个哈夫曼编码表项
    HuffmanEntry* table = (HuffmanEntry*)malloc(100 * sizeof(HuffmanEntry));
    *tableSize = 0;

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (line[0] == '\n')
        {
            continue;  // 忽略空行
        }

        if (line[0] == ' ' && line[1] == ' ')
        {
            sscanf(line, "  %s", table[*tableSize].code);
            table[*tableSize].character = ' ';  // 空格的字符为 ' '
        }
        else
        {
            sscanf(line, "%c %s", &(table[*tableSize].character), table[*tableSize].code);
        }
        (*tableSize)++;

        if (*tableSize == 100)
        {
            printf("too many characters\n");
            break;
        }
    }

    fclose(file);
    return table;
}

// 根据哈夫曼编码表编码输入文本，并写入文件
void encodeToFile(const char* source, HuffmanEntry* huffmanTable, int tableSize, const char* outputFilename)
{
    int sourceLength = strlen(source);
    FILE* outputFile = fopen(outputFilename, "w");

    for (int i = 0; i < sourceLength; i++)
    {
        char currentChar = source[i];
        int found = 0;

        for (int j = 0; j < tableSize; j++)
        {
            if (huffmanTable[j].character == currentChar)
            {
                fprintf(outputFile, "%s", huffmanTable[j].code);
                found = 1;
                break;
            }
        }

        if (!found)
        {
            printf("未找到字符 %c 的哈夫曼编码。\n", currentChar);
            fclose(outputFile);
            return;
        }
    }

    fclose(outputFile);
}

// 读取txt文件第一行
char* readFirstLine(char* filename)
{
    FILE* file = fopen(filename, "r");

    // 读取一行
    char buffer[16384];
    if (fgets(buffer, sizeof(buffer), file) == NULL)
    {
        fprintf(stderr, "Error reading file: %s\n", filename);
        fclose(file);
        return NULL;
    }

    // 去除换行符
    size_t length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n')
    {
        buffer[length - 1] = '\0';
    }

    // 关闭文件
    fclose(file);

    // 分配新的内存来存储结果并返回
    char* result = (char*)malloc(strlen(buffer) + 1);
    strcpy(result, buffer);

    return result;
}

// 解码
char* decode(char* code, HuffmanEntry* huffmanTable)
{
    char* decode_txt;
    int i = 0;  // huffmanTable行指针
    int j = 0;  // huffmanTable列指针
    int m = 0;  // code指针
    int n = 0;  // decode指针
    while (code[m] != '\0')
    {
        // printf("%c %c\n", huffmanTable[i].code[j], code[m]);
        while(huffmanTable[i].code[j] != code[m])
        {
            i++;
        }
        if (huffmanTable[i].code[j+1] == '\0')
        {
            decode_txt[n++] = huffmanTable[i].character;
            i = 0;
            j = 0;
            m++;
            continue;
        }
        j++;
        m++;
    }
    decode_txt[n] = '\0';
    return decode_txt;
}

// 将解码写入文件
void decode2file(char* decode_txt, char* output_file)
{
    remove(output_file);
    FILE* file = fopen(output_file, "w");
    fprintf(file, "%s\n", decode_txt);
    fclose(file);
}
