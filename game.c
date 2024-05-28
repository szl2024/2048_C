#include "game.h"

static int *map;
static size_t size;
static size_t len;

static int score = 0;

char topStr[512] = {0};
char midStr[512] = {0};
char bottomStr[512] = {0};


/**
 * Initialize border strings
 *
 * This function initializes three border strings: top border (topStr), middle border (midStr),
 * and bottom border (bottomStr). The border strings are composed of "━" and "┃" characters,
 * with lengths dynamically adjusted according to the global variable size.
 * 
 * Parameters:
 * None
 * 
 * Return Value:
 * None
 */
static void initStr()
{
    // Initialize the top border string
    strcpy(topStr, "┏━━━━━");
    for (size_t i = 0; i < size - 1; i++)
    {
        strcat(topStr, "┳━━━━━");
    }
    strcat(topStr, "┓");

    // Initialize the middle border string
    strcpy(midStr, "┣━━━━━");
    for (size_t i = 0; i < size - 1; i++)
    {
        strcat(midStr, "╋━━━━━");
    }
    strcat(midStr, "┫");

    //Initialize the bottom border string
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

/**
 * Inserts a new point into the array
 *
 * This function is used to insert a new point into an array where a series of 0s represent empty spaces.
 * The function randomly selects an empty space and sets it to 2, indicating that a point has been inserted.
 *
 * @return Returns 1 if the array is full and a new point cannot be inserted; returns 0 if the point is successfully inserted.
 */
int insert_one() 
{
    // 1. Count the number of empty spaces (values equal to 0) in the array.
    size_t count = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (map[i] == 0)
            ++count;
    }

    // Check if the array is full, preventing further insertion if so.
    if (count == 0)
        return 1;

    // 2. Generate a random number based on the number of empty spaces to determine the insertion position.
    size_t num = rand() % count;

    // 3. Iterate through the array, find the nth empty space, and set it to 2.
    count = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (map[i] == 0)
        {
            if (count++ == num)
                map[i] = 2; // Found the designated position, insert the new point.
        }
    }
    // Insertion successful, return 0.
    return 0;
}
/**
 * @brief Merges and sums groups of identical numbers in an array of integer pointers.
 *
 * @param arr A pointer to an array of integer pointers. Each pointer points to an integer representing the numbers to be processed.
 *
 * This function iterates through the given number groups, merging identical numbers and accumulating the sum of merged values into a global variable 'score'.
 * Each number can belong to only one group, and when two numbers are merged, one is set to zero to indicate processing.
 */
void move_group(int *arr[])
{
    // Initialize the index of the current group and its head element pointer
    int cur = 0;
    int *pHead = arr[cur];

    // Iterate through all numbers in the groups
    for (size_t i = 1; i < size; i++)
    {
        int *pNum = arr[i];

        // Skip over zero numbers
        if (*pNum == 0)
            continue;

        // If the current group is empty, assign the current number as the new group's head element
        if (*pHead == 0)
        {
            *pHead = *pNum;
            *pNum = 0;
            continue;
        }

        // If the current number equals the head element's value, merge them and update the score
        if (*pHead == *pNum)
        {
            *pHead = *pHead + *pNum;
            score += *pHead; // Update the score
            *pNum = 0; // Mark the current number as processed
            pHead = arr[++cur]; // Move to the next group
        }
        else
        {
            // If the current number doesn't match the head element's value, move it to the next group
            pHead = arr[++cur];

            // If the next group's pointer is not equal to the current number's pointer, copy the current number to the new group
            if (pHead != pNum)
            {
                *pHead = *pNum;
                *pNum = 0; // Mark the current number as processed
            }
        }
    }
}
/**
 * @brief Main game loop function, controls the progression of the game.
 *
 * This function continuously reads user input and executes the corresponding game operations such as moving game blocks, inserting new blocks, and displaying the game state,
 * until the game ends.
 */
void play() // Game operations
{
    while (1)
    {
        int ch = getchar(); // Read user input

        switch (ch)
        {
        // Handle user input for block movement commands
        case 65: // ASCII code for 'W', move up
        case 119:
            // Move blocks upward
            for (int i = 0; i < size; i++)
            {
                int *pArr[size];
                int index = 0;
                // Build an array of pointers to the current row's blocks
                for (int j = 0; j < size; j++)
                {
                    pArr[index++] = (int *)map + j * size + i;
                }
                move_group(pArr); // Move the current row's blocks
            }
            break;
        case 97: // ASCII code for 'a', move left
        case 68:
            // Move blocks left
            for (int i = 0; i < size; i++)
            {
                int *pArr[size];
                int index = 0;
                // Build an array of pointers to the current column's blocks
                for (int j = 0; j < size; j++)
                    pArr[index++] = (int *)map + i * size + j;
                move_group(pArr); // Move the current column's blocks
            }
            break;
        case 100: // ASCII code for 'd', move right
        case 67:
            // Move blocks right
            for (int i = size - 1; i >= 0; i--)
            {
                int *pArr[size];
                int index = 0;
                // Build an array of pointers to the reversed column's blocks
                for (int j = size - 1; j >= 0; j--)
                    pArr[index++] = (int *)map + i * size + j;
                move_group(pArr); // Move the reversed column's blocks
            }
            break;
        case 115: // ASCII code for 's', move down
        case 66:
            // Move blocks downward
            for (int i = 0; i < size; i++)
            {
                int *pArr[size];
                int index = 0;
                // Build an array of pointers to the reversed row's blocks
                for (int j = size - 1; j >= 0; j--)
                    pArr[index++] = (int *)map + j * size + i;
                move_group(pArr); // Move the reversed row's blocks
            }
            break;
        // Handle game exit logic
        case 113:
            game_over();
            break;
        default:
            continue; // Ignore other inputs
        }

        // Attempt to insert a new block
        int ret = insert_one();
        if (ret == 1) // If insertion fails, the game ends
        {
            printf("\nGame Over!\n");
            game_over(); // Handle game end
            return;
        }

        // Update the game display
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
