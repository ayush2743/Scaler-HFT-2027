#include "order_book.h"
#include <iostream>
#include <iomanip>


void OrderBook::add_order(const Order& order) {
    if (order.is_buy) {
        // add to bid side
        bids[order.price].push_back(order);
        
        // get pointer to the order we just added
        Order* order_ptr = &bids[order.price].back();
        order_lookup[order.order_id] = order_ptr;
        
    } else {
        // add to ask side
        asks[order.price].push_back(order);
        
        Order* order_ptr = &asks[order.price].back();
        order_lookup[order.order_id] = order_ptr;
    }
}



bool OrderBook::cancel_order(uint64_t order_id) {
    // check if order exists
    auto it = order_lookup.find(order_id);
    if (it == order_lookup.end()) {
        return false;  // order not found
    }

    Order* order_ptr = it->second;
    double price = order_ptr->price;
    bool is_buy = order_ptr->is_buy;


    // remove from the correct side
    if (is_buy) {
        auto& order_list = bids[price];
        
        // find and remove the order
        for (auto list_it = order_list.begin(); list_it != order_list.end(); ++list_it) {
            if (list_it->order_id == order_id) {
                order_list.erase(list_it);
                break;
            }
        }

        // if no orders left at this price, remove the price level
        if (order_list.empty()) {
            bids.erase(price);
        }
        
    } else {
        auto& order_list = asks[price];
        
        for (auto list_it = order_list.begin(); list_it != order_list.end(); ++list_it) {
            if (list_it->order_id == order_id) {
                order_list.erase(list_it);
                break;
            }
        }

        if (order_list.empty()) {
            asks.erase(price);
        }
    }

    // remove from lookup table
    order_lookup.erase(order_id);
    return true;
}




bool OrderBook::amend_order(uint64_t order_id, double new_price, uint64_t new_quantity) {
    // find the order first
    auto it = order_lookup.find(order_id);
    if (it == order_lookup.end()) {
        return false;
    }

    Order* old_order = it->second;
    double old_price = old_order->price;
    

    // if price changed, we need to cancel and re-add
    if (old_price != new_price) {
        // save order details
        Order new_order = *old_order;
        new_order.price = new_price;
        new_order.quantity = new_quantity;
        
        // cancel old one
        cancel_order(order_id);
        
        // add new one
        add_order(new_order);
        
    } else {
        // just quantity changed, update in place
        old_order->quantity = new_quantity;
    }

    return true;
}



void OrderBook::get_snapshot(size_t depth, std::vector<PriceLevel>& bid_levels, std::vector<PriceLevel>& ask_levels) const {
    bid_levels.clear();
    ask_levels.clear();

    // collect bids (already sorted high to low)
    size_t count = 0;
    for (const auto& pair : bids) {
        if (count >= depth) break;
        
        double price = pair.first;
        const auto& order_list = pair.second;
        
        // sum up all quantities at this price
        uint64_t total_qty = 0;
        for (const auto& order : order_list) {
            total_qty += order.quantity;
        }
        
        bid_levels.push_back({price, total_qty});
        count++;
    }


    // collect asks (already sorted low to high)
    count = 0;
    for (const auto& pair : asks) {
        if (count >= depth) break;
        
        double price = pair.first;
        const auto& order_list = pair.second;
        
        uint64_t total_qty = 0;
        for (const auto& order : order_list) {
            total_qty += order.quantity;
        }
        
        ask_levels.push_back({price, total_qty});
        count++;
    }
}




void OrderBook::print_book(size_t depth) const {
    std::vector<PriceLevel> bid_levels, ask_levels;
    get_snapshot(depth, bid_levels, ask_levels);

    std::cout << "\n========== ORDER BOOK ==========\n";
    std::cout << std::fixed << std::setprecision(2);
    
    // print asks in reverse (highest first)
    std::cout << "\n--- ASKS ---\n";
    for (int i = ask_levels.size() - 1; i >= 0; i--) {
        std::cout << "  " << ask_levels[i].price << " | " << ask_levels[i].total_quantity << "\n";
    }
    
    std::cout << "------------\n";
    
    // print bids (highest first)
    std::cout << "--- BIDS ---\n";
    for (const auto& level : bid_levels) {
        std::cout << "  " << level.price << " | " << level.total_quantity << "\n";
    }
    
    std::cout << "\n================================\n\n";
}

