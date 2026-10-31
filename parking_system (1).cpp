/*
 * Smart Parking Management System - C++ OOP Mini Project
 * Submitted by : Arun Saini (UID: 25BCD10196)
 * Submitted to : Mr. Abhinay Sood, Professor
 *
 * Concepts: classes & objects, encapsulation, abstraction,
 * inheritance, polymorphism, constructors/destructors,
 * static members, friend function, operator overloading.
 */
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

// ===== PART 1: Vehicle classes (abstraction, inheritance) =====
class Vehicle {                       // abstract base class
protected:
    string plate;
    int entryHour;
    static int totalParked;           // shared by all objects
public:
    Vehicle(const string& p, int h) : plate(p), entryHour(h) {
        totalParked++;
    }
    virtual ~Vehicle() { totalParked--; }

    virtual string type() const = 0;           // pure virtual
    virtual double ratePerHour() const = 0;    // pure virtual

    virtual double calculateFee(int exitHour) const {
        int hours = exitHour - entryHour;
        if (hours < 1) hours = 1;              // minimum 1 hour
        return hours * ratePerHour();
    }
    string getPlate() const { return plate; }
    int getEntryHour() const { return entryHour; }
    static int getTotalParked() { return totalParked; }

    friend ostream& operator<<(ostream& os, const Vehicle& v);
};
int Vehicle::totalParked = 0;

ostream& operator<<(ostream& os, const Vehicle& v) {
    os << left << setw(7) << v.type() << setw(12) << v.plate
       << "Entry: " << v.entryHour << ":00";
    return os;
}

class Car : public Vehicle {
public:
    Car(const string& p, int h) : Vehicle(p, h) {}
    string type() const override { return "Car"; }
    double ratePerHour() const override { return 20; }
};

class Bike : public Vehicle {
public:
    Bike(const string& p, int h) : Vehicle(p, h) {}
    string type() const override { return "Bike"; }
    double ratePerHour() const override { return 10; }
};

class Truck : public Vehicle {
public:
    Truck(const string& p, int h) : Vehicle(p, h) {}
    string type() const override { return "Truck"; }
    double ratePerHour() const override { return 40; }
    double calculateFee(int exitHour) const override {
        return Vehicle::calculateFee(exitHour) + 50;  // heavy charge
    }
};

// ===== PART 2: ParkingLot class (encapsulation, composition) =====
class ParkingLot {
private:
    int capacity;
    vector<Vehicle*> slots;
    double earnings;
    int served;

    int findPlate(const string& plate) const {
        for (int i = 0; i < capacity; i++)
            if (slots[i] && slots[i]->getPlate() == plate) return i;
        return -1;
    }
public:
    ParkingLot(int cap) : capacity(cap), slots(cap, nullptr),
                          earnings(0), served(0) {}
    ~ParkingLot() {
        for (Vehicle* v : slots) delete v;
    }

    void park(Vehicle* v) {
        if (findPlate(v->getPlate()) != -1) {
            cout << "\nVehicle already parked!\n";
            delete v;
            return;
        }
        for (int i = 0; i < capacity; i++) {
            if (!slots[i]) {
                slots[i] = v;
                cout << "\n" << v->type() << " " << v->getPlate()
                     << " parked in Slot " << i + 1 << ".\n";
                return;
            }
        }
        cout << "\nParking lot is FULL!\n";
        delete v;
    }

    void removeVehicle(const string& plate, int exitHour) {
        int i = findPlate(plate);
        if (i == -1) { cout << "\nVehicle not found!\n"; return; }
        if (exitHour < slots[i]->getEntryHour()) {
            cout << "\nExit time cannot be before entry time!\n";
            return;
        }
        double fee = slots[i]->calculateFee(exitHour);   // polymorphism
        cout << "\n------------- PARKING TICKET -------------\n";
        cout << *slots[i] << "\n";
        cout << "Exit : " << exitHour << ":00\n";
        cout << "Fee  : Rs. " << fee << "\n";
        cout << "------------------------------------------\n";
        earnings += fee;
        served++;
        delete slots[i];
        slots[i] = nullptr;
    }

    void showStatus() const {
        cout << "\n--------------- PARKING STATUS ---------------\n";
        for (int i = 0; i < capacity; i++) {
            cout << "Slot " << i + 1 << " : ";
            if (slots[i]) cout << *slots[i] << "\n";
            else cout << "Empty\n";
        }
        cout << "Vehicles parked now: " << Vehicle::getTotalParked() << "\n";
    }

    void report() const {
        cout << "\n----------------- TODAY'S REPORT -----------------\n";
        cout << "Vehicles served : " << served << "\n";
        cout << "Total earnings  : Rs. " << earnings << "\n";
    }
};

// ===== PART 3: main program (menu-driven) =====
int main() {
    ParkingLot lot(5);
    int choice;
    cout << "========================================\n";
    cout << "     SMART PARKING MANAGEMENT SYSTEM\n";
    cout << "========================================\n";
    do {
        cout << "\n1. Park a vehicle\n2. Remove a vehicle (exit)\n";
        cout << "3. Show parking status\n4. Today's report\n0. Quit\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int t, hour; string plate;
            cout << "Type (1-Car 2-Bike 3-Truck): "; cin >> t;
            cout << "Number plate: "; cin >> plate;
            cout << "Entry hour (0-23): "; cin >> hour;
            if (t == 1) lot.park(new Car(plate, hour));
            else if (t == 2) lot.park(new Bike(plate, hour));
            else if (t == 3) lot.park(new Truck(plate, hour));
            else cout << "\nInvalid vehicle type!\n";
        } else if (choice == 2) {
            int hour; string plate;
            cout << "Number plate: "; cin >> plate;
            cout << "Exit hour (0-23): "; cin >> hour;
            lot.removeVehicle(plate, hour);
        } else if (choice == 3) lot.showStatus();
        else if (choice == 4) lot.report();
        else if (choice != 0) cout << "\nInvalid choice, try again.\n";
    } while (choice != 0);

    cout << "\nThank you for using the Parking System!\n";
    return 0;
}
