#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <set> // For BST (optional, can be custom BST)
#include <algorithm> // For sorting & searching
#include <iomanip> // For setw
#include <chrono> // For time
#include <ctime> // For time
#include <limits> // For numeric_limits

using namespace std; // Using namespace std to avoid writing std::

// ===============================================
//           STRUCTURES AND CLASSES (DEFINED FIRST)
// ===============================================

// Data for Dosen (newly added)
struct Dosen {
    string id;
    string nama;
    string mata_kuliah;
};

// Data for Books
struct Buku {
    string id;
    string judul;
    string dosen_referensi;
    bool tersedia;

    // For sorting and searching (required by std::set)
    bool operator<(const Buku& other) const {
        return judul < other.judul;
    }
};

// Data for Consultation
struct Konsultasi {
    int no_urut; // Sequential number for display in queue
    string nama_mahasiswa;
    string nim_mahasiswa; // Add NIM for unique identification
    string tanggal_daftar;
    string nama_dosen;
};

// 1. Linked List - For Loan History and Activity
// Node for Linked List
template <typename T>
struct Node {
    T data;
    Node* next;
    Node(T val) : data(val), next(nullptr) {}
};

// Simple Linked List Class
template <typename T>
class LinkedList {
private:
    Node<T>* head;
public:
    LinkedList() : head(nullptr) {}

    // Destructor to free memory
    ~LinkedList() {
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
    }

    void add(T val) {
        Node<T>* newNode = new Node<T>(val);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node<T>* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    Node<T>* getHead() const {
        return head;
    }

    bool isEmpty() const {
        return head == nullptr;
    }
};

// Data for Student
struct Mahasiswa {
    string nim;
    string password;
    string nama;
    LinkedList<pair<string, string>> riwayat_peminjaman; // <Date, Book Title>
    LinkedList<pair<string, string>> riwayat_aktivitas; // <Date, Activity Description>
    bool sudah_daftar_konsultasi; // Indicates if student has registered for consultation
    string dosen_konsultasi_terdaftar; // Stores the mentor's name if registered
    
    // Default constructor
    Mahasiswa() : sudah_daftar_konsultasi(false), dosen_konsultasi_terdaftar("") {}

