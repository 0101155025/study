#include <iostream>
#define INT 1

using namespace std;

#define MAXSIZE 100
#define MAX 0x7fffffff

typedef char VertexType;
typedef int EdgeType;
typedef struct
{
    VertexType vertex[MAXSIZE];
    EdgeType arc[MAXSIZE][MAXSIZE];
    int vertex_num;
    int edge_num;
}Mat_Graph;

int visited[MAXSIZE];
int front = 0;
int rear = 0;
int queue[MAXSIZE];

/**
 * @brief 创建邻接矩阵表示的无向图
 * @param G 指向Mat_Graph结构体的指针，用于存储图的信息
 *
 * 该函数负责初始化并构建一个包含9个顶点、15条边的无向图，
 * 使用邻接矩阵表示法，其中1表示顶点间有边相连，0表示无边相连。
 */
void create_graph(Mat_Graph* G);

/**
 * @brief 创建邻接矩阵表示的无向图的最小生成树
 * @param G 指向Mat_Graph结构体的指针，用于存储图的信息
 *
 * 该函数负责初始化并构建一个包含9个顶点、15条边的无向图的最小生成树，
 * 使用邻接矩阵表示法，其中1表示顶点间有边相连，0表示无边相连。
 */
void create_graph_prime(Mat_Graph* G);

/**
 * @brief 深度优先搜索遍历图
 * @param G Mat_Graph结构体，代表要遍历的图
 * @param i 当前访问的顶点索引
 *
 * 该函数实现了深度优先搜索遍历图的过程，从指定的顶点开始，
 * 递归地访问所有未被访问过的邻接顶点，直到所有顶点都被访问过。
 */
void dfs(Mat_Graph G, int i);

/**
 * @brief 广度优先搜索遍历图
 * @param G Mat_Graph结构体，代表要遍历的图
 *
 * 该函数实现了广度优先搜索遍历图的过程，从指定的顶点开始，
 * 迭代地访问所有未被访问过的邻接顶点，直到所有顶点都被访问过。
 */
void bfs(Mat_Graph G);

/**
 * @brief 最小生成树 Prim 算法
 * @param G 指向Mat_Graph结构体的指针，用于存储图的信息
 *
 * 该函数负责初始化并构建一个包含9个顶点、15条边的无向图的最小生成树，
 * 使用邻接矩阵表示法，其中1表示顶点间有边相连，0表示无边相连。
 */
void prime(Mat_Graph* G);

int main()
{
    Mat_Graph G;
    create_graph(&G);

    // 深度优先搜索遍历图
    for (int i = 0;i < G.vertex_num;i++)
        visited[i] = 0;
    dfs(G,0);
    cout << endl;

    // 广度优先搜索遍历图
    for (int i = 0;i < G.vertex_num;i++)
        visited[i] = 0;
    bfs(G);
    cout << endl;

    // 最小生成树 Prim 算法
    create_graph_prime(&G);
    prime(&G);

    return 0;
}

void create_graph(Mat_Graph* G)
{
    G->vertex_num = 9;
    G->edge_num = 15;
    G->vertex[0] = 'A';
    G->vertex[1] = 'B';
    G->vertex[2] = 'C';
    G->vertex[3] = 'D';
    G->vertex[4] = 'E';
    G->vertex[5] = 'F';
    G->vertex[6] = 'G';
    G->vertex[7] = 'H';
    G->vertex[8] = 'I';

    for (int i = 0;i < G->vertex_num;i++)
        for(int j = 0;j < G->vertex_num;j++)
            G->arc[i][j] = 0;

    //A-B A-F
    G->arc[0][1] = 1;
    G->arc[0][5] = 1;

    //B-C B-G B-I
    G->arc[1][2] = 1;
    G->arc[1][6] = 1;
    G->arc[1][8] = 1;

    //C-D C-I
    G->arc[2][3] = 1;
    G->arc[2][8] = 1;

    //D-E D-G D-H D-I
    G->arc[3][4] = 1;
    G->arc[3][6] = 1;
    G->arc[3][7] = 1;
    G->arc[3][8] = 1;

    //E-F E-H
    G->arc[4][5] = 1;
    G->arc[4][7] = 1;

    //F-G
    G->arc[5][6] = 1;

    //G-H
    G->arc[6][7] = 1;

    for (int i = 0;i < G->vertex_num;i++)
        for (int j = 0;j < G->vertex_num;j++)
            G->arc[j][i] = G->arc[i][j];
}

