/*
 * Name: Amy Saad
 * File: main.cpp
 *
 * Program: uses a linked list to manage a movie database that
 * stores information of the movie name, year, audience score,
 * and rotten tomato score. It allows the user to interact with
 * the database to search for movies, list movies by top 15 audience
 * and rotten tomato score, find the worst movie, insert a movie, and
 * update an existing movie.
 *
 * class initialDatabase:
 * initialDatabase(string fileName) : constructor
 * dbNode* searchMovieName(const string& movieName) : search by movie name
 * void Top15Audience(): lists top 15 movies by audience score
 * void Top15RottenTomato(): lists top 15 movies by rotten tomato score
 * void worstMovie(): displays the worst movie
 * void insertMovie(): inserts a movie into the database
 * void updateMovie(): updates an existing movie in the database
 *
 */
#include <iostream>
#include <fstream>
#include <iomanip> 
using namespace std;

struct dbNode {
    string movie;
    int year;
    float audience;
    float rotten;
    dbNode *nextAud;
    dbNode *nextRot;
};

class initialDatabase {
private:
    dbNode* head;

    void sortByAudience() {
        dbNode* sorted = nullptr;
        while (head) {
            dbNode* current = head;
            head = head->nextAud;
            if (!sorted || sorted->audience <= current->audience) {
                current->nextAud = sorted;
                sorted = current;
            } else {
                dbNode* temp = sorted;
                while (temp->nextAud && temp->nextAud->audience > current->audience) {
                    temp = temp->nextAud;
                }
                current->nextAud = temp->nextAud;
                temp->nextAud = current;
            }
        }
        head = sorted;
    }

    void sortByRottenTomato() {
        dbNode* sorted = nullptr;
        while (head) {
            dbNode* current = head;
            head = head->nextRot;
            if (!sorted || sorted->rotten <= current->rotten) {
                current->nextRot = sorted;
                sorted = current;
            } else {
                dbNode* temp = sorted;
                while (temp->nextRot && temp->nextRot->rotten > current->rotten) {
                    temp = temp->nextRot;
                }
                current->nextRot = temp->nextRot;
                temp->nextRot = current;
            }
        }
        head = sorted;
    }

public:
    initialDatabase(string fileName) {
        head = nullptr;
        ifstream file(fileName);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                dbNode* newNode = new dbNode;
                file >> newNode->year >> newNode->audience >> newNode->rotten;
                file.get();
                getline(file, newNode->movie);
                newNode->nextAud = nullptr;
                newNode->nextRot = nullptr;
                if (!head || newNode->rotten < head->rotten) {
                    newNode->nextRot = head;
                    head = newNode;
                } else {
                    dbNode* current = head;
                    while (current->nextRot && current->nextRot->rotten < newNode->rotten)
                        current = current->nextRot;
                    newNode->nextRot = current->nextRot;
                    current->nextRot = newNode;
                }
            }
            file.close();
        } else {
            cout << "Hmm.. can't read file." << endl;
        }
    }

    dbNode* searchMovieName(const string& movieName) {
        dbNode* current = head;
        while (current != nullptr) {
            if (current->movie == movieName) {
                return current;
            }
            current = current->nextRot;
        }
        return nullptr;
    }

    void Top15Audience() {
        sortByAudience();
        dbNode* current = head;
        int count = 0;
        cout << "Rank  Movie                      Year   Audience Score   Rotten Tomatoes" << endl;
        cout << "---------------------------------------------------------------------" << endl;
        while (current && count < 15) {
            cout << setw(4) << count + 1 << ".  " << left << setw(25) << current->movie << setw(6) << current->year
                 << setw(17) << fixed << setprecision(1) << current->audience * 10 << setw(19) << fixed << setprecision(0) << current->rotten << "%"
                 << endl;
            current = current->nextAud;
            count++;
        }
    }

    void Top15RottenTomato() {
        sortByRottenTomato();
        dbNode* current = head;
        int count = 0;
        cout << "Rank  Movie                      Year   Audience Score   Rotten Tomatoes" << endl;
        cout << "----------------------------------------------------------------------" << endl;
        while (current && count < 15) {
            cout << setw(5) << count + 1 << left << setw(25) << current->movie << setw(11) << current->year
                 << setw(16) << fixed << setprecision(1) << current->audience * 10 << setw(19) << fixed << setprecision(0) << current->rotten << "%"
                 << endl;
            current = current->nextRot;
            count++;
        }
    }

    void worstMovie() {
        dbNode* current = head;
        dbNode* worst = head;
        while (current) {
            if (current->rotten < worst->rotten) {
                worst = current;
            }
            current = current->nextRot;
        }
        cout << "Worst Movie: " << worst->movie << "   Audience Score: " << worst->audience
             << "   Rotten Tomatoes Score: " << worst->rotten << endl;
    }

    void insertMovie() {
        string movie;
        int year;
        float audience;
        float rotten;
        cout << "Enter the movie title: ";
        cin.ignore();
        getline(cin, movie);
        cout << "Enter the year of the movie: ";
        cin >> year;
        cout << "Enter the audience score: ";
        cin >> audience;
        cout << "Enter the Rotten Tomatoes score: ";
        cin >> rotten;
        dbNode* newNode = new dbNode;
        newNode->movie = movie;
        newNode->year = year;
        newNode->audience = audience;
        newNode->rotten = rotten;
        newNode->nextRot = nullptr;
        if (!head || newNode->rotten < head->rotten) {
            newNode->nextRot = head;
            head = newNode;
        } else {
            dbNode* current = head;
            while (current->nextRot && current->nextRot->rotten < newNode->rotten)
                current = current->nextRot;
            newNode->nextRot = current->nextRot;
            current->nextRot = newNode;
        }
        cout << "Movie inserted successfully!" << endl;
    }

    void updateMovie() {
        string movie;
        cout << "Enter the movie title: ";
        cin.ignore();
        getline(cin, movie);
        dbNode* current = head;
        bool found = false;
        while (current) {
            if (current->movie == movie) {
                cout << "Enter the updated year: ";
                cin >> current->year;
                cout << "Enter the updated audience score: ";
                cin >> current->audience;
                cout << "Enter the updated Rotten Tomatoes score: ";
                cin >> current->rotten;
                found = true;
                break;
            }
            current = current->nextRot;
        }
        if (!found) {
            cout << "Movie not found." << endl;
        }
    }
};