    // Parameterized constructor for initial student setup
    Mahasiswa(string n, string p, string nm)
        : nim(n), password(p), nama(nm),
          sudah_daftar_konsultasi(false), 
          dosen_konsultasi_terdaftar("") {}
};


// ===============================================
//           GLOBAL DATA AND POINTERS (DECLARED AFTER STRUCTS/CLASSES)
// ===============================================

// Global constants
const string KAMPUS_CAKRAWALA = "Universitas Cakrawala";

// Global container for Dosen data
map<string, Dosen> dataDosen; // Key: ID Dosen, Value: Dosen Object

// 2. Queue - For Academic Consultation Queue (Two separate queues per mentor)
queue<Konsultasi> antreanIdrusMadani;
queue<Konsultasi> antreanDwiWulan;

// 4. Hash Table - For Student Login (using map as simulation)
map<string, Mahasiswa> dataMahasiswa; // Key: NIM, Value: Mahasiswa Object

// 5. Tree (BST) - For Library Book List (using set as BST simulation)
set<Buku> daftarBuku; // Will be automatically sorted based on operator< in Buku struct

// 6. Graph - For Campus Navigation (simple static routes)
map<string, vector<string>> ruteKampus; // Key: Station Name, Value: Vector of route steps

// Global variable to store current logged-in user
Mahasiswa* currentUser = nullptr;


// ===============================================
//           FORWARD DECLARATIONS OF FUNCTIONS
// ===============================================

// Utility functions
void clearScreen();
string getCurrentDate();
void pressEnterToContinue();
void displayHeader();

// UI functions (formerly in ConsoleUI)
void displayLoginMenu();
void displayMainMenu();
void KonsultasiAkademikMenu();
void PerpustakaanMenu();
void RiwayatPeminjamanMenu();
void RiwayatAktivitasMenu();
void NavigasiKampusMenu();

// Service-related helper functions (formerly in Services/*.h)
void initializeData(); // Formerly in DataInitializer
void displayBooksTable(const set<Buku>& bukuList); // Formerly in LibraryService
void borrowBook(const string& bookId, const string& dosenRujukan); // Formerly in LibraryService
set<Buku> searchBooks(const string& keyword, const string& dosenRujukan); // Formerly in LibraryService
set<Buku> filterAvailableBooks(const string& dosenRujukan); // Formerly in LibraryService

// New/modified consultation service functions
void registerConsultationLogic(string idDosen); // Simplified logic after ID is validated
void displayDetailedConsultationQueue(const string& nim_mahasiswa); // Displays table and user's status

void displayRoute(const string& stationName); // Formerly in CampusNavigationService

// ===============================================
//           UTILITY FUNCTIONS
// ===============================================

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string getCurrentDate() {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm* ltm = localtime(&now_c);

    string year = to_string(1900 + ltm->tm_year);
    string month = to_string(1 + ltm->tm_mon);
    string day = to_string(ltm->tm_mday);

    if (month.length() == 1) month = "0" + month;
    if (day.length() == 1) day = "0" + day;

    return day + "-" + month + "-" + year;
}

void pressEnterToContinue() {
    cout << "   Silahkan tekan 'Enter' untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer before getting new input
    cin.get();
}

void displayHeader() {
    cout << "============================================================================================\n";
    cout << "  _____ ___ ___   ____  ____  ______   _____ ______  __ __  ___      ___  ____   ______ \n";
    cout << " / ___/|   |   | /    ||    \\|      | / ___/|      ||  |  ||   \\    /  _]|    \\ |      |\n";
    cout << "(   \\_ | _   _ ||  o  ||  D  )      |(   \\_ |      ||  |  ||    \\  /  [_ |  _  ||      |\n";
    cout << " \\__  ||  \\_/  ||     ||    /|_|  |_| \\__  ||_|  |_||  |  ||  D  ||    _]|  |  ||_|  |_|\n";
    cout << " /  \\ ||   |   ||  _  ||    \\  |  | __/  \\ |  |  |  |  :  ||     ||   [_ |  |  |  |  |  \n";
    cout << " \\    ||   |   ||  |  ||  .  \\ |  ||  \\    |  |  |  |     ||     ||     ||  |  |  |  |  \n";
    cout << "  \\___||___|___||__|__||__|\\_| |__||__|\\___|  |__|   \\__,_||_____||_____||__|__|  |__|  \n";
    cout << "\n";
    cout << "                                     CAKRAWALA UNIVERSITY                                    \n";
    cout << "============================================================================================\n";
    cout << "\n";
}

// ===============================================
//           DATA INITIALIZATION FUNCTION
// ===============================================

void initializeData() {
    // Data Mahasiswa (Hash Table)
    dataMahasiswa["217403"] = Mahasiswa("217403", "15071999", "Aulia Putri");
    dataMahasiswa["532618"] = Mahasiswa("532618", "03082002", "Rafi Akbar");
    dataMahasiswa["894205"] = Mahasiswa("894205", "22121997", "Laras Wulan");
    dataMahasiswa["670349"] = Mahasiswa("670349", "09052000", "Dito Pratama");
    dataMahasiswa["356127"] = Mahasiswa("356127", "17111998", "Sinta Devi");

    // Data Dosen (newly added)
    dataDosen["DSN1"] = {"DSN1", "Idrus Madani", "Data Structures"};
    dataDosen["DSN2"] = {"DSN2", "Dwi Wulan", "Algoritma dan Pemrograman"};

    // Book Data (BST represented by std::set)
    daftarBuku.insert({"A1B2", "Pengantar Python untuk Pemula", dataDosen["DSN1"].nama, true});
    daftarBuku.insert({"C3D4", "Desain Web Responsif dengan HTML & CSS", dataDosen["DSN1"].nama, true});
    daftarBuku.insert({"E5F6", "Struktur Data dan Algoritma dalam Java", dataDosen["DSN1"].nama, true});
    daftarBuku.insert({"G7H8", "Prinsip Desain UI/UX Modern", dataDosen["DSN1"].nama, true});
    daftarBuku.insert({"I9J0", "Pengembangan Aplikasi Android dengan Kotlin", dataDosen["DSN1"].nama, true});

    daftarBuku.insert({"K1L2", "Seni Desain Grafis Digital", dataDosen["DSN2"].nama, true});
    daftarBuku.insert({"M3N4", "Basis Data SQL untuk Pengembang", dataDosen["DSN2"].nama, true});
    daftarBuku.insert({"O5P6", "Desain Interaksi Pengguna", dataDosen["DSN2"].nama, true});
    daftarBuku.insert({"Q7R8", "Pengantar Machine Learning dengan Python", dataDosen["DSN2"].nama, true});
    daftarBuku.insert({"S9T0", "Tipografi dalam Desain Grafis", dataDosen["DSN2"].nama, true});

    // Campus Navigation Routes (Static Routes)
    ruteKampus["Tebet"] = {
        "Tebet",
        "Jl. Raya Kasablanka",
        "Jl. H. Rasuna Said",
        "Jl. Mampang Prapatan",
        "Jl. Kemang Utara",
        "Jl. Kemang Timur (" + KAMPUS_CAKRAWALA + ")"
    };
    ruteKampus["Blok M"] = {
        "Blok M",
        "Jl. Melawai Raya",
        "Jl. Prapanca Raya",
        "Jl. Kemang Raya",
        "Jl. Kemang Selatan",
        "Jl. Kemang Timur (" + KAMPUS_CAKRAWALA + ")"
    };
    ruteKampus["Lb. Bulus"] = {
        "Lebak Bulus",
        "Jl. RS. Fatmawati Raya",
        "Jl. TB Simatupang",
        "Jl. Ampera Raya",
        "Jl. Kemang Timur (" + KAMPUS_CAKRAWALA + ")"
    };
    ruteKampus["Tanjung Barat"] = {
        "Tanjung Barat",
        "Jl. TB Simatupang",
        "Jl. Ampera Raya",
        "Jl. Kemang Timur (" + KAMPUS_CAKRAWALA + ")"
    };
    ruteKampus["Taman Mini"] = {
        "Taman Mini",
        "Jl. Keong Mas",
        "Jl. Raya Mabes Hankam",
        "Jl. TB Simatupang",
        "Jl. Ampera Raya",
        "Jl. Kemang Timur (" + KAMPUS_CAKRAWALA + ")"
    };
}

// ===============================================
//           AUTHENTICATION SERVICE FUNCTIONS
// ===============================================

void loginUser(string nim, string password) {
    auto it = dataMahasiswa.find(nim);
    if (it != dataMahasiswa.end() && it->second.password == password) {
        currentUser = &(it->second);
    } else {
        currentUser = nullptr; // Ensure currentUser is null on failed login
    }
}

// ===============================================
//           CONSULTATION SERVICE FUNCTIONS
// ===============================================

// Helper function to display a specific queue table
void displayQueueTableInternal(queue<Konsultasi> q_copy) { // Pass by value to work on a copy
    cout << string(92, '-') << endl;
    cout << left << setw(5) << "No."
              << setw(20) << "Nama Mahasiswa"
              << setw(20) << "Tanggal Daftar"
              << setw(30) << "Nama Dosen" << endl;
    cout << string(92, '-') << endl;

    int no_display = 1;
    while (!q_copy.empty()) {
        Konsultasi k = q_copy.front();
        cout << left << setw(5) << no_display++
                  << setw(20) << k.nama_mahasiswa
                  << setw(20) << k.tanggal_daftar
                  << setw(30) << k.nama_dosen << endl;
        q_copy.pop();
    }
    cout << endl;
}

// Function to register consultation based on Dosen ID
void registerConsultationLogic(string idDosen) {
    if (dataDosen.count(idDosen)) {
        string namaDosen = dataDosen[idDosen].nama;
        Konsultasi newKonsultasi;
        newKonsultasi.nama_mahasiswa = currentUser->nama;
        newKonsultasi.nim_mahasiswa = currentUser->nim;
        newKonsultasi.tanggal_daftar = getCurrentDate();
        newKonsultasi.nama_dosen = namaDosen;

        if (namaDosen == dataDosen["DSN1"].nama) { // "Idrus Madani"
            antreanIdrusMadani.push(newKonsultasi);
        } else if (namaDosen == dataDosen["DSN2"].nama) { // "Dwi Wulan"
            antreanDwiWulan.push(newKonsultasi);
        }

        currentUser->riwayat_aktivitas.add({getCurrentDate(), "Mendaftar konsultasi akademik dengan " + namaDosen});
        currentUser->sudah_daftar_konsultasi = true;
        currentUser->dosen_konsultasi_terdaftar = namaDosen;
        cout << "\n   " << currentUser->nama << " telah berhasil mendaftarkan konsultasi dengan " << namaDosen << ".\n";
    } else {
        cout << "\n   ID Dosen tidak ditemukan. Pendaftaran gagal.\n";
    }
    pressEnterToContinue();
}

// Displays queue for the user's chosen mentor or general message if not registered
void displayDetailedConsultationQueue(const string& nim_mahasiswa) {
    bool userInQueue = false;
    int userQueueNumber = 0;
    string dosenTerdaftar = "";

    // Check Idrus Madani's queue
    queue<Konsultasi> tempQueueIdrus = antreanIdrusMadani; // Make a copy
    int currentQueuePos = 1;
    while (!tempQueueIdrus.empty()) {
        if (tempQueueIdrus.front().nim_mahasiswa == nim_mahasiswa) {
            userInQueue = true;
            userQueueNumber = currentQueuePos;
            dosenTerdaftar = dataDosen["DSN1"].nama; // Use dataDosen map for consistency
            break;
        }
        tempQueueIdrus.pop();
        currentQueuePos++;
    }

    // If not found, check Dwi Wulan's queue
    if (!userInQueue) {
        queue<Konsultasi> tempQueueDwi = antreanDwiWulan; // Make a copy
        currentQueuePos = 1; 
        while (!tempQueueDwi.empty()) {
            if (tempQueueDwi.front().nim_mahasiswa == nim_mahasiswa) {
                userInQueue = true;
                userQueueNumber = currentQueuePos;
                dosenTerdaftar = dataDosen["DSN2"].nama; // Use dataDosen map for consistency
                break;
            }
            tempQueueDwi.pop();
            currentQueuePos++;
        }
    }

    // Update status in currentUser if found in queue but not set
    if (userInQueue && !currentUser->sudah_daftar_konsultasi) {
        currentUser->sudah_daftar_konsultasi = true;
        currentUser->dosen_konsultasi_terdaftar = dosenTerdaftar;
    }

    if (userInQueue) {
        if (dosenTerdaftar == dataDosen["DSN1"].nama) {
            displayQueueTableInternal(antreanIdrusMadani); 
        } else if (dosenTerdaftar == dataDosen["DSN2"].nama) {
            displayQueueTableInternal(antreanDwiWulan); 
        }

        cout << "   Anda terdaftar dalam antrean : [" << userQueueNumber << "]\n";
        cout << "   Konsultasi dengan " << dosenTerdaftar << ".\n";
        cout << "\n";
        // pressEnterToContinue() will be called by ConsoleUI after this function returns
    } else {
        // This message is only shown if the user is NOT in queue and no other queue is displayed
        // No table here, as per new requirement: "Belum ada antrean yang terdaftar." is handled in ConsoleUI if option 2 is chosen.
    }
}

// ===============================================
//           LIBRARY SERVICE FUNCTIONS
// ===============================================

void displayBooksTable(const set<Buku>& bukuList) {
    cout << string(92, '-') << endl;
    cout << left << setw(10) << "ID"
              << setw(50) << "Judul Buku"
              << setw(15) << "Status" << endl;
    cout << string(92, '-') << endl;
    if (bukuList.empty()) {
        cout << "Tidak ada buku untuk ditampilkan.\n";
    } else {
        for (const auto& buku : bukuList) {
            cout << left << setw(10) << buku.id
                      << setw(50) << buku.judul
                      << setw(15) << (buku.tersedia ? "Tersedia" : "Tidak Tersedia") << endl;
        }
    }
    cout << endl;
}

void borrowBook(const string& bookId, const string& dosenRujukan) {
    bool foundBook = false;
    Buku tempBook; 

    for (auto it = daftarBuku.begin(); it != daftarBuku.end(); ++it) {
        if (it->id == bookId && it->dosen_referensi == dosenRujukan) {
            if (it->tersedia) {
                tempBook = *it; 
                daftarBuku.erase(it); 
                tempBook.tersedia = false; 
                daftarBuku.insert(tempBook); 
                
                currentUser->riwayat_peminjaman.add({getCurrentDate(), tempBook.judul});
                currentUser->riwayat_aktivitas.add({getCurrentDate(), "Meminjam buku dengan judul \"" + tempBook.judul + "\""});

                cout << "\n   Buku \"" << tempBook.judul << "\" telah berhasil dipinjam.\n";
                foundBook = true;
            } else {
                cout << "\n   Buku \"" << it->judul << "\" tidak tersedia (sudah dipinjam).\n";
                foundBook = true;
            }
            break;
        }
    }
    if (!foundBook) {
        cout << "\nID Buku tidak ditemukan atau bukan rujukan dosen ini.\n";
    }
    pressEnterToContinue();
}

set<Buku> searchBooks(const string& keyword, const string& dosenRujukan) {
    set<Buku> searchResults;
    for (const auto& buku : daftarBuku) {
        if (buku.dosen_referensi == dosenRujukan) {
            string judulLower = buku.judul;
            string keywordLower = keyword;
            transform(judulLower.begin(), judulLower.end(), judulLower.begin(), ::tolower);
            transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);

            if (judulLower.find(keywordLower) != string::npos) {
                searchResults.insert(buku);
            }
        }
    }
    return searchResults;
}

