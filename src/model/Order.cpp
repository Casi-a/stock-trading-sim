#include "Order.h"

// 생성자 구현
Order::Order(std::string sym, bool buy, int qty, double p, std::string d)
    : symbol(sym), isBuy(buy), quantity(qty), price(p), date(d) {
}

// Getter 구현
const std::string& Order::getSymbol() const {
    return symbol;
}

bool Order::getIsBuy() const {
    return isBuy;
}

int Order::getQuantity() const {
    return quantity;
}

double Order::getPrice() const {
    return price;
}

const std::string& Order::getDate() const {
    return date;
}