int main() {
    ifstream inFile;
    string fileName;
    char menu;
    string searchTitle;
    cout << "Enter the name of your file: " << endl;
    cin >> fileName;
    initialDatabase database(fileName);
    inFile.open(fileName);
    if (!inFile) {
        cout << "Error - can't read file" << endl;
        return 1;
    } else {
        inFile.close();
    }
    do {
        cout << " " << endl;
        cout << "Menu ~~~~" << endl;
        cout << "m - Search by (m)ovie name" << endl;
        cout << "a - List top 15 movies by (a)udience score" << endl;
        cout << "r - List top 15 movies by (R)otten Tomatoes score" << endl;
        cout << "w - Display (w)orst movie by Rotten Tomatoes score" << endl;
        cout << "i - (I)nsert a new movie into the database" << endl;
        cout << "u - (U)pdate a movie" << endl;
        cout << "q - (Q)uit" << endl;
        cout << " " << endl;
        cout << "Enter a menu option: ";
        cin >> menu;
        if (menu == 'm') {
            cout << "Enter the movie title: " << endl;
            cin.ignore();
            getline(cin, searchTitle);
            dbNode* foundIt = database.searchMovieName(searchTitle);
            if (foundIt != nullptr) {
                cout << "Movie: " << foundIt->movie << ", Year: " << foundIt->year
                     << ", Audience Score: " << foundIt->audience
                     << ", Rotten Tomatoes Score: " << foundIt->rotten << endl;
            } else {
                cout << "Movie not found" << endl;
            }
        } else if (menu == 'a') {
            database.Top15Audience();
        } else if (menu == 'r') {
            database.Top15RottenTomato();
        } else if (menu == 'w') {
            database.worstMovie();
        } else if (menu == 'i') {
            database.insertMovie();
        } else if (menu == 'u') {
            database.updateMovie();
        }
    } while (menu != 'q');
    return 0;
}