set<Buku> filterAvailableBooks(const string& dosenRujukan) {
    set<Buku> filteredResults;
    for (const auto& buku : daftarBuku) {
        if (buku.tersedia && buku.dosen_referensi == dosenRujukan) {
            filteredResults.insert(buku);
        }
    }
    return filteredResults;
}

// ===============================================
//           CAMPUS NAVIGATION SERVICE FUNCTIONS
// ===============================================

void displayRoute(const string& stationName) {
    if (ruteKampus.count(stationName)) { // Check if station exists in map
        clearScreen();
        displayHeader();
        cout << "   " << stationName << " ke " << KAMPUS_CAKRAWALA << "\n\n";
        
        int step = 1;
        for (const string& jalur : ruteKampus[stationName]) {
            cout << "   " << step++ << ". " << jalur << endl;
        }
        cout << endl;
    } else {
        cout << "\n   Rute dari stasiun tersebut belum tersedia.\n";
    }
    pressEnterToContinue();
}

// ===============================================
//           CONSOLE UI FUNCTIONS
// ===============================================

void displayLoginMenu() {
    string nim, password;
    bool loggedIn = false;
    string errorMessage = "";

    while (!loggedIn) {
        clearScreen();
        displayHeader();

        if (!errorMessage.empty()) {
            cout << "   !!!" << errorMessage << "\n\n";
            errorMessage = "";
        }
        
        cout << "   LOGIN\n\n";
        cout << "   > Username (NIM) : ";
        cin >> nim;
        cout << "   > Password       : ";
        cin >> password;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

        // Call login function
        bool loginSuccess = false;
        auto it = dataMahasiswa.find(nim);
        if (it != dataMahasiswa.end() && it->second.password == password) {
            currentUser = &(it->second);
            loginSuccess = true;
        } else {
            currentUser = nullptr; // Ensure currentUser is null on failed login
        }

        if (loginSuccess) {
            loggedIn = true;
            cout << "\n   Login berhasil! Selamat datang, " << currentUser->nama << "!\n";
            pressEnterToContinue();
        } else {
            errorMessage = "Username / Password yang anda masukkan salah."; // Set error message
        }
    }
}

