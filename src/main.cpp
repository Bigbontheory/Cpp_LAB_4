#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include <clocale>

#include "ordinal_print.hpp"
#include "ordinal.hpp"
#include "lazy_sequence.hpp"

#include "streams/lazy_input_streams.hpp"
#include "../statistics/statistics_processor.hpp"

const int MAX_SEQUENCES = 20;

int natural_numbers_rule(std::size_t index) {
    return static_cast<int>(index + 1);
}

int even_numbers_rule(std::size_t index) {
    return static_cast<int>((index + 1) * 2);
}

int square_numbers_rule(std::size_t index) {
    return static_cast<int>(index * index);
}

int fibonacci_rule(const MutableArraySequence<int>& window) {
    if (window.get_size() < 2) {
        throw std::invalid_argument("Fibonacci rule requires a window of at least 2 elements");
    }
    return window.get(window.get_size() - 2) + window.get(window.get_size() - 1);
}

bool is_even_predicate(const int& val) {
    return val % 2 == 0;
}

bool is_greater_than_five_predicate(const int& val) {
    return val > 5;
}

int multiply_by_ten_mapper(const int& val) {
    return val * 10;
}

int cube_mapper(const int& val) {
    return val * val * val;
}

void print_menu() {
    std::cout << "\n=========================================\n";
    std::cout << "          LAZY SEQUENCE MANAGER          \n";
    std::cout << "=========================================\n";
    std::cout << "1. Create Infinite Sequence (Natural numbers, length w)\n";
    std::cout << "2. Create Finite Sequence (via MutableArraySequence)\n";
    std::cout << "3. Create Function Sequence (Even / Squares, Finite or w)\n";
    std::cout << "4. Create Fibonacci Sequence (RecurrentGenerator, Finite or w)\n";
    std::cout << "5. Show all created sequences\n";
    std::cout << "-----------------------------------------\n";
    std::cout << "6. Select Active Sequence for operations\n";
    std::cout << "7. Get Length of Active Sequence\n";
    std::cout << "8. Get Element by index (int) from Active\n";
    std::cout << "9. Get Element by index (Ordinal) from Active\n";
    std::cout << "10. Get First element of Active\n";
    std::cout << "11. Get Last element of Active\n";
    std::cout << "-----------------------------------------\n";
    std::cout << "12. Append element to Active Sequence\n";
    std::cout << "13. Apply 'where' filter to Active Sequence\n";
    std::cout << "14. Apply 'map' transformation to Active Sequence\n";
    std::cout << "15. Concat Active Sequence with another Sequence\n";
    std::cout << "16. Prepend element to Active Sequence\n";
    std::cout << "17. Insert another Sequence at Ordinal index\n";
    std::cout << "-----------------------------------------\n";
    std::cout << "18. Calculate Stream Statistics for Active Sequence\n";
    std::cout << "0. Exit\n";
    std::cout << "=========================================\n";
    std::cout << "Choice: ";
}

