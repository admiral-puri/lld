#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <stdexcept>
#include <string>

// Base Court Class
class Court {
public:
    virtual int getPrice() const = 0;
    virtual ~Court() = default;
};

class GrassCourt : public Court {
public:
    int getPrice() const override { return 100; }
};

class ClayCourt : public Court {
public:
    int getPrice() const override { return 200; }
};

class HardCourt : public Court {
public:
    int getPrice() const override { return 300; }
};

// Factory for Courts
class CourtFactory {
public:
    static std::shared_ptr<Court> createCourt(const std::string& type) {
        if (type == "Grass") return std::make_shared<GrassCourt>();
        if (type == "Clay") return std::make_shared<ClayCourt>();
        if (type == "Hard") return std::make_shared<HardCourt>();
        throw std::invalid_argument("Invalid court type");
    }
};

// Base Rental Item Class
class RentalItem {
public:
    virtual int getPrice() const = 0;
    virtual ~RentalItem() = default;
};

class Ballpack : public RentalItem {
public:
    int getPrice() const override { return 20; }
};

class Racket : public RentalItem {
public:
    int getPrice() const override { return 30; }
};

class Grips : public RentalItem {
public:
    int getPrice() const override { return 40; }
};

// Factory for Rental Items
class RentalItemFactory {
public:
    static std::shared_ptr<RentalItem> createRentalItem(const std::string& type) {
        if (type == "Ballpack") return std::make_shared<Ballpack>();
        if (type == "Racket") return std::make_shared<Racket>();
        if (type == "Grips") return std::make_shared<Grips>();
        throw std::invalid_argument("Invalid rental item type");
    }
};

// Pricing Strategy
class PricingStrategy {
public:
    static int calculateTotal(const std::shared_ptr<Court>& court, const std::vector<std::shared_ptr<RentalItem>>& items) {
        int total = court->getPrice();
        for (const auto& item : items) {
            total += item->getPrice();
        }
        return total;
    }
};

// Payment Processor Interface
class PaymentProcessor {
public:
    virtual void collectPaymentDetails() = 0;
    virtual void processPayment(int amount) const = 0;
    virtual ~PaymentProcessor() = default;
};

class CardPayment : public PaymentProcessor {
    std::string cardNumber;
    std::string expiryDate;
    std::string cardHolderName;
    std::string cvv;

public:
    void collectPaymentDetails() override {
        std::cout << "Enter Card Number: ";
        std::cin >> cardNumber;
        std::cout << "Enter Expiry Date (MM/YY): ";
        std::cin >> expiryDate;
        std::cout << "Enter Card Holder Name: ";
        std::cin.ignore();
        std::getline(std::cin, cardHolderName);
        std::cout << "Enter CVV: ";
        std::cin >> cvv;
    }

    void processPayment(int amount) const override {
        std::cout << "Processing card payment of ₹" << amount << "\n";
        std::cout << "Payment successful for card ending with " << cardNumber.substr(cardNumber.size() - 4) << "\n";
    }
};

class UPIPayment : public PaymentProcessor {
    std::string upiId;

public:
    void collectPaymentDetails() override {
        std::cout << "Enter UPI ID: ";
        std::cin >> upiId;
    }

    void processPayment(int amount) const override {
        std::cout << "Processing UPI payment of ₹" << amount << "\n";
        std::cout << "Payment successful for UPI ID " << upiId << "\n";
    }
};

class PaymentProcessorFactory {
public:
    static std::shared_ptr<PaymentProcessor> createProcessor(const std::string& method) {
        if (method == "Card") return std::make_shared<CardPayment>();
        if (method == "UPI") return std::make_shared<UPIPayment>();
        throw std::invalid_argument("Invalid payment method");
    }
};

// Court Inventory
class CourtInventory {
    std::map<std::string, int> courtAvailability;  // { "Grass": 10, "Clay": 5, "Hard": 8 }
public:
    CourtInventory() {
        courtAvailability["Grass"] = 10;
        courtAvailability["Clay"] = 5;
        courtAvailability["Hard"] = 8;
    }

    bool isAvailable(const std::string& courtType) const {
        return courtAvailability.at(courtType) > 0;
    }

    void bookCourt(const std::string& courtType) {
        if (!isAvailable(courtType)) throw std::runtime_error("No courts available for type: " + courtType);
        courtAvailability[courtType]--;
    }
};

// // Booking Class
// class Booking {
//     std::shared_ptr<Court> court;
//     std::vector<std::shared_ptr<RentalItem>> rentalItems;
// public:
//     Booking(std::shared_ptr<Court> court) : court(court) {}

