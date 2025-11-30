#include "../../include/model/Order.h"

// 생성자 구현
Order::Order(std::string sym, bool buy, int qty, double p, std::string d)
    : symbol(sym), isBuy(buy), quantity(qty), price(p), date(d) {
}