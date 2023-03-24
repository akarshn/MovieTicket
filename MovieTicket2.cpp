#include <bits/stdc++.h>

using namespace std;

// function prototypes
void displayMainMenu();
void displayAdminMenu();
void displayBookingAgentMenu();
void displayGuestMenu();
void addMovie();
void removeMovie();
void addShow();
void removeShow();
void setOccupancy();
void bookTicket();
void cancelTicket();
void displayOccupancy();
void purgeOldData();
void loadDataFromFiles();
void saveDataToFiles();

// data structures for storing data
struct Movie
{
    int id;
    string name;
};

struct Show
{
    int id;
    int movieId;
    int screenNum;
    string startTime;
    int duration;
    int seats[50]; // 1 means seat is available, 0 means it's booked
};

vector<Movie> movies;
unordered_map<int, vector<Show>> shows; // maps screen number to list of shows for that screen
vector<Show> allShows;                  // all shows across all screens
vector<int> bookings;                   // list of seat indices for booked seats across all shows

// constants for file names and record sizes
const string MOVIES_FILE = "Movies.dat";
const string SHOWS_FILE = "Shows.dat";
const string BOOKINGS_FILE = "Bookings.dat";
const int MOVIE_RECORD_SIZE = sizeof(int) + 50 * sizeof(char);
const int SHOW_RECORD_SIZE = 6 * sizeof(int) + 50 * sizeof(int);
const int BOOKING_RECORD_SIZE = sizeof(int);
const int NUM_SCREENS = 3;

// main function
int main()
{
    loadDataFromFiles();
    displayMainMenu();
    saveDataToFiles();
    return 0;
}

// displays main menu and routes to appropriate submenu based on user input
void displayMainMenu()
{
    int choice;
    do
    {
        cout << "\nMain Menu\n";
        cout << "1. Login as Admin\n";
        cout << "2. Login as Booking Agent\n";
        cout << "3. Login as Guest\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            displayAdminMenu();
            break;
        case 2:
            displayBookingAgentMenu();
            break;
        case 3:
            displayGuestMenu();
            break;
        case 4:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 4);
}

void displayAdminMenu()
{
    int choice;
    do
    {
        cout << "\nADMIN MENU\n";
        cout << "1. Add movie\n";
        cout << "2. Remove movie\n";
        cout << "3. Add show\n";
        cout << "4. Remove show\n";
        cout << "5. View occupancy report\n";
        cout << "6. Purge old data\n";
        cout << "7. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addMovie();
            break;
        case 2:
            removeMovie();
            break;
        case 3:
            addShow();
            break;
        case 4:
            removeShow();
            break;
        case 5:
            setOccupancy();
            break;
        case 6:
            purgeOldData();
            break;
        case 7:
            cout << "\nLogging out...\n";
            break;
        default:
            cout << "\nInvalid choice. Please try again.\n";
            break;
        }
    } while (choice != 7);
}

