#include <algorithm>
#include <cctype>
#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class FleetError : public std::exception {
 public:
  explicit FleetError(const std::string& message) : message_(message) {}

  const char* what() const noexcept override {
    return message_.c_str();
  }

 private:
  std::string message_;
};

class InvalidArguments : public FleetError {
 public:
  using FleetError::FleetError;
};

class InspectionRejected : public FleetError {
 public:
  InspectionRejected() : FleetError("inspection rejected") {}
};

class InventoryItem {
 public:
  virtual ~InventoryItem() = default;
  virtual const std::string& inventoryId() const = 0;
  virtual const std::string& name() const = 0;
  virtual std::string type() const = 0;
  virtual void describe(std::ostream& out) const = 0;

  virtual bool suitableForNovice(int) const {
    return false;
  }
};

std::ostream& operator<<(std::ostream& out, const InventoryItem& item) {
  item.describe(out);
  return out;
}

bool operator<(const InventoryItem& left, const InventoryItem& right) {
  return left.inventoryId() < right.inventoryId();
}

bool operator==(const InventoryItem& left, const InventoryItem& right) {
  return left.inventoryId() == right.inventoryId();
}

class NamedItem : public InventoryItem {
 public:
  NamedItem(const std::string& id, const std::string& name)
      : id_(id), name_(name) {
    if (id.empty() || name.empty()) {
      throw InvalidArguments("id and name must not be empty");
    }
  }

  const std::string& inventoryId() const override {
    return id_;
  }

  const std::string& name() const override {
    return name_;
  }

  void describe(std::ostream& out) const override {
    out << inventoryId() << ' ' << name();
  }

 private:
  std::string id_;
  std::string name_;
};

class EnergyConsumer {
 public:
  virtual ~EnergyConsumer() = default;
  virtual double dailyEnergy() const = 0;
};

class Transport : public NamedItem {
 public:
  Transport(const std::string& id, const std::string& name, int score)
      : NamedItem(id, name), score_(score) {
    if (score < 1 || score > 10) {
      throw InvalidArguments("score must be between 1 and 10");
    }
  }

  bool suitableForNovice(int minimum) const override {
    return score_ >= minimum;
  }

 private:
  int score_;
};

double checkedEnergy(double energy) {
  if (!std::isfinite(energy) || energy < 0.0) {
    throw InvalidArguments("energy must be finite and non-negative");
  }
  return energy;
}

class Bike : public Transport {
 public:
  using Transport::Transport;

  std::string type() const override {
    return "bike";
  }
};

class Scooter : public Transport, public EnergyConsumer {
 public:
  Scooter(const std::string& id, const std::string& name, double energy,
          int score)
      : Transport(id, name, score), energy_(checkedEnergy(energy)) {}

  std::string type() const override {
    return "scooter";
  }

  double dailyEnergy() const override {
    return energy_;
  }

 private:
  double energy_;
};

class EBike : public Transport, public EnergyConsumer {
 public:
  EBike(const std::string& id, const std::string& name, double energy, int score)
      : Transport(id, name, score), energy_(checkedEnergy(energy)) {}

  std::string type() const override {
    return "ebike";
  }

  double dailyEnergy() const override {
    return energy_;
  }

 private:
  double energy_;
};

class Helmet : public NamedItem {
 public:
  using NamedItem::NamedItem;

  std::string type() const override {
    return "helmet";
  }
};

class Dock : public NamedItem, public EnergyConsumer {
 public:
  Dock(const std::string& id, const std::string& name, double energy)
      : NamedItem(id, name), energy_(checkedEnergy(energy)) {}

  std::string type() const override {
    return "dock";
  }

  double dailyEnergy() const override {
    return energy_;
  }

 private:
  double energy_;
};

class ESkate : public Transport, public EnergyConsumer {
 public:
  ESkate(const std::string& id, const std::string& name, double energy, int score)
      : Transport(id, name, score), energy_(checkedEnergy(energy)) {}

  std::string type() const override {
    return "eskate";
  }

  double dailyEnergy() const override {
    return energy_;
  }

 private:
  double energy_;
};

template <typename T>
class Registry {
 public:
  void add(T item) {
    items_.push_back(std::move(item));
  }

  void remove(std::size_t index) {
    items_.erase(items_.begin() + index);
  }

  std::size_t size() const {
    return items_.size();
  }

  const T& operator[](std::size_t index) const {
    return items_.at(index);
  }

  const std::vector<T>& items() const {
    return items_;
  }

 private:
  std::vector<T> items_;
};

using Park = Registry<std::shared_ptr<InventoryItem>>;

template <typename T>
T readArgument(std::istream& input) {
  T value{};
  if (!(input >> value)) {
    throw InvalidArguments("missing or invalid argument");
  }
  return value;
}

void requireEnd(std::istream& input) {
  std::string extra;
  if (input >> extra) {
    throw InvalidArguments("too many arguments");
  }
}