void displayMainMenu() {
    int choice;
    do {
        clearScreen();
        displayHeader();
        cout << "   Selamat datang di portal Smart.Student, " << currentUser->nama << "\n\n";
        cout << "      1. > Konsultasi Akademik\n";
        cout << "      2. > Perpustakaan\n";
        cout << "      3. > Riwayat Peminjaman\n";
        cout << "      4. > Riwayat Aktivitas\n";
        cout << "      5. > Navigasi Kampus\n";
        cout << "      6. > Logout\n";
        cout << "\n   Pilih Menu (1 - 6) = ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: KonsultasiAkademikMenu(); break;
            case 2: 
                if (!currentUser->sudah_daftar_konsultasi) {
                    clearScreen();
                    displayHeader();
                    cout << "   !!! Anda harus memilih dosen untuk konsultasi akademik terlebih dahulu.\n\n";
                    pressEnterToContinue();
                } else {
                    PerpustakaanMenu(); 
                }
                break;
            case 3: RiwayatPeminjamanMenu(); break;
            case 4: RiwayatAktivitasMenu(); break;
            case 5: NavigasiKampusMenu(); break;
            case 6:
                cout << "\n" << "   " << currentUser->nama << " berhasil logout.\n";
                pressEnterToContinue();
                currentUser = nullptr; // Reset current user
                break;
            default:
                cout << "\nPilihan tidak valid. Silakan coba lagi.\n";
                pressEnterToContinue();
                break;
        }
    } while (currentUser != nullptr);
}

