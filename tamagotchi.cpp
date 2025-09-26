/*********************************************************
 * tamagotchi game in terminal. creates a exe file       *
 *                                                       *
 *                                                       *
 * Author: Amelia Gannon                                 *
 *                                                       *
 * Date: 2025-09-23                                      *
 *                                                       *
**********************************************************/
#include <iostream>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

// Clamp helper
int clampi(int v, int lo, int hi) { return max(lo, min(v, hi)); }

// RNG
static random_device rd;
static mt19937 rng(rd());
bool chance(int percent) { uniform_int_distribution<int> d(1,100); return d(rng) <= percent; }

struct Pet {
    string name = "Mochi";
    int hunger = 40;       // 0=full, 100=starving
    int happiness = 70;    // 0..100
    int energy = 70;       // 0..100
    int cleanliness = 70;  // 0..100
    int health = 80;       // 0..100
    int ageDays = 0;       // increments over time
    bool alive = true;
    bool sick = false;

    // Apply a "time tick" of decay; dt is minutes
    void tick(int dt = 10) {
        if (!alive) return;

        // Baseline decay per 10 min
        int steps = max(1, dt / 10);
        for (int i = 0; i < steps; ++i) {
            hunger = clampi(hunger + 4, 0, 100);
            energy = clampi(energy - 3, 0, 100);
            cleanliness = clampi(cleanliness - 2, 0, 100);
            // Low stats reduce happiness
            if (hunger > 70) happiness = clampi(happiness - 3, 0, 100);
            if (energy < 30) happiness = clampi(happiness - 2, 0, 100);
            if (cleanliness < 40) happiness = clampi(happiness - 2, 0, 100);

            // Random sickness if dirty/tired/hungry
            if (!sick && (cleanliness < 30 || hunger > 80 || energy < 25)) {
                if (chance(10)) sick = true;
            }
            // Sickness degrades health faster
            if (sick) {
                health = clampi(health - 3, 0, 100);
                happiness = clampi(happiness - 2, 0, 100);
            } else {
                // Natural small drift
                health = clampi(health - (hunger > 85 ? 2 : 1), 0, 100);
            }

            // Starvation or exhaustion penalties
            if (hunger > 95) health = clampi(health - 4, 0, 100);
            if (energy < 10) health = clampi(health - 2, 0, 100);

            // Aging: every ~6 ticks (~1 hour)
            static int ticks = 0;
            if (++ticks >= 6) { ticks = 0; ageDays += 1; }

            // Death checks
            if (health <= 0 || happiness == 0) {
                alive = false;
                break;
            }
        }
    }

    // Actions
    string feed() {
        if (!alive) return "Nothing happens...";
        hunger = clampi(hunger - 35, 0, 100);
        happiness = clampi(happiness + 5, 0, 100);
        cleanliness = clampi(cleanliness - 3, 0, 100);
        tick(10); // time passes
        return "Nom nom! That hit the spot.";
    }
    string play() {
        if (!alive) return "Nothing happens...";
        if (energy < 20 || hunger > 80) return "Too tired or hungry to play.";
        happiness = clampi(happiness + 12, 0, 100);
        energy = clampi(energy - 15, 0, 100);
        hunger = clampi(hunger + 8, 0, 100);
        cleanliness = clampi(cleanliness - 6, 0, 100);
        tick(10);
        return "We played a fun game!";
    }
    string sleep() {
        if (!alive) return "Nothing happens...";
        energy = clampi(energy + 35, 0, 100);
        hunger = clampi(hunger + 10, 0, 100);
        cleanliness = clampi(cleanliness - 2, 0, 100);
        // Sleeping improves health slightly
        health = clampi(health + 8, 0, 100);
        tick(20); // more time passes while napping
        return "Zzz... Feeling rested.";
    }
    string clean() {
        if (!alive) return "Nothing happens...";
        cleanliness = clampi(cleanliness + 35, 0, 100);
        happiness = clampi(happiness + 3, 0, 100);
        // Cleaning reduces sickness chance; may cure mild sickness
        if (sick && chance(30)) { sick = false; health = clampi(health + 6, 0, 100); return "All clean! That helped, feeling better."; }
        tick(10);
        return "Sparkly clean!";
    }
    string medicate() {
        if (!alive) return "Nothing happens...";
        if (!sick) return "No meds needed right now.";
        sick = false;
        health = clampi(health + 15, 0, 100);
        happiness = clampi(happiness - 5, 0, 100); // bitter medicine
        tick(10);
        return "Medicine taken. On the mend.";
    }
    string waitAction() {
        if (!alive) return "Time passes in silence...";
        tick(10);
        return "You spend some quiet time together.";
    }

