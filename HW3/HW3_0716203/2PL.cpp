#include <iostream>
#include <vector>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <semaphore.h>
#include <fstream>
#include <queue>
#include <ctime>
using namespace std;


vector<int> num;
vector<vector<string>> number(100);
vector<vector<string>> operation(100);
sem_t semaphore, semaphore2;
vector<pthread_mutex_t> locker;
pthread_mutex_t job_lock;
clock_t Begin, End;
double duration;

struct mystruct
{
    vector<string> number;
    vector<string> operation;
    vector<int> var;
};

queue<mystruct> q;
void *cal(void *p);

int main(int argc, char *argv[])
{

    Begin = clock(); //開始計時
    int thread_num = atoi(argv[1]);
    string output_name = argv[2];
    int N;
    cin >> N;
    num.resize(N);
    locker.resize(N);

    for (int i = 0; i < N; i++)
    {
        cin >> num[i];
        pthread_mutex_init(&locker[i], 0);
    }

    string temp;
    vector<string> equation;
    vector<vector<string>> sequence(100);
    vector<vector<int>> var(100);
    sem_init(&semaphore, 0, 0);
    sem_init(&semaphore2, 0, 0);

    getline(cin, temp);
    while (getline(cin, temp))
        equation.push_back(temp);

    int front = 0, back;
    for (int i = 0; i < equation.size(); i++)
    {

        front = 0;
        for (int j = 0; j < equation[i].size(); j++)
        {
            if (equation[i][j] == ' ')
            {
                back = j;
                string s;
                for (int k = front; k < back; k++)
                    s.push_back(equation[i][k]);
                sequence[i].push_back(s);
                front = back + 1;
            }
        }
        back = equation[i].size();
        string s;
        for (int k = front; k < back; k++)
            s.push_back(equation[i][k]);
        sequence[i].push_back(s);
    }

    //int sum = 0;
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < sequence[i].size(); j++)
        {
            if (sequence[i][j] == "+" || sequence[i][j] == "-")
                operation[i].push_back(sequence[i][j]);
            else if (sequence[i][j] != "=")
            {
                number[i].push_back(sequence[i][j]);
                if (sequence[i][j][0] == '$')
                {
                    int t = stoi(sequence[i][j].erase(0, 1));
                    bool same = false;
                    for (int k = 0; k < var[i].size(); k++)
                    {
                        if (t == var[i][k])
                        {
                            same = true;
                            break;
                        }
                    }
                    if (same == false)
                        var[i].push_back(t);
                }
            }
        }
    }
    struct mystruct data[100];
    for (int i = 0; i < 100; i++)
    {
        data[i].number = number[i];
        data[i].operation = operation[i];
        data[i].var = var[i];
        q.push(data[i]);
        sem_post(&semaphore2);
    }

    pthread_t t[thread_num]; // 宣告 pthread 變數
    for (int i = 0; i < thread_num; i++)
        pthread_create(&t[i], NULL, cal, NULL); // 建立子執行緒

    for (int i = 0; i < 100; i++)
        sem_wait(&semaphore);

    for (int i = 0; i < thread_num; i++)
        pthread_join(t[i], NULL);

    ofstream file(output_name);
    if (file.is_open())
    {
        for (int i = 0; i < num.size(); i++)
            file << '$' << i << " = " << num[i] << endl;
    }

    End = clock(); //結束計時
    duration = double(End - Begin) / CLOCKS_PER_SEC;
    cout << "Thread number : " << thread_num << endl;
    cout << "Time :" << duration << " s" << endl;

    return 0;
}

void *cal(void *p)
{
    while (q.size() != 0)
    {
        if (sem_trywait(&semaphore2)==0)
        {
            pthread_mutex_lock(&job_lock);
            struct mystruct my_data;
            my_data = q.front();
            q.pop();
            for (int i = 0; i < my_data.var.size(); i++)
                pthread_mutex_lock(&locker[my_data.var[i]]);
            pthread_mutex_unlock(&job_lock);
            vector<int> input;
            input = num;
            for (int i = 1; i < my_data.var.size(); i++)
                pthread_mutex_unlock(&locker[my_data.var[i]]);

            int write_target = my_data.var[0];
            vector<string> read_string;
            vector<int> read_data;

            int j = 0;
            for (int i = 1; i < my_data.number.size(); i++)
            {
                if (my_data.number[i][0] == '$')
                {
                    read_string.push_back(my_data.number[i].erase(0, 1));
                    int read_target = stoi(read_string[j]);
                    read_data.push_back(input[read_target]);
                    j++;
                }
                else
                    read_data.push_back(stoi(my_data.number[i]));
            }

            int sum = read_data[0];
            for (int i = 0; i < my_data.operation.size(); i++)
            {
                if (my_data.operation[i] == "+")
                    sum += read_data[i + 1];
                else if (my_data.operation[i] == "-")
                    sum -= read_data[i + 1];
            }
            num[write_target] = sum;
            pthread_mutex_unlock(&locker[write_target]);
            sem_post(&semaphore);
            
        }
        
        
    }
    pthread_exit(NULL);
}