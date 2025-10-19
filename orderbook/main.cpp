#include "order_book.h"
#include <iostream>
#include <chrono>

using namespace std;


uint64_t get_timestamp() {
    auto now = chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto nanos = chrono::duration_cast<chrono::nanoseconds>(duration);
    return nanos.count();
}


int main() {
    OrderBook book;

    cout << "Testing Order Book Implementation\n";
    cout << "==================================\n\n";


    // add some buy orders
    cout << "Adding buy orders...\n";
    book.add_order({1, true, 100.50, 10, get_timestamp()});
    book.add_order({2, true, 100.50, 5, get_timestamp()});
    book.add_order({3, true, 100.00, 20, get_timestamp()});
    book.add_order({4, true, 99.50, 15, get_timestamp()});


    // add some sell orders
    cout << "Adding sell orders...\n";
    book.add_order({5, false, 101.00, 8, get_timestamp()});
    book.add_order({6, false, 101.50, 12, get_timestamp()});
    book.add_order({7, false, 101.00, 7, get_timestamp()});
    book.add_order({8, false, 102.00, 25, get_timestamp()});

    // initial state
    book.print_book(5);


    // test cancel
    cout << "Canceling order 2 (buy @ 100.50)...\n";
    if (book.cancel_order(2)) {
        cout << "Order cancelled successfully\n";
    }
    book.print_book(5);


    // test amend - quantity only
    cout << "Amending order 3 quantity from 20 to 30...\n";
    if (book.amend_order(3, 100.00, 30)) {
        cout << "Order amended successfully\n";
    }
    book.print_book(5);


    // test amend - price change
    cout << "Amending order 4 price from 99.50 to 100.25...\n";
    if (book.amend_order(4, 100.25, 15)) {
        cout << "Order amended successfully\n";
    }
    book.print_book(5);


    // test snapshot
    cout << "Getting snapshot (depth 3)...\n";
    vector<PriceLevel> bids, asks;
    book.get_snapshot(3, bids, asks);
    
    cout << "\nBids:\n";
    for (const auto& level : bids) {
        cout << "  Price: " << level.price << ", Qty: " << level.total_quantity << "\n";
    }
    
    cout << "\nAsks:\n";
    for (const auto& level : asks) {
        cout << "  Price: " << level.price << ", Qty: " << level.total_quantity << "\n";
    }


    // add more orders to test aggregation
    cout << "\n\nAdding more orders for aggregation test...\n";
    book.add_order({9, true, 100.50, 100, get_timestamp()});
    book.add_order({10, true, 100.50, 50, get_timestamp()});
    book.add_order({11, false, 101.00, 30, get_timestamp()});
    
    book.print_book(5);


    cout << "\nAll tests completed!\n";
    return 0;
}