void displayBookingAgentMenu()
{
    int choice;
    do
    {
        cout << "\nBooking Agent Menu\n";
        cout << "1. Book ticket\n";
        cout << "2. Cancel ticket\n";
        cout << "3. Display occupancy report\n";
        cout << "4. Return to main menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            bookTicket();
            break;
        case 2:
            cancelTicket();
            break;
        case 3:
            displayOccupancy();
            break;
        case 4:
            cout << "Returning to main menu...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 4);
}

// displays guest menu and routes to appropriate action based on user input
void displayGuestMenu()
{
    int choice;
    do
    {
        cout << "\nGuest Menu\n";
        cout << "1. Display movies\n";
        cout << "2. Book ticket\n";
        cout << "3. Cancel ticket\n";
        cout << "4. Return to main menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "\nMovies:\n";
            for (const auto &movie : movies)
            {
                cout << movie.id << " - " << movie.name << "\n";
            } // 1 - Avengers
            break;
        case 2:
            bookTicket();
            break;
        case 3:
            cancelTicket();
            break;
        case 4:
            cout << "Returning to main menu...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 4);
}

void addMovie()
{
    int id;
    string name;
    cout << "\nEnter movie id: ";
    cin >> id;
    cout << "Enter movie name: ";
    cin.ignore();
    getline(cin, name);
    movies.push_back({id, name});
    cout << "Movie added successfully.\n";
}

// removes an existing movie from the system
void removeMovie()
{
    int id;
    cout << "\nEnter movie id: ";
    cin >> id;
    auto it = find_if(movies.begin(), movies.end(), [id](const Movie &movie)
                      { return movie.id == id; });
    if (it != movies.end())
    {
        movies.erase(it);
        // remove all shows for this movie
        for (int i = 1; i <= NUM_SCREENS; i++)
        {
            auto &screenShows = shows[i];
            auto showIt = remove_if(screenShows.begin(), screenShows.end(), [id](const Show &show)
                                    { return show.movieId == id; });
            screenShows.erase(showIt, screenShows.end());
        }
        cout << "Movie removed successfully.\n";
    }
    else
    {
        cout << "Movie not found.\n";
    }
}

void addShow()
{
    Show show;
    cout << "\nEnter show details:\n";
    cout << "Movie ID: ";
    cin >> show.movieId;
    cout << "Screen Number (1-" << NUM_SCREENS << "): ";
    cin >> show.screenNum;
    cout << "Start Time (hh:mm): ";
    cin >> show.startTime;
    cout << "Duration (in minutes): ";
    cin >> show.duration;
    for (int i = 0; i < 50; i++)
    {
        show.seats[i] = 1;
    }

    // add show to shows and allShows data structures
    show.id = allShows.size() + 1;
    shows[show.screenNum].push_back(show);
    allShows.push_back(show);

    cout << "\nShow added successfully!\n";
}

void removeShow()
{
    int screenNum, showId;
    cout << "Enter screen number: ";
    cin >> screenNum;
    if (shows.find(screenNum) == shows.end())
    {
        cout << "No shows found for screen " << screenNum << endl;
        return;
    }
    cout << "Shows for screen " << screenNum << ":" << endl;
    vector<Show> &screenShows = shows[screenNum];
    for (const auto &show : screenShows)
    {
        cout << show.id << " - " << show.startTime << endl;
    }
    cout << "Enter show id: ";
    cin >> showId;
    auto it = find_if(screenShows.begin(), screenShows.end(),
                      [=](const Show &s)
                      { return s.id == showId; });
    if (it != screenShows.end())
    {
        // remove booked seats from the global list
        for (int i = 0; i < 50; i++)
        {
            if (it->seats[i] == 0)
            {
                auto bookIt = find(bookings.begin(), bookings.end(), screenNum * 100 + showId * 10 + i);
                if (bookIt != bookings.end())
                {
                    bookings.erase(bookIt);
                }
            }
        }
        // remove show from the screen and allShows vectors
        allShows.erase(remove(allShows.begin(), allShows.end(), *it), allShows.end());
        screenShows.erase(it);
        cout << "Show removed successfully." << endl;
    }
    else
    {
        cout << "No show found with id " << showId << endl;
    }
}

void setOccupancy()
{
    int screenNum, showId;
    cout << "\nEnter screen number: ";
    cin >> screenNum;
    cout << "Enter show id: ";
    cin >> showId;

    // Find the show with the given screen number and show id
    auto screenShows = shows.find(screenNum);
    if (screenShows == shows.end())
    {
        cout << "\nNo shows found for screen " << screenNum << ".\n";
        return;
    }

    auto show = find_if(screenShows->second.begin(), screenShows->second.end(), [&](const Show &s)
                        { return s.id == showId; });
    if (show == screenShows->second.end())
    {
        cout << "\nNo show found with id " << showId << " for screen " << screenNum << ".\n";
        return;
    }

    // Set the occupancy for the show
    int numSeats = sizeof(show->seats) / sizeof(show->seats[0]);
    int occupancy;
    do
    {
        cout << "\nEnter new occupancy percentage (0-100): ";
        cin >> occupancy;
        if (occupancy < 0 || occupancy > 100)
        {
            cout << "\nInvalid occupancy percentage. Please try again.\n";
        }
    } while (occupancy < 0 || occupancy > 100);

    int numOccupiedSeats = static_cast<int>(round(occupancy / 100.0 * numSeats));
    fill_n(show->seats, numOccupiedSeats, 0);
    fill_n(show->seats + numOccupiedSeats, numSeats - numOccupiedSeats, 1);

    cout << "\nOccupancy set for show " << showId << " on screen " << screenNum << ".\n";
}

void bookTicket()
{
    int showId, seatNum;
    cout << "\nEnter show id: ";
    cin >> showId;

    // check if show id is valid
    auto it = find_if(allShows.begin(), allShows.end(), [=](const Show &s)
                      { return s.id == showId; });
    if (it == allShows.end())
    {
        cout << "Invalid show id. Please try again.\n";
        return;
    }

    Show &show = *it;
    if (show.duration <= 0)
    {
        cout << "This show has already ended. Please choose another show.\n";
        return;
    }

    cout << "\nEnter seat number (1-50): ";
    cin >> seatNum;

    // check if seat number is valid
    if (seatNum < 1 || seatNum > 50)
    {
        cout << "Invalid seat number. Please try again.\n";
        return;
    }

    // check if seat is available
    if (show.seats[seatNum - 1] == 0)
    {
        cout << "Seat " << seatNum << " is already booked. Please choose another seat.\n";
        return;
    }

    // book the ticket
    show.seats[seatNum - 1] = 0; // mark seat as booked
    int bookingId = bookings.size();
    bookings.push_back(show.id * 100 + seatNum); // encode booking information

    cout << "Ticket booked successfully. Booking id is " << bookingId << ".\n";
}

void cancelTicket()
{
    int showId, seatNum;
    cout << "\nEnter show id: ";
    cin >> showId;
    if (shows.find(showId) == shows.end())
    {
        cout << "Invalid show ID. Please try again.\n";
        return;
    }
    cout << "Enter seat number: ";
    cin >> seatNum;
    if (seatNum < 1 || seatNum > 50)
    {
        cout << "Invalid seat number. Please try again.\n";
        return;
    }
    Show &show = allShows[showId];
    if (show.seats[seatNum - 1] == 1)
    {
        cout << "Seat " << seatNum << " is not booked. Please try again.\n";
        return;
    }
    show.seats[seatNum - 1] = 1;
    bookings.erase(remove(bookings.begin(), bookings.end(), showId * 50 + seatNum - 1), bookings.end());
    cout << "Ticket cancelled successfully.\n";
    saveDataToFiles();
}

void displayOccupancy()
{
    cout << "\nOccupancy Report\n";
    for (int i = 1; i <= NUM_SCREENS; i++)
    {
        cout << "Screen " << i << ":\n";
        for (const auto &show : shows[i])
        {
            cout << "Show ID: " << show.id << "\n";
            cout << "Movie Name: " << movies[show.movieId - 1].name << "\n";
            cout << "Start Time: " << show.startTime << "\n";
            cout << "Seats: ";
            int numAvailable = 0;
            for (int j = 0; j < 50; j++)
            {
                if (show.seats[j] == 1)
                {
                    cout << j + 1 << " ";
                    numAvailable++;
                }
            }
            cout << "\nTotal Seats: 50\n";
            cout << "Available Seats: " << numAvailable << "\n";
            cout << "Booked Seats: " << 50 - numAvailable << "\n\n";
        }
    }
}

int parseTime(const std::string &timeStr)
{
    stringstream ss(timeStr);
    int hours, minutes;
    char colon;
    if (ss >> hours >> colon >> minutes && colon == ':')
    {
        return hours * 60 + minutes;
    }
    else
    {
        throw invalid_argument("Invalid time format");
    }
}

void purgeOldData()
{
    time_t now = time(nullptr);

    for (auto it = allShows.begin(); it != allShows.end();)
    {
        time_t start = parseTime((*it).startTime);
        time_t end = start + (*it).duration * 60;

        if (now > end)
        {
            for (int i = 0; i < 50; i++)
            {
                if ((*it).seats[i] == 0)
                {
                    bookings.erase(remove(bookings.begin(), bookings.end(), (*it).id * 50 + i), bookings.end());
                }
            }

            shows[(*it).screenNum].erase(remove(shows[(*it).screenNum].begin(), shows[(*it).screenNum].end(), *it), shows[(*it).screenNum].end());
            it = allShows.erase(it);
        }
        else
        {
            ++it;
        }
    }

    cout << "\nOld shows have been purged from the system.\n";
}

void saveDataToFiles()
{
    // Save movies to file
    ofstream moviesFile(MOVIES_FILE, ios::binary);
    for (const auto &movie : movies)
    {
        moviesFile.write(reinterpret_cast<const char *>(&movie.id), sizeof(int));
        moviesFile.write(movie.name.c_str(), 50);
    }
    moviesFile.close();

    // Save shows to file
    ofstream showsFile(SHOWS_FILE, ios::binary);
    for (const auto &screenShows : shows)
    {
        for (const auto &show : screenShows.second)
        {
            showsFile.write(reinterpret_cast<const char *>(&show.id), sizeof(int));
            showsFile.write(reinterpret_cast<const char *>(&show.movieId), sizeof(int));
            showsFile.write(reinterpret_cast<const char *>(&show.screenNum), sizeof(int));
            showsFile.write(show.startTime.c_str(), 50);
            showsFile.write(reinterpret_cast<const char *>(&show.duration), sizeof(int));
            showsFile.write(reinterpret_cast<const char *>(show.seats), 50 * sizeof(int));
        }
    }
    showsFile.close();

    // Save bookings to file
    ofstream bookingsFile(BOOKINGS_FILE, ios::binary);
    for (const auto &seat : bookings)
    {
        bookingsFile.write(reinterpret_cast<const char *>(&seat), sizeof(int));
    }
    bookingsFile.close();
}

void loadDataFromFiles()
{
    // load movies from file
    ifstream moviesFile(MOVIES_FILE, ios::binary);
    if (moviesFile)
    {
        int id;
        char name[50];
        while (moviesFile.read(reinterpret_cast<char *>(&id), sizeof(int)) &&
               moviesFile.read(name, sizeof(name)))
        {
            movies.push_back({id, name});
        }
        moviesFile.close();
    }

    // load shows from file
    ifstream showsFile(SHOWS_FILE, ios::binary);
    if (showsFile)
    {
        int id, movieId, screenNum, duration;
        char startTime[50];
        int seats[50];
        while (showsFile.read(reinterpret_cast<char *>(&id), sizeof(int)) &&
               showsFile.read(reinterpret_cast<char *>(&movieId), sizeof(int)) &&
               showsFile.read(reinterpret_cast<char *>(&screenNum), sizeof(int)) &&
               showsFile.read(startTime, sizeof(startTime)) &&
               showsFile.read(reinterpret_cast<char *>(&duration), sizeof(int)) &&
               showsFile.read(reinterpret_cast<char *>(seats), sizeof(seats)))
        {
            Show show = {id, movieId, screenNum, startTime, duration};
            for (int i = 0; i < 50; i++)
            {
                show.seats[i] = seats[i];
            }
            allShows.push_back(show);
            shows[screenNum].push_back(show);
        }
        showsFile.close();
    }

    // load bookings from file
    ifstream bookingsFile(BOOKINGS_FILE, ios::binary);
    if (bookingsFile)
    {
        int seatIndex;
        while (bookingsFile.read(reinterpret_cast<char *>(&seatIndex), sizeof(int)))
        {
            bookings.push_back(seatIndex);
        }
        bookingsFile.close();
    }
}