void KonsultasiAkademikMenu() {
    clearScreen();
    displayHeader();
    cout << "   Antrean Konsultasi Akademik\n\n";

    // Check user's current consultation status and display detailed queue if applicable
    // This call will set currentUser->sudah_daftar_konsultasi and dosen_konsultasi_terdaftar
    displayDetailedConsultationQueue(currentUser->nim); 

    if (currentUser->sudah_daftar_konsultasi) {
        // Message and press enter are handled inside displayDetailedConsultationQueue
        pressEnterToContinue(); // Ensure the "Press Enter to continue" is shown after the queue info
        return; 
    }

    // Display available mentors table ONLY if student is NOT already registered
    cout << string(92, '-') << endl;
    cout << left << setw(15) << "ID Dosen"
              << setw(25) << "Nama Dosen"
              << setw(40) << "Mata Kuliah" << endl;
    cout << string(92, '-') << endl;
    for (const auto& pair : dataDosen) { // Iterate through dataDosen map
        cout << left << setw(15) << pair.second.id
                  << setw(25) << pair.second.nama
                  << setw(40) << pair.second.mata_kuliah << endl;
    }
    cout << endl;

    // Menu options for new registration
    int choice;
    cout << "      1. > Daftarkan Konsultasi\n";
    cout << "      2. > Lihat Antrean\n"; // This option now just calls the display function again
    cout << "      3. > Kembali\n";
    cout << "\n   Pilih Menu (1 - 3) = ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1) { // Register Consultation
        string idDosenInput;
        cout << "\n   Masukkan ID Dosen yang dipilih untuk konsultasi: ";
        cin >> idDosenInput;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        registerConsultationLogic(idDosenInput); // Calls the service logic
    } else if (choice == 2) { // View Queue
        clearScreen();
        displayHeader();
        cout << "   Lihat Antrean Konsultasi Akademik\n\n";
        displayDetailedConsultationQueue(currentUser->nim); // Re-use the existing logic to show user's queue
        pressEnterToContinue(); // Press enter after viewing the queue
    } else if (choice == 3) {
        // Return to main menu
    } else {
        cout << "\nPilihan tidak valid. Silakan coba lagi.\n";
        pressEnterToContinue();
    }
}

