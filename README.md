# ✈️ Airline Booking System (C++)

## 📌 Overview
This project is a **menu-driven Airline Booking System** implemented in **C++** using **Object-Oriented Programming (OOP)** concepts.  
It simulates real-world airline operations such as:

- Managing **airports**
- Adding **passengers, pilots, and flight attendants**
- Booking **tickets** with dynamic pricing & return options
- Simulating **time progression** to expire tickets
- Enforcing rules such as **crew requirement** and **maximum passenger capacity**

---

## ⚙️ Features
- **Airports**
  - Predefined list of Indian airports (DEL, BOM, BLR, HYD, MAA, CCU).
  - Distance calculation based on (x,y) coordinates.

- **Passengers & Crew**
  - Supports normal **Passengers**, **Pilots**, and **Flight Attendants**.
  - Uses **inheritance & polymorphism** for roles.
  - Enforces crew requirements (at least 1 Pilot and 1 Flight Attendant needed).

- **Tickets**
  - Dynamic pricing:
    - Base fare + distance cost.
    - Price increases if booked close to travel date.
    - Discounted multiplier for return tickets.
  - Validity period (default: 1 year).
  - Expiry simulation via time progression.

- **System Rules**
  - Prevents booking if:
    - No crew onboard.
    - Flight is at max capacity (180 passengers).
    - Invalid source/destination airports.
    - Source and destination are the same.
    - Passenger is not a normal traveler (crew cannot book).

---

## 🛠️ Technologies Used
- **Language:** C++17
- **Libraries:** STL (`vector`, `unordered_map`, `memory`, `cmath`, etc.)
- **Concepts:** OOP (Inheritance, Polymorphism, Encapsulation), Smart Pointers (`shared_ptr`)

---

## ▶️ How to Run
### 1. Compile the Program
```bash
g++ -std=c++17 -O2 -o airline main.cpp
```

### 2. Run the Program
```bash
./airline
```

---

## 📋 Menu Options
```
=== Airline Booking System ===
1. List Airports
2. Add Passenger
3. Add Pilot
4. Add Flight Attendant
5. List Passengers/Crew
6. Book Ticket
7. List Tickets
8. Simulate Time
0. Exit
```

---

## 🎯 Example Usage
1. Add passengers and crew.  
2. Book a ticket (e.g., DEL → BLR, return ticket, 10 days before travel).  
3. List tickets to see details (fare, validity, etc.).  
4. Simulate time (e.g., +2 years) → tickets expire.  

---

## 📊 Ticket Pricing Formula
```
Base Fare = 3000 + (distance * 5)
Fare = Base Fare * Proximity Factor
If return ticket → Fare *= 1.8

Proximity Factor:
- >30 days: 1.0
- 7-30 days: 1.2
- ≤7 days: 1.5
```

## 🚀 Future Improvements
- Add flight schedules with specific dates/times.
- Add payment/seat selection system.
- Persist data to a file (save/load system state).
- GUI version for better user interaction.

---

## 👩‍💻 Author
Developed by **Bhavana Nali**