    // ASCII face based on mood
    string face() const {
        if (!alive) return "(x_x)";
        if (sick) return "(>_<)";
        if (happiness >= 75) return "^_^";
        if (happiness >= 45) return "-_-";
        return "T_T";
    }

    // Render status line
    void show() const {
        cout << "\n==============\n";
        cout << name << "  " << face() << (sick ? "  [SICK]" : "") << "\n";
        cout << "Age: " << ageDays << " day(s)\n";
        cout << "Hunger:      " << hunger << "/100 (higher = hungrier)\n";
        cout << "Happiness:   " << happiness << "/100\n";
        cout << "Energy:      " << energy << "/100\n";
        cout << "Cleanliness: " << cleanliness << "/100\n";
        cout << "Health:      " << health << "/100\n";
        if (!alive) cout << "\nYour pet has passed away... Thank you for caring.\n";
        cout << "==============\n";
    }
};

void printMenu(bool alive) {
    cout << "\nChoose an action:\n";
    if (alive) {
        cout << " 1) Feed\n";
        cout << " 2) Play\n";
        cout << " 3) Sleep\n";
        cout << " 4) Clean\n";
        cout << " 5) Medicate\n";
        cout << " 6) Wait (pass time)\n";
    }
    cout << " 7) Stats\n";
    cout << " 8) Rename\n";
    cout << " 9) Help\n";
    cout << " 0) Quit\n> ";
}

void printHelp() {
    cout << "\nHelp:\n"
         << "- Keep Hunger low (feed), keep Energy up (sleep), and keep Cleanliness up (clean).\n"
         << "- Playing boosts Happiness but costs Energy and raises Hunger.\n"
         << "- Dirty/tired/hungry pets can get sick; medicate to cure.\n"
         << "- Health falls if starving, exhausted, sick, or neglected.\n"
         << "- Time passes a little after each action; 'Wait' passes time deliberately.\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Pet pet;
    cout << "Welcome to TinyGotchi!\n";
    cout << "Name your pet (press Enter for '" << pet.name << "'): ";
    string n; getline(cin, n);
    if (!n.empty()) pet.name = n;

    cout << "\nTake good care of " << pet.name << "!\n";
    pet.show();
    printHelp();

    while (true) {
        if (!pet.alive) {
            cout << "\n(type 7 to view stats, 8 to rename/restart, or 0 to quit)\n";
        }
        printMenu(pet.alive);
        string choice;
        if (!getline(cin, choice)) break;
        if (choice.empty()) continue;

        if (choice == "0") {
            cout << "Goodbye!\n";
            break;
        } else if (choice == "7") {
            pet.show();
        } else if (choice == "8") {
            cout << "Enter new name (or same to keep): ";
            string nn; getline(cin, nn);
            if (!nn.empty()) {
                bool wasDead = !pet.alive;
                pet.name = nn;
                cout << "Name updated.\n";
                // If pet is dead, allow soft restart on rename
                if (wasDead) {
                    pet.alive = true;
                    pet.hunger = 40; pet.happiness = 70; pet.energy = 70;
                    pet.cleanliness = 70; pet.health = 80; pet.sick = false; pet.ageDays = 0;
                    cout << "A new life begins for " << pet.name << "!\n";
                }
            }
            pet.show();
        } else if (choice == "9") {
            printHelp();
        } else if (!pet.alive) {
            cout << "Your pet is gone. Rename (8) to start anew, or Quit (0).\n";
        } else if (choice == "1") {
            cout << pet.feed() << "\n";
            pet.show();
        } else if (choice == "2") {
            cout << pet.play() << "\n";
            pet.show();
        } else if (choice == "3") {
            cout << pet.sleep() << "\n";
            pet.show();
        } else if (choice == "4") {
            cout << pet.clean() << "\n";
            pet.show();
        } else if (choice == "5") {
            cout << pet.medicate() << "\n";
            pet.show();
        } else if (choice == "6") {
            cout << pet.waitAction() << "\n";
            pet.show();
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