void PerpustakaanMenu() {
    string dosenRujukan = currentUser->dosen_konsultasi_terdaftar;
    set<Buku> currentDisplayBuku; 

    // Initialize currentDisplayBuku with all books from the relevant mentor on entry
    for (const auto& buku : daftarBuku) {
        if (buku.dosen_referensi == dosenRujukan) {
            currentDisplayBuku.insert(buku);
        }
    }
    
    do { 
        clearScreen();
        displayHeader();
        cout << "   Daftar buku perpustakaan rujukan (" << dosenRujukan << ")\n\n";

        displayBooksTable(currentDisplayBuku);

        cout << "      1. > Pinjam Buku\n";
        cout << "      2. > Cari Buku\n";
        cout << "      3. > Filter Buku (Tersedia)\n";
        cout << "      4. > Reset Filter\n"; 
        cout << "      5. > Kembali ke Menu Utama\n"; 
        cout << "\n   Pilih Menu (1 - 5) = ";
        int subChoice;
        cin >> subChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string inputId;
        string keyword;

        switch (subChoice) {
            case 1: { // Pinjam Buku
                cout << "   Masukkan ID Buku yang dipilih : ";
                cin >> inputId;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                borrowBook(inputId, dosenRujukan); 
                
                // Refresh display after action: reset currentDisplayBuku to all books for this mentor
                currentDisplayBuku.clear();
                for (const auto& buku : daftarBuku) {
                    if (buku.dosen_referensi == dosenRujukan) {
                        currentDisplayBuku.insert(buku);
                    }
                }
                break;
            }
            case 2: { // Cari Buku
                cout << "   Masukkan keyword buku: ";
                getline(cin, keyword);
                currentDisplayBuku = searchBooks(keyword, dosenRujukan); 
                break;
            }
            case 3: { // Filter Buku (Tersedia)
                currentDisplayBuku = filterAvailableBooks(dosenRujukan); 
                break;
            }
            case 4: { // Reset Filter
                currentDisplayBuku.clear();
                for (const auto& buku : daftarBuku) {
                    if (buku.dosen_referensi == dosenRujukan) {
                        currentDisplayBuku.insert(buku);
                    }
                }
                break;
            }
            case 5: // Kembali ke Menu Utama
                return; 
            default:
                cout << "\nPilihan tidak valid. Silakan coba lagi.\n";
                pressEnterToContinue();
                // On invalid input, reset display to all books for this mentor
                currentDisplayBuku.clear();
                for (const auto& buku : daftarBuku) {
                    if (buku.dosen_referensi == dosenRujukan) {
                        currentDisplayBuku.insert(buku);
                    }
                }
                break;
        }
    } while (true);
}

