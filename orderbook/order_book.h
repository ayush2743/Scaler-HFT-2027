#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <list>


struct Order {
    uint64_t order_id;
    bool is_buy;           // buy or sell
    double price;
    uint64_t quantity;
    uint64_t timestamp_ns;
};

struct PriceLevel {
    double price;
    uint64_t total_quantity;
};



class OrderBook {
private:
    
    // for each price, we keep a list of orders (FIFO)
    
    
    // bids: highest price first (descending)
    std::map<double, std::list<Order>, std::greater<double>> bids;
    
    // asks: lowest price first (ascending)
    std::map<double, std::list<Order>, std::less<double>> asks;

    // quick lookup for cancel/amend operations
    std::unordered_map<uint64_t, Order*> order_lookup;

public:

    void add_order(const Order& order);
 
    bool cancel_order(uint64_t order_id);



    bool amend_order(uint64_t order_id, double new_price, uint64_t new_quantity);


    void get_snapshot(size_t depth, std::vector<PriceLevel>& bids, std::vector<PriceLevel>& asks) const;


    void print_book(size_t depth = 10) const;
};