void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    std::setlocale(LC_ALL, "");

    LazySeq<int>* sequences[MAX_SEQUENCES] = { nullptr };
    int sequence_count = 0;
    int active_idx = -1;
    int choice = -1;

    while (choice != 0) {
        print_menu();

        if (active_idx != -1) {
            std::cout << "[Active Sequence ID: " << active_idx << "]\n";
        }
        else {
            std::cout << "[No Active Sequence Selected]\n";
        }

        if (!(std::cin >> choice)) {
            std::cout << "Input error. Please enter a number.\n";
            clear_input();
            continue;
        }

        try {
            switch (choice) {
            case 1: {
                if (sequence_count >= MAX_SEQUENCES) { std::cout << "-> Storage full!\n"; break; }
                sequences[sequence_count] = new LazySeq<int>(natural_numbers_rule, Ordinal::omega());
                std::cout << "-> Infinite sequence of natural numbers created at ID: " << sequence_count << "\n";
                active_idx = sequence_count;
                sequence_count++;
                break;
            }
            case 2: {
                if (sequence_count >= MAX_SEQUENCES) { std::cout << "-> Storage full!\n"; break; }

                int size;
                std::cout << "Enter number of elements for finite sequence: ";
                std::cin >> size;
                if (size <= 0) throw std::invalid_argument("Size must be greater than 0");

                int* temp_arr = new int[size];
                std::cout << "Enter " << size << " integers:\n";
                for (int i = 0; i < size; ++i) {
                    std::cout << "Element [" << i << "]: ";
                    std::cin >> temp_arr[i];
                }

                MutableArraySequence<int> mut_seq(temp_arr, size);
                delete[] temp_arr;

                sequences[sequence_count] = new LazySeq<int>(mut_seq);
                std::cout << "-> Finite sequence created at ID: " << sequence_count << "\n";
                active_idx = sequence_count;
                sequence_count++;
                break;
            }
            case 3: {
                if (sequence_count >= MAX_SEQUENCES) { std::cout << "-> Storage full!\n"; break; }

                std::cout << "Choose function rule:\n";
                std::cout << "1. Even numbers (2, 4, 6, 8...)\n";
                std::cout << "2. Square numbers (0, 1, 4, 9...)\n";
                std::cout << "Your choice: ";
                int func_choice;
                std::cin >> func_choice;

                int (*selected_func)(std::size_t) = nullptr;
                if (func_choice == 1) selected_func = even_numbers_rule;
                else if (func_choice == 2) selected_func = square_numbers_rule;
                else throw std::invalid_argument("Invalid function rule choice");

                std::cout << "Choose length type for Function Sequence:\n";
                std::cout << "1. Finite length\n";
                std::cout << "2. Infinite length (w)\n";
                std::cout << "Your choice: ";
                int len_choice;
                std::cin >> len_choice;

                Ordinal length;
                if (len_choice == 1) {
                    std::size_t finite_len;
                    std::cout << "Enter finite length size: ";
                    std::cin >> finite_len;
                    if (finite_len <= 0) throw std::invalid_argument("Length must be greater than 0");
                    length = Ordinal(finite_len);
                }
                else if (len_choice == 2) {
                    length = Ordinal::omega();
                }
                else {
                    throw std::invalid_argument("Invalid length choice");
                }

                sequences[sequence_count] = new LazySeq<int>(selected_func, length);
                std::cout << "-> Function sequence created at ID: " << sequence_count << "\n";
                active_idx = sequence_count;
                sequence_count++;
                break;
            }
            case 4: {
                if (sequence_count >= MAX_SEQUENCES) { std::cout << "-> Storage full!\n"; break; }

                std::cout << "Choose length type for Fibonacci:\n";
                std::cout << "1. Finite length\n";
                std::cout << "2. Infinite length (w)\n";
                std::cout << "Your choice: ";
                int len_choice;
                std::cin >> len_choice;

                Ordinal length;
                if (len_choice == 1) {
                    std::size_t finite_len;
                    std::cout << "Enter finite length size: ";
                    std::cin >> finite_len;
                    if (finite_len <= 0) throw std::invalid_argument("Length must be greater than 0");
                    length = Ordinal(finite_len);
                }
                else if (len_choice == 2) {
                    length = Ordinal::omega();
                }
                else {
                    throw std::invalid_argument("Invalid length choice");
                }

                int initial_values[] = { 0, 1 };
                MutableArraySequence<int> initial_seq(initial_values, 2);

                sequences[sequence_count] = new LazySeq<int>(fibonacci_rule, initial_seq, length);
                std::cout << "-> Fibonacci sequence created at ID: " << sequence_count << "\n";
                active_idx = sequence_count;
                sequence_count++;
                break;
            }
            case 5: {
                std::cout << "--- Available Sequences ---\n";
                if (sequence_count == 0) std::cout << "(No sequences created yet)\n";
                for (int i = 0; i < sequence_count; ++i) {
                    std::cout << "ID: " << i
                        << " | Type: " << (sequences[i]->is_infinite() ? "Infinite (length " : "Finite (length ")
                        << sequences[i]->get_ordinal_length() << ")"
                        << (i == active_idx ? " [ACTIVE]" : "") << "\n";
                }
                break;
            }
            case 6: {
                if (sequence_count == 0) { std::cout << "Create a sequence first!\n"; break; }
                int target;
                std::cout << "Enter Sequence ID to select (0 to " << sequence_count - 1 << "): ";
                std::cin >> target;

                if (target < 0 || target >= sequence_count) std::cout << "Invalid ID!\n";
                else {
                    active_idx = target;
                    std::cout << "-> Active sequence changed to ID: " << active_idx << "\n";
                }
                break;
            }
            case 7: {
                if (active_idx == -1) { std::cout << "No active sequence selected!\n"; break; }
                std::cout << "-> Length: " << sequences[active_idx]->get_ordinal_length() << "\n";
                break;
            }
            case 8: {
                if (active_idx == -1) { std::cout << "No active sequence selected!\n"; break; }
                int index;
                std::cout << "Enter integer index (int): ";
                std::cin >> index;
                std::cout << "-> Element at index " << index << ": " << sequences[active_idx]->get(index) << "\n";
                break;
            }
            case 9: {
                if (active_idx == -1) { std::cout << "No active sequence selected!\n"; break; }
                std::size_t w_part, f_part;
                std::cout << "Enter w multiplier (w_count): ";
                std::cin >> w_part;
                std::cout << "Enter finite part (finite_part): ";
                std::cin >> f_part;

                Ordinal ord_index(w_part, f_part);
                std::cout << "-> Element at ordinal index " << ord_index << ": " << sequences[active_idx]->get(ord_index) << "\n";
                break;
            }
            case 10: {
                if (active_idx == -1) { std::cout << "No active sequence selected!\n"; break; }
                std::cout << "-> First element: " << sequences[active_idx]->get_first() << "\n";
                break;
            }
            case 11: {
                if (active_idx == -1) { std::cout << "No active sequence selected!\n"; break; }
                std::cout << "-> Last element: " << sequences[active_idx]->get_last() << "\n";
                break;
            }
            case 12: {
                if (active_idx == -1) { std::cout << "No active sequence selected!\n"; break; }
                if (sequence_count >= MAX_SEQUENCES) { std::cout << "-> Storage full!\n"; break; }

                int value_to_append;
                std::cout << "Enter integer value to append: ";
                std::cin >> value_to_append;

                LazySeq<int>* appended_seq = sequences[active_idx]->append(value_to_append);
                sequences[sequence_count] = appended_seq;
                std::cout << "-> Element appended. New sequence created at ID: " << sequence_count << "\n";

                active_idx = sequence_count;
                sequence_count++;
                break;
            }
            case 13: {
                if (active_idx == -1) { std::cout << "No active sequence selected!\n"; break; }
                if (sequence_count >= MAX_SEQUENCES) { std::cout << "-> Storage full!\n"; break; }

                std::cout << "Choose filter predicate:\n";
                std::cout << "1. Only Even numbers (val % 2 == 0)\n";
                std::cout << "2. Only Numbers greater than 5 (val > 5)\n";
                std::cout << "Your choice: ";
                int pred_choice;
                std::cin >> pred_choice;

                bool (*selected_predicate)(const int&) = nullptr;
                if (pred_choice == 1) {
                    selected_predicate = is_even_predicate;
                }
                else if (pred_choice == 2) {
                    selected_predicate = is_greater_than_five_predicate;
                }
                else {
                    throw std::invalid_argument("Invalid predicate choice");
                }

                LazySeq<int>* filtered_seq = sequences[active_idx]->where(selected_predicate);
                sequences[sequence_count] = filtered_seq;
                std::cout << "-> Filter applied. New sequence created at ID: " << sequence_count << "\n";

                active_idx = sequence_count;
                sequence_count++;
                break;
            }
            case 14: {
                if (active_idx == -1) { std::cout << "No active sequence selected!\n"; break; }
                if (sequence_count >= MAX_SEQUENCES) { std::cout << "-> Storage full!\n"; break; }

                std::cout << "Choose map transformation:\n";
                std::cout << "1. Multiply by 10 (val * 10)\n";
                std::cout << "2. Cube the number (val * val * val)\n";
                std::cout << "Your choice: ";
                int map_choice;
                std::cin >> map_choice;

                int (*selected_mapper)(const int&) = nullptr;
                if (map_choice == 1) {
                    selected_mapper = multiply_by_ten_mapper;
                }
                else if (map_choice == 2) {
                    selected_mapper = cube_mapper;
                }
                else {
                    throw std::invalid_argument("Invalid map transformation choice");
                }

                LazySeq<int>* mapped_seq = sequences[active_idx]->map(selected_mapper);
                sequences[sequence_count] = mapped_seq;
                std::cout << "-> Map applied. New sequence created at ID: " << sequence_count << "\n";

                active_idx = sequence_count;
                sequence_count++;
                break;
            }
            case 15: {
                if (active_idx == -1) { std::cout << "No active sequence selected!\n"; break; }
                if (sequence_count >= MAX_SEQUENCES) { std::cout << "-> Storage full!\n"; break; }

                std::cout << "--- Available Sequences for Concat ---\n";
                for (int i = 0; i < sequence_count; ++i) {
                    std::cout << "ID: " << i << " | Length: " << sequences[i]->get_ordinal_length() << "\n";
                }
                std::cout << "Enter Second Sequence ID to concat with Active: ";
                int second_id;
                std::cin >> second_id;

                if (second_id < 0 || second_id >= sequence_count) {
                    throw std::invalid_argument("Invalid Second Sequence ID");
                }

                LazySeq<int>* concatenated_seq = sequences[active_idx]->concat(*sequences[second_id]);
                sequences[sequence_count] = concatenated_seq;
                std::cout << "-> Sequences concatenated. New sequence created at ID: " << sequence_count << "\n";

                active_idx = sequence_count;
                sequence_count++;
                break;
            }
            case 16: {
                if (active_idx == -1) { std::cout << "No active sequence selected!\n"; break; }
                if (sequence_count >= MAX_SEQUENCES) { std::cout << "-> Storage full!\n"; break; }

                int value_to_prepend;
                std::cout << "Enter integer value to prepend: ";
                std::cin >> value_to_prepend;

                LazySeq<int>* prepended_seq = sequences[active_idx]->prepend(value_to_prepend);
                sequences[sequence_count] = prepended_seq;
                std::cout << "-> Element prepended. New sequence created at ID: " << sequence_count << "\n";

                active_idx = sequence_count;
                sequence_count++;
                break;
            }
            case 17: {
                if (active_idx == -1) { std::cout << "No active sequence selected!\n"; break; }
                if (sequence_count >= MAX_SEQUENCES) { std::cout << "-> Storage full!\n"; break; }

                std::cout << "--- Available Sequences for Insertion ---\n";
                for (int i = 0; i < sequence_count; ++i) {
                    std::cout << "ID: " << i << " | Length: " << sequences[i]->get_ordinal_length() << "\n";
                }

                std::cout << "Enter Sequence ID to INSERT into Active Sequence: ";
                int inserted_id;
                std::cin >> inserted_id;

                if (inserted_id < 0 || inserted_id >= sequence_count) {
                    throw std::invalid_argument("Invalid Sequence ID to insert");
                }

                std::size_t w_part, f_part;
                std::cout << "Enter Target Ordinal Index where sequence will be inserted (w count): ";
                std::cin >> w_part;
                std::cout << "Enter Target Ordinal Index (finite part): ";
                std::cin >> f_part;
                Ordinal target_idx(w_part, f_part);

                LazySeq<int>* inserted_seq = sequences[active_idx]->insert_at(target_idx, *sequences[inserted_id]);
                sequences[sequence_count] = inserted_seq;
                std::cout << "-> Sequence " << inserted_id << " inserted at " << target_idx
                    << ". New sequence created at ID: " << sequence_count << "\n";

                active_idx = sequence_count;
                sequence_count++;
                break;
            }
            case 18: {
                if (active_idx == -1 || sequences[active_idx] == nullptr) {
                    std::cout << "[ERROR]: No active sequence selected! Please select or create one first.\n";
                    break;
                }

                int window_size;
                int count;

                std::cout << "Enter median window size: ";
                std::cin >> window_size;

                std::cout << "Enter number of elements to process: ";
                std::cin >> count;

                if (window_size <= 0 || count <= 0) {
                    std::cout << "[ERROR]: Window size and count must be greater than 0!\n";
                    break;
                }

                std::cout << "\n--- Stream Statistics Pipeline ---\n";

                try {
                    LazyInputStream<int> stream(sequences[active_idx]);
                    stream.open();
                    StatisticsProcessor<int> processor(&stream, window_size);
                    for (int i = 0; i < count; ++i) {
                        try {
                            processor.process_next();
                            std::cout << "Step [" << i + 1 << "]:\n";
                            std::cout << "  Count:   " << processor.get_count() << "\n";
                            std::cout << "  Average: " << processor.get_average() << "\n";
                            std::cout << "  Median:  " << processor.get_median() << "\n";
                            std::cout << "  Min:     " << processor.get_min() << "\n";
                            std::cout << "  Max:     " << processor.get_max() << "\n";
                            std::cout << "----------------------------------\n";
                        }
                        catch (const std::out_of_range& e) {
                            std::cout << "\n[Stream ended gracefully]: " << e.what() << " (Total elements processed: " << i << ")\n";
                            break;
                        }
                    }

                    stream.close();
                }
                catch (const std::exception& e) {
                    std::cout << "\n[ERROR]: Pipeline execution failed: " << e.what() << "\n";
                }

                std::cout << "> Statistics processing complete.\n";
                break;
            }
            }
        }
        catch (const std::exception& e) {
            std::cout << "\n[ERROR]: " << e.what() << "\n";
        }
    }

    for (int i = 0; i < sequence_count; ++i) {
        delete sequences[i];
    }

    return 0;
}