std::unique_ptr<InventoryItem> createItem(std::istream& input) {
  const auto type = readArgument<std::string>(input);
  const auto id = readArgument<std::string>(input);
  const auto name = readArgument<std::string>(input);

  if (type == "scooter" || type == "ebike" || type == "eskate") {
    const double energy = readArgument<double>(input);
    const int score = readArgument<int>(input);
    if (type == "scooter") {
      return std::make_unique<Scooter>(id, name, energy, score);
    }
    if (type == "ebike") {
      return std::make_unique<EBike>(id, name, energy, score);
    }
    return std::make_unique<ESkate>(id, name, energy, score);
  }
  if (type == "bike") {
    const int score = readArgument<int>(input);
    return std::make_unique<Bike>(id, name, score);
  }
  if (type == "helmet") {
    return std::make_unique<Helmet>(id, name);
  }
  if (type == "dock") {
    const double energy = readArgument<double>(input);
    return std::make_unique<Dock>(id, name, energy);
  }
  throw InvalidArguments("unknown item type");
}

void inspect(const InventoryItem& item) {
  if (dynamic_cast<const Transport*>(&item) == nullptr) {
    return;
  }

  std::string name = item.name();
  std::transform(name.begin(), name.end(), name.begin(), [](unsigned char ch) {
    return static_cast<char>(std::tolower(ch));
  });
  if (name.find("broken") != std::string::npos) {
    throw InspectionRejected();
  }
}

void addItem(Park& park, std::unique_ptr<InventoryItem> item) {
  for (const auto& existing : park.items()) {
    if (*existing == *item) {
      throw InvalidArguments("duplicate id");
    }
  }

  inspect(*item);
  park.add(std::shared_ptr<InventoryItem>(std::move(item)));
}

std::size_t findItem(const Park& park, const std::string& id) {
  for (std::size_t i = 0; i < park.size(); ++i) {
    if (park[i]->inventoryId() == id) {
      return i;
    }
  }
  throw InvalidArguments("unknown id");
}

template <typename Container>
double totalEnergy(const Container& items) {
  double total = 0.0;
  for (const auto& item : items) {
    const auto* consumer = dynamic_cast<const EnergyConsumer*>(item.get());
    if (consumer != nullptr) {
      total += consumer->dailyEnergy();
    }
  }
  return total;
}

double diversity(const Park& park) {
  if (park.size() < 2) {
    return 0.0;
  }

  std::map<std::string, std::size_t> counts;
  for (const auto& item : park.items()) {
    ++counts[item->type()];
  }

  double sameTypePairs = 0.0;
  for (const auto& entry : counts) {
    const double count = static_cast<double>(entry.second);
    sameTypePairs += count * (count - 1.0);
  }

  const double total = static_cast<double>(park.size());
  return 100.0 * (1.0 - sameTypePairs / (total * (total - 1.0)));
}

void printList(const Park& park, std::ostream& out) {
  auto sorted = park.items();
  std::sort(sorted.begin(), sorted.end(), [](const auto& left, const auto& right) {
    return *left < *right;
  });
  for (const auto& item : sorted) {
    out << *item << '\n';
  }
}

void printNovices(const Park& park, int minimum, std::ostream& out) {
  for (const auto& item : park.items()) {
    if (item->suitableForNovice(minimum)) {
      out << *item << '\n';
    }
  }
}

bool processCommand(const std::string& line, Park& park,
                    std::weak_ptr<InventoryItem>& focus, std::ostream& out) {
  std::istringstream input(line);
  std::string command;
  if (!(input >> command)) {
    return true;
  }

  if (command == "add") {
    auto item = createItem(input);
    requireEnd(input);
    addItem(park, std::move(item));
  } else if (command == "remove") {
    const auto id = readArgument<std::string>(input);
    requireEnd(input);
    park.remove(findItem(park, id));
  } else if (command == "focus") {
    const auto id = readArgument<std::string>(input);
    requireEnd(input);
    focus = park[findItem(park, id)];
  } else if (command == "show_focus") {
    requireEnd(input);
    const auto item = focus.lock();
    if (item) {
      out << *item << '\n';
    } else {
      out << "expired\n";
    }
  } else if (command == "list") {
    requireEnd(input);
    printList(park, out);
  } else if (command == "novice") {
    requireEnd(input);
    printNovices(park, 6, out);
  } else if (command == "energy") {
    requireEnd(input);
    out << std::fixed << std::setprecision(1) << totalEnergy(park.items()) << '\n';
  } else if (command == "stats") {
    requireEnd(input);
    out << park.size() << ' ' << std::fixed << std::setprecision(1)
        << totalEnergy(park.items()) << '\n';
  } else if (command == "diversity") {
    requireEnd(input);
    out << std::fixed << std::setprecision(1) << diversity(park) << "%\n";
  } else if (command == "quit") {
    requireEnd(input);
    return false;
  } else {
    throw InvalidArguments("unknown command");
  }
  return true;
}

int main() {
  Park park;
  std::weak_ptr<InventoryItem> focus;
  std::string line;

  while (std::getline(std::cin, line)) {
    try {
      if (!processCommand(line, park, focus, std::cout)) {
        break;
      }
    } catch (const std::exception& error) {
      std::cout << "error: " << error.what() << '\n';
    }
  }
  return 0;
}
