#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <stdlib.h>

using namespace std;

// Linked list node structure
typedef struct Node
{
    int process;             // Process ID
    int arrival_time;        // Arrival time of the process
    int burst_time;          // Burst time of the process
    int priority;            // Priority of the process
    int turnaround_time;     // Turnaround time of the process
    int waiting_time;        // Waiting time of the process
    int time_remaining;      // Remaining time for the process (used in Round Robin)
    float average_time;      // Average time (not used)
    struct Node *next;       // Pointer to the next node in the linked list
} NODE;

NODE *head, *tail = NULL;  // Head and tail pointers for the linked list
// Validator for input options
int Validator(int option, int startLimit, int endLimit)
{
    if ((option <= startLimit || option <= endLimit) && (option != 0))
    {
        return true;
    }
    else
    {
        // Clear input buffer and ignore invalid input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
}

// Validator for time inputs
int timeValidator(int time)
{
    if (time == 0)
    {
        // Clear input buffer and ignore invalid time
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    else
    {
        return true;
    }
}

// Function to sort the linked list based on different criteria
void sortHandler(int method)
{

    NODE *current = head, *index = NULL;

    int temp, burst, prior;

    if (head == NULL)
    {
        return;
    }
    else
    {
        while (current != NULL)
        {
            index = current->next;

            while (index != NULL)
            {
                if (method == 1) // Sort list according to arrival time
                {
                    if (current->arrival_time > index->arrival_time)
                    {
                        // Swap arrival time, burst time, and priority
                        temp = current->arrival_time;
                        current->arrival_time = index->arrival_time;
                        index->arrival_time = temp;

                        burst = current->burst_time;
                        current->burst_time = index->burst_time;
                        index->burst_time = burst;

                        prior = current->priority;
                        current->priority = index->priority;
                        index->priority = prior;
                    }
                }
                else if (method == 2) // Sort list according to burst time
                {
                    if (current->burst_time > index->burst_time)
                    {
                        // Swap burst time, arrival time, and priority
                        burst = current->burst_time;
                        current->burst_time = index->burst_time;
                        index->burst_time = burst;

                        temp = current->arrival_time;
                        current->arrival_time = index->arrival_time;
                        index->arrival_time = temp;

                        prior = current->priority;
                        current->priority = index->priority;
                        index->priority = prior;
                    }
                }
                else if (method == 3) //sort list according to priority
                {
                    if (current->priority > index->priority)
                    {
                        // Swap priority, burst time, and arrival time
                        prior = current->priority;
                        current->priority = index->priority;
                        index->priority = prior;

                        burst = current->burst_time;
                        current->burst_time = index->burst_time;
                        index->burst_time = burst;

                        temp = current->arrival_time;
                        current->arrival_time = index->arrival_time;
                        index->arrival_time = temp;
                    }
                }

                index = index->next;
            }
            current = current->next;
        }
    }
}

// Function to control Round Robin Scheduling
void RRScontroller(NODE *head, int time_quantum, int count, string file)
{

    float total = 0;
    ofstream myfile;
    myfile.open(file.c_str(), ios_base::app);

    // Round Robin scheduling linked list
    sortHandler(1);

    head->waiting_time = 0;

    int current_time = 0;
    int i = 0;
    myfile << "\nScheduling Method: Robin Round Scheduling Method" << endl;
    myfile << "Process Waiting Times :\n"
           << endl;
    do
    {
        bool counter = true;

        while (head != NULL)
        {
            if (head->time_remaining > 0)
            {
                counter = false;

                if (head->time_remaining > time_quantum)
                {
                    current_time = current_time + time_quantum;

                    head->time_remaining -= time_quantum;
                }
                else
                {
                    current_time = current_time + head->time_remaining;

                    head->waiting_time = current_time - head->burst_time;

                    head->time_remaining = 0;
                }
            }
            total += head->waiting_time;
            i++;
            myfile << "P" << i << " : " << head->waiting_time << endl;
            head = head->next;
        }
        if (counter == true)
            break;
    } while (true);

    myfile << "\nAverage Waiting Time: " << total / count << endl;
    myfile.close();
}

//insert data into the linked list
void insertHandler(NODE **head, int process, int arrival_time, int burst_time, int priority, int turnaround_time)
{

    NODE *point, *r = *head;
    point = (NODE *)malloc(sizeof(NODE));
    point->process = process;
    point->arrival_time = arrival_time;
    point->burst_time = burst_time;
    point->priority = priority;
    point->turnaround_time = turnaround_time;
    point->time_remaining = point->burst_time; //for round robin scenario

    point->next = NULL;
    if (*head == NULL)
        *head = point;
    else
    {
        while (r->next != NULL)
            r = r->next;
        r->next = point;
    }
}

// Calculate waiting times for processes
void calculationController(NODE *point, int method)
{
    sortHandler(method);
    while (point != NULL)
    {
        point->waiting_time = point->turnaround_time - point->burst_time;
        point = point->next;
    }
}

//display the linked list
void outputHandler(NODE *p, int count, string output_file, int method, string input_file)
{
    ofstream myfile;
    myfile.open(output_file.c_str(), ios_base::app);

    if (method == 1)
    {
        myfile << "\n\nScheduling Method: First Come First Serve" << endl;
        myfile << "Screen Output: " << input_file << ":\n" << endl;
        myfile << "Process Waiting Times :\n"
               << endl;
    }
    else if (method == 2)
    {
        myfile << "\n\nScheduling Method: Shortest Job First - Non-Premtive Mode" << endl;
        myfile << "Screen Output: " << input_file << ":\n" << endl;
        myfile << "Process Waiting Times :\n"
               << endl;
    }
    else if (method == 3)
    {
        myfile << "\n\nScheduling Method: Priority Scheduling - Non-Premtive Mode" << endl;
        myfile << "Screen Output: " << input_file << ":\n" << endl;
        myfile << "Process Waiting Times :\n"
               << endl;
    }

    float totalWaitingTime = 0.0;
    while (p != NULL)
    {
        myfile << "P" << p->process << " : " << p->waiting_time << " ms" << endl; // append " ms" to the waiting time
        totalWaitingTime += p->waiting_time;
        p = p->next;
    }
    myfile << "\nAverage Waiting Time: " << totalWaitingTime / count << " ms" << endl; // append " ms" to the average waiting time
    myfile.close();
}

//Shortest Job First Premtive Mode Algoritham
void premtiveSJF(NODE *head, int count, string file)
{

    int t = 0;
    int i = 0;
    int complete = 0;
    int time_taken;
    bool serve = false;
    float total = 0;

    ofstream myfile;
    myfile.open(file.c_str(), ios_base::app);

    //sort linked list according to burst_time
    sortHandler(2);
    myfile << "\nScheduling Method: Shortest Job First - Premtive Mode" << endl;
    myfile << "Process Waiting Times :\n"
           << endl;
    while (complete != count)
    {
        while (head != NULL)
        {

            if ((head->arrival_time <= t) && (head->time_remaining > 0))
            {
                serve = true;
            }

            if (serve == false)
            {
                t++;
                continue;
            }
            //linked list is sorted so whenever the time will come we will decrease it until it will be zero
            head->time_remaining = head->time_remaining - 1;

            if (head->time_remaining == 0)
            {
                complete++;

                serve = false;

                time_taken = t + 1;

                head->waiting_time = time_taken - head->burst_time - head->arrival_time;

                myfile << "P" << i + 1 << " : " << head->waiting_time << endl;

                total += head->waiting_time;
                i++;
                head = head->next;
            }

            t++;
        }
    }
    myfile << "\nAverage Waiting Time: " << total / count << endl;
    myfile.close();
}

//Priority Scheduling Premtive Mode Algoritham
void premtivePriorityScheduler(NODE *head, int count, string file)
{

    int t = 0;
    int i = 0;
    int complete = 0;
    int time_taken;
    float total = 0;
    bool serve = false;

    ofstream myfile;
    myfile.open(file.c_str(), ios_base::app);

    //sort linked list according to burst_time
    sortHandler(3);
    myfile << "\nScheduling Method: Priority Scheduling - Premtive Mode" << endl;
    myfile << "Process Waiting Times :\n"
           << endl;
    while (complete != count)
    {
        while (head != NULL)
        {

            if ((head->arrival_time <= t) && (head->time_remaining > 0))
            {
                serve = true;
            }

            if (serve == false)
            {
                t++;
                continue;
            }
            //linked list is sorted so whenever the time will come we will decrease it until it will be zero
            head->time_remaining = head->time_remaining - 1;

            if (head->time_remaining == 0)
            {
                complete++;

                serve = false;

                time_taken = t + 1;

                head->waiting_time = time_taken - head->burst_time - head->arrival_time;

                myfile << "P" << i + 1 << " : " << head->waiting_time << endl;
                total += head->waiting_time;
                i++;
                head = head->next;
            }

            t++;
        }
    }
    myfile << "\nAverage Waiting Time: " << total / count << endl;
    myfile.close();
}

//main
int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        cout << "Type error: command line arguments missing for input and output file \nPlease re-run it";
        cout << "\n\nType ./main -h for help [*]\n";

        return 1;
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-h") == 0)
        {
            cout << "Commands:\n"
                 << endl;
            cout << "-f <input file>\n";
            cout << "-o <output file>\n";
        }
    }

    else
    {
        int i = 0;
        int count = 0;
        int chr = 0;
        int option;
        bool validate = true;
        int sum = 0;
        string data;

        ifstream input(argv[2]);
        ifstream input2;
        input2.open(argv[2]);

        //To know the length of lines of the file
        //number of lines in file = lenth of the array of struct

        while (!input2.eof())
        {
            getline(input2, data);

            count++;
        }

        //assigning values to structure processModel
        for (i = 0; i < count; i++)
        {
            int counter = 0;
            string burst_time, arrival, priority;
            getline(input, data);
            chr = data.length();

            for (int j = 0; j < chr; j++)
            {

                if (data[j] == ':')
                {
                    counter++;
                    continue;
                }
                //counter = 0 means that we did not detect the first colon
                if (counter == 0)
                {
                    burst_time += data[j];
                }
                //when counter is 1 it means we passed the first colom and we detct the first colon
                if (counter == 1)
                {
                    arrival += data[j];
                }
                // counter = 2 means we are the the last colon part
                if (counter == 2)
                {
                    priority += data[j];
                }
            }

            int burst, arrival2, prior;

            stringstream burstTime;
            burstTime << burst_time;
            burstTime >> burst;

            stringstream arrivalTime;
            arrivalTime << arrival;
            arrivalTime >> arrival2;

            stringstream priorityValue;
            priorityValue << priority;
            priorityValue >> prior;

            sum += burst;
            // head = insertController(head, i+1 , arrival2, burst, prior);
            insertHandler(&head, i + 1, arrival2, burst, prior, sum);
        }

        //Ask user to input method
        do
        {

            if (validate)
            {
                cout << "\nPlease choose from the given options below" << endl;
            }

            cout << "\n 1) Scheduling Method " << endl;
            cout << "\n 2) Show Result" << endl;
            cout << "\n 3) End Program\n"
                 << endl;

            cout << "Option> ";

            cin >> option;

            //validator return false for not valid data
            validate = Validator(option, 1, 3);

            if (!validate)
            {
                cout << "\n Please enter the correct choice" << endl;
            }
            else
            {
                // 1)
                if (option == 1)
                {
                    do
                    {
                        cout << "\n 1) First Come, First Served Scheduling" << endl;
                        cout << "\n 2) Shortest-Job-First Scheduling " << endl;
                        cout << "\n 3) Priority Scheduling " << endl;
                        cout << "\n 4) Round-Robin Scheduling\n"
                             << endl;

                        cout << "Option> ";

                        cin >> option;

                        validate = Validator(option, 1, 4);
                        if (!validate)
                        {
                            cout << "\n Please enter the correct choice" << endl;
                        }
                        else
                        {
                            // 1) FCFS
                            if (option == 1)
                            {

                                calculationController(head, 1);

outputHandler(head, count, argv[4], 1, "input_file");  
                            }
                            // 2) SJF
                            else if (option == 2)
                            {

                                do
                                {
                                    cout << "\n 1) Premtive Mode \t";
                                    cout << " 2) Non-Premtive Mode \n"
                                         << endl;

                                    cout << "Option> ";

                                    cin >> option;

                                    validate = Validator(option, 1, 2);

                                    if (!validate)
                                    {
                                        cout << "\n Please enter the correct choice" << endl;
                                    }

                                } while (!validate);

                                if (option == 1)
                                {
                                    premtiveSJF(head, count, argv[4]);
                                }
                                else if (option == 2)
                                {
                                    calculationController(head, 2);

outputHandler(head, count, argv[4], 2, "input_file");                                  }
                            }
                            // 3) PS
                            else if (option == 3)
                            {

                                do
                                {
                                    cout << "\n 1) Premtive Mode \t";
                                    cout << " 2) Non-Premtive Mode \n"
                                         << endl;

                                    cout << "Option> ";

                                    cin >> option;

                                    validate = Validator(option, 1, 2);

                                } while (!validate);

                                if (option == 1)
                                {
                                    premtivePriorityScheduler(head, count, argv[4]);
                                }
                                else if (option == 2)
                                {
                                    calculationController(head, 3);

                                    outputHandler(head, count, argv[4], 3, "input_file");  
                                }
                            }
                            // 4) RRS
                            else if (option == 4)
                            {
                                validate = true;
                                int timeQ;
                                do
                                {
                                    if (validate)
                                    {

                                        cout << "Please enter the Time Quantum \n";
                                    }
                                    cout << "Option> ";
                                    cin >> timeQ;
                                    cout << endl;

                                    validate = timeValidator(timeQ);

                                    if (!validate)
                                    {

                                        cout << "\nPlease enter the valid Time Quantum \n";
                                    }
                                    else
                                    {
                                        RRScontroller(head, timeQ, count, argv[4]);
                                    }

                                } while (!validate);
                            }
                        }

                    } while (!validate);
                }

                // 2)
                else if (option == 2)
                {
                    string result;

                    ifstream myfile(argv[4]);

                    if (myfile.is_open())
                    {
                        while (getline(myfile, result))
                        {
                            cout << result << '\n';
                        }
                        myfile.close();
                    }

                    else
                    {
                        cout << "Unable to open file";
                    }
                }
                // 3)
                else if (option == 3)
                {
                    cout << "Program is terminated sucessfully";
                    break;
                }
            }

            //if the data is valid
            //the validator will send true i will make it false to break the loop
            //if the validator send false i will make it true to loop again

        } while (!validate);
    }

    return 0;
}