//     void addRentalItem(std::shared_ptr<RentalItem> item) {
//         rentalItems.push_back(item);
//     }

//     int calculateTotal() const {
//         return PricingStrategy::calculateTotal(court, rentalItems);
//     }
// };

// Booking Validator
class BookingValidator {
public:
    static void validateCourtType(const std::string& courtType) {
        if (courtType != "Grass" && courtType != "Clay" && courtType != "Hard") {
            throw std::invalid_argument("Invalid court type");
        }
    }

    static void validatePaymentMethod(const std::string& paymentMethod) {
        if (paymentMethod != "Card" && paymentMethod != "UPI") {
            throw std::invalid_argument("Invalid payment method");
        }
    }
};

// Payment Service
class PaymentService {
public:
    void processPayment(const std::string& paymentMethod, int amount) const {
        auto paymentProcessor = PaymentProcessorFactory::createProcessor(paymentMethod);
        paymentProcessor->collectPaymentDetails();
        paymentProcessor->processPayment(amount);
    }
};

// Booking Service
class BookingService {
public:
    Booking createBooking(const std::string& courtType, const std::vector<std::string>& rentalTypes) {
        auto court = CourtFactory::createCourt(courtType);
        Booking booking(court);
        for (const auto& rentalType : rentalTypes) {
            booking.addRentalItem(RentalItemFactory::createRentalItem(rentalType));
        }
        return booking;
    }
};


// Inventory Service
class InventoryService {
    CourtInventory inventory;
public:
    bool checkAvailability(const std::string& courtType) {
        return inventory.isAvailable(courtType);
    }

    void reserveCourt(const std::string& courtType) {
        inventory.bookCourt(courtType);
    }
};

// Discount Strategy Interface
class DiscountStrategy {
public:
    virtual int applyDiscount(int total) const = 0;
    virtual ~DiscountStrategy() = default;
};

class NoDiscount : public DiscountStrategy {
public:
    int applyDiscount(int total) const override {
        return total;  // No discount
    }
};

class PercentageDiscount : public DiscountStrategy {
    int percentage;
public:
    PercentageDiscount(int percentage) : percentage(percentage) {}
    int applyDiscount(int total) const override {
        return total - (total * percentage / 100);
    }
};

// Updated Booking Class
class Booking {
    std::shared_ptr<Court> court;
    std::vector<std::shared_ptr<RentalItem>> rentalItems;
    std::shared_ptr<DiscountStrategy> discountStrategy;

public:
    Booking(std::shared_ptr<Court> court, std::shared_ptr<DiscountStrategy> discount = std::make_shared<NoDiscount>())
        : court(court), discountStrategy(discount) {}

    void addRentalItem(std::shared_ptr<RentalItem> item) {
        rentalItems.push_back(item);
    }

    int calculateTotal() const {
        int total = PricingStrategy::calculateTotal(court, rentalItems);
        return discountStrategy->applyDiscount(total);
    }
};

class Logger {
public:
    static void logInfo(const std::string& message) {
        std::cout << "[INFO] " << message << "\n";
    }

    static void logError(const std::string& message) {
        std::cerr << "[ERROR] " << message << "\n";
    }
};

// Booking Manager
class BookingManager {
    InventoryService inventoryService;
    BookingService bookingService;
    PaymentService paymentService;

public:
void bookCourt(const std::string& courtType, const std::vector<std::string>& rentalTypes, const std::string& paymentMethod) {
    try {
            Logger::logInfo("Starting booking process.");
            BookingValidator::validateCourtType(courtType);
            BookingValidator::validatePaymentMethod(paymentMethod);
            
            if (!inventoryService.checkAvailability(courtType)) {
                throw std::runtime_error("Selected court type is unavailable");
            }
            
            auto discountStrategy = std::make_shared<PercentageDiscount>(10);  // 10% discount
            Booking booking = bookingService.createBooking(courtType, rentalTypes);
            booking.applyDiscountStrategy(discountStrategy);
            
            int total = booking.calculateTotal();
            
            std::cout << "Total Price after discount: ₹" << total << "\n";
            paymentService.processPayment(paymentMethod, total);
            inventoryService.reserveCourt(courtType);
            
            Logger::logInfo("Booking completed successfully.");
        } catch (const std::exception& e) {
            Logger::logError(e.what());
            throw;
        }
    }    
};

// Main Function
int main() {
    BookingManager manager;

    try {
        manager.bookCourt("Grass", {"Racket", "Racket"}, "UPI");  // Example: Grass court with 2 rackets
        manager.bookCourt("Clay", {"Ballpack", "Grips"}, "Card");  // Example: Clay court with ballpack and grips
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
