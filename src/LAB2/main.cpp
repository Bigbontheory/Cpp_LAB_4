```cpp
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include "lazy_sequence.hpp"
#include "ordinal.hpp"

using namespace std;

struct Record {
    int id;
    LazySeq<int>* seq;
    bool deleted;
};

class UI {
private:
    vector<Record> data;
    int next_id = 0;

public:

    // =======================
    // helpers
    // =======================

    LazySeq<int>* get_seq(int id) {
        if (id < 0 || id >= data.size() || data[id].deleted) {
            throw runtime_error("Invalid ID");
        }
        return data[id].seq;
    }

    int add(LazySeq<int>* seq) {
        data.push_back({ next_id, seq, false });
        return next_id++;
    }

    Ordinal read_ordinal() {
        size_t omega, finite;
        cout << "omega_count finite_part: ";
        cin >> omega >> finite;
        return Ordinal(omega, finite);
    }

    // =======================
    // create finite
    // =======================

    void create_array() {
        cout << "Enter numbers: ";
        cin.ignore();

        string line;
        getline(cin, line);

        stringstream ss(line);
        vector<int> temp;

        int x;
        while (ss >> x) temp.push_back(x);

        if (temp.empty()) {
            cout << "Empty\n";
            return;
        }

        int* arr = new int[temp.size()];
        for (int i = 0; i < temp.size(); i++) {
            arr[i] = temp[i];
        }

        auto seq = new LazySeq<int>(arr, temp.size());
        cout << "Created ID = " << add(seq) << "\n";
    }

    // =======================
    // create ω naturals
    // =======================

    static int naturals_func(size_t i) {
        return (int)i;
    }

    void create_naturals() {
        auto seq = new LazySeq<int>(naturals_func, Ordinal::omega());
        cout << "Naturals (ω) ID = " << add(seq) << "\n";
    }

    // =======================
    // append
    // =======================

    void append() {
        int id, value;
        cout << "Seq ID: ";
        cin >> id;

        cout << "Value: ";
        cin >> value;

        try {
            auto seq = get_seq(id);
            auto new_seq = seq->append(value);

            cout << "New ID = " << add(new_seq) << "\n";
        }
        catch (exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    // =======================
    // get int
    // =======================

    void get_int() {
        int id, index;
        cout << "Seq ID: ";
        cin >> id;

        cout << "Index: ";
        cin >> index;

        try {
            auto seq = get_seq(id);
            cout << "Value = " << seq->get(index) << "\n";
        }
        catch (exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    // =======================
    // get ordinal
    // =======================

    void get_ord() {
        int id;
        cout << "Seq ID: ";
        cin >> id;

        try {
            auto seq = get_seq(id);
            Ordinal ord = read_ordinal();

            cout << "Value = " << seq->get(ord) << "\n";
        }
        catch (exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    // =======================
    // length
    // =======================

    void length() {
        int id;
        cout << "Seq ID: ";
        cin >> id;

        try {
            auto seq = get_seq(id);
            cout << "Length = " << seq->get_ordinal_length() << "\n";
        }
        catch (exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    // =======================
    // print prefix
    // =======================

    void print_prefix() {
        int id, n;
        cout << "Seq ID: ";
        cin >> id;

        cout << "Count: ";
        cin >> n;

        try {
            auto seq = get_seq(id);

            for (int i = 0; i < n; i++) {
                cout << seq->get(i) << " ";
            }
            cout << "\n";
        }
        catch (exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    // =======================
    // list
    // =======================

    void list() {
        cout << "\n=== SEQUENCES ===\n";
        for (auto& r : data) {
            if (!r.deleted) {
                cout << "ID: " << r.id
                    << " | length: " << r.seq->get_ordinal_length()
                    << " | infinite: " << (r.seq->is_infinite() ? "yes" : "no")
                    << "\n";
            }
        }
    }

    // =======================
    // main loop
    // =======================

    void run() {
        while (true) {
            cout << "\n=== MENU ===\n";
            cout << "1. Create finite sequence\n";
            cout << "2. Create naturals (ω)\n";
            cout << "3. List\n";
            cout << "4. Append\n";
            cout << "5. Get (int)\n";
            cout << "6. Get (ordinal)\n";
            cout << "7. Length\n";
            cout << "8. Print prefix\n";
            cout << "0. Exit\n";

            int cmd;
            cin >> cmd;

            switch (cmd) {
            case 1: create_array(); break;
            case 2: create_naturals(); break;
            case 3: list(); break;
            case 4: append(); break;
            case 5: get_int(); break;
            case 6: get_ord(); break;
            case 7: length(); break;
            case 8: print_prefix(); break;
            case 0: return;
            default: cout << "Unknown\n";
            }
        }
    }
};

int main() {
    UI ui;
    ui.run();
    return 0;
}
```
