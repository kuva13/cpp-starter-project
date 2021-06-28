#include <iostream>
#include <ctime>
#include <cstdlib>//stdlib.h — заголовочный файл стандартной библиотеки, который содержит в себе функции, занимающиеся выделением памяти, контролем процесса выполнения программы, преобразованием типов и другие.
#include <windows.h>
#include <string> //Реализует стандартные строковые классы и шаблоны

using namespace std;

// размеры поля
#define WIDTH 30
#define HEIGHT 15

#define RECORDS_COUNT 5

// константы для ячеек поля
const char EMPTY_CELL = ' ';
const char HEAD_CELL = '@';
const char SNAKE_CELL = '#';
const char FOOD_CELL = 'x';

// константы для направлений
const int UP = 0;
const int DOWN = 1;
const int RIGHT = 2;
const int LEFT = 3;

// элемент списка(используется для змеи и еды)
struct Node {
    int x; // координаты
    int y;
    Node *next; // указатель на следующий элемент списка
};

// список
struct List {
    Node *head; // указатель на начало списка
};

// змея
struct Snake {
    List cells; // список занимаемых клеток
    int direction; // направление
    int remain; // остаток не добавленного в хвост
};

// игра
struct Game {
    Snake snake; // змея
    int foodX; // еда
    int foodY; // еда
    int time; // время задержки - по факту уровень игры
    int scores; // очки
};


// инициализация пустого списка
List InitList() {
    List list;
    list.head = NULL; // обнуляем указатель на начало списка

    return list; // возвращаем созданный пустой список
}

// добавление в начало списка
void AddFront(List &list, int x, int y) {
    Node *node = new Node; // выделяем память под элемент списка

    node->x = x;
    node->y = y;
    node->next = list.head; // следующий элемент - первый в списке
    list.head = node; // обновляем начало списка
}

// добавление в конец списка
void AddBack(List &list, int x, int y) {
    Node *node = new Node; // создаём элемент

    node->x = x;
    node->y = y;
    node->next = NULL; // после элемента ничего нет

    if (list.head == NULL) { // если начала списка нет
        list.head = node; // то этот элемент и является началом
    }
    else {
        Node *prev = list.head;

        // иначе идём до последнего элемента
        while (prev->next)
            prev = prev->next;

        prev->next = node; // и обновляем указатель на следующий элемент
    }
}

// получение элемента в списке по координата
Node* ElemInList(List list, int x, int y) {
    for (Node *node = list.head; node; node = node->next)
        if (node->x == x && node->y == y)
            return node;

    return NULL; // если не нашли, возвратили NULL
}

// ход змеи
void MoveSnake(Snake &snake) {
    Node* head = snake.cells.head; // получили клетку её головы

    // добавили новую клетку для головы змеи в нужном направлении
    if (snake.direction == UP) {
        AddFront(snake.cells, head->x, (head->y - 1 + HEIGHT) % HEIGHT);
    }
    else if (snake.direction == DOWN) {
        AddFront(snake.cells, head->x, (head->y + 1) % HEIGHT);
    }
    else if (snake.direction == RIGHT) {
        AddFront(snake.cells, (head->x + 1) % WIDTH, head->y);
    }
    else {
        AddFront(snake.cells, (head->x - 1 + WIDTH) % WIDTH, head->y);
    }

    // если змея ещё не расширилась до нужного размера - выходим
    if (snake.remain > 0 || !snake.cells.head->next) {
        snake.remain--;
        return;
    }

    // удаляем последний элемент в списке (чтоб змея сдвигалась сзади)
    Node *elem = snake.cells.head; // получаем голову змеи

    // находим предпоследний элемент
    while (elem->next->next) 
        elem = elem->next;

    // удаляем его
    Node* tmp = elem->next;
    elem->next = NULL;
    delete tmp;
}

// проверка корректного положения змеи
int IsSnakeValid(Snake snake) {
    // проверяем на пересечение головы с хвостом
    for (Node* node = snake.cells.head->next; node; node = node->next)
        if (node->x == snake.cells.head->x && node->y == snake.cells.head->y)
            return 0;

    return 1;
}

// инициализация еды
void InitFood(Snake snake, int &foodX, int &foodY) {
    // генерируем координаты
    do {
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;
    } while (ElemInList(snake.cells, foodX, foodY));
}

// обновления списка еды (если нужно будет) и изменение скорости змеи
void UpdateFood(Game &game) {
    // получили координаты головы змеи
    int x = game.snake.cells.head->x;
    int y = game.snake.cells.head->y;

    if (x != game.foodX || y != game.foodY) // если не нашли еду
        return; // вышли из функции

    game.snake.remain++; // нарастили кол-во на которое нужно увеличить змею
    game.scores++; // увеличили очки
    game.time = game.time * 10 / 11; // обновили время

    InitFood(game.snake, game.foodX, game.foodY);
}

// инициализация змеи
Snake InitSnake() {
    Snake snake; // змея

    // задали параметры
    snake.cells = InitList();
    snake.remain = 3;
    snake.direction = rand() % 4;

    // добавили голову
    AddFront(snake.cells, 2 + rand() % (WIDTH - 4), 2 + rand() % (HEIGHT - 4));

    // сместили голову на нужное кол-во
    while (snake.remain) {
        MoveSnake(snake);
    }

    return snake;
}

