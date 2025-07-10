# 🎓 Smart Student Portal 🚀

A comprehensive student portal application for **Universitas Cakrawala** that provides academic consultation, library management, activity history, and campus navigation services.

## ✨ Features

### 🔐 1. User Authentication
- 🔒 Secure login system for students using NIM and password
- 👥 Five pre-registered student accounts for demonstration

### 🗣️ 2. Academic Consultation
- 🚦 Queue-based system for academic consultations
- 👨‍🏫 Two available mentors with different subjects:
  - Idrus Madani (📊 Data Structures)
  - Dwi Wulan (🧠 Algorithm and Programming)
- 📊 Real-time queue display with position tracking

### 📚 3. Digital Library
- 📖 Book borrowing system linked to academic mentors
- 🔍 Book search and filtering capabilities:
  - 🔎 Search by keyword
  - 🏷️ Filter by availability
- 🤖 Automatic book recommendation based on consultation mentor

### 📝 4. Activity Tracking
- 📅 Loan history tracking
- 📋 Comprehensive activity log
- ⏱️ Timestamped records for all student actions

### 🗺️ 5. Campus Navigation
- 🚏 Route guidance from 5 different stations to campus
- 👣 Step-by-step directions

## 🛠️ Data Structures Used

1. **🔗 Linked List** - For tracking loan history and student activities
2. **📶 Queue** - For managing academic consultation queues
3. **🔑 Hash Table (Map)** - For student login authentication
4. **🌳 Binary Search Tree (Set)** - For organizing library books
5. **🕸️ Graph** - For campus navigation routes

## 🚀 How to Use

1. Clone the repository `git clone [repo-url]`
2. Compile the program: `g++ smartstudent.cpp -o smartstudent`
3. Run the executable: `./smartstudent`

### 👥 Demo Accounts

| NIM    | Password | Name         | Icon |
|--------|----------|--------------|------|
| 217403 | 15071999 | Aulia Putri  | 👩‍🎓 |
| 532618 | 03082002 | Rafi Akbar   | 👨‍🎓 |
| 894205 | 22121997 | Laras Wulan  | 👩‍🎓 |
| 670349 | 09052000 | Dito Pratama | 👨‍🎓 |
| 356127 | 17111998 | Sinta Devi   | 👩‍🎓 |

## 🔄 Program Flow

1. 🔑 Login with NIM and password
2. 🏠 Main menu offers 5 options:
   - 🗣️ Academic Consultation
   - 📚 Library
   - 📜 Loan History
   - 📋 Activity History
   - 🗺️ Campus Navigation

### 🗣️ Academic Consultation
- ⚠️ Must be completed before accessing library services
- 🚦 Shows current queue position
- ➕ Allows new registration if not already in queue

### 📚 Library Services
- 📖 Shows books recommended by consultation mentor
- 🔍 Allows searching, filtering, and borrowing books

## 💻 Technical Details

- 💻 Written in C++
- 🧩 Uses Standard Template Library (STL) containers
- 🖥️ Platform-independent (Windows/Linux/macOS)
- 🎨 Clean console interface with ASCII art header

## 🔮 Future Enhancements

1. 📥 Add book return functionality
2. 🔄 Implement password change feature
3. 🗺️ Expand campus navigation routes
4. 👨‍💼 Add admin interface for data management
5. 💾 Implement file persistence for data storage

## 📜 License

This project is open-source and available under the MIT License.

---

Made with ❤️ for Universitas Cakrawala students!
