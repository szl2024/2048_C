#include "game.h"

static int *map;
static size_t size;
static size_t len;

static int score = 0;

char topStr[512] = {0};
char midStr[512] = {0};
char bottomStr[512] = {0};

/**
 * 初始化字符串数组，用于绘制图形边框。
 * 该函数不接受参数，也不返回任何值。
 * 全局变量size决定了边框的长度。
 */
static void initStr()
{
    // 初始化顶部边框字符串
    strcpy(topStr, "┏━━━━━");
    for (size_t i = 0; i < size - 1; i++)
    {
        strcat(topStr, "┳━━━━━");
    }
    strcat(topStr, "┓");

    // 初始化中间边框字符串
    strcpy(midStr, "┣━━━━━");
    for (size_t i = 0; i < size - 1; i++)
    {
        strcat(midStr, "╋━━━━━");
    }
    strcat(midStr, "┫");

    // 初始化底部边框字符串
    strcpy(bottomStr, "┗━━━━━");
    for (size_t i = 0; i < size - 1; i++)
    {
        strcat(bottomStr, "┻━━━━━");
    }
    strcat(bottomStr, "┛");
}

/**
 * 初始化游戏界面和基本设置
 * @param m_size 游戏地图的大小（边长）
 */
void init(size_t m_size) 
{
    // 使用当前时间做种子初始化随机数生成器
    srand((unsigned)time(NULL));
    // 清除屏幕
    system("clear");
    // 设置终端为无缓冲、无回显模式
    system("stty -icanon -echo");
    // 隐藏光标
    printf("\033[?25l");
    // 保存当前光标位置
    printf("\033[s");

    // 设置地图大小并计算总格数
    size = m_size;
    len = size * size;
    // 为地图申请内存
    map = (int *)calloc(sizeof(int), len);
    // 初始化游戏字符串（未给出的函数）
    initStr();

    // 插入一个游戏元素（示例功能，具体实现未给出）
    insert_one();
    // 展示游戏地图（未给出的函数）
    show();
}

/**
 * @brief 显示游戏界面
 * 
 * 该函数负责输出游戏的界面，包括游戏标题、数字方块、分隔线以及当前分数。
 * 利用控制字符调整光标位置、清除屏幕部分区域，并以规定的格式显示游戏内容。
 * 
 * @param 无
 * @return 无
 */
void show() 
{
    // 恢复光标到初始位置，清除当前行剩余内容，保存当前光标位置
    printf("\033[u"); 
    printf("\033[K"); 
    printf("\033[s"); 
    
    // 显示游戏标题
    printf("\n\t\x1b[1;33;49m2048小游戏\x1b[0m\n");
    printf("%s\n", topStr); // 打印顶部分隔线
    
    // 遍历地图数组，以规定的格式显示每个方块的数字，或空方块
    size_t i, j;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            size_t num = (size_t) * (map + i * size + j); // 获取当前方块的数字
            if (num > 0)
                printf("┃%4zu ", num); // 如果方块有数字，格式化后显示
            else
                printf("┃     "); // 如果方块为空，显示空白占位
        }
        printf("┃\n"); // 打印每行结束的分隔线
        
        if (i == size - 1)
            printf("%s\n", bottomStr); // 如果是最后一行，打印底部分隔线
        else
            printf("%s\n", midStr); // 否则，打印中间分隔线
    }
    
    // 显示当前分数
    printf("\n分数: \x1b[1;33;49m%d\x1b[0m\n", score);
}

int insert_one() // 插入一个新的点
{
    // 1.获取为0的点的个数;
    size_t count = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (map[i] == 0)
            ++count;
    }
    if (count == 0)
        return 1;

    // 2.随机数n
    size_t num = rand() % count;

    // 3.找到第n个数,设置为数字2
    count = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (map[i] == 0)
            if (count++ == num)
                map[i] = 2;
    }
    return 0;
}

void move_group(int *arr[]) // 移动一组
{
    int cur = 0;
    int *pHead = arr[cur];
    for (size_t i = 1; i < size; i++)
    {
        int *pNum = arr[i];
        if (*pNum == 0)
            continue;
        if (*pHead == 0)
        {
            *pHead = *pNum;
            *pNum = 0;
            continue;
        }

        if (*pHead == *pNum)
        {
            *pHead = *pHead + *pNum;
            score += *pHead;
            *pNum = 0;
            pHead = arr[++cur];
        }
        else
        {
            pHead = arr[++cur];
            if (pHead != pNum)
            {
                *pHead = *pNum;
                *pNum = 0;
            }
        }
    }
}

void play() // 游戏操作
{
    while (1)
    {
        int ch = getchar();
        switch (ch)
        {
        case 65: // 向上移动
        case 119:
            for (int i = 0; i < size; i++)
            {
                int *pArr[size];
                int index = 0;
                for (int j = 0; j < size; j++)
                {
                    pArr[index++] = (int *)map + j * size + i;
                }
                move_group(pArr);
            }
            break;
        case 97: // 向左移动
        case 68:
            for (int i = 0; i < size; i++)
            {
                int *pArr[size];
                int index = 0;
                for (int j = 0; j < size; j++)
                    pArr[index++] = (int *)map + i * size + j;
                move_group(pArr);
            }
            break;
        case 100: // 向右移动
        case 67:
            for (int i = size - 1; i >= 0; i--)
            {
                int *pArr[size];
                int index = 0;
                for (int j = size - 1; j >= 0; j--)
                    pArr[index++] = (int *)map + i * size + j;
                move_group(pArr);
            }
            break;
        case 115:
        case 66: // 向下移动
            for (int i = 0; i < size; i++)
            {
                int *pArr[size];
                int index = 0;
                for (int j = size - 1; j >= 0; j--)
                    pArr[index++] = (int *)map + j * size + i;
                move_group(pArr);
            }
            break;
        case 113:
            game_over();
            break;
        default:
            continue;
        }
        int ret = insert_one();
        if (ret == 1)
        {
            printf("\n 游戏结束!\n");
            game_over();
            return;
        }
        show();
    }
}

void game_over() // 游戏结束
{
    if (map != NULL)
    {
        free(map);
        map = NULL;
    }
    printf("\033[?25h");
    system("stty icanon echo");
    exit(0);
}