void create_graph_prime(Mat_Graph* G)
{
    G->vertex_num = 9;
    G->edge_num = 15;
    G->vertex[0] = 'A';
    G->vertex[1] = 'B';
    G->vertex[2] = 'C';
    G->vertex[3] = 'D';
    G->vertex[4] = 'E';
    G->vertex[5] = 'F';
    G->vertex[6] = 'G';
    G->vertex[7] = 'H';
    G->vertex[8] = 'I';

    for (int i = 0;i < G->vertex_num;i++)
        for(int j = 0;j < G->vertex_num;j++)
            if (i == j) G->arc[i][j] = 0;
            else G->arc[i][j] = MAX;

    //A-B A-F
    G->arc[0][1] = 10;
    G->arc[0][5] = 11;

    //B-C B-G B-I
    G->arc[1][2] = 18;
    G->arc[1][6] = 16;
    G->arc[1][8] = 12;

    //C-D C-I
    G->arc[2][3] = 22;
    G->arc[2][8] = 8;

    //D-E D-G D-H D-I
    G->arc[3][4] = 20;
    G->arc[3][6] = 24;
    G->arc[3][7] = 16;
    G->arc[3][8] = 21;

    //E-F E-H
    G->arc[4][5] = 26;
    G->arc[4][7] = 7;

    //F-G
    G->arc[5][6] = 17;

    //G-H
    G->arc[6][7] = 19;

    for (int i = 0;i < G->vertex_num;i++)
        for (int j = 0;j < G->vertex_num;j++)
            G->arc[j][i] = G->arc[i][j];
}

void dfs(Mat_Graph G, int i)
{
    visited[i] = 1;
    cout << G.vertex[i] << ' ';
    for (int j = 0;j < G.vertex_num;j++)
        if (G.arc[i][j] == 1 && visited[j] == 0)
            dfs(G,j);
}

void bfs(Mat_Graph G)
{
    int i = 0;
    visited[i] = 1;
    cout << G.vertex[i] << ' ';
    queue[rear++] = i;
    while (front != rear)
    {
        i = queue[front++];
        for (int j = 0;j < G.vertex_num;j++)
            if (G.arc[i][j] == 1 && visited[j] == 0)
            {
                visited[j] = 1;
                cout << G.vertex[j] << ' ';
                queue[rear++] = j;
            }
    }
}

void prime(Mat_Graph* G)
{
    int i,j,temp;
    int min;
    int weight[MAXSIZE]; // 候选边
    int vex_index[MAXSIZE]; // 值表示出发点,下标对应到达点

    // 先从第一个顶点开始
    weight[0] = 0; // vex_index某点与下标对应点的权重
    vex_index[0] = 0;
    for (i = 1;i < G->vertex_num;i++)
    {
        weight[i] = G->arc[0][i];
        vex_index[i] = 0;
    }

    for (i = 0;i < G->vertex_num;i++)
    {
        min = MAX;
        j = 0;
        temp = 0;
        while (j < G->vertex_num)
        {
            if (weight[j] != 0 && weight[j] < min)
            {
                min = weight[j];
                temp = j;
            }
            j++;
        }
        cout << G->vertex[vex_index[temp]] << ' ' << G->vertex[temp] << '\n';
        weight[temp] = 0;

        for (j = 0;j < G->vertex_num;j++)
            if (weight[j] != 0 && G->arc[temp][j] < weight[j])
            {
                weight[j] = G->arc[temp][j];
                vex_index[j] = temp;
            }
    }
}

//
// Created by Fly on 2025/11/20.
//