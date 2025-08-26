#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <limits>
#include <unordered_map>
#include <iomanip>
#include <algorithm>

using namespace std;

// ===================== Config / Constants =====================
constexpr double BASE_TICKET_PRICE = 3000.0;
constexpr int TICKET_VALIDITY_YEARS = 1;
constexpr int MAX_PASSENGERS = 180;
constexpr double DISTANCE_COST_FACTOR = 5.0;
constexpr double RETURN_TICKET_MULTIPLIER = 1.8;

// ===================== Utilities =====================
static inline void clearCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ===================== Airport =====================
class Airport {
    string code;
    string name;
    double x, y;
public:
    Airport() : x(0), y(0) {}
    Airport(string code, string name, double x, double y)
        : code(std::move(code)), name(std::move(name)), x(x), y(y) {}
    const string& getCode() const { return code; }
    const string& getName() const { return name; }
    double distanceTo(const Airport& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx*dx + dy*dy);
    }
};

// ===================== Passenger & Roles =====================
class Passenger {
protected:
    string name;
    int id;
public:
    Passenger(string name, int id) : name(std::move(name)), id(id) {}
    virtual ~Passenger() = default;
    const string& getName() const { return name; }
    int getId() const { return id; }
    virtual string getRole() const { return "Passenger"; }
};

class Pilot : public Passenger {
    int yearsOfExperience;
public:
    Pilot(string name, int id, int years)
        : Passenger(std::move(name), id), yearsOfExperience(years) {}
    string getRole() const override { return "Pilot"; }
    int getYears() const { return yearsOfExperience; }
};

class FlightAttendant : public Passenger {
    string airline;
public:
    FlightAttendant(string name, int id, string airline)
        : Passenger(std::move(name), id), airline(std::move(airline)) {}
    string getRole() const override { return "Flight Attendant"; }
    const string& getAirline() const { return airline; }
};

// ===================== Ticket =====================
class Ticket {
    int id;
    Airport src;
    Airport dst;
    shared_ptr<Passenger> passenger;
    bool isReturnTicket;
    int daysUntilTravel;
    int validityYears;
    double price;

    static double proximityMultiplier(int daysUntilTravel) {
        if (daysUntilTravel > 30) return 1.0;
        if (daysUntilTravel > 7)  return 1.2;
        return 1.5;
    }

public:
    Ticket(int id,
           const Airport& src,
           const Airport& dst,
           shared_ptr<Passenger> pax,
           bool isReturn,
           int daysUntilTravel)
        : id(id), src(src), dst(dst), passenger(std::move(pax)),
          isReturnTicket(isReturn), daysUntilTravel(daysUntilTravel),
          validityYears(TICKET_VALIDITY_YEARS), price(0.0) {
        double distance = src.distanceTo(dst);
        double baseFare = BASE_TICKET_PRICE + (distance * DISTANCE_COST_FACTOR);
        double fare = baseFare * proximityMultiplier(daysUntilTravel);
        if (isReturnTicket) fare *= RETURN_TICKET_MULTIPLIER;
        price = fare;
    }

    int getId() const { return id; }
    const string& sourceCode() const { return src.getCode(); }
    const string& destCode() const { return dst.getCode(); }
    double getPrice() const { return price; }
    const shared_ptr<Passenger>& getPassenger() const { return passenger; }
    bool isReturn() const { return isReturnTicket; }
    bool isValid() const { return validityYears > 0; }
    void updateValidity(int years) { validityYears += years; }

    void print() const {
        cout << "\n----- Ticket -----\n";
        cout << "Ticket ID       : " << id << "\n";
        cout << "Passenger       : " << passenger->getName()
             << " (" << passenger->getRole() << ")\n";
        cout << "Route           : " << src.getCode() << " -> " << dst.getCode() << "\n";
        cout << "Return Ticket   : " << (isReturnTicket ? "Yes" : "No") << "\n";
        cout << "Days to Travel  : " << daysUntilTravel << "\n";
        cout << "Validity (years): " << validityYears 
             << (isValid() ? "" : " (Expired)") << "\n";
        cout << fixed << setprecision(2) << "Price           : " << price << "\n";
    }
};

// ===================== Airline System =====================
class AirlineSystem {
    vector<shared_ptr<Passenger>> passengers;
    vector<Ticket> tickets;
    unordered_map<string, Airport> airports;
    int nextPassengerId = 1;
    int nextTicketId = 1;

public:
    AirlineSystem() {
        airports.emplace("DEL", Airport("DEL", "Delhi", 0, 0));
        airports.emplace("BOM", Airport("BOM", "Mumbai", 100, 40));
        airports.emplace("BLR", Airport("BLR", "Bangalore", 50, -80));
        airports.emplace("HYD", Airport("HYD", "Hyderabad", 60, -30));
        airports.emplace("MAA", Airport("MAA", "Chennai", 70, -90));
        airports.emplace("CCU", Airport("CCU", "Kolkata", 120, 90));
    }

    void listAirports() const {
        cout << "\n--- Airports ---\n";
        for (const auto& kv : airports)
            cout << kv.first << " - " << kv.second.getName() << "\n";
    }

    bool hasAirport(const string& code) const { return airports.find(code) != airports.end(); }
    const Airport& getAirport(const string& code) const { return airports.at(code); }