void RiwayatPeminjamanMenu() {
    clearScreen();
    displayHeader();
    cout << "   Riwayat peminjaman buku " << currentUser->nama << "\n\n";

    if (currentUser->riwayat_peminjaman.isEmpty()) {
        cout << "   !!! Belum meminjam buku apapun.\n";
    } else {
        cout << string(92, '-') << endl;
        cout << left << setw(20) << "Tanggal"
                  << setw(50) << "Judul Buku" << endl;
        cout << string(92, '-') << endl;

        Node<pair<string, string>>* current = currentUser->riwayat_peminjaman.getHead();
        while (current != nullptr) {
            cout << left << setw(20) << current->data.first
                      << setw(50) << current->data.second << endl;
            current = current->next;
        }
    }
    cout << endl;
    pressEnterToContinue();
}

void RiwayatAktivitasMenu() {
    clearScreen();
    displayHeader();
    cout << "   Riwayat aktivitas " << currentUser->nama << "\n\n";

    if (currentUser->riwayat_aktivitas.isEmpty()) {
        cout << "   Belum melakukan aktivitas apapun.\n";
    } else {
        cout << string(92, '-') << endl;
        cout << left << setw(20) << "Tanggal"
                  << setw(60) << "Riwayat Aktivitas" << endl;
        cout << string(92, '-') << endl;

        Node<pair<string, string>>* current = currentUser->riwayat_aktivitas.getHead();
        while (current != nullptr) {
            cout << left << setw(20) << current->data.first
                      << setw(60) << current->data.second << endl;
            current = current->next;
        }
    }
    cout << endl;
    pressEnterToContinue();
}

void NavigasiKampusMenu() {
    clearScreen();
    displayHeader();
    cout << "   Navigasi Kampus\n\n";

    cout << "      1. > Stasiun Tebet\n";
    cout << "      2. > Stasiun Blok M\n";
    cout << "      3. > Stasiun Lb. Bulus\n";
    cout << "      4. > Stasiun Tanjung Barat\n";
    cout << "      5. > Stasiun Taman Mini\n";
    cout << "      6. > Kembali\n"; 
    cout << "\n   Pilih titik awal (1 - 6) = ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string selectedStation = "";

    switch (choice) {
        case 1: selectedStation = "Tebet"; break;
        case 2: selectedStation = "Blok M"; break;
        case 3: selectedStation = "Lb. Bulus"; break;
        case 4: selectedStation = "Tanjung Barat"; break;
        case 5: selectedStation = "Taman Mini"; break;
        case 6: return; 
        default:
            cout << "\nPilihan tidak valid. Silakan coba lagi.\n";
            pressEnterToContinue();
            return;
    }

    displayRoute(selectedStation); // Call displayRoute function
}

// ===============================================
//           MAIN FUNCTION
// ===============================================

int main() {
    initializeData(); // Initialize all global data once at startup

    // Main application loop: handles login and then the main menu
    while (true) {
        displayLoginMenu(); // Manages the login process; loops until successful login
        if (currentUser != nullptr) { // If a user is successfully logged in
            displayMainMenu(); // Enter the main application menu loop
        } else {
            // This part is reached if currentUser becomes nullptr (e.g., after logout)
            // It implicitly loops back to displayLoginMenu
        }
    }

    return 0;
}