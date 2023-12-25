#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <conio.h>
#include <windows.h>

#define BOARDSIZE (30)
#define FRUITCOUNT (5)
struct body
{
    int x;
    int y;
    struct body *next;
};

char **create_map();
void generate_fruit(char **map);
int input(struct body **snake, int ori_direction, int *gameover);
void clear_map(char **map);
void show_map(char **map, int *score);
void draw_snake(char **map, struct body *tail);
struct body *snake_move(char **map, struct body *snake, int direction, int *score, int *gameover);
struct body *reverse(struct body *snake);
int get_reverse_direction(struct body *snake, int ori_direction);

int main()
{
    int gameover = 0;
    int score = 0;
    int fps = 3;

    srand( time(NULL) );
    printf("Choose your fps of this game:\t");
    scanf("%d", &fps);
    char **map = create_map();
    for(int i = 0; i < FRUITCOUNT; i++)
        generate_fruit(map);
    //show_map(map, &score);

    struct body *snake1 = (struct body*)malloc(sizeof(struct body)); 
    snake1->x = BOARDSIZE/2;
    snake1->y = BOARDSIZE/2;
    snake1->next = NULL;
    int snake1_dir = 0;

    while(!gameover)
    {
        clear_map(map);
        draw_snake(map, snake1);
        show_map(map, &score);
        printf("now direction:%d\n",snake1_dir);
        Sleep(1000/fps); 
        snake1_dir = input(&snake1, snake1_dir, &gameover);    
        if(snake1_dir != 0)
            snake1 = snake_move(map, snake1, snake1_dir, &score, &gameover);
        struct body *show = snake1;
        // while(show)
        // {
        //     printf("(%d,%d)->",show->x, show->y);
        //     show = show->next;
        // }
        Sleep(1000/fps);
    }


    printf("\nGameover, your score: %d!\n",score);

    return 0;
}

char **create_map()
{
    char **map = (char**)malloc((BOARDSIZE+2)*sizeof(char*));
    for(int i = 0; i < BOARDSIZE+2; i++)
        map[i] = (char*)malloc((BOARDSIZE+2)*sizeof(char));
    
    for(int i = 0; i < BOARDSIZE+2; i++)
    {
        for(int j = 0; j < BOARDSIZE+2; j++)
        {
            if(i == 0 || i == BOARDSIZE+1 || j == 0 || j == BOARDSIZE+1)
                map[i][j] = '#';
            else
                map[i][j] = ' ';
        }
    }

    return map;
    
}
void generate_fruit(char **map)
{
    int x = rand() % (BOARDSIZE+1) + 1; //generate between 1 and BOARDSIZE+1
    int y = rand() % (BOARDSIZE+1) + 1;
    while(map[x][y]!=' ') // overlap to snake, random again
    {
        x = rand() % (BOARDSIZE+1) + 1; 
        y = rand() % (BOARDSIZE+1) + 1;
    }
    map[x][y] = '*';
}
void clear_map(char **map)
{
    for(int i = 0; i < BOARDSIZE+2; i++)
    {
        for(int j = 0; j < BOARDSIZE+2; j++)
        {
            if(map[i][j] == 'O' || map[i][j] == 'D')
                map[i][j] = ' ';
        }
    }
}
void show_map(char **map, int *score)
{
    system("cls");
    for(int i = 0; i < BOARDSIZE+2; i++)
    {
        for(int j = 0; j < BOARDSIZE+2; j++)
            printf("%c ", map[i][j]);
        if(i != BOARDSIZE+1)
            printf("\n");
    }
    printf("\t\t\tScore:%d\n", *score);
}
void draw_snake(char **map, struct body *snake)
{
    struct body *current = snake;
    while(current) 
    {   //change ' ' to snake's body for each node of snake
        if(current->next == NULL)   //head
            map[current->x][current->y] = 'D'; 
        else                        //body
            map[current->x][current->y] = 'O'; 
        current = current->next;
    }
}
int input(struct body **snake, int ori_direction, int *gameover)
{
    struct body *show = *snake;
    struct body *temp = NULL;
    int new_direction = ori_direction;
    if(_kbhit())
    {
        switch (_getch())
        {
            case 'w':   // up
                new_direction = 1; 
                break;
            case 'd':   // right
                new_direction = 10; 
                break;
            case 's':   // down
                new_direction = -1; 
                break;
            case 'a':   // left
                new_direction = -10; 
                break;
            case 'r':   // reverse
                new_direction = get_reverse_direction(*snake, ori_direction);
                temp = reverse(*snake);
                *snake = temp;
                return new_direction;
                break;
            case 'x':
                printf("leave\n");
                *gameover = 1;
                break;
        }
        if(ori_direction !=0 && new_direction == -1 * ori_direction) //press opposite direction
            return  ori_direction;
        // if(ori_direction !=0 && abs(new_direction - ori_direction) == 2) //press opposite direction
        //     return ori_direction;
        else
            return new_direction;
    }
    return ori_direction;
}
struct body *snake_move(char **map, struct body *snake, int direction, int *score, int *gameover)
{
    
    struct body *head = snake;
    
    while(head->next)
        head = head->next; //get to head of this snake
    struct body *new = (struct body*)malloc(sizeof(struct body)); 
    head->next = new; //make a new head for snake
    new->next = NULL;
    switch (direction) //decide new head's location
    {
        case 1://up
            new->x = head->x-1;
            new->y = head->y;
            break;
        case 10://right
            new->x = head->x;
            new->y = head->y+1;
            break;
        case -1://down
            new->x = head->x+1;
            new->y = head->y;
            break;
        case -10://left
            new->x = head->x;
            new->y = head->y-1;
            break;
        default:
            return snake;
            break;
    }

    if(map[new->x][new->y] == ' ' || (new->x == snake->x) && (new->y == snake->y))
    {   //Free tail and insert head for moving.
        struct body *tail = snake->next; 
        free(snake);
        return tail; //return new tail of snake
    }
    else if(map[new->x][new->y] == '*')
    {   //If snake ate a fruit, it get new body on tail, so don't have to free tail
        *score+=10; 
        generate_fruit(map); //A fruit was ate, generate a new fruit
        return snake; 
    }    
    else 
    {   //maybe wall(#) or snake's body, end game
        map[new->x][new->y] = 'x';
        *gameover = 1;
        return snake;
    }
}

struct body *reverse(struct body *snake)
{
    struct body *current = snake;
    struct body *prev = NULL;
    struct body *prec = NULL;
    while(current)
    {
        prec = current->next;
        current->next = prev;
        prev = current;
        current = prec;
    }
    return prev;
}

int get_reverse_direction(struct body *snake, int ori_direction)
{
    if(!snake->next)
        return -1*ori_direction;
    if(snake->x == snake->next->x - 1) //origin go down, go up
        return 1;
    else if(snake->y == snake->next->y + 1) // origin go left, go right
        return 10;
    else if(snake->x == snake->next->x + 1) //origin go up, go down
        return -1;
    else if(snake->y == snake->next->y - 1) // origin go right, go left
        return -10;    
}
       