    shared_ptr<Passenger> addPassenger(const string& name) {
        auto p = make_shared<Passenger>(name, nextPassengerId++);
        passengers.push_back(p);
        return p;
    }
    shared_ptr<Pilot> addPilot(const string& name, int years) {
        auto p = make_shared<Pilot>(name, nextPassengerId++, years);
        passengers.push_back(p);
        return p;
    }
    shared_ptr<FlightAttendant> addFlightAttendant(const string& name, const string& airline) {
        auto p = make_shared<FlightAttendant>(name, nextPassengerId++, airline);
        passengers.push_back(p);
        return p;
    }

    void listPassengers() const {
        cout << "\n--- Passengers/Crew ---\n";
        for (auto& p : passengers)
            cout << "#" << p->getId() << " - " << p->getName()
                 << " (" << p->getRole() << ")\n";
        cout << "Total onboard: " << passengers.size() << "\n";
    }

    shared_ptr<Passenger> findPassengerById(int id) const {
        for (auto& p : passengers)
            if (p->getId() == id) return p;
        return nullptr;
    }

    bool hasCrew() const {
        bool pilot = false, attendant = false;
        for (auto& p : passengers) {
            string role = p->getRole();
            if (role == "Pilot") pilot = true;
            if (role == "Flight Attendant") attendant = true;
        }
        return pilot && attendant;
    }

    int passengerCountOnly() const {
        int count = 0;
        for (auto& p : passengers)
            if (p->getRole() == "Passenger") count++;
        return count;
    }

    bool capacityAvailable() const { return passengerCountOnly() < MAX_PASSENGERS; }

    bool bookTicket(const string& srcCode, const string& dstCode, int pid, bool isReturn, int daysUntilTravel) {
        if (!hasAirport(srcCode) || !hasAirport(dstCode)) { cout << "Invalid airport code(s).\n"; return false; }
        if (srcCode == dstCode) { cout << "Source and destination cannot be the same.\n"; return false; }
        if (!hasCrew()) { cout << "Crew missing!\n"; return false; }
        if (!capacityAvailable()) { cout << "Flight full!\n"; return false; }

        auto pax = findPassengerById(pid);
        if (!pax) { cout << "Passenger not found.\n"; return false; }
        if (pax->getRole() != "Passenger") { cout << "Only passengers can book tickets.\n"; return false; }

        tickets.emplace_back(nextTicketId++, getAirport(srcCode), getAirport(dstCode), pax, isReturn, daysUntilTravel);
        cout << "Ticket booked for " << pax->getName() << ".\n";
        return true;
    }

    void listTickets() const {
        int shown = 0;
        for (auto& t : tickets) {
            t.print();
            shown++;
        }
        if (shown == 0) cout << "\nNo tickets booked.\n";
        else cout << "Total Tickets: " << shown << "\n";
    }

    void expireTickets(int years) {
        for (auto& t : tickets) t.updateValidity(-years);
        cout << years << " year(s) passed. Ticket validity updated.\n";
    }
};

// ===================== Menu =====================
void printMenu() {
    cout << "\n=== Airline Booking System ===\n";
    cout << "1. List Airports\n2. Add Passenger\n3. Add Pilot\n4. Add Flight Attendant\n";
    cout << "5. List Passengers/Crew\n6. Book Ticket\n7. List Tickets\n8. Simulate Time\n0. Exit\n";
    cout << "Choice: " << flush; // flush ensures immediate display
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    AirlineSystem sys;

    // Seed minimal crew
    sys.addPilot("Capt. Sharma", 15);
    sys.addFlightAttendant("Anita", "IndiGo");

    int choice;
    while (true) {
        printMenu();
        if (!(cin >> choice)) { clearCin(); continue; }
        if (choice == 0) break;

        switch (choice) {
            case 1: sys.listAirports(); break;
            case 2: {
                cout << "Passenger name: " << flush; clearCin();
                string name; getline(cin, name);
                auto p = sys.addPassenger(name);
                cout << "Added Passenger #" << p->getId() << " - " << p->getName() << "\n";
                break;
            }
            case 3: {
                cout << "Pilot name: " << flush; clearCin();
                string name; getline(cin, name);
                cout << "Years experience: " << flush; int years; cin >> years;
                auto p = sys.addPilot(name, years);
                cout << "Added Pilot #" << p->getId() << " - " << p->getName() << "\n";
                break;
            }
            case 4: {
                cout << "Attendant name: " << flush; clearCin();
                string name; getline(cin, name);
                cout << "Airline: " << flush; string airline; getline(cin, airline);
                auto p = sys.addFlightAttendant(name, airline);
                cout << "Added Flight Attendant #" << p->getId() << " - " << p->getName() << "\n";
                break;
            }
            case 5:
                sys.listPassengers();
                cout << (sys.hasCrew() ? "Crew OK.\n" : "Crew missing!\n");
                break;
            case 6: {
                cout << "Source code: " << flush; string src; cin >> src;
                cout << "Destination code: " << flush; string dst; cin >> dst;
                cout << "Passenger ID: " << flush; int pid; cin >> pid;
                cout << "Return ticket (y/n): " << flush; char r; cin >> r;
                cout << "Days until travel: " << flush; int days; cin >> days;
                sys.bookTicket(src, dst, pid, (r=='y'||r=='Y'), days);
                break;
            }
            case 7: sys.listTickets(); break;
            case 8: {
                cout << "Years to simulate: " << flush; int y; cin >> y;
                sys.expireTickets(y);
                break;
            }
            default: cout << "Invalid choice.\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}