// инициализация игры
Game InitGame() {
    Game game;

    game.snake = InitSnake();
    game.scores = 0;
    game.time = 100;

    InitFood(game.snake, game.foodX, game.foodY);

    return game;
}

// отрисовка поля
void DrawGame(Game game, int isGameOver) {
    char field[HEIGHT + 2][WIDTH + 3]; // матрица для отрисовки

    // заполнили границы
    field[0][0] = '+';
    field[0][WIDTH + 1] = '+';
    field[HEIGHT + 1][0] = '+';
    field[HEIGHT + 1][WIDTH + 1] = '+';

    field[0][WIDTH + 2] = '\0';
    field[HEIGHT + 1][WIDTH + 2] = '\0';

    for (int i = 1; i < HEIGHT + 1; i++) {
        field[i][0] = '|';
        field[i][WIDTH + 1] = '|';
        field[i][WIDTH + 2] = '\0';
    }

    for (int i = 1; i < WIDTH + 1; i++) {
        field[0][i] = '-';
        field[HEIGHT + 1][i] = '-';
    }

    // заполнили внутреннюю часть пустыми клетками
    for (int i = 1; i < HEIGHT + 1; i++)
        for (int j = 1; j < WIDTH + 1; j++)
            field[i][j] = EMPTY_CELL;

    List snake = game.snake.cells;

    // заполнили змею на поле
    for (Node* node = snake.head; node; node = node->next)
        field[node->y + 1][node->x + 1] = node == snake.head ? HEAD_CELL : SNAKE_CELL;

    if (!isGameOver) {
        field[game.foodY + 1][game.foodX + 1] = FOOD_CELL; // заполнили еду на поле
    }
    else {
        int y = HEIGHT / 2;
        int x = (WIDTH - 10) / 2;
        char s[] = "GAME OVER!";

        for (int i = 0; i < 10; i++)
            field[y][x + i] = s[i];
    }

    system("cls");
    cout << "                         Score: " << game.scores << endl; // вывели текущий результат

    // вывели поле
    for (int i = 0; i < HEIGHT + 2; i++)
        cout << field[i] << endl;
}

// реакция на клавиши
void Control(Game &game) {
    int c = clock() + game.time;
    while (clock() < c) {
        // меняем направление по клавише
        if (GetAsyncKeyState(0x41) && game.snake.direction != RIGHT) {
            game.snake.direction = LEFT;
        }
        else if (GetAsyncKeyState(0x44) && game.snake.direction != LEFT) {
            game.snake.direction = RIGHT;
        }
        else if (GetAsyncKeyState(0x57) && game.snake.direction != DOWN) {
            game.snake.direction = UP;
        }
        else if (GetAsyncKeyState(0x53) && game.snake.direction != UP) {
            game.snake.direction = DOWN;
        }
    }    
}

// задержка на время
void Delay(int ms) {
    int c = clock() + ms;

    while (clock() < c);
}

// игра
int Play() {
    Game game = InitGame();
    DrawGame(game, 0);
    Delay(1500);
/**
 * @brief game принимает значение параметра функции InitGame
 * DrawGame функция рисующая поля
 * Delay функия задерки времени на начале игры
 * 
 */
    while (1) {
        DrawGame(game, 0);
        Control(game);
        MoveSnake(game.snake);

        if (!IsSnakeValid(game.snake)) // если врезалась змея
            break; // выходим

        UpdateFood(game);
    }

    DrawGame(game, 1);
    return game.scores;

/**
 * @brief Control функия отвечающая на реакцию клавиш для передвежения
 * MoveSnake фунция хода змеи
 * 
 * 
 */
}

int main() {
    srand(time(NULL));
    string answer;
/**
 * @brief srand выполняет инициализацию генератора случайных чисел rand.
 * @brief answer принимает стринговое значение 
 * 
 */
    do {
        int scores = Play();
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        cout << "Was eaten " << scores << endl;
        cout << "Play again (1) or Close the game (0)?: ";
        getline(cin, answer);

        while (answer != "1" && answer != "0") {
            cout << "Invalid action number, try again: " << endl;
            getline(cin, answer);
        }
    } while (answer == "1");
/**
 * @brief scores равен функции Play()
 * getline принимает папарметры cin и answer
 * answer принимает значение 1 или 0 
 * когда answer равен 1, то оно перезапкскает игру, иначе выходит из игры 
 * 
 * @param answer-answer принимает только 1 или 0
 */
    return 0;
}


/*Библиотека:
https://demin.ws/blog/russian/2012/12/10/snake-game/
https://docs.microsoft.com/ru-ru/cpp/cpp/cpp-language-reference?view=msvc-160
https://habr.com/ru/post/348262/
https://ru.wikipedia.org/wiki/%D0%A1%D1%82%D0%B0%D0%BD%D0%B4%D0%B0%D1%80%D1%82%D0%BD%D0%B0%D1%8F_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA%D0%B0_%D1%8F%D0%B7%D1%8B%D0%BA%D0%B0_C%2B%2B
И несколько видео на ютубе